#ifndef INSOMNIA_THING_H
#define INSOMNIA_THING_H
#include <user/screen.h>
#define THINGLIST_START	(1024*16)
#define THINGLIST_STEP	1024

#define THING_THING	0
#define THING_CELL	1
#define THING_CREATURE	2
#define THING_SPELL	3
#define THING_STRUCT	4

class Cell;

class Thing  {
  public:
  Thing();
  virtual ~Thing();
  int Listen();
  void Talk();
  void ClearTalk();
  void StartTalk();
  void EndTalk();
  void update();
  void tick();
  Thing *FindBySpriteNumber(int);
  void ReScale(); 
  void ReAlign(int, int); 
  virtual void Select(); 
  virtual void Deselect();
  virtual void ToggleSelect();
  int Number()  { return thingnum; }
  char Type()  { return type; }
  static void DeleteAll();
  Cell *Location(int v1)  { return(location[v1]); };

  int Enter(Thing *v1, int, int, int, int);
  int CanEnter(Thing *v1, int, int, int, int);
  void ForceEnter(Thing *v1);
  int EnterHalf(Thing *v1, char v2);
  void Leave(Thing *v1);
  void LeaveHalf(Thing *v1);

  int DistanceTo(Thing *);
  int DirectionTo(Thing *);
  int RealDirectionTo(Thing *);
  void FlipToCloserThing(IntList &);
  int Height()  {return altitude+height;};
  Thing *Inside(int v) {
	if(inside == NULL || inside[v] == this) return NULL;
	return inside[v];
	};
  void Fall();
  Thing *GetThingByNumber(int n) { return list[n]; };

  protected:
  int Interp(char *);
  virtual void ReScaleme(); 
  virtual void ReAlignme(int, int); 
  void ClaimSprite(int);
  void UnclaimSprite(int);
  virtual void tickme();
  virtual void updateme();
  void Add();
  void Remove();
  static unsigned char sendbuf[65536];
  static int sendind;
  static Thing **list;
  static Thing *splist[MAX_SPRITES];
  static char *Changed;
  static char *Waiting;
  static int listsize;
  static int listnext;
  static char talking;
  char type, exists, selected, flying;
  int thingnum;
  int height, altitude;
  IntList spell;
  Cell *location[2];
  Thing **inside;
  friend class Spell;
  };

#endif
