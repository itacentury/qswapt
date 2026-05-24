#ifndef STATION_H
#define STATION_H

#include <QString>

struct Station {
  QString name;
  QString id{};

  bool operator<(const Station& other) const {
    if (this->name == other.name) {
      return this->id < other.id;
    }
    return QString::localeAwareCompare(this->name, other.name) < 0;
  }

  [[nodiscard]] QString toString() const {
    return QString("name: %1, id: %2").arg(name).arg(id);
  }
};

#endif