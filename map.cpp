#include "../user/screen.h"
#include <stdio.h>
#include "gmode.h"
#include "map.h"

#define gm gmode[cmode]

extern GMode gmode[10];
extern char cmode;
extern Screen *screen;
extern Panel mainp;

void Map::DrawCell(int X, int Y)  {
  int x, y, xb, yb;
  Cell *curc;
  x = gm.xorig + (X-gm.xstart)*gm.xstep/2 - gm.xstep/2;
  y = gm.yorig + (Y-gm.ystart)*gm.ystep - gm.ystep/2;
  xb = (X%8) * gm.xstep/2;
  yb = (Y%4) * gm.ystep;
  curc = cells[X>>1][Y];
  screen->DrawPartialGraphic(*(Cell::pics
	[(curc->discovered*255) & curc->Terrain()]
	[gmode[cmode].xstep>>5][curc->visible]
	[(curc->ftemp > curc->fresist)+(curc->ftemp > 0)]),
	x, y, xb, yb, gm.xstep, gm.ystep, mainp);
  }

void Map::Draw(int X1, int Y1, int X2, int Y2)  {
  X1=Y1; //Function Obsolete?
  X2=Y2;
  }

void Map::Draw()  {
  Draw(gm.xorig, gm.yorig, gm.xedge, gm.yedge);
  }

void Map::ReScale()  {
  Draw();
  int ctrx, ctry;
  for(ctry=gm.ystart; ctry<=(gm.ystart+((gm.yedge-gm.yorig)/gm.ystep)); ctry++){
    for(ctrx=gm.xstart/2; 
	ctrx<=((gm.xstart/2)+((gm.xedge-gm.xorig)/gm.xstep)-(ctry%2));
	ctrx++){
      cells[ctrx][ctry]->ReScaleme();
      }
    }
  }

void Map::ReAlign(int x, int y)  {
  int ctrx, ctry;
  if(((x<<2)>=(gm.xsize/gm.xstep)) || ((y<<2)>=(gm.ysize/gm.ystep)) ||
	((x<<2)<=(-(gm.xsize/gm.xstep))) || ((y<<2)<=(-(gm.ysize/gm.ystep)))) {
    Draw();
    for(ctry=gm.ystart; ctry<=(gm.ystart+((gm.yedge-gm.yorig)/gm.ystep)); ctry++){
      for(ctrx=gm.xstart/2; 
		ctrx<=((gm.xstart/2)+((gm.xedge-gm.xorig)/gm.xstep)-(ctry%2));
		ctrx++){
	cells[ctrx][ctry]->ReAlignme(x, y);
	}
      }
    return;
    }
  if(y<0)  {
    Draw(gm.xorig, gm.yorig, gm.xedge, gm.yorig-(gm.ystep*y));
    for(ctry=gm.ystart; ctry<=(gm.ystart-(y<<2)); ctry++){
      for(ctrx=gm.xstart/2; 
		ctrx<=((gm.xstart/2)+((gm.xedge-gm.xorig)/gm.xstep)-(ctry%2));
		ctrx++){
	cells[ctrx][ctry]->ReAlignme(x, y);
	}
      }
    }
  else if(y>0)  {
    Draw(gm.xorig, gm.yedge-(gm.ystep*y), gm.xedge, gm.yedge);
    for(ctry=(gm.ystart+((gm.yedge-gm.yorig)/gm.ystep)-(y<<2));
	ctry<=(gm.ystart+((gm.yedge-gm.yorig)/gm.ystep)); ctry++){
      for(ctrx=gm.xstart/2;
		ctrx<=((gm.xstart/2)+((gm.xedge-gm.xorig)/gm.xstep)-(ctry%2));
		ctrx++){
	cells[ctrx][ctry]->ReAlignme(x, y);
	}
      }
    }
  if(x<0)  {
    for(ctry=gm.ystart; ctry<=(gm.ystart+((gm.yedge-gm.yorig)/gm.ystep)); ctry++){
      for(ctrx=gm.xstart/2; ctrx<=((gm.xstart/2)-(x<<2))-(ctry%2); ctrx++){
	cells[ctrx][ctry]->ReAlignme(x, y);
	}
      }
    }
  else if(x>0)  {
    for(ctry=gm.ystart; ctry<=(gm.ystart+((gm.yedge-gm.yorig)/gm.ystep)); ctry++){
      for(ctrx=(gm.xstart/2)+((gm.xedge-gm.xorig)/gm.xstep)-(x<<2);
		ctrx<=(gm.xstart/2)+((gm.xedge-gm.xorig)/gm.xstep)-(ctry%2);
		ctrx++){
	cells[ctrx][ctry]->ReAlignme(x, y);
	}
      }
    }
  }

