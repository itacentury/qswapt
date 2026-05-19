#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QString>

#include "station.h"

QList<Station> readStations() {
  QString filename = QString(":/resources/stations.txt");
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::warning(nullptr, "Error",
                         QString("Could not open file: '%1'").arg(filename));
    return {};
  }

  QTextStream in(&file);
  QList<Station> stations;
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList stationAndId = line.split(";");
    if (stationAndId.length() != 2) {
      continue;
    }

    Station station;
    station.name = stationAndId.at(0);
    station.id = stationAndId.at(1);
    stations.append(station);
  }

  file.close();

  std::sort(stations.begin(), stations.end());

  return stations;
}
