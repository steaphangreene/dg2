#ifndef INSOMNIA_GAME_H
#define INSOMNIA_GAME_H
#include "player.h"

class Game  {
  public:
  Game(char *, int, int);
  void Play(int);
  ~Game();
  void SlowDown();
  void SpeedUp();
  void SetSpeed();

  private:
  int SPEED;
  int SPEED_MAX;
  int SPEED_MIN;
  int SPEED_STEP;
  int PACKET_DELAY;
  void Menus();
  Graphic *backplate;
  int MainMenu();
  int EnvironMenu();
  Player **players;
  int num_players;
  char quit;
  friend class Player;
  };

#endif
