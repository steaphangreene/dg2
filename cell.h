#ifndef	INSOMNIA_CELL_H
#define	INSOMNIA_CELL_H
#include "material.h"
#include "thing.h"

#define TERRAIN_VOID		0
#define TERRAIN_PATH		1
#define TERRAIN_DIRT		2
#define TERRAIN_GRASS		3
#define TERRAIN_BRUSH		4
#define TERRAIN_FOREST		5
#define TERRAIN_ROCK		6
#define TERRAIN_SAND		7
#define TERRAIN_ASHES		8
#define TERRAIN_MAXREAL		9

#define TERRAIN_LOWWATER	9
#define TERRAIN_WATER		10
#define TERRAIN_OCEAN		11
#define TERRAIN_MAX		12

#define TERRAIN_CINDERS		12


#define TERRAIN_BATTLEMENT	
#define TERRAIN_DRY		
#define TERRAIN_DEBRIS		
#define TERRAIN_STUMPS		
#define TERRAIN_PLANKS		
#define TERRAIN_METAL		
#define TERRAIN_MARBLE		

#define	DIR_E	0
#define	DIR_ES	1
#define	DIR_SE	2
#define	DIR_S	3
#define	DIR_SW	4
#define	DIR_WS	5
#define	DIR_W	6
#define	DIR_WN	7
#define	DIR_NW	8
#define	DIR_N	9
#define	DIR_NE	10
#define	DIR_EN	11

class Cell : public Thing {
  public:
  Cell(int, int, int);
  virtual ~Cell();
  virtual void Select();
  virtual void ToggleSelect();
  void Place(int, int);
  void PlaceNear(int, Cell*);
  int IsNextTo(Cell *);
  int XCoord()  {return xcoord;};
  int YCoord()  {return ycoord;};
  int XPos();
  int YPos();
  int GroundLevel() { return altitude; };
  int WaterLevel() { return waltitude; };
  int WaterDepth() { return waltitude-altitude; };
  Thing *Owner(int v1);
  Cell *Next(int dir);
  void DrawMyMini();
  void DrawMe();
  void HalfDisable();
  void Disable();
  void MSee()  {
	if(!mviewers)  Changed[thingnum] = 1;
	mviewers++;
	See();
	};
  void MCantSee()  {
	mviewers--;
	if(!mviewers)  Changed[thingnum] = 1;
	CantSee();
	};
  void See()  {
	if(!viewers)  Changed[thingnum] = 1;
	viewers++; discovered=1;
	visible = 1;
	};
  void CantSee()  {
	viewers--;
	if(!viewers)  {
		Changed[thingnum] = 1;
		visible = 0;
		}
	};
  int InView()  { return viewers; };
  int Terrain();
  void SetTerrain(int t) { terrain = t; Changed[thingnum] = 1; };
  int IsPath();

  int Claim(Thing *, int, int);
  int Claim(Thing *, int, int, int, int);
  int ClaimHalf(Thing *, char, int, int);
  int ClaimHalf(Thing *, char, int, int, int, int);
  void UnClaim(Thing *);
  void UnClaimHalf(Thing *);
  int Claimed(Thing *);
  int ClaimedHalf(Thing *);
  void StrikeAllIn(Thing *, int, int, int);
  int HaveMaterial(int type);
  int AddMaterials(int type, int ammt); 
  int TakeMaterials(int type, int ammt); 
  IntList CellsAtRange(int rng1, int rng2, int fac, int arcsz);

  protected:
  static void SetupTerrainGraphics();
  virtual void tickme();
  virtual void updateme();
  virtual void ReScaleme();
  virtual void ReAlignme(int, int);
  static int GraphicsInitialized;
  static Graphic *pics[TERRAIN_MAX][3][2][3];
  int terrain;
  void  LinkMe(int, Cell *);
  char visible, discovered, mapped;
  Cell *near[6];
  Cell *lastport;
  int xcoord, ycoord, viewers, mviewers, waltitude;
  Thing *dibs[2];
  friend class Map;
  friend class Spell;
  friend class Thing;
  int contains[MATERIAL_MAX];
  };

#endif
