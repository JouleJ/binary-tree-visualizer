#pragma once

#include <QColor>

namespace common {
inline const auto GOLD = QColor(255, 215, 0);
inline const auto ORANGE = QColor(255, 140, 0);

inline const auto defaultTextColor = Qt::black;
inline const auto defaultMetaColor = Qt::red;

inline constexpr size_t defaultAnimationDelay = 1000;
inline constexpr size_t defaultSizeUnit = 16;
} // namespace common
