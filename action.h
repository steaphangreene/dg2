#ifndef	INSOMNIA_ACTION_H
#define	INSOMNIA_ACTION_H

#define	ACTION_NOTHING		0
#define	ACTION_GO		1
#define	ACTION_TURN		2
#define	ACTION_ATTACK		3
#define	ACTION_KILL		4
#define	ACTION_FOLLOW		5
#define	ACTION_FOLLOW_PATH	6
#define	ACTION_PATROL		8
#define	ACTION_FLEE		9
#define	ACTION_STOP_SPELLS	10
#define	ACTION_EXCUSED		11
#define	ACTION_EXTINGUISH	12
#define	ACTION_STOP		13
#define	ACTION_DIG		14
#define	ACTION_LOOK		15
#define	ACTION_CAST		32
#define	ACTION_PRAY		64
#define	ACTION_BUILD		96	//MUST BE HIGHEST!!!

#include "cell.h"
#include <user/list.h>

class Action  {
  public:
  Action()  {
    goal = 0;
    connected = 0;
    };
  Action(Action& in)  {
    goal = in.goal;
    objects = in.objects;
    connected = 0;
    };
  Action(int v1)  {
    goal = v1;
    connected = 0;
    };
  Action(int v1, const IntList &v2)  {
    goal = v1;
    objects = (IntList &)v2;
    connected = 0;
    };
  Action(int v1, Thing *v2)  {
    goal = v1;
    objects.Clear();
    if(v2 != NULL)  objects += v2->Number();
    connected = 0;
    };
  Action(int v1, int v2)  {
    goal = v1;
    objects.Clear();
    objects += v2;
    connected = 0;
    };
  Action operator =(Action& in)  {
    goal = in.goal;
    objects = in.objects;
    connected = 0;
    return *this;
    };
  int Object()  {
    if(objects.Size() == 0)  return 0;
    else  return objects[0];
    }
  void DoneFirst()  {
    objects -= objects[0];
    if(objects.Size() < 1)  goal = 0;
    };
  void Done()  {
    goal = 0;
    objects.Clear();
    };
  void Attach()  {
    connected++;
    };
  void Detach()  {
    connected--;
    if(connected <= 0) delete this;
    };
  int Goal()  { return goal;};
  void SetGoal(int v1)  { goal=v1;};
  IntList objects;

  private:
  int goal, connected;
  };

#endif
