#include <string.h>
#include <user.h>

#include "dg2.h"
#include "creature.h"
#include "gmode.h"
#include "map.h"
#include "spell.h"
#include "player.h"
#include "struct.h"
#include "ammo.h"
#include "game.h"

#define gm gmode[cmode]
#define EXPLORE_FOR	4

//Uncomment to disable debug info for just this file
//#undef Debug(x)
//#define Debug(x) {}

extern long long ticker;
extern char sprng[64];

extern Player *p1;
extern Screen *screen;
extern Panel mainp;
extern GMode gmode[10];
extern char cmode;
extern Map *curmap;
extern Sound *Punch;
extern Sound *Death;
extern char multi;
extern Game *curgame;

Graphic *Creature::pics[3][CREATURE_MAX][FRAME_MAX];
Graphic *Creature::selboxg[3] = {NULL, NULL, NULL};
int Creature::GraphicsInitialized = 0;
Statistics *Creature::cstats[CREATURE_MAX] = {NULL};

color Creature::trc1d[CREATURE_MAX];
color Creature::trc1l[CREATURE_MAX];
color Creature::trc2d[CREATURE_MAX];   
color Creature::trc2l[CREATURE_MAX];
color Creature::rrcd[CREATURE_MAX];
color Creature::rrcl[CREATURE_MAX];
color Creature::tcol[TCOL_MAX][2];
color Creature::rcol[RANK_MAX][2];
unsigned char **Creature::cremap[CREATURE_MAX][RANK_MAX];

Sound *Creature::ToldYou = NULL;

Sound *Creature::GotYou = NULL;

Creature::~Creature()  {
  int ctr;
  UnclaimSprite(image.Number());
  for(ctr=0; ctr<spell.Size(); ctr++)  {
    if(list[spell[ctr]] != NULL)  {
      delete ((Spell *)list[spell[ctr]]);
      ctr--;
      }
    }
  spell.Clear();
  for(ctr=0; ctr<sustained_spell.Size(); ctr++)  {
    if(list[sustained_spell[ctr]] != NULL)  {
      ((Spell *)list[sustained_spell[ctr]])->caster = NULL;
      delete ((Spell *)list[sustained_spell[ctr]]);
      ctr--;
      }
    }
  sustained_spell.Clear();
  if(Location(1) != NULL)  {
    Location(0)->LeaveHalf(this);
    Location(1)->LeaveHalf(this);
    UndoVision();
    }
  else  if(Location(0) != NULL)  {
    if(goal[0] != NULL && goal[0]->Goal() == ACTION_EXCUSED)
	Location(0)->LeaveHalf(this);
    else  Location(0)->Leave(this);
    UndoVision();
    }
  if(dirt >= 0)  {
    if(dirt % 2)  {
      if(Location(1) != NULL)  {
        Location(0)->Next(dirt+1)->UnClaim(this);
        }
      else  {
        Location(0)->Next(dirt)->UnClaim(this);
        Location(0)->Next(dirt+1)->UnClaimHalf(this);
        Location(0)->Next(dirt+11)->UnClaimHalf(this);
        }
      }
    else  {
      Location(0)->Next(dirt)->UnClaim(this);
      }
    }
  Deselect();
  image.Erase();
  Remove();
  if(stats != cstats[gtype])  delete stats;
  exists = 0;
  }

void Creature::Init()  {
  if(cstats[gtype] == NULL)  {
    cstats[gtype] = new Statistics;
    cstats[gtype]->speed[TERRAIN_VOID] = 0;
    cstats[gtype]->speed[TERRAIN_PATH] = 6;
    cstats[gtype]->speed[TERRAIN_DIRT] = 6;
    cstats[gtype]->speed[TERRAIN_GRASS] = 6;
    cstats[gtype]->speed[TERRAIN_BRUSH] = 8;
    cstats[gtype]->speed[TERRAIN_FOREST] = 10;
    cstats[gtype]->speed[TERRAIN_LOWWATER] = 12;
    cstats[gtype]->speed[TERRAIN_WATER] = 18;
    cstats[gtype]->speed[TERRAIN_OCEAN] = 24;
    cstats[gtype]->speed[TERRAIN_ROCK] = 10;
    cstats[gtype]->speed[TERRAIN_SAND] = 8;
    cstats[gtype]->speed[TERRAIN_ASHES] = 8;
    }
  stats = cstats[gtype];
  spells = 0;
  prayers = 0;
  mat_type = MATERIAL_NONE;
  mat_ammt = 0;
  cw=-1;
  hit = 100;
  fatigue = 100000;
  vision = stats->vision;
  mvision = 0;
  height = stats->height;
  speedmul = 1;
  frame = FRAME_STAND;
  xpos = 0;
  ypos = 0;
  attacking = 0;
  crawling = 0;
  limping = 0;
  down = 0;
  dirt = -1;
  dirf = -1;
  distt = -1;
  progress = 0;
  target = 0;
  weap[1].range = -1;
  location[0] = NULL;
  location[1] = NULL;
  exists = 0;
  selected = 0;
  if(selboxg[0] == NULL)  {
    selboxg[0] = new Graphic("graphics/select/selb16.bmp");
    selboxg[1] = new Graphic("graphics/select/selb32.bmp");
    selboxg[2] = new Graphic("graphics/select/selb64.bmp");
    }
  selboxg[0]->Trim();
  selboxg[0]->FindTrueCenter();
  selboxg[1]->Trim();
  selboxg[1]->FindTrueCenter();
  selboxg[2]->Trim();
  selboxg[2]->FindTrueCenter();
  screen->MakeFriendly(selboxg[0]);
  screen->MakeFriendly(selboxg[1]);
  screen->MakeFriendly(selboxg[2]);
  ClaimSprite(image.Number());
  Waiting[thingnum] = 1;
  int ctr;
  for(ctr=0; ctr<12; ctr++)  {
    goal[ctr] = NULL;
    mission[ctr] = NULL;
    }
  }

void Creature::SetCreatureGraphic(int ctype, char *dir)  {
  Debug("Creature::SetCreatureGraphic()  Begin");
  if(GraphicsInitialized)  return;
  GraphicsInitialized = 1;
  int ctr, ctr2, ctr3, strst = strlen(dir);
  char *buf = new char[strst+20];
  strcpy(buf, dir);
  strcpy(&buf[strst], ".crf");

  Debug("Creature::SetCreatureGraphic()  Getting Team Colors");
  Graphic tcols("graphics/allteams.bmp");
  for(ctr=0; ctr<TCOL_MAX; ++ctr) {
    tcol[ctr][0] = tcols.image[ctr].uc[0];
    tcol[ctr][1] = tcols.image[ctr].uc[1];
    }

  Debug("Creature::SetCreatureGraphic()  Getting Rank Colors");
  Graphic rcols("graphics/ranks.bmp");
  for(ctr=0; ctr<RANK_MAX; ++ctr) {
    rcol[ctr][0] = rcols.image[ctr].uc[0];
    rcol[ctr][1] = rcols.image[ctr].uc[1];
    }

  Debug("Creature::SetCreatureGraphic()  Before Resfile");
  ResFile crf(buf);

  Debug("Creature::SetCreatureGraphic()  Within Resfile");
  CharBag *ver = crf.GetCharBag();
  Graphic *trc = crf.GetGraphic();
  trc1d[ctype] = trc->image[0].uc[0];
  trc1l[ctype] = trc->image[0].uc[1];
  trc2d[ctype] = trc->image[1].uc[0];
  trc2l[ctype] = trc->image[1].uc[1];
  rrcd[ctype] = trc->image[2].uc[0];
  rrcl[ctype] = trc->image[2].uc[1];
  if((*ver)[0] != VER1 || (*ver)[1] != VER2
              || (*ver)[2] != VER3 || (*ver)[3] != VER4)  {
    if((*ver)[3] == 0)  {
      U2_Exit(1, "Incompatible version, \"%s\" was made by CRE v%d.%d.%d!\n",
              buf, (*ver)[0], (*ver)[1], (*ver)[2]);
      }
    else  {
      U2_Exit(1, "Incompatible version, \"%s\" was made by CRE v%d.%d.%d-B%d!\n",
              buf, (*ver)[0], (*ver)[1], (*ver)[2], (*ver)[3]);
      }
    }
  delete ver;

  for(ctr=0; ctr<FRAME_MAX; ctr++)  {
    pics[2][ctype][ctr] = crf.GetGraphic();
    pics[1][ctype][ctr] = crf.GetGraphic();
    pics[0][ctype][ctr] = crf.GetGraphic();
    ShortBag *tmpb = crf.GetShortBag();
    if(pics[2][ctype][ctr]) screen->MakeFriendly(pics[2][ctype][ctr]);
    if(pics[1][ctype][ctr]) screen->MakeFriendly(pics[1][ctype][ctr]);
    if(pics[0][ctype][ctr]) screen->MakeFriendly(pics[0][ctype][ctr]);
    delete tmpb;
    }

  for(ctr=0; ctr<RANK_MAX; ++ctr) {
    cremap[ctype][ctr] = new unsigned char*[curgame->NumPlayers()];
    for(ctr2=0; ctr2<curgame->NumPlayers(); ++ctr2) {
      cremap[ctype][ctr][ctr2] = new unsigned char[256];
      for(ctr3=0; ctr3<256; ++ctr3) {
	cremap[ctype][ctr][ctr2][ctr3] = ctr3;
	}
      cremap[ctype][ctr][ctr2][trc1d[ctype]] =
		tcol[curgame->players[ctr2]->TeamColor1()][0];
      cremap[ctype][ctr][ctr2][trc1l[ctype]] =
		tcol[curgame->players[ctr2]->TeamColor1()][1];
      cremap[ctype][ctr][ctr2][trc2d[ctype]] =
		tcol[curgame->players[ctr2]->TeamColor2()][0];
      cremap[ctype][ctr][ctr2][trc2l[ctype]] =
		tcol[curgame->players[ctr2]->TeamColor2()][1];
      cremap[ctype][ctr][ctr2][rrcd[ctype]] = rcol[ctr][0];
      cremap[ctype][ctr][ctr2][rrcl[ctype]] = rcol[ctr][1];
      }
    }

  Debug("Creature::SetCreatureGraphic()  End");
  }

