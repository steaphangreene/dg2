#include <user/screen.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void DrawLine(int, int, int, int, char);
void DrawBox(int, int, int, int, char);
void Fill(int, int, char, char);
void PutTile(int, int, char);
void UsageExit();
void Save();
void Load();
void DisplayMapFrom(FILE *);

unsigned char *tile2let = (unsigned char *)"-PDGBFwWORSCA\0\0\0";
unsigned char *tile2col;
unsigned char *let2tile;
unsigned char *col2tile;

Screen *screen;
Mouse *mouse;
Window mapw;


int xsize, ysize;
char *filename;

int main(int argc, char **argv)  {
  FILE *mapfl = NULL;
  { char *tmp, xe=0, ye=0;
    if(argc < 2)  UsageExit();
    filename = argv[1];
    mapfl=fopen(filename, "rb");
    if(mapfl == NULL)  {
      if(argc > 2)  {
	xsize=strtol(argv[2], &tmp, 10);
	if(xsize < 64 || xsize > 1024)  UsageExit();
	if(*tmp != 'x' && *tmp != 'X')  UsageExit();
	ysize=strtol(&tmp[1], &tmp, 10);
	if(ysize < 64 || xsize > 1024)  UsageExit();
	if(*tmp != '\0')  UsageExit();
	}
      else  {
	printf("Mapfile: \"%s\" not found!\n", filename);
	UsageExit();
        }
      }
    else  {
      fscanf(mapfl, "%dx%d\n", &xsize, &ysize);
      }
    xsize >>= 1;
    while(xsize)  {
      xsize >>= 1;
      xe++;
      }
    ysize >>= 1;
    while(ysize)  {
      ysize >>= 1;
      ye++;
      }
    xsize = (1 << xe);
    ysize = (1 << ye);
    }
  int ctr;
  screen = new Screen(1280, 1024);
  User user;
  mouse = new Mouse;
  int quit=0;
  screen->FullScreenBMP("backg.bmp");
  screen->GetBMPPalette("backg.bmp");
  screen->SetPaletteEntry(255, 255, 255, 255);
  screen->SetPaletteEntry(254, 0, 0, 0);
  screen->RefreshFull();
  screen->ShowScreen();
  mouse->ShowCursor();

  mapw = screen->NewWindow(256, 0, 1280, 1024);
  Window contw = screen->NewWindow(0, 0, 256, 1024);
  mouse->SetBehavior(MOUSE_CLICK, MOUSE_CLICK, MOUSE_CLICK);
  mouse->SetWindowBehavior(mapw, MOUSE_DRAW, MOUSE_CLICK, MOUSE_BOX);
  mouse->SetWindowBehavior(contw, MOUSE_CLICK, MOUSE_CLICK, MOUSE_CLICK);

  int xx, xy;
  Graphic xg("x.bmp");
  xg.FindTrueCenter();
  Sprite xs;
  xs.SetImage(xg);

  tile2col = new unsigned char[64];
  for(ctr=0; ctr<13; ctr++)  {
    unsigned char *tmpc = screen->GetBlock32(((ctr & 3) << 1),
		((ctr >> 2) << 6) + 32);
    tile2col[ctr]= tmpc[16];
    }
  col2tile = new unsigned char[256];
  for(ctr=0; ctr<13; ctr++)  {
    col2tile[tile2col[ctr]] = ctr;
    }  
  let2tile = new unsigned char[256];
  for(ctr=0; tile2let[ctr] != '\0'; ctr++)  {
    let2tile[tile2let[ctr]] = ctr;
    }  

  UserAction curact;
  int curtile=0;
  int lastx=-1, lasty=-1, color=0;

  if(mapfl != NULL)  {
    DisplayMapFrom(mapfl);
    fclose(mapfl);
    }

  while(!quit)  {
    curact = user.Action();
    if(curact.Type() == -1);
    else if(curact.Type() == USERACTION_MOUSE && curact.ButtonPressed() == 1
	&& curact.Win() == mapw) {
      if(lastx >= 0)  {
	DrawLine(lastx, lasty, curact.Startx(), curact.Starty(), color);
	lastx = curact.Startx();
	lasty = curact.Starty();
	}
      else  {
        PutTile(curact.Startx(), curact.Starty(), color);
	lastx = curact.Startx();
	lasty = curact.Starty();
	}
      }
    else {
      lastx = -1;
      if(curact.Type() == USERACTION_MOUSE && curact.ButtonPressed() == 2
		&& curact.Win() == mapw) {
	DrawBox(curact.Startx(), curact.Starty(), curact.Endx(),
		curact.Endy(), color);
	}
      else  if(curact.Type() == USERACTION_MOUSE && curact.ButtonPressed() == 3
		&& curact.Win() == mapw) {
	IntList tmpl = screen->EraseWindowSprites(mapw);
	int x = curact.Startx();
	int y = curact.Starty();
	char orc = screen->GetPoint(x, y);
	Fill(x, y, color, orc);
	screen->RedrawSprites(tmpl);
	}
      else if(curact.Type() == USERACTION_KEYRELEASED
		&& (curact.Key() == SCAN_Q || curact.Key() == SCAN_ESC)) {
	quit = 1;
	}
      else if(curact.Type() == USERACTION_KEYRELEASED
		&& curact.Key() == SCAN_S)  {
	Save();
	}
      else if(curact.Type() == USERACTION_KEYRELEASED
		&& curact.Key() == SCAN_L)  {
	Load();
	}
      else  if(curact.Type() == USERACTION_MOUSE && curact.Win() == contw)  {
	curtile = (((curact.Starty()) >> 6)  << 2) + ((curact.Startx()) >> 6);
	}
      }
    if(!mouse->AButton())  lastx=-1;
    {
      int tmpx=((curtile & 3) << 6) + 32;
      int tmpy=((curtile >> 2) << 6) + 32;
      if(tmpx != xx || tmpy != xy)  xs.Move(tmpx, tmpy);
      xx = tmpx; xy = tmpy;
      }
    { unsigned char *tmpc;
      tmpc = screen->GetBlock32(((curtile & 3) << 1),
		((curtile >> 2) << 6) + 32);
      color = tmpc[16];
      }
    screen->Refresh();
    }
  Save();
  delete mouse;
  delete screen;
  }

