#ifndef STATION_READER_H
#define STATION_READER_H

#include <QList>
#include <optional>

#include "station.h"

std::optional<QList<Station>> readStations();

#endif