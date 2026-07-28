#include "PlayerBar.h"




PlayerBar::PlayerBar(Adafruit_SSD1306* display_ptr)
 : display_ptr(display_ptr)
{
  int start_pos = 64;
  width = 30;
  height = 4;
  Pos.x = start_pos - width/2;
  Pos.y = 56;
}

void PlayerBar::draw() {
  display_ptr->drawRect(Pos.x, Pos.y, width, height, SSD1306_WHITE);
}

void PlayerBar::updatePos(float newXPos) {
  Pos.x = newXPos;
}

bool PlayerBar::checkCollide (Ball* myBall, Point2D* dir) {
  Point2D ballPos = myBall->getPos();

  if (ballPos.y >= Pos.y-1 && ballPos.y <= Pos.y+height) {
    if (ballPos.x <= Pos.x+width && ballPos.x >= Pos.x-1) {
      auto center_x = Pos.x+width/2;
      auto center_y = Pos.y+height/2;
      auto magnitude = Point2D::calcLineMag(ballPos.x, ballPos.y, center_x, center_y);
      dir->x = ((ballPos.x-center_x)/magnitude);
      dir->y = ((ballPos.y-center_y)/magnitude);
      return true;
    }
  }
  return false;
}

float PlayerBar::getHeight() { return height; }
float PlayerBar::getWidth() { return width; }
Point2D PlayerBar::getPos() { return Pos; }