Creature::Creature(int ctype, Player *pown)  {
//  if(GotYou == NULL)  GotYou = new Sound("sounds/What.wav");
//  if(ToldYou == NULL)  ToldYou = new Sound("sounds/Ok.wav");
  inside = new (Thing*)[2];
  inside[0] = NULL;
  inside[1] = NULL;
  type = THING_CREATURE;
  gtype = ctype;
  owner = pown;
  image.SetPanel(mainp);
  UpdateCMap();
  selbox.SetPanel(mainp);
  selbox.DisableCollisions();
  weaps.SetPanel(mainp);
  weaps.DisableCollisions();
  Init();
  }

void Creature::Place(Cell *in)  {
  Place(in, 0);
  }

void Creature::Place(Cell *in, int fac)  {
  if((!in->Claim(this, altitude, Height()))
	|| (!in->Enter(this, altitude, Height(), 1, 2))) {
    delete screen;
    printf("Invalid Creature Placement\n");
    exit(1);
    }
  Claimed(in);
  in->UnClaim(this);
  location[0] = in;
  xpos = 0;
  ypos = 0;
  exists = 1;
  facing = fac;
  DoVision();
  Changed[thingnum] = 1;
  Waiting[thingnum] = 1;
  }

void Creature::ReAlignme(int x, int y)  {
  //Changed[thingnum] = 1;
  }

void Creature::ReScaleme()  {
  Changed[thingnum] = 1;
  }

void Creature::Select()  {
  if(owner != p1)  return;
  p1->AddToSelectList(this);
  Changed[thingnum] = 1;
  selected=1;
  }

void Creature::Deselect()  {
  if(owner != p1 || (!selected))  return;
  selected = 0;
  p1->RemoveFromSelectList(this);
  Changed[thingnum] = 1;
  }

void Creature::ToggleSelect()  {
  if(owner != p1)  return;
  selected=1-selected;
  Changed[thingnum] = 1;
  if(selected)  {
    p1->AddToSelectList(this);
    }
  else  {
    p1->RemoveFromSelectList(this);
    }
  }

void Creature::tickme()  {
  if(exists)  Act();
  }

void Creature::UpdateCondition()  {
  if(fatigue <= -50000 && IsInWater())  {
    Damage(10);
    }
  if(hit <= 10 || (inside[0] != NULL
	&& ((Creature*)inside[0])->altitude - altitude <= stats->height)
	|| (inside[1] != NULL
	&& ((Creature*)inside[1])->altitude - altitude <= stats->height)
	|| fatigue <= 0)  {
    if(!crawling)  {
      crawling = 1;
      limping = 1;
      SetSpeedMul(4);
//	Do the FRAME_DOWN, FRAME_CRAWL, FRAME_LIMP, FRAME_SPLAT stuff.
      if(frame == FRAME_STAND || frame == FRAME_SPLAT)  frame = FRAME_DOWN;
      height = 1;
      if(inside[0] != NULL)  inside[0]->Fall();
      Changed[thingnum] = 1;
      int tmp = vision;
      vision = 2;
      DoVision();
      vision = tmp;
      UndoVision();
      vision = 2;
      if(hit <= 10)  {
	Deselect();
	if(dirf>=0) dirf=-1;
	if(dirt>=0 && distt<1) dirt=-1;
//	Action *tmpa = new Action(ACTION_FLEE, SafestDirection());
//	Do(tmpa);
	}
      }
    if(hit <= 0 || fatigue <= -50000)  {
      if(goal[0] != NULL && goal[0]->Goal() != ACTION_EXCUSED)
	GoalAbort();
      if(!down) Deselect();
      down = 1;
      }
    else down = 0;
    }
  else if(hit <= 50 || fatigue <= 50000)  {
    if((!limping) || crawling || down)  {
      height = stats->height;
      limping = 1;
      SetSpeedMul(2);
//	Do the FRAME_DOWN, FRAME_CRAWL, FRAME_LIMP, FRAME_SPLAT stuff.
      if(frame == FRAME_DOWN || frame == FRAME_SPLAT)  frame = FRAME_STAND;
      Changed[thingnum] = 1;
      if(crawling || down)  {
	int tmp = vision;
	vision = stats->vision;
	DoVision();
	if(!down)  {
	  vision = tmp;
	  UndoVision();
	  vision = stats->vision;
	  }
	}
      crawling = 0;
      down = 0;
      }
    }
  else  {
    if(crawling || limping || down)  {
      height = stats->height;
      SetSpeedMul(1);
//	Do the FRAME_DOWN, FRAME_CRAWL, FRAME_LIMP, FRAME_SPLAT stuff.
      if(frame == FRAME_DOWN || frame == FRAME_SPLAT)  frame = FRAME_STAND;
      Changed[thingnum] = 1;
      if(crawling || down)  {
	int tmp = vision;
	vision = stats->vision;
	DoVision();
	if(!down)  {
	  vision = tmp;
	  UndoVision();
	  vision = stats->vision;
	  }
	}
      crawling = 0;
      limping = 0;
      down = 0;
      }
    }
  }

