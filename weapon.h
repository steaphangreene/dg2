#ifndef INSOMNIA_WEAPON_H
#define INSOMNIA_WEAPON_H

#include <user/graphic.h>

class Weapon  {
  public:
  Weapon()  {
    range = 1;
    mrange = 0;
    pdam = 0;
    bdam = 10;
    rof = 0;
    delay = 0;
    ammo = -1;
    image[0][0] = NULL;
    image[1][0] = NULL;
    image[0][1] = NULL;
    image[1][1] = NULL;
    image[0][2] = NULL;
    image[1][2] = NULL;
    }

  ~Weapon()  {
    
    }

  Weapon(const Weapon &in)  {
    range = in.range;
    mrange = in.mrange;
    pdam = in.pdam;
    bdam = in.bdam;
    rof = in.rof;
    delay = in.delay;
    ammo = in.ammo;
    image[0][0] = in.image[0][0];
    image[0][1] = in.image[0][1];
    image[0][2] = in.image[0][2];
    image[1][0] = in.image[1][0];
    image[1][1] = in.image[1][1];
    image[1][2] = in.image[1][2];
    };

  Weapon Weapon::operator =(const Weapon &in)  {
    range = in.range;
    mrange = in.mrange;
    pdam = in.pdam;
    bdam = in.bdam;
    rof = in.rof;
    delay = in.delay;
    ammo = in.ammo;
    image[0][0] = in.image[0][0];
    image[0][1] = in.image[0][1];
    image[0][2] = in.image[0][2];
    image[1][0] = in.image[1][0];
    image[1][1] = in.image[1][1];
    image[1][2] = in.image[1][2];
    return *this;
    };

  void SetImage(char *dir)  {
    char buf[128];
    sprintf(buf, "%s/base.bmp%c", dir, 0);
    image[0][2] = new Graphic(buf);
    image[0][2]->FindTrueCenter();
//    image[0][2]->Trim();
    sprintf(buf, "%s/0_1.bmp%c", dir, 0);
    image[1][2] = new Graphic(buf);
    image[1][2]->FindTrueCenter();
    image[1][2]->xcenter = 0;
//    image[1][2]->Trim();
    image[0][1] = new Graphic(image[0][2]->Scaled(0.5));
    image[1][1] = new Graphic(image[1][2]->Scaled(0.5));
    image[0][0] = new Graphic(image[0][2]->Scaled(0.25));
    image[1][0] = new Graphic(image[1][2]->Scaled(0.25));
    };

  char delay;
  char rof;
  char range;
  char mrange;
  char pdam;
  char bdam;
  char ammo;
  Graphic *image[2][3];
  };

#endif
