#include <user/screen.h>
#include <user/net.h>
#include <stdlib.h>
#include "player.h"
#include "creature.h"
#include "gmode.h"
#include "map.h"
#include "dg2.h"
#include "game.h"
#include "spell.h"
#include "struct.h"

#define gm gmode[cmode]

extern char spact[64][4];
extern char sprng[64];

extern GMode gmode[10];
extern char cmode;
extern Screen *screen;
extern InputQueue *input;
extern Keyboard *key;
extern Map *curmap;
extern Game *curgame;
extern Network net;
extern Thing things;
extern Mouse *mouse;
extern char multi;
extern Panel mainp;
extern Panel minip;
extern int MainButtonNum;

    Graphic Normalg("graphics/pointers/normal.bmp");

    Graphic Fleeg("graphics/pointers/actions/flee.bmp");
    Graphic Attackg("graphics/pointers/actions/attack.bmp");
    Graphic Buildg("graphics/pointers/actions/build.bmp");
    Graphic Castg("graphics/pointers/actions/cast.bmp");
    Graphic Digg("graphics/pointers/actions/dig.bmp");
    Graphic Followg("graphics/pointers/actions/follow.bmp");
    Graphic Getg("graphics/pointers/actions/get.bmp");
    Graphic Harvestg("graphics/pointers/actions/harvest.bmp");
    Graphic Igniteg("graphics/pointers/actions/ignite.bmp");
    Graphic Joing("graphics/pointers/actions/join.bmp");
    Graphic Killg("graphics/pointers/actions/kill.bmp");
    Graphic Lookg("graphics/pointers/actions/look.bmp");
    Graphic Moveg("graphics/pointers/actions/move.bmp");
    Graphic Patrolg("graphics/pointers/actions/patrol.bmp");
    Graphic Natureg("graphics/pointers/actions/nature.bmp");
    Graphic Quitg("graphics/pointers/actions/quit.bmp");
    Graphic Rung("graphics/pointers/actions/run.bmp");
    Graphic Stopg("graphics/pointers/actions/stop.bmp");
    Graphic Tailg("graphics/pointers/actions/tail.bmp");
    Graphic Workg("graphics/pointers/actions/work.bmp");
    Graphic Extg("graphics/pointers/actions/ext.bmp");

    Graphic ArrowsSg("graphics/pointers/spells/arrows.bmp");
    Graphic BlindSg("graphics/pointers/spells/blind.bmp");
    Graphic ControlSg("graphics/pointers/spells/control.bmp");
    Graphic DisinSg("graphics/pointers/spells/disin.bmp");
    Graphic EraseSg("graphics/pointers/spells/erase.bmp");
    Graphic FireballSg("graphics/pointers/spells/fireball.bmp");
    Graphic GlobeSg("graphics/pointers/spells/globe.bmp");
    Graphic InvisSg("graphics/pointers/spells/invis.bmp");
    Graphic JumpSg("graphics/pointers/spells/jump.bmp");
    Graphic KickbackSg("graphics/pointers/spells/kickback.bmp");
    Graphic MannaSg("graphics/pointers/spells/manna.bmp");
    Graphic NegateSg("graphics/pointers/spells/negate.bmp");
    Graphic OpenSg("graphics/pointers/spells/open.bmp");
    Graphic ProtectSg("graphics/pointers/spells/protect.bmp");
    Graphic QuickSg("graphics/pointers/spells/quick.bmp");
    Graphic ReopenSg("graphics/pointers/spells/reopen.bmp");
    Graphic SinkholeSg("graphics/pointers/spells/sinkhole.bmp");
    Graphic TeleportSg("graphics/pointers/spells/teleport.bmp");
    Graphic VisionSg("graphics/pointers/spells/vision.bmp");
    Graphic WallSg("graphics/pointers/spells/wall.bmp");
    Graphic ExtSg("graphics/pointers/spells/ext.bmp");

    Graphic BlessNg("graphics/pointers/nature/bless.bmp");
    Graphic ControlNg("graphics/pointers/nature/control.bmp");
    Graphic DeathNg("graphics/pointers/nature/death.bmp");
    Graphic EarthNg("graphics/pointers/nature/earth.bmp");
    Graphic FrenzyNg("graphics/pointers/nature/frenzy.bmp");
    Graphic HealNg("graphics/pointers/nature/heal.bmp");
    Graphic IceNg("graphics/pointers/nature/ice.bmp");
    Graphic LightNg("graphics/pointers/nature/light.bmp");
    Graphic MeteorNg("graphics/pointers/nature/meteor.bmp");
    Graphic PlagueNg("graphics/pointers/nature/plague.bmp");
    Graphic RaiseNg("graphics/pointers/nature/raise.bmp");
    Graphic SmiteNg("graphics/pointers/nature/smite.bmp");
    Graphic TsunamiNg("graphics/pointers/nature/tsunami.bmp");
    Graphic UndeathNg("graphics/pointers/nature/undeath.bmp");
    Graphic WhirlNg("graphics/pointers/nature/whirl.bmp");
    Graphic ExcorNg("graphics/pointers/nature/excor.bmp");

    Graphic AquaductBg("graphics/pointers/structs/bridge.bmp");
    Graphic BridgeBg("graphics/pointers/structs/bridge.bmp");
    Graphic CartBg("graphics/pointers/structs/bridge.bmp");
    Graphic DrawbridgeBg("graphics/pointers/structs/bridge.bmp");
    Graphic EnclosureBg("graphics/pointers/structs/bridge.bmp");
    Graphic FarmlandBg("graphics/pointers/structs/bridge.bmp");
    Graphic GateBg("graphics/pointers/structs/bridge.bmp");
    Graphic HullBg("graphics/pointers/structs/bridge.bmp");
    Graphic IrrigationBg("graphics/pointers/structs/bridge.bmp");
    Graphic MachineBg("graphics/pointers/structs/bridge.bmp");
    Graphic PathBg("graphics/pointers/structs/ramp.bmp");
    Graphic RampBg("graphics/pointers/structs/ramp.bmp");
    Graphic SeigeEngineBg("graphics/pointers/structs/bridge.bmp");
    Graphic TowerBg("graphics/pointers/structs/wall.bmp");
    Graphic WallBg("graphics/pointers/structs/wall.bmp");

    Graphic RockMg("graphics/pointers/mat/rock.bmp");
    Graphic WoodMg("graphics/pointers/mat/wood.bmp");

int __com[256];
int __sb[256];
int __nb[256];
int __bb[256];
int __mb[256];

char s2n[256];

Player::Player(int pnum, char ptype, int c1, int c2)  {
  number = pnum;
  shown = 0;
  type = ptype;
  Selectsize = 0;
  Selectlist = NULL;
  selcast = 0;
  selpray = 0;
  tcol1 = c1;
  tcol2 = c2;
  if(type == PLAYER_CONSOLE)  {
    bzero((char *)__com, 1024);
    bzero((char *)__sb, 1024);
    bzero((char *)__nb, 1024);
    bzero((char *)__bb, 1024);
    bzero((char *)__mb, 1024);
    bzero((char *)s2n, 256);

s2n[KEY_A]=0; s2n[KEY_B]=1; s2n[KEY_C]=2; s2n[KEY_D]=3; s2n[KEY_E]=4;
s2n[KEY_F]=5; s2n[KEY_G]=6; s2n[KEY_H]=7; s2n[KEY_I]=8; s2n[KEY_J]=9;
s2n[KEY_K]=10; s2n[KEY_L]=11; s2n[KEY_M]=12; s2n[KEY_N]=13; s2n[KEY_O]=14;
s2n[KEY_P]=15; s2n[KEY_Q]=16; s2n[KEY_R]=17; s2n[KEY_S]=18; s2n[KEY_T]=19;
s2n[KEY_U]=20; s2n[KEY_V]=21; s2n[KEY_W]=22; s2n[KEY_X]=23; s2n[KEY_Z]=24;

    Clickey *tmpb;
    Graphic upbg("graphics/800/buttons/blanks0.bmp");
    Graphic pbg("graphics/800/buttons/blanks1.bmp");
    Graphic pg, upg;
    upbg.FindTrueCenter();	pbg.FindTrueCenter();
    upbg.tcolor = 33;		pbg.tcolor = 33;

    Normalg.Trim();
    Fleeg.Trim();
    Attackg.Trim();
    Buildg.Trim();
    Castg.Trim();
    Followg.Trim();
    Digg.Trim();
    Getg.Trim();
    Harvestg.Trim();
    Igniteg.Trim();
    Joing.Trim();
    Killg.Trim();
    Lookg.Trim();
    Moveg.Trim();
    Natureg.Trim();
    Patrolg.Trim();
    Quitg.Trim();
    Rung.Trim();
    Stopg.Trim();
    Tailg.Trim();
    Workg.Trim();
    Extg.Trim();

    ArrowsSg.Trim();
    BlindSg.Trim();
    ControlSg.Trim();

    DisinSg.Trim();
    EraseSg.Trim();
    FireballSg.Trim();
    GlobeSg.Trim();
    InvisSg.Trim();
    JumpSg.Trim();
    KickbackSg.Trim();
    MannaSg.Trim();
    NegateSg.Trim();
    OpenSg.Trim();
    ProtectSg.Trim();
    QuickSg.Trim();
    ReopenSg.Trim();
    SinkholeSg.Trim();
    TeleportSg.Trim();
    VisionSg.Trim();
    WallSg.Trim();
    ExtSg.Trim();

    BlessNg.Trim();
    ControlNg.Trim();
    DeathNg.Trim();
    EarthNg.Trim();
    FrenzyNg.Trim();
    HealNg.Trim();
    IceNg.Trim();
    LightNg.Trim();
    MeteorNg.Trim();
    PlagueNg.Trim();
    RaiseNg.Trim();
    SmiteNg.Trim();
    TsunamiNg.Trim();
    UndeathNg.Trim();
    WhirlNg.Trim();
    ExcorNg.Trim();

    WallBg.Trim();
    BridgeBg.Trim();
    RampBg.Trim();

    RockMg.Trim();
    WoodMg.Trim();

    Normalg.SetCenter(1,1);
    Fleeg.FindTrueCenter();
    Attackg.FindTrueCenter();
    Buildg.FindTrueCenter();
    Castg.FindTrueCenter();
    Followg.FindTrueCenter();
    Digg.FindTrueCenter();
    Getg.FindTrueCenter();
    Harvestg.FindTrueCenter();
    Igniteg.FindTrueCenter();
    Joing.FindTrueCenter();
    Killg.FindTrueCenter();
    Lookg.FindTrueCenter();
    Moveg.FindTrueCenter();
    Natureg.FindTrueCenter();
    Patrolg.FindTrueCenter();
    Quitg.FindTrueCenter();
    Rung.FindTrueCenter();
    Stopg.FindTrueCenter();
    Tailg.FindTrueCenter();
    Workg.FindTrueCenter();
    Extg.FindTrueCenter();

    ArrowsSg.FindTrueCenter();
    BlindSg.FindTrueCenter();
    ControlSg.FindTrueCenter();

    DisinSg.FindTrueCenter();
    EraseSg.FindTrueCenter();
    FireballSg.FindTrueCenter();
    GlobeSg.FindTrueCenter();
    InvisSg.FindTrueCenter();
    JumpSg.FindTrueCenter();
    KickbackSg.FindTrueCenter();
    MannaSg.FindTrueCenter();
    NegateSg.FindTrueCenter();
    OpenSg.FindTrueCenter();
    ProtectSg.FindTrueCenter();
    QuickSg.FindTrueCenter();
    ReopenSg.FindTrueCenter();
    SinkholeSg.FindTrueCenter();
    TeleportSg.FindTrueCenter();
    VisionSg.FindTrueCenter();
    WallSg.FindTrueCenter();
    ExtSg.FindTrueCenter();

    BlessNg.FindTrueCenter();
    ControlNg.FindTrueCenter();
    DeathNg.FindTrueCenter();
    EarthNg.FindTrueCenter();
    FrenzyNg.FindTrueCenter();
    HealNg.FindTrueCenter();
    IceNg.FindTrueCenter();
    LightNg.FindTrueCenter();
    MeteorNg.FindTrueCenter();
    PlagueNg.FindTrueCenter();
    RaiseNg.FindTrueCenter();
    SmiteNg.FindTrueCenter();
    TsunamiNg.FindTrueCenter();
    UndeathNg.FindTrueCenter();
    WhirlNg.FindTrueCenter();
    ExcorNg.FindTrueCenter();

    WallBg.FindTrueCenter();
    BridgeBg.FindTrueCenter();
    RampBg.FindTrueCenter();

    RockMg.FindTrueCenter();
    WoodMg.FindTrueCenter();

    Normalg.tcolor = 33;
    Fleeg.tcolor = 33;
    Attackg.tcolor = 33;
    Buildg.tcolor = 33;
    Castg.tcolor = 33;
    Followg.tcolor = 33;
    Digg.tcolor = 33;
    Getg.tcolor = 33;
    Harvestg.tcolor = 33;
    Igniteg.tcolor = 33;
    Joing.tcolor = 33;
    Killg.tcolor = 33;
    Lookg.tcolor = 33;
    Moveg.tcolor = 33;
    Natureg.tcolor = 33;
    Patrolg.tcolor = 33;
    Quitg.tcolor = 33;
    Rung.tcolor = 33;
    Stopg.tcolor = 33;
    Tailg.tcolor = 33;
    Workg.tcolor = 33;
    Extg.tcolor = 33;

    ArrowsSg.tcolor = 33;
    BlindSg.tcolor = 33;
    ControlSg.tcolor = 33;

    DisinSg.tcolor = 33;
    EraseSg.tcolor = 33;
    FireballSg.tcolor = 33;
    GlobeSg.tcolor = 33;
    InvisSg.tcolor = 33;
    JumpSg.tcolor = 33;
    KickbackSg.tcolor = 33;
    MannaSg.tcolor = 33;
    NegateSg.tcolor = 33;
    OpenSg.tcolor = 33;
    ProtectSg.tcolor = 33;
    QuickSg.tcolor = 33;
    ReopenSg.tcolor = 33;
    SinkholeSg.tcolor = 33;
    TeleportSg.tcolor = 33;
    VisionSg.tcolor = 33;
    WallSg.tcolor = 33;
    ExtSg.tcolor = 33;

    BlessNg.tcolor = 33;
    ControlNg.tcolor = 33;
    DeathNg.tcolor = 33;
    EarthNg.tcolor = 33;
    FrenzyNg.tcolor = 33;
    HealNg.tcolor = 33;
    IceNg.tcolor = 33;
    LightNg.tcolor = 33;
    MeteorNg.tcolor = 33;
    PlagueNg.tcolor = 33;
    RaiseNg.tcolor = 33;
    SmiteNg.tcolor = 33;
    TsunamiNg.tcolor = 33;
    UndeathNg.tcolor = 33;
    WhirlNg.tcolor = 33;
    ExcorNg.tcolor = 33;

    WallBg.tcolor = 33;
    BridgeBg.tcolor = 33;
    RampBg.tcolor = 33;

    RockMg.tcolor = 33;
    WoodMg.tcolor = 33;

    pbg.SetCenter(pbg.xcenter+2, pbg.ycenter+2);

    upg = upbg+Fleeg;		pg = pbg+Fleeg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 193);		__com[KEY_BQUOTE] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Moveg;		pg = pbg+Moveg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 223);		__com[KEY_M] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Lookg;		pg = pbg+Lookg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 283);	__com[KEY_L] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Patrolg;		pg = pbg+Patrolg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 223);	__com[KEY_P] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Rung;		pg = pbg+Rung;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 193);	__com[KEY_R] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();


    upg = upbg+Buildg;		pg = pbg+Buildg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 253);		__com[KEY_B] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Castg;		pg = pbg+Castg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 313);		__com[KEY_C] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Digg;		pg = pbg+Digg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 253);	__com[KEY_D] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Natureg;		pg = pbg+Natureg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 313);	__com[KEY_N] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Getg;		pg = pbg+Getg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 283);		__com[KEY_G] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Followg;		pg = pbg+Followg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 223);	__com[KEY_F] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Tailg;		pg = pbg+Tailg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 223);	__com[KEY_T] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();


    upg = upbg+Harvestg;	pg = pbg+Harvestg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 253);	__com[KEY_H] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Workg;		pg = pbg+Workg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 253);	__com[KEY_W] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Extg;		pg = pbg+Extg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 253);	__com[KEY_X] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Quitg;		pg = pbg+Quitg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 313);	__com[KEY_Q] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();


    upg = upbg+Attackg;		pg = pbg+Attackg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 193);	__com[KEY_A] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Killg;		pg = pbg+Killg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 193);	__com[KEY_K] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Igniteg;		pg = pbg+Igniteg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 193);	__com[KEY_I] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Joing;		pg = pbg+Joing;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 223);	__com[KEY_J] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+Stopg;		pg = pbg+Stopg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 313);	__com[KEY_S] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();


    upg = upbg+ArrowsSg;	pg = pbg+ArrowsSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 193);		__sb[KEY_A] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+BlindSg;		pg = pbg+BlindSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 193);	__sb[KEY_B] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+ControlSg;	pg = pbg+ControlSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 193);	__sb[KEY_C] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+DisinSg;		pg = pbg+DisinSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 193);	__sb[KEY_D] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+EraseSg;		pg = pbg+EraseSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 193);	__sb[KEY_E] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+FireballSg;	pg = pbg+FireballSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 223);		__sb[KEY_F] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+GlobeSg;		pg = pbg+GlobeSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 223);	__sb[KEY_G] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+InvisSg;		pg = pbg+InvisSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 223);	__sb[KEY_I] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+JumpSg;		pg = pbg+JumpSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 223);	__sb[KEY_J] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+KickbackSg;	pg = pbg+KickbackSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 253);		__sb[KEY_K] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+MannaSg;		pg = pbg+MannaSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 253);	__sb[KEY_M] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+NegateSg;	pg = pbg+NegateSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 253);	__sb[KEY_N] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+OpenSg;		pg = pbg+OpenSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 253);	__sb[KEY_O] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+ProtectSg;	pg = pbg+ProtectSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 283);		__sb[KEY_P] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+QuickSg;		pg = pbg+QuickSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 283);	__sb[KEY_Q] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+ReopenSg;	pg = pbg+ReopenSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 283);	__sb[KEY_R] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+SinkholeSg;	pg = pbg+SinkholeSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 283);	__sb[KEY_S] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+TeleportSg;	pg = pbg+TeleportSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 283);	__sb[KEY_T] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+VisionSg;	pg = pbg+VisionSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 313);	__sb[KEY_V] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+WallSg;		pg = pbg+WallSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 313);	__sb[KEY_W] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+ExtSg;		pg = pbg+ExtSg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 313);	__sb[KEY_X] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();


    upg = upbg+BlessNg;		pg = pbg+BlessNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 193);	__nb[KEY_B] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+ControlNg;	pg = pbg+ControlNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 193);	__nb[KEY_C] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+DeathNg;		pg = pbg+DeathNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 193);	__nb[KEY_D] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+EarthNg;		pg = pbg+EarthNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 193);	__nb[KEY_E] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+FrenzyNg;	pg = pbg+FrenzyNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 223);		__nb[KEY_F] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+HealNg;		pg = pbg+HealNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 223);	__nb[KEY_H] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+IceNg;		pg = pbg+IceNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 223);	__nb[KEY_I] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+LightNg;		pg = pbg+LightNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 253);	__nb[KEY_L] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+MeteorNg;	pg = pbg+MeteorNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 253);	__nb[KEY_M] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+PlagueNg;	pg = pbg+PlagueNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 283);		__nb[KEY_P] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+RaiseNg;		pg = pbg+RaiseNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 283);	__nb[KEY_R] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+SmiteNg;		pg = pbg+SmiteNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 283);	__nb[KEY_S] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+TsunamiNg;	pg = pbg+TsunamiNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(125, 283);	__nb[KEY_T] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+UndeathNg;	pg = pbg+UndeathNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 313);		__nb[KEY_U] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+WhirlNg;		pg = pbg+WhirlNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 313);	__nb[KEY_W] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+ExcorNg;		pg = pbg+ExcorNg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(95, 313);	__nb[KEY_X] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+WallBg;		pg = pbg+WallBg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 193);		__bb[KEY_W] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+RampBg;		pg = pbg+RampBg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 193);	__bb[KEY_R] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+BridgeBg;	pg = pbg+BridgeBg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(65, 193);	__bb[KEY_B] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+RockMg;		pg = pbg+RockMg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(5, 193);		__mb[KEY_R] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

    upg = upbg+WoodMg;		pg = pbg+WoodMg;
    upg.SetCenter(0, 0);	pg.SetCenter(0, 0);
    tmpb = new Clickey();	tmpb->SetImage(upg, pg);
    tmpb->Move(35, 193);	__mb[KEY_W] = tmpb->Number();
    tmpb->Erase();		tmpb->Disable();

#define Trim SetCenter
    Normalg.Trim(0, 0);	   Fleeg.Trim(0, 0);	  Attackg.Trim(0, 0);
    Buildg.Trim(0,0);  Castg.Trim(0,0);  Followg.Trim(0,0);  Digg.Trim(0,0);
    Getg.Trim(0, 0);	   Harvestg.Trim(0, 0);	  Igniteg.Trim(0, 0);
    Joing.Trim(0, 0);	   Killg.Trim(0, 0);	  Lookg.Trim(0, 0);
    Moveg.Trim(0, 0);	   Natureg.Trim(0, 0);	  Patrolg.Trim(0, 0);
    Quitg.Trim(0, 0);	   Rung.Trim(0, 0);	  Stopg.Trim(0, 0);
    Tailg.Trim(0, 0);	   Workg.Trim(0, 0);	  Extg.Trim(0, 0);

    ArrowsSg.Trim(0, 0);   BlindSg.Trim(0, 0);	  ControlSg.Trim(0, 0);
    DisinSg.Trim(0, 0);	   EraseSg.Trim(0, 0);	  FireballSg.Trim(0, 0);
    GlobeSg.Trim(0, 0);	   InvisSg.Trim(0, 0);	  JumpSg.Trim(0, 0);
    KickbackSg.Trim(0, 0); MannaSg.Trim(0, 0);	  NegateSg.Trim(0, 0);
    OpenSg.Trim(0, 0);	   ProtectSg.Trim(0, 0);  QuickSg.Trim(0, 0);
    ReopenSg.Trim(0, 0);   SinkholeSg.Trim(0, 0); TeleportSg.Trim(0, 0);
    VisionSg.Trim(0, 0);   WallSg.Trim(0, 0);	  ExtSg.Trim(0, 0);

    BlessNg.Trim(0, 0);	   ControlNg.Trim(0, 0);  DeathNg.Trim(0, 0);
    EarthNg.Trim(0, 0);	   FrenzyNg.Trim(0, 0);	  HealNg.Trim(0, 0);
    IceNg.Trim(0, 0);	   LightNg.Trim(0, 0);	  MeteorNg.Trim(0, 0);
    PlagueNg.Trim(0, 0);   RaiseNg.Trim(0, 0);	  SmiteNg.Trim(0, 0);
    TsunamiNg.Trim(0, 0);  UndeathNg.Trim(0, 0);  WhirlNg.Trim(0, 0);
    ExcorNg.Trim(0, 0);

    WallBg.Trim(0, 0);    BridgeBg.Trim(0, 0);	RampBg.Trim(0, 0);

    RockMg.Trim(0, 0);	WoodMg.Trim(0, 0);
#undef Trim

    Lookg.SetCenter(5, 5);
    Natureg.SetCenter(5, 5);
    Castg.SetCenter(3, 3);

    ResetState();
    }
  }

Player::~Player()  {
  if(Selectlist != NULL)  {
    ClearSelectList();
    delete Selectlist;
    }
  int ctr;
  Clickey *cb;
  for(ctr=0; ctr<256; ctr++)  {
    if(__com[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__com[ctr]);
      delete cb;
      }
    if(__sb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__sb[ctr]);
      delete cb;
      }
    if(__nb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__nb[ctr]);
      delete cb;
      }
    if(__bb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__bb[ctr]);
      delete cb;
      }
    }
  }

void Player::TakeTurn(int pkg)  {
  Debug("Player::TakeTurn(): Begin");
  int SIGNAL_MENU[2];
  SIGNAL_MENU[0] = -1;
  SIGNAL_MENU[1] = 1;
  int SIGNAL_SIZE = 8;

  int syscom;
  switch(type)  {
    case(PLAYER_NETWORK):  if(!pkg) {
      syscom = things.Listen();
      while((syscom != 0) && (!curgame->quit))  { 
	curgame->Menus();
        if(!curgame->quit)syscom = things.Listen();
	}
      } break;
    case(PLAYER_CONSOLE):  {
      Debug("Player::TakeTurn(): PLAYER_CONSOLE: Start");
      int rescale = 0;
      int realignx = 0;
      int realigny = 0;
      InputAction *curact;
      Debug("Player::TakeTurn(): Talk()");
      if(multi)  things.StartTalk();
      else  things.ClearTalk();
      Debug("Player::TakeTurn(): doing...");
      if(doing == DOING_NORMAL && cur_comm == COMMAND_EXTINGUISH)  {
	Action *tmpa = new Action(ACTION_EXTINGUISH, intparam[0]);
	CommandSelected(tmpa);
	ResetState();
	}
      else if(doing == DOING_NORMAL && cur_comm == COMMAND_IGNITE)  {
	Action *tmpa = new Action(ACTION_IGNITE, intparam[0]);
	CommandSelected(tmpa);
	ResetState();
	}
      else if(doing == DOING_NORMAL && cur_comm == COMMAND_LOOK)  {
	Action *tmpa = new Action(ACTION_LOOK, intparam[0]);
	CommandSelected(tmpa);
	ResetState();
	}
      else if(doing == DOING_NORMAL && cur_comm == COMMAND_DIG)  {
	Action *tmpa = new Action(ACTION_DIG, listparam[0]);
	CommandSelected(tmpa);
	ResetState();
	}
      else if(doing == DOING_NORMAL && cur_comm == COMMAND_BUILD)  {
	if(argstate==1)  {
	  intparam[1] = -1;
	  PickMaterial(&intparam[1]);
	  }
	else if(argstate==2)  {
	  SelectLCells(&listparam[0]);
	  }
	else {
	  Action *tmpa = new Action(ACTION_BUILD+(intparam[0]*MATERIAL_MAXBUILD)
		+intparam[1], listparam[0]);
	  CommandSelected(tmpa);
	  ResetState();
	  }
	argstate++;
	}
      else if(doing == DOING_NORMAL && cur_comm == COMMAND_HARVEST)  {
	if(argstate==1)  {
	  Action *tmpa = new Action(ACTION_HARVEST, intparam[0]);
	  CommandSelected(tmpa);
	  ResetState();
	  }
	}
      else if(doing == DOING_NORMAL && (cur_comm == COMMAND_CAST
		|| cur_comm == COMMAND_PRAY))  {
//	printf("Spell #%d initialized\r\n", intparam[0]);
	if(spact[intparam[0]][argstate-1] != -1 && argstate < 5)  {
	  if(intparam[argstate-1] != -1)  {
	    switch(spact[intparam[0]][argstate-1])  {
	      case(DOING_SELECTTHING):
		SelectThing(&intparam[argstate]); break;
	      case(DOING_SELECTCELL):
		SelectCell(&intparam[argstate]); break;
	      case(DOING_SELECTCELLS):
		SelectCells(&listparam[argstate]); break;
	      case(DOING_SELECTLCELLS):
		SelectLCells(&listparam[argstate]); break;
	      case(DOING_SELECTCREATURES):
		SelectCreatures(&listparam[argstate]); break;
	      case(DOING_SELECTFCREATURES):
		SelectFriendlyCreatures(&listparam[argstate]); break;
	      case(DOING_SELECTECREATURES):
		SelectEnemyCreatures(&listparam[argstate]); break;
	      case(DOING_SELECTOBJ):
		SelectObject(&intparam[argstate]); break;
	      case(DOING_SELECTOBJS):
		SelectObjects(&listparam[argstate]); break;
	      case(DOING_SELECTFOBJS):
		SelectFriendlyObjects(&listparam[argstate]); break;
	      case(DOING_SELECTEOBJS):
		SelectEnemyObjects(&listparam[argstate]); break;
	      }
	    argstate++;
	    }
	  }
	else {
	  int ctr;
	  IntList tmpl;
	  if(intparam[1] != -1) tmpl += intparam[1];
	  if(intparam[2] != -1) tmpl += intparam[2];
	  if(intparam[3] != -1) tmpl += intparam[3];
	  if(listparam[0].Size() > 0) tmpl += listparam[0];
	  if(listparam[1].Size() > 0) tmpl += listparam[1];
	  if(listparam[2].Size() > 0) tmpl += listparam[2];
	  if(listparam[3].Size() > 0) tmpl += listparam[3];
	  Action *tmpa = new Action(ACTION_CAST+intparam[0], tmpl);
	  for(ctr=0; ctr<Selectsize; ctr++)  {
	    if(((Creature *)Selectlist[ctr])->CanCast(intparam[0])
		&& (!((Creature *)Selectlist[ctr])->IsSustaining()))  {
	      ((Creature *)Selectlist[ctr])->Do(tmpa);
//	      printf("Told Creature to cast!\r\n");
	      break;
	      }
	    }
//	  printf("finished cast order!\r\n");
	  ResetState();
	  }
	}
      Debug("Player::TakeTurn(): NextAction()");
      curact = input->NextAction();
      if(curact == NULL)
	{ curact = new InputAction; curact->g.type = INPUTACTION_NONE; }
      Debug("Player::TakeTurn(): curact->g.type");
      if(curact->g.type == INPUTACTION_NONE);
      else if((curact->g.type == INPUTACTION_KEYDOWN && curact->k.key == KEY_ESC)
	    || (curact->g.type == INPUTACTION_MOUSEDOWN
	    && curact->m.button == 3
	    && cur_comm != COMMAND_DEFAULT))  {
	ResetState();
	ResetInput();
	curact->g.type = INPUTACTION_NONE;
	}
      else if(doing != DOING_NORMAL)  DoSpecial(curact);
      else if(curact->g.type == INPUTACTION_KEYDOWN && curact->k.key == KEY_Z
	    && curact->g.modkeys) {
        if(((gm.xstep == 32) && (cmode != 0)) || (gm.xstep == 16))  {
	  gm.xstep *= 2;		gm.ystep *= 2;
	  gm.xstart += ((gm.xedge-gm.xorig)/gm.xstep) & 252;
	  gm.ystart += ((gm.yedge-gm.yorig)/(gm.ystep*2)) & 254;
	  rescale = 1;
	  }
        }
      else if(curact->g.type == INPUTACTION_KEYDOWN && curact->k.key==KEY_Z){
        if((gm.xstep == 32) || (gm.xstep == 64))  {
	  gm.xstep /= 2;		gm.ystep /= 2;
	  gm.xstart -= ((gm.xedge-gm.xorig)/(gm.xstep*2)) & 252;
	  gm.ystart -= ((gm.yedge-gm.yorig)/(gm.ystep*4)) & 254;
	  if(gm.xstart < 0)  gm.xstart = 0;
          else  if((gm.xstart/2) >= ((curmap->XSize()-1) - ((gm.xedge - gm.xorig)/gm.xstep)))
            gm.xstart = 2 *((curmap->XSize()-1) - ((gm.xedge - gm.xorig)/gm.xstep));
	  if(gm.ystart < 0)  gm.ystart = 0;
          else if(gm.ystart >= ((curmap->YSize()-1) - ((gm.yedge - gm.yorig)/gm.ystep)))
            gm.ystart = (curmap->YSize()-1) - ((gm.yedge - gm.yorig)/gm.ystep);
	  rescale = 1;
	  }
        }

      Debug("Player::TakeTurn: CheckScroll");
      if((!mouse->DrawingSelector()) && (key->IsPressed(KEY_DOWN)
	  || mouse->YPos() >= ((long)screen->YSize()-3))
          && gm.ystart < ((curmap->YSize()-1) -
		((gm.yedge - gm.yorig)/gm.ystep)))  {
        realigny = 1;
        }
      else if((!mouse->DrawingSelector()) && (key->IsPressed(KEY_UP) || 
	  mouse->YPos() <= 2) && gm.ystart > 0)  {
        realigny = -1;
        }
      if((!mouse->DrawingSelector()) && (key->IsPressed(KEY_LEFT) ||
	  mouse->XPos() <= 2) && gm.xstart > 0)  {
        realignx = -1;
        }
      else if((!mouse->DrawingSelector()) && (key->IsPressed(KEY_RIGHT)
          || mouse->XPos() >= ((long)screen->XSize()-3))
          && (gm.xstart/2) < ((curmap->XSize()-1) -
		((gm.xedge-gm.xorig)/gm.xstep)))  {
        realignx = 1;
        }

      Debug("Player::TakeTurn(): Check Zoom");
      if(curact->g.type == INPUTACTION_KEYDOWN && 
	  (curact->k.key == KEY_EQUALS || curact->k.key == KEY_GREY_PLUS)) {
	curgame->SpeedUp();
        }
      else if(curact->g.type == INPUTACTION_KEYDOWN &&
	  (curact->k.key == KEY_MINUS || curact->k.key == KEY_GREY_MINUS)) {
	curgame->SlowDown();
        }

      Debug("Player::TakeTurn(): Check Select/Command");
      if(curact->g.type == INPUTACTION_MOUSEDOWN)  {
	IntList selected = screen->CollideRectangle(
	    curact->m.x, curact->m.y, curact->m.xs, curact->m.ys);
        if(curact->m.panel == mainp)  {
	  if(curact->m.button == 2)  {
	    }
	  else  if(cur_comm == COMMAND_DEFAULT && curact->m.button == 3)  {
	    OrderSelected(cur_comm, curact);
            }
	  else  if(cur_comm == COMMAND_FOLLOW && curact->m.button == 1)  {
	    OrderSelected(cur_comm, curact);
            }
	  else  if(selected.Size() < 1)  {
	    }
	  else  if(cur_comm == COMMAND_DEFAULT && curact->m.button == 1)  {
	    Select(curact, selected);
            }
	  else  if(curact->m.button == 3)  {
            }
	  else  {
	    OrderSelected(cur_comm, curact);
	    }
	  cur_comm = COMMAND_DEFAULT;
	  }
        else  if(curact->m.panel == minip)  {
	  if(curact->m.button == 3)  {
	    OrderSelected(COMMAND_MINIDEFAULT, curact);
	    }
	  else if(curact->m.button == 1)  {
	    while((input->PeekNextAction() != NULL)
		&& (input->PeekNextAction()->g.type == INPUTACTION_MOUSEDOWN) 
		&& (input->PeekNextAction()->m.panel == minip) 
		&& (input->PeekNextAction()->m.button == 1))  {
	      curact=input->NextAction();
	      }
	    int x=curact->m.x;
	    int y=curact->m.y;
	    curmap->Mini2Cell(x, y);
	    x-=((gm.xsize/gm.xstep)/2);
	    y-=((gm.ysize/gm.ystep)/2);
	    if(x<0) x=0;
	    if(y<0) y=0;
	    realignx = ((x*2)-gm.xstart)/4;
	    realigny = (y-gm.ystart)/2;
	    if((gm.xstart/2+realignx*2)
		> ((curmap->XSize()-1) - ((gm.xedge-gm.xorig)/gm.xstep)))  {
	      realignx = (((curmap->XSize()-1) - ((gm.xedge-gm.xorig)/gm.xstep))
		-(gm.xstart/2))/2;
	      }
	    if((gm.ystart+realigny*2)
		> ((curmap->YSize()-1) - ((gm.yedge-gm.yorig)/gm.ystep)))  {
	      realigny = (((curmap->YSize()-1) - ((gm.yedge-gm.yorig)/gm.ystep))
		-(gm.ystart))/2;
	      }
	    }
	  }
        }

      Debug("Player::TakeTurn(): realignx || realigny");
      if(realignx || realigny)  {
        if((realignx*2) < (gm.xsize)/gm.xstep && 
		(realigny*2) < (gm.ysize/2)/gm.ystep && 
		(-realignx*2) < (gm.xsize)/gm.xstep && 
		(-realigny*2) < (gm.ysize/2)/gm.ystep)  {
	  screen->ScrollPanel(mainp, realignx*(2*gm.xstep),
		realigny*(2*gm.ystep));
	  }
//        else  {
//	  screen->ErasePanelSprites(mainp);
//	  }
        gm.xstart+=4*realignx;
        gm.ystart+=2*realigny;
        curmap->ReAlign(realignx, realigny);
        }
      if(rescale)  {
        screen->ErasePanelSprites(mainp);
        curmap->ReScale();
        }

      Debug("Player::TakeTurn(): Command Buttons");
      if(curact->g.type == INPUTACTION_CONTROLDOWN
		&& curact->c.control == MainButtonNum) {
        cur_comm = COMMAND_DEFAULT;
        }
      else  if(curact->g.type == INPUTACTION_SYSTEM_QUIT
		|| (curact->g.type == INPUTACTION_CONTROLUP
		&& curact->c.control == MainButtonNum)) {
        if(multi)  net.Send(SIGNAL_MENU, SIGNAL_SIZE);
        curgame->Menus();
        }
      else if(curact->g.type == INPUTACTION_CONTROLDOWN && NumSelected() > 0) {
	if(curact->c.control == __com[KEY_A])  cur_comm = COMMAND_ATTACK;
	else if(curact->c.control==__com[KEY_K])
	  cur_comm = COMMAND_KILL;
	else if(curact->c.control==__com[KEY_F])
	  cur_comm = COMMAND_FOLLOW;
	else if(curact->c.control==__com[KEY_S])  {
	  int ctr;
	  for(ctr = 0; ctr < Selectsize; ctr++)  {
	    Action *tmpa = new Action(ACTION_STOP);
	    ((Creature *)Selectlist[ctr])->Do(tmpa);
	    }
	  cur_comm = COMMAND_DEFAULT;
	  }
	else if(curact->c.control==__com[KEY_L])  {
	  cur_comm = COMMAND_LOOK;
	  SelectThing(&intparam[0]);
	  }
	else if(curact->c.control==__com[KEY_I])  {
	  cur_comm = COMMAND_IGNITE;
	  SelectCell(&intparam[0]);
	  }
	else if(curact->c.control==__com[KEY_X])  {
	  cur_comm = COMMAND_EXTINGUISH;
	  SelectCell(&intparam[0]);
	  }
	else if(curact->c.control==__com[KEY_D])  {
	  cur_comm = COMMAND_DIG;
	  SelectLCells(&listparam[0]);
	  }
	else if(curact->c.control==__com[KEY_Q])  {
	  int ctr;
	  for(ctr = 0; ctr < Selectsize; ctr++)  {
	    Action *tmpa = new Action(ACTION_STOP_SPELLS);
	    ((Creature *)Selectlist[ctr])->Do(tmpa);
	    }
	  cur_comm = COMMAND_DEFAULT;
	  }
	else if(curact->c.control==__com[KEY_H])  {
	  cur_comm = COMMAND_HARVEST;
	  intparam[0] = -1;
	  PickMaterial(&intparam[0]);
	  argstate = 1;	  }
	else if(curact->c.control==__com[KEY_B])  {
	  cur_comm = COMMAND_BUILD;
	  PickStructure(&intparam[0]);
	  argstate = 1;	  }
	else if(curact->c.control==__com[KEY_N])  {
	  cur_comm = COMMAND_PRAY;
	  PickPrayer(&intparam[0]);
	  argstate = 1;	  }
	else if(curact->c.control==__com[KEY_C])  {
	  cur_comm = COMMAND_CAST;
	  PickSpell(&intparam[0]);
	  argstate = 1;	  }
	else if(curact->c.control==__com[KEY_BQUOTE]){
	  int ctr;
	  for(ctr = 0; ctr < Selectsize; ctr++)  {
	    Action *tmpa = new Action(ACTION_FLEE,
		((Creature *)Selectlist[ctr])->SafestDirection());
	    ((Creature *)Selectlist[ctr])->Do(tmpa);
	    }
	  cur_comm = COMMAND_DEFAULT;
	  }
	}

      Debug("Player::TakeTurn(): Multi ");
      if(multi)  {
	if(!pkg) things.Talk();
	else  things.EndTalk();
	}
      Debug("Player::TakeTurn(): Pick Correct Cursor ");
      switch(cur_comm)  {
	case COMMAND_DEFAULT:	mouse->SetCursor(&Normalg);	break;
	case COMMAND_ATTACK:	mouse->SetCursor(&Attackg);	break;
	case COMMAND_BUILD:	mouse->SetCursor(&Buildg);	break;
	case COMMAND_HARVEST:	mouse->SetCursor(&Harvestg);	break;
	case COMMAND_LOOK:	mouse->SetCursor(&Lookg);	break;
	case COMMAND_FOLLOW:	mouse->SetCursor(&Followg);	break;
	case COMMAND_KILL:	mouse->SetCursor(&Killg);	break;
	case COMMAND_CAST:	mouse->SetCursor(&Castg);	break;
	case COMMAND_PRAY:	mouse->SetCursor(&Natureg);	break;
	case COMMAND_EXTINGUISH:mouse->SetCursor(&Extg);	break;
	case COMMAND_IGNITE:	mouse->SetCursor(&Igniteg);	break;
	case COMMAND_DIG:	mouse->SetCursor(&Digg);	break;
	}
      Debug("Player::TakeTurn(): Print Stats");
      if(Selectsize == 1)  {
	static Graphic status_graphic;
	static color white = screen->GetPalette().GetClosestColor(255,255,255);
	Creature *ind = (Creature*)Selectlist[0];
	int cgl = 0;
	if(ind->goal[0] != NULL)  cgl = ind->goal[0]->Goal();
	if(!shown)  {
	  screen->CGPrint(&status_graphic, 0, 0, 0, white,
		"Altitude:\nHeight:\nHit:\nStamina:\nGoal:\nMaterial:\n Ammt:");
	  output.SetImage(status_graphic);
	  output.Move(7, 350);
	  oldval[0] = ind->altitude;
	  oldval[1] = ind->height;
	  oldval[2] = ind->hit;
	  oldval[3] = ind->fatigue;
	  oldval[4] = cgl;
	  oldval[5] = ind->mat_type;
	  oldval[6] = ind->mat_ammt;
	  screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[0]);
	  outval[0].SetImage(status_graphic);
	  outval[0].Move(64, 350);
	  screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[1]);
	  outval[1].SetImage(status_graphic);
	  outval[1].Move(64, 362);
	  screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[2]);
	  outval[2].SetImage(status_graphic);
	  outval[2].Move(64, 374);
	  screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[3]);
	  outval[3].SetImage(status_graphic);
	  outval[3].Move(64, 386);
	  screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[4]);
	  outval[4].SetImage(status_graphic);
	  outval[4].Move(64, 398);
	  screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[5]);
	  outval[5].SetImage(status_graphic);
	  outval[5].Move(64, 410);
	  screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[6]);
	  outval[6].SetImage(status_graphic);
	  outval[6].Move(64, 422);
	  shown = 1;
	  }
	else  {
	  if(oldval[0] != ind->altitude)  {
	    oldval[0] = ind->altitude;
	    screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[0]);
	    outval[0].SetImage(status_graphic);
	    outval[0].Move(64, 350);
	    }
	  if(oldval[1] != ind->height)  {
	    oldval[1] = ind->height;
	    screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[1]);
	    outval[1].SetImage(status_graphic);
	    outval[1].Move(64, 362);
	    }
	  if(oldval[2] != ind->hit)  {
	    oldval[2] = ind->hit;
	    screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[2]);
	    outval[2].SetImage(status_graphic);
	    outval[2].Move(64, 374);
	    }
	  if(oldval[3] != ind->fatigue)  {
	    oldval[3] = ind->fatigue;
	    screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[3]);
	    outval[3].SetImage(status_graphic);
	    outval[3].Move(64, 386);
	    }
	  if(oldval[4] != cgl)  {
	    oldval[4] = cgl;
	    screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[4]);
	    outval[4].SetImage(status_graphic);
	    outval[4].Move(64, 398);
	    }
	  if(oldval[5] != ind->mat_type)  {
	    oldval[5] = ind->mat_type;
	    screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[5]);
	    outval[5].SetImage(status_graphic);
	    outval[5].Move(64, 410);
	    }
	  if(oldval[6] != ind->mat_ammt)  {
	    oldval[6] = ind->mat_ammt;
	    screen->CGPrintf(&status_graphic, 0, 0, 0, white, "%d", oldval[6]);
	    outval[6].SetImage(status_graphic);
	    outval[6].Move(64, 422);
	    }
	  }
	}
      else  if(shown) {
	output.Erase();
	outval[0].Erase();
	outval[1].Erase();
	outval[2].Erase();
	outval[3].Erase();
	outval[4].Erase();
	outval[5].Erase();
	outval[6].Erase();
	shown = 0;
	}
//      Debug("Player::TakeTurn(): Refresh()");
//      if(rescale || realignx || realigny)  screen->RefreshFull();
//      else  screen->Refresh();
      Debug("Player::TakeTurn(): PLAYER_CONSOLE: End");
      } break;
    case(PLAYER_COMPUTER):  {
      } break;
    }
  Debug("Player::TakeTurn(): End");
  }

void Player::OrderSelected(Command com, InputAction *ma)  {
  if(Selectsize < 1)  return;
  IntList mas = screen->CollideRectangle(ma->m.x, ma->m.y, ma->m.xs, ma->m.ys);
  if(com == COMMAND_MINIDEFAULT)  mas.Clear();
  int ctr;

  int tmpc;
  int x1=ma->m.x, y1=ma->m.y;
  int x2=x1+ma->m.xs, y2=y1+ma->m.ys;
  if(com == COMMAND_MINIDEFAULT)  {
    curmap->Mini2Cell(x1, y1);
    curmap->Mini2Cell(x2, y2);
    }
  else  {
    curmap->Screen2Cell(x1, y1);
    curmap->Screen2Cell(x2, y2);
    }
  if(x1>x2)  {
    tmpc = x1;
    x1=x2;
    x2=tmpc;
    }
  if(y1>y2)  {
    tmpc = y1;
    y1=y2;
    y2=tmpc;
    }
  if((x1 < 0) || (x2 < 0)) return;

  if(ma->m.button == 3)  {
    for(ctr=0; ctr<mas.Size(); ctr++)  {
      Thing *tmpt;
      tmpt = things.FindBySpriteNumber(mas[ctr]);
      if(tmpt == NULL || tmpt->Type() != THING_CREATURE)  {
	mas -= mas[ctr];
	ctr--;
	}
      }
    }

  if((ma->m.button == 3) || (com != COMMAND_DEFAULT))  {
    if(mas.Size() > 0 && com != COMMAND_FOLLOW)  {
      int whoop = 0;
      IntList whooped, hit;
      Creature *tst;
      for(ctr = 0; ctr < mas.Size(); ctr++)  {
	tst = (Creature *)(things.FindBySpriteNumber(mas[ctr]));
	hit += tst->Number();
	if(tst != NULL && IsEnemy(tst))  {
	  whoop++;
	  whooped += tst->Number();
	  }
	}
      if(whoop > 0)  {
	if(((Creature *)Selectlist[0])->ToldYou != NULL)
		((Creature *)Selectlist[0])->ToldYou->Play();
	Action *tmpa;
	if(com == COMMAND_KILL)  tmpa = new Action(ACTION_KILL, whooped);
	else  tmpa = new Action(ACTION_ATTACK, whooped);
	CommandSelected(tmpa);
	return;
	}
      else  if(com == COMMAND_ATTACK)  {
	if(((Creature *)Selectlist[0])->ToldYou != NULL)
		((Creature *)Selectlist[0])->ToldYou->Play();
	Action *tmpa = new Action(ACTION_ATTACK, hit);
	CommandSelected(tmpa);
	return;
	}
      else  if(com == COMMAND_KILL)  {
	if(((Creature *)Selectlist[0])->ToldYou != NULL)
		((Creature *)Selectlist[0])->ToldYou->Play();
	Action *tmpa = new Action(ACTION_KILL, hit);
	CommandSelected(tmpa);
	return;
	}
      else  com = COMMAND_FOLLOW;
      }
    if(com == COMMAND_FOLLOW)  {
      int handled = 0;
      if(mas.Size() > 0)  {
        IntList who;
	for(ctr = 0; ctr < mas.Size(); ctr++)  {
	  int per = Selectlist[0]->FindBySpriteNumber(mas[ctr])->Number();
	  if(things.GetThingByNumber(per)->Type() == THING_CREATURE)
		who += per;
	  }
	if(who.Size() > 0)  {
//	  if(((Creature *)Selectlist[0])->ToldYou != NULL)
//		((Creature *)Selectlist[0])->ToldYou->Play();
	  for(ctr = 0; ctr < Selectsize; ctr++)  {
	    Action *tmpa = new Action(ACTION_FOLLOW, who);
	    ((Creature *)Selectlist[ctr])->Do(tmpa);
	    handled = 1;
	    }
	  }
        }
      if(!handled)  {
	if(curmap->CellAt(x1, y1)->IsPath())  {
	  if(((Creature *)Selectlist[0])->ToldYou != NULL)
		((Creature *)Selectlist[0])->ToldYou->Play();
	  for(ctr = 0; ctr < Selectsize; ctr++)  {
	    Action *tmpa = new Action(ACTION_FOLLOW_PATH, curmap->CellAt(x1, y1));
	    ((Creature *)Selectlist[ctr])->Do(tmpa);
	    }
	  }
	}
      return;
      }
    if(Selectsize == 1)  {
      if((x1==x2) && (y1==y2) && 
		(x1==((Cell *)((Creature *)Selectlist[0])->Location(0))->XCoord()>>1) &&
		(y1==((Cell *)((Creature *)Selectlist[0])->Location(0))->YCoord()))  {
	int xd, yd, xneg, yneg, ndir;
	xd = ma->m.x - ((Cell *)((Creature *)Selectlist[0])->Location(0))->XPos();
	yd = ma->m.y - ((Cell *)((Creature *)Selectlist[0])->Location(0))->YPos();
	xneg = 0;         yneg = 0;
	if(xd < 0)  { xd=-xd; xneg = 1; }
	if(yd < 0)  { yd=-yd; yneg = 1; }
	if(9*xd <= yd<<1)  ndir = DIR_S;
	else if(xd <= yd)  ndir = DIR_SE;
	else if(xd <= 3*yd)  ndir = DIR_ES;
	else ndir = DIR_E;
	if((xneg) && (yneg))  ndir = 6+ndir;
	else if(xneg)  ndir = 6-ndir;
	else if(yneg)  ndir = 12-ndir;
	ndir %= 12;

	if(((Creature *)Selectlist[0])->ToldYou != NULL)
		((Creature *)Selectlist[0])->ToldYou->Play();
	Action *tmpa = new Action(ACTION_TURN, ndir);
	CommandSelected(tmpa);
	return;
	}
      else  {
	x1=(x1+x2)>>1;
	y1=(y1+y2)>>1;
	x2=x1;
	y2=y1;
	}
      }
    if((x1==x2) && (y1==y2))  {
      if(((Creature *)Selectlist[0])->ToldYou != NULL)
		((Creature *)Selectlist[0])->ToldYou->Play();
      Action *tmpa = new Action(ACTION_GO, curmap->CellAt(x1, y1));
      CommandSelected(tmpa);
      }
    else  {
      int xd = x2+1-x1, yd = y2+1-y1;
      if(((Creature *)Selectlist[0])->ToldYou != NULL)
		((Creature *)Selectlist[0])->ToldYou->Play();
      for(ctr=0; ctr<Selectsize; ctr++)  {
	Action *tmpa = new Action(ACTION_GO, curmap->CellAt(x1+(random()%xd),
		y1+(random()%yd)));
	((Creature *)Selectlist[ctr])->Do(tmpa);
	}
      }
    }
  }

int Player::IsEnemy(Thing *inguy)  {
  return (((Creature *)inguy)->Owner() != this);
  }

int Player::IsAlly(Thing *inguy)  {
  return (((Creature *)inguy)->Owner() == this);
  }

int Player::IsOwn(Thing *inguy)  {
  return (((Creature *)inguy)->Owner() == this);
  }

void Player::ClearSelectList()  {
  int ctr, tmps;
  Thing **tmpl;
  tmpl=Selectlist;
  tmps=Selectsize;
  Selectlist = NULL;
  Selectsize = 0;
  for(ctr=0; ctr<tmps; ctr++)  {
    tmpl[ctr]->Deselect();
    }
  if(tmpl != NULL)  delete tmpl;
  }

int Player::IsInSelectList(Thing *in)  {
  int ctr;
  for(ctr=0; ctr<Selectsize; ctr++)  {
    if(Selectlist[ctr] == in)  return(1==1);
    }
  return(1==2);
  }

void Player::RemoveFromSelectList(Thing *in)  {
  if(!IsInSelectList(in))  return;
  in->Deselect();
  int ctr;
  for(ctr=0; ctr<Selectsize; ctr++)  {
    if(Selectlist[ctr] == in)  {
      Selectsize--;
      for(; ctr<Selectsize; ctr++)  {
	Selectlist[ctr] = Selectlist[ctr+1];
	}
      Selectlist[ctr] = NULL;
      }
    }
  }

void Player::AddToSelectList(Thing *in)  {
  if(((Creature *)in)->Owner() != this || IsInSelectList(in))  return;
  int ctr;
  Thing **tmpl;
  tmpl=Selectlist;
  Selectlist=new Thing*[Selectsize+1];
  for(ctr=0; ctr<Selectsize; ctr++)  {
    Selectlist[ctr] = tmpl[ctr];
    }
  Selectlist[ctr] = in;
  Selectsize++;
  }


void Player::Select(InputAction *ma, IntList &mas)  {
  Thing *sel;
  int mod = 0, ctr, said = 0;
  for(ctr=0; ctr<mas.Size(); ctr++)  {
    sel = things.FindBySpriteNumber(mas[ctr]);
    if(sel != NULL)  {
      if(((Creature *)sel)->Owner() == this)  {
/*
	if(ma.ModKeyPressed(MOD_RCTRL) || ma.ModKeyPressed(MOD_LCTRL))  {
	  sel->ToggleSelect();
	  if((!said) && (((Creature *)sel)->IsSelected())
		&& (((Creature *)sel)->Owner() == this)
		&& ((Creature *)sel)->GotYou != NULL)  {
	    ((Creature *)sel)->GotYou->Play();
	    said = 1;
	    }
	  }
	else if(ma.ModKeyPressed(MOD_RSHIFT) || ma.ModKeyPressed(MOD_LSHIFT)) {
	  sel->Select();
	  if((!said) && (((Creature *)sel)->Owner() == this)
		&& ((Creature *)sel)->GotYou != NULL)  {
	    ((Creature *)sel)->GotYou->Play();
	    said = 1;
	    }
	  }
	else ********** */ {
	  if(mod == 0)  {
	    ClearSelectList();
	    mod=1;
	    if(((Creature *)sel)->GotYou != NULL)  {
	      ((Creature *)sel)->GotYou->Play();
	      said = 1;
	      }
	    }
	  sel->Select();
	  }
	}
      }
    }
  RefigureSpellcasters();
  }

void Player::DoSpecial(InputAction *curact)  {
  IntList selected;
  if(curact->g.type == INPUTACTION_MOUSEDOWN)
    selected = screen->CollideRectangle(
	    curact->m.x, curact->m.y, curact->m.xs, curact->m.ys);
  switch(doing)  {
    case(DOING_PICKPRAYER):  {
      int key = -1;
      if(curact->g.type == INPUTACTION_KEYDOWN) key = curact->k.key;
      else if(curact->g.type == INPUTACTION_CONTROLDOWN)  {
	int ctr;
	for(ctr=0; ctr<256; ctr++) {
	  if(curact->c.control == __nb[ctr]) key = ctr;
	  }
	}
      if(key >= 0)  {
	switch(key)  {
	  case(KEY_A): *workingint = PRAYER_NONE;		break;
	  case(KEY_B): *workingint = PRAYER_BLESS;		break;
	  case(KEY_C): *workingint = PRAYER_CONTROL_UNDEAD;	break;
	  case(KEY_D): *workingint = PRAYER_DEATH;		break;
	  case(KEY_E): *workingint = PRAYER_EARTHQUAKE;	break;
	  case(KEY_F): *workingint = PRAYER_FRENZY;		break;
	  case(KEY_G): *workingint = PRAYER_NONE;		break;
	  case(KEY_H): *workingint = PRAYER_HEAL;		break;
	  case(KEY_I): *workingint = PRAYER_NONE;		break;
	  case(KEY_J): *workingint = PRAYER_NONE;		break;
	  case(KEY_K): *workingint = PRAYER_NONE;		break;
	  case(KEY_L): *workingint = PRAYER_CALL_LIGHTNING;	break;
	  case(KEY_M): *workingint = PRAYER_METEOR_STRIKE;	break;
	  case(KEY_N): *workingint = PRAYER_NONE;		break;
	  case(KEY_O): *workingint = PRAYER_NONE;		break;
	  case(KEY_P): *workingint = PRAYER_PLAGUE;		break;
	  case(KEY_Q): *workingint = PRAYER_NONE;		break;
	  case(KEY_R): *workingint = PRAYER_RESSURECT;		break;
	  case(KEY_S): *workingint = PRAYER_STORM;		break;
	  case(KEY_T): *workingint = PRAYER_TSUNAMI;		break;
	  case(KEY_U): *workingint = PRAYER_UNDEATH;		break;
	  case(KEY_V): *workingint = PRAYER_NONE;		break;
	  case(KEY_W): *workingint = PRAYER_WHIRLWIND;		break;
	  case(KEY_X): *workingint = PRAYER_EXCORCISM;		break;
	  case(KEY_Y): *workingint = PRAYER_NONE;		break;
	  }
	if(key != KEY_ESC)  {
	  curact->g.type = INPUTACTION_NONE;
	  }
	if(*workingint >= 0)  {
//	  printf("Picked Prayer!\r\n");
	  workingint = NULL;
	  doing = DOING_NORMAL;
	  curact->g.type = INPUTACTION_NONE;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_PICKSTRUCTURE):  {
      int key = -1;
      if(curact->g.type == INPUTACTION_KEYDOWN) key = curact->k.key;
      else if(curact->g.type == INPUTACTION_CONTROLDOWN)  {
	int ctr;
	for(ctr=0; ctr<256; ctr++) {
	  if(curact->c.control == __bb[ctr]) key = ctr;
	  }
	}
      if(key >= 0)  {
	switch(key)  {
	  case(KEY_A): *workingint = STRUCT_AQUADUCT;		break;
	  case(KEY_B): *workingint = STRUCT_BRIDGE;		break;
	  case(KEY_C): *workingint = STRUCT_CART;		break;
	  case(KEY_D): *workingint = STRUCT_DRAWBRIDGE;	break;
	  case(KEY_E): *workingint = STRUCT_ENCLOSURE;		break;
	  case(KEY_F): *workingint = STRUCT_FARMLAND;		break;
	  case(KEY_G): *workingint = STRUCT_GATE;		break;
	  case(KEY_H): *workingint = STRUCT_HULL;		break;
	  case(KEY_I): *workingint = STRUCT_IRRIGATION_DITCH;	break;
	  case(KEY_J): *workingint = -1;			break;
	  case(KEY_K): *workingint = -1;			break;
	  case(KEY_L): *workingint = -1;			break;
	  case(KEY_M): *workingint = STRUCT_MACHINE;		break;
	  case(KEY_N): *workingint = -1;			break;
	  case(KEY_O): *workingint = -1;			break;
	  case(KEY_P): *workingint = STRUCT_PATH;		break;
	  case(KEY_Q): *workingint = -1;			break;
	  case(KEY_R): *workingint = STRUCT_RAMP;		break;
	  case(KEY_S): *workingint = STRUCT_SEIGE_ENGINE;	break;
	  case(KEY_T): *workingint = STRUCT_TOWER;		break;
	  case(KEY_U): *workingint = -1;			break;
	  case(KEY_V): *workingint = -1;			break;
	  case(KEY_W): *workingint = STRUCT_WALL;		break;
	  case(KEY_Y): *workingint = -1;			break;
	  case(KEY_X): *workingint = -1;			break;
	  }
	if(key != KEY_ESC)  {
	  curact->g.type = INPUTACTION_NONE;
	  }
	if(*workingint >= 0)  {
//	  printf("Picked Struct!\r\n");
	  workingint = NULL;
	  doing = DOING_NORMAL;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_PICKMATERIAL):  {
      int key = -1;
      if(curact->g.type == INPUTACTION_KEYDOWN) key = curact->k.key;
      else if(curact->g.type == INPUTACTION_CONTROLDOWN)  {
	int ctr;
	for(ctr=0; ctr<256; ctr++) {
	  if(curact->c.control == __mb[ctr]) key = ctr;
	  }
	}
      if(key >= 0)  {
	switch(key)  {
	  case(KEY_A): *workingint = MATERIAL_ADM;	break;
	  case(KEY_B): *workingint = MATERIAL_BRONZE;	break;
	  case(KEY_C): *workingint = -1;		break;
	  case(KEY_D): *workingint = MATERIAL_DIRT;	break;
	  case(KEY_E): *workingint = -1;		break;
	  case(KEY_F): *workingint = -1;		break;
	  case(KEY_G): *workingint = -1;		break;
	  case(KEY_H): *workingint = -1;		break;
	  case(KEY_I): *workingint = MATERIAL_IRON;	break;
	  case(KEY_J): *workingint = -1;		break;
	  case(KEY_K): *workingint = -1;		break;
	  case(KEY_L): *workingint = -1;		break;
	  case(KEY_M): *workingint = -1;		break;
	  case(KEY_N): *workingint = -1;		break;
	  case(KEY_O): *workingint = -1;		break;
	  case(KEY_P): *workingint = -1;		break;
	  case(KEY_Q): *workingint = -1;		break;
	  case(KEY_R): *workingint = MATERIAL_ROCK;	break;
	  case(KEY_S): *workingint = MATERIAL_STEEL;	break;
	  case(KEY_T): *workingint = -1;		break;
	  case(KEY_U): *workingint = -1;		break;
	  case(KEY_V): *workingint = -1;		break;
	  case(KEY_W): *workingint = MATERIAL_WOOD;	break;
	  case(KEY_Y): *workingint = -1;		break;
	  case(KEY_X): *workingint = -1;		break;
	  }
	if(key != KEY_ESC)  {
	  curact->g.type = INPUTACTION_NONE;
	  }
	if(*workingint >= 0)  {
//	  printf("Picked Material = %d!\r\n", *workingint);
	  workingint = NULL;
	  doing = DOING_NORMAL;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_PICKSPELL):  {
      int key = -1;
      if(curact->g.type == INPUTACTION_KEYDOWN) key = curact->k.key;
      else if(curact->g.type == INPUTACTION_CONTROLDOWN)  {
	int ctr;
	for(ctr=0; ctr<256; ctr++) {
	  if(curact->c.control == __sb[ctr]) key = ctr;
	  }
	}
      if(key >= 0)  {
	switch(key)  {
	  case(KEY_A): *workingint = SPELL_ARROWS;		break;
	  case(KEY_B): *workingint = SPELL_BLINDNESS;		break;
	  case(KEY_C): *workingint = SPELL_CONTROL_CREATURE;	break;
	  case(KEY_D): *workingint = SPELL_DISINTIGRATE;	break;
	  case(KEY_E): *workingint = SPELL_NONE;		break;
	  case(KEY_F): *workingint = SPELL_FIREBALL;		break;
	  case(KEY_G): *workingint = SPELL_GLOBE_OF_SEEING;	break;
	  case(KEY_H): *workingint = SPELL_NONE;		break;
	  case(KEY_I): *workingint = SPELL_INVISIBILITY;	break;
	  case(KEY_J): *workingint = SPELL_JUMP;		break;
	  case(KEY_K): *workingint = SPELL_NONE;		break;
	  case(KEY_L): *workingint = SPELL_LIGHTNING_BOLT;	break;
	  case(KEY_M): *workingint = SPELL_MANNA_DRAIN;	break;
	  case(KEY_N): *workingint = SPELL_NEGATE_MAGIC;	break;
	  case(KEY_O): *workingint = SPELL_OPEN_PORTAL;	break;
	  case(KEY_P): *workingint = SPELL_PROTECTION;		break;
	  case(KEY_Q): *workingint = SPELL_QUICKNESS;		break;
	  case(KEY_R): *workingint = SPELL_REOPEN_PORTAL;	break;
	  case(KEY_S): *workingint = SPELL_SINKHOLE;		break;
	  case(KEY_T): *workingint = SPELL_TELEPORT;		break;
	  case(KEY_U): *workingint = SPELL_UNDO_DAMAGE;	break;
	  case(KEY_V): *workingint = SPELL_VISION;		break;
	  case(KEY_W): *workingint = SPELL_WALL_OF_STONE;	break;
	  case(KEY_X): *workingint = SPELL_EXTINGUISH;		break;
	  case(KEY_Y): *workingint = SPELL_NONE;		break;
	  }
	if(key != KEY_ESC)  {
	  curact->g.type = INPUTACTION_NONE;
	  }
	if(*workingint >= 0)  {
//	  printf("Picked Spell!\r\n");
	  workingint = NULL;
	  doing = DOING_NORMAL;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_SELECTTHING):  {
      if(curact->g.type == INPUTACTION_MOUSEDOWN
	&& (curact->m.panel == mainp || curact->m.panel == minip))  {
	int x = curact->m.x, y = curact->m.y;
	Cell *tmpc;
	if(curact->m.panel == mainp)  curmap->Screen2Cell(x, y);
	else  curmap->Mini2Cell(x, y);
	tmpc = curmap->CellAt(x, y);
	if(tmpc == NULL) *workingint = 0;
	else *workingint = tmpc->Number();
	workingint = NULL;
	doing = DOING_NORMAL;
	curact->g.type = INPUTACTION_NONE;
	ResetInput();
	}
      }break;
    case(DOING_SELECTCELL):  {
      if(curact->g.type == INPUTACTION_MOUSEDOWN
	&& (curact->m.panel == mainp || curact->m.panel == minip))  {
	int x = curact->m.x, y = curact->m.y;
	Cell *tmpc;
	if(curact->m.panel == mainp)  curmap->Screen2Cell(x, y);
	else  curmap->Mini2Cell(x, y);
	tmpc = curmap->CellAt(x, y);
	if(tmpc == NULL) *workingint = 0;
	else *workingint = tmpc->Number();
	workingint = NULL;
	doing = DOING_NORMAL;
	curact->g.type = INPUTACTION_NONE;
	ResetInput();
	}
      }break;
    case(DOING_SELECTCELLS):  {
      if(curact->g.type == INPUTACTION_MOUSEDOWN
	&& (curact->m.panel == mainp || curact->m.panel == minip))  {
	int x1 = curact->m.x, y1 = curact->m.y;
	int x2 = x1+curact->m.xs, y2 = y1+curact->m.ys;
	if(x1>x2)  { int tmp = x1; x1=x2; x2=tmp; }
	if(y1>y2)  { int tmp = y1; y1=y2; y2=tmp; }
	if(curact->m.panel == mainp)  {
	  curmap->Screen2Cell(x1, y1);
	  curmap->Screen2Cell(x2, y2);
	  }
	else  {
	  curmap->Mini2Cell(x1, y1);
	  curmap->Mini2Cell(x2, y2);
	  }
	Cell *tmpc;
	int ctrx, ctry;
	for(ctrx=x1; ctrx<=x2; ctrx++)  {
	  for(ctry=y1; ctry<=y2; ctry++)  {
	    tmpc = curmap->CellAt(ctrx, ctry);
	    if(tmpc != NULL && tmpc->Type() == THING_CELL)  {
	      *workinglist += tmpc->Number();
	      }
	    }
	  }
	workinglist = NULL;
	doing = DOING_NORMAL;
	curact->g.type = INPUTACTION_NONE;
	ResetInput();
	}
      }break;
    case(DOING_SELECTLCELLS):  {
      if(curact->g.type == INPUTACTION_MOUSEDOWN
	&& (curact->m.panel == mainp || curact->m.panel == minip))  {
	int x1 = curact->m.x, y1 = curact->m.y;
	int x2 = x1+curact->m.xs, y2 = y1+curact->m.ys;
//	if(x1>x2)  { int tmp = x1; x1=x2; x2=tmp; }
//	if(y1>y2)  { int tmp = y1; y1=y2; y2=tmp; }
	if(curact->m.panel == mainp)  {
	  curmap->Screen2Cell(x1, y1);
	  curmap->Screen2Cell(x2, y2);
	  }
	else  {
	  curmap->Mini2Cell(x1, y1);
	  curmap->Mini2Cell(x2, y2);
	  }
	Cell *tmpc, *endc;
	tmpc = curmap->CellAt(x1, y1);
	endc = curmap->CellAt(x2, y2);
	while(tmpc != NULL)  {
	  *workinglist += tmpc->Number();
	  if(tmpc == endc)  tmpc = NULL;
	  else tmpc = tmpc->Next(tmpc->RealDirectionTo(endc));
	  }
	workinglist = NULL;
	doing = DOING_NORMAL;
	curact->g.type = INPUTACTION_NONE;
	ResetInput();
	}
      }break;
    case(DOING_SELECTCREATURE):  {
      if(workinglist == NULL)  U2_Exit(1, "Bad workinglist!!!\r\n");
      if(curact->g.type == INPUTACTION_MOUSEDOWN && selected.Size() > 0)  {
	int ctr;
	Thing *sel;
	for(ctr=0; (ctr<selected.Size()) && (workingint != NULL); ctr++)  {
	  sel = things.FindBySpriteNumber(selected[ctr]);
	  if(sel != NULL && sel->Type() == THING_CREATURE)  {
	    *workingint = sel->Number();
	    workingint = NULL;
	    doing = DOING_NORMAL;
	    curact->g.type = INPUTACTION_NONE;
	    ResetInput();
	    }
	  }
	}
      }break;
    case(DOING_SELECTCREATURES):  {
      if(workinglist == NULL)  U2_Exit(1, "Bad workinglist!!!\r\n");
      if(curact->g.type == INPUTACTION_MOUSEDOWN && selected.Size() > 0)  {
	int ctr;
	Thing *sel;
	for(ctr=0; ctr<selected.Size(); ctr++)  {
	  sel = things.FindBySpriteNumber(selected[ctr]);
	  if(sel != NULL && sel->Type() == THING_CREATURE)
	    *workinglist += sel->Number();
	  }
	if(workinglist->Size() > 0)  {
	  workinglist = NULL;
	  doing = DOING_NORMAL;
	  curact->g.type = INPUTACTION_NONE;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_SELECTFCREATURES):  {
      if(workinglist == NULL)  U2_Exit(1, "Bad workinglist!!!\r\n");
      if(curact->g.type == INPUTACTION_MOUSEDOWN && selected.Size() > 0)  {
	int ctr;
	IntList alies, enemies;
	Thing *sel;
	for(ctr=0; ctr<selected.Size(); ctr++)  {
	  sel = things.FindBySpriteNumber(selected[ctr]);
	  if(sel != NULL && sel->Type() == THING_CREATURE)  {
	    if(this->IsOwn(sel))
	      *workinglist += sel->Number();
	    else if(this->IsAlly(sel))
	      alies += sel->Number();
	    else
	      enemies += sel->Number();
	    }
	  }
	*workinglist += alies;
	if(workinglist->Size() <= 0)  *workinglist += enemies;
	if(workinglist->Size() > 0)  {
	  workinglist = NULL;
	  doing = DOING_NORMAL;
	  curact->g.type = INPUTACTION_NONE;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_SELECTECREATURES):  {
      if(workinglist == NULL)  U2_Exit(1, "Bad workinglist!!!\r\n");
      if(curact->g.type == INPUTACTION_MOUSEDOWN && selected.Size() > 0)  {
	int ctr;
	IntList alies, friends;
	Thing *sel;
	for(ctr=0; ctr<selected.Size(); ctr++)  {
	  sel = things.FindBySpriteNumber(selected[ctr]);
	  if(sel != NULL && sel->Type() == THING_CREATURE)  {
	    if(this->IsOwn(sel))
	      friends += sel->Number();
	    else if(this->IsAlly(sel))
	      alies += sel->Number();
	    else
	      *workinglist += sel->Number();
	    }
	  }
	if(workinglist->Size() <= 0)  *workinglist += alies;
	if(workinglist->Size() <= 0)  *workinglist += friends;
	if(workinglist->Size() > 0)  {
	  workinglist = NULL;
	  doing = DOING_NORMAL;
	  curact->g.type = INPUTACTION_NONE;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_SELECTOBJS):  {
      if(workinglist == NULL)  U2_Exit(1, "Bad workinglist!!!\r\n");
      if(curact->g.type == INPUTACTION_MOUSEDOWN && selected.Size() > 0)  {
	int ctr;
	Thing *sel;
	for(ctr=0; ctr<selected.Size(); ctr++)  {
	  sel = things.FindBySpriteNumber(selected[ctr]);
	  if(sel != NULL && (sel->Type() == THING_CREATURE
		|| sel->Type() == THING_STRUCT))
	    *workinglist += sel->Number();
	  }
	if(workinglist->Size() > 0)  {
	  workinglist = NULL;
	  doing = DOING_NORMAL;
	  curact->g.type = INPUTACTION_NONE;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_SELECTFOBJS):  {
      if(workinglist == NULL)  U2_Exit(1, "Bad workinglist!!!\r\n");
      if(curact->g.type == INPUTACTION_MOUSEDOWN && selected.Size() > 0)  {
	int ctr;
	IntList alies, enemies, objs;
	Thing *sel;
	for(ctr=0; ctr<selected.Size(); ctr++)  {
	  sel = things.FindBySpriteNumber(selected[ctr]);
	  if(sel != NULL && (sel->Type() == THING_CREATURE
		|| sel->Type() == THING_STRUCT))  {
	    if(sel->Type() == THING_STRUCT)
	      objs += sel->Number();
	    else if(this->IsOwn(sel))
	      *workinglist += sel->Number();
	    else if(this->IsAlly(sel))
	      alies += sel->Number();
	    else
	      enemies += sel->Number();
	    }
	  }
	*workinglist += alies;
	if(workinglist->Size() <= 0)  *workinglist += objs;
	if(workinglist->Size() <= 0)  *workinglist += enemies;
	if(workinglist->Size() > 0)  {
	  workinglist = NULL;
	  doing = DOING_NORMAL;
	  curact->g.type = INPUTACTION_NONE;
	  ResetInput();
	  }
	}
      }break;
    case(DOING_SELECTEOBJS):  {
      if(workinglist == NULL)  U2_Exit(1, "Bad workinglist!!!\r\n");
      if(curact->g.type == INPUTACTION_MOUSEDOWN && selected.Size() > 0)  {
	int ctr;
	IntList alies, friends, objs;
	Thing *sel;
	for(ctr=0; ctr<selected.Size(); ctr++)  {
	  sel = things.FindBySpriteNumber(selected[ctr]);
	  if(sel != NULL && (sel->Type() == THING_CREATURE
		|| sel->Type() == THING_STRUCT))  {
	    if(sel->Type() == THING_STRUCT)
	      objs += sel->Number();
	    else if(this->IsOwn(sel))
	      friends += sel->Number();
	    else if(this->IsAlly(sel))
	      alies += sel->Number();
	    else
	      *workinglist += sel->Number();
	    }
	  }
	if(workinglist->Size() <= 0)  *workinglist += objs;
	if(workinglist->Size() <= 0)  *workinglist += alies;
	if(workinglist->Size() <= 0)  *workinglist += friends;
	if(workinglist->Size() > 0)  {
	  workinglist = NULL;
	  doing = DOING_NORMAL;
	  curact->g.type = INPUTACTION_NONE;
	  ResetInput();
	  }
	}
      }break;
    }
  if(curact->g.type == INPUTACTION_MOUSEDOWN && curact->m.button == 1)  {
    curact->g.type = INPUTACTION_NONE;
    }
  }

void Player::RefigureSpellcasters()  {
  if(type != PLAYER_CONSOLE)  return;
//  printf("Refiguring...\r\n");
  int ctr;
  selcast = 0;
  selpray = 0;
  for(ctr = 0; ctr < Selectsize; ctr++)  {
//    if(!((Creature*)Selectlist[ctr])->IsSustaining())  {
      selcast |= ((Creature*)Selectlist[ctr])->Spells();
      selpray |= ((Creature*)Selectlist[ctr])->Prayers();
//      }
//    else printf("Sustaining!!!\r\n");
//    if((!magical) && (((Creature*)Selectlist[ctr])->Spells()
//	|| ((Creature*)Selectlist[ctr])->Spells()))  magical = 1;
    }
  Clickey *cb;
  cb = (Clickey*)screen->GetSpriteByNumber(__com[KEY_C]);
  if(selcast)  {
    if(!cb->Visible())  {
      cb->Draw();
      input->MapKeyToControl(KEY_C, cb);
      cb->Enable();
      }
    }
  else  {
    if(cb->Visible())  {
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    }
  cb = (Clickey*)screen->GetSpriteByNumber(__com[KEY_N]);
  if(selpray)  {
    if(!cb->Visible())  {
      cb->Draw();
      input->MapKeyToControl(KEY_N, cb);
      cb->Enable();
      }
    }
  else  {
    if(cb->Visible())  {
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    }
  cb = (Clickey*)screen->GetSpriteByNumber(__com[KEY_Q]);
  if(selcast | selpray)  {
    if(!cb->Visible())  {
      cb->Draw();
      input->MapKeyToControl(KEY_Q, cb);
      cb->Enable();
      }
    }
  else  {
    if(cb->Visible())  {
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    }
  }

void Player::ResetState()  {
  argstate = 0;
  doing = DOING_NORMAL;
  listparam[0].Clear();
  listparam[1].Clear();
  listparam[2].Clear();
  listparam[3].Clear();
  intparam[0] = -1;
  intparam[1] = -1;
  intparam[2] = -1;
  intparam[3] = -1;
  cur_comm = COMMAND_DEFAULT;
  workingint = NULL;
  workinglist = NULL;
  int ctr;
  Clickey *cb;
  for(ctr=0; ctr<256; ctr++)  {
    if(__com[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__com[ctr]);
      if(!cb->Visible())  {
	cb->Draw();
	input->MapKeyToControl(ctr, cb);
	cb->Enable();
	}
      }
    if(__sb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__sb[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__nb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__nb[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__bb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__bb[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__mb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__mb[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    }
  RefigureSpellcasters();
  }

void Player::PickSpell(int *v)  {
  int ctr;
  Clickey *cb;
  for(ctr=0; ctr<256; ctr++)  {
    if(__com[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__com[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__sb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__sb[ctr]);
      if(cb->State())  cb->SetState(0);	cb->Draw();
      input->MapKeyToControl(ctr, cb);
      cb->Enable();
      }
    if(__sb[ctr] != 0 && (selcast & (1 << s2n[ctr])))  {
      cb = (Clickey*)screen->GetSpriteByNumber(__sb[ctr]);
      if(cb->State())  cb->SetState(0);	cb->Draw();
      input->MapKeyToControl(ctr, cb);
      cb->Enable();
      }
    }
  workingint = v, workinglist = NULL, doing = DOING_PICKSPELL;
  }

void Player::PickPrayer(int *v)  { 
  int ctr;
  Clickey *cb;
  for(ctr=0; ctr<256; ctr++)  {
    if(__com[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__com[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__nb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__nb[ctr]);
      if(cb->State())  cb->SetState(0);	cb->Draw();
      input->MapKeyToControl(ctr, cb);
      cb->Enable();
      }
    }
  workingint = v, workinglist = NULL, doing = DOING_PICKPRAYER;
  }

void Player::PickStructure(int *v)  {
  int ctr;
  Clickey *cb;
  for(ctr=0; ctr<256; ctr++)  {
    if(__com[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__com[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__bb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__bb[ctr]);
      if(cb->State())  cb->SetState(0);	cb->Draw();
      input->MapKeyToControl(ctr, cb);
      cb->Enable();
      }
    }
  workingint = v, workinglist = NULL, doing = DOING_PICKSTRUCTURE;
  }

void Player::PickMaterial(int *v)  {
  int ctr;
  Clickey *cb;
  for(ctr=0; ctr<256; ctr++)  {
    if(__com[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__com[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__bb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__bb[ctr]);
      if(cb->State())  cb->SetState(0);
      cb->Erase();
      input->UnmapControl(cb);
      cb->Disable();
      }
    if(__mb[ctr] != 0)  {
      cb = (Clickey*)screen->GetSpriteByNumber(__mb[ctr]);
      if(cb->State())  cb->SetState(0);	cb->Draw();
      input->MapKeyToControl(ctr, cb);
      cb->Enable();
      }
    }
  workingint = v, workinglist = NULL, doing = DOING_PICKMATERIAL;
//  printf("picking mat!\n");
  }

void Player::SelectCell(int *v)
  {
  mouse->SetBehavior(mainp, 1, MB_CLICK);
  mouse->SetBehavior(mainp, 2, MB_CLICK);
  mouse->SetBehavior(mainp, 3, MB_CLICK);
  mouse->SetBehavior(minip, 1, MB_CLICK);
  mouse->SetBehavior(minip, 2, MB_CLICK);
  mouse->SetBehavior(minip, 3, MB_CLICK);
  workingint = v, workinglist = NULL, doing = DOING_SELECTCELL;
  }

void Player::SelectCells(IntList *v)   
  {
  workingint = NULL, workinglist = v, doing = DOING_SELECTCELLS;
  }

void Player::SelectLCells(IntList *v)   
  {
  mouse->SetBehavior(mainp, 1, MB_LINE);
  mouse->SetBehavior(mainp, 2, MB_CLICK);
  mouse->SetBehavior(mainp, 3, MB_CLICK);
  mouse->SetBehavior(minip, 1, MB_LINE);
  mouse->SetBehavior(minip, 2, MB_CLICK);
  mouse->SetBehavior(minip, 3, MB_CLICK);
  workingint = NULL, workinglist = v, doing = DOING_SELECTLCELLS;
  }

void Player::SelectThing(int *v)
  {
  workingint = v, workinglist = NULL, doing = DOING_SELECTTHING;
  }

void Player::SelectCreature(int *v)
  {
  workingint = v, workinglist = NULL, doing = DOING_SELECTCREATURE;
  }

void Player::SelectFriendlyCreatures(IntList *v)
  {
  workingint = NULL, workinglist = v, doing = DOING_SELECTFCREATURES;
  }

void Player::SelectEnemyCreatures(IntList *v)
  {
  workingint = NULL, workinglist = v, doing = DOING_SELECTECREATURES;
  }

void Player::SelectCreatures(IntList *v)
  {
  workingint = NULL, workinglist = v, doing = DOING_SELECTCREATURES;
  }

void Player::SelectObject(int *v)
  {
  workingint = v, workinglist = NULL, doing = DOING_SELECTOBJ;
  }

void Player::SelectObjects(IntList *v)
  {
  workingint = NULL, workinglist = v, doing = DOING_SELECTOBJS;
  }

void Player::SelectFriendlyObjects(IntList *v)
  {
  workingint = NULL, workinglist = v, doing = DOING_SELECTFOBJS;
  }

void Player::SelectEnemyObjects(IntList *v)
  {
  workingint = NULL, workinglist = v, doing = DOING_SELECTEOBJS;
  }

void Player::ResetInput()  {
  mouse->SetBehavior(0, 1, MB_CLICK);
  mouse->SetBehavior(0, 2, MB_IGNORE);
  mouse->SetBehavior(0, 3, MB_CLICK);
  mouse->SetBehavior(mainp, 1, MB_BOX);
  mouse->SetBehavior(mainp, 2, MB_CLICK);
  mouse->SetBehavior(mainp, 3, MB_BOX);
  mouse->SetBehavior(minip, 1, MB_CLICKDRAW);
  mouse->SetBehavior(minip, 2, MB_CLICK);
  mouse->SetBehavior(minip, 3, MB_BOX);
  input->MapKeyToControl(KEY_PAUSE, MainButtonNum);
  }

void Player::CommandSelected(Action *tmpa)  {
  int ctr;
  if(key->IsPressed(KEY_LSHIFT))  {
    for(ctr=0; ctr<Selectsize; ctr++)  {
      ((Creature *)Selectlist[ctr])->DoWhenDone(tmpa);
      }
    }
  else  {
    for(ctr=0; ctr<Selectsize; ctr++)  {
      ((Creature *)Selectlist[ctr])->Do(tmpa);
      }
    }
  }

void Player::SetTeamColors(int c1, int c2) {
  U2_Exit(0, "Dynamic Team Color change not supported\n");
  }

int Player::TeamColor1() {
  return tcol1;
  }

int Player::TeamColor2() {
  return tcol2;
  }