void Creature::Act()  {
  if(!exists)  return;
  Debug("Creature::Act() Begin");
  Waiting[thingnum] = 1;
  UpdateCondition();
  Think();
  if(hit==0)  { //not down
    if((ticker & 255) == (thingnum & 255)) DamageFull(1);
    Debug("Creature::Act() Return 1");
    return;
    }
  else if(hit<0 && hit >= -125)  { //not down
    if((ticker & 15) == (thingnum & 15)) DamageFull(1);
    Debug("Creature::Act() Return 2");
    return;
    }
  else if(hit < -125)  {
//    printf("Dying here!\r\n");
    delete this;
//    printf("Died here!\r\n");
    Debug("Creature::Act() Return 3");
    return;
    }

//  if((inside[0] != NULL && ((Creature *)inside[0])->altitude <= Height())
//      || (inside[1] != NULL && ((Creature *)inside[1])->altitude <= Height()))
//    Debug("Creature::Act() Return 4");
//    return;
  if(exists && target != 0)  {
    { int tf = fatigue/50000;
      if(IsInWater())  fatigue -= 100;
      else  fatigue -= 50;
      if(fatigue<-100000) fatigue = -100000;
      if(tf != (fatigue/50000)) UpdateCondition();
      }
    if(list[target] == NULL)  {
      target = thingnum;
      }
    if(((frame & 4080) == FRAME_ATT || (frame & 4080) == FRAME_ATTP)
	&& (target == thingnum || target == 0))  {
      ResetFrame();
      Debug("Creature::Act() Return 5");
      return;
      }
    if((frame & 4080) == FRAME_ATT || (frame & 4080) == FRAME_ATTP)  {
      if(weap[1].range > 0 && InWeaponRange((Creature *)list[target], 1))
	cw=1;
      else if(weap[0].range>0 && InWeaponRange((Creature *)list[target], 0))
	cw=0;
      else { target = 0; Debug("Creature::Act() Return 6"); return; }
      }
//    if(frame == FRAME_WALK || frame == FRAME_LIMP) frame=FRAME_M0_1;
    if(frame == FRAME_STAND) frame=FRAME_ATTP;
    else if((frame & 4080) == FRAME_ATTP) {
      ++frame;
      if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	frame = FRAME_ATT;
	Attack((Creature *)list[target], cw);
	if(goal[0] == NULL)  {
	  }
	else if(target == 0 || list[target] == NULL)  {
	  GoalDoneFirst();
	  EvaluateGoal();
	  }
	else if((((Creature *)list[target])->hit <= 10)
	    && (goal[0]->Goal() == ACTION_ATTACK)
	    && (goal[0]->Object() == target)) {
	  GoalDoneFirst();
	  EvaluateGoal();
	  }
	else if((((Creature *)list[target])->hit <= 0)
	    && (goal[0]->Goal() == ACTION_KILL)
	    && (goal[0]->Object() == target)) {
	  GoalDoneFirst();
	  EvaluateGoal();
	  }
	}
      }
    else if((frame & 4080) == FRAME_ATT)  {
      frame++;
      if(pics[gm.xstep>>5][gtype][frame] == NULL)
	{ ResetFrame(); target = 0; }
      }
    else { ResetFrame(); target = 0; }
    Changed[thingnum] = 1;
    Waiting[thingnum] = 4;
    }

  else  if(exists && (dirt >= 0))  {
    dirt %= 12;
    if(Location(1) == NULL && !GetPath(dirt))  {
      dirt = -1;
      if((frame%4) == 2)  {
	frame--;
	Changed[thingnum] = 1;
	}
//      else  if((frame != FRAME_WALK) && (frame != FRAME_LIMP) && 
//		(frame != FRAME_CRAWL))  {
      else  if(frame != FRAME_STAND)  {
	ResetFrame();
	Changed[thingnum] = 1;
	}
      }
    else  {
      Changed[thingnum] = 1;
      if((distt < 0) && (facing == dirt))  {
//	Location(0)->UnClaim(this);	// HERE!
	location[1] = NULL;
	distt = 0;
	}
      if(distt > -1)  {
	{ int tf = fatigue/50000;
	  if(IsInWater())  fatigue -= 20;
	  else  fatigue -= 10;
	  if(fatigue<-100000) fatigue = -100000;
	  if(tf != (fatigue/50000)) UpdateCondition();
	  }
	int steps = speed;
	distt++;
//	int trv = (frame & 15);
	if((frame & 248) == FRAME_WALKL)  {
	  if(progress == 0)  {
	    ++frame;
	    if(pics[gm.xstep>>5][gtype][frame] == NULL)
	      { frame-=2; progress=1; }
	    }
	  else  {
	    --frame;
	    if(frame < FRAME_WALKL)
	      { frame=FRAME_WALKR; progress=0; }
	    }
	  }
	else if((frame & 248) == FRAME_WALKR)  {
	  if(progress == 0)  {
	    ++frame;
	    if(pics[gm.xstep>>5][gtype][frame] == NULL)
	      { frame-=2; progress=1; }
	    }
	  else  {
	    --frame;
	    if(frame < FRAME_WALKR)
	    if(pics[gm.xstep>>5][gtype][frame] == NULL)
	      { frame=FRAME_WALKL; progress=0; }
	    }
	  }
	else  { progress = 0; frame=FRAME_WALKL + 1; }
	if(dirt % 2)  {
	  steps *= 7;
	  steps /= 4;
	  if(distt == (steps/2))  {
	    char room;
	    room = Location(0)->Next(dirt+1)->EnterHalf(this, dirt+9);
	    if(room)
	      room = Location(0)->Next(dirt+11)->EnterHalf(this, dirt+3);
	    if(room)  {
	      Cell *tmp1 = location[0], *tmp2 = location[1];
	      location[0]->Leave(this);
	      location[1] = Location(0)->Next(dirt+1);
	      location[0] = Location(0)->Next(dirt+11);
	      DoVision();
	      location[0] = tmp1;
	      location[1] = tmp2;
	      UndoVision();
	      location[1] = Location(0)->Next(dirt+1);
	      location[0] = Location(0)->Next(dirt+11);
//	      printf("finishing halves....\r\n");
	      Location(0)->UnClaimHalf(this);
	      Location(1)->UnClaimHalf(this);
	      }
	    else  distt--;
	    }
	  }
	if(distt >= steps)  {
	  if(Location(0)->Next(((dirt+1)/2)*2)->
		CanEnter(this, altitude, Height(), 1, 2))  {
	    if(Location(1) != NULL)  {
	      Location(0)->LeaveHalf(this);
	      Location(1)->LeaveHalf(this);
	      }
	    else  {
	      Location(0)->Leave(this);
	      }
	    if(!Location(0)->Next(((dirt+1)/2)*2)->
		Enter(this, altitude, Height(), 1, 2))
			U2_Exit(1, "Bad guarenteed Enter!\r\n");
	    Cell *tmp1 = location[0], *tmp2 = location[1];
	    location[0] = Location(0)->Next(((dirt+1)/2)*2);
	    location[1] = NULL;
	    DoVision();
	    location[0] = tmp1;
	    location[1] = tmp2;
	    UndoVision();
	    location[0] = Location(0)->Next(((dirt+1)/2)*2);
	    location[1] = NULL;
	    Location(0)->UnClaim(this);
	    xpos = 0;
	    ypos = 0;
	    distt = -1;
	    dirt = -1;
	    }
	  else  {
	    distt--;
//	    printf("%p refused entrance from //debug_position %d!\n", 
//		Location(0)->Next(((dirt+1)/2)*2), //debug_position);
	    }
	  }
	else  {
	  int dtt = distt;
	  if(dtt >= (steps/2))  dtt -=(steps/2);
	  switch(dirt)  {
	    case DIR_S:
	      xpos = 0;
	      ypos = (gm.ystep*dtt)/speed;
	      break;
	    case DIR_N:
	      xpos = 0;
	      ypos = -((gm.ystep*dtt)/speed);
	      break;
	    case DIR_E:
	      ypos = 0;
	      xpos = (gm.xstep*distt)/speed;
	      break;
	    case DIR_W:
	      ypos = 0;
	      xpos = -((gm.xstep*distt)/speed);
	      break;
	    case DIR_SE:
	      xpos = (gm.xstep*distt)/(speed*2);
	      ypos = (gm.ystep*distt)/speed;
	      break;
	    case DIR_ES:
	      ypos = (gm.ystep*dtt)/(speed*2);
	      xpos = (gm.xstep*dtt)/speed;
	      break;
	    case DIR_SW:
	      xpos = -((gm.xstep*distt)/(speed*2));
	      ypos = (gm.ystep*distt)/speed;
	      break;
	    case DIR_EN:
	      ypos = -((gm.ystep*dtt)/(speed*2));
	      xpos = (gm.xstep*dtt)/speed;
	      break;
	    case DIR_WS:
	      ypos = (gm.ystep*dtt)/(speed*2);
	      xpos = -((gm.xstep*dtt)/speed);
	      break;
	    case DIR_NE:
	      xpos = (gm.xstep*distt)/(speed*2);
	      ypos = -((gm.ystep*distt)/speed);
	      break;
	    case DIR_NW:
	      xpos = -((gm.xstep*distt)/(speed*2));
	      ypos = -((gm.ystep*distt)/speed);
	      break;
	    case DIR_WN:
	      ypos = -((gm.ystep*dtt)/(speed*2));
	      xpos = -((gm.xstep*dtt)/speed);
	      break;
	    }
	  }
	}
      else  {
	if((frame%4) == 2)  {
	  frame--;
	  Changed[thingnum] = 1;
	  }
	else  if(frame != FRAME_STAND)  {
	  ResetFrame();
	  Changed[thingnum] = 1;
	  }
	int add_dir = 1;
	if(facing > dirt)  {
	  if((facing-dirt) < ((dirt+12)-facing))
		add_dir = 11;
	  }
	else  {
	  if((dirt-facing) > ((facing+12)-dirt))
		add_dir = 11;
	  }
	TurnTo(facing+add_dir);
	}
      }
    }

//  else if(exists &&  (hit < -125))  {
//    Deselect();
//    image.Erase();
//    Remove();
//    exists = 0;
//    Location(0)->UnClaim(this);
//    Location(0)->Leave(this);
//    delete this;
//    printf("Dies at 2!\r\n");
//    Debug("Creature::Act() Return 7");
//    return;
//    }

  else if(exists && (dirf >= 0))  {
    Changed[thingnum] = 1;
    int add_dir = 1;
    if(frame == FRAME_STAND)  frame=FRAME_WALKL;
    else if(frame == FRAME_WALKL)  frame=FRAME_WALKR;
    else if(frame == FRAME_WALKR)  frame=FRAME_STAND;
    if(facing > dirf)  {
      if((facing-dirf) < ((dirf+12)-facing))  add_dir = 11;
      }
    else  {
      if((dirf-facing) > ((facing+12)-dirf))  add_dir = 11;
      }
    TurnTo(facing+add_dir);
    if(dirf == facing)  {
      dirf = -1;
      ResetFrame();
      }
    }

  else if(exists)  {
    { int tf = fatigue/50000;
      if(IsInWater())  {
	fatigue -= 20;
	if(fatigue<-100000) fatigue = -100000;
	}
      else  {
	fatigue += 10;
	if(fatigue>1000*hit) fatigue = 1000*hit;
	}
      if(tf != (fatigue/50000)) UpdateCondition();
      }
    if(goal[0] == NULL) {
      if((frame%4) == 2)  {
	frame--;
	Changed[thingnum] = 1;
	}
      else  if(frame != FRAME_STAND)  {
	ResetFrame();
	Changed[thingnum] = 1;
	}
      }
    }
  Debug("Creature::Act() End");
  }

void Creature::EvaluateGoal()  {
  if(goal[0] == NULL)  return;
  Debug("Creature::EvaluateGoal() Begin");
  if(hit<=10)  {
    target = 0;
    if(goal[0] != NULL && (goal[0]->Goal() == ACTION_ATTACK
	|| goal[0]->Goal() == ACTION_KILL
	|| goal[0]->Goal() == ACTION_PATROL
	|| goal[0]->Goal() == ACTION_FOLLOW
	|| goal[0]->Goal() == ACTION_FOLLOW_PATH
	|| goal[0]->Goal() == ACTION_DIG
	|| goal[0]->Goal() == ACTION_EXTINGUISH))  {
      ClearGoals();
      Debug("Creature::EvaluateGoal() Return 1");
      return;
      }
    }
  if(target != 0)  {
    if(list[target] == NULL || ((Creature *)list[target])->exists == 0)
      target = 0;
//    else if(goal[0]->Goal() == ACTION_KILL && goal[0]->objects == target)
//      if(((Creature *)list[target])->hit <= 0)
//        target = 0;
//    else if(((Creature *)list[target])->hit <= 10)
//      target = 0;
    }
  if(goal[0]->Goal() == ACTION_ATTACK)  {
    while(goal[0] != NULL && goal[0]->Goal() == ACTION_ATTACK
	&& (list[goal[0]->Object()] == NULL
	|| ((Creature *)list[goal[0]->Object()])->exists == 0
	|| ((Creature *)list[goal[0]->Object()])->hit <= 10))  {
      GoalDoneFirst();
      }
    }
  else if(goal[0]->Goal() == ACTION_KILL)  {
    while(goal[0] != NULL && goal[0]->Goal() == ACTION_KILL
	&& (list[goal[0]->Object()] == NULL
	|| ((Creature *)list[goal[0]->Object()])->exists == 0
	|| ((Creature *)list[goal[0]->Object()])->hit <= 0))  {
      GoalDoneFirst();
      }
    }
  if(goal[0] == NULL) { Debug("Creature::EvaluateGoal() Return 2"); return; }
  if(goal[0]->Goal() == ACTION_KILL || goal[0]->Goal() == ACTION_ATTACK)  {
    int ctr;
    for(ctr=1; ctr < goal[0]->objects.Size(); ctr++)  {
      int tmpv = goal[0]->objects[ctr];
      if(list[tmpv] == NULL || ((Creature *)list[tmpv])->exists == 0
		|| ((Creature *)list[tmpv])->hit <= 0)  {
	goal[0]->objects -= tmpv;
	ctr--;
	}
      }
    if(goal[0]->objects.Size() > 1)  FlipToCloserThing(goal[0]->objects);
    else if(goal[0]->objects.Size() < 1) GoalDone();
    }
  Debug("Creature::EvaluateGoal() End");
  }

void Creature::Think()  {
  Debug("Creature::Think() Begin");
  EvaluateGoal();
  if((target != 0) || (dirt >= 0) || (dirf >= 0) || (down)) 
    { Debug("Creature::Think() Return 1"); return; }
  if(hit > 10 && (goal[0] == NULL || goal[0]->Goal() == 0))  {
    int ctr;
    if(((Creature *)Location(0))->ftemp > 0)  {
      Action *tmpa = new Action(ACTION_EXTINGUISH, Location(0));
      Do(tmpa);
      }
    if((ticker & 31) == (thingnum & 31))  {
     IntList cells = Location(0)->
	CellsAtRange(weap[cw].mrange, weap[cw].range, 0, 4);
     for(ctr=0; ctr<cells.Size(); ctr++)  {
      if(list[cells[ctr]] != NULL)  {
	Thing *targ = list[cells[ctr]]->Inside(0);
	while(targ != NULL && ((Creature*)targ)->inside[0] != targ)  {
	  if(targ->Type() == THING_CREATURE && owner->IsEnemy(targ)
		&& ((Creature*)targ)->hit > 10) {
	    Action *tmpa = new Action(ACTION_ATTACK, (Creature *)targ);
	    Do(tmpa);
	    targ = NULL;
	    break;
	    }
	  else targ = ((Creature*)targ)->inside[0];
	  }
	}
      }
     }
    if(goal[0] == NULL || goal[0]->Goal() == 0)  {
      for(ctr=0; ctr<10; ctr+=2)  {
	if(Location(0)->Next(ctr) != NULL
		&& (goal[0] == NULL || goal[0]->Goal() == 0)
		&& ((Creature *)Location(0)->Next(ctr))->ftemp > 0)  {
	  Action *tmpa = new Action(ACTION_EXTINGUISH, Location(0)->Next(ctr));
	  Do(tmpa);
	  }
	}
      }
    else if(goal[0] == NULL || goal[0]->Goal() == 0
	&& ((Creature *)Location(0))->ftemp > 0)  {
      Action *tmpa = new Action(ACTION_EXTINGUISH, Location(0));
      Do(tmpa);
      }
    }
  if(goal[0] == NULL || goal[0]->Goal() == 0) 
    { Debug("Creature::Think() Return 2"); return; }
  switch(goal[0]->Goal())  {

    case (ACTION_CAST+SPELL_INVISIBILITY):
    case (ACTION_CAST+SPELL_VISION):
    case (ACTION_CAST+SPELL_DISINTIGRATE):
    case (ACTION_CAST+PRAYER_HEAL):  {
      Debug("Casting To-Creature Spell: Start");
      int spname = goal[0]->Goal() - ACTION_CAST;
      int range = sprng[spname];
      if(range == 0) range = vision;
      FlipToCloserThing(goal[0]->objects);

      if(progress == -99)  {
	--frame;
	if((frame & 15) > 8)  {
	  frame=FRAME_STAND;
	  GoalDoneFirst();
	  }
	}
      else if(Approach(list[goal[0]->Object()], range))  {
	if(spname < 32) frame = FRAME_CAST+progress;
	else frame = FRAME_PRAY+progress;
	if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	  Spell *tmps = new Spell(this, spname, goal[0]->objects);
	  sustained_spell += tmps->Number();
	  if(owner == p1) p1->RefigureSpellcasters();
	  progress = -99;
	  --frame;
	  }
	else ++progress;
	}
      else  progress = 0;
      Changed[thingnum] = 1;
      Waiting[thingnum] = 4;
      Debug("Casting To-Creature Spell: Finish");
      }break;

//    case (ACTION_CAST+SPELL_GLOBE_OF_SEEING):
    case (ACTION_CAST+SPELL_FIREBALL):
    case (ACTION_CAST+SPELL_REOPEN_PORTAL):
    case (ACTION_CAST+SPELL_OPEN_PORTAL):
    case (ACTION_CAST+SPELL_WALL_OF_STONE):
    case (ACTION_CAST+SPELL_ARROWS):  {
//      printf("Creature casting!\r\n");
      int spname = goal[0]->Goal() - ACTION_CAST;
      int range = sprng[spname];
      if(range == 0) range = vision;
      if(Approach(list[goal[0]->Object()], range))  {
	Spell *tmps = new Spell(this, spname, goal[0]->objects);
	sustained_spell += tmps->Number();
	GoalDone();
	if(owner == p1) p1->RefigureSpellcasters();
	}
      }break;

    case ACTION_ATTACK:  {
      if(hit<=10) { GoalAbort(); break; }
      if((list[goal[0]->Object()] == NULL)
	    || (((Creature *)list[goal[0]->Object()])->exists == 0))  {
	GoalDoneFirst();
	EvaluateGoal();
	break;
	}
      else  if(((Creature *)list[goal[0]->Object()])->hit > 10)  {
	if(facing == DirectionTo((Creature *)list[goal[0]->Object()]))  {
	  if(InWeaponRange((Creature *)list[goal[0]->Object()], 0))  {
            if(frame != FRAME_STAND)  {
	      ResetFrame();
              Changed[thingnum] = 1;
	      }
	    Engage((Creature *)list[goal[0]->Object()]);
	    }
	  else  {
	    Move(((Creature *)list[goal[0]->Object()])->Location(0));
	    }
	  }
	else  {
	  Face(DirectionTo((Creature *)list[goal[0]->Object()]));
	  }
	}
      else  {
	GoalDoneFirst();
 	EvaluateGoal();
	}
      }  break;

    case ACTION_KILL:  {
      if(hit<=10) { GoalAbort(); break; }
      if((list[goal[0]->Object()] == NULL)
	    || (((Creature *)list[goal[0]->Object()])->exists == 0))  {
	GoalDoneFirst();
	EvaluateGoal();
	break;
	}
      else  if(((Creature *)list[goal[0]->Object()])->hit >= 0)  {
	if(InWeaponRange((Creature *)list[goal[0]->Object()], 0))  {
          if(frame != FRAME_STAND)  {
	    ResetFrame();
	    Changed[thingnum] = 1;
	    }
	  if(facing == DirectionTo((Creature *)list[goal[0]->Object()]))  {
	    Engage((Creature *)list[goal[0]->Object()]);
            if(((Creature *)list[goal[0]->Object()])->hit <= 0)  {
	      GoalDoneFirst();
	      EvaluateGoal();
	      break;
	      }
	    }
	  else  {
	    Face(DirectionTo((Creature *)list[goal[0]->Object()]));
	    }
	  }
	else  {
	  Move(((Creature *)list[goal[0]->Object()])->Location(0));
	  }
	}
      }  break;

    case ACTION_TURN:  {
      if((goal[0]->Object() < 0) || (goal[0]->Object() == facing))  {
	GoalDone();
	break;
	}
      Face(goal[0]->Object());
      }  break;

    case ACTION_FOLLOW:  {
      if(list[goal[0]->Object()] == NULL
		|| ((Creature *)list[goal[0]->Object()])->exists == 0)  {
	GoalDoneFirst();
	}
      else  {
	if(!(location[0] == (((Creature *)list[goal[0]->Object()])->location[0])
		|| Location(0)->IsNextTo(
		((Creature *)list[goal[0]->Object()])->Location(0))))  {
	  Move(((Creature *)list[goal[0]->Object()])->Location(0));
	  }
	}
      } break;

    case ACTION_EXCUSED:  {
      if(Location(0)->Claim(this, altitude, height, 0, 0)
	  && Location(0)->Enter(this, altitude, height, 0, 0))  {
	Location(0)->UnClaim(this);
	ThankYou();
	GoalDone();
	}
      else Waiting[thingnum] = 1;
//      printf("Checking, got deb_loc of %d!\n", //debug_position);
      } break;

    case ACTION_DIG:  {
      if(Approach(list[goal[0]->Object()], 1))  {
	if((frame & 248) == FRAME_DIG)  { ++frame; }
	if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	  frame=FRAME_STAND;
	  if(list[goal[0]->Object()]->BurnFeul(10) <= 0)  {
	    ((Cell*)list[goal[0]->Object()])->SetTerrain(TERRAIN_DIRT);
	    }
	  ResetFrame();
	  if(((Cell*)list[goal[0]->Object()])->Terrain() == TERRAIN_DIRT)
	    GoalDoneFirst();
	  }
	else { frame=FRAME_DIG; }
	Changed[thingnum] = 1;
	}
      } break;

    case ACTION_LOOK:  {
      int dir = DirectionTo(list[goal[0]->Object()]->Location(0)->Location(0));
      if(facing != dir) Face(dir);
      GoalDone();
      } break;

    case ACTION_IGNITE:  {
      if(goal[0]->IsDone())  {
	Move(Location(0)->Location(0)->Next(facing+6));
	GoalDone();
	break;
	}
      if(Approach(list[goal[0]->Object()], 1))  {
	if(progress==0)  {
	  for(frame=FRAME_STORE; pics[gm.xstep>>5][gtype][frame] != NULL;
		++frame);
	  --frame; progress=1;
	  }
	else if(progress==1)  {
	  --frame;
	  if(frame < FRAME_STORE)  { frame=FRAME_IGNP; progress=2; }
	  }
	else if(progress==2)  {
	  ++frame;
	  if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	    frame=FRAME_IGN; progress=3;
	    }
	  }
	else if(progress==3)  {
	  ++frame;
	  if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	    list[goal[0]->Object()]->Heat(1, 500);
	    if(((Creature*)list[goal[0]->Object()])->ftemp >
		(((Creature*)list[goal[0]->Object()])->fresist)+1
		|| ((Creature*)list[goal[0]->Object()])->ffeul <= 0)  {
	      progress=4;
	      for(frame=FRAME_IGNP; pics[gm.xstep>>5][gtype][frame] != NULL;
			++frame);
	      --frame; progress=4;
	      }
	    else { frame=FRAME_IGN; }
	    }
	  }
	else if(progress==4)  {
	  --frame;
	  if(frame < FRAME_IGNP)  {
	    frame=FRAME_STORE; progress=5;
	    }
	  }
	else if(progress==5)  {
	  ++frame;
	  if(pics[gm.xstep>>5][gtype][frame] != NULL)  {
	    ResetFrame();
	    GoalDoneFirst();
	    Move(Location(0)->Location(0)->Next(facing+6));
	    }
	  }
	Changed[thingnum] = 1;
	Waiting[thingnum] = 4;
	}
      } break;

    case ACTION_GOGET:
    case ACTION_HARVEST:  {
      if(mat_type != goal[0]->Object())  {
	mat_type = goal[0]->Object();
	mat_ammt = 1;
	}
      else if(mat_ammt < 100)  {
	mat_ammt++;
	}
      if(mat_ammt > 99) GoalDone();
      } break;

    case ACTION_EXTINGUISH:  {
      if(Approach(list[goal[0]->Object()], 1))  {
	if(progress==0)  {
	  for(frame=FRAME_STORE; pics[gm.xstep>>5][gtype][frame] != NULL;
		++frame);
	  --frame; progress=1;
	  }
	else if(progress==1)  {
	  --frame;
	  if(frame < FRAME_STORE)  { frame=FRAME_EXTP; progress=2; }
	  }
	else if(progress==2)  {
	  ++frame;
	  if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	    frame=FRAME_EXT; progress=3;
	    }
	  }
	else if(progress==3)  {
	  ++frame;
	  if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	    list[goal[0]->Object()]->Heat(-1, 500);
	    location[0]->Heat(-1, 500);

	    if(((Creature*)list[goal[0]->Object()])->ftemp <= 0
		&& ((Creature*)Location(0))->ftemp <= 0)  {
	      progress=4;
	      for(frame=FRAME_EXTP; pics[gm.xstep>>5][gtype][frame] != NULL;
			++frame);
	      --frame; progress=4;
	      }
	    else { frame=FRAME_EXT; }
	    }
	  }
	else if(progress==4)  {
	  --frame;
	  if(frame < FRAME_EXTP)  {
	    frame=FRAME_STORE; progress=5;
	    }
	  }
	else if(progress==5)  {
	  ++frame;
	  if(pics[gm.xstep>>5][gtype][frame] != NULL)  {
	    ResetFrame();
	    GoalDoneFirst();
	    }
	  }
	Changed[thingnum] = 1;
	Waiting[thingnum] = 4;
	}
      } break;

    case ACTION_FOLLOW_PATH:  {
      if(goal[0]->objects.Size() > 0)  {
	if(list[goal[0]->Object()] == NULL
		|| ((Creature *)list[goal[0]->Object()])->exists == 0)  {
	  GoalAbort();
	  }
	else  {
	  if(location[0]->Number() != goal[0]->Object())  {
	    if(GetPath(DirectionTo((Cell *)list[goal[0]->Object()])))
	      Move((Cell *)list[goal[0]->Object()]);
	    }
	  else  {
	    goal[0]->objects.Clear();
	    exploring = 0;
	    }
	  }
	}
      else  {
	if(Location(0)->Next(facing) && Location(0)->Next(facing)->IsPath())  {
	  if(GetPath(facing))  {
	    Move((Cell *)Location(0)->Next(facing));
	    exploring = 0;
	    }
	  }
	else if(Location(0)->Next(facing+1) && Location(0)->Next(facing+1)->IsPath())  {
	  if(GetPath(facing+1))  {
	    Move((Cell *)Location(0)->Next(facing+1));
	    exploring = 0;
	    }
	  }
	else if(Location(0)->Next(facing+11) && Location(0)->Next(facing+11)->IsPath()) {
	  if(GetPath(facing+11))  {
	    Move((Cell *)Location(0)->Next(facing+11));
	    exploring = 0;
	    }
	  }
	else if(Location(0)->Next(facing+2) && Location(0)->Next(facing+2)->IsPath())  {
	  if(GetPath(facing+2))  {
	    Move((Cell *)Location(0)->Next(facing+2));
	    exploring = 0;
	    }
	  }
	else if(Location(0)->Next(facing+10) && Location(0)->Next(facing+10)->IsPath()) {
	  if(GetPath(facing+10))  {
	    Move((Cell *)Location(0)->Next(facing+10));
	    exploring = 0;
	    }
	  }
	else  {
	  exploring+=(1+(facing&1));
	  if(exploring > EXPLORE_FOR)  GoalAbort();
	  else  {
	    if(GetPath(facing))  {
	      Move((Cell *)Location(0)->Next(facing));
	      }
	    else  if(Location(0)->Next(facing) != NULL)  {
	      Move(Location(0)->Next(facing)->Next(facing));
	      exploring+=(1+(facing&1));
	      if(exploring > EXPLORE_FOR)  GoalAbort();
	      }
	    }
	  }
	}
      } break;

    case ACTION_FLEE:  {
      if(Location(0)->Next(goal[0]->Object()) == NULL)  GoalAbort();
      else  {
	Move(Location(0)->Next(goal[0]->Object()));
	}
      }	break;

    case ACTION_STOP_SPELLS:  {
      int ctr;
      for(ctr=0; ctr<sustained_spell.Size(); ctr++)  {
	if(list[sustained_spell[ctr]] != NULL)  {
	  delete ((Spell *)list[sustained_spell[ctr]]);
	  ctr--;
	  }
	}
      sustained_spell.Clear();
      GoalAbort();
      }	break;

    case ACTION_GO:  {
      if((goal[0]->Object() == 0) || (list[goal[0]->Object()] == NULL)
		|| ((list[goal[0]->Object()] == Location(0))
		&& (NULL == location[1])))  {
	GoalDone();
        if(frame != FRAME_STAND)  {
	  ResetFrame();
          Changed[thingnum] = 1;
	  }
        Debug("Creature::Think() Return 3");
	return;
	}
//      if(Move(((Creature *)list[goal[0]->Object()])->Location(0)))
      if(Move(((Cell *)list[goal[0]->Object()])))
	GoalDone();
      if(dirt == -1)  GoalDone();
      }	break;

    default:  {						//BUILD!!
      int mat, st = goal[0]->Goal() - ACTION_BUILD;
      if(st < 0)  break; //U2_Exit(1, "Bad action for creature (%d)\r\n", st);
      mat = st%MATERIAL_MAXBUILD;
      st /= MATERIAL_MAXBUILD;
//      printf("Building %d out of %d!\r\n", st, mat);
//      if(mat_type != mat || (mat_ammt < struct_qty[st] && mat_ammt < 100))  {
      if(progress < 4 && (mat_type != mat || mat_ammt < 1))  {
	Action *tmpa = new Action(ACTION_GOGET, mat, struct_qty[st]);
	DoFirst(tmpa);
        }
      else  {
	FlipToCloserThing(goal[0]->objects);
	if(DistanceTo(list[goal[0]->Object()]) == 0)  {
	  Move(Location(0)->Next(0));
	  }
	else if(Approach(list[goal[0]->Object()], 1))  {
	  if(progress==0)  {
	    for(frame=FRAME_STORE; pics[gm.xstep>>5][gtype][frame] != NULL;
		++frame);
	    --frame; progress=1;
	    }
	  else if(progress==1)  {
	    --frame;
	    if(frame < FRAME_STORE)  { frame=FRAME_BUILDP; progress=2; }
	    }
	  else if(progress==2)  {
	    ++frame;
	    if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	      frame=FRAME_BUILD; progress=3;
	      }
	    }
	  else if(progress==3)  {
	    ++frame;
	    if(pics[gm.xstep>>5][gtype][frame] == NULL)  {
	      frame = FRAME_BUILD;
	      if(list[goal[0]->Object()]->Type() == THING_STRUCT)  {
		Structure *tmps = (Structure*)list[goal[0]->Object()];
		mat_ammt -= tmps->AddMaterials(mat_type, 1);
		if(mat_ammt == 0) mat_type = MATERIAL_NONE;
		if(tmps->IsFinished()) { progress = 4; }
		}
	      else  {
		Structure *tmps = new Structure(st, mat);
		if(!(tmps->Place((Cell*)list[goal[0]->Object()])))  {
		  delete tmps;
		  //progress = 4;
		  }
		else  {
		  goal[0]->objects -= goal[0]->Object();
		  goal[0]->objects += tmps->Number();
		  goal[0]->objects.RotateToElement(goal[0]->objects.Size()-1);
		  }
		}
	      }
	    if(progress == 4)  { // Ready for next step
	      for(frame=FRAME_BUILDP; pics[gm.xstep>>5][gtype][frame] != NULL;
			++frame);
	      --frame; progress=4;
	      }
	    }
	  else if(progress==4)  {
	    --frame;
	    if(frame < FRAME_BUILDP)  {
	      frame=FRAME_STORE; progress=5;
	      }
	    }
	  else if(progress==5)  {
	    ++frame;
	    if(pics[gm.xstep>>5][gtype][frame] != NULL)  {
	      ResetFrame();
	      GoalDoneFirst();
	      }
	    }
	  Changed[thingnum] = 1;
	  Waiting[thingnum] = 4;
	  }
	}
      } break;
    }
  Debug("Creature::Think() End");
  }

