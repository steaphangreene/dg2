#ifndef	INSOMNIA_MAP_H
#define	INSOMNIA_MAP_H
#include "thing.h"
#include "cell.h"

#define TILE_XSIZE	256
#define TILE_YSIZE	240

class Map {
  public:
  Map(char *);
  ~Map();
  void update();
  void ReScale();
  void ReAlign(int, int);
  Cell *CellAt(int, int);
  int XSize()  { return MAP_XSIZE; };
  int YSize()  { return MAP_YSIZE; };
  void Screen2Cell(int &, int &);
  void Mini2Cell(int &, int &);
  void Draw();
  void Draw(int, int, int, int);

  private:
  Cell ***cells;
  int MAP_XSIZE, MAP_YSIZE;
  };

#endif
