#include <user.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char **argv)  {
  int ctr;
  for(ctr=1; ctr<argc; ctr++)  {
    if(access(argv[ctr], R_OK))
	U2_Exit(1, "Hey, \"%s\" isn't there!\n", argv[ctr]);
    Palette pl(argv[ctr]);
    Graphic tmpg(argv[ctr]);
    Graphic saveg(tmpg.RotatedCounterClock());
    saveg.SaveBMP(argv[ctr], pl);
    }
  return 0;
  }
