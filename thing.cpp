#include <string.h>
#include <stdio.h>
#include "action.h"
#include "thing.h"
#include "cell.h"
#include "spell.h"
#include "creature.h"
#include <user/engine.h>
#include <user/net.h>

extern long long ticker;

unsigned char Thing::sendbuf[65536];

int Thing::sendind = 4;

Thing **Thing::list = NULL;

char *Thing::Changed = NULL;

char *Thing::Waiting = NULL;

Thing *Thing::splist[MAX_SPRITES];

int Thing::listsize = 0;

int Thing::listnext = 0;

char Thing::talking = 0;

#define MOUSE_UPDATE_INTERVAL	16384

#include <user/screen.h>
extern Screen *screen;
extern Network net;

void Thing::StartTalk()  {
  talking = 1;
  }

void Thing::EndTalk()  {
  talking = 0;
  }

void Thing::ClearTalk()  {
  sendind = 4;
  }

int Thing::Interp(char *buf)  {
  int ret = 0, ind, size, tmpn, tmpg, tmps, tmpv, ctr;
/*
  memcpy(&size, buf, 4);

//  printf("Interpreting one, size = %d\r\n", size);
  if(size == -1)  {
    memcpy(&ret, &buf[4], 4);
    }
  else  for(ind = 4; ind < size;)  {
    memcpy(&tmpn, &buf[ind], 4);
    ind += 4;
    memcpy(&tmpg, &buf[ind], 4);
    ind += 4;
    memcpy(&tmps, &buf[ind], 4);
    ind += 4;
    Action tmpa;
    tmpa.SetGoal(tmpg);
    for(ctr=0; ctr<tmps; ctr+=4)  {
      memcpy(&tmpv, &buf[ind], 4);
      ind += 4;
      tmpa.objects += tmpv;
      }
    ((Creature *)list[tmpn])->goal = tmpa;
//    printf("Set one creature's goal to (%d, %d)!\r\n", tmpg, tmpv);
    }
//  printf("Done!\r\n");
*/
  return(ret);
  }

void Thing::Talk()  {
/*
  int tmp = sendind;
  memcpy(sendbuf, &tmp, 4);

//  printf("Talking\r\n");
//  printf("Interpreting own!\r\n");
  Interp((char *)sendbuf);
  net.Send(sendbuf, sendind);
//  printf("Sent One, size = %d!\r\n", tmp);
  sendind = 4;
*/
  }

int Thing::Listen()  {
  int ret;
/*
//  printf("Listening\r\n");
  char *buf = (char *)net.Receive();
  while(buf == NULL)  buf = (char *)net.Receive();
//  printf("Interping other!\r\n");
  ret = Interp(buf);
  delete buf;
*/
  return ret;
  }

void Thing::ClaimSprite(int spnum)  {
  splist[spnum] = this;
  }

void Thing::UnclaimSprite(int spnum)  {
  splist[spnum] = NULL;
  }

Thing *Thing::FindBySpriteNumber(int spnum)  {
  return splist[spnum];
  }

Thing::Thing()  {
  altitude = 0;
  height = 0;
  ftemp = 0;
  fresist = 100;
  ffeul = 100;
  inside = NULL;
  location[0] = NULL;
  location[1] = NULL;
  selected = 0;
  exists = 0;
  flying = 0;
  type = THING_THING;
  Add();
  }

Thing::~Thing()  {
  int ctr;
  for(ctr=0; ctr<spell.Size(); ctr++)  {
    if(list[spell[ctr]] != NULL)  {
      delete ((Spell *)list[spell[ctr]]);
      ctr--;
      }
    }
  spell.Clear();
  Remove();
  }

void Thing::DeleteAll()  {
  int ctr;
  for(ctr=listsize-1; ctr>=0; ctr--)  {
    if(list[ctr] != NULL)  {
      delete list[ctr];
      list[ctr] = NULL;
      }
    Changed[ctr] = 0;
    Waiting[ctr] = 0;
    }
  listsize=0;
  listnext=0;
  }

void Thing::Remove()  {
//  int ctr;
//  for(ctr=0; ctr<listsize; ctr++)  {
//    if(list[ctr] == this)  {
//      list[ctr] = NULL;
//      Changed[ctr] = 0;
//      Waiting[ctr] = 0;
//      break;
//      }
//    }
  if(list[thingnum] == this)  {
    list[thingnum] = NULL;
    Changed[thingnum] = 0;
    Waiting[thingnum] = 0;
    }
  }

void Thing::update()  {
  int ctr;
  for(ctr=0; ctr<listnext; ctr++)  {
    if(Changed[ctr] > 0)  {
      Changed[ctr]--;
      if(Changed[ctr] == 0)  list[ctr]->updateme();
      }
    }
  }

void Thing::Add()  {
  if(listsize == 0)  {
    listsize = THINGLIST_START;
    list = new Thing*[THINGLIST_START];
    if(list == NULL)  Exit(1, "Out of Memory for Things list!!\r\n");
    bzero(list, THINGLIST_START*sizeof(Thing *));

    Changed = new char[THINGLIST_START];
    if(Changed == NULL)
	Exit(1, "Out of Memory for Things-changed flags!!\r\n");
    bzero(Changed, THINGLIST_START*sizeof(char));

    Waiting = new char[THINGLIST_START];
    if(Waiting == NULL)
	Exit(1, "Out of Memory for Things-waiting flags!!\r\n");
    bzero(Waiting, THINGLIST_START*sizeof(char));

    bzero(splist, MAX_SPRITES*sizeof(Thing **));
    }
  if(listnext == listsize)  {
    Thing **tmpl = list;
    char *tmpc = Changed;
    char *tmpw = Waiting;
    list = new Thing*[listsize+THINGLIST_STEP];
    bzero(list, (listsize+THINGLIST_STEP)*sizeof(Thing *));

    Changed = new char[listsize+THINGLIST_STEP];
    bzero(Changed, (listsize+THINGLIST_STEP)*sizeof(char));

    Waiting = new char[listsize+THINGLIST_STEP];
    bzero(Waiting, (listsize+THINGLIST_STEP)*sizeof(char));

    memcpy(Changed, tmpc, listsize*sizeof(char));
    memcpy(Waiting, tmpw, listsize*sizeof(char));
    memcpy(list, tmpl, listsize*sizeof(Thing *));
    listsize+=THINGLIST_STEP;
    delete tmpl;
    delete tmpc;
    delete tmpw;
    }
  if(listnext > listsize)  {
    delete screen;
    printf("listnext > listsize (HUH?)\n");
    exit(1);
    }
  thingnum = listnext;
  list[listnext++] = this;
  }

void Thing::ReScale()  {
  int ctr;
  for(ctr=0; ctr<listsize; ctr++)  {
    if(list[ctr] != NULL)  list[ctr]->ReScaleme();
    }
  }

void Thing::ReAlign(int x, int y)  {
  int ctr;
  for(ctr=0; ctr<listsize; ctr++)  {
    if(list[ctr] != NULL)  list[ctr]->ReAlignme(x, y);
    }
  }

void Thing::ReScaleme()  {
  }

void Thing::ReAlignme(int x, int y)  {
  if(x==y); //UNUSED!!!
  }

void Thing::Select()  {
  }

void Thing::Deselect()  {
  }

void Thing::ToggleSelect()  {
  }

void Thing::tick()  {
  int ctr;
  for(ctr=0; ctr<listnext; ctr++)  {
//    if((ctr & MOUSE_UPDATE_INTERVAL) == MOUSE_UPDATE_INTERVAL)
//      screen->Refresh();
    if(list[ctr] != NULL)  {
      if((ctr & 63) == (ticker & 63))  {
        Waiting[ctr] = 0;
        list[ctr]->tickme();
        }
      else if(Waiting[ctr] > 0)  {
        Waiting[ctr]--;
        if(Waiting[ctr] == 0)  list[ctr]->tickme();
        }
      }
    }
  }

void Thing::tickme()  {
  if((ticker & 255) == (thingnum & 255))  {
    if(ftemp>0)  Heat(-1, 500);
    }
  if(ftemp <= fresist)  {
//    Changed[thingnum] = 0;
    Waiting[thingnum] = 0;
    return;  
    }
  if(ffeul<1)  {
    ftemp = 0;
    fresist = 32767;
    if(type == THING_CELL) ((Cell*)this)->terrain = TERRAIN_ASHES;
    Changed[thingnum] = 1;
    return;
    }
  --ffeul;
  Location(0)->StrikeAllIn(Location(0), 200, ftemp, ftemp<<1);
  if(fresist >= ffeul)  fresist = ffeul-1;
  if(ftemp > ffeul)  ftemp = ffeul;
  Waiting[thingnum] = 8;
  int ctr;
  ctr = (random()%6) << 1;
  if(Location(0)->Next(ctr) != NULL)  {
    Location(0)->Next(ctr)->Heat(5, ftemp);
    Location(0)->Next(ctr)->StrikeAllIn(Location(0), 200, ftemp>>2, ftemp>>1);
    }  
  }

