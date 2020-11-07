void io_hlt(void);
void io_cli(void);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void init_palette(void);
void set_palette(int start, int end, unsigned char *rgb);

void HariMain(void)
{
  int i;
  char *p;

  init_palette();
  p = (char *)0xa0000;
  for(i = 0; i <= 0xffff; i++){
    p[i] =  i & 0x0f;
  }
  
  for(;;){
    io_hlt();
  }
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
  eflags = io_load_eflags(); //Š„‚èž‚Ýflag Šm”F
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
