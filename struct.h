#include "cell.h"
#include "thing.h"
#include "material.h"

#define STRUCT_FARMLAND		0
#define STRUCT_IRRIGATION_DITCH	1
#define STRUCT_PATH		2
#define STRUCT_MACHINE		3 // synonyms
#define STRUCT_SEIGE_ENGINE	3 // synonyms
#define STRUCT_MAXCOMP		4

#define STRUCT_AQUADUCT		4
#define STRUCT_BRIDGE		5
#define STRUCT_CART		6
#define STRUCT_DRAWBRIDGE	7
#define STRUCT_ENCLOSURE	8
#define STRUCT_GATE		9
#define STRUCT_HULL		10
#define STRUCT_RAMP		11
#define STRUCT_TOWER		12
#define STRUCT_WALL		13
#define STRUCT_MAX		14
const int struct_qty[STRUCT_MAX] = { -1, -1, -1, -1, 10, 100, 200, 100,
	200, 400, 1000, 500, 1000, 1000 };

#define NEIGHBOR_MAX		64

class Structure : public Thing {
  public:
  Structure(int Type, int Material);
  virtual ~Structure();
  int Place(Cell *);
  void Damage(int, int);
  virtual void Select(); 
  virtual void Deselect();
  virtual void ToggleSelect();
  static void InitGraphics();
  void FigureNeighbors();
  void InitNeighbors();
  int StructType()  { return struct_type; };
  int HaveMaterial(int type);
  int AddMaterials(int type, int ammt);
  int TakeMaterials(int type, int ammt);
  int IsFinished() { return finished; };

  private:
  static Graphic *stgr[STRUCT_MAX][MATERIAL_MAXBUILD][3][3][NEIGHBOR_MAX];
  static int graphicsinitialized;
  virtual void ReScaleme(); 
  virtual void ReAlignme(int, int); 
  virtual void tickme();
  virtual void updateme();
  char struct_type;
  char material;
  char hit;
  char discovered;
  char finished;
  char neighbors;
  Sprite image;
  int contains[MATERIAL_MAX];
  };
