// src/easing.h
#pragma once
#include <cmath>

namespace Easing {
// Simple linear interpolation
inline float linear(float t) {
  return t;
}

// Quadratic easing
inline float easeInQuad(float t) {
  return t * t;
}
inline float easeOutQuad(float t) {
  return t * (2 - t);
}
inline float easeInOutQuad(float t) {
  return t < 0.5 ? 2 * t * t : -1 + (4 - 2 * t) * t;
}

// Cubic easing
inline float easeInCubic(float t) {
  return t * t * t;
}
inline float easeOutCubic(float t) {
  const float f = t - 1;
  return f * f * f + 1;
}
inline float easeInOutCubic(float t) {
  return t < 0.5 ? 4 * t * t * t : (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
}

// Add more as needed (e.g., Bounce, Elastic)
} // namespace Easing
