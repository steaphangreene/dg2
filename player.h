#ifndef	INSOMNIA_PLAYER_H
#define	INSOMNIA_PLAYER_H
#include <user/screen.h>
#include "thing.h"
#include "action.h"

typedef int Command;

#define COMMAND_DEFAULT		0
#define COMMAND_MINIDEFAULT	1
#define COMMAND_MOVE		2
#define COMMAND_ATTACK		3
#define COMMAND_KILL		4
#define COMMAND_FOLLOW		5
#define COMMAND_LOOK		6
#define COMMAND_CAST		7
#define COMMAND_PRAY		8
#define COMMAND_PATROL		9
#define COMMAND_FLEE		10
#define COMMAND_BUILD		11
#define COMMAND_EXTINGUISH	12
#define COMMAND_DIG		13

#define PLAYER_CONSOLE	0
#define PLAYER_NETWORK	1
#define PLAYER_COMPUTER	2

#define DOING_NORMAL		0
#define DOING_SELECTTHING	1
#define DOING_SELECTCELL	2
#define DOING_SELECTCELLS	3
#define DOING_SELECTLCELLS	4
#define DOING_SELECTCREATURE	5
#define DOING_SELECTCREATURES	6
#define DOING_SELECTFCREATURES	7
#define DOING_SELECTECREATURES	8
#define DOING_SELECTOBJ		9
#define DOING_SELECTOBJS	10
#define DOING_SELECTFOBJS	11
#define DOING_SELECTEOBJS	12
#define DOING_PICKSPELL		16
#define DOING_PICKPRAYER	17
#define DOING_PICKSTRUCTURE	18
#define DOING_PICKMATERIAL	19

class Player  {
  public:
  Player(char);
  ~Player();
  void AddToSelectList(Thing *);
  void RemoveFromSelectList(Thing *);
  void Select(UserAction);
  void OrderSelected(Command, UserAction);
  int NumSelected()  {return Selectsize; };
  int IsInSelectList(Thing *);
  int IsEnemy(Thing *);
  int IsAlly(Thing *);
  int IsOwn(Thing *);
  char CurrentCommand()  { return cur_comm;};
  void TakeTurn(int);
  void ResetState();
  void PickSpell(int *v);
  void PickPrayer(int *v);
  void PickStructure(int *v);
  void PickMaterial(int *v);
  void SelectThing(int *v);
  void SelectCell(int *v);
  void SelectCells(IntList *v);
  void SelectLCells(IntList *v);
  void SelectObject(int *v);
  void SelectObjects(IntList *v);
  void SelectFriendlyObjects(IntList *v);
  void SelectEnemyObjects(IntList *v);
  void SelectCreature(int *v);
  void SelectFriendlyCreatures(IntList *v);
  void SelectEnemyCreatures(IntList *v);
  void SelectCreatures(IntList *v);
  void RefigureSpellcasters();
  char *GetCMap();
  void SetCMap(char *);

  private:
  void ResetInput();
  void DoSpecial(UserAction &);
  void ClearSelectList();
  Thing **Selectlist;
  int Selectsize;
  char type, doing, cur_comm, argstate;
  unsigned long selcast, selpray;
  int *workingint;
  IntList *workinglist;
  int intparam[4];
  IntList listparam[4];
  char cmap[256];
  Sprite output;
  Sprite outval[16];
  char shown;
  int oldval[16];
  };

#endif