void Thing::updateme()  {
  }

void Thing::ForceEnter(Thing *in)  {
  if(inside[0] != inside[1])
    Exit(1, "Force-Entering a half-entered thing!\r\n");
  if(inside[0] != in && inside[0] != NULL)
    { inside[0]->ForceEnter(in); return; }
  inside[0] = in;
  inside[1] = in;
  in->altitude = Height();
  return;
  }

int Thing::Enter(Thing *in, int alt, int ht, int up, int down)  {
//  debug_position = 5500;
//  printf("Entering!\r\n");
  if(!(location[0]->location[0]->Claimed(in)))  {
    Exit(0, "Attempt to enter unclaimed Cell, type = %d!\r\n", type);
    return (1==2);
    }	//  printf("Done checking Claim!\r\n");
//  debug_position = 5510;
  if(in->location[0] == NULL)  {
    if(inside[0] == NULL)  {
      inside[0] = in;
      inside[1] = in;
      in->altitude = Height();
      return (1==1);
      }
    else if(inside[0] != inside[1])  Exit(1, "Bad tele-placement!\n");
    else  {
      return inside[0]->Enter(in, alt, ht, up, down);
      }
    }
//  debug_position = 5520;
  if(Height() > alt+up)  {
    in->altitude++;	//    printf("Climb Up!\r\n");
    return (1==2);
    }
//  debug_position = 5530;
  if((inside[0] != NULL && inside[1] != NULL)
	&& (inside[0] != in && inside[1] != in))  {
    if(inside[0] != inside[1]) return (1==2);
    else if(alt >= Height() && (alt+ht) < inside[0]->altitude 
	&& (alt+ht) < inside[1]->altitude) {
      if(Height() < alt-down)  {
        if(in->altitude > in->Location(0)->Location(0)->WaterLevel()-(in->height))
	  {
	  in->altitude--;	//      printf("Climb Down!\r\n");
	  return (1==2);
	  }
        }
//      printf("Going under!\r\n");
      in->inside[0] = inside[0];
      in->inside[1] = inside[1];
      inside[0] = in;
      inside[1] = in;
//      printf("Taking %p\r\n", in->inside[0]);
      return (1==1);
      }
    else return (inside[0]->Enter(in, alt, ht, up, down));
    }
//  debug_position = 5560;
  if(Height() < alt-down)  {
//    debug_position = 5561;
    if(in->altitude > (Location(0)->Location(0)->WaterLevel() - in->height)) {
//      debug_position = 5562;
      in->altitude--;	//      printf("Climb Down!\r\n");
//      debug_position = 5563;
      return (1==2);
      }
//    debug_position = 5564;
    }
//  debug_position = 5570;
  inside[0] = in;
  inside[1] = in;
  Cell *tmpc[2];
  tmpc[0] = in->location[0];
  tmpc[1] = in->location[1];
  in->location[0] = location[0];
  in->location[1] = location[1];
  in->Fall();
  in->location[0] = tmpc[0];
  in->location[1] = tmpc[1];
//  debug_position = 5580;

//  in->altitude = Height();	//  printf("Done Entering!\r\n");
//  if(Location(0)->Location(0)->WaterDepth() >= in->height)
//	in->altitude = -in->height;
  if(type == THING_CREATURE)  {
    Creature *tmpc = (Creature *)this;
    tmpc->Damage(5);
    }
//  printf("Entered at height %d.\r\n", in->altitude);
//  debug_position = 5590;
  return (1==1);
  }

