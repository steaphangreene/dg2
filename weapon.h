#ifndef INSOMNIA_WEAPON_H
#define INSOMNIA_WEAPON_H

class Weapon  {
  public:
  Weapon()  {
    range = 1;
    pdam = 0;
    bdam = 10;
    }

  char range;
  char pdam;
  char bdam;
  };

#endif
