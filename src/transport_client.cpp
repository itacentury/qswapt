#include "transport_client.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScopedPointer>
#include <QUrl>
#include <QVariant>

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

  auto* reply = m_manager->get(request);
  connect(reply, &QNetworkReply::finished, this,
          [this, reply, stationId]() { handleReply(reply, stationId); });
}

void TransportClient::handleReply(QNetworkReply* reply,
                                  const QString& stationId) {
  QScopedPointer<QNetworkReply, QScopedPointerDeleteLater> scopedReply(reply);

  if (reply->error() != QNetworkReply::NoError) {
    const int httpStatus =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Reply error:" << reply->error() << "http:" << httpStatus
             << "errorString:" << reply->errorString();
    emit departuresFailed(reply->error(), httpStatus,
                          QString::fromUtf8(reply->readAll()));

    return;
  }

  const QByteArray content = reply->readAll();
  auto jsonContent = QJsonDocument::fromJson(content);
  if (jsonContent.isNull()) {
    emit departuresInvalid("Received JSON content is null");
    return;
  }

  if (!jsonContent.isObject()) {
    emit departuresInvalid("Received JSON content is not an object");
    return;
  }

  const QJsonObject rootObject = jsonContent.object();
  const QJsonArray departuresJson = rootObject.value("departures").toArray();
  QList<Departure> departures;

  for (const auto& jsonObject : departuresJson) {
    const QJsonObject obj = jsonObject.toObject();
    Departure departure;
    departure.line = obj.value("line").toObject().value("name").toString();
    departure.direction = obj.value("direction").toString();
    departure.scheduled = obj.value("plannedWhen").toString();
    departure.expected = obj.value("when").toString();
    departure.delay =
        QDateTime::fromString(departure.scheduled, Qt::DateFormat::ISODate)
            .secsTo(QDateTime::fromString(departure.expected,
                                          Qt::DateFormat::ISODate)) /
        60;

    departures.append(departure);
  }

  emit departuresReceived(stationId, departures);
}
