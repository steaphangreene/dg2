#include <time.h>
#include <user/screen.h>
#include "action.h"
#include "thing.h"
#include "creature.h"
#include "cell.h"
#include "map.h"
#include "player.h"
#include "gmode.h"
#include "game.h"
#include "spell.h"
#include "ammo.h"

#define NUM_GUYS	(3*30)
#define GUY_WIDTH	30

long long ticker = 0;

Player *p1;
extern DigSample *ButtonClick;

extern Screen *screen;
extern User *user;
Map *curmap = NULL;

#define gm gmode[cmode]
extern GMode gmode[10];
extern char cmode;

int MainButtonNum;
Window mainw = 0;
Window miniw = 0;
Thing things;
extern Mouse *mouse;

Game *curgame = NULL;

Game::Game(char *inmap, int nump, int numcomp)  {
  debug_position = 1010;
  SPEED = 40;
  SPEED_STEP = 10;
  SPEED_MIN = 20;
  SPEED_MAX = 80;
  SetSpeed();
  PACKET_DELAY = 6;
  gm.xstart = 0;
  gm.ystart = 0;
  gm.xstep = 32;
  gm.ystep = 30;
  if(cmode == 0)  { gm.ystep>>=1; gm.xstep>>=1; }
  int ctr;
  num_players = nump+numcomp;
  players = new Player*[num_players];
  for(ctr=0; ctr<nump; ctr++)  {
    players[ctr] = new Player(PLAYER_NETWORK);
    (players[ctr]->GetCMap())[223] = 87;
    (players[ctr]->GetCMap())[216] = 86;
    (players[ctr]->GetCMap())[106] = 255;
    (players[ctr]->GetCMap())[87] = 245;
    }
  for(; ctr<num_players; ctr++)  {
    players[ctr] = new Player(PLAYER_COMPUTER);
    (players[ctr]->GetCMap())[223] = 87;
    (players[ctr]->GetCMap())[216] = 86;
    (players[ctr]->GetCMap())[106] = 255;
    (players[ctr]->GetCMap())[87] = 245;
    }
  curmap = new Map(inmap);

  curgame = this;
  debug_position = 1090;
  }

Game::~Game()  {
  things.DeleteAll();
  delete curmap;
  curmap = NULL;
  curgame = NULL;
  }

void Game::Play(int pnum)  {
  debug_position = 1100;
  delete players[pnum];
  debug_position = 1101;
  p1 = new Player(PLAYER_CONSOLE);
  debug_position = 1102;
  (p1->GetCMap())[223] = 191;
  debug_position = 1103;
  (p1->GetCMap())[216] = 191;
  debug_position = 1104;
  (p1->GetCMap())[106] = 179;
  debug_position = 1105;
  (p1->GetCMap())[87] = 44;
  debug_position = 1106;
  players[pnum] = p1;
  debug_position = 1107;
  int ctr, ctr2;
  debug_position = 1108;
  Graphic *MainButton0 = NULL;
  debug_position = 1109;
  Graphic *MainButton1 = NULL;
  debug_position = 1110;
  Graphic Normalg("graphics/pointers/normal.bmp");
  debug_position = 1111;
  switch(cmode)  {
    case(0):	debug_position = 1111;
		screen->FullScreenBMP("graphics/320/backg.bmp");
		MainButton0 = new Graphic("graphics/320/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/320/buttons/menu1.bmp");
		backplate = new Graphic("graphics/320/menuback.bmp");
		break;
    case(1):	debug_position = 1111;
		screen->FullScreenBMP("graphics/640/backg.bmp");
		MainButton0 = new Graphic("graphics/640/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/640/buttons/menu1.bmp");
		backplate = new Graphic("graphics/640/menuback.bmp");
		break;
    case(2):	debug_position = 1112;
		screen->FullScreenBMP("graphics/800/backg.bmp");
		debug_position = 1113;
		MainButton0 = new Graphic("graphics/800/buttons/menu0.bmp");
		debug_position = 1114;
		MainButton1 = new Graphic("graphics/800/buttons/menu1.bmp");
		debug_position = 1115;
		backplate = new Graphic("graphics/800/menuback.bmp");
		debug_position = 1116;
		break;
    case(3):	debug_position = 1111;
		screen->FullScreenBMP("graphics/1024/backg.bmp");
		MainButton0 = new Graphic("graphics/1024/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/1024/buttons/menu1.bmp");
		backplate = new Graphic("graphics/1024/menuback.bmp");
		break;
    case(4):	debug_position = 1111;
		screen->FullScreenBMP("graphics/1280/backg.bmp");
		MainButton0 = new Graphic("graphics/1280/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/1280/buttons/menu1.bmp");
		backplate = new Graphic("graphics/1280/menuback.bmp");
		break;
    }
  debug_position = 1120;
  mainw = screen->NewWindow(gm.xorig, gm.yorig, gm.xedge, gm.yedge);
  mouse->SetCursor(&Normalg);
  mouse->ShowCursor();
  mouse->SetBehavior(MOUSE_CLICK, MOUSE_IGNORE, MOUSE_CLICK);
  mouse->SetWindowBehavior(mainw, MOUSE_BOX, MOUSE_CLICK, MOUSE_BOX);

  debug_position = 1130;
  miniw = screen->NewWindow(gm.mxorig, gm.myorig, gm.mxedge, gm.myedge);
  for(ctr=gm.mxorig; ctr<gm.mxedge; ctr++)  {
    for(ctr2=gm.myorig; ctr2<gm.myedge; ctr2++)  {
      screen->BSetPoint(ctr, ctr2, 0);
      }
    }
  mouse->SetWindowBehavior(miniw, MOUSE_DRAW, MOUSE_CLICK, MOUSE_BOX);

  debug_position = 1140;
  Creature *dude = NULL;
  debug_position = 1141;
  dude->SetCreatureGraphic(CREATURE_DUDE, "dude");
  debug_position = 1142;
  for(ctr2=0; ctr2<num_players; ctr2++)  {
    for(ctr=0; ctr<NUM_GUYS; ctr++)  {
      debug_position = 1143;
      dude = new Creature(CREATURE_DUDE, players[ctr2]);
      debug_position = 1144;
      if(dude == NULL)  Exit(1, "Out of Memory for creatures!!!\r\n");
      debug_position = 1145;
      dude->Place(curmap->CellAt((ctr/GUY_WIDTH)+1+(6*ctr2),
		(ctr%GUY_WIDTH)+11));
      debug_position = 1146;
      if(ctr2 == 0 && ctr == 0) {
	for(int sp = 0; sp < 64; sp++)  {
          debug_position = 1147;
	  dude->LearnSpell(sp);
          debug_position = 1148;
	  }
	}
      else if(ctr2 == 0 && ctr == 1) dude->LearnSpell(SPELL_VISION);
      debug_position = 1150;
      }
    }
  dude = NULL;

  debug_position = 1180;
  IntList pp;
  pp += curmap->CellAt(4, 4)->Number();
  pp += curmap->CellAt(40, 40)->Number();

  Sprite vp1, vp2, vp3, vp4;
  Graphic viewptrg(4, 4);
  viewptrg.DefLinH("FFFFFFFF"); 
  viewptrg.DefLinH("FF------"); 
  viewptrg.DefLinH("FF------"); 
  viewptrg.DefLinH("FF------"); 
  vp1.SetImage(viewptrg);
  viewptrg.XFlip();
  vp2.SetImage(viewptrg);
  viewptrg.YFlip();
  vp4.SetImage(viewptrg);
  viewptrg.XFlip();
  vp3.SetImage(viewptrg);

  debug_position = 1190;
  char scshow = 0;
  Button MainButton;
  MainButton.SetImage(MainButton0, MainButton1);
  MainButton.SetSound(ButtonClick, NULL);
  MainButton.Move(gm.bx, gm.by);
  MainButtonNum = MainButton.SpriteNumber();
  user->MapKeyToButton(SCAN_PAUSE, &MainButton);
  quit = 0;
  int oldxe=-1, oldxb=-1, oldye=-1, oldyb=-1;
  int packetgo = 0;
  while(!quit)  {
    debug_position = 1200;
    for(ctr=0; ctr<num_players; ctr++)  {
      players[ctr]->TakeTurn(packetgo);
      }
    packetgo++;
    if(packetgo >= PACKET_DELAY)  packetgo = 0;
    if(p1->CurrentCommand() == COMMAND_DEFAULT)  mouse->SetCursor(&Normalg);

    { int xvs = gm.xsize/gm.xstep;
      int yvs = gm.ysize/gm.ystep;
      int xs = curmap->XSize()-1;
      int ys = curmap->YSize()-1;
      int xb = ((gm.xstart/2)*gm.mxsize)/xs;
      int yb = (gm.ystart*gm.mysize)/ys;
      int xe = ((((gm.xstart/2) + xvs)*gm.mxsize)/xs)-1;
      int ye = (((gm.ystart + yvs)*gm.mysize)/ys)-1;
      if(oldxe != xe || oldxb != xb || oldye != ye || oldyb != yb)  {
	vp1.Move(gm.mxorig+xb, gm.myorig+yb);
	vp2.Move(gm.mxorig+xe, gm.myorig+yb);
	vp3.Move(gm.mxorig+xb, gm.myorig+ye);
	vp4.Move(gm.mxorig+xe, gm.myorig+ye);
	}
      oldxb = xb;
      oldxe = xe;
      oldyb = yb;
      oldye = ye;
      }

    debug_position = 1210;
    if(scshow == 0)  { screen->RefreshFull(); screen->FadeIn(4); scshow = 1; }
//    screen->Refresh();
    ticker++;
    }
  delete backplate;
  screen->FadeOut(4);
  screen->DeleteWindow(miniw);
  screen->DeleteWindow(mainw);
  screen->EraseWindowSprites(0);
  screen->EraseWindowBackground(0);
  screen->ClearScreen();
  debug_position = 1250;
  }

