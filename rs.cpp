#include <user.h>

int main(int argc, char **argv)  {
  InitUserEngine(argc, argv);
  Screen sc(320, 200, 8);
  Keyboard key;
  Exit(0);
  return 0;
  }
