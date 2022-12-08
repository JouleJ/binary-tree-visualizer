#pragma once

#include <QColor>

namespace common {
inline auto GOLD = QColor(255, 215, 0);
inline auto ORANGE = QColor(255, 140, 0);

inline auto defaultTextColor = Qt::black;
inline auto defaultMetaColor = Qt::red;

inline size_t defaultAnimationDelay = 1000;
inline size_t defaultSizeUnit = 16;
} // namespace common
