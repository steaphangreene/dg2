#include "cell.h"
#include "thing.h"

#define STRUCT_BRIDGE		0
#define STRUCT_CART		1
#define STRUCT_HULL		2
#define STRUCT_MACHINE		3
#define STRUCT_RAMP		4
#define STRUCT_STRUCTURE	5
#define STRUCT_TOWER		6
#define STRUCT_WALL		7
#define STRUCT_MAXMAT		8

#define STRUCT_AQUADUCT		8
#define STRUCT_DITCH		9
#define STRUCT_FARMLAND		10
#define STRUCT_PATH		11
#define STRUCT_MAX		12

#define MATERIAL_ROCK		0
#define MATERIAL_WOOD		1
#define MATERIAL_COPPER		2
#define MATERIAL_TIN		3
#define MATERIAL_BRONZE		4
#define MATERIAL_IRON		5
#define MATERIAL_STEEL		6
#define MATERIAL_ADM		7
#define MATERIAL_MAXBUILD	8

#define MATERIAL_GOLD		8
#define MATERIAL_COAL		9
#define MATERIAL_OIL		10
#define MATERIAL_MAX		11

class Structure : public Thing {
  public:
  Structure(int Type, int Material);
  virtual ~Structure();
  int Place(Cell *);
  int Damage(int, int);
  virtual void Select(); 
  virtual void Deselect();
  virtual void ToggleSelect();
  static void InitGraphics();
  int StructType()  { return struct_type; };

  private:
  static Graphic *stgr[STRUCT_MAX][MATERIAL_MAXBUILD][3][3];
  static int graphicsinitialized;
  virtual void ReScaleme(); 
  virtual void ReAlignme(int, int); 
  virtual void tickme();
  virtual void updateme();
  char struct_type;
  char material;
  char hit;
  char discovered;
  Sprite image;
  };
