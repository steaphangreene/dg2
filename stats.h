#ifndef INSOMNIA_STATS_H
#define INSOMNIA_STATS_H

#include "cell.h"
#include "string.h"

class Statistics  {
  public:
  Statistics()  {
    bzero(speed, TERRAIN_MAX);
    walk_frames = 0;
    height = 6;
    weight = 16;
    vision = 8;
    fieldofvision = 2;

    str = 6;
    body = 6;
    qui = 6;
    armor = 0; 

    siege = 0;
    misile = 0;
    melee = 1;
    block = 0;
    shield = 1;
    dodge = 1;
    magic = 0;
    }

  //Game Mechanix
  char speed[TERRAIN_MAX];
  char walk_frames;
  char height;
  char weight;
  char vision;
  char fieldofvision;

  //Combat Stats
  char str;
  char body;
  char qui;
  char armor;

  //Skills
  char siege;
  char melee;
  char misile;
  char block;
  char shield;
  char dodge;
  char magic;
  };

#endif
