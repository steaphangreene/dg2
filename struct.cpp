#include <user/screen.h>
#include "struct.h"
#include "cell.h"
#include "gmode.h"

#define gm gmode[cmode]

extern Window mainw;
extern GMode gmode[10];
extern char cmode;

Graphic *Structure::stgr[STRUCT_MAX][MATERIAL_MAXBUILD][3][3];

int Structure::graphicsinitialized = 0;

Structure::Structure(int Type, int Material)  {
  if(!graphicsinitialized)  InitGraphics();
  struct_type = Type;
  material = Material;
  if(Type == STRUCT_BRIDGE || Type == STRUCT_RAMP) height = 1;
  else height = 10;
  location[0] = NULL;
  location[1] = NULL;
  inside = new (Thing*)[2];
  inside[0] = NULL;
  inside[1] = NULL;
  type = THING_STRUCT;
  ClaimSprite(image.SpriteNumber());
  image.SetWindow(mainw);
  discovered = 0;
  }

Structure::~Structure()  {
  if(location[0] != NULL)  location[0]->Leave(this);
  location[0] = NULL;
  delete inside;
  UnclaimSprite(image.SpriteNumber());
  image.Erase();
  }

int Structure::Place(Cell *targ)  {
  int alt=-100000, malt = 0;
  if(stgr[struct_type][material][2][0] == NULL)  return 0;
  switch(struct_type)  {
    case(STRUCT_RAMP):
    case(STRUCT_WALL):  {
      if(targ->Terrain() ==TERRAIN_LOWWATER || targ->Terrain() ==TERRAIN_WATER
	|| targ->Terrain() == TERRAIN_OCEAN) return 0;
      }break;
    case(STRUCT_BRIDGE):  {
      int ctr;
      flying = 1;
      for(ctr=0; ctr<12 && alt<=-100000; ctr+=2)  {
	Thing *tmpt = targ->Next(ctr)->Inside(0);
	while(tmpt != NULL)  {
	  if(tmpt->Type() == THING_STRUCT &&
		((Structure*)tmpt)->StructType() == STRUCT_BRIDGE)  {
	    alt = ((Structure*)tmpt)->altitude;
//	    printf("Set relative bridge alt to %d.\r\n", alt);
	    }
	  tmpt = tmpt->Inside(0);
	  }
	}
      if(alt <= -100000 && targ->Terrain() !=TERRAIN_LOWWATER
		&& targ->Terrain() !=TERRAIN_WATER
		&& targ->Terrain() != TERRAIN_OCEAN)  {
	Thing *tmpt = targ;
	while(tmpt->Inside(0) != NULL)  tmpt = tmpt->Inside(0);
	if(tmpt->Type() == THING_CELL || tmpt->Type() == THING_STRUCT)  {
	  alt = tmpt->Height();
	  break;
	  }
	return 0;
	}
      if(alt <= -100000) return 0;
      }break;
    }
  if(alt <= -100000) alt = targ->Height();
  else malt = alt-targ->Height();
  if(malt < 0) return 0;

//  printf("Attempting to claim for struct!\r\n");
  if(!(targ->Claim(this, alt, height, 0, malt)))  return 0;
//  printf("Attempting to enter for struct!\r\n");
  if(!(targ->Enter(this, alt, height, 0, malt)))  {
    targ->UnClaim(this);
    return 0;
    }
//  printf("Success!\r\n");
  targ->UnClaim(this);
  location[0] = targ;
  Changed[thingnum] = 1;
  if(struct_type == STRUCT_BRIDGE)  {
    altitude = alt;
//    printf("Placing Struct at altitude %d\r\n", altitude);
    }
  return 1;
  }

int Structure::Damage(int, int);

void Structure::Select()  {}

void Structure::Deselect()  {}

void Structure::ToggleSelect()  {}

void Structure::ReScaleme()  {}

void Structure::ReAlignme(int, int)  {}

void Structure::tickme()  { Waiting[thingnum]=0; }

void Structure::updateme()  { 
  Changed[thingnum]=0;
  if(((Cell *)location[0])->InView())  {
    int x, y;
    x = Location(0)->XPos(); 
    y = Location(0)->YPos();
    image.UseImage(stgr[struct_type][material][gm.xstep/32][0]);
    image.SetPriority(100000-(Height()));
    image.Move(x, y);
    discovered = 1;
    }
  else if(discovered)  {
    int x, y;
    x = Location(0)->XPos(); 
    y = Location(0)->YPos();
    image.UseImage(stgr[struct_type][material][gm.xstep/32][1+(gm.xstep==32
	|| (gm.xstep==16 && (location[0]->XCoord() & 1) == 0))]);
    image.SetPriority(100000-(Height()));
    image.Move(x, y);
    discovered = 1;
    }
  else image.Erase();
  }

void Structure::InitGraphics()  {
  if(graphicsinitialized)  return;
  int ctr, ctr2, ctr3;
  graphicsinitialized = 1;

  for(ctr=0; ctr<STRUCT_MAX; ctr++)  {
    for(ctr2=0; ctr2<MATERIAL_MAXBUILD; ctr2++)  {
      stgr[ctr][ctr2][2][0] = NULL;
      }
    }
  stgr[STRUCT_WALL][MATERIAL_ROCK][2][0] =
	new Graphic("graphics/structs/wall.bmp");
  stgr[STRUCT_RAMP][MATERIAL_ROCK][2][0] =
	new Graphic("graphics/structs/ramp.bmp");
  stgr[STRUCT_BRIDGE][MATERIAL_WOOD][2][0] =
	new Graphic("graphics/structs/bridge.bmp");
  for(ctr=0; ctr<STRUCT_MAX; ctr++)  {
    for(ctr2=0; ctr2<MATERIAL_MAXBUILD; ctr2++)  {
      if(stgr[ctr][ctr2][2][0] != NULL)  {
	stgr[ctr][ctr2][1][0] =
	  new Graphic(stgr[ctr][ctr2][2][0]->Scaled(0.5));
	stgr[ctr][ctr2][0][0] =
	  new Graphic(stgr[ctr][ctr2][2][0]->Scaled(0.25));
	for(ctr3=0; ctr3<3; ctr3++)  {
	  stgr[ctr][ctr2][ctr3][1] =
		new Graphic(stgr[ctr][ctr2][ctr3][0]->Hashed());
	  stgr[ctr][ctr2][ctr3][2] =
		new Graphic(stgr[ctr][ctr2][ctr3][0]->OffHashed());
	  stgr[ctr][ctr2][ctr3][0]->FindTrueCenter();
	  stgr[ctr][ctr2][ctr3][1]->FindTrueCenter();
	  stgr[ctr][ctr2][ctr3][2]->FindTrueCenter();
	  }
	}
      }
    }
  }
