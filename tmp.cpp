#include "creature.h"
#include "cell.h"
#include "ammo.h"
#include "spell.h"
main()  {
  printf("Thing    = %d bytes\r\n", sizeof(Thing));
  printf("Cell     = %d bytes\r\n", sizeof(Cell));
  printf("Creature = %d bytes\r\n", sizeof(Creature));
  printf("Ammo     = %d bytes\r\n", sizeof(Ammo));
  printf("Spell    = %d bytes\r\n", sizeof(Spell));
  }
