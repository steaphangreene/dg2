#include <stdlib.h>
#include <string.h>
#include <user/screen.h>
#include <user/resfile.h>
#include "creature.h"

int main(int argc, char **argv)  {
  if(argc != 2)  {
    printf("USAGE: bcr directory_name\n");
    exit(1);
    }
  char *dir = argv[1];

  Graphic *pics[3][FRAME_MAX];

  FILE *ckf; int fm;
  int ctr, ctr2, ctr3, strst = strlen(dir);
  char *buf = new char[strst+20];
  strcpy(buf, dir);

  strst++;
  buf[strst-1] = '/';
  buf[strst] = '\0';

  for(ctr=0; ctr<FRAME_MAX; ctr++)  pics[2][ctr] = NULL;

  strcpy(&buf[strst], "Down.bmp");
  pics[2][FRAME_DOWN] = new Graphic(buf);
  strcpy(&buf[strst], "Splat.bmp");
  pics[2][FRAME_SPLAT] = new Graphic(buf);

  for(ctr=0; ctr<32; ctr++)  {
    fm=FRAME_WALK+ctr;
    sprintf(&buf[strst], "walk%.2d.bmp%c", ctr, 0);
    ckf=fopen(buf, "r");
    if(ckf != NULL)  {
      fclose(ckf);
      pics[2][fm] = new Graphic(buf);
      }
    else  pics[2][fm] = NULL; 
    }
  for(ctr=0; ctr<32; ctr++)  {
    fm=FRAME_M0+ctr;
    sprintf(&buf[strst], "m1a%.2d.bmp%c", ctr, 0);
    ckf=fopen(buf, "r");
    if(ckf != NULL)  {
      fclose(ckf);
      pics[2][fm] = new Graphic(buf);
      }
    else  pics[2][fm] = NULL; 
    }
  for(ctr=0; ctr<32; ctr++)  {
    fm=FRAME_LIMP+ctr;
    sprintf(&buf[strst], "limp%.2d.bmp%c", ctr, 0);
    ckf=fopen(buf, "r");
    if(ckf != NULL)  {
      fclose(ckf);
      pics[2][fm] = new Graphic(buf);
      }
    else  pics[2][fm] = NULL; 
    }
  for(ctr=0; ctr<32; ctr++)  {
    fm=FRAME_CRAWL+ctr;
    sprintf(&buf[strst], "crawl%.2d.bmp%c", ctr, 0);
    ckf=fopen(buf, "r");
    if(ckf != NULL)  {
      fclose(ckf);
      pics[2][fm] = new Graphic(buf);
      }
    else  pics[2][fm] = NULL; 
    }

  printf("Got here!\n");
  for(ctr2=0; ctr2<FRAME_MAX; ctr2++)  {
    if(pics[2][ctr2] != NULL) {
      pics[1][ctr2] =
        new Graphic((pics[2][ctr2])->Scaled(
                (pics[2][ctr2])->xsize/2, 
                (pics[2][ctr2])->ysize/2));
      pics[0][ctr2] =
        new Graphic((pics[2][ctr2])->Scaled(
                (pics[2][ctr2])->xsize/4, 
                (pics[2][ctr2])->ysize/4));
      pics[0][ctr2]->FindTrueCenter();
      pics[0][ctr2]->Trim();
      pics[1][ctr2]->FindTrueCenter();
      pics[1][ctr2]->Trim();
      pics[2][ctr2]->FindTrueCenter();
      pics[2][ctr2]->Trim();

// Goes Here

      }
    else  {
      pics[0][ctr2] = NULL;
      pics[1][ctr2] = NULL;
      }
    }
  strst--;
  strcpy(&buf[strst], ".crf");

  char *ind = &buf[strst];
  while(ind != buf && ind[-1] != '/')  ind--;
  if(buf != ind)  strcpy(buf, ind);

  NewResFile crf(buf);
  for(ctr3=0; ctr3<3; ctr3++)  {
    for(ctr2=0; ctr2<FRAME_MAX; ctr2++)  {
      crf.Add(pics[ctr3][ctr2]);
      }
    }
  return 0;
  }

//    for(ctr=1; ctr<12; ctr++)  {
//      pics[0][ctr2][ctr] =
//        new Graphic((pics[0][ctr2][0])->
//	  Rotated(256-((ctr*256)/12)));
//      pics[1][ctr2][ctr] =
//        new Graphic((pics[1][ctr2][0])->
//	  Rotated(256-((ctr*256)/12)));
//      pics[2][ctr2][ctr] =
//        new Graphic((pics[2][ctr2][0])->
//	  Rotated(256-((ctr*256)/12)));
//      pics[0][ctr2][ctr]->Trim();
//      pics[1][ctr2][ctr]->Trim();
//      pics[2][ctr2][ctr]->Trim();
//      }

