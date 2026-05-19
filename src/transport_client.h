#ifndef TRANSPORT_CLIENT_H
#define TRANSPORT_CLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

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

 private:
  QNetworkAccessManager* m_manager;
  void handleReply(QNetworkReply* reply, const QString& stationId);

  static constexpr std::string_view kBaseUrl = "https://v6.db.transport.rest";
  static constexpr int kRequestTimeoutMs = 5000;
};

#endif
