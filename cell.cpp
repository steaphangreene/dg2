#include <user/engine.h>
#include "map.h"
#include "cell.h"
#include "gmode.h"
#include "creature.h"
#include "struct.h"
#include <string.h>

#define gm gmode[cmode]

extern GMode gmode[10];
extern char cmode;
extern Screen *screen;
extern Map *curmap;

Graphic *Cell::pics[TERRAIN_MAX][3][2][3];

int Cell::GraphicsInitialized = 0;

Cell::~Cell()  {
  }

Cell::Cell(int ter, int alt, int walt)  {
  if(!(GraphicsInitialized))  SetupTerrainGraphics();
  terrain = ter;
  switch(ter)  {
    case(TERRAIN_GRASS): ffeul = 20; fresist = 10; break;
    case(TERRAIN_BRUSH): ffeul = 50; fresist = 20; break;
    case(TERRAIN_FOREST): ffeul = 100; fresist = 40; break;
    default: ffeul = 0; fresist = 32767; break;
    }
  selected = 0;
  exists = 0;
  altitude = alt;
  waltitude = walt;
  viewers = 0;
  mviewers = 0;
  visible = 0;
  discovered = 0;
  mapped = 0;
  near[0] = NULL;
  near[1] = NULL;
  near[2] = NULL;
  near[3] = NULL;
  near[4] = NULL;
  near[5] = NULL;
  lastport = NULL;
  inside = new (Thing*)[2];
  inside[0] = NULL;
  inside[1] = NULL;
  dibs[0] = NULL;
  dibs[1] = NULL;
  location[0] = this;
  location[1] = this;
  type = THING_CELL;
  }

void Cell::SetupTerrainGraphics()  {
  int t;
  Graphic *fireg = new Graphic("graphics/fire.bmp");
  Graphic *smokeg = new Graphic("graphics/smoke.bmp");
  fireg->tcolor = 33;
  smokeg->tcolor = 33;
  *fireg = fireg->DoubleX();
  *smokeg = smokeg->DoubleX();
  for(t=0; t<TERRAIN_MAX; t++)  {
    char *name = NULL;
    switch(t)  {
      case(TERRAIN_VOID):
	name = "graphics/tiles/null.bmp";
	break;
      case(TERRAIN_PATH):
	name = "graphics/tiles/path.bmp";
	break;
      case(TERRAIN_DIRT):
	name = "graphics/tiles/dirt.bmp";
	break;
      case(TERRAIN_GRASS):
	name = "graphics/tiles/grass.bmp";
	break;
      case(TERRAIN_BRUSH):
	name = "graphics/tiles/bushes.bmp";
	break;
      case(TERRAIN_FOREST):
	name = "graphics/tiles/forest.bmp";
	break;
      case(TERRAIN_LOWWATER):
	name = "graphics/tiles/shallow.bmp";
	break;
      case(TERRAIN_WATER):
	name = "graphics/tiles/deep.bmp";
	break;
      case(TERRAIN_OCEAN):
	name = "graphics/tiles/ocean.bmp"; 
	break;
      case(TERRAIN_ROCK):
	name = "graphics/tiles/stone.bmp";
	break;
      case(TERRAIN_SAND):
	name = "graphics/tiles/sand.bmp";
	break;
      case(TERRAIN_ASHES):
	name = "graphics/tiles/ashes.bmp";
	break;
      }
    pics[t][2][1][0] = new Graphic(name);
    *pics[t][2][1][0] = pics[t][2][1][0]->DoubleX();
    pics[t][2][0][0] = new Graphic(pics[t][2][1][0]->Hashed(0));
    pics[t][1][1][0] = new Graphic(pics[t][2][1][0]->HalfSize());
    pics[t][1][0][0] = new Graphic(pics[t][1][1][0]->Hashed(0));
    pics[t][0][1][0] = new Graphic(pics[t][1][1][0]->HalfSize());
    pics[t][0][0][0] = new Graphic(pics[t][0][1][0]->Hashed(0));

    if(t != TERRAIN_VOID)  {
      pics[t][2][1][1] = new Graphic(*pics[t][2][1][0] + *smokeg);
      pics[t][2][1][2] = new Graphic(*pics[t][2][1][0] + *fireg);
      }
    else  {
      pics[t][2][1][1] = new Graphic(*pics[t][2][1][0]);
      pics[t][2][1][2] = new Graphic(*pics[t][2][1][0]);
      }
    pics[t][2][0][1] = new Graphic(pics[t][2][1][0]->Hashed(0));
    pics[t][1][1][1] = new Graphic(pics[t][2][1][1]->HalfSize());
    pics[t][1][0][1] = new Graphic(pics[t][1][1][0]->Hashed(0));
    pics[t][0][1][1] = new Graphic(pics[t][1][1][1]->HalfSize());
    pics[t][0][0][1] = new Graphic(pics[t][0][1][0]->Hashed(0));
    pics[t][2][0][2] = new Graphic(pics[t][2][1][0]->Hashed(0));
    pics[t][1][1][2] = new Graphic(pics[t][2][1][2]->HalfSize());
    pics[t][1][0][2] = new Graphic(pics[t][1][1][0]->Hashed(0));
    pics[t][0][1][2] = new Graphic(pics[t][1][1][2]->HalfSize());
    pics[t][0][0][2] = new Graphic(pics[t][0][1][0]->Hashed(0));
    }
  delete fireg;
  GraphicsInitialized = 1;
  }

Cell *Cell::Next(int dir)  {
  dir %= 12;
  if(!(dir % 2))
    return near[dir/2];
  else if((near[((dir+1)/2)%6] == NULL) || (near[((dir+11)/2)%6] == NULL))
    return NULL;
  else 
    return near[((dir+1)/2)%6]->near[((dir+11)/2)%6];
  }

void Cell::Select()  {
  }

void Cell::ToggleSelect()  {
  }

void  Cell::LinkMe(int dir, Cell *in)  {
  if(((in != NULL) && (in->near[(dir+3)%6] != NULL)) || (near[dir] != NULL))  {
    delete screen;
    printf("Cell Placed Where Cell Exists!!! (Dir=%d, Pos=(%d, %d)).\n",
	(dir+3)%6, in->xcoord, in->ycoord);
    exit(1);
    }
  near[dir] = in;
  if(in != NULL)  in->near[(dir+3)%6] = this;
  }

void Cell::PlaceNear(int indir, Cell *in)  {
  int dir = indir/2;
  LinkMe((dir+3)%6, in);

  LinkMe((dir+4)%6, in->near[(dir+5)%6]);
  if(near[(dir+4)%6] != NULL)
    LinkMe((dir+5)%6, near[(dir+4)%6]->near[dir]);
  if(near[(dir+5)%6] != NULL)
    LinkMe(dir, near[(dir+5)%6]->near[(dir+1)%6]);

  LinkMe((dir+2)%6, in->near[(dir+1)%6]);
  if(near[(dir+2)%6] != NULL)
    LinkMe((dir+1)%6, near[(dir+2)%6]->near[dir]);

  if((near[dir] != NULL) && (near[(dir+1)%6] == NULL))
    LinkMe((dir+1)%6, near[dir]->near[(dir+2)%6]);

  if((near[(dir+1)%6] != NULL) && (near[dir] == NULL))
    LinkMe(dir, near[(dir+1)%6]->near[(dir+5)%6]);
  if((near[dir] != NULL) && (near[(dir+5)%6] == NULL))
    LinkMe((dir+5)%6, near[dir]->near[(dir+4)%6]);

  switch(dir*2)  {
    case (DIR_NE):
	xcoord = in->xcoord+1;
	ycoord = in->ycoord-1;
	break;
    case (DIR_SE):
	xcoord = in->xcoord+1;
	ycoord = in->ycoord+1;
	break;
    case (DIR_NW):
	xcoord = in->xcoord-1;
	ycoord = in->ycoord-1;
	break;
    case (DIR_SW):
	xcoord = in->xcoord-1;
	ycoord = in->ycoord+1;
	break;
    case (DIR_W):
	xcoord = in->xcoord-2;
	ycoord = in->ycoord;
	break;
    case (DIR_E):
	xcoord = in->xcoord+2;
	ycoord = in->ycoord;
	break;
    default:
	delete screen;
	printf("Bogus dir!!! (Dir=%d).\n", dir);
	exit(1);
    }
  exists = 1;
  Changed[thingnum] = 1;
  }

int Cell::IsNextTo(Cell *in)  {
  return((near[0]==in) || (near[1]==in) || (near[2]==in) || (near[3]==in)
	|| (near[4]==in) || (near[5]==in));
  }

void Cell::Place(int x, int y)  {
  xcoord = x;
  ycoord = y;
  exists = 1;
  Changed[thingnum] = 1;
  }

int Cell::XPos()  {
  return((xcoord-gm.xstart)*gm.xstep/2+gm.xorig);
  }

int Cell::YPos()  {
  return((ycoord-gm.ystart)*gm.ystep+gm.yorig);
  }

void Cell::tickme()  {
  Thing::tickme();
/*
  if(ftemp <= fresist)  {
    Changed[thingnum] = 1;
    Waiting[thingnum] = 0;
    return;
    }
  if(ffeul<1)  {
    ftemp = 0;
    fresist = 32767;
    terrain = TERRAIN_ASHES;
    Changed[thingnum] = 1;
    return;
    }
  --ffeul;
  StrikeAllIn(this, 200, ftemp, ftemp<<1);
  if(fresist >= ffeul)  fresist = ffeul-1;
  if(ftemp > ffeul)  ftemp = ffeul;
  Waiting[thingnum] = 8;
  int ctr;
  ctr = (random()%6) << 1;
  if(Next(ctr) != NULL)  {
    Next(ctr)->Heat(5, ftemp);
    Next(ctr)->StrikeAllIn(this, 200, ftemp>>2, ftemp>>1);
    }
*/
  }

void Cell::ReScaleme()  {
  Changed[thingnum] = 1;
  }

void Cell::ReAlignme(int x, int y)  {
  Changed[thingnum] = 1;
  }

void Cell::DrawMyMini()  {
  mapped = 1;
  if((xcoord/2) >= (curmap->XSize()-1) || ycoord >= (curmap->YSize()-1))
    return;
  int xs = curmap->XSize()-1;
  int ys = curmap->YSize()-1;
  int xb = ((xcoord/2)*gm.mxsize)/xs;
  int yb = (ycoord*gm.mysize)/ys;
  int xm = (((xcoord/2)+1)*gm.mxsize)/xs;
  int ym = ((ycoord+1)*gm.mysize)/ys;
  int yp, xp, vt = Terrain();
  for(xp=xb; xp < xm; xp++)  {
    for(yp=yb; yp < ym; yp++)  {
      screen->SetPoint(gm.mxorig+xp, gm.myorig+yp, pics[vt &
	(discovered*255)][gmode[cmode].xstep/32][visible][0]->image[0].uc[0]);
		// **************** 8-bit hardcode...
      }
    }
  }

int Cell::Terrain()  {
  if(waltitude > 12+altitude)  return TERRAIN_OCEAN;
  else if(waltitude > 6+altitude)  return TERRAIN_WATER;
  else if(waltitude > altitude)  return TERRAIN_LOWWATER;
  else return terrain;
  }

int Cell::IsPath()  {
  if(terrain == TERRAIN_PATH)  return (1==1);
  Thing *tmpt = inside[0];
  for(;tmpt != NULL; tmpt = tmpt->Inside(0))  {
    if(tmpt->Type() == THING_STRUCT && 
	((Structure*)tmpt)->StructType() == STRUCT_BRIDGE) return (1==1);
    }
  return (1==2);
  }

void Cell::DrawMe()  {
  curmap->DrawCell(xcoord, ycoord);
//  int x1, x2, y1, y2;
//  x1=((xcoord-gm.xstart)-1)*gm.xstep>>1;
//  x2=x1+gm.xstep;
//  y1=(ycoord-gm.ystart)*gm.ystep-(gm.ystep>>1);
//  y2=y1+gm.ystep;
//  if(x1<0) x1=0;
//  if(y1<0) y1=0;
//  x1+=gm.xorig;
//  x2+=gm.xorig;
//  y1+=gm.yorig;
//  y2+=gm.yorig;
//  if(x2>gm.xedge) x2=gm.xedge;
//  if(y2>gm.yedge) y2=gm.yedge;
//  if((x1>=x2) || (y1>=y2))  return;
////  Exit(1, "Nope, got here!  (%d, %d)->(%d, %d)\n"x1, y1, x2, y2);
//  curmap->Draw(x1, y1, x2, y2);
  }

void Cell::updateme()  {
  if((!mapped) && (discovered))  DrawMyMini();
  if(!(XPos() < gm.xorig || YPos() < gm.yorig || XPos() > gm.xedge
	|| YPos() > gm.yedge))
    DrawMe();
  Changed[thingnum] = 0;
  Thing *tmpt = inside[0];
  int ctr;
  while(tmpt != NULL && tmpt != this)  {
    Changed[((Cell *)tmpt)->thingnum] = 1;
    if(tmpt != ((Cell*)tmpt)->inside[0])  tmpt = ((Cell*)tmpt)->inside[0];
    else tmpt = NULL;
    }
  tmpt = inside[1];
  while(tmpt != NULL && tmpt != this)  {
    Changed[((Cell *)tmpt)->thingnum] = 1;
    if(tmpt != ((Cell*)tmpt)->inside[1])  tmpt = ((Cell*)tmpt)->inside[1];
    else tmpt = NULL;
    }
  if(dibs[0] != NULL && dibs[0] != this)  
	Changed[((Cell *)dibs[0])->thingnum] = 1;
  if(dibs[1] != NULL && dibs[1] != this)
	Changed[((Cell *)dibs[1])->thingnum] = 1;
  for(ctr=0; ctr < spell.Size(); ctr++)  {
    Changed[spell[ctr]] = 1;
    }
  if(ftemp>fresist && Waiting[thingnum] < 1)  Waiting[thingnum] = 1;
  }

void Cell::HalfDisable()  {
  dibs[0] = this;
  inside[0] = this;
  }

void Cell::Disable()  {
  dibs[0] = this;
  inside[0] = this;
  dibs[1] = this;
  inside[1] = this;
  }

Thing *Cell::Owner(int v1)  {
  if(dibs[v1] != this)  return dibs[v1];
  else return NULL;
  }

int Cell::Claim(Thing *in, int alt, int ht)  {
  int up=ht/2, down=ht;
  if(in->Location(0) != NULL)
    if(alt < ((Cell *)in->Location(0)->Location(0))->WaterLevel()) up=ht+1;
  if((WaterDepth() > 0) && alt-down <= WaterLevel()) down+=WaterDepth();
  return Claim(in, alt, ht, up, down);
  }

int Cell::Claim(Thing *in, int alt, int ht, int up, int down)  {
  if(Claimed(in)) return(1==1);
  if(dibs[0] == in || dibs[1] == in)  {
    if(dibs[0] == NULL) dibs[0] = in;
    else if(dibs[1] == NULL) dibs[1] = in;
    else  return (1==2);
    return(1==1);
    }
//Exit(1, "Claiming half-claimed Cell!!!\r\n");
  if(dibs[0] != NULL || dibs[1] != NULL)
    return (1==2);

  if(inside[0] != NULL && inside[0]->Type() == THING_STRUCT &&
    ((Structure*)inside[0])->StructType() == STRUCT_RAMP)
      down += 9;

  if(in->Location(0) != NULL)  {
    if(in->Location(0) != NULL && in->Location(0)->Inside(0) != NULL &&
      in->Location(0)->Inside(0)->Type() == THING_STRUCT &&
      ((Structure*)in->Location(0)->Inside(0))->StructType() == STRUCT_RAMP)
        up += 9;
    }
  Thing *tmpt = this;
  while(((Cell*)tmpt)->inside[0] != NULL)  {
    if(tmpt->Inside(0) != in && tmpt->Inside(1) != in)  {
      if(((Cell*)tmpt)->inside[0] != ((Cell*)tmpt)->inside[1])  return (1==2);
      if(tmpt == ((Cell*)tmpt)->inside[0]) return (1==2);

      if(tmpt->Height() <= alt+up && tmpt->Height() >= alt-down
	  && (tmpt->Type() != THING_CREATURE || ((Cell*)tmpt)->height <= ((ht)/2))
	  && ((Cell*)tmpt->Inside(0))->altitude >= (tmpt->Height()+ht))  {
        dibs[0] = in;  dibs[1] = in;  return (1==1); }
      }
    else  {
      if(tmpt->Inside(0) == NULL || tmpt->Inside(1) == NULL
		|| (tmpt->Inside(0) == tmpt->Inside(1)))  {
	dibs[0] = in;  dibs[1] = in;  return (1==1);
	}
      }
    tmpt = tmpt->Inside(0);
    }
  if(tmpt->Height() > alt+up || tmpt->Height() < alt-down
      || (((Cell*)tmpt)->height > ((ht)/2) && tmpt->Type() == THING_CREATURE))
    return (1==2);
  dibs[0] = in;
  dibs[1] = in;
  return (1==1);
  }

int Cell::ClaimHalf(Thing *in, char cdir, int alt, int ht)  {
  int up=ht/2, down=ht;
  if(in->Location(0) != NULL)
    if(alt < ((Cell *)in->Location(0)->Location(0))->WaterLevel()) up=ht;
  if((WaterDepth() > 0) && alt-down <= WaterLevel()) down+=WaterDepth();
  return ClaimHalf(in, cdir, alt, ht, up, down);
  }

int Cell::ClaimHalf(Thing *in, char cdir, int alt, int ht, int up, int down)  {
  if(ClaimedHalf(in))  return(1==1);
  if(dibs[0] != NULL && dibs[1] != NULL)  return (1==2);

  if(Height() > alt+up || Height() < alt-down)  return (1==2);
  if(inside[0] != NULL && inside[0] != in && inside[1] != NULL
	&& inside[1] != in)  return (1==2);
  if(dibs[0] == NULL) dibs[0] = in;
  else dibs[1] = in;
  return (1==1);
  }

void Cell::UnClaim(Thing *in)  {
//  printf("Unclaiming!\n");
  if(Claimed(in))  {
    dibs[0] = NULL;
    dibs[1] = NULL;
    }
  else  Exit(0, "Unclaiming non-claimed cell!\r\n");
  }

void Cell::UnClaimHalf(Thing *in)  {
//  printf("Unclaiming Half!\n");
  if(dibs[0] == in)  dibs[0] = NULL;
  else if(dibs[1] == in)  dibs[1] = NULL;
  else Exit(1, "Half-Unclaiming a non-claimed cell!!\r\n");
  }

int Cell::Claimed(Thing *in)  {
  return (dibs[0] == in && dibs[1] == in);
  }

int Cell::ClaimedHalf(Thing *in)  {
  return ((dibs[0] == in || dibs[1] == in) && (dibs[0] != dibs[1]));
  }

void Cell::StrikeAllIn(Thing *attacker, int skill, int pdam, int bdam)  {
  Thing *tmpt = location[0]->Inside(0);
  for(; tmpt != NULL; tmpt=tmpt->Inside(0))
    if(tmpt->Type() == THING_CREATURE)
      ((Creature *)tmpt)->Strike((Creature*)attacker, skill, pdam>>1, bdam>>1);
  tmpt = location[0]->Inside(1);
  for(; tmpt != NULL; tmpt=tmpt->Inside(1))
    if(tmpt->Type() == THING_CREATURE)
      ((Creature *)tmpt)->Strike((Creature*)attacker, skill, pdam>>1, bdam>>1);
  }

int Cell::HaveMaterial(int type)  {
  return contains[type];        
  }

int Cell::TakeMaterials(int type, int ammt)  {
  if(contains[type] >= ammt)  {
    contains[type] -= ammt;
    return ammt;
    }
  else  {   
    int tmp;
    tmp = contains[type];
    contains[type] = 0;
    return tmp;
    }
  }

int Cell::AddMaterials(int type, int ammt)  {
  contains[type] += ammt;
  return 0;
  }

IntList Cell::CellsAtRange(int rng1, int rng2, int fac, int arcsz)  {
  IntList ret;
/*
  Cell *cur[6];
  int rng, ctr, ctr2;
  for(ctr=0; ctr<6; ctr++)  cur[ctr] = this;
  for(rng=1; rng<=rng2; rng++)  {
    for(ctr=0; ctr<6; ctr++)  {
      if(cur[ctr] != NULL) cur[ctr] = cur[ctr]->Next(ctr<<1);
      }
    if(rng >= rng1)  {
      for(ctr=0; ctr<6; ctr++)  {
	Cell *tmpc = cur[ctr];
	if(tmpc != NULL)  {
	  ret += tmpc->Number();
	  for(ctr2=1; ctr2<rng && tmpc != NULL; ctr2++)  {
	    tmpc = tmpc->Next(((ctr<<1)+4)%12);
	    if(tmpc != NULL) ret += tmpc->Number();
	    }
	  }
	}
      }
    }
*/
  int ctrx, ctry;
  Cell *tmpc;
  for(ctry=0; ctry<=rng2; ctry++)  {
    if(ctry<rng1)  {
      for(ctrx=(rng1<<1)-ctry; ctrx<=(rng2<<1)-ctry; ctrx+=2)  {
	tmpc = curmap->CellAt((xcoord+ctrx)>>1, ycoord+ctry);
	if(tmpc != NULL) ret += tmpc->Number();
	tmpc = curmap->CellAt((xcoord-ctrx)>>1, ycoord+ctry);
	if(tmpc != NULL) ret += tmpc->Number();
	if(ctry > 0)  {
	  tmpc = curmap->CellAt((xcoord+ctrx)>>1, ycoord-ctry);
	  if(tmpc != NULL) ret += tmpc->Number();
	  tmpc = curmap->CellAt((xcoord-ctrx)>>1, ycoord-ctry);
	  if(tmpc != NULL) ret += tmpc->Number();
	  }
	}
      }
    else  {
      for(ctrx=ctry-(rng2<<1); ctrx<=(rng2<<1)-ctry; ctrx+=2)  {
	tmpc = curmap->CellAt((xcoord+ctrx)>>1, ycoord+ctry);
	if(tmpc != NULL) ret += tmpc->Number();
	if(ctry > 0)  {
	  tmpc = curmap->CellAt((xcoord+ctrx)>>1, ycoord-ctry);
	  if(tmpc != NULL) ret += tmpc->Number();
	  }
	}
      }
    }

  return ret;
  }
