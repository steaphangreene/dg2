#include <user.h>

int main(int argc, char **argv)  {
  Screen screen(1024, 768);
  Keyboard key;
  screen.SetPalette("graphics/creature/guy/walkl00.bmp");
  Graphic *sw[4];
  sw[0] = new Graphic("graphics/creature/guy/sword/base.bmp");
  sw[1] = new Graphic("graphics/creature/guy/sword/ready00.bmp");
  sw[2] = new Graphic("graphics/creature/guy/sword/ready01.bmp");
  sw[3] = new Graphic("graphics/creature/guy/sword/ready02.bmp");
  sw[4] = NULL;
  Graphic *gg[128];
  gg[0] = new Graphic("graphics/creature/guy/walkl00.bmp");
  gg[1] = new Graphic("graphics/creature/guy/walkl01.bmp");
  gg[2] = new Graphic("graphics/creature/guy/walkl02.bmp");
  gg[3] = new Graphic("graphics/creature/guy/walkl03.bmp");
  gg[4] = new Graphic("graphics/creature/guy/walkl04.bmp");
  gg[5] = new Graphic("graphics/creature/guy/walkl05.bmp");
  gg[6] = new Graphic("graphics/creature/guy/walkl06.bmp");
  gg[7] = new Graphic("graphics/creature/guy/walkl07.bmp");
  gg[8] = new Graphic("graphics/creature/guy/walkl08.bmp");
  gg[9] = new Graphic("graphics/creature/guy/walkl09.bmp");
  gg[10] = new Graphic("graphics/creature/guy/walkl10.bmp");
  gg[11] = new Graphic("graphics/creature/guy/walkl11.bmp");
  gg[12] = new Graphic("graphics/creature/guy/walkl12.bmp");
  gg[13] = new Graphic("graphics/creature/guy/walkl13.bmp");
  gg[14] = new Graphic("graphics/creature/guy/walkl14.bmp");
  gg[15] = new Graphic("graphics/creature/guy/walkl15.bmp");
  gg[16] = new Graphic("graphics/creature/guy/walkl16.bmp");
  gg[17] = new Graphic("graphics/creature/guy/walkl17.bmp");
  gg[18] = new Graphic("graphics/creature/guy/walkl18.bmp");
  gg[19] = new Graphic("graphics/creature/guy/walkl19.bmp");
  gg[20] = new Graphic("graphics/creature/guy/walkl20.bmp");
  gg[21] = new Graphic("graphics/creature/guy/walkl21.bmp");
  gg[22] = new Graphic("graphics/creature/guy/walkl22.bmp");
  gg[23] = new Graphic("graphics/creature/guy/walkl23.bmp");
  gg[24] = new Graphic("graphics/creature/guy/walkl24.bmp");
  gg[25] = new Graphic("graphics/creature/guy/walkl25.bmp");
  gg[26] = NULL;
  gg[32] = new Graphic("graphics/creature/guy/walkr00.bmp");
  gg[33] = new Graphic("graphics/creature/guy/walkr01.bmp");
  gg[34] = new Graphic("graphics/creature/guy/walkr02.bmp");
  gg[35] = new Graphic("graphics/creature/guy/walkr03.bmp");
  gg[36] = new Graphic("graphics/creature/guy/walkr04.bmp");
  gg[37] = new Graphic("graphics/creature/guy/walkr05.bmp");
  gg[38] = new Graphic("graphics/creature/guy/walkr06.bmp");
  gg[39] = new Graphic("graphics/creature/guy/walkr07.bmp");
  gg[40] = new Graphic("graphics/creature/guy/walkr08.bmp");
  gg[41] = new Graphic("graphics/creature/guy/walkr09.bmp");
  gg[42] = new Graphic("graphics/creature/guy/walkr10.bmp");
  gg[43] = new Graphic("graphics/creature/guy/walkr11.bmp");
  gg[44] = new Graphic("graphics/creature/guy/walkr12.bmp");
  gg[45] = new Graphic("graphics/creature/guy/walkr13.bmp");
  gg[46] = new Graphic("graphics/creature/guy/walkr14.bmp");
  gg[47] = new Graphic("graphics/creature/guy/walkr15.bmp");
  gg[48] = new Graphic("graphics/creature/guy/walkr16.bmp");
  gg[49] = new Graphic("graphics/creature/guy/walkr17.bmp");
  gg[50] = new Graphic("graphics/creature/guy/walkr18.bmp");
  gg[51] = new Graphic("graphics/creature/guy/walkr19.bmp");
  gg[52] = new Graphic("graphics/creature/guy/walkr20.bmp");
  gg[53] = new Graphic("graphics/creature/guy/walkr21.bmp");
  gg[54] = new Graphic("graphics/creature/guy/walkr22.bmp");
  gg[55] = new Graphic("graphics/creature/guy/walkr23.bmp");
  gg[56] = new Graphic("graphics/creature/guy/walkr24.bmp");
  gg[57] = new Graphic("graphics/creature/guy/walkr25.bmp");
  gg[58] = NULL;
  gg[64] = new Graphic("graphics/creature/guy/stance00.bmp");
  gg[65] = new Graphic("graphics/creature/guy/stance01.bmp");
  gg[66] = new Graphic("graphics/creature/guy/stance02.bmp");
  gg[67] = NULL;
  int ctr, xpos=100;
  for(ctr=0; gg[ctr]!=NULL; ctr++) gg[ctr]->FindTrueCenter();
  for(ctr=32; gg[ctr]!=NULL; ctr++) gg[ctr]->FindTrueCenter();
  for(ctr=64; gg[ctr]!=NULL; ctr++) gg[ctr]->FindTrueCenter();
  for(ctr=0; sw[ctr]!=NULL; ctr++) sw[ctr]->FindTrueCenter();
  Sprite g, s;
  g.UseImage(gg[0]);
  g.SetPriority(1000);
  g.Move(100, 100);
  s.UseImage(sw[0]);
  s.SetPriority(1001);
  s.Move(100, 100);
  screen.Show();
  screen.Refresh();
  screen.SetFrameRate(80);
  while(!key.IsPressed(KEY_SPACE));
  for(ctr=0; gg[ctr]!= NULL; ++ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move(((ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  --ctr;
  for(; ctr>=0; --ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move(((ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  for(ctr=33; gg[ctr]!= NULL; ++ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move((-(ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  --ctr;
  for(; ctr>=33; --ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move((-(ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  for(ctr=0; gg[ctr]!= NULL; ++ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move(((ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  --ctr;
  for(; ctr>=0; --ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move(((ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  for(ctr=33; gg[ctr]!= NULL; ++ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move((-(ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  --ctr;
  for(; ctr>=33; --ctr)  {
    g.UseImage(gg[ctr]);
    g.Move(xpos, 100);
    s.Move((-(ctr%32)/2)+xpos++, 100);
    screen.Refresh();
    }
  screen.SetFrameRate(10);
  for(ctr=64; gg[ctr]!=NULL; ++ctr);
  --ctr;
  for(; ctr>=64; --ctr)  {
    g.UseImage(gg[ctr]);
    s.UseImage(sw[(ctr+1)-64]);
    s.SetPriority(999);
    g.Move(xpos, 100);
    s.Move(xpos, 100);
    screen.Refresh();
    }
  while(!key.IsPressed(KEY_ESC));
  }
