#include "Ball.h"


Ball::Ball(Adafruit_SSD1306* display_ptr, void(*buzzerStart_ptr_in)(int), void(*stopGame_ptr_in)(void))
  : display_ptr(display_ptr)
{
  buzzerStart_ptr = buzzerStart_ptr_in;
  stopGame_ptr = stopGame_ptr_in;

  restart();
}

void Ball::draw() {
  display_ptr->drawPixel(Pos.x, Pos.y, SSD1306_INVERSE);
}

void Ball::updatePos() {
  Pos.x += Vel.x;
  if (Pos.x >= SCREEN_WIDTH) {// hit screen right
    Vel.x *= -1;
    buzzerStart_ptr(1000);
  } else if (Pos.x <= 0) {// hit screen left
    Vel.x *= -1;
    buzzerStart_ptr(1000);
  }
  Pos.y += Vel.y;
  if (Pos.y >= SCREEN_HEIGHT) {// hit screen bottom
    stopGame_ptr();
    Vel.y *= -1;
    buzzerStart_ptr(500);
  } else if (Pos.y <= 0) {// hit screen top
    Vel.y *= -1;
    buzzerStart_ptr(1000);
  }
}

void Ball::bounce(Point2D newVelocity) {
  this->Vel = newVelocity;
}

Point2D Ball::getPos() { return Pos; }
Point2D Ball::getVel() { return Vel; }

void Ball::restart() {
  Pos.x = 64;
  Pos.y = 50;
  Vel.x = 1;
  Vel.y = -1;
}

void Ball::disable() {
  Pos.x = 64;
  Pos.y = 50;
  Vel.x = 0;
  Vel.y = 0;
}