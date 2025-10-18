//
//  Player.cpp
//  Lab 2 Pong Game
//
//  Created by Harry Zhang on 2025-09-15

#include "Player.h"

#include "Globals.h"

Player::Player() {}

Player::Player(double x, double y, int height) {
  this -> x = x;
  this -> y = y;
  this -> height = height;
}

double Player::getX() { return x; }

double Player::getY() { return y; }

int Player::getHeight() { return height; }

int Player::getWidth() { return 1; }

void Player::decreaseHeight(int delta_to_decrease_by) {
  height -= delta_to_decrease_by;
  if (height < 3) {
    height = 3;
  }  // limit the height of the paddle to 3
}

void Player::update(char c) {
  if (c == 'A') {  // up
    y += 2;
  } else if (c == 'B') {  // down
    y -= 2;
  }
  // boundary cases
  if ((y + height) > HEIGHT - 1) {
    y = HEIGHT - 1 - height;
  }  // celling
  if (y < 0) {
    y = 0;
  }  // floor
}

void Player::draw(Screen& screen_to_draw_to) {
  for (int i = 0; i < height; i++) {
    screen_to_draw_to.addPixel(x, y + i, '#');
  }
}  // repeatedly draw the paddle until it reaches the paddle height
