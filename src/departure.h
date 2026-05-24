#ifndef DEPARTURE_H
#define DEPARTURE_H

#include <QString>
#include <optional>

struct Departure {
  QString line;
  QString direction;
  QString scheduled;
  std::optional<QString> expected;
  int delay{};

  [[nodiscard]] QString toString() const {
    return QString(
               "line: %1, direction: %2, scheduled: %3, expected: %4, "
               "delay: %5")
        .arg(line)
        .arg(direction)
        .arg(scheduled)
        .arg(expected.value_or("-"))
        .arg(QString::number(delay));
  }
};

#endif