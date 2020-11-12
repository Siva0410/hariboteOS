#include "bootpack.h"

void putblock8_8(char *vram, int vxsize, int pxsize, int pysize, int px0, int py0, char *buf, int bxsize){
  int x, y;
  for(y = 0; y < pysize; y++){
    for(x = 0; x < pxsize; x++){
      vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
    }
  }
  return ;
}
    
void init_screen(char *vram, int xsize, int ysize){
  boxfill8(vram, xsize, COL8_008484,  0,         0,          xsize -  1, ysize - 29);
  boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 28, xsize -  1, ysize - 28);
  boxfill8(vram, xsize, COL8_FFFFFF,  0,         ysize - 27, xsize -  1, ysize - 27);
  boxfill8(vram, xsize, COL8_C6C6C6,  0,         ysize - 26, xsize -  1, ysize -  1);

  boxfill8(vram, xsize, COL8_FFFFFF,  3,         ysize - 24, 59,         ysize - 24);
  boxfill8(vram, xsize, COL8_FFFFFF,  2,         ysize - 24,  2,         ysize -  4);
  boxfill8(vram, xsize, COL8_848484,  3,         ysize -  4, 59,         ysize -  4);
  boxfill8(vram, xsize, COL8_848484, 59,         ysize - 23, 59,         ysize -  5);
  boxfill8(vram, xsize, COL8_000000,  2,         ysize -  3, 59,         ysize -  3);
  boxfill8(vram, xsize, COL8_000000, 60,         ysize - 24, 60,         ysize -  3);

  boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 24, xsize -  4, ysize - 24);
  boxfill8(vram, xsize, COL8_848484, xsize - 47, ysize - 23, xsize - 47, ysize -  4);
  boxfill8(vram, xsize, COL8_FFFFFF, xsize - 47, ysize -  3, xsize -  4, ysize -  3);
  boxfill8(vram, xsize, COL8_FFFFFF, xsize -  3, ysize - 24, xsize -  3, ysize -  3);
  return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1){
  int x, y;
  for(y = y0; y <= y1; y++){
    for(x = x0; x <= x1; x++){
      vram[y * xsize + x] = c;
    }
  }
  return;
}

void init_palette(void){
  static unsigned char table_rgb[16 * 3] = {
    0x00, 0x00, 0x00, //0_black
    0xff, 0x00, 0x00, //1_red
    0x00, 0xff, 0x00, //2_green
    0xff, 0xff, 0x00, //3_yellow
    0x00, 0x00, 0xff, //4_blue
    0xff, 0x00, 0xff, //5_purple
    0x00, 0xff, 0xff, //6_skyblue
    0xff, 0xff, 0xff, //7_white
    0xc6, 0xc6, 0xc6, //8_gray
    0x84, 0x00, 0x00, //9_darkred
    0x00, 0x84, 0x00, //10_darkgreen
    0x84, 0x84, 0x00, //11_darkyellow
    0x00, 0x00, 0x84, //12_darkblue
    0x84, 0x00, 0x84, //13_darkpurple
    0x00, 0x84, 0x84, //14_darkskyblue
    0x84, 0x84, 0x84  //15_darkgray
  };
  set_palette(0, 15, table_rgb);
  return;
}

void set_palette(int start, int end, unsigned char *rgb){
  int i, eflags;
  eflags = io_load_eflags(); //割り込みflag 確認
  io_cli();
  io_out8(0x03c8, start);
  for(i = start; i <= end; i++){
    io_out8(0x3c9,rgb[0]/4);
    io_out8(0x3c9,rgb[1]/4);
    io_out8(0x3c9,rgb[2]/4);
    rgb += 3;
  }
  io_store_eflags(eflags); //reset eflags
  return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s){
  extern char hankaku[4096];
  for(;*s != 0x00; s++){
    putfont8(vram, xsize, x, y, c, hankaku + *s *16);
    x += 8;
  }
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font){
  int i;
  char *p, d;
  for(i=0; i < 16; i++){
    p = vram + (y + i) * xsize + x;
    d = font[i];
    if ((d & 0x80) != 0) { p[0] = c; }
    if ((d & 0x40) != 0) { p[1] = c; }
    if ((d & 0x20) != 0) { p[2] = c; }
    if ((d & 0x10) != 0) { p[3] = c; }
    if ((d & 0x08) != 0) { p[4] = c; }
    if ((d & 0x04) != 0) { p[5] = c; }
    if ((d & 0x02) != 0) { p[6] = c; }
    if ((d & 0x01) != 0) { p[7] = c; }
  }
  return ;
}

void init_mouse_cursor8(char *mouse, char bc){
  static char cursor[16][16] = {
    "**************..",
    "*OOOOOOOOOOO*...",
    "*OOOOOOOOOO*....",
    "*OOOOOOOOO*.....",
    "*OOOOOOOO*......",
    "*OOOOOOO*.......",
    "*OOOOOOO*.......",
    "*OOOOOOOO*......",
    "*OOOO**OOO*.....",
    "*OOO*..*OOO*....",
    "*OO*....*OOO*...",
    "*O*......*OOO*..",
    "**........*OOO*.",
    "*..........*OOO*",
    "............*OO*",
    ".............***"
  };
  int x, y;

  for(y = 0; y < 16; y++){
      for(x = 0; x < 16; x++){
	if(cursor[y][x] == '*'){
	  mouse[y*16 + x] = COL8_000000;
	}
	if(cursor[y][x] == 'O'){
	  mouse[y*16 + x] = COL8_FFFFFF;
	}
	if(cursor[y][x] == '.'){
	  mouse[y*16 + x] = bc;
	}
      }
  }
  return ;
}
