#include "bootpack.h"
#include<stdio.h>

void HariMain(void)
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  char s[40], mcursor[256];
  int mx, my;

  init_gdtidt();
  
  init_palette();
  init_screen(binfo->vram, binfo->scrnx, binfo->scrny);
  // putfonts8_asc(binfo->vram, binfo->scrnx, 8, 8, COL8_FFFFFF, "ABC123");
  // putfonts8_asc(binfo->vram, binfo->scrnx, 31, 31, COL8_000000, "Haribote OS.");
  // putfonts8_asc(binfo->vram, binfo->scrnx, 32, 32, COL8_FFFFFF, "Haribote OS.");

  mx = (binfo->scrnx - 16)/ 2;
  my = (binfo->scrny - 28 -16)/ 2;
  init_mouse_cursor8(mcursor, COL8_008484);
  putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
  
  sprintf(s,"scrnx = %d", binfo->scrnx);
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s);
  
  for(;;){
    io_hlt();
  }
}