Map::Map(char *fn)  {
  int ctr, ctr2;
  FILE *mapfl = fopen(fn, "rb");
  if(mapfl == NULL)  {
    U2_Exit(1, "Mapfile: \"%s\" not found!\n", fn);
    }
  fscanf(mapfl, "%dx%d\n", &MAP_XSIZE, &MAP_YSIZE);
  cells = new Cell**[MAP_XSIZE];
  if(cells == NULL)  U2_Exit(1, "Out of Memory for Total Cells!!!\r\n");
  for(ctr=0; ctr<MAP_XSIZE; ctr++)  {
    cells[ctr] = new Cell*[MAP_YSIZE];
    if(cells[ctr] == NULL)  U2_Exit(1, "Out of Memory for Cells!!!\r\n");
    }
  for(ctr2=0; ctr2<MAP_YSIZE; ctr2++)  {
    for(ctr=0; ctr<(MAP_XSIZE - (ctr2 & 1)); ctr++)  {
      char tert;
      fscanf(mapfl, "%c", &tert);
      switch(tert)  {
	case('P'):  cells[ctr][ctr2] = new Cell(TERRAIN_PATH, 1, 0);
	  break;
	case('D'):  cells[ctr][ctr2] = new Cell(TERRAIN_DIRT, 1, 0);
	  break;
	case('G'):  cells[ctr][ctr2] = new Cell(TERRAIN_GRASS, 1, 0);
	  break;
	case('S'):  cells[ctr][ctr2] = new Cell(TERRAIN_SAND, 1, 0);
	  break;
	case('R'):  cells[ctr][ctr2] = new Cell(TERRAIN_ROCK, 1, 0);
	  break;
	case('B'):  cells[ctr][ctr2] = new Cell(TERRAIN_BRUSH, 1, 0);
	  break;
	case('F'):  cells[ctr][ctr2] = new Cell(TERRAIN_FOREST, 1, 0);
	  break;
	case('w'):  cells[ctr][ctr2] = new Cell(TERRAIN_DIRT, -2, 0);
	  break;
	case('W'):  cells[ctr][ctr2] = new Cell(TERRAIN_DIRT, -8, 0);
	  break;
	case('O'):  cells[ctr][ctr2] = new Cell(TERRAIN_DIRT, -20, 0);
	  break;
	case('V'):  cells[ctr][ctr2] = new Cell(TERRAIN_VOID, 0, 0);
	  break;
	default:  printf("Encountered bad tile \"%c\" (%d) in mapfile!\r\n",
			tert, tert);;
	  break;
	}
      if(cells[ctr][ctr2] == NULL)  U2_Exit(1, "Out of Memory for Cell!!!\r\n");
      }
    if(ctr2 & 1)  {
      cells[MAP_XSIZE-1][ctr2] = NULL;
      fscanf(mapfl, "%*c");
      }
    fscanf(mapfl, "\n");
    }
  fclose(mapfl);
  cells[0][0]->Place(0,0);
  cells[0][1]->PlaceNear(DIR_SE, cells[0][0]);
  for(ctr2=2; ctr2<MAP_YSIZE; ctr2+=2)  {
    cells[0][ctr2]->PlaceNear(DIR_SW, cells[0][ctr2-1]);
    cells[0][ctr2]->HalfDisable();
    if(ctr2+1 < MAP_YSIZE)  cells[0][ctr2+1]->PlaceNear(DIR_SE, cells[0][ctr2]);
    }
  for(ctr2=0; ctr2<MAP_YSIZE; ctr2++)  {
    for(ctr=1; ctr<(MAP_XSIZE - (ctr2 % 2)); ctr++)  {
      cells[ctr][ctr2]->PlaceNear(DIR_E, cells[ctr-1][ctr2]);
      }
    }
  for(ctr=0; ctr < MAP_XSIZE; ctr++)  {
    cells[ctr][0]->Disable();
    cells[ctr][MAP_YSIZE-1]->Disable();
    }
  for(ctr2=0; ctr2<MAP_YSIZE; ctr2+=2)  cells[MAP_XSIZE-1][ctr2]->HalfDisable();
  }

Map::~Map()  {
  int ctr;
//  for(ctr=0; ctr<MAP_XSIZE; ctr++)
//    for(ctr2=0; ctr2<MAP_YSIZE; ctr2++)
//      if(cells[ctr][ctr2] != NULL)  delete cells[ctr][ctr2];
  for(ctr=0; ctr<MAP_XSIZE; ctr++)
    delete cells[ctr];
  delete cells;
  }

Cell *Map::CellAt(int x, int y)  {
  if(x<0 || x >= MAP_XSIZE || y < 0 || y >= MAP_YSIZE) return NULL;
  return cells[x][y];
  }

void Map::Screen2Cell(int &x, int &y)  {
  y+=(gm.ystep>>1);
  y-=gm.yorig;
  y/=gm.ystep;
  y+=gm.ystart;
  if((y % 2) == 0)  x+=(gm.xstep>>1);
  x-=gm.xorig;
  x/=gm.xstep;
  x+=(gm.xstart>>1);
  if(!(x >= 0 && y >= 0 && x < MAP_XSIZE && y < MAP_YSIZE))  { x=-1; y=-1; }
  }

void Map::Mini2Cell(int &x, int &y)  {
  y-=gm.myorig;
  y*=(MAP_YSIZE-1);
  y/=gm.mysize;
  x-=gm.mxorig;
  x*=(MAP_XSIZE-1);
  x/=gm.mxsize;
  if(!(x >= 0 && y >= 0 && x < MAP_XSIZE && y < MAP_YSIZE))  { x=-1; y=-1; }
  }

void Map::update()  {
  }
