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

  void getDepartures(int stationId);

 signals:
  void departuresReceived(int stationId, const QList<Departure>& departures);
  void departuresFailed(QNetworkReply::NetworkError error, int httpStatus,
                        const QString& message);

 private:
  QNetworkAccessManager* m_manager;
  void handleReply(QNetworkReply* reply, int stationId);
};

#endif
