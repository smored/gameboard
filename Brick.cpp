#include "Brick.h"

Brick::Brick(int n, Adafruit_SSD1306* display_ptr)
  : PlayerBar(display_ptr)
{
  setID(n);

  width = SCREEN_WIDTH/NUM_BRICKS_H;
  height = (SCREEN_HEIGHT/2)/NUM_BRICKS_V;

  Pos.x = width*(n%NUM_BRICKS_H);
  Pos.y = height*(n/NUM_BRICKS_H);

  health = 1;
}

bool Brick::checkCollide (Ball* myBall, Point2D* dir) {
  Point2D ballPos = myBall->getPos();

  if (ballPos.y >= Pos.y-1 && ballPos.y  <= Pos.y+height) {
    if (ballPos.x <= Pos.x+width && ballPos.x >= Pos.x-1) {
      float center_x = Pos.x + width/2;
      float center_y = Pos.y + height/2;
      float diff_x = ballPos.x - center_x;
      float diff_y = ballPos.y - center_y;

      Point2D rect_r; rect_r.x = Pos.x + width; rect_r.y = Pos.y + height/2;
      Point2D rect_l; rect_l.x = Pos.x; rect_r.y = Pos.y + height/2;
      Point2D rect_u; rect_u.x = Pos.x + width/2; rect_r.y = Pos.y;
      Point2D rect_d; rect_d.x = Pos.x + width/2; rect_r.y = Pos.y + height;

      float d_r = Point2D::calcLineMag(ballPos, rect_r);
      float d_l = Point2D::calcLineMag(ballPos, rect_l);
      float d_u = Point2D::calcLineMag(ballPos, rect_u);
      float d_d = Point2D::calcLineMag(ballPos, rect_d);

      dir->x = 1;
      dir->y = 1;
      
      // go back a frame and check where the ball was before it collided
      Point2D lastBallPos = ballPos - myBall->getVel();
      // if it was between the top and bottom of the cube, we know it must be on the sides,
      if (lastBallPos.y >= Pos.y-1 && lastBallPos.y  <= Pos.y+height) {
        dir->x = -1;
      } else if(lastBallPos.x <= Pos.x+width && lastBallPos.x >= Pos.x-1) { // and if its between the left and right it must've been above or below
        dir->y = -1;
      }
      return true;
    }
  }
  return false;
}


void Brick::takeDmg() { health--; }
bool Brick::isDead() { return health<=0; }
void Brick::restart() { health = 1; }
void Brick::setID(int id) { this->id = id; }
