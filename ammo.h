#ifndef INSOMNIA_AMMO_H
#define INSOMNIA_AMMO_H
#include "../user/graphic.h"
#include "thing.h"
#include "cell.h"

#define AMMO_RAY	0
#define AMMO_SPARK	1
#define AMMO_ARROW	2
#define AMMO_BOLT	3
#define AMMO_MAX	4

class Ammo : public Thing  {
  public:
  Ammo(int, Cell *, Cell *, int);
  Ammo(int, Thing *, Thing *, int, int, int, int);
  void Create(int, Cell *, Cell *, int);
  virtual ~Ammo();
  virtual void Select()  {}
  virtual void Deselect()  {}
  virtual void ToggleSelect()  {}

  protected:
  virtual void ReScaleme(); 
  virtual void ReAlignme(int, int); 
  virtual void tickme();
  virtual void updateme();
  Graphic pic;
  Sprite image;
  static Graphic *ammog[AMMO_MAX];
  static int grinit;
  static void InitGraphics();
  double xp, yp;
  double xx, yy;
  int gsize;
  Thing *target, *source;
  int skill, pdam, bdam;
  };

#endif