void Game::Menus()  {
  screen->SetFrameRate(0);
  char menu=MainMenu();
  while(menu != 0)  {
    if(menu==1)  {
      EnvironMenu();
      }
    menu=MainMenu();
    }
  SetSpeed();
  }

int Game::MainMenu()  {
  IntList overwritten;
  unsigned char *buf;
  char inmainmenu = 1, othermenu = 0;
  Window menuwin = screen->NewWindow(gm.mxb, gm.myb, gm.mxe, gm.mye);
  mouse->SetWindowBehavior(menuwin, MOUSE_CLICK, MOUSE_CLICK, MOUSE_CLICK);
  overwritten = screen->EraseWindowSprites(menuwin); 
  screen->EraseWindowBackground(menuwin); 
  buf = screen->BackupWindow(menuwin);

  UserAction curact;

  Graphic *EB0 = NULL;
  Graphic *EB1 = NULL;
  Graphic *RB0 = NULL;
  Graphic *RB1 = NULL;
  Graphic *QB0 = NULL;
  Graphic *QB1 = NULL;
  int y1=0, y2=0, y3=0;
  int x1=0, x2=0;
  switch(cmode)  {
    case(0):
	EB0 = new Graphic("graphics/320/buttons/environ0.bmp");
	EB1 = new Graphic("graphics/320/buttons/environ1.bmp");
	RB0 = new Graphic("graphics/320/buttons/resume0.bmp");
	RB1 = new Graphic("graphics/320/buttons/resume1.bmp");
	QB0 = new Graphic("graphics/320/buttons/quit0.bmp");
	QB1 = new Graphic("graphics/320/buttons/quit1.bmp");
	y1=160; y2=140; y3=120; x1=135; x2=199;
	break;
    case(1):
	EB0 = new Graphic("graphics/640/buttons/environ0.bmp");
	EB1 = new Graphic("graphics/640/buttons/environ1.bmp");
	RB0 = new Graphic("graphics/640/buttons/resume0.bmp");
	RB1 = new Graphic("graphics/640/buttons/resume1.bmp");
	QB0 = new Graphic("graphics/640/buttons/quit0.bmp");
	QB1 = new Graphic("graphics/640/buttons/quit1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    case(2):
	EB0 = new Graphic("graphics/800/buttons/environ0.bmp");
	EB1 = new Graphic("graphics/800/buttons/environ1.bmp");
	RB0 = new Graphic("graphics/800/buttons/resume0.bmp");
	RB1 = new Graphic("graphics/800/buttons/resume1.bmp");
	QB0 = new Graphic("graphics/800/buttons/quit0.bmp");
	QB1 = new Graphic("graphics/800/buttons/quit1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    case(3):
	EB0 = new Graphic("graphics/1024/buttons/environ0.bmp");
	EB1 = new Graphic("graphics/1024/buttons/environ1.bmp");
	RB0 = new Graphic("graphics/1024/buttons/resume0.bmp");
	RB1 = new Graphic("graphics/1024/buttons/resume1.bmp");
	QB0 = new Graphic("graphics/1024/buttons/quit0.bmp");
	QB1 = new Graphic("graphics/1024/buttons/quit1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    case(4):
	EB0 = new Graphic("graphics/1280/buttons/environ0.bmp");
	EB1 = new Graphic("graphics/1280/buttons/environ1.bmp");
	RB0 = new Graphic("graphics/1280/buttons/resume0.bmp");
	RB1 = new Graphic("graphics/1280/buttons/resume1.bmp");
	QB0 = new Graphic("graphics/1280/buttons/quit0.bmp");
	QB1 = new Graphic("graphics/1280/buttons/quit1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    }

  screen->PasteGraphic(*backplate, gm.mxb, gm.myb);
  screen->RefreshFull();

  Button EnvironButton;
  EnvironButton.SetImage(EB0, EB1);
  EnvironButton.Move(x1, y2);
  user->MapKeyToButton(SCAN_E, &EnvironButton);

  Button ResumeButton;
  ResumeButton.SetImage(RB0, RB1);
  ResumeButton.Move(x1, y1);
  user->MapKeyToButton(SCAN_R, &ResumeButton);
  user->MapKeyToButton(SCAN_ESC, &ResumeButton);

  Button QuitButton;
  QuitButton.SetImage(QB0, QB1);
  QuitButton.Move(x2, y1);
  user->MapKeyToButton(SCAN_Q, &QuitButton);

  while(inmainmenu && (!quit) && (!othermenu))  {
    curact = user->Action();
    if(curact.Type() == USERACTION_SYSTEM_QUIT)  quit = 1;
    if(curact.Type() == USERACTION_BUTTONRELEASED)  {
      if(curact.ButtonPressed() == QuitButton.SpriteNumber())  quit=1;
      else if(curact.ButtonPressed() == ResumeButton.SpriteNumber())  {
	inmainmenu=0;
	}
      else if(curact.ButtonPressed() == EnvironButton.SpriteNumber())  {
	othermenu=1;
	}
      }
    screen->Refresh();
    }
  screen->EraseWindowSprites(menuwin); 
  screen->EraseWindowBackground(menuwin); 
  screen->RestoreWindow(menuwin, buf);
  screen->DeleteWindow(menuwin);
  screen->RedrawSprites(overwritten);
  return othermenu;
  }

int Game::EnvironMenu()  {
  IntList overwritten;
  unsigned char *buf;
  char inmainmenu = 1, othermenu = 0;
  Window menuwin = screen->NewWindow(gm.mxb, gm.myb, gm.mxe, gm.mye);
  mouse->SetWindowBehavior(menuwin, MOUSE_CLICK, MOUSE_CLICK, MOUSE_CLICK);
  overwritten = screen->EraseWindowSprites(menuwin); 
  screen->EraseWindowBackground(menuwin); 
  buf = screen->BackupWindow(menuwin);

  UserAction curact;

  Graphic *EB0 = NULL;
  Graphic *EB1 = NULL;
  Graphic *RB0 = NULL;
  Graphic *RB1 = NULL;
  Graphic *QB0 = NULL;
  Graphic *QB1 = NULL;
  int y1=0, y2=0, y3=0;
  int x1=0, x2=0;
  switch(cmode)  {
    case(0):
	EB0 = new Graphic("graphics/320/buttons/lblank0.bmp");
	EB1 = new Graphic("graphics/320/buttons/lblank1.bmp");
	RB0 = new Graphic("graphics/320/buttons/return0.bmp");
	RB1 = new Graphic("graphics/320/buttons/return1.bmp");
	QB0 = new Graphic("graphics/320/buttons/lblank0.bmp");
	QB1 = new Graphic("graphics/320/buttons/lblank1.bmp");
	y1=160; y2=140; y3=120; x1=135; x2=199;
	break;
    case(1):
	EB0 = new Graphic("graphics/640/buttons/lblank0.bmp");
	EB1 = new Graphic("graphics/640/buttons/lblank1.bmp");
	RB0 = new Graphic("graphics/640/buttons/return0.bmp");
	RB1 = new Graphic("graphics/640/buttons/return1.bmp");
	QB0 = new Graphic("graphics/640/buttons/lblank0.bmp");
	QB1 = new Graphic("graphics/640/buttons/lblank1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    case(2):
	EB0 = new Graphic("graphics/800/buttons/lblank0.bmp");
	EB1 = new Graphic("graphics/800/buttons/lblank1.bmp");
	RB0 = new Graphic("graphics/800/buttons/return0.bmp");
	RB1 = new Graphic("graphics/800/buttons/return1.bmp");
	QB0 = new Graphic("graphics/800/buttons/lblank0.bmp");
	QB1 = new Graphic("graphics/800/buttons/lblank1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    case(3):
	EB0 = new Graphic("graphics/1024/buttons/lblank0.bmp");
	EB1 = new Graphic("graphics/1024/buttons/lblank1.bmp");
	RB0 = new Graphic("graphics/1024/buttons/return0.bmp");
	RB1 = new Graphic("graphics/1024/buttons/return1.bmp");
	QB0 = new Graphic("graphics/1024/buttons/lblank0.bmp");
	QB1 = new Graphic("graphics/1024/buttons/lblank1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    case(4):
	EB0 = new Graphic("graphics/1280/buttons/lblank0.bmp");
	EB1 = new Graphic("graphics/1280/buttons/lblank1.bmp");
	RB0 = new Graphic("graphics/1280/buttons/return0.bmp");
	RB1 = new Graphic("graphics/1280/buttons/return1.bmp");
	QB0 = new Graphic("graphics/1280/buttons/lblank0.bmp");
	QB1 = new Graphic("graphics/1280/buttons/lblank1.bmp");
	y1=360; y2=300; y3 = 260; x1=270; x2=398;
	break;
    }

  screen->PasteGraphic(*backplate, gm.mxb, gm.myb);
  screen->RefreshFull();

  Button EnvironButton;
  EnvironButton.SetImage(EB0, EB1);
  EnvironButton.Move(x1, y3);

  Button QuitButton;
  QuitButton.SetImage(QB0, QB1);
  QuitButton.Move(x1, y2);

  Button ReturnButton;
  ReturnButton.SetImage(RB0, RB1);
  ReturnButton.Move(x1, y1);

  user->MapKeyToButton(SCAN_M, &ReturnButton);
  user->MapKeyToButton(SCAN_R, &ReturnButton);
  user->MapKeyToButton(SCAN_ESC, &ReturnButton);

  while(inmainmenu && (!quit) && (!othermenu))  {
    curact = user->Action();
    if(curact.Type() == USERACTION_SYSTEM_QUIT)  inmainmenu = 1;
    if(curact.Type() == USERACTION_BUTTONRELEASED)  {
      if(curact.ButtonPressed() == ReturnButton.SpriteNumber())  {
	inmainmenu=0;
	}
      }
    screen->Refresh();
    }
  screen->EraseWindowSprites(menuwin); 
  screen->EraseWindowBackground(menuwin); 
  screen->RestoreWindow(menuwin, buf);
  screen->DeleteWindow(menuwin);
  screen->RedrawSprites(overwritten);
  return othermenu;
  }

void Game::SlowDown()  {
  SPEED -= SPEED_STEP;
  if(SPEED < SPEED_MIN)  SPEED = SPEED_MIN;
  SetSpeed();
  }

void Game::SpeedUp()  {
  SPEED += SPEED_STEP;
  if(SPEED > SPEED_MAX)  SPEED = SPEED_MAX;
  SetSpeed();
  }

void Game::SetSpeed()  {
  screen->SetFrameRate(SPEED);
  }
