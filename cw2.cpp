#include <time.h>
#include <string.h>
#include <user/net.h>
#include <user/screen.h>
#include "action.h"
#include "thing.h"
#include "creature.h"
#include "cell.h"
#include "map.h"
#include "player.h"
#include "gmode.h"
#include "game.h"
#include "cw2.h"

Screen *screen = NULL;
User *user = NULL;
Network net;
char multi;

GMode gmode[10];
char cmode = 2;

DigSample *Death;
DigSample *Punch;
DigSample *Klang1;
DigSample *Klang2;
DigSample *Klang3;
DigSample *BowFire;
DigSample *BowHit;
DigSample *AxeThrow;
DigSample *ButtonClick;

int GetNetPlayers();
int JoinNetGame();
int StartNetGame();

Mouse *mouse = NULL;

char **ARGV;
int ARGC;

int main(int argc, char **argv)  {
  debug_position=10;
  ARGV = argv;
  ARGC = argc;
//  SoundCard SB(22050);
//  Death = new DigSample("sounds/die.wav");
//  Punch = new DigSample("sounds/punch.wav");
//  Klang1 = new DigSample("sounds/klang1.wav");
//  Klang2 = new DigSample("sounds/klang2.wav");
//  Klang3 = new DigSample("sounds/klang3.wav");
//  BowFire = new DigSample("sounds/bowfire.wav");
//  BowHit = new DigSample("sounds/bowhit.wav");
//  AxeThrow = new DigSample("sounds/axethrow.wav");
//  ButtonClick = new DigSample("sounds/bowHit.wav");

  Graphic NormalMouseG("graphics/pointers/normal.bmp");

  gmode[0].x = 320;
  gmode[0].y = 200;
  gmode[0].xedge = 320;
  gmode[0].yedge = 190;
  gmode[0].xorig = 64;
  gmode[0].yorig = 10;
  gmode[0].xsize = 256;
  gmode[0].ysize = 180;
  gmode[0].xstep = 16;
  gmode[0].ystep = 15;
  gmode[0].xstart = 0;
  gmode[0].ystart = 0;
  gmode[0].mxedge = 60;
  gmode[0].myedge = 78;
  gmode[0].mxorig = 4;
  gmode[0].myorig = 22;
  gmode[0].mxsize = 56;
  gmode[0].mysize = 56;
  gmode[0].bx = 7;
  gmode[0].by = 7;
  gmode[0].mxb = 128;
  gmode[0].myb = 20;
  gmode[0].mxe = 256;
  gmode[0].mye = 180;

  gmode[1].x = 640;
  gmode[1].y = 480;
  gmode[1].xedge = 640;
  gmode[1].yedge = 480;
  gmode[1].xorig = 128;
  gmode[1].yorig = 0;
  gmode[1].xsize = 512;
  gmode[1].ysize = 480;
  gmode[1].xstep = 32;
  gmode[1].ystep = 30;
  gmode[1].xstart = 0;
  gmode[1].ystart = 0;
  gmode[1].mxedge = 120;
  gmode[1].myedge = 156;
  gmode[1].mxorig = 8;
  gmode[1].myorig = 44;
  gmode[1].mxsize = 112;
  gmode[1].mysize = 112;
  gmode[1].bx = 14;
  gmode[1].by = 14;
  gmode[1].mxb = 256;
  gmode[1].myb = 80;
  gmode[1].mxe = 512;
  gmode[1].mye = 400;

  gmode[2].x = 800;
  gmode[2].y = 600;
  gmode[2].xedge = 800;
  gmode[2].yedge = 600;
  gmode[2].xorig = 160;
  gmode[2].yorig = 0;
  gmode[2].xsize = 640;
  gmode[2].ysize = 600;
  gmode[2].xstep = 32;
  gmode[2].ystep = 30;
  gmode[2].xstart = 0;
  gmode[2].ystart = 0;
  gmode[2].mxedge = 144;
  gmode[2].myedge = 188;
  gmode[2].mxorig = 16;
  gmode[2].myorig = 60;
  gmode[2].mxsize = 128;
  gmode[2].mysize = 128;
  gmode[2].bx = 18;
  gmode[2].by = 18;
  gmode[2].mxb = 256;
  gmode[2].myb = 80;
  gmode[2].mxe = 512;
  gmode[2].mye = 400;

  gmode[3].x = 1024;
  gmode[3].y = 768;
  gmode[3].xedge = 1024;
  gmode[3].yedge = 744;
  gmode[3].xorig = 256;
  gmode[3].yorig = 24;
  gmode[3].xsize = 768;
  gmode[3].ysize = 720;
  gmode[3].xstep = 32;
  gmode[3].ystep = 30;
  gmode[3].xstart = 0;
  gmode[3].ystart = 0;
  gmode[3].mxedge = 120;
  gmode[3].myedge = 156;
  gmode[3].mxorig = 8;
  gmode[3].myorig = 44;
  gmode[3].mxsize = 112;
  gmode[3].mysize = 112;
  gmode[3].bx = 14;
  gmode[3].by = 14;
  gmode[3].mxb = 256;
  gmode[3].myb = 80;
  gmode[3].mxe = 512;
  gmode[3].mye = 400;

  gmode[4].x = 1280;
  gmode[4].y = 1024;
  gmode[4].xedge = 1280;
  gmode[4].yedge = 992;
  gmode[4].xorig = 256;
  gmode[4].yorig = 32;
  gmode[4].xsize = 1024;
  gmode[4].ysize = 960;
  gmode[4].xstep = 32;
  gmode[4].ystep = 30;
  gmode[4].xstart = 0;
  gmode[4].ystart = 0;
  gmode[4].mxedge = 120;
  gmode[4].myedge = 156;
  gmode[4].mxorig = 8;
  gmode[4].myorig = 44;
  gmode[4].mxsize = 112;
  gmode[4].mysize = 112;
  gmode[4].bx = 14;
  gmode[4].by = 14;
  gmode[4].mxb = 256;
  gmode[4].myb = 80;
  gmode[4].mxe = 512;
  gmode[4].mye = 400;

  screen = new Screen;
  if(argc > 1)  {
    if(argv[1][0] == '3')  cmode = 0;
    else if(argv[1][0] == '6')  cmode = 1;
    else if(argv[1][0] == '8')  cmode = 2;
    else if(argv[1][1] == '0')  cmode = 3;
    else if(argv[1][1] == '2')  cmode = 4;
    }
  else  {
    if(screen->DefaultXSize() == 320)  cmode = 0;
    else if(screen->DefaultXSize() == 640)  cmode = 1;
    else if(screen->DefaultXSize() == 800)  cmode = 2;
    else if(screen->DefaultXSize() == 1024)  cmode = 3;
    else if(screen->DefaultXSize() == 1280)  cmode = 4;
    }
//  while(!screen->ModeSupported(gmode[cmode].cmode))  cmode--;
//  screen ->SetMode(gmode[cmode].cmode);
  screen->SetSize(gmode[cmode].x, gmode[cmode].y);
  screen->SetFont("basic10.sgf");
  user = new User;
  mouse = new Mouse;
  mouse->SetCursor(NormalMouseG);
  mouse->ShowCursor();
  screen->GetPSPPalette("graphics/cw2.pal");
  screen->SetPaletteEntry(0, 0, 0, 0);

  debug_position=30;

  char quit = 0, play = 0, pnum = 0, nump = 1;
  while(!quit)  {
    mouse->SetBehavior(MOUSE_CLICK, MOUSE_CLICK, MOUSE_CLICK);
    UserAction curact;
    Graphic QB0("graphics/640/buttons/quit0.bmp"); 
    Graphic QB1("graphics/640/buttons/quit1.bmp");
    Graphic SB0("graphics/640/buttons/start0.bmp"); 
    Graphic SB1("graphics/640/buttons/start1.bmp");
    Graphic CB0("graphics/640/buttons/create0.bmp"); 
    Graphic CB1("graphics/640/buttons/create1.bmp");
    Graphic JB0("graphics/640/buttons/join0.bmp"); 
    Graphic JB1("graphics/640/buttons/join1.bmp");
    QB0.FindTrueCenter();
    QB1.FindTrueCenter();
    SB0.FindTrueCenter();
    SB1.FindTrueCenter();
    CB0.FindTrueCenter();
    CB1.FindTrueCenter();
    JB0.FindTrueCenter();
    JB1.FindTrueCenter();
    debug_position=60;
    {
      Button QB;
      Button SB;
      Button CB;
      Button JB;
      QB.SetImage(&QB0, &QB1);
      SB.SetImage(&SB0, &SB1);
      CB.SetImage(&CB0, &CB1);
      JB.SetImage(&JB0, &JB1);
      QB.Move((gmode[cmode].x/2), (gmode[cmode].y*5)/6);
      SB.Move((gmode[cmode].x/2), (gmode[cmode].y*5)/12);
      CB.Move((gmode[cmode].x/2), (gmode[cmode].y*6)/12);
      JB.Move((gmode[cmode].x/2), (gmode[cmode].y*7)/12);
      user->MapKeyToButton(SCAN_Q, &QB);
      user->MapKeyToButton(SCAN_P, &SB);
      user->MapKeyToButton(SCAN_S, &SB);
      user->MapKeyToButton(SCAN_C, &CB);
      user->MapKeyToButton(SCAN_M, &JB);
      user->MapKeyToButton(SCAN_J, &JB);
      screen->Refresh();
      screen->FadeIn(4);
      debug_position=70;
      while((!play) && (!quit))  {
	pnum = 0;
	curact = user->Action();
	if(curact.Type() == USERACTION_SYSTEM_QUIT)  quit = 1;
	if(curact.Type() == USERACTION_BUTTONRELEASED)  {
	  if(curact.ButtonPressed() == QB.SpriteNumber())  {
	    quit = 1;
	    }
	  if(curact.ButtonPressed() == SB.SpriteNumber())  {
	    play = 1; nump = 1; multi = 0;
	    }
	  if(curact.ButtonPressed() == CB.SpriteNumber())  {
	    play = 1; multi = 1;
//	    screen->FadeOut();
//	    QB.Erase();  SB.Erase();  CB.Erase();  JB.Erase();
//	    screen->Refresh();
	    nump = GetNetPlayers();
//	    QB.Draw();  SB.Draw();  CB.Draw();  JB.Draw();
//	    screen->Refresh();
//	    screen->FadeIn();
	    if(nump==1)  multi=0;
	    }
	  if(curact.ButtonPressed() == JB.SpriteNumber())  {
	    play = 1; multi = 1;
	    pnum = JoinNetGame();
	    if(pnum)  nump = StartNetGame();
	    else  { play=0; nump=0; multi=0; pnum=0; }
	    }
	  }
	screen->Refresh();
	}
      }
    screen->FadeOut(4);
    screen->ClearScreen();
    debug_position=80;
    if(play)  {
      if(multi)  net.IPX_ChangeSocket(7810);
      Game *thisgame = new Game("maps/map1.cw2", nump, 1);
      thisgame->Play(pnum);
      delete thisgame;
      mouse->SetCursor(NormalMouseG);
      screen->RefreshFull();
      if(multi)  net.IPX_ChangeSocket(7446);
      }
    play = 0;
    }
  debug_position=90;
  delete screen;
  delete user;
  return 0;
  }