void Creature::DoFirst(const Action *in)  {
  if(goal[0] != NULL && goal[0]->Goal() == ACTION_EXCUSED)  return;
  int ctr;
  if(goal[11] != NULL) goal[11]->Detach();
  goal[11] = NULL;
  for(ctr=11; ctr>0; ctr--) goal[ctr] = goal[ctr-1];

  goal[0] = (Action *)in;
  goal[0]->Attach();
  Waiting[thingnum] = 1;
  }

void Creature::DoWhenDone(const Action *in)  {
  if(goal[0] != NULL && goal[0]->Goal() == ACTION_EXCUSED)  return;
  int ctr;
  for(ctr=0; ctr<12 && goal[ctr] != NULL; ctr++);
  if(ctr<12)  {
    goal[ctr] = (Action *)in;
    goal[ctr]->Attach();
    Waiting[thingnum] = 1;
    }
  }

void Creature::Do(const Action *in)  {
  if(goal[0] != NULL && goal[0]->Goal() == ACTION_EXCUSED)  return;
  ClearGoals();
  goal[0] = (Action *)in;
  goal[0]->Attach();
  Waiting[thingnum] = 1;
  }

void Creature::Go(int indir)  {
  if(dirt < 0)  dirt = indir;
  }

void Creature::updateme()  {
  Debug("Creature::updateme() Begin");
  if(!(Location(0)->InView()))  {
    Debug("Creature::updateme() Out of View");
    image.Erase();
    selbox.Erase();
    weaps.Erase();
    }
  else  {
    if(exists)  {
      Debug("Creature::updateme() Find Location");
      int x, y;
      x = xpos+Location(0)->XPos(); 
      y = ypos+Location(0)->YPos();
      if(location[1] != NULL)  {
        x += xpos+Location(1)->XPos(); 
        y += ypos+Location(1)->YPos();
        x /= 2;	y /= 2;
        }
      if(hit >= -125)  {
	Debug("Creature::updateme() I am still there");
        if(hit <= -100)  { 
	  Debug("Creature::updateme() I am dead");
	  frame = FRAME_SPLAT;
	  height = 0;
	  if(inside[0] != NULL)  inside[0]->Fall();
	  }
        else  if(hit <= 0)  {
	  Debug("Creature::updateme() I am down");
	  frame = FRAME_DOWN;
	  }
	Debug("Creature::updateme() Setup My Image");
//	printf("Using pics[%d][%d][%d]\n", gm.xstep>>5, gtype, frame);
	image.UseImage(pics[gm.xstep>>5][gtype][frame]);
	Debug("Creature::updateme() Setup My SelectboxImage");
	selbox.UseImage(selboxg[gm.xstep>>5]);
	Debug("Creature::updateme() Setup Priorities");
	image.SetPriority(100000-(Height()));
	selbox.SetPriority(100001-(Height()));
	weaps.SetPriority(100002-(Height()));
	Debug("Creature::updateme() Setup Position");
	image.Move(x, y, -((facing<<16)/12));
	Debug("Creature::updateme() Setup Select Box");
	if(selected) selbox.Move(x, y);
	else selbox.Erase();
	Debug("Creature::updateme() Setup Weapon Image");
	if(cw>=0 && weap[cw].image[0][gm.xstep>>5] != NULL) {
	  weaps.UseImage(weap[cw].image[0][gm.xstep>>5]);
	  //weaps.SetImage(weap[cw].image[0][gm.xstep>>5]);
	  //weaps.UseImage(weap[cw].image[frame >= FRAME_M0_1
		//&& frame <= FRAME_M0_7][gm.xstep>>5]);
	  weaps.Move(x, y, -((facing<<16)/12));
	  }
	else weaps.Erase();
        }
      }
    }
  Debug("Creature::updateme() Spells");
  int ctr;
  for(ctr=0; ctr<spell.Size(); ctr++)  {
    Changed[spell[ctr]] = 1;
    }
  Debug("Creature::updateme() End");
  }

int Creature::GetPath(char dir)  {
  if(location[0] == location[1])  return (1==2);
  char g1=0, g2=0, g3=0;
  if(Location(0)->Next(dir) != NULL)  {
    if(stats->speed[Location(0)->Next(dir)->Terrain()] <= 0)  {
      return (1==2);
      }
    g1 = Location(0)->Next(dir)->Claim(this, altitude, height);
//    if(g1) printf("Got claim from %p at deb_pos %d!\n", 
//	Location(0)->Next(dir), //debug_position);
    g2 = 1;	g3 = 1;
    if(dir % 2)  {
      Creature *tmpc;
      g2 = Location(0)->Next(dir+1)->
		ClaimHalf(this, (dir+9)%12, altitude, height);
      g3 = Location(0)->Next(dir+11)->
		ClaimHalf(this, (dir+3)%12, altitude, height);
      if(!g2)  {
	tmpc = (Creature*)Location(0)->Next(dir+1)->Inside(0);
	if(tmpc!=NULL)
	  g2 = (tmpc->Type() == THING_CREATURE && tmpc->ExcuseMe(dir));
          if(g2) g2 = Location(0)->Next(dir+1)->
		ClaimHalf(this, (dir+9)%12, altitude, height);
	}
      if(!g3)  {
	tmpc = (Creature*)Location(0)->Next(dir+11)->Inside(0);
	if(tmpc!=NULL)
	  g3 = (tmpc->Type() == THING_CREATURE && tmpc->ExcuseMe(dir));
          if(g3) g3 = Location(0)->Next(dir+11)->
		ClaimHalf(this, (dir+3)%12, altitude, height);
	}
      }
    if(!(g1 && g2 && g3))  {
//      printf("Unclaiming Shit!\n");
      if(g1)  Location(0)->Next(dir)->UnClaim(this);
      if(dir % 2)  {
        if(g2)  Location(0)->Next(dir+1)->UnClaimHalf(this);
        if(g3)  Location(0)->Next(dir+11)->UnClaimHalf(this);
        }
      return(1==2);
      }
    Claimed(Location(0)->Next(dir));
    return(1==1);
    }
  return(1==2);
  }

void Creature::Claimed(Thing *in)  {
  speedbase = stats->speed[((Cell *)in)->Terrain()];
  if(in->Inside(0) != NULL && in->Inside(0)->Type() == THING_STRUCT)
    speedbase = stats->speed[TERRAIN_PATH];
  speed = speedbase * speedmul;
  }

void Creature::SetSpeedMul(int in)  {
  if(in == speedmul)  return;
  if(distt > 0)  {
    distt *= in;
    distt /= speedmul;
    }
  speedmul = in;
  speed = speedbase * speedmul;
  }

