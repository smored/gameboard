#pragma once

#include "config.h"
#include <math.h>

class Point2D {
  private:

  public:
    float x;
    float y;

    Point2D();
    Point2D(float x, float y);

    Point2D operator - (const Point2D& other) const;
    Point2D operator + (const Point2D& other) const;
    Point2D operator * (const float& other) const;

    static float calcLineMag(float x1, float y1, float x2, float y2);
    static float calcLineMag(Point2D p1, Point2D p2);

};
