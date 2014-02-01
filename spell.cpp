#include "../user/engine.h"
#include "spell.h"
#include "map.h"
#include "cell.h"
#include "creature.h"
#include "gmode.h"
#include "ammo.h"
#include "struct.h"
#include <string.h>

#define gm gmode[cmode]
#define Image pics[terrain & (discovered*255)][gmode[cmode].xstep/32][visible]

extern GMode gmode[10];
extern char cmode;
extern Screen *screen;
extern Map *curmap;
extern Panel mainp;
extern Player *p1;

char spact[64][4] = {
//Spells
/* 0:ARROWS */		{ DOING_SELECTECREATURES, -1, -1, -1 },
	{ -1, -1, -1, -1 },     { -1, -1, -1, -1 },
/* 3:DISINTIGRATE */	{ DOING_SELECTEOBJS, -1, -1, -1 },
	{ -1, -1, -1, -1 },
/* 5:FIREBALL */	{ DOING_SELECTCELL, -1, -1, -1 },
/* 6:GLOBE_OF_SEEING */	{ DOING_SELECTCELL, -1, -1, -1 },
        { -1, -1, -1, -1 },
/* 8:INVISIBILITY */	{ DOING_SELECTFCREATURES, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
/* 14:OPEN_PORTAL */	{ DOING_SELECTCELL, DOING_SELECTCELL, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
/* 17:REOPEN_PORTAL */	{ DOING_SELECTCELL, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
/* 21:VISION */		{ DOING_SELECTFCREATURES, -1, -1, -1 },
/* 22:WALL_OF_STONE */	{ DOING_SELECTLCELLS, -1, -1, -1 },
        { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },

// Prayers
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },
/* 39:HEAL */		{ DOING_SELECTFCREATURES, -1, -1, -1 },
        { -1, -1, -1, -1 },     
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 },     { -1, -1, -1, -1 },
        { -1, -1, -1, -1 },     { -1, -1, -1, -1 } };

char sprng[64] = {
//Spells
/* 0:ARROWS */		0,
	-1, -1,
/* 3:DISINTIGRATE */	0,
	-1,
/* 5:FIREBALL */	0,
/* 6:GLOBE_OF_SEEING */	-1,
	-1,
/* 8:INVISIBILITY */	0,
	-1, -1, -1, -1, -1,
/* 14:OPEN_PORTAL */	0,
        -1, -1,
/* 17:REOPEN_PORTAL */	0,
	-1, -1, -1,
/* 21:VISION */		0,
/* 22:WALL_OF_STONE */	0,
	-1, -1, -1, -1, -1, -1, -1, -1, -1,

// Prayers
        -1, -1, -1, -1, -1, -1, -1,
/* 39:HEAL */           1,
        -1, -1, -1, -1, -1, -1, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


Spell::Spell(Thing *cstr, int nm, const IntList &pms)  {
  Debug("Spell::Spell() Begin");
//  printf("Creating spell #%d\r\n", nm);
  gsize = gm.xstep;
  caster = cstr;
  images = NULL;
  items.Clear();
  status = 0;
  memory = 0;
  params = pms;
  name = nm;
  type = THING_SPELL;
  Waiting[thingnum] = 1;
  switch(name)  {
    case(SPELL_WALL_OF_STONE):  {
      int ctr;
      for(ctr=0; ctr<params.Size(); ctr++)  {
	Cell *targ = (Cell *)list[params[ctr]];
	if(targ->inside[0] == NULL && targ->inside[1] == NULL)  {
	  Structure *wl = new Structure(STRUCT_WALL, MATERIAL_ROCK);
	  if(!(wl->Place(targ)))  {
	    delete wl;
	    printf("Can't build wall there!\r\n");
	    }
	  }
	}
      } break;
    case(PRAYER_HEAL):  {
      Debug("Spell::Spell(): case(PRAYER_HEAL): Start");
      ((Creature*)list[params[0]])->hit = 100;
      ((Creature*)list[params[0]])->fatigue = 100000;
      ((Creature*)list[params[0]])->UpdateCondition();
      Debug("Spell::Spell(): case(PRAYER_HEAL): Finish");
      } break;
    case(SPELL_VISION):  {
      images = new Sprite *[1];
      images[0] = new Sprite;
      images[0]->DisableCollisions();
      images[0]->SetPanel(mainp);
      location[0] = (Cell*)list[params[0]];
      location[0]->spell += thingnum;
      Changed[thingnum] = 1;
      ((Creature *)location[0])->UndoVision();
      ((Creature *)location[0])->GetOwnStats();
      if(((Creature *)location[0])->mvision == 0)  {
	((Creature *)location[0])->stats->vision <<= 1;
	((Creature *)location[0])->vision <<= 1;
	((Creature *)location[0])->stats->fieldofvision += 2;
	}
      ((Creature *)location[0])->mvision++;
      ((Creature *)location[0])->DoVision();
      } break;
    case(SPELL_INVISIBILITY):  {
      images = new Sprite *[1];
      images[0] = new Sprite;
      images[0]->DisableCollisions();
      images[0]->SetPanel(mainp);
      location[0] = (Cell*)list[params[0]];
      location[0]->spell += thingnum;
      Changed[thingnum] = 1;
      } break;
    case(SPELL_FIREBALL):  {
      images = new Sprite *[1];
      images[0] = new Sprite;
      images[0]->DisableCollisions();
      images[0]->SetPanel(mainp);
      location[0] = list[params[0]]->Location(0)->Location(0);
      location[0]->spell += thingnum;
      Changed[thingnum] = 0;
      Thing *tmpt;
      tmpt = new Ammo(AMMO_SPARK, (Cell *)caster->Location(0)->Location(0),
				(Cell *)location[0], 32);
      dodad = tmpt->thingnum;
      } break; 
    case(SPELL_GLOBE_OF_SEEING):  {
      images = new Sprite *[1];
      images[0] = new Sprite;
      images[0]->DisableCollisions();
      images[0]->SetPanel(mainp);
      location[0] = list[params[0]]->Location(0)->Location(0);
      location[0]->spell += thingnum;
      {
	Creature *tmpc;
	tmpc = new Creature(CREATURE_GUY, p1);
	location[1] = (Cell*)tmpc;
	tmpc->facing = 0;
	tmpc->location[0] = location[0];
	tmpc->GetOwnStats();
	tmpc->stats->fieldofvision = 6;
	tmpc->DoVision();
//	tmpc->UndoVision();
	tmpc->location[0] = NULL;
	}
      Changed[thingnum] = 1;
      } break; 
    case(SPELL_ARROWS):  {
      int ctr, na = 10;
      Thing *tmpt;
      for(ctr=0; ctr<na; ctr++)  {
	tmpt = new Ammo(AMMO_ARROW, caster, list[params[ctr%(params.Size())]],
		32, 100, 50, 50);
//        tmpt = new Ammo(AMMO_ARROW, caster->location[0]->location[0],
//		(Cell*)list[params[ctr%(params.Size())]]->
//		Location(0)->Location(0), 32);
	items += tmpt->thingnum;
	}
      } break;
    case(SPELL_DISINTIGRATE):  {
      Thing *tmpt;
      tmpt = new Ammo(AMMO_RAY, caster->location[0]->location[0],
	(Cell*)list[params[0]]->Location(0)->Location(0), 64);
      dodad = tmpt->thingnum;
      Waiting[thingnum] = 1;
      } break;
    case(SPELL_REOPEN_PORTAL):  {
      Cell *tmpc = ((Cell*)list[params[0]]);
      tmpc = tmpc->lastport;
      if(tmpc == NULL)  params += -1;
      else params += tmpc->thingnum;
      name = SPELL_OPEN_PORTAL;
      }
    case(SPELL_OPEN_PORTAL):  {
      if(params.Size()!=2)
	U2_Exit(0, "Improperly Cast Portal Spell (!= 2 args)!\r\n");
      if(list[params[0]]->Type() != THING_CELL)
	U2_Exit(0, "Improperly Cast Portal Spell (Origin not Cell)!\r\n");
      if(params[1] != -1 && list[params[1]]->Type() != THING_CELL)
	U2_Exit(0, "Improperly Cast Portal Spell (Destination not Cell)!\r\n");
      location[0] = (Cell*)list[params[0]];
      if(params[1] != -1)  location[1] = (Cell*)list[params[1]];
      else location[1] = NULL;

      location[0]->spell += thingnum;

      if(location[1] != NULL)  {
	((Cell *)location[0])->lastport = location[1];
	location[1]->spell += thingnum;
	((Cell *)location[1])->lastport = location[0];
	}
      altitude = 2000;
      params -= params[0];
      params -= params[1];
      images = new Sprite *[3];
      images[0] = new Sprite;
      images[1] = new Sprite;
      images[2] = new Sprite;
      images[0]->DisableCollisions();
      images[1]->DisableCollisions();
      images[2]->DisableCollisions();
      images[0]->SetPanel(mainp);
      images[1]->SetPanel(mainp);
      images[2]->SetPanel(mainp);
      Changed[thingnum] = 1;
      } break;
    default:  {
      U2_Exit(0, "Unknown Spell (#%d)!\r\n", name);
      } break;
    }
  ReScaleme();
  Debug("Spell::Spell() End");
  }

Spell::~Spell()  {
  Remove();
  if(caster != NULL)  {
    if(((Creature *)caster)->sustained_spell == thingnum)  {
      ((Creature *)caster)->sustained_spell -= thingnum;
      ((Creature *)caster)->Owner()->RefigureSpellcasters();
      }
    caster = NULL;
    }
  switch(name)  {
    case(SPELL_VISION):  {
      images[0]->Erase();
      delete images[0];
      delete images;
      Changed[location[0]->Number()] = 1;
      location[0]->spell -= thingnum;
      ((Creature *)location[0])->mvision--;
      if(((Creature *)location[0])->mvision == 0)  {
	((Creature *)location[0])->vision >>= 1;
	((Creature *)location[0])->stats->fieldofvision -= 2;
	((Creature *)location[0])->stats->vision >>= 1;
	((Creature *)location[0])->DoVision();
	((Creature *)location[0])->mvision++;
	((Creature *)location[0])->vision <<= 1;
	((Creature *)location[0])->stats->fieldofvision += 2;
	((Creature *)location[0])->UndoVision();
	((Creature *)location[0])->mvision--;
	((Creature *)location[0])->vision >>= 1;
	((Creature *)location[0])->stats->fieldofvision -= 2;
	}
      location[0] = NULL;
      } break;
    case(SPELL_ARROWS):  {
      int ctr;
      for(ctr=0; ctr<items.Size(); ctr++)  {
	if((Ammo*)list[items[ctr]] != NULL)  delete ((Ammo*)list[items[ctr]]);
	}
      } break;
    case(SPELL_DISINTIGRATE):  {
      if(list[dodad] != NULL)  delete ((Ammo*)list[dodad]);
      } break;
    case(SPELL_FIREBALL):  {
      delete images[0];
      delete images;
      location[0]->spell -= thingnum;
      location[0] = NULL;
      }break;
    case(SPELL_GLOBE_OF_SEEING):  {
      location[0]->spell -= thingnum;
      delete images[0];
      delete images;
      {
	Creature *tmpc;
	tmpc = (Creature *)location[1];
	tmpc->UndoVision();
	tmpc->location[0] = NULL;
	delete tmpc;
	}
      location[0] = NULL;
      }break;
    case(SPELL_INVISIBILITY):  {
      images[0]->Erase();
      delete images[0];
      delete images;
      Changed[location[0]->Number()] = 1;
      location[0]->spell -= thingnum;
      location[0] = NULL;
      }break;
    case(SPELL_OPEN_PORTAL):  {
      images[0]->Erase();
      images[1]->Erase();
      images[2]->Erase();
      delete images[2];
      delete images[1];
      delete images[0];
      delete images;
      location[0]->spell -= thingnum;
      if(location[1] != NULL)  location[1]->spell -= thingnum;
      delete inside;
      }break;
    }
  }

void Spell::Select()  {
  }

void Spell::ToggleSelect()  {
  }

void Spell::updateme()  {
  Debug("Spell::updateme() Begin");
  if(gsize != gm.xstep)  {
    gsize = gm.xstep;
    ReScaleme();
    }
  switch(name)  {
    case(SPELL_VISION):  {
      if(((Creature *)location[0])->Owner() == p1)  {
	int x, y;
	x = ((Creature *)location[0])->image.XPos();
	y = ((Creature *)location[0])->image.YPos();
	x -= gm.xstep>>1;
	y -= gm.ystep>>1;
	images[0]->Move(x, y);
	}
      } break;
    case(SPELL_GLOBE_OF_SEEING):  {
      if(location[0] == NULL)  return;
      if(location[0]->mviewers || (((Creature *)caster)->Owner() == p1))
	images[0]->Move(Location(0)->XPos(), Location(0)->YPos());
      else  images[0]->Erase();
      } break;
    case(SPELL_FIREBALL):  {
      if(status == 1)  {
	int x, y, ctr;
	x = ((Cell *)location[0])->XPos();
	y = ((Cell *)location[0])->YPos();
	images[0]->Move(x, y);

	Cell *tmpc;
	location[0]->Heat(200, 200);
	((Cell *)location[0])->StrikeAllIn(this, 200, 500, 500);
	for(ctr=0; ctr<12; ctr+=2)  {
	  tmpc = location[0]->Next(ctr);
	  if(tmpc != NULL)  {
	    tmpc->Heat(200, 200);
	    tmpc->StrikeAllIn(this, 200, 250, 450);
	    tmpc = tmpc->Next(ctr);
	    if(tmpc != NULL)  {
	      tmpc->Heat(200, 200);
	      tmpc->StrikeAllIn(this, 200, 150, 400);
	      tmpc = tmpc->Next(ctr+4);
	      if(tmpc != NULL)  {
	        tmpc->Heat(200, 200);
		tmpc->StrikeAllIn(this, 200, 150, 400);
		}
	      }
	    }
	  }

	Changed[thingnum] = 1;
	status++;
	}
      } break;
    case(SPELL_INVISIBILITY):  {
      if(((Creature *)location[0])->Owner() != p1)  {
	((Creature *)location[0])->image.Erase();
	Changed[location[0]->Number()] = 0;
	}
      else  {
	int x, y;
	x = ((Creature *)location[0])->image.XPos();
	y = ((Creature *)location[0])->image.YPos();
	images[0]->Move(x, y);
	}
      } break;
    case(SPELL_OPEN_PORTAL):  {
      if(location[0] == NULL)  return;

      if(location[0]->mviewers)  {
	images[1]->Move(Location(0)->XPos(), Location(0)->YPos());
	images[0]->Erase();
	}
      else  {
	if(location[0]->visible)  {
	  images[0]->Move(Location(0)->XPos(), Location(0)->YPos());
	  }
	else images[0]->Erase();
	images[1]->Erase();
	}

      if(location[1] != NULL)  {
	if(location[1]->mviewers)
	  images[2]->Move(Location(1)->XPos(), Location(1)->YPos());
        else images[2]->Erase();
	}
      } break;
    }
  Debug("Spell::updateme() End");
  }

void Spell::tickme()  {
  int ctr;
  Waiting[thingnum] = 1;
  switch(name)  {
    case(SPELL_ARROWS):  {
      int ctr, done = 1;
      for(ctr=0; ctr<items.Size(); ctr++)  {
	if(list[items[ctr]] == NULL)  {
//	  ((Creature*)list[params[ctr%(params.Size())]])->
//		Strike((Creature *)this, 100, 50, 50);
	  }
	else done = 0;
	}
      if(done)  {
	delete this;
	return;
	}
      } break;
    case(SPELL_DISINTIGRATE):  {
      if(list[dodad] == NULL)  {
	delete list[params[0]];
	delete this;
	return;
	}
      } break;
    case(SPELL_WALL_OF_STONE):  {
      delete this;
      return;
      } break;
    case(PRAYER_HEAL):  {
      delete this;
      return;
      } break;
    case(SPELL_FIREBALL):  {
      if(status < 1 && list[dodad] == NULL)  {
	status=1;
	Changed[thingnum] = 1;
	}
      else if(status >= 5)  {
	delete this;
	return;
	}
      else if(status > 1)  status++;
      Waiting[thingnum] = 1;
      } break;
    case(SPELL_GLOBE_OF_SEEING):
    case(SPELL_VISION):
    case(SPELL_INVISIBILITY):  {
      Waiting[thingnum] = 0;
      } break;
    case(SPELL_OPEN_PORTAL):  {
      Cell *orig = location[0], *dest = location[1];
      if(dest == NULL)  {
	if(orig->Inside(0) == orig->Inside(1) && orig->Inside(0) != orig
	    && orig->Inside(0) != NULL)  {
	  if(((Creature *)orig->Inside(0))->sustained_spell == thingnum)  {
	    ((Creature *)orig->Inside(0))->sustained_spell -= thingnum;
	    caster = NULL;
	    delete ((Creature *)orig->Inside(0));
	    delete this;
	    return;
	    }
	  delete ((Creature *)orig->Inside(0));
	  }
	Waiting[thingnum] = 1;
	return;
	}
      if(orig->Inside(0) == orig->Inside(1) && orig->Inside(0) != orig
	    && orig->Inside(0) != NULL && dest->Inside(0) == dest->Inside(1)
	    && dest->Inside(0) == NULL && dest->Owner(0) == dest->Owner(1)
	    && dest->Owner(0) == NULL)  {
	Creature *movee = (Creature*)orig->Inside(0);
        if(movee == (Creature *)this)  U2_Exit(1, "Porting Portal!\r\n");
        else if(movee == NULL)  U2_Exit(1, "Porting NULL!\r\n");
	movee->UndoVision();
	orig->Leave(movee);
	if(!dest->Claim(movee, dest->Height(), 0, 1000, 1000))  {
	  U2_Exit(1, "Portal Object Failed to Claim!\r\n");
	  }
	if(!dest->Enter(movee, dest->Height(), 0, 1000, 1000))  {
	  U2_Exit(1, "Portal Object Failed to Enter!\r\n");
	  }
	dest->UnClaim(movee);
	movee->location[0] = dest;
	movee->location[1] = NULL;
	movee->Deselect();
	movee->GoalDone();
	movee->dirf = -1;
	movee->dirt = -1;
	movee->distt = -1;
	movee->DoVision();
	Changed[movee->Number()] = 1;
	for(ctr = memory+12; ctr > memory; ctr-=2)  {
	  if(dest->Next(ctr) != NULL && dest->Next(ctr)->Inside(0) == NULL)  {
	    location[1]->spell -= thingnum;
	    location[1] = dest->Next(ctr);
	    location[1]->spell += thingnum;
	    altitude += 1000;
	    memory = (ctr&62)+4;
	    memory %= 12;
	    Changed[thingnum] = 1;
	    break;
	    }
	  }
	}
      }break;
    }
  }

void Spell::ReScaleme()  {
  switch(name)  {
    case(SPELL_WALL_OF_STONE):
    case(PRAYER_HEAL):  {
      }break;
    case(SPELL_VISION):  {
      Graphic pg("graphics/spells/eye.bmp");
      pg.FindTrueCenter();
      if(gsize != 64) pg = pg.Scaled(gsize, gsize);
      pg.Trim();
      images[0]->SetImage(pg);
      }break;
    case(SPELL_GLOBE_OF_SEEING):  {
      Graphic pg("graphics/spells/globe.bmp");
      pg.FindTrueCenter();
      if(gsize != 64) pg = pg.Scaled(gsize, gsize);
      pg.Trim();
      images[0]->SetImage(pg);
      }break;
    case(SPELL_FIREBALL):  {
      Graphic pg("graphics/spells/fireball.bmp");
      pg.FindTrueCenter();
      if(gsize != 64)
	pg = pg.Scaled((pg.xsize * gsize)/64, (pg.xsize * gsize)/64);
      pg.Trim();
      images[0]->SetImage(pg);
      }break;
    case(SPELL_INVISIBILITY):  {
      Graphic pg("graphics/spells/cloud.bmp");
      pg.FindTrueCenter();
      if(gsize != 64) pg = pg.Scaled(gsize, gsize);
      pg.Trim();
      images[0]->SetImage(pg);
      }break;
    case(SPELL_OPEN_PORTAL):  {
      Graphic pg1("graphics/spells/portal.bmp");
      Graphic pg2("graphics/spells/port_in.bmp");
      Graphic pg3("graphics/spells/port_out.bmp");
      pg1.FindTrueCenter();
      pg2.FindTrueCenter();
      pg3.FindTrueCenter();
      if(gsize != 64)   {
	pg1 = pg1.Scaled(gsize, gsize);
	pg2 = pg2.Scaled(gsize, gsize);
	pg3 = pg3.Scaled(gsize, gsize);
	}
      pg1.Trim();
      pg2.Trim();
      pg3.Trim();
      images[0]->SetImage(pg1);
      images[1]->SetImage(pg2);
      images[2]->SetImage(pg3);
      }break;
    }
  Changed[thingnum] = 1;
  }

void Spell::ReAlignme(int x, int y)  {
  Changed[thingnum] = 1;
  }
