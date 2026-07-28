#pragma once

#include "config.h"
#include <Adafruit_SSD1306.h>
#include "Point2D.h"
#include "Ball.h"

class PlayerBar {
  protected:
    Point2D Pos;
    float width;
    float height;
    Adafruit_SSD1306* display_ptr;

  public:
    PlayerBar(Adafruit_SSD1306* display_ptr);

    void draw();
    void updatePos(float newXPos);
    virtual bool checkCollide (Ball* myBall, Point2D* dir);

    float getHeight();
    float getWidth();
    Point2D getPos();

};