int GetNetPlayers()  {
  user->KeyCurrent();
  int other;
  int ctr=1, done=0;
  net.SetIPX(IPX_TAG, 7446);
  net.SetTCPIP("127.0.0.1", 7446);
  while(!done)  {
    char *recv = NULL;
    char buf[20];
    while(recv == NULL && !user->IsPressed(SCAN_SPACE))  {
      if(net.PacketReceived())  {
	recv = (char *)net.Receive();
	while(recv == NULL)  recv = (char *)net.Receive();
	if(strcmp(recv, "JOIN"))  {
	  delete recv;
	  recv = NULL;
	  }
	}
      }
    if(recv != NULL)  {
      other = net.LastSource();
      printf("Got JOIN from %d\r\n", other);
      strcpy(buf, "ACCEPT");
      buf[6] = ctr++;
      buf[7] = 0;
      net.Send(buf, 8, other);
      delete recv;
      recv = NULL;
      printf("Sent ACCEPT\r\n");
      done = 1;
      }
    else  done = 1;
    }
  user->KeyQueue();
  { char *buf;
    buf = new char[10];
    strcpy(buf, "STARTNET");
    buf[8] = ctr;
    buf[9] = 0;
    net.Send(buf, 10);
    delete buf;
    printf("Sent STARTNET\r\n");
    }
  return ctr;
  }

int StartNetGame()  {
  int num;
  char *buf;
//  net.SetIPX(IPX_TAG, 7446);
//  net.SetTCPIP("127.0.0.1", 7446);
  printf("Starting!\r\n");
  buf = (char *)net.Receive();
  while(buf == NULL || strncmp(buf, "STARTNET", 8))
    buf = (char *)net.Receive();
  printf("Got STARTNET\r\n");
  num = buf[8];
  delete buf;
  return num;
  }

int JoinNetGame()  {
  int num;
  char *recv = NULL;
  net.SetIPX(IPX_TAG, 7446);
  net.SetTCPIP("127.0.0.1", 7446);
  net.Send("JOIN", 5);
  while(recv == NULL && !user->IsPressed(SCAN_SPACE))  {
    if(net.PacketReceived())  {
      recv = (char *)net.Receive();
      if(recv != NULL && strncmp(recv, "ACCEPT", 6))  {
	delete recv;
	recv = NULL;
	}
      }
    }
  if(recv != NULL)  {
    printf("Got ACCEPT!\r\n");
    num = recv[6];
    delete recv;
    return num;
    }
  else  return 0;
  }
