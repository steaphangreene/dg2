#include <user.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char **argv)  {
  if(argc < 2)  U2_Exit(1, "USAGE: chcl <palette> <file> [<file>]...\n");
  Palette pal(argv[1]);
  int ctr;
  for(ctr=2; ctr<argc; ctr++)  {
    if(access(argv[ctr], R_OK))
	U2_Exit(1, "Hey, \"%s\" isn't there!\n", argv[ctr]);
    Graphic tmpg(argv[ctr], pal);
    tmpg.SaveBMP(argv[ctr], pal);
    }
  return 0;
  }
