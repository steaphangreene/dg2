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
extern Sound *ButtonClick;

extern Screen *screen;
extern InputQueue *input;
Map *curmap = NULL;

#define gm gmode[cmode]
extern GMode gmode[10];
extern char cmode;

int MainButtonNum;
Panel mainp = 0;
Panel minip = 0;
Thing *things = NULL;
extern Mouse *mouse;

Game *curgame = NULL;

Game::Game(char *inmap, int nump, int numcomp)  {
  Debug("In Game::Game");
  SPEED = 40;
  SPEED_STEP = 10;
  SPEED_MIN = 10;
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
    players[ctr] = new Player(ctr, PLAYER_NETWORK, ctr/10, (ctr+1)%10);
    }
  for(; ctr<num_players; ctr++)  {
    players[ctr] = new Player(ctr, PLAYER_COMPUTER, ctr/10, (ctr+1)%10);
    }
  curmap = new Map(inmap);

  curgame = this;
  Debug("Out Game::Game");
  }

Game::~Game()  {
  things->DeleteAll();
  delete curmap;
  curmap = NULL;
  curgame = NULL;
  }

void Game::Play(int pnum)  {
  Debug("Game::Play() Begin");
  delete players[pnum];
  p1 = new Player(pnum, PLAYER_CONSOLE, 9, 6);
  players[pnum] = p1;
  int ctr, ctr2;
  Graphic *MainButton0 = NULL;
  Graphic *MainButton1 = NULL;
  Graphic Normalg("graphics/pointers/normal.bmp");
  Debug("Game::Play() Before switch");
  switch(cmode)  {
    case(0):	screen->FullScreenBMP("graphics/320/backg.bmp");
		MainButton0 = new Graphic("graphics/320/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/320/buttons/menu1.bmp");
		backplate = new Graphic("graphics/320/menuback.bmp");
		break;
    case(1):	screen->FullScreenBMP("graphics/640/backg.bmp");
		MainButton0 = new Graphic("graphics/640/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/640/buttons/menu1.bmp");
		backplate = new Graphic("graphics/640/menuback.bmp");
		break;
    case(2):	screen->FullScreenBMP("graphics/800/backg.bmp");
		MainButton0 = new Graphic("graphics/800/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/800/buttons/menu1.bmp");
		backplate = new Graphic("graphics/800/menuback.bmp");
		break;
    case(3):	screen->FullScreenBMP("graphics/1024/backg.bmp");
		MainButton0 = new Graphic("graphics/1024/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/1024/buttons/menu1.bmp");
		backplate = new Graphic("graphics/1024/menuback.bmp");
		break;
    case(4):	screen->FullScreenBMP("graphics/1280/backg.bmp");
		MainButton0 = new Graphic("graphics/1280/buttons/menu0.bmp");
		MainButton1 = new Graphic("graphics/1280/buttons/menu1.bmp");
		backplate = new Graphic("graphics/1280/menuback.bmp");
		break;
    }
  Debug("Game::Play() After switch");
  MainButton0->tcolor = 0;
  MainButton1->tcolor = 0;
  backplate->tcolor = 0;
  mainp = screen->NewPanel(gm.xorig, gm.yorig, gm.xedge, gm.yedge);
  mouse->SetCursor(&Normalg);
  mouse->ShowCursor();
  mouse->SetBehavior(0, 1, MB_CLICK);
  mouse->SetBehavior(0, 2, MB_IGNORE);
  mouse->SetBehavior(0, 3, MB_CLICK);
  mouse->SetBehavior(mainp, 1, MB_BOX);
  mouse->SetBehavior(mainp, 2, MB_CLICK);
  mouse->SetBehavior(mainp, 3, MB_BOX);
  mouse->SetSelColor(screen->GetPalette().GetClosestColor(255,255,0));

  minip = screen->NewPanel(gm.mxorig, gm.myorig, gm.mxedge, gm.myedge);
  for(ctr=gm.mxorig; ctr<gm.mxedge; ctr++)  {
    for(ctr2=gm.myorig; ctr2<gm.myedge; ctr2++)  {
//      screen->SetPoint(ctr, ctr2, 0);
      }
    }
  mouse->SetBehavior(minip, 1, MB_CLICKDRAW);
  mouse->SetBehavior(minip, 2, MB_CLICK);
  mouse->SetBehavior(minip, 3, MB_BOX);

  Debug("Game::Play() After behavior");
  Creature *dude = NULL;
  dude->SetCreatureGraphic(CREATURE_GUY, "guy");

  Debug("Game::Play() Before Weapons");
  Weapon bow, spear, sword;
  bow.ammo = AMMO_ARROW;
  bow.range = 6;
  bow.pdam = 50;
  bow.bdam = 0;
  bow.rof = 80;
  bow.SetImage("graphics/weapons/longbow");
  spear.range = 2;
  spear.mrange = 2;
  spear.pdam = 20;
  spear.bdam = 20;
  spear.SetImage("graphics/weapons/spear");
  sword.range = 1;
  sword.pdam = 10;
  sword.bdam = 30;
  sword.SetImage("graphics/weapons/sword");

  Debug("Game::Play() After Weapons");

  for(ctr2=0; ctr2<num_players; ctr2++)  {
    for(ctr=0; ctr<NUM_GUYS; ctr++)  {
      dude = new Creature(CREATURE_GUY, players[ctr2]);
      if(dude == NULL)  U2_Exit(1, "Out of Memory for creatures!!!\r\n");
      dude->Place(curmap->CellAt((ctr/GUY_WIDTH)+1+(8*ctr2),
		(ctr%GUY_WIDTH)+11));
      if(ctr2 == 0 && ctr == 0) {
	for(int sp = 0; sp < 64; sp++)  {
	  dude->LearnSpell(sp);
	  }
	}
      else if(ctr2 == 0 && ctr == 1) dude->LearnSpell(SPELL_VISION);
      else if(ctr2 == 0 && ctr == GUY_WIDTH) dude->LearnSpell(PRAYER_HEAL);
      if(ctr2 == 0 && ctr < 30) dude->AddWeapon(bow);
      else if(ctr2 == 0 && ctr < 60) dude->AddWeapon(spear);
      else if(ctr2 == 0) dude->AddWeapon(sword);
      else if(ctr2 == 1 && ctr < 30) dude->AddWeapon(sword);
      else if(ctr2 == 1 && ctr < 60) dude->AddWeapon(spear);
      else if(ctr2 == 1) dude->AddWeapon(bow);
      }
    }
  dude = NULL;

  Debug("Game::Play() After Guy");

  Sprite vp1, vp2, vp3, vp4;
  Graphic viewptrg(4, 4);
  viewptrg.tcolor = (screen->GetPalette().GetClosestColor(255,255,255) == 0);
  viewptrg.DrawFillRect(0, 0, 4, 1, 8,
	screen->GetPalette().GetClosestColor(255,255,255));
  viewptrg.DrawFillRect(0, 1, 1, 3, 8,
	screen->GetPalette().GetClosestColor(255,255,255));
  vp1.SetImage(viewptrg);
  viewptrg.XFlip();
  vp2.SetImage(viewptrg);
  viewptrg.YFlip();
  vp4.SetImage(viewptrg);
  viewptrg.XFlip();
  vp3.SetImage(viewptrg);

  Debug("Game::Play() After Minimap Construction");

  char scshow = 0;
  Clickey MainButton;
  MainButton.SetImage(MainButton0, MainButton1);
  MainButton.SetSound(ButtonClick, NULL);
  MainButton.Move(gm.bx, gm.by);
  MainButtonNum = MainButton.Number();
  input->MapKeyToControl(KEY_PAUSE, &MainButton);
  quit = 0;
  int oldxe=-1, oldxb=-1, oldye=-1, oldyb=-1;
  int packetgo = 0;
  Debug("Game::Play() Before Main Loop");
  while(!quit)  {
    Debug("Game::Play() Loop: Start");
    for(ctr=0; ctr<num_players; ctr++)  {
      Debug("Game::Play() Loop: Player->TakeTurn");
      players[ctr]->TakeTurn(packetgo);
      }
    Debug("Game::Play(): Tick Things");
    things->tick();
    Debug("Game::Play(): Update Things");
    things->update();
    Debug("Game::Play(): screen->Refresh()");
    screen->Refresh();
    Debug("Game::Play() Loop: Packet Variable Stuff");
    packetgo++;
    if(packetgo >= PACKET_DELAY)  packetgo = 0;
    Debug("Game::Play() Loop: Mouse Cursor Reset");
    if(p1->CurrentCommand() == COMMAND_DEFAULT)  mouse->SetCursor(&Normalg);

    Debug("Game::Play() Loop: Setup Minimap Indicator");
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

    Debug("Game::Play() Loop: Refresh and Ticker++");
    if(scshow == 0)  { screen->RefreshFull(); screen->FadeIn(4); scshow = 1; }
    ticker++;
    Debug("Game::Play() Loop: Finish");
    }
  delete backplate;
  screen->FadeOut(4);
  screen->ErasePanelSprites(minip);
  screen->RemovePanel(minip);
  screen->ErasePanelSprites(mainp);
  screen->RemovePanel(mainp);
  screen->ErasePanelSprites(0);
  screen->Clear();
  Debug("Game::Play() End");
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
  char inmainmenu = 1, othermenu = 0;
  Panel menuwin = screen->NewPanel(gm.mxb, gm.myb, gm.mxe, gm.mye);
  mouse->SetBehavior(menuwin, 1, MB_CLICK);
  mouse->SetBehavior(menuwin, 2, MB_CLICK);
  mouse->SetBehavior(menuwin, 3, MB_CLICK);

  InputAction *curact;

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

  EB0->tcolor = 0;
  EB1->tcolor = 0;
  RB0->tcolor = 0;
  RB1->tcolor = 0;
  QB0->tcolor = 0;
  QB1->tcolor = 0;

  Sprite plate(*backplate);
  plate.SetPriority(-9000);
  plate.SetFlag(SPRITE_SOLID|SPRITE_RECTANGLE);
  plate.Move(gm.mxb, gm.myb);
  screen->Refresh();

  Clickey EnvironButton;
  EnvironButton.SetPriority(-9001);
  EnvironButton.SetImage(EB0, EB1);
  EnvironButton.Move(x1, y2);
  input->MapKeyToControl(KEY_E, &EnvironButton);

  Clickey ResumeButton;
  ResumeButton.SetPriority(-9001);
  ResumeButton.SetImage(RB0, RB1);
  ResumeButton.Move(x1, y1);
  input->MapKeyToControl(KEY_R, &ResumeButton);
  input->MapKeyToControl(KEY_ESC, &ResumeButton);

  Clickey QuitButton;
  QuitButton.SetPriority(-9001);
  QuitButton.SetImage(QB0, QB1);
  QuitButton.Move(x2, y1);
  input->MapKeyToControl(KEY_Q, &QuitButton);

  while(inmainmenu && (!quit) && (!othermenu))  {
    curact = input->WaitForNextAction();
    if(curact->g.type == INPUTACTION_SYSTEM_QUIT)  quit = 1;
    if(curact->g.type == INPUTACTION_CONTROLUP)  {
      if(curact->c.control == QuitButton.Number())  quit=1;
      else if(curact->c.control == ResumeButton.Number())  {
	inmainmenu=0;
	}
      else if(curact->c.control == EnvironButton.Number())  {
	othermenu=1;
	}
      }
    screen->Refresh();
    }
  screen->RemovePanel(menuwin);
  return othermenu;
  }

int Game::EnvironMenu()  {
  char inmainmenu = 1, othermenu = 0;
  Panel menuwin = screen->NewPanel(gm.mxb, gm.myb, gm.mxe, gm.mye);
  mouse->SetBehavior(menuwin, 1, MB_CLICK);
  mouse->SetBehavior(menuwin, 2, MB_CLICK);
  mouse->SetBehavior(menuwin, 3, MB_CLICK);

  InputAction *curact;

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

  EB0->tcolor = 0;
  EB1->tcolor = 0;
  RB0->tcolor = 0;
  RB1->tcolor = 0;
  QB0->tcolor = 0;
  QB1->tcolor = 0;

  Sprite plate(*backplate);
  plate.SetPriority(-9000);
  plate.SetFlag(SPRITE_SOLID|SPRITE_RECTANGLE);
  plate.Move(gm.mxb, gm.myb);
  screen->Refresh();

  Clickey EnvironButton;
  EnvironButton.SetPriority(-9001);
  EnvironButton.SetImage(EB0, EB1);
  EnvironButton.Move(x1, y3);

  Clickey QuitButton;
  QuitButton.SetPriority(-9001);
  QuitButton.SetImage(QB0, QB1);
  QuitButton.Move(x1, y2);

  Clickey ReturnButton;
  ReturnButton.SetPriority(-9001);
  ReturnButton.SetImage(RB0, RB1);
  ReturnButton.Move(x1, y1);

  input->MapKeyToControl(KEY_M, &ReturnButton);
  input->MapKeyToControl(KEY_R, &ReturnButton);
  input->MapKeyToControl(KEY_ESC, &ReturnButton);

  while(inmainmenu && (!quit) && (!othermenu))  {
    curact = input->WaitForNextAction();
    if(curact->g.type == INPUTACTION_SYSTEM_QUIT)  inmainmenu = 1;
    if(curact->g.type == INPUTACTION_CONTROLUP)  {
      if(curact->c.control == ReturnButton.Number())  {
	inmainmenu=0;
	}
      }
    screen->Refresh();
    }
  screen->RemovePanel(menuwin);
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

int Game::NumPlayers()  {
  return num_players;
  }
