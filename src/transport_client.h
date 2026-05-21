#ifndef TRANSPORT_CLIENT_H
#define TRANSPORT_CLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>

#include "departure.h"

class TransportClient : public QObject {
  Q_OBJECT

 public:
  explicit TransportClient(QObject* parent = nullptr);

  void getDepartures(const QString& stationId);

 signals:
  void departuresReceived(const QString& stationId,
                          const QList<Departure>& departures);
  void departuresFailed(QNetworkReply::NetworkError error, int httpStatus,
                        const QString& message);
  void departuresInvalid(const QString& message);

 private:
  QNetworkAccessManager* m_manager;
  void handleReply(QNetworkReply* reply, const QString& stationId);
  static QList<Departure> extractDepartures(const QJsonArray& departuresJson);

  const QString kBaseUrl = "https://v6.db.transport.rest";
  static constexpr int kRequestTimeoutMs = 5000;
};

#endif
