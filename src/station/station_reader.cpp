#include <QDir>
#include <QFile>
#include <QIODevice>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include "station.h"

std::optional<QList<Station>> readStations() {
  QString filename = QString(":/resources/stations.txt");
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
    return std::nullopt;
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

  std::sort(stations.begin(), stations.end());

  return stations;
}
