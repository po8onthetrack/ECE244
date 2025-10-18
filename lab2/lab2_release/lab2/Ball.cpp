//
//  Ball.cpp
//  Lab 2 Pong Game
//
//  Created by Created by Harry Zhang on 2025-09-17

#include "Ball.h"

#include "Globals.h"

Ball::Ball() {}

Ball::Ball(double x, double y, double velocity_x, double velocity_y, int id) {
  this -> x = x;
  this -> y = y;
  this -> velocity_x = velocity_x;
  this -> velocity_y = velocity_y;
  this -> id = id;
}

void Ball::update() {
  x += velocity_x * timeStep;
  velocity_y = velocity_y - 0.02 * timeStep;
  y += velocity_y * timeStep;
}

int Ball::getID() { return id; }

double Ball::getX() { return x; }
int Ball::overlap(Ball& b) { return NO_OVERLAP; }  // temporary

int Ball::overlap(Player& p) {
  int pw = p.getX() + p.getWidth();
  int ph = p.getY() + p.getHeight();
  // check for overlap
  if (x <= pw && y <= ph && y >= p.getY() - 1) {
    // check for horizontal overlap
    if (y < ph && y > ph - 1) {  // collides from the top right corner
      if ((pw - x) < (ph - y)) {
        return VERTICAL_OVERLAP;
      } else
        return HORIZONTAL_OVERLAP;
    } else if (y < p.getY() &&
               y > p.getY() - 1) {  // collides from the bottom right corner
      if ((pw - x) < (y + 1 - p.getY())) {
        return VERTICAL_OVERLAP;
      } else {
        return HORIZONTAL_OVERLAP;
      }
    } else {
      return VERTICAL_OVERLAP;
    }
  }
  return NO_OVERLAP;
}

void Ball::bounce(Ball arr[], int ballCount, Player player) {
  if (x + 1 >= WIDTH) {  // collides with right wall
    velocity_x = -velocity_x;
  }
  if (y <= 0) {  // collides with floor
    velocity_y = -velocity_y;
  }
  int overlapType = overlap(player);
  if (overlapType == VERTICAL_OVERLAP) {
    velocity_x = -velocity_x;
  } else if (overlapType == HORIZONTAL_OVERLAP) {
    velocity_y = -velocity_y;
  }
}

void Ball::draw(Screen& screen_to_draw_to) {
  screen_to_draw_to.addPixel(x, y, 'o');
}