void Creature::TurnTo(char dir)  {
  if(dir == facing)  return;
  char tmp = facing;
  facing = dir;
  facing %= 12;
  DoVision();
  facing = tmp;
  UndoVision();
  facing = dir;
  facing %= 12;
  }

void Creature::FillVision(char dir, char field)  {
  if(field == 0)  return;
  int ctr, ctr2, newdir = dir+2;
  Cell *look, *look1;
  if(field<0)  { field = -field;  newdir+=8; }
  look = Location(0);
  for(ctr=0; look != NULL && ctr < vision; ctr++)  {
    look1 = look->Next(newdir);
    for(ctr2=0; look1!=NULL && ctr2<(vision-ctr) &&
	(field > 1 || ctr2 < (ctr+1)); ctr2++)  {
      if(!mvision)  look1->See();
      else  look1->MSee();
      look1 = look1->Next(newdir);
      }
    look1 = look->Next(dir);
    if(look1 == NULL)  {
      look1 = look->Next(newdir);
      if(look1 != NULL)  look1 = look1->Next(dir);
      ctr++;
      if(ctr < vision && look1 != NULL)  {
	if(!mvision)  look1->See();
	else  look1->MSee();
	}
      }
    look = look1;
    }
  }

void Creature::ClearVision(char dir, char field)  {
  if(field == 0)  return;
  int ctr, ctr2, newdir = dir+2;
  Cell *look, *look1;
  if(field<0)  { field = -field;  newdir+=8; }
  look = Location(0);
  for(ctr=0; look != NULL && ctr < vision; ctr++)  {
    look1 = look->Next(newdir);
    for(ctr2=0; look1!=NULL && ctr2<(vision-ctr) &&
	(field > 1 || ctr2 < (ctr+1)); ctr2++)  {
      if(!mvision)  look1->CantSee();
      else  look1->MCantSee();
      look1 = look1->Next(newdir);
      }
    look1 = look->Next(dir);
    if(look1 == NULL)  {
      look1 = look->Next(newdir);
      if(look1 != NULL)  look1 = look1->Next(dir);
      ctr++;
      if(ctr < vision && look1 != NULL)  {
	if(!mvision)  look1->CantSee();
	else  look1->MCantSee();
	}
      }
    look = look1;
    }
  }

void Creature::DoVision()  {
  if(hit < 0)  return;
  if(owner != p1)  return;
  if(!mvision)  Location(0)->See();
  else  Location(0)->MSee();
  if(hit < 1)  return;
  char fieldofvision = stats->fieldofvision;
  int ctr;
  Cell *look, *tmp2;
  if(location[1] != NULL)  {
    if(!mvision)  Location(1)->See();
    else  Location(1)->MSee();
    look = Location(0)->Next(facing-1);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->See();
      else  look->MSee();
      look = look->Next(facing-1);
      }
    look = Location(1)->Next(facing+1);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->See();
      else  look->MSee();
      look = look->Next(facing+1);
      }
    if(fieldofvision > 0)
      FillVision(facing-1, 2);
    if(fieldofvision > 1)  {
      FillVision(facing+11, -(fieldofvision-1));
      tmp2 = Location(0);
      location[0] = location[1];
      FillVision(facing+1, fieldofvision-1);
      location[0] = tmp2;
      }
    if(fieldofvision > 3)  {
      FillVision(facing+9, -(fieldofvision-3));
      tmp2 = location[0];
      location[0] = location[1];
      FillVision(facing+3, fieldofvision-3);
      location[0] = tmp2;
      }
    if(fieldofvision > 5)  {
      FillVision(facing+7, -2);
      }
    }
  else  if(facing % 2)  {
    look = Location(0)->Next(facing-1);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->See();
      else  look->MSee();
      look = look->Next(facing-1);
      }
    if(fieldofvision > 0)
      FillVision(facing-1, 2);
    if(fieldofvision > 1)  {
      FillVision(facing+1, fieldofvision-1);
      FillVision(facing+11, -(fieldofvision-1));
      }
    if(fieldofvision > 3)  {
      FillVision(facing+3, fieldofvision-3);
      FillVision(facing+9, -(fieldofvision-3));
      }
    if(fieldofvision > 5)  {
      FillVision(facing+5, 2);
      look = Location(0)->Next(facing+7);
      for(ctr=0; look != NULL && ctr < vision; ctr++)  {
	if(!mvision)  look->CantSee();
	else  look->MCantSee();
	look = look->Next(facing+7);
	}
      }
    }
  else  {
    look = Location(0)->Next(facing);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->See();
      else  look->MSee();
      look = look->Next(facing);
      }
    FillVision(facing, fieldofvision);
    FillVision(facing, -fieldofvision);
    if(fieldofvision > 2)  {
      FillVision(facing+2, fieldofvision-2);
      FillVision(facing+10, -(fieldofvision-2));
      }
    if(fieldofvision > 4)  {
      FillVision(facing+4, fieldofvision-4);
      FillVision(facing+8, -(fieldofvision-4));
      look = Location(0)->Next(facing+6);
      for(ctr=0; look != NULL && ctr < vision; ctr++)  {
	if(!mvision)  look->CantSee();
	else  look->MCantSee();
	look = look->Next(facing+6);
	}
      }
    }
  }

void Creature::UndoVision()  {
  if(hit < 0)  return;
  if(owner != p1)  return;
  if(!mvision)  Location(0)->CantSee();
  else  Location(0)->MCantSee();
  if(hit < 1)  return;
  char fieldofvision = stats->fieldofvision;
  int ctr;
  Cell *look, *tmp2;
  if(Location(1) != NULL)  {
    if(!mvision)  Location(1)->CantSee();
    else  Location(1)->MCantSee();
    look = Location(0)->Next(facing-1);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->CantSee();
      else  look->MCantSee();
      look = look->Next(facing-1);
      }
    look = Location(1)->Next(facing+1);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->CantSee();
      else  look->MCantSee();
      look = look->Next(facing+1);
      }
    if(fieldofvision > 0)
      ClearVision(facing-1, 2);
    if(fieldofvision > 1)  {
      ClearVision(facing+11, -(fieldofvision-1));
      tmp2 = location[0];
      location[0] = location[1];
      ClearVision(facing+1, fieldofvision-1);
      location[0] = tmp2;
      }
    if(fieldofvision > 3)  {
      ClearVision(facing+9, -(fieldofvision-3));
      tmp2 = location[0];
      location[0] = location[1];
      ClearVision(facing+3, fieldofvision-3);
      location[0] = tmp2;
      }
    if(fieldofvision > 5)  {
      ClearVision(facing+7, -2);
      }
    }
  else  if(facing % 2)  {
    look = Location(0)->Next(facing-1);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->CantSee();
      else  look->MCantSee();
      look = look->Next(facing-1);
      }
    if(fieldofvision > 0)
      ClearVision(facing-1, 2);
    if(fieldofvision > 1)  {
      ClearVision(facing+1, fieldofvision-1);
      ClearVision(facing+11, -(fieldofvision-1));
      }
    if(fieldofvision > 3)  {
      ClearVision(facing+3, fieldofvision-3);
      ClearVision(facing+9, -(fieldofvision-3));
      }
    if(fieldofvision > 5)  {
      ClearVision(facing+5, 2);
      look = Location(0)->Next(facing+7);
      for(ctr=0; look != NULL && ctr < vision; ctr++)  {
	if(!mvision)  look->See();
	else  look->MSee();
	look = look->Next(facing+7);
	}
      }
    }
  else  {
    look = Location(0)->Next(facing);
    for(ctr=0; look != NULL && ctr < vision; ctr++)  {
      if(!mvision)  look->CantSee();
      else  look->MCantSee();
      look = look->Next(facing);
      }
    ClearVision(facing, fieldofvision);
    ClearVision(facing, -fieldofvision);
    if(fieldofvision > 2)  {
      ClearVision(facing+2, fieldofvision-2);
      ClearVision(facing+10, -(fieldofvision-2));
      }
    if(fieldofvision > 4)  {
      ClearVision(facing+4, fieldofvision-4);
      ClearVision(facing+8, -(fieldofvision-4));
      look = Location(0)->Next(facing+6);
      for(ctr=0; look != NULL && ctr < vision; ctr++)  {
	if(!mvision)  look->See();
	else  look->MSee();
	look = look->Next(facing+6);
	}
      }
    }
  }

void Creature::Damage(int dam)  {
  if(hit <= -100)  return;
  UndoVision();
  int tmphit = (int)hit;
  tmphit -= dam;
  if(tmphit <= -100)  {
    hit = (char)-100;
    }
  else  if(tmphit < 0 && hit >= 0)  {
    Changed[thingnum] = 1;
    hit = (char)tmphit;
    Death->Play();
    DoVision();
    }
  else  {
    if((hit > 0 && tmphit <= 0) || (hit > -100 && tmphit <= -100))
        Changed[thingnum] = 1;
    hit = (char)tmphit;
    DoVision();
    }
  if(fatigue>1000*hit) fatigue = 1000*hit;
  }

