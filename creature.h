#ifndef	INSOMNIA_CREATURE_H
#define	INSOMNIA_CREATURE_H

#include "thing.h"
#include "cell.h"
#include "action.h"
#include "player.h"
#include "stats.h"
#include "weapon.h"

#define	CREATURE_GUY	0
#define CREATURE_MAX	1

#include "graphics/creature/cre.h"

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
  void Do(const Action *);
  void DoWhenDone(const Action *);
  void DoFirst(const Action *);
  Player *Owner()  { return(owner); };
  void SetCreatureGraphic(int, char *);
  int Move(Cell *);
  void Face(int);
  void Damage(int);
  void DamageFull(int);
  void Engage(Creature *);
  int InWeaponRange(Creature *, int);
  void Attack(Creature *, int);
  void Strike(Creature *, int, int, int);
  int IsSelected() { return selected; };
  int IsInWater();
  static Sound *GotYou, *ToldYou;
//  Cell *Location(int v1)  { return (Cell *)location[v1]; };
  int CanCast(int);
  unsigned long Spells() { return spells; };
  unsigned long Prayers() { return prayers; };
  void LearnSpell(int);
  int AddWeapon(Weapon &);
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
  int Approach(Thing *, int);
  void Claimed(Thing *);
  void SetSpeedMul(int);
  static Graphic *selboxg[3];
  static Graphic *pics[3][CREATURE_MAX][FRAME_MAX];
  static color trc1d[CREATURE_MAX];
  static color trc1l[CREATURE_MAX];
  static color trc2d[CREATURE_MAX];
  static color trc2l[CREATURE_MAX];
  static color rrcd[CREATURE_MAX];
  static color rrcl[CREATURE_MAX];
  static color tcol[TCOL_MAX][2];
  static color rcol[RANK_MAX][2];
  static unsigned char **cremap[CREATURE_MAX][RANK_MAX];
  static int GraphicsInitialized;
  static Statistics *cstats[CREATURE_MAX];
  void ResetFrame();
  void UndoVision();
  void DoVision();
  void TurnTo(char);
  void Think();
  void EvaluateGoal();
  void GoalAbort();
  void GoalDone();
  void GoalDoneFirst();
  void ClearGoals();
  int SafestDirection();
  void GetOwnStats();
  void UpdateCMap();
  Action *goal[12], *mission[12];
  void Init();
  char dirt, dirf, facing, distt, attacking, limping, crawling, down, progress;
  int xpos, ypos, gtype, frame;
  Sprite image, selbox, weaps;
  Player *owner;
  IntList sustained_spell;
  void FillVision(char, char);
  void ClearVision(char, char);
  Statistics *stats;
  unsigned long spells, prayers;
  Weapon weap[2];
  int target;
  char exploring, speed, speedbase, speedmul, hit, vision, mvision, cw;
  int fatigue;
  int rank;
  friend class Player;
  friend class Thing;
  friend class Spell;
  char mat_type, mat_ammt;
  };

#endif
