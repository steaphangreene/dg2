#include <math.h>
#include "ammo.h"
#include "gmode.h"
#include "creature.h"

Graphic *Ammo::ammog[AMMO_MAX];
int Ammo::grinit = 0;

extern Window mainw;
extern Map *curmap;
extern GMode gmode[10];
extern int cmode;

#define gm gmode[cmode]

Ammo::Ammo(int gnum, Cell *orig, Cell *dest, int speed)  {
  Create(gnum, orig, dest, speed);
  }

Ammo::Ammo(int gnum, Thing *att, Thing *targ, int speed,
	int sk, int pd, int bd)  {
  Create(gnum, (Cell *)att->Location(0)->Location(0),
	(Cell *)targ->Location(0)->Location(0),	speed);
  skill = sk; target = targ; source = att; pdam = pd; bdam = bd;
  }

void Ammo::Create(int gnum, Cell *orig, Cell *dest, int speed)  {
  if(!grinit) InitGraphics();
  skill = -1;
  if(speed > 59) speed = 59;
  location[0] = ((Cell *)orig->Location(0)->Location(0));
  location[1] = ((Cell *)dest->Location(0)->Location(0));
  ((Ammo *)location[0])->spell += thingnum;
  int x = (dest->XCoord() - orig->XCoord())*8;
  int y = (orig->YCoord() - dest->YCoord())*15;
  double da = atan2((double)y, (double)x);
  xx = ((double)speed)*cos(da);
  yy = -((double)speed)*sin(da);
  da *= (double)128;
  da /= M_PI;
  int ang = (int)da;
  ang += 1024;
  ang &= 255;
  pic = ammog[gnum]->Rotated(ang);
  pic.Trim();
  image.SetWindow(mainw);
  image.DisableCollisions();
  Changed[thingnum] = 1;
  Waiting[thingnum] = 1;
  yp = (double)0;
  xp = (double)0;
  gsize = 0;
  }

Ammo::~Ammo()  {
  ((Ammo *)location[0])->spell -= thingnum;
  image.Erase();
  }

void Ammo::tickme()  {
  xp += xx;
  yp += yy;
  Waiting[thingnum] = 1;
  Changed[thingnum] = 1;
  Thing *tmpt = location[0];
  if(yp > 30)  {
    yp -= 60.00;    xp -= 32.00;
    location[0] = location[0]->Next(DIR_SE);
    }
  else if(yp < -30)  {
    yp += 60.00;    xp -= 32.00;
    location[0] = location[0]->Next(DIR_NE);
    }
  if(xp > 32)  {
    xp -= 64.00;
    location[0] = location[0]->Next(DIR_E);
    }
  else if(xp < -32)  {
    xp += 64.00;
    location[0] = location[0]->Next(DIR_W);
    }
  if(location[0] != tmpt)  {
    ((Ammo *)tmpt)->spell -= thingnum;
    ((Ammo *)location[0])->spell += thingnum;
    }
  if(location[0] == location[1])  {
    Waiting[thingnum] = 0;
    Changed[thingnum] = 0;
    if(skill >= 0)  {
      ((Creature*)target)->Strike((Creature*)source, skill, pdam, bdam);
      }
    delete this;
    }
  }

void Ammo::updateme()  {
  if(gsize != gm.xstep)  {
    gsize = gm.xstep;
    ReScaleme();
    }
  if(location[0]->InView())
	image.Move(location[0]->XPos()+(int)((xp+0.5)/(64/gm.xstep)),
	location[0]->YPos()+(int)((yp+0.5)/(64/gm.xstep)));
  else image.Erase();
  }

void Ammo::ReScaleme()  {
  double scale = (double)gsize;
  scale /= 64.00;
  image.SetImage(pic.Scaled(scale));
  }

void Ammo::ReAlignme(int x, int y)  {
  }

void Ammo::InitGraphics()  {
  int ctr;
  grinit = 1;

  ammog[AMMO_RAY]	= new Graphic("graphics/spells/ray.bmp");
  ammog[AMMO_SPARK]	= new Graphic("graphics/spells/spark.bmp");
  ammog[AMMO_ARROW]	= new Graphic("graphics/spells/arrow.bmp");
  ammog[AMMO_BOLT]	= new Graphic("graphics/spells/bolt.bmp");

  for(ctr=0; ctr<AMMO_MAX; ctr++)  {
    ammog[ctr]->FindTrueCenter();
    ammog[ctr]->Trim();
    }
  }