void UsageExit()  {
  printf("USAGE:\tmapedit <filename> [MxN]\n");
  printf("\tfilename must conform to 8.3 dos crap.\n");
  printf("\tN and M must be between 64 and 1024.\n");
  exit(1);
  }

void Fill(int x, int y, char c, char orc)  {
  char lorc = screen->GetPoint(x, y);
  if(lorc != orc)  {
    return;
    }
  screen->BSetPoint(x, y, c);
  if(x < 1279)
    Fill(x+1, y, c, orc);
  if(x > 0)
    Fill(x-1, y, c, orc);
  if(y < 1023)
    Fill(x, y+1, c, orc);
  if(y > 0)
    Fill(x, y-1, c, orc);
  }

void DrawLine(int x1, int y1, int x2, int y2, char c)  {
  int xinc = 1, yinc = 1;
  if(x2<x1)  xinc=-1;
  if(y2<y1)  yinc=-1;
  int ctrx, ctry;
  int xd, yd;
  xd = (x2-x1)*xinc;
  yd = (y2-y1)*yinc;
  if(xd == 0)  {
    for(ctry=y1; ctry != (y2+yinc); ctry+=yinc)  {
      PutTile(x1, ctry, c);
      }
    }
  else if(yd == 0)  {
    for(ctrx=x1; ctrx != (x2+xinc); ctrx+=xinc)  {
      PutTile(ctrx, y1, c);
      }
    }
  else if(xd > yd)  {
    for(ctrx=x1; ctrx != (x2+xinc); ctrx+=xinc)  {
      if((xinc*yinc)>0)  ctry = y1+(((ctrx-x1)*yd)/xd);
      else  ctry = y1-(((ctrx-x1)*yd)/xd);
      PutTile(ctrx, ctry, c);
      }
    }
  else  {
    for(ctry=y1; ctry != (y2+yinc); ctry+=yinc)  {
      if((xinc*yinc)>0)  ctrx = x1+(((ctry-y1)*xd)/yd);
      else  ctrx = x1-(((ctry-y1)*xd)/yd);
      PutTile(ctrx, ctry, c);
      }
    }
  }

void DrawBox(int x1, int y1, int x2, int y2, char c)  {
  int xstep = 1024/xsize;
  int ystep = 1024/ysize;
  int xinc = 1, yinc = 1;
  if(x2<x1)  xinc=-1;
  if(y2<y1)  yinc=-1;
  int ctrx, ctry;
  for(ctrx=x1; xinc*ctrx < xinc*(x2+xinc); ctrx+=(xinc*(xstep/2)))  {
    for(ctry=y1; yinc*ctry < yinc*(y2+yinc); ctry+=(yinc*ystep))  {
      PutTile(ctrx, ctry, c);
      }
    }
  }

void PutTile(int x, int y, char c)  {
  if(xsize == 1024 && ysize == 1024)  screen->BSetPoint(x, y, c);
  else  {
    int xstep = 1024/xsize;
    int ystep = 1024/ysize;
    int x1=(x/xstep)*xstep, y1=(y/ystep)*ystep;
    int x2=(x/xstep+1)*xstep, y2=(y/ystep+1)*ystep;
    if((y/ystep) % 2 == 0)  {
      x1=((x+(xstep/2))/xstep)*xstep-(xstep/2);
      x2=((x+(xstep/2))/xstep+1)*xstep-(xstep/2);
      if(x1 < screen->WindowXStart(mapw))  x1 = screen->WindowXStart(mapw);
      if(x2 >= screen->WindowXEnd(mapw))  x2 = screen->WindowXEnd(mapw);
      }
    int ctrx, ctry;
    for(ctrx=x1; ctrx < x2; ctrx++)  {
      for(ctry=y1; ctry < y2; ctry++)  {
        screen->BSetPoint(ctrx, ctry, c);
        }
      }
    }
  }

void Save()  {
  int xstep = 1024/xsize;
  int ystep = 1024/ysize;
  int ctrx, ctry;
  mouse->SetPosition(0, 0);
  FILE *sf = fopen(filename, "wb");
  fprintf(sf, "%dx%d\n", xsize+1, ysize+1);
  for(ctry=0; ctry<ysize; ctry++)  {
    for(ctrx=0; ctrx<xsize; ctrx++)  {
      fprintf(sf, "%c",
	tile2let[col2tile[screen->GetPoint(ctrx*xstep+256, ctry*ystep)]]);
      }
    fprintf(sf, "%c\n",
	tile2let[col2tile[screen->GetPoint(1279, ctry*ystep+1)]]);
    }
  fclose(sf);
  }

void Load()  {
  int xe=0, ye=0;
  FILE *mapfl = fopen(filename, "rb");
  if(mapfl == NULL)  {
    delete screen;
    printf("Error opening map file for reading.\n");
    exit(1);
    }
  fscanf(mapfl, "%dx%d\n", &xsize, &ysize);
  xsize >>= 1;
  while(xsize)  {
    xsize >>= 1;
    xe++;
    }
  ysize >>= 1;
  while(ysize)  {
    ysize >>= 1;
    ye++;
    }
  xsize = (1 << xe);
  ysize = (1 << ye);
  DisplayMapFrom(mapfl);
  fclose(mapfl);
  }

void DisplayMapFrom(FILE *infl)  {
  int xstep = 1024/xsize;
  int ystep = 1024/ysize;
  int ctrx, ctry;
  mouse->SetPosition(0, 0);
  screen->EraseWindowBackground(mapw);
  unsigned char tmpc;
  for(ctry=0; ctry<ysize; ctry++)  {
    for(ctrx=0; ctrx<xsize; ctrx++)  {
      fscanf(infl, "%c", &tmpc);
      PutTile(ctrx*xstep+256, ctry*ystep, tile2col[let2tile[tmpc]]);
      }
    fscanf(infl, "%c\n", &tmpc);
    PutTile(1279, ctry*ystep, tile2col[let2tile[tmpc]]);
    }
  }