int Thing::CanEnter(Thing *in, int alt, int ht, int up, int down)  {
//  debug_position = 5600;
//  printf("Entering!\r\n");
  if(!(location[0]->location[0]->Claimed(in)))  {
//    Exit(0, "Attempt to enter unclaimed Cell, type = %d!\r\n", type);
    return (1==2);
    }	//  printf("Done checking Claim!\r\n");
//  debug_position = 5610;
  if(in->location[0] == NULL)  {
    if(inside[0] == NULL)  {
      return (1==1);
      }
    else if(inside[0] != inside[1])  Exit(1, "Bad tele-placement!\n");
    else  {
      return inside[0]->CanEnter(in, alt, ht, up, down);
      }
    }
//  debug_position = 5620;
  if(Height() > alt+up)  {
    in->altitude++;	//    printf("Climb Up!\r\n");
    return (1==2);
    }
//  debug_position = 5630;
  if((inside[0] != NULL && inside[1] != NULL)
	&& (inside[0] != in && inside[1] != in))  {
    if(inside[0] != inside[1]) return (1==2);
    else if(alt >= Height() && (alt+ht) < inside[0]->altitude 
	&& (alt+ht) < inside[1]->altitude) {
      if(Height() < alt-down)  {
        if(in->altitude > in->Location(0)->Location(0)->WaterLevel()-(in->height))
	  {
	  in->altitude--;	//      printf("Climb Down!\r\n");
	  return (1==2);
	  }
        }
//      printf("Going under!\r\n");
      return (1==1);
      }
    else return (inside[0]->CanEnter(in, alt, ht, up, down));
    }
//  debug_position = 5660;
  if(Height() < alt-down)  {
    if(in->altitude > (Location(0)->Location(0)->WaterLevel() - in->height)) {
      in->altitude--;	//      printf("Climb Down!\r\n");
      return (1==2);
      }
    }
//  debug_position = 5690;
  return (1==1);
  }

int Thing::EnterHalf(Thing *in, char dir)  {
//  if(!((Cell *)location[0])->ClaimedHalf(in) ||
//        ((inside[0] != NULL) && (inside[1] != NULL) &&
//        (inside[0] != in) && (inside[1] != in)))  return (1==2);
  if(!((Cell *)location[0])->ClaimedHalf(in))  {
//    printf("Denied Half-Enter due to non-claimedness...\r\n");
    return (1==2);
    }
  if((inside[0] != NULL) && (inside[1] != NULL)
	&& (inside[0] != in) && (inside[1] != in))  {
//    printf("Denied Half-Enter due to non-emptyness...\r\n");
    return (1==2);
    }
  if((inside[0] == NULL) || (inside[0] == in))  inside[0] = in;
  else  inside[1] = in;
  in->altitude = Height();
  return (1==1);
  }

void Thing::Fall()  {
  if(flying) return;
  Changed[thingnum] = 1;
  Thing *tmpt = location[0];
  while(tmpt->inside[0] != NULL && tmpt->inside[0] != this &&
	tmpt->inside[0]->altitude < Height())
    tmpt = tmpt->inside[0];
  altitude = tmpt->Height();
  if(tmpt->type == THING_CELL && ((Cell*)tmpt)->WaterDepth() >= height)  {
    altitude = ((Cell*)tmpt)->WaterLevel()-height;
    //printf("water height set to %d\n", altitude);
    }
  if(inside[0] != NULL && inside[0] != this)  inside[0]->Fall();
  }

void Thing::Leave(Thing *in)  {
  if(inside[0] == in && inside[1] == in)  {
    inside[0] = in->inside[0]; 
    inside[1] = in->inside[1];
    in->inside[0] = NULL; 
    in->inside[1] = NULL;
    if(inside[0] != NULL && inside[0] != this)  inside[0]->Fall();
//    printf("Placing %p\r\n", inside[0]);
    }
  else if(inside[0] == inside[1] && inside[0] != NULL)  {
    inside[0]->Leave(in);
    }
  else  {
    if(inside[0] == in || inside[1] == in)
      Exit(1, "Exiting half-entered Cell!!!\n");
    else  Exit(1, "Exiting non-entered Cell!!!\n");
    }
  }

void Thing::LeaveHalf(Thing *in)  {
  if(inside[1] == in)  {
    inside[1] = in->inside[0];
    in->inside[1] = NULL;
    }
  else if(inside[0] == in)  {
    inside[0] = in->inside[1];
    in->inside[0] = NULL;
    }
  else if(inside[0] == inside[1] && inside[0] != NULL)  {
    inside[0]->LeaveHalf(in);
    }
  else {
    Exit(1, "Exiting non-entered thing!!!\n");
    }
  }

