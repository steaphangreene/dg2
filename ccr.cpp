#include <user.h>
#include <user/resfile.h>
#include <unistd.h>

#define ACT_WALKL	0
#define ACT_WALKR	1
#define ACT_RUNL	2
#define ACT_RUNR	3
#define ACT_WORK	4
#define ACT_EXTINGUISH	5
#define ACT_FARM	6
#define ACT_CHOP	7
#define ACT_DIG		8
#define ACT_KICK	9
#define ACT_ATTACK	10
#define ACT_BASH	11
#define ACT_BLOCK	12
#define ACT_MAX		13

const char *fn[ACT_MAX] = { "walkl", "walkr", "runl", "runr", "work", "ext",
		"farm", "chop", "dig", "kick", "attack", "bash", "block" };


Graphic *img[ACT_MAX][16];

int main(int argc, char **argv)  {
  if(argc > 1)  {
    if(!access(argv[1], R_OK|F_OK|D_OK))  {
      char buf[256];
      int ctra, ctrn;
      for(ctra=0; ctra<ACT_MAX; ctra++)  {
	for(ctrn=0; ctrn<16; ctrn++)  {
	  sprintf(buf, "%s/%s%.2d.bmp%c", argv[1], fn[ctra], ctrn, 0);
	  if(access(buf, R_OK|F_OK)) img[ctra][ctrn] = NULL;
	  else  {
	    img[ctra][ctrn] = new Graphic(buf);
	    img[ctra][ctrn]->FindTrueCenter();
	    img[ctra][ctrn]->Trim();
	    }
	  }
	}
      }
//    else if(!access(argv[1], R_OK|F_OK))  {
//      }
    else Exit(1, "File/Directory \"%s\" not found!\n", argv[1]);
    }
  Screen screen;
  User user;
  screen.SetSize(640, 480);
  screen.GetBMPPalette("graphics/creature/guy/walkl00.bmp");
  screen.ShowScreen();
  screen.SetDefaultTransparentColor(33);
  Mouse mouse;
  int quit = 0;
  screen.SetFont("basic10.sgf");
  cval ct = screen.GetPalette().GetClosestColor(50, 0, 0);
  cval c1 = screen.GetPalette().GetClosestColor(196, 196, 196);
  cval c2 = screen.GetPalette().GetClosestColor(128, 128, 128);
  cval c3 = screen.GetPalette().GetClosestColor(64, 64, 64);
  Button qb(64, 16, "Quit", ct, c1, c2, c3);
  qb.Move(0,0);
  Button bb(64, 16, "<<Back", ct, c1, c2, c3);
  bb.Move(64,0);
  Button fb(64, 16, "Forward>>", ct, c1, c2, c3);
  fb.Move(128,0);
  user.MapKeyToButton(SCAN_Q, qb);
  user.MapKeyToButton(SCAN_ESC, qb);
  user.MapKeyToButton(SCAN_RIGHT, fb);
  user.MapKeyToButton(SCAN_LEFT, bb);
  mouse.ShowCursor();
  mouse.SetBehavior(MB_CLICK, MB_CLICK, MB_CLICK);
  InputAction a;
  Sprite s;
//  ResFile crf("dude.crf", argv[0]);

  Graphic nullg = screen.gprintf(0, ct, "(NULL)");
  nullg.Trim();
  nullg.FindTrueCenter();

  int inda=0, indn=0;
  if(img[inda][indn] != NULL) s.SetImage(img[inda][indn]->Scaled(4.0));
  else s.SetImage(nullg.Scaled(2.0));
  s.Move(128+64, 480-(128+64));
  while(!quit)  {
    screen.Refresh();
    a = user.Action();
    switch(a.Type())  {
      case(INPUTACTION_BUTTONRELEASED):  {
	if(a.ButtonPressed() == qb.Number())  {
	  quit = 1;
	  }
	if(a.ButtonPressed() == bb.Number())  {
	  indn--;
	  if(indn<0) indn = 0;
	  if(img[inda][indn] != NULL) s.SetImage(img[inda][indn]->Scaled(4.0));
	  else s.SetImage(nullg.Scaled(2.0));
	  s.Move(128+64, 480-(128+64));
	  }
	if(a.ButtonPressed() == fb.Number())  {
	  indn++;
	  if(indn>15) indn = 15;
	  if(img[inda][indn] != NULL) s.SetImage(img[inda][indn]->Scaled(4.0));
	  else s.SetImage(nullg.Scaled(2.0));
	  s.Move(128+64, 480-(128+64));
	  }
	}break;
      }
    }
  return 1;
  }
