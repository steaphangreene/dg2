#include <user.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char **argv)  {
  if(argc < 3)  Exit(1, "USAGE: chcl <c1> <c2> <file> [<file>]...\n");
  if((!isdigit(argv[1][0])) || (!isdigit(argv[2][0])))
	Exit(1, "USAGE: chcl <c1> <c2> <file> [<file>]...\n");
  int c1=strtol(argv[1], NULL, 10);
  int c2=strtol(argv[2], NULL, 10);
  int ctr, ctrx, ctry;
  for(ctr=3; ctr<argc; ctr++)  {
    if(access(argv[ctr], R_OK))
	Exit(1, "Hey, \"%s\" isn't there!\n", argv[ctr]);
    Graphic tmpg(argv[ctr]);
    Palette pl;
    pl.GetBMPPalette(argv[ctr]);
    for(ctry=0; ctry<(int)tmpg.ysize; ctry++)  {
      for(ctrx=0; ctrx<(int)tmpg.xsize; ctrx++)  {
	if(tmpg.image[ctry][ctrx] == c1)
	  tmpg.image[ctry][ctrx] = c2;
	}
      }
    tmpg.SaveBMP(argv[ctr], pl);
    }
  return 0;
  }
