#include <user.h>

int main(int argc, char **argv)  {
  U2_Init(argc, argv);
  Screen sc(320, 200, 8);
  Keyboard key;
  U2_Exit(0);
  return 0;
  }
