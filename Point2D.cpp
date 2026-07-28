#include "Point2D.h"
#include <math.h>

Point2D::Point2D() {
  this->x = 0;
  this->y = 0;
}

Point2D::Point2D(float x, float y) {
  this->x = x;
  this->y = y;
}

Point2D Point2D::operator - (const Point2D& other) const {
  return {x - other.x, y - other.y};
}

Point2D Point2D::operator + (const Point2D& other) const {
  return {x + other.x, y + other.y};
}

Point2D Point2D::operator * (const float& other) const {
  return {x * other, y * other};
}

float Point2D::calcLineMag(float x1, float y1, float x2, float y2) {
  auto a = x2 - x1;
  auto b = y2 - y1;
  auto c = sqrt(a*a + b*b);

  return c;
}

float Point2D::calcLineMag(Point2D p1, Point2D p2) {
  return calcLineMag(p1.x, p1.y, p2.x, p2.y);
}