int Thing::RealDirectionTo(Thing *destt)  {
  if(destt == NULL)  return(-1);
  Cell *orig = (Cell *)Location(0);
  Cell *dest = (Cell *)destt->Location(0);
  if(dest == NULL || orig == NULL)  return(-1);
  dest = (Cell *)dest->Location(0);
  orig = (Cell *)orig->Location(0);
  if(dest == NULL || orig == NULL)  return(-1);
  int dirret, dx, dy, x, y, xd, yd, xneg, yneg;
  dx = dest->XCoord();
  dy = dest->YCoord();
  x = orig->XCoord();
  y = orig->YCoord();
  xd = dx - x;
  yd = dy - y;
  xneg = 0;		yneg = 0;
  if(xd < 0)  { xd=-xd; xneg = 1; }
  if(yd < 0)  { yd=-yd; yneg = 1; }
  if(xd <= 2*yd)  dirret = DIR_SE;
  else dirret = DIR_E;
  if((xneg) && (yneg))  dirret = 6+dirret;
  else if(xneg)  dirret = 6-dirret;
  else if(yneg)  dirret = 12-dirret;
  dirret %= 12;
  return dirret;
  }

int Thing::DirectionTo(Thing *destt)  {
  if(destt == NULL)  return(-1);
  Cell *orig = (Cell *)Location(0);
  Cell *dest = (Cell *)destt->Location(0);
  if(dest == NULL || orig == NULL)  return(-1);
  dest = (Cell *)dest->Location(0);
  orig = (Cell *)orig->Location(0);
  if(dest == NULL || orig == NULL)  return(-1);
  int dirret, dx, dy, x, y, xd, yd, xneg, yneg;
  dx = dest->XCoord();
  dy = dest->YCoord();
  x = orig->XCoord();
  y = orig->YCoord();
  xd = dx - x;
  yd = dy - y;
  xneg = 0;		yneg = 0;
  if(xd < 0)  { xd=-xd; xneg = 1; }
  if(yd < 0)  { yd=-yd; yneg = 1; }
  if(2*xd <= yd)  dirret = DIR_S;
  else if(xd <= 2*yd)  dirret = DIR_SE;
  else if(xd <= 6*yd)  dirret = DIR_ES;
  else dirret = DIR_E;
  if((xneg) && (yneg))  dirret = 6+dirret;
  else if(xneg)  dirret = 6-dirret;
  else if(yneg)  dirret = 12-dirret;
  dirret %= 12;
  return dirret;
  }

int Thing::DistanceTo(Thing *destt)  {
  if(destt == NULL)  return(-1);
  Cell *orig = (Cell *)Location(0);
  Cell *dest = (Cell *)destt->Location(0);
  if(dest == NULL || orig == NULL)  return(-1);
  dest = (Cell *)dest->Location(0);
  orig = (Cell *)orig->Location(0);
  if(dest == NULL || orig == NULL)  return(-1);
  int xd, yd;
  xd = dest->XCoord() - orig->XCoord();
  yd = dest->YCoord() - orig->YCoord();
  if(xd < 0)  xd = -xd;
  if(yd < 0)  yd = -yd;
  xd-=yd;
  if(xd < 0)  xd = 0;
  xd/=2;
  return(xd+yd);
  }

void Thing::FlipToCloserThing(IntList &in)  {
  int ctr, dist, tmpd, best = 0;
  dist = DistanceTo(list[in[0]]);
  for(ctr=1; ctr < in.Size(); ctr++)  {
    tmpd = DistanceTo(list[in[ctr]]);
    if(tmpd < dist)  {
      dist = tmpd;
      best = ctr;
      }
    }
  if(best != 0)  in.RotateToElement(best);
  }

void Thing::Heat(int temp, int max)  {
  if(Inside(0) != NULL)  Inside(0)->Heat(temp, max);
  if(type != THING_CELL && type != THING_STRUCT)  return;
  if(temp > 0 && (ffeul < 1 || ftemp > max))  return;
  if((ftemp > fresist) + (ftemp > 0) != 
	((ftemp+temp) > fresist) + ((ftemp+temp) > 0))
    Changed[thingnum] = 1;
  ftemp += temp;
  if(ftemp > max)  ftemp = max;
  if(ftemp < 0)  ftemp = 0;
  if(ftemp > ffeul)  ftemp = ffeul;
  }