void Creature::DamageFull(int dam)  {
  UndoVision();
  int tmphit = (int)hit;
  tmphit -= dam;
  if(tmphit < -125)  {
    Changed[thingnum] = 1;
    hit = -126;
    }
  else  if(tmphit < 0 && hit >= 0)  {
    Changed[thingnum] = 1;
    hit = (char)tmphit;
    Death->Play();
    DoVision();
    }
  else  {
    if((hit > 0 && tmphit <= 0) || (hit > -100 && tmphit <= -100))
        Changed[thingnum] = 1;
    hit = (char)tmphit;
    DoVision();
    }
  if(fatigue>1000*hit) fatigue = 1000*hit;
  }

void Creature::Attack(Creature *target, int w)  {
//  printf("Attack with %d\n", w);
  if(!(InWeaponRange(target, w)))  return;
  if(weap[w].ammo < 0)  {
    target->Strike(this, stats->melee, weap[w].pdam*stats->str,
	weap[w].bdam*stats->str);
    }
  else  {
    new Ammo(weap[w].ammo, this, target, 32, stats->missile,
        weap[w].pdam*stats->str, weap[w].bdam*stats->str);
    }
  }

int Creature::InWeaponRange(Creature *targ, int w)  {
//  printf("In %d's weapon %d's range of %d?\n", thingnum, w, weap[w].range);
  return (DistanceTo(targ) <= (weap[w].range)
//	<= (stats->reach+weap[w].range)
	&& targ->Height() > altitude-(height/2)
	&& targ->altitude < Height()+(height/2));
  }

void Creature::Strike(Creature *attacker, int skill, int pdam, int bdam)  {
  Punch->Play();
  int dam = bdam;
  if(stats->body < 1)  dam = 500+stats->armor;
  else  {
    dam -= (stats->armor*stats->body);
    dam += pdam;
    dam /= stats->body;
    }
  if(dam < 0)  dam = 0;
  Damage(dam);
//  if((goal[0]->Goal() == 0) && (hit > 10))  {
//  if((target == NULL || target == this) && hit > 10)  {
  if(goal[0] == NULL || (goal[0]->Goal() != ACTION_ATTACK
	&& goal[0]->Goal() != ACTION_KILL
	&& hit > 10 && goal[0]->Goal() != ACTION_EXCUSED))  {
    if(attacker->type == THING_CREATURE)  {
      Action *tmpa = new Action(ACTION_ATTACK, attacker);
      Do(tmpa);
      }
    }
  }

void Creature::Engage(Creature *targ)  {
  if(targ != this)  target = targ->Number();
  }

void Creature::Face(int dirto)  {
  dirf = dirto;
  }

int Creature::Move(Cell *cellto)  {
  int ret = (1==2);
  dirt = DirectionTo(cellto);
  if(!GetPath(dirt))  {
    if(Location(0)->IsNextTo(cellto))  {
      if(frame != FRAME_STAND)  {
        ResetFrame();
        Changed[thingnum] = 1;
	}
      ret = (1==1);
      dirt = -1;
      return ret;
      }
    else  {
      char full=1;
      char ctr;
      for(ctr=0; ctr<12 && full == 1; ctr+=2)
        if((cellto->Next(ctr) == NULL)
		|| (cellto->Next(ctr)->Inside(0) == NULL)
		|| (cellto->Next(ctr)->Inside(1) == NULL))
	  full = 0;
      if(full == 1 && DistanceTo(cellto) < 3)  {
        if(frame != FRAME_STAND) {
          ResetFrame();
          Changed[thingnum] = 1;
          }
        dirt = -1;
        ret = (1==1);
        return ret;
        }
      }
    if(GetPath(dirt+1))
      { dirt+=1;	dirt%=12;  }
    else if(GetPath(dirt+11))
      { dirt+=11;	dirt%=12;  }
//    else if(((dirt+8)%12) != facing && GetPath(dirt+2))
    else if(GetPath(dirt+2))
      { dirt+=2;	dirt%=12;  }
    else if(((dirt+4)%12) != facing && GetPath(dirt+10))
      { dirt+=10;	dirt%=12;  }
    else if(((dirt+9)%12) != facing && GetPath(dirt+3))
      { dirt+=3;	dirt%=12;  }
    else if(GetPath(dirt+3))
      { dirt+=3;	dirt%=12;  }
    else if(((dirt+10)%12) != facing && GetPath(dirt+4))
      { dirt+=4;	dirt%=12;  }
//    else if(((dirt+3)%12) != facing && GetPath(dirt+9))
//      { dirt+=9;	dirt%=12;  }
//    else if(((dirt+2)%12) != facing && GetPath(dirt+8))
//      { dirt+=8;	dirt%=12;  }
    }
  return ret;
  }

void  Creature::ResetFrame()  {
//  if(!limping)
	frame = FRAME_STAND;
//  else  if(!crawling)  frame = FRAME_LIMP;
//  else  frame = FRAME_CRAWL;
  progress = 0;
  }

int Creature::SafestDirection()  return ret {
  ret = -1;
  int ctr, dirs = 0x0FFF, tmpd = 0x0FFF;
  for(ctr=0; ctr<12; ctr++)  {
    if(Location(0)->Next(ctr) == NULL
	|| Location(0)->Next(ctr)->Owner(0) != NULL)  {
      tmpd &= (0x0FFF - (1<<ctr));
      }
    }
  if(tmpd != 0) dirs = tmpd;
  for(ctr=facing; ret == -1 && ctr<facing+12; ctr++)  {
    if(tmpd & (1<<(ctr%12)))  ret = ctr;
    }
  }

int Creature::CanCast(int sp)  return ret {
  if(sp < 32) ret = spells & (1 << sp);
  else ret = prayers & (1 << (sp-32));
  }

void Creature::LearnSpell(int sp)  {
  if(sp < 32) spells |= (1 << sp);
  else prayers |= (1 << (sp-32));
  UpdateCMap();
  }

int Creature::AddWeapon(Weapon &w)  {
  if(w.range > weap[0].range)  {
//    printf("adding first as %d to %d\n", w.range, thingnum);
    weap[1] = weap[0];
    weap[0] = w;
    cw = 0;
    }
  else {
    weap[1] = w;
    cw = 1;
    }
  return (1==1);
  }

void Creature::GetOwnStats()  {
  if(stats == cstats[gtype])  stats = new Statistics(*stats);
  }

int Creature::IsInWater()  {
  if(Location(0)->Inside(0) != this) return 0;
  int ter = Location(0)->Location(0)->Terrain();
  return(ter > TERRAIN_LOWWATER);
  }

int Creature::ExcuseMe(char dir)  {
  return (1==2);
//  printf("Excuse Me!!!\n");
  if(target > 0 || dirt != -1 || dirf != -1 || (!goal[0]->IsDone()))
    return (1==2);
//  if(!(Location(0)->Location(0)->Claimed(this)))  return(1==2);
//  Location(0)->Location(0)->UnClaimHalf(this);
  Location(0)->Location(0)->LeaveHalf(this);
  Action *tmpa = new Action(ACTION_EXCUSED);
  Do(tmpa);
  Waiting[thingnum] = 1;
  return (1==1);
  }

int Creature::ThankYou()  {
//  location[0]->Claim(this, altitude, height);
//  location[0]->Enter(this, altitude, height, height, height);
  return (1==1);
  }

int Creature::Approach(Thing *targ, int range)  {
  if(DistanceTo(targ) == 0)  return(1==1);
  if(DirectionTo(targ) == facing)  {
    if(DistanceTo(targ) <= range)  {
      return(1==1);
      }
    else if(DistanceTo(targ) == (range+1))  {
      if(GetPath(facing&254))
	Move(Location(0)->Location(0)->Next(facing&254));
      else
	Move(Location(0)->Location(0)->Next((facing&254)+2));
      }
    else {
//      Move(Location(0)->Location(0)->Next(facing));
      Move(targ->Location(0));
      }
    }
  else  {
    Face(DirectionTo(targ));
    }
  return(1==2);
  }

void Creature::ClearGoals()  {
  int ctr;
  for(ctr=0; ctr<12; ctr++)  {
    if(goal[ctr] != NULL) goal[ctr]->Detach();
    goal[ctr] = NULL;
    }
  progress = 0;
  }

void Creature::GoalAbort()  {
  if(goal[0] == NULL)  return;
  int ctr;
  goal[0]->Detach(); 
  for(ctr=0; ctr<11; ctr++)  goal[ctr] = goal[ctr+1];
  goal[11] = NULL;
  progress = 0;
  }

void Creature::GoalDone()  {
  if(goal[0] == NULL)  return;
  int ctr;
  goal[0]->Finish();
  goal[0]->Detach();
  for(ctr=0; ctr<11; ctr++)  goal[ctr] = goal[ctr+1];
  goal[11] = NULL;
  progress = 0;
  }

void Creature::GoalDoneFirst()  {
  if(goal[0] == NULL)  return;
  int ctr;
  goal[0]->FinishFirst();
  if(goal[0]->IsDone())  {
    goal[0]->Detach();
    for(ctr=0; ctr<11; ctr++)  goal[ctr] = goal[ctr+1];
    goal[11] = NULL;
    }
  progress = 0;
  }

void Creature::UpdateCMap()  {
  if(spells && prayers) rank = RANK_TEMPLAR;
  else if(prayers) rank = RANK_PRIEST;
  else if(spells) rank = RANK_MAGE;
  else rank = RANK_GRUNT;
  image.SetColormap(cremap[gtype][rank][owner->Number()]);
  Changed[thingnum] = 1;
  }
