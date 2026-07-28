#pragma once

#include "config.h"
#include <Adafruit_SSD1306.h>
#include "Point2D.h"

class Ball {
  private:
    Point2D Pos;
    Point2D Vel;
    Adafruit_SSD1306* display_ptr;

    
    void (*buzzerStart_ptr)(int);
    void (*stopGame_ptr)(void);

  public:
    Ball(Adafruit_SSD1306* display_ptr, void(*buzzerStart_ptr)(int), void(*stopGame_ptr)(void));

    void draw();
    void updatePos() ;
    void bounce(Point2D newVelocity);

    Point2D getPos();
    Point2D getVel();

    void restart();
    void disable();

};