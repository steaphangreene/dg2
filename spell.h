#ifndef	INSOMNIA_SPELL_H
#define	INSOMNIA_SPELL_H
#include "thing.h"

#define SPELL_NONE		-1
#define PRAYER_NONE		-1
#define SPELL_ARROWS		0
#define SPELL_BLINDNESS		1
#define SPELL_CONTROL_CREATURE	2
#define SPELL_DISINTIGRATE	3
#define SPELL_FIREBALL		5
#define SPELL_GLOBE_OF_SEEING	6
#define SPELL_INVISIBILITY	8
#define SPELL_JUMP		9
#define SPELL_LIGHTNING_BOLT	11
#define SPELL_MANNA_DRAIN	12
#define SPELL_NEGATE_MAGIC	13
#define SPELL_OPEN_PORTAL	14
#define SPELL_PROTECTION	15
#define SPELL_QUICKNESS		16
#define SPELL_REOPEN_PORTAL	17
#define SPELL_SINKHOLE		18
#define SPELL_TELEPORT		19
#define SPELL_UNDO_DAMAGE	20
#define SPELL_VISION		21
#define SPELL_WALL_OF_STONE	22
#define SPELL_EXTINGUISH	23
#define SPELL_ZAP		25
#define PRAYER_BLESS		(32+1)
#define PRAYER_CONTROL_UNDEAD	(32+2)
#define PRAYER_DEATH		(32+3)
#define PRAYER_EARTHQUAKE	(32+4)
#define PRAYER_FRENZY		(32+5)
#define PRAYER_HEAL		(32+7)
#define PRAYER_CALL_LIGHTNING	(32+11)
#define PRAYER_METEOR_STRIKE	(32+12)
#define PRAYER_PLAGUE		(32+15)
#define PRAYER_RESSURECT	(32+17)
#define PRAYER_STORM		(32+18)
#define PRAYER_TSUNAMI		(32+19)
#define PRAYER_UNDEATH		(32+20)
#define PRAYER_WHIRLWIND	(32+22)
#define PRAYER_EXCORCISM	(32+23)
#define SPELLPRAYER_MAX		64

class Spell : public Thing {
  public:
  Spell(Thing *, int, const IntList &);
  virtual ~Spell();
  virtual void Select();
  virtual void ToggleSelect();
  void LostCaster(Thing *);
  void LostSubject(Thing *);
  void Dispel();

  protected:
  char name, gsize, status, memory;
  int dodad;
  IntList params, items;
  Sprite **images;
  Thing *caster;
  virtual void tickme();
  virtual void updateme();
  virtual void ReScaleme();
  virtual void ReAlignme(int, int);
  friend class Creature;
  };

#endif
