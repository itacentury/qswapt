#include "transport_client.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QScopedPointer>
#include <QUrl>

#include "departure.h"

TransportClient::TransportClient(QObject* parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)) {
}

void TransportClient::getDepartures(const QString& stationId) {
  QUrl url(QString("%1/stops/%2/departures").arg(kBaseUrl).arg(stationId));
  QNetworkRequest request{url};
  request.setRawHeader("User-Agent", "QSwapt/0.1");
  request.setRawHeader("Accept", "application/json");
  request.setTransferTimeout(kRequestTimeoutMs);

  if (m_reply != nullptr) {
    m_reply->abort();
    delete m_reply;
    m_reply = nullptr;
  }

  m_reply = m_manager->get(request);
  connect(m_reply, &QNetworkReply::finished, this,
          [this, stationId]() { handleReply(m_reply, stationId); });
}

QList<Departure> TransportClient::extractDepartures(
    const QJsonArray& departuresJson) {
  QList<Departure> departures;

  for (const auto& jsonObject : departuresJson) {
    const QJsonObject obj = jsonObject.toObject();
    const QJsonValue lineValue = obj.value("line");
    const QJsonValue directionValue = obj.value("direction");
    const QJsonValue plannedWhenValue = obj.value("plannedWhen");
    const QJsonValue whenValue = obj.value("when");
    const QJsonValue delayValue = obj.value("delay");

    if (!lineValue.isObject() || !directionValue.isString() ||
        !plannedWhenValue.isString()) {
      continue;
    }

    const QJsonValue nameValue = lineValue.toObject().value("name");

    if (!nameValue.isString()) {
      continue;
    }

    Departure departure;
    departure.line = nameValue.toString();
    departure.direction = directionValue.toString();
    departure.scheduled = plannedWhenValue.toString();
    departure.delay =
        delayValue.isNull() ? 0 : qRound(delayValue.toDouble() / 60.0);
    if (!whenValue.isNull()) {
      departure.expected = whenValue.toString();
    }

    departures.append(departure);
  }

  return departures;
}

void TransportClient::handleReply(QNetworkReply* reply,
                                  const QString& stationId) {
  QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> scopedReply(reply);

  if (reply->error() != QNetworkReply::NoError) {
    const int httpStatus =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    emit departuresFailed(reply->error(), httpStatus,
                          QString::fromUtf8(reply->readAll()));

    return;
  }

  const QByteArray content = reply->readAll();
  auto jsonContent = QJsonDocument::fromJson(content);
  if (jsonContent.isNull()) {
    emit departuresInvalid(QString("JSON parsing failed for station '%1': "
                                   "response was empty or malformed. "
                                   "Raw content: %2")
                               .arg(stationId)
                               .arg(QString::fromUtf8(content.left(200))));
    return;
  }

  if (!jsonContent.isObject()) {
    emit departuresInvalid(
        QString("Unexpected JSON structure for station '%1': "
                "expected object, got %2")
            .arg(stationId)
            .arg(jsonContent.isArray() ? "array" : "unknown type"));
    return;
  }

  const QJsonObject rootObject = jsonContent.object();
  const QJsonValue departuresValue = rootObject.value("departures");
  if (!departuresValue.isArray()) {
    emit departuresInvalid(
        QString("Missing 'departures' array in response for station '%1'. "
                "Available keys: [%2]")
            .arg(stationId)
            .arg(QStringList(rootObject.keys()).join(", ")));
    return;
  }

  const QJsonArray departuresArray = departuresValue.toArray();
  const QList<Departure> departures = extractDepartures(departuresArray);

  emit departuresReceived(stationId, departures);
}
