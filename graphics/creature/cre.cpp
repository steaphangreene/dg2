#include <unistd.h>
#include <user.h>
#include <sys/stat.h>

#include "cre.h"

#define MAX_WEAP	10
const char *weapn[MAX_WEAP] = { "punch", "dagger", "sword", "btlaxe",
	"spear", "pike", "cross", "heavcr", "bow", "knife" };
int weapk[MAX_WEAP] = { KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6,
	KEY_7, KEY_8, KEY_9, KEY_0 };

Screen *screen;
InputQueue *input;
Keyboard *key;
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

Stickey *StanceB, *FrameB;
Clickey *LeftB, *RightB, *UpB, *DownB, *PrevB, *NextB, *TopB, *ClockB, *CClockB;

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
void WeaponManipEvent(InputAction *act);
void ManipWeapon();

int main(int argc, char **argv)  {
  InitUserEngine(argc, argv);
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
  screen = new Screen;
  screen->SetApparentDepth(8);
  screen->SetSize(640, 480);
  screen->SetFrameRate(10);
  input = new InputQueue;
  key = new Keyboard;

  if(Directory == NULL)  {
    Directory = new char[strlen(ResourceFile)+1];
    strcpy(Directory, ResourceFile);
    Directory[strlen(ResourceFile)-4] = 0;
    }

  screen->SetPalette("system/base.pal");
  Graphic backg("system/backg.bmp", screen->GetPalette());
  screen->FullScreenGraphic(backg);
  spg = new Graphic("system/space.bmp", screen->GetPalette());
  spg->FindTrueCenter();
  space = new Sprite; space->UseImage(spg);
  atc = new Graphic("system/allteams.bmp");
  sprintf(buf, "%s/team.bmp%c", Directory, 0);
  tc = new Graphic(buf, screen->GetPalette());
  screen->SetFont("system/basic10.sgf");

  Debug("main()  Before Mouse");
  Mouse mouse;
  Graphic cursor("system/cursor.bmp", screen->GetPalette());
  cursor.tcolor = cursor.image[4].uc[0];
  mouse.SetCursor(cursor);
  mouse.ShowCursor();
  mouse.SetBehavior(0, 1, MB_CLICK);
  mouse.SetBehavior(0, 2, MB_CLICK);
  mouse.SetBehavior(0, 3, MB_CLICK);
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
  g->SetColormap(remap);
  for(ctr=0; ctr<256; ctr++)  remap[ctr] = ctr;
  remap[tc->image[0].uc[0]] = atc->image[1].uc[0];
  remap[tc->image[0].uc[1]] = atc->image[1].uc[1];
  remap[tc->image[1].uc[0]] = atc->image[2].uc[0];
  remap[tc->image[1].uc[1]] = atc->image[2].uc[1];

  frame=FRAME_STAND;
  g->UseImage(gg[frame]);
  g->SetPriority(1000);
  g->Move(100, 100);
  DrawWeapon();

  color cd, cl, cb, ct;
  Palette p = screen->GetPalette();
  cd = p.GetClosestColor(51, 0, 0);
  cb = p.GetClosestColor(153, 102, 51);
  cl = p.GetClosestColor(204, 153, 102);
  ct = p.GetClosestColor(102, 51, 0);

  Clickey QuitB(128, 16, "Quit (ESC)", ct, cl, cb, cd);
   QuitB.Move(0, 16);
   input->MapKeyToControl(KEY_ESC, QuitB);

  Clickey ResetB(128, 16, "Reset Pos (BackSpace)", ct, cl, cb, cd);
   ResetB.Move(128, 16);
   input->MapKeyToControl(KEY_BACKSP, ResetB);
  Clickey SaveB(128, 16, "Build CRF (F12)", ct, cl, cb, cd);
   SaveB.Move(256, 16);
   input->MapKeyToControl(KEY_F12, SaveB);
  Clickey RebuildB(128, 16, "Save VAL files (F11)", ct, cl, cb, cd);
  RebuildB.Move(384, 16);
  input->MapKeyToControl(KEY_F11, RebuildB);

  Clickey *WeapB[MAX_WEAP];
  for(ctrw=0; ctrw<MAX_WEAP; ctrw++)  {
    WeapB[ctrw] = new Clickey(64, 16, weapn[ctrw], ct, cl, cb, cd);
    WeapB[ctrw]->Move(ctrw*64, 336);
    input->MapKeyToControl(weapk[ctrw], *WeapB[ctrw]);
    }

  Clickey WalkB(64, 16, "Walk", ct, cl, cb, cd);
   WalkB.Move(0, 384);
   input->MapKeyToControl(KEY_W, WalkB);
  Clickey WorkB(64, 16, "worK", ct, cl, cb, cd);
   WorkB.Move(64, 384);
   input->MapKeyToControl(KEY_K, WorkB);
  Clickey BuildB(64, 16, "Build", ct, cl, cb, cd);
   BuildB.Move(128, 384);
   input->MapKeyToControl(KEY_B, BuildB);
  Clickey FarmB(64, 16, "Farm", ct, cl, cb, cd);
   FarmB.Move(192, 384);
   input->MapKeyToControl(KEY_F, FarmB);
  Clickey DigB(64, 16, "Dig", ct, cl, cb, cd);
   DigB.Move(256, 384);
   input->MapKeyToControl(KEY_D, DigB);
  Clickey ChopB(64, 16, "cHop", ct, cl, cb, cd);
   ChopB.Move(320, 384);
   input->MapKeyToControl(KEY_H, ChopB);

  Clickey IgnB(64, 16, "Ignite", ct, cl, cb, cd);
   IgnB.Move(0, 400);
   input->MapKeyToControl(KEY_I, IgnB);
  Clickey ExtB(64, 16, "eXtinguish", ct, cl, cb, cd);
   ExtB.Move(64, 400);
   input->MapKeyToControl(KEY_X, ExtB);
  Clickey MineB(64, 16, "Mine", ct, cl, cb, cd);
   MineB.Move(128, 400);
   input->MapKeyToControl(KEY_M, MineB);
  Clickey OperB(64, 16, "Operate", ct, cl, cb, cd);
   OperB.Move(192, 400);
   input->MapKeyToControl(KEY_O, OperB);
  Clickey GrabB(64, 16, "Grab", ct, cl, cb, cd);
   GrabB.Move(256, 400);
   input->MapKeyToControl(KEY_G, GrabB);

  Clickey CastB(64, 16, "Cast", ct, cl, cb, cd);
   CastB.Move(0, 416);
   input->MapKeyToControl(KEY_C, CastB);
  Clickey PrayB(64, 16, "Pray", ct, cl, cb, cd);
   PrayB.Move(64, 416);
   input->MapKeyToControl(KEY_P, PrayB);
  Clickey VestB(64, 16, "Vest", ct, cl, cb, cd);
   VestB.Move(128, 416);
   input->MapKeyToControl(KEY_V, VestB);
  Clickey TeachB(64, 16, "Teach", ct, cl, cb, cd);
   TeachB.Move(192, 416);
   input->MapKeyToControl(KEY_T, TeachB);
  Clickey LearnB(64, 16, "Learn", ct, cl, cb, cd);
   LearnB.Move(256, 416);
   input->MapKeyToControl(KEY_L, LearnB);

  StanceB = new Stickey(64, 16, "Stance", ct, cl, cb, cd);
   StanceB->Move(0, 448);
   input->MapKeyToControl(KEY_S, *StanceB);
  Clickey RunB(64, 16, "Run", ct, cl, cb, cd);
   RunB.Move(64, 448);
   input->MapKeyToControl(KEY_R, RunB);
  Clickey AttB(64, 16, "attack(A)", ct, cl, cb, cd);
   AttB.Move(128, 448);
   input->MapKeyToControl(KEY_A, AttB);
  Clickey BashB(64, 16, "bash(Z)", ct, cl, cb, cd);
   BashB.Move(192, 448);
   input->MapKeyToControl(KEY_Z, BashB);
  Clickey KickB(64, 16, "kick(Q)", ct, cl, cb, cd);
   KickB.Move(256, 448);
   input->MapKeyToControl(KEY_Q, KickB);

  FrameB = new Stickey(192, 16, "Frame by Frame (TAB)", ct, cl, cb, cd);
   FrameB->Move(448, 368);
   input->MapKeyToControl(KEY_TAB, *FrameB);
  LeftB = new Clickey(64, 16, "Left", ct, cl, cb, cd);
   LeftB->Move(448, 400);
   input->MapKeyToControl(KEY_PAD_4, *LeftB);
  RightB = new Clickey(64, 16, "Right", ct, cl, cb, cd);
   RightB->Move(576, 400);
   input->MapKeyToControl(KEY_PAD_6, *RightB);
  UpB = new Clickey(64, 16, "Up", ct, cl, cb, cd);
   UpB->Move(512, 384);
   input->MapKeyToControl(KEY_PAD_8, *UpB);
  DownB = new Clickey(64, 16, "Down", ct, cl, cb, cd);
   DownB->Move(512, 416);
   input->MapKeyToControl(KEY_PAD_2, *DownB);
  ClockB = new Clickey(64, 16, "Clockwise", ct, cl, cb, cd);
   ClockB->Move(448, 384);
   input->MapKeyToControl(KEY_PAD_9, *ClockB);
  CClockB = new Clickey(64, 16, "CntrClkws", ct, cl, cb, cd);
   CClockB->Move(576, 384);
   input->MapKeyToControl(KEY_PAD_7, *CClockB);
  TopB = new Clickey(64, 16, "On Top", ct, cl, cb, cd);
   TopB->Move(512, 400);
   input->MapKeyToControl(KEY_PAD_5, *TopB);
  PrevB = new Clickey(64, 16, "Prev Grap", ct, cl, cb, cd);
   PrevB->Move(448, 416);
   input->MapKeyToControl(KEY_PAD_1, *PrevB);
  NextB = new Clickey(64, 16, "Next Grap", ct, cl, cb, cd);
   NextB->Move(576, 416);
   input->MapKeyToControl(KEY_PAD_3, *NextB);

  int NUM_TEAMCOLORS = atc->ysize;
  Clickey *Clrs1[NUM_TEAMCOLORS], *Clrs2[NUM_TEAMCOLORS];
  for(ctr=0; ctr<NUM_TEAMCOLORS; ctr++)  {
    Clrs1[ctr] = new Clickey(16, 16, "", 0,
	atc->image[ctr].uc[0], atc->image[ctr].uc[1], atc->image[ctr].uc[0]);
    Clrs1[ctr]->Move(624-16*ctr, 448);
    }
  for(ctr=0; ctr<NUM_TEAMCOLORS; ctr++)  {
    Clrs2[ctr] = new Clickey(16, 16, "", 0,
	atc->image[ctr].uc[0], atc->image[ctr].uc[1], atc->image[ctr].uc[0]);
    Clrs2[ctr]->Move(624-16*ctr, 464);
    }

  screen->Show();
  screen->RefreshFast();
  int quit = 0;
  InputAction *act;
  while(!quit)  {
    screen->RefreshFast();
//    static int tmp = 0;
//    ++tmp;
//    if(tmp > 1000000) quit = 1;
//    }
//  if(0) {
    do { screen->Refresh(); act = input->NextAction(); } while(act==NULL);
    if(act->g.type == INPUTACTION_CONTROLUP) {
      if(act->c.control == StanceB->Number())  {
	if(stance) UnStance();
	}
      else {
	WeaponManipEvent(act);
	}
      }
    else if(act->g.type == INPUTACTION_CONTROLDOWN) {
      if(act->c.control >= Clrs1[0]->Number()
	    && act->c.control <= Clrs1[9]->Number())  {
	int sel =  act->c.control - Clrs1[0]->Number();
	remap[tc->image[0].uc[0]] = atc->image[sel].uc[0];
	remap[tc->image[0].uc[1]] = atc->image[sel].uc[1];
        g->Erase();  g->Draw();
	}
      else if(act->c.control >= Clrs2[0]->Number()
	    && act->c.control <= Clrs2[9]->Number())  {
	int sel =  act->c.control - Clrs2[0]->Number();
	remap[tc->image[1].uc[0]] = atc->image[sel].uc[0];
	remap[tc->image[1].uc[1]] = atc->image[sel].uc[1];
        g->Erase();  g->Draw();
	}
      else if(act->c.control == StanceB->Number())  {
	if(!stance) Stance();
	}
      else if(act->c.control == WalkB.Number())  {
	if(stance)  UnStance();
	PlayLoopM(FRAME_WALKL+1);
	xpos+=4; SetFrame(FRAME_WALKL);
//	xpos+=4; ResetFrame();
	PlayLoopM(FRAME_WALKR+1);
	xpos+=4; SetFrame(FRAME_WALKR);
//	xpos+=4; ResetFrame();
	}
      else if(act->c.control == ExtB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_EXTP);
	PlayLoop(FRAME_EXT);
	PlayLoop(FRAME_EXT);
	PlayDown(FRAME_EXTP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == IgnB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_IGNP);
	PlayLoop(FRAME_IGN);
	PlayLoop(FRAME_IGN);
	PlayDown(FRAME_IGNP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == FarmB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_FARMP);
	PlayLoop(FRAME_FARM);
	PlayLoop(FRAME_FARM);
	PlayDown(FRAME_FARMP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == DigB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_DIGP);
	PlayLoop(FRAME_DIG);
	PlayLoop(FRAME_DIG);
	PlayDown(FRAME_DIGP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == ChopB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_CHOPP);
	PlayLoop(FRAME_CHOP);
	PlayLoop(FRAME_CHOP);
	PlayDown(FRAME_CHOPP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == WorkB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_WORKP);
	PlayLoop(FRAME_WORK);
	PlayLoop(FRAME_WORK);
	PlayDown(FRAME_WORKP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == BuildB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_BUILDP);
	PlayLoop(FRAME_BUILD);
	PlayLoop(FRAME_BUILD);
	PlayDown(FRAME_BUILDP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == TeachB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_TEACHP);
	PlayLoop(FRAME_TEACH);
	PlayLoop(FRAME_TEACH);
	PlayDown(FRAME_TEACHP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == LearnB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_LEARNP);
	PlayLoop(FRAME_LEARN);
	PlayLoop(FRAME_LEARN);
	PlayDown(FRAME_LEARNP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == MineB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_MINEP);
	PlayLoop(FRAME_MINE);
	PlayLoop(FRAME_MINE);
	PlayDown(FRAME_MINEP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == OperB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayUp(FRAME_OPERP);
	PlayLoop(FRAME_OPER);
	PlayLoop(FRAME_OPER);
	PlayDown(FRAME_OPERP);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == GrabB.Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	PlayLoop(FRAME_GRAB);
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == CastB.Number())  {
	if(stance)  UnStance();
	PlayLoop(FRAME_CAST);
	ResetFrame();
	}
      else if(act->c.control == PrayB.Number())  {
	if(stance)  UnStance();
	PlayLoop(FRAME_PRAY);
	ResetFrame();
	}
      else if(act->c.control == VestB.Number())  {
	if(stance)  UnStance();
	PlayLoop(FRAME_VEST);
	ResetFrame();
	}
      else if(act->c.control >= WeapB[0]->Number()
	     && act->c.control <= WeapB[MAX_WEAP-1]->Number())  {
	if(stance)  UnStance();
	PlayDown(FRAME_STORE);
	weapon = act->c.control - WeapB[0]->Number();
	PlayUp(FRAME_STORE);
	ResetFrame();
	}
      else if(act->c.control == RunB.Number())  {
	if(!stance)  Stance();
	PlayLoopM((weapon*WEAP_INC)+FRAME_RUNL+1);
	xpos+=4; ResetFrame();
	PlayLoopM((weapon*WEAP_INC)+FRAME_RUNR+1);
	xpos+=4; ResetFrame();
	//Exit(1, "Did reset!\n");
	}
      else if(act->c.control == AttB.Number())  {
	if(!stance)  Stance();
	PlayUp((weapon*WEAP_INC)+FRAME_ATTP);
	PlayUp((weapon*WEAP_INC)+FRAME_ATT);
	ResetFrame();
	}
      else if(act->c.control == KickB.Number())  {
	if(!stance)  Stance();
	PlayUp((weapon*WEAP_INC)+FRAME_KICKP);
	PlayUp((weapon*WEAP_INC)+FRAME_KICK);
	ResetFrame();
	}
      else if(act->c.control == BashB.Number())  {
	if(!stance)  Stance();
	PlayUp((weapon*WEAP_INC)+FRAME_BASHP);
	PlayUp((weapon*WEAP_INC)+FRAME_BASH);
	ResetFrame();
	}
      else if(act->c.control == ResetB.Number())  {
	xpos=100;
	stance=0;
	ResetFrame();
	}
      else if(act->c.control == RebuildB.Number())  {
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
      else if(act->c.control == SaveB.Number())  {
	int ctr;
	NewResFile crf(ResourceFile);
	CharBag ver(4);
	ver[0] = VER1; ver[1] = VER2; ver[2] = VER3; ver[3] = VER4;
	crf.Add(&ver);
	crf.Add(tc);
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
      else if(act->c.control == QuitB.Number()) {
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
    }
  Exit(0);
  return 0;
  }

void QuickResetFrame()  {
  if(stance) frame=weapon*WEAP_INC + FRAME_STANCE;
  else frame=FRAME_STAND;
  g->UseImage(gg[frame]);
  g->SetPriority(1000);
  g->Move(xpos, 100);
  DrawWeapon();
  if((!stance) && StanceB->State())  StanceB->SetState(0);
  else if(stance && (!StanceB->State()))  StanceB->SetState(1);
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
  if(!StanceB->State()) StanceB->SetState(1);
  }

void UnStance() {
  PlayUp(weapon*WEAP_INC + FRAME_STANCE+1);
  stance=0;
  ResetFrame();
  if(StanceB->State()) StanceB->SetState(0);
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
  if(!FrameB->State()) return;
  printf("Still In (%d)!\n", FrameB->State());
  space->Move(320, 240);
  while(key->IsPressed(KEY_SPACE)) ManipWeapon();
  while(!key->IsPressed(KEY_SPACE)) ManipWeapon();
  space->Erase();
  printf("Done!\n");
  }

void WeaponManipEvent(InputAction *act)  {
//  if(act->g.type == INPUTACTION_KEYDOWN) {
//    if(act->Key() >= KEY_F1 && act->Key() <= KEY_F10)  {
//      int sl = act->Key() - KEY_F1;
//      if(act->ModKeyPressed(0) || act->ModKeyPressed(1))  {
//	mem[sl][0] = (*gv[frame])[0];
//	mem[sl][1] = (*gv[frame])[1];
//	mem[sl][2] = (*gv[frame])[2];
//	mem[sl][3] = (*gv[frame])[3];
//	}
//      else {
//	(*gv[frame])[0] = mem[sl][0];
//	(*gv[frame])[1] = mem[sl][1];
//	(*gv[frame])[2] = mem[sl][2];
//	(*gv[frame])[3] = mem[sl][3];
//	DrawWeapon();
//	}
//      }
//    }
  if(act->g.type != INPUTACTION_CONTROLDOWN) return;
  else if(act->c.control == LeftB->Number())  {
    (*gv[frame])[1]--;  DrawWeapon();
    }
  else if(act->c.control == RightB->Number())  {
    (*gv[frame])[1]++;  DrawWeapon();
    }
  else if(act->c.control == UpB->Number())  {
    (*gv[frame])[2]--;  DrawWeapon();
    }
  else if(act->c.control == DownB->Number())  {
    (*gv[frame])[2]++;  DrawWeapon();
    }
  else if(act->c.control == PrevB->Number())  {
    if((*gv[frame])[0] > -1)  {
      (*gv[frame])[0]--;
      if((*gv[frame])[0] > 10 && (*gv[frame])[0] < 256)  (*gv[frame])[0] = -1;
      DrawWeapon();
      }
    }
  else if(act->c.control == NextB->Number())  {
    if(((*gv[frame])[0] & 255) < 8 || (*gv[frame])[0] < 0)  {
      (*gv[frame])[0]++;  DrawWeapon();
      }
    }
  else if(act->c.control == TopB->Number())  {
    (*gv[frame])[0] ^= 256;  s->Erase(); DrawWeapon();
    }
  else if(act->c.control == ClockB->Number())  {
    (*gv[frame])[3] += 256;  DrawWeapon();
    }
  else if(act->c.control == CClockB->Number())  {
    (*gv[frame])[3] -= 256;  DrawWeapon();
    }
  }

void ManipWeapon()  {
  screen->Refresh();
  InputAction *act;
  do { screen->Refresh(); act = input->NextAction(); } while(act==NULL);
//  while(act->g.type != INPUTACTION_NONE)  {
//    WeaponManipEvent(act);
//    act = input->NextAction();
//    }
  }
