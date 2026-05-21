#include <QApplication>
#include <QObject>

#include "mainwindow.h"
#include "transport_client.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  // not needed for now
  // std::optional<QList<Station>> stations = readStations();
  // if (!stations.has_value()) {
  //   return -1;
  // }

  TransportClient transportClient;

  QObject::connect(
      &transportClient, &TransportClient::departuresReceived,
      [](const QString& stationId, const QList<Departure>& departures) {
        qDebug() << "Got" << departures.size() << "departures for station"
                 << stationId;
        for (const auto& departure : departures) {
          qDebug() << departure.toString();
        }
      });
  QObject::connect(&transportClient, &TransportClient::departuresFailed,
                   [](QNetworkReply::NetworkError error, int httpStatus,
                      const QString& message) {
                     qDebug()
                         << "Error code: " << error << "http:" << httpStatus
                         << "body/msg:" << message;
                   });
  QObject::connect(
      &transportClient, &TransportClient::departuresInvalid,
      [](const QString& message) { qDebug() << "Error: " << message; });

  transportClient.getDepartures("8000013");

  return QApplication::exec();
}
