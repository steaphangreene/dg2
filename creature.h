#ifndef	INSOMNIA_CREATURE_H
#define	INSOMNIA_CREATURE_H

#include "thing.h"
#include "cell.h"
#include "action.h"
#include "player.h"
#include "stats.h"
#include "weapon.h"

#define	CREATURE_DUDE	0
#define CREATURE_MAX	1

#define	FRAME_DOWN	0
#define	FRAME_SPLAT	1
#define	FRAME_WALK	32
#define	FRAME_WALK00	32
#define	FRAME_WALK01	33
#define	FRAME_WALK02	34
#define	FRAME_WALK03	35
#define	FRAME_WALK04	36
#define	FRAME_WALK05	37
#define	FRAME_WALK06	38
#define	FRAME_WALK07	39
#define	FRAME_M0	64
#define	FRAME_M0_0	64
#define	FRAME_M0_1	65
#define	FRAME_M0_2	66
#define	FRAME_M0_3	67
#define	FRAME_M0_4	68
#define	FRAME_M0_5	69
#define	FRAME_M0_6	70
#define	FRAME_M0_7	71
#define	FRAME_LIMP	96
#define	FRAME_LIMP00	96
#define	FRAME_LIMP01	97
#define	FRAME_LIMP02	98
#define	FRAME_LIMP03	99
#define	FRAME_LIMP04	100
#define	FRAME_LIMP05	101
#define	FRAME_LIMP06	102
#define	FRAME_LIMP07	103
#define	FRAME_LIMP08	104
#define	FRAME_LIMP09	105
#define	FRAME_LIMP10	106
#define	FRAME_LIMP11	107
#define	FRAME_LIMP12	108
#define	FRAME_LIMP13	109
#define	FRAME_LIMP14	110
#define	FRAME_LIMP15	111
#define	FRAME_CRAWL	128
#define	FRAME_CRAWL00	128
#define	FRAME_CRAWL01	129
#define	FRAME_CRAWL02	130
#define	FRAME_CRAWL03	131
#define	FRAME_MAX	160


class Creature : public Thing {
  public:
  Creature(int, Player *);
  virtual ~Creature();
  virtual void Select();
  virtual void Deselect();
  virtual void ToggleSelect();
  void Place(Cell*);
  void Place(Cell *, int);
  void Go(int);
  void Do(const Action &);
  Player *Owner()  { return(owner); };
  void SetCreatureGraphic(int, char *);
  int Move(Cell *);
  void Face(int);
  void Damage(int);
  void DamageFull(int);
  void SwingAt(Creature *);
  void ShootAt(Creature *);
  int InMeleeRange(Creature *);
  int InMisileRange(Creature *);
  void MeleeAttack(Creature *);
  void MisileAttack(Creature *);
  void Strike(Creature *, int, int, int);
  int IsSelected() { return selected; };
  int IsInWater();
  static DigSample *GotYou, *ToldYou;
//  Cell *Location(int v1)  { return (Cell *)location[v1]; };
  int CanCast(int);
  unsigned long Spells() { return spells; };
  unsigned long Prayers() { return prayers; };
  void LearnSpell(int);
  int IsSustaining() { return (sustained_spell.Size()); };
  void UpdateCondition();
  int ExcuseMe(char);
  int ThankYou();

  protected:
  virtual void tickme();
  virtual void updateme();
  virtual void ReScaleme();
  virtual void ReAlignme(int, int);
  void Act();
  int GetPath(char);
  void Claimed(Thing *);
  void SetSpeedMul(int);
  static Graphic *selboxg[3];
  static Graphic *pics[3][CREATURE_MAX][FRAME_MAX];
  static int GraphicsInitialized;
  static Statistics *cstats[CREATURE_MAX];
  void ResetFrame();
  void UndoVision();
  void DoVision();
  void TurnTo(char);
  void Think();
  void EvaluateGoal();
  int SafestDirection();
  void GetOwnStats();
  Action goal;
  void Init();
  char dirt, dirf, facing, distt, attacking, limping, crawling, down;
  int xpos, ypos, gtype, frame;
  Sprite image, selbox;
  Player *owner;
  IntList sustained_spell;
  void FillVision(char, char);
  void ClearVision(char, char);
  Statistics *stats;
  unsigned long spells, prayers;
  Weapon meleeweap, misileweap;
  int meleetarget, misiletarget;
  char exploring, speed, speedbase, speedmul, hit, vision, mvision;
  int fatigue;
  friend class Player;
  friend class Thing;
  friend class Spell;
  };

#endif
