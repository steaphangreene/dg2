#include <user.h>
#include <unistd.h>

int main(int argc, char **argv)  {
  Graphic tm("team.bmp");
  Palette pl("base.pal");
  int tc1_1 = tm.image[0][0];
  int tc2_1 = tm.image[1][0];
  int tc1_2 = tm.image[0][1];
  int tc2_2 = tm.image[1][1];
  int ctr, ctrx, ctry;
  for(ctr=1; ctr<argc; ctr++)  {
    if(access(argv[ctr], R_OK))
	Exit(1, "Hey, \"%s\" isn't there!\n", argv[ctr]);
    Graphic tmpg(argv[ctr]);
    for(ctry=0; ctry<tmpg.ysize; ctry++)  {
      for(ctrx=0; ctrx<tmpg.xsize; ctrx++)  {
	if(tmpg.image[ctry][ctrx] == tc1_1)
	  tmpg.image[ctry][ctrx] = tc2_1;
	else if(tmpg.image[ctry][ctrx] == tc1_2)
	  tmpg.image[ctry][ctrx] = tc2_2;
	else if(tmpg.image[ctry][ctrx] == tc2_1)
	  tmpg.image[ctry][ctrx] = tc1_1;
	else if(tmpg.image[ctry][ctrx] == tc2_2)
	  tmpg.image[ctry][ctrx] = tc1_2;
	}
      }
    tmpg.YFlip();
    if(argv[ctr][3] == 'l') argv[ctr][3] = 'r';
    else if(argv[ctr][3] == 'r') argv[ctr][3] = 'l';
    else Exit(1, "\"%s\" is not a r/l run graphic\n", argv[ctr]);
    tmpg.SaveBMP(argv[ctr], pl);
    }
  }