#include <unistd.h>
#include <user.h>
#include <sys/stat.h>

#include "cre.h"

#define MAX_WEAP	10
const char *weapn[MAX_WEAP] = { "punch", "dagger", "sword", "btlaxe",
	"spear", "pike", "cross", "heavcr", "bow", "knife" };
int weapk[MAX_WEAP] = { SCAN_1, SCAN_2, SCAN_3, SCAN_4, SCAN_5, SCAN_6,
	SCAN_7, SCAN_8, SCAN_9, SCAN_0 };

Screen *screen;
User *user;
Sprite *s, *g, *space;
Graphic *gg[FRAME_MAX], *spg;
Graphic *sw[MAX_WEAP][64];
int weapon = 0;
ShortBag *gv[FRAME_MAX];
char *vfn[FRAME_MAX];
int xpos=100;
int stance=0;
char *Directory;
char *ResourceFile;
int fromres;
int frame;
short mem[10][4];

Button *StanceB;
Button *FrameB;

Button *LeftB, *RightB, *UpB, *DownB, *PrevB, *NextB, *TopB, *ClockB, *CClockB;

void Stance();
void UnStance();
void LoadImage(int fnum, const char *fn);
void LoadAnimation(int fnum, const char *basename, int start, int max);
void LoadAnimation(int wnum, int fnum, const char *basename, int start, int max);
void ResetFrame();
void QuickResetFrame();
void SetFrame(int fbase);
void PlayUp(int fbase);
void PlayDown(int fbase);
void PlayLoop(int fbase);
void PlayUpM(int fbase);
void PlayDownM(int fbase);
void PlayLoopM(int fbase);
void DrawWeapon();
void OnFrame();
void WeaponManipEvent(UserAction act);
void ManipWeapon();

int Main(int argc, char **argv)  {
  char buf[64]; int ctr, ctrw;
  Graphic *atc, *tc;

  if(argc != 2)  Exit(1, "USAGE: cre <directory> -or- cre <crf_file>\n");
#ifdef D_OK
  if(!access(argv[1], F_OK|R_OK|D_OK))  {
#else
  struct stat st;
  stat(argv[1], &st);
  if (S_ISDIR(st.st_mode))  {
#endif
    Directory = argv[1];
    ResourceFile = new char[64];
    int ind=strlen(argv[1])-1;
    while(ind > 0 && argv[1][ind-1] != '\\' && argv[1][ind-1] != '/')  ind--;
    sprintf(ResourceFile, "%s.crf%c", &(argv[1][ind]), 0);
    fromres=0;
    }
  else if(!access(argv[1], F_OK|R_OK))  {
    Directory = NULL;
    ResourceFile = argv[1];
    ResFile crf(ResourceFile);
    CharBag *ver = crf.GetCharBag();
    if((*ver)[0] != VER1 || (*ver)[1] != VER2
		|| (*ver)[2] != VER3 || (*ver)[3] != VER4)  {
      if((*ver)[3] == 0)  {
	Exit(1, "Incompatible version, \"%s\" was made by CRE v%d.%d.%d!\n",
		argv[1], (*ver)[0], (*ver)[1], (*ver)[2]);
	}
      else  {
	Exit(1, "Incompatible version, \"%s\" was made by CRE v%d.%d.%d-B%d!\n",
		argv[1], (*ver)[0], (*ver)[1], (*ver)[2], (*ver)[3]);
	}
      }
    delete ver;
    Graphic *tmpg;
    for(ctr=0; ctr<FRAME_MAX; ctr++)  {
      gg[ctr] = crf.GetGraphic();
      tmpg = crf.GetGraphic(); delete tmpg;
      tmpg = crf.GetGraphic(); delete tmpg;
      gv[ctr] = crf.GetShortBag();
      }
    fromres=1;
    }
  else  {
    Exit(1, "Unable to access \"%s\"!\n", argv[1]);
    }
  screen = new Screen(640, 480);
  screen->SetFrameRate(10);
  user = new User;

  if(Directory == NULL)  {
    Directory = new char[strlen(ResourceFile)+1];
    strcpy(Directory, ResourceFile);
    Directory[strlen(ResourceFile)-4] = 0;
    }

  screen->GetPalette("system/base.pal");
  Graphic backg("system/backg.bmp", screen->GetPalette());
  screen->FullScreenGraphic(backg);
  spg = new Graphic("system/space.bmp", screen->GetPalette());
  spg->FindTrueCenter();
  space = new Sprite; space->UseImage(spg);
  atc = new Graphic("system/allteams.bmp");
  sprintf(buf, "%s/team.bmp%c", Directory, 0);
  tc = new Graphic(buf, screen->GetPalette());
  screen->SetFont("system/basic10.sgf");

  debug_position = 12;
  Mouse mouse;
  mouse.ShowCursor();
  mouse.SetBehavior(MOUSE_CLICK, MOUSE_CLICK, MOUSE_CLICK);
  for(ctrw=0; ctrw<MAX_WEAP; ctrw++)  {
    for(ctr=0; ctr<64; ctr++)  {
      sw[ctrw][ctr] = NULL;
      }
    }
  for(ctrw=0; ctrw<MAX_WEAP; ctrw++)  if(weapn[ctrw] != NULL)  {
    sprintf(buf, "weapons/%s%c", weapn[ctrw], 0);
    for(ctr=0; ctr<64; ctr++)  {
      char buf2[64];
      sprintf(buf2, "%s%.2d.bmp%c", buf, ctr, 0);
      if(access(buf2, R_OK)) {
	sw[ctrw][ctr] = NULL;
	ctr=64;
	}
      else  {
	sw[ctrw][ctr] = new Graphic(buf2, screen->GetPalette());
	sw[ctrw][ctr]->FindTrueCenter();
	}
      }
    }
  if(fromres == 0)  {
    for(ctr=0; ctr<FRAME_MAX; ctr++)  { gg[ctr] = NULL; vfn[ctr] = NULL; }
    LoadImage(FRAME_STAND, "stand");
    LoadAnimation(FRAME_WALKL, "walkl", 0, 16);
    LoadAnimation(FRAME_WALKR, "walkr", 0, 16);

    LoadAnimation(FRAME_DOWN, "down", 0, 1);
    LoadAnimation(FRAME_SPLAT, "splat", 0, 1);

    LoadAnimation(FRAME_STORE, "store", 0, 16);

    LoadAnimation(FRAME_IGNP, "ignp", 0, 16);
    LoadAnimation(FRAME_IGN, "ign", 0, 16);

    LoadAnimation(FRAME_PRAY, "pray", 0, 16);
    LoadAnimation(FRAME_CAST, "cast", 0, 16);
    LoadAnimation(FRAME_VEST, "vest", 0, 16);

    LoadAnimation(FRAME_EXTP, "extp", 0, 16); 
    LoadAnimation(FRAME_EXT, "ext", 0, 16);

    LoadAnimation(FRAME_FARMP, "farmp", 0, 16); 
    LoadAnimation(FRAME_FARM, "farm", 0, 16);

    LoadAnimation(FRAME_DIGP, "digp", 0, 16); 
    LoadAnimation(FRAME_DIG, "dig", 0, 16);

    LoadAnimation(FRAME_CHOPP, "chopp", 0, 16);
    LoadAnimation(FRAME_CHOP, "chop", 0, 16);

    LoadAnimation(FRAME_WORKP, "workp", 0, 16);
    LoadAnimation(FRAME_WORK, "work", 0, 16);

    LoadAnimation(FRAME_BUILDP, "buildp", 0, 16);
    LoadAnimation(FRAME_BUILD, "build", 0, 16);

    LoadAnimation(FRAME_MINEP, "minep", 0, 16);
    LoadAnimation(FRAME_MINE, "mine", 0, 16);

    LoadAnimation(FRAME_TEACHP, "teachp", 0, 16);
    LoadAnimation(FRAME_TEACH, "teach", 0, 16);

    LoadAnimation(FRAME_LEARNP, "learnp", 0, 16);
    LoadAnimation(FRAME_LEARN, "learn", 0, 16);

    LoadAnimation(FRAME_OPERP, "operp", 0, 16);
    LoadAnimation(FRAME_OPER, "oper", 0, 16);

    LoadAnimation(FRAME_GRAB, "grab", 0, 16);

    for(ctrw=0; ctrw<MAX_WEAP; ctrw++)  {
      LoadAnimation(ctrw, FRAME_STANCE, "stanc", 0, 16);

      LoadAnimation(ctrw, FRAME_RUNL, "runl", 1, 16);
      LoadAnimation(ctrw, FRAME_RUNR, "runr", 1, 16);

      LoadAnimation(ctrw, FRAME_ATTP, "attp", 0, 16); 
      LoadAnimation(ctrw, FRAME_ATT, "att", 0, 16);

      LoadAnimation(ctrw, FRAME_BASHP, "bashp", 0, 16); 
      LoadAnimation(ctrw, FRAME_BASH, "bash", 0, 16);

      LoadAnimation(ctrw, FRAME_KICKP, "kickp", 0, 16); 
      LoadAnimation(ctrw, FRAME_KICK, "kick", 0, 16);
      }

    for(ctr=0; ctr<FRAME_MAX; ctr++) if(gg[ctr] != NULL) gg[ctr]->FindTrueCenter();
    for(ctr=0; ctr<FRAME_MAX; ctr++) if(gg[ctr] != NULL) gg[ctr]->Trim();
    }
  g = new Sprite;
  s = new Sprite;
  unsigned char remap[256];
  g->SetColormap((char *)remap);
  for(ctr=0; ctr<256; ctr++)  remap[ctr] = ctr;
  remap[tc->image[0][0]] = atc->image[1][0];
  remap[tc->image[0][1]] = atc->image[1][1];
  remap[tc->image[1][0]] = atc->image[2][0];
  remap[tc->image[1][1]] = atc->image[2][1];

  frame=FRAME_STAND;
  g->UseImage(gg[frame]);
  g->SetPriority(1000);
  g->Move(100, 100);
  DrawWeapon();

  cval cd, cl, cb, ct;
  cd = screen->GetPalette().GetClosestColor(51, 0, 0);
  cb = screen->GetPalette().GetClosestColor(153, 102, 51);
  cl = screen->GetPalette().GetClosestColor(204, 153, 102);
  ct = screen->GetPalette().GetClosestColor(102, 51, 0);
  
  Button QuitB(128, 16, "Quit (ESC)", ct, cl, cb, cd);
   QuitB.Move(0, 16);
   user->MapKeyToButton(SCAN_ESC, QuitB);
  Button ResetB(128, 16, "Reset Pos (BackSpace)", ct, cl, cb, cd);
   ResetB.Move(128, 16);
   user->MapKeyToButton(SCAN_BACKSP, ResetB);
  Button SaveB(128, 16, "Build CRF (F12)", ct, cl, cb, cd);
   SaveB.Move(256, 16);
   user->MapKeyToButton(SCAN_F12, SaveB);
  Button RebuildB(128, 16, "Save VAL files (F11)", ct, cl, cb, cd);
  RebuildB.Move(384, 16);
  user->MapKeyToButton(SCAN_F11, RebuildB);

  Button *WeapB[MAX_WEAP];
  for(ctrw=0; ctrw<MAX_WEAP; ctrw++)  {
    WeapB[ctrw] = new Button(64, 16, weapn[ctrw], ct, cl, cb, cd);
    WeapB[ctrw]->Move(ctrw*64, 336);
    user->MapKeyToButton(weapk[ctrw], *WeapB[ctrw]);
    }

  Button WalkB(64, 16, "Walk", ct, cl, cb, cd);
   WalkB.Move(0, 384);
   user->MapKeyToButton(SCAN_W, WalkB);
  Button WorkB(64, 16, "worK", ct, cl, cb, cd);
   WorkB.Move(64, 384);
   user->MapKeyToButton(SCAN_K, WorkB);
  Button BuildB(64, 16, "Build", ct, cl, cb, cd);
   BuildB.Move(128, 384);
   user->MapKeyToButton(SCAN_B, BuildB);
  Button FarmB(64, 16, "Farm", ct, cl, cb, cd);
   FarmB.Move(192, 384);
   user->MapKeyToButton(SCAN_F, FarmB);
  Button DigB(64, 16, "Dig", ct, cl, cb, cd);
   DigB.Move(256, 384);
   user->MapKeyToButton(SCAN_D, DigB);
  Button ChopB(64, 16, "cHop", ct, cl, cb, cd);
   ChopB.Move(320, 384);
   user->MapKeyToButton(SCAN_H, ChopB);

  Button IgnB(64, 16, "Ignite", ct, cl, cb, cd);
   IgnB.Move(0, 400);
   user->MapKeyToButton(SCAN_I, IgnB);
  Button ExtB(64, 16, "eXtinguish", ct, cl, cb, cd);
   ExtB.Move(64, 400);
   user->MapKeyToButton(SCAN_X, ExtB);
  Button MineB(64, 16, "Mine", ct, cl, cb, cd);
   MineB.Move(128, 400);
   user->MapKeyToButton(SCAN_M, MineB);
  Button OperB(64, 16, "Operate", ct, cl, cb, cd);
   OperB.Move(192, 400);
   user->MapKeyToButton(SCAN_O, OperB);
  Button GrabB(64, 16, "Grab", ct, cl, cb, cd);
   GrabB.Move(256, 400);
   user->MapKeyToButton(SCAN_G, GrabB);

  Button CastB(64, 16, "Cast", ct, cl, cb, cd);
   CastB.Move(0, 416);
   user->MapKeyToButton(SCAN_C, CastB);
  Button PrayB(64, 16, "Pray", ct, cl, cb, cd);
   PrayB.Move(64, 416);
   user->MapKeyToButton(SCAN_P, PrayB);
  Button VestB(64, 16, "Vest", ct, cl, cb, cd);
   VestB.Move(128, 416);
   user->MapKeyToButton(SCAN_V, VestB);
  Button TeachB(64, 16, "Teach", ct, cl, cb, cd);
   TeachB.Move(192, 416);
   user->MapKeyToButton(SCAN_T, TeachB);
  Button LearnB(64, 16, "Learn", ct, cl, cb, cd);
   LearnB.Move(256, 416);
   user->MapKeyToButton(SCAN_L, LearnB);

  StanceB = new Button(64, 16, "Stance", ct, cl, cb, cd);
   StanceB->Stick(); StanceB->Move(0, 448);
   user->MapKeyToButton(SCAN_S, *StanceB);
  Button RunB(64, 16, "Run", ct, cl, cb, cd);
   RunB.Move(64, 448);
   user->MapKeyToButton(SCAN_R, RunB);
  Button AttB(64, 16, "attack(A)", ct, cl, cb, cd);
   AttB.Move(128, 448);
   user->MapKeyToButton(SCAN_A, AttB);
  Button BashB(64, 16, "bash(Z)", ct, cl, cb, cd);
   BashB.Move(192, 448);
   user->MapKeyToButton(SCAN_Z, BashB);
  Button KickB(64, 16, "kick(Q)", ct, cl, cb, cd);
   KickB.Move(256, 448);
   user->MapKeyToButton(SCAN_Q, KickB);

  FrameB = new Button(192, 16, "Frame by Frame (TAB)", ct, cl, cb, cd);
   FrameB->Stick(); FrameB->Move(448, 368);
   user->MapKeyToButton(SCAN_TAB, *FrameB);
  LeftB = new Button(64, 16, "Left", ct, cl, cb, cd);
   LeftB->Move(448, 400);
   user->MapKeyToButton(SCAN_PAD_4, *LeftB);
  RightB = new Button(64, 16, "Right", ct, cl, cb, cd);
   RightB->Move(576, 400);
   user->MapKeyToButton(SCAN_PAD_6, *RightB);
  UpB = new Button(64, 16, "Up", ct, cl, cb, cd);
   UpB->Move(512, 384);
   user->MapKeyToButton(SCAN_PAD_8, *UpB);
  DownB = new Button(64, 16, "Down", ct, cl, cb, cd);
   DownB->Move(512, 416);
   user->MapKeyToButton(SCAN_PAD_2, *DownB);
  ClockB = new Button(64, 16, "Clockwise", ct, cl, cb, cd);
   ClockB->Move(448, 384);
   user->MapKeyToButton(SCAN_PAD_9, *ClockB);
  CClockB = new Button(64, 16, "CntrClkws", ct, cl, cb, cd);
   CClockB->Move(576, 384);
   user->MapKeyToButton(SCAN_PAD_7, *CClockB);
  TopB = new Button(64, 16, "On Top", ct, cl, cb, cd);
   TopB->Move(512, 400);
   user->MapKeyToButton(SCAN_PAD_5, *TopB);
  PrevB = new Button(64, 16, "Prev Grap", ct, cl, cb, cd);
   PrevB->Move(448, 416);
   user->MapKeyToButton(SCAN_PAD_1, *PrevB);
  NextB = new Button(64, 16, "Next Grap", ct, cl, cb, cd);
   NextB->Move(576, 416);
   user->MapKeyToButton(SCAN_PAD_3, *NextB);

  int NUM_TEAMCOLORS = atc->ysize;
  Button *Clrs1[NUM_TEAMCOLORS], *Clrs2[NUM_TEAMCOLORS];
  for(ctr=0; ctr<NUM_TEAMCOLORS; ctr++)  {
    Clrs1[ctr] = new Button(16, 16, "", 0,
	atc->image[ctr][0], atc->image[ctr][1], atc->image[ctr][0]);
    Clrs1[ctr]->Move(624-16*ctr, 448);
    }
  for(ctr=0; ctr<NUM_TEAMCOLORS; ctr++)  {
    Clrs2[ctr] = new Button(16, 16, "", 0,
	atc->image[ctr][0], atc->image[ctr][1], atc->image[ctr][0]);
    Clrs2[ctr]->Move(624-16*ctr, 464);
    }

  screen->ShowScreen();
  screen->RefreshFast();
  int quit = 0;
  UserAction act;
  while(!quit)  {
    debug_position = 13;
    screen->RefreshFast();
    debug_position = 14;
    act = user->Action();
    debug_position = 15;
    if(act.Type() == USERACTION_BUTTONRELEASED) {
      debug_position = 16;
      if(act.ButtonPressed() == StanceB->SpriteNumber())  {
	if(stance) UnStance();
	}
      else {
	WeaponManipEvent(act);
	}
      }
    else if(act.Type() == USERACTION_BUTTONPRESSED) {
      debug_position = 17;
      if(act.ButtonPressed() >= Clrs1[0]->SpriteNumber()
	    && act.ButtonPressed() <= Clrs1[7]->SpriteNumber())  {
	int sel =  act.ButtonPressed() - Clrs1[0]->SpriteNumber();
	remap[tc->image[0][0]] = atc->image[sel][0];
	remap[tc->image[0][1]] = atc->image[sel][1];
        g->Erase();  g->Draw();
	}
      else if(act.ButtonPressed() >= Clrs2[0]->SpriteNumber()
	    && act.ButtonPressed() <= Clrs2[7]->SpriteNumber())  {
	int sel =  act.ButtonPressed() - Clrs2[0]->SpriteNumber();
	remap[tc->image[1][0]] = atc->image[sel][0];
	remap[tc->image[1][1]] = atc->image[sel][1];
        g->Erase();  g->Draw();
	}
      else if(act.ButtonPressed() == StanceB->SpriteNumber())  {
	if(!stance) Stance();
	}
      else if(act.ButtonPressed() == WalkB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayLoopM(FRAME_WALKL+1);
	xpos+=4; SetFrame(FRAME_WALKL);
//	xpos+=4; ResetFrame();
	PlayLoopM(FRAME_WALKR+1);
	xpos+=4; SetFrame(FRAME_WALKR);
//	xpos+=4; ResetFrame();
	}
      else if(act.ButtonPressed() == ExtB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_EXTP);
	PlayLoop(FRAME_EXT);
	PlayLoop(FRAME_EXT);
	PlayDown(FRAME_EXTP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == IgnB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_IGNP);
	PlayLoop(FRAME_IGN);
	PlayLoop(FRAME_IGN);
	PlayDown(FRAME_IGNP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == FarmB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_FARMP);
	PlayLoop(FRAME_FARM);
	PlayLoop(FRAME_FARM);
	PlayDown(FRAME_FARMP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == DigB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_DIGP);
	PlayLoop(FRAME_DIG);
	PlayLoop(FRAME_DIG);
	PlayDown(FRAME_DIGP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == ChopB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_CHOPP);
	PlayLoop(FRAME_CHOP);
	PlayLoop(FRAME_CHOP);
	PlayDown(FRAME_CHOPP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == WorkB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_WORKP);
	PlayLoop(FRAME_WORK);
	PlayLoop(FRAME_WORK);
	PlayDown(FRAME_WORKP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == BuildB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_BUILDP);
	PlayLoop(FRAME_BUILD);
	PlayLoop(FRAME_BUILD);
	PlayDown(FRAME_BUILDP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == TeachB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_TEACHP);
	PlayLoop(FRAME_TEACH);
	PlayLoop(FRAME_TEACH);
	PlayDown(FRAME_TEACHP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == LearnB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_LEARNP);
	PlayLoop(FRAME_LEARN);
	PlayLoop(FRAME_LEARN);
	PlayDown(FRAME_LEARNP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == MineB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_MINEP);
	PlayLoop(FRAME_MINE);
	PlayLoop(FRAME_MINE);
	PlayDown(FRAME_MINEP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == OperB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_OPERP);
	PlayLoop(FRAME_OPER);
	PlayLoop(FRAME_OPER);
	PlayDown(FRAME_OPERP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == GrabB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayLoop(FRAME_GRAB);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == CastB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayLoop(FRAME_CAST);
	ResetFrame();
	}
      else if(act.ButtonPressed() == PrayB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayLoop(FRAME_PRAY);
	ResetFrame();
	}
      else if(act.ButtonPressed() == VestB.SpriteNumber())  {
	if(stance)  UnStance();
	PlayLoop(FRAME_VEST);
	ResetFrame();
	}
      else if(act.ButtonPressed() >= WeapB[0]->SpriteNumber()
	     && act.ButtonPressed() <= WeapB[MAX_WEAP-1]->SpriteNumber())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	weapon = act.ButtonPressed() - WeapB[0]->SpriteNumber();
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act.ButtonPressed() == RunB.SpriteNumber())  {
	if(!stance)  Stance();
	PlayLoopM((weapon*WEAP_INC)+FRAME_RUNL+1);
	xpos+=4; ResetFrame();
	PlayLoopM((weapon*WEAP_INC)+FRAME_RUNR+1);
	xpos+=4; ResetFrame();
	//Exit(1, "Did reset!\n");
	}
      else if(act.ButtonPressed() == AttB.SpriteNumber())  {
	if(!stance)  Stance();
	PlayUp((weapon*WEAP_INC)+FRAME_ATTP);
	PlayUp((weapon*WEAP_INC)+FRAME_ATT);
	ResetFrame();
	}
      else if(act.ButtonPressed() == KickB.SpriteNumber())  {
	if(!stance)  Stance();
	PlayUp((weapon*WEAP_INC)+FRAME_KICKP);
	PlayUp((weapon*WEAP_INC)+FRAME_KICK);
	ResetFrame();
	}
      else if(act.ButtonPressed() == BashB.SpriteNumber())  {
	if(!stance)  Stance();
	PlayUp((weapon*WEAP_INC)+FRAME_BASHP);
	PlayUp((weapon*WEAP_INC)+FRAME_BASH);
	ResetFrame();
	}
      else if(act.ButtonPressed() == ResetB.SpriteNumber())  {
	xpos=100;
	stance=0;
	ResetFrame();
	}
      else if(act.ButtonPressed() == RebuildB.SpriteNumber())  {
	int ctr;
	for(ctr=0; ctr<FRAME_MAX; ctr++)  {
	  if(vfn[ctr] != NULL)  {
	    FILE *vf = fopen(vfn[ctr], "w");
	    fprintf(vf, "%d\n%d\n%d\n%d\n", (*gv[ctr])[0],
		(*gv[ctr])[1], (*gv[ctr])[2], (*gv[ctr])[3]);
	    fclose(vf);
	    }
	  }
	}
      else if(act.ButtonPressed() == SaveB.SpriteNumber())  {
	int ctr;
	NewResFile crf(ResourceFile);
	CharBag ver(4);
	ver[0] = VER1; ver[1] = VER2; ver[2] = VER3; ver[3] = VER4;
	crf.Add(&ver);
	Graphic *tmpg1, *tmpg2;
	for(ctr=0; ctr<FRAME_MAX; ctr++)  {
	  crf.Add(gg[ctr]);
	  if(gg[ctr] != NULL)  {
	    tmpg1 = new Graphic(gg[ctr]->HalfSize()); crf.Add(tmpg1);
	    tmpg2 = new Graphic(tmpg1->HalfSize()); crf.Add(tmpg2);
	    delete tmpg1; delete tmpg2;
	    }
	  else  { crf.Add(gg[ctr]); crf.Add(gg[ctr]); }
	  crf.Add(gv[ctr]);
	  }
	}
      else if(act.ButtonPressed() == QuitB.SpriteNumber()) {
	quit++;
	}
      else {
	QuickResetFrame();
	WeaponManipEvent(act);
	}
      }
    else {
      QuickResetFrame();
      WeaponManipEvent(act);
      }
    debug_position = 20;
    }
  return 0;
  }

void QuickResetFrame()  {
  if(stance) frame=weapon*WEAP_INC + FRAME_STANCE;
  else frame=FRAME_STAND;
  g->UseImage(gg[frame]);
  g->SetPriority(1000);
  g->Move(xpos, 100);
  DrawWeapon();
  if((!stance) && StanceB->IsPressed())  StanceB->StealthClick();
  else if(stance && (!StanceB->IsPressed()))  StanceB->StealthClick();
  screen->Refresh();
  }

void ResetFrame()  {
  QuickResetFrame();
  OnFrame();
  }

void PlayUp(int fbase)  {
  for(frame=fbase; gg[frame] != NULL; ++frame)  {
    g->UseImage(gg[frame]);
    g->Move(xpos, 100);
    DrawWeapon();
    screen->Refresh();
    OnFrame();
    }
  }

void PlayDown(int fbase)  {
  for(frame=fbase; gg[frame] != NULL; ++frame);
  --frame;
  for(; frame>=fbase; --frame)  {
    g->UseImage(gg[frame]);
    g->Move(xpos, 100);
    DrawWeapon();
    screen->Refresh();
    OnFrame();
    }
  }

void PlayLoop(int fbase)  {
  for(frame=fbase; gg[frame]!= NULL; ++frame)  {
    g->UseImage(gg[frame]);
    g->Move(xpos, 100);
    DrawWeapon();
    screen->Refresh();
    OnFrame();
    }
  --frame; --frame;
  for(; frame>=fbase; --frame)  {
    g->UseImage(gg[frame]);
    g->Move(xpos, 100);
    DrawWeapon();
    screen->Refresh();
    OnFrame();
    }
  }

void PlayUpM(int fbase)  {
  }

void PlayDownM(int fbase)  {
  }

void SetFrame(int fbase)  {
  frame = fbase;
  g->UseImage(gg[frame]);
  DrawWeapon();
  g->Move(xpos, 100);
  screen->Refresh();
  OnFrame();
  }

void PlayLoopM(int fbase)  {
  for(frame=fbase; gg[frame]!= NULL; ++frame)  {
    xpos += 4;
    g->UseImage(gg[frame]);
    DrawWeapon();
    g->Move(xpos, 100);
    screen->Refresh();
    OnFrame();
    }
  --frame; --frame;
  for(; frame>=fbase; --frame)  {
    xpos += 4;
    g->UseImage(gg[frame]);
    DrawWeapon();
    g->Move(xpos, 100);
    screen->Refresh();
    OnFrame();
    }
  }

void LoadImage(int fnum, const char *fn)  {
  char buf[64];
  sprintf(buf, "%s/%s.bmp%c", Directory, fn, 0);
  if(access(buf, R_OK)) { gg[fnum]=NULL; gv[fnum]=NULL; }
  else  {
    Graphic *tmpg = new Graphic(buf, screen->GetPalette());
    gg[fnum] = new Graphic(tmpg->RotatedCounterClock());
    delete tmpg;
//    gg[fnum] = new Graphic(buf, screen->GetPalette());
    sprintf(buf, "%s/%s.val%c", Directory, fn, 0);
    gv[fnum] = new ShortBag(4);
    vfn[fnum] = new char[strlen(buf)+2];
    sprintf(vfn[fnum], "%s%c", buf, 0);
    if(access(buf, R_OK)) {
      (*gv[fnum])[0] = 0;
      (*gv[fnum])[1] = 5;
      (*gv[fnum])[2] = 27;
      (*gv[fnum])[3] = 0;
      }
    else  {
      int tmp1, tmp2, tmp3, tmp4;
      FILE *fl = fopen(buf, "r");
      fscanf(fl, "%d %d %d %d", &tmp1, &tmp2, &tmp3, &tmp4);
      fclose(fl);
      (*gv[fnum])[0] = tmp1;
      (*gv[fnum])[1] = tmp2;
      (*gv[fnum])[2] = tmp3;
      (*gv[fnum])[3] = tmp4;
      }
    }
  }

void LoadAnimation(int wnum, int fnum, const char *basename, int start, int max)  {
  int ctr;  char buf[64];
  for(ctr=start; ctr<max; ctr++)  {
    sprintf(buf, "%.1d%s%.2d%c", (wnum+1)%10, basename, ctr, 0);
    LoadImage(ctr+fnum+(wnum*WEAP_INC), buf);
    if(gg[ctr+fnum+(wnum*WEAP_INC)] == NULL) ctr=max;
    }
  }

void LoadAnimation(int fnum, const char *basename, int start, int max)  {
  int ctr;  char buf[64];
  for(ctr=start; ctr<max; ctr++)  {
    sprintf(buf, "%s%.2d%c", basename, ctr, 0);
    LoadImage(ctr+fnum, buf);
    if(gg[ctr+fnum] == NULL) ctr=max;
    }
  }

void Stance() {
  PlayDown(weapon*WEAP_INC + FRAME_STANCE);
  stance=1;
  ResetFrame();
  if(!StanceB->IsPressed()) StanceB->StealthClick();
  }

void UnStance() {
  PlayUp(weapon*WEAP_INC + FRAME_STANCE+1);
  stance=0;
  ResetFrame();
  if(StanceB->IsPressed()) StanceB->StealthClick();
  }

void DrawWeapon()  {
  int fm = ((*gv[frame])[0]);
  if(fm >= 0 && weapon > 0 && sw[weapon][fm&255] != NULL)  {
    s->UseImage(sw[weapon][fm & 255]);
    s->SetPriority(1001-(fm>>7));
    s->Move(xpos+(*gv[frame])[1], 100+(*gv[frame])[2], (*gv[frame])[3]);
    }
  else s->Erase();
  }

void OnFrame()  {
  if(!FrameB->IsPressed()) return;
  space->Move(320, 240);
  while(user->IsPressed(SCAN_SPACE)) ManipWeapon();
  while(!user->IsPressed(SCAN_SPACE)) ManipWeapon();
  space->Erase();
  }

void WeaponManipEvent(UserAction act)  {
  if(act.Type() == USERACTION_KEYPRESSED) {
    if(act.Key() >= SCAN_F1 && act.Key() <= SCAN_F10)  {
      int sl = act.Key() - SCAN_F1;
      if(act.ModKeyPressed(0) || act.ModKeyPressed(1))  {
	mem[sl][0] = (*gv[frame])[0];
	mem[sl][1] = (*gv[frame])[1];
	mem[sl][2] = (*gv[frame])[2];
	mem[sl][3] = (*gv[frame])[3];
	}
      else {
	(*gv[frame])[0] = mem[sl][0];
	(*gv[frame])[1] = mem[sl][1];
	(*gv[frame])[2] = mem[sl][2];
	(*gv[frame])[3] = mem[sl][3];
	DrawWeapon();
	}
      }
    }
  if(act.Type() != USERACTION_BUTTONPRESSED) return;
  else if(act.ButtonPressed() == LeftB->SpriteNumber())  {
    (*gv[frame])[1]--;  DrawWeapon();
    }
  else if(act.ButtonPressed() == RightB->SpriteNumber())  {
    (*gv[frame])[1]++;  DrawWeapon();
    }
  else if(act.ButtonPressed() == UpB->SpriteNumber())  {
    (*gv[frame])[2]--;  DrawWeapon();
    }
  else if(act.ButtonPressed() == DownB->SpriteNumber())  {
    (*gv[frame])[2]++;  DrawWeapon();
    }
  else if(act.ButtonPressed() == PrevB->SpriteNumber())  {
    if((*gv[frame])[0] > -1)  {
      (*gv[frame])[0]--;
      if((*gv[frame])[0] > 10 && (*gv[frame])[0] < 256)  (*gv[frame])[0] = -1;
      DrawWeapon();
      }
    }
  else if(act.ButtonPressed() == NextB->SpriteNumber())  {
    if(((*gv[frame])[0] & 255) < 8 || (*gv[frame])[0] < 0)  {
      (*gv[frame])[0]++;  DrawWeapon();
      }
    }
  else if(act.ButtonPressed() == TopB->SpriteNumber())  {
    (*gv[frame])[0] ^= 256;  s->Erase(); DrawWeapon();
    }
  else if(act.ButtonPressed() == ClockB->SpriteNumber())  {
    (*gv[frame])[3] += 256;  DrawWeapon();
    }
  else if(act.ButtonPressed() == CClockB->SpriteNumber())  {
    (*gv[frame])[3] -= 256;  DrawWeapon();
    }
  }

void ManipWeapon()  {
  screen->Refresh();
  UserAction act = user->Action();
  while(act.Type() != USERACTION_NONE)  {
    WeaponManipEvent(act);
    act = user->Action();
    }
  }
