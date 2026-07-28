#pragma once

#include "config.h"
#include "Point2D.h"
#include "Ball.h"
#include "PlayerBar.h"

class Brick : public PlayerBar {
  private:
    int health;
    int id;

  public:
    
    Brick(int n, Adafruit_SSD1306* display_ptr);

    bool checkCollide (Ball* myBall, Point2D* dir) override;

    void takeDmg();
    bool isDead();
    void restart();
    void setID(int id);

};