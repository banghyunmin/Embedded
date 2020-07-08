#ifndef __LCD__H__
#define __LCD__H__

#include <stdio.h>


//--- lcd global variable ---
typedef unsigned short int U32;
unsigned int makepixel(U32  r, U32 g, U32 b)
{
    return (U32)((r<<16)|(g<<8)|b);
}


int frame_fd;
struct fb_var_screeninfo fvs;

// sound define
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

//--- global my variable ---
#define KEY_NUM         1234
#define MEM_SIZE        1024

#define HOME	0
#define RIGHT	1
#define BACK	2
#define LEFT	3
#define END	777

#define LOCK	10

#define QUIZ	20

#define EMPTY	0
#define KNIFE	1
#define REMOTE	2
#define DRIVER	3
#define KEY	4
#define CD	5

#define PHOTO	0
#define TV	1
#define LAPTOP	2
#define HELP	3
#define RDRAWER	4
#define WALL	5

#define LDRAWER	10

#define FIRST	0
#define SECOND	1

//  int lock[5] = {1,1,1,1,1};
//  int dismod = HOME;
//  int selquiz = 0;
//  int n[4] = {45,45,45,45};

//  int answer[5][4] = {
//{0,0,0,0},
//{0,0,0,0},
//{0,0,0,0},
//{0,0,0,0},
//{0,0,0,0}
//};

typedef struct shm_data {
  int dismod;
  int n[4];

  int selquiz;
  int findquiz;
  int quizflag[30];

  int item[6];
  int selItem;

  int lock[5];
  int answer[5][4];

  int arr_tone[8];
}SHM_DATA;

int shm_id;
SHM_DATA *shm_data;

void initSHM() {
  int i, j;
  int PASSWORD[5][4] = {
{0,3,0,8},
{6,2,9,8},
{0,4,1,0},
{8,9,9,9},
{1,2,0,9}
};
  for(i=0; i<5; i++) {
    shm_data->lock[i] = 1;
    shm_data->item[i] = EMPTY;
  }
  shm_data->item[5] = EMPTY;

  shm_data->dismod = HOME;
  shm_data->selquiz=0;
  shm_data->findquiz=0;
  shm_data->selItem = 0;

  for(i=0; i<4; i++) {
    shm_data->n[i] = 45;
  }
  for(i=0; i<5; i++) {
    for(j=0; j<4; j++) {
      shm_data->answer[i][j] = PASSWORD[i][j];
    }
  }


  for(i=0; i<8; i++) {
    shm_data->arr_tone[i] = 0;
  }
  for(i=0; i<30; i++) {
    shm_data->quizflag[i] = FIRST;
  }
}

//#######################################
//### _____ __   __  ___  __   ____   ###
//### ||    ||   ||  ||\  ||  //      ###
//### ||==  ||   ||  || \ || ||       ###
//### ||    \\___//  ||  \||  \\___   ###
//#######################################

void initTone(int *tone, char* input) {
  int i;
  for(i=0; i<8; i++) {
    switch(input[i]) {
    case 'c':
      tone[i] = NOTE_C4;
    break;
    case 'd':
      tone[i] = NOTE_D4;
    break;
    case 'e':
      tone[i] = NOTE_E4;
    break;
    case 'f':
      tone[i] = NOTE_F4;
    break;
    case 'g':
      tone[i] = NOTE_G4;
    break;
    case 'a':
      tone[i] = NOTE_A4;
    break;
    case 'b':
      tone[i] = NOTE_B4;
    break;
    case 'C':
      tone[i] = NOTE_C5;
    break;
    case 'D':
      tone[i] = NOTE_D5;
    break;
    case 'E':
      tone[i] = NOTE_E5;
    break;
    case 'F':
      tone[i] = NOTE_F5;
    break;
    case 'G':
      tone[i] = NOTE_G5;
    break;
    case 'A':
      tone[i] = NOTE_A5;
    break;
    case 'B':
      tone[i] = NOTE_B5;
    break;
    case ' ':
      tone[i] = 0;
    break;
    case '.':
      tone[i] = -1;
    break;
    }
  }
}

void tone_1(int dev, int state, int tone, int time) {
        int bz_cnt = 0;
	int divide = time / 4;

        if(tone == 0) {
          usleep(1000000/divide);
          return;
        } else if ( tone == -1 ) {
	  return;
	}

	state = BUZZER_ON;
        while(bz_cnt < tone/divide){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                write(dev, &state, 1);
                usleep(1000000/tone);
        }
	state = BUZZER_OFF;
	write(dev, &state, 1);
}

void tone_1_8th(int dev, int state, int tone) {
  tone_1(dev, state, tone, 8);
/*
        int bz_cnt = 0;

        if(tone == 0) {
          usleep(500000);
          return;
        } else if ( tone == -1 ) {
	  return;
	}

        while(bz_cnt < tone/2){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                write(dev, &state, 1);
                usleep(1000000/tone);
        }
*/
}

void toneMeasure(int dev, int *tone) {
  int i;
  unsigned char state;

  state = BUZZER_ON;
  for(i=0; i<8; i++) {
    tone_1_8th(dev, state, tone[i]);
  }

  state = BUZZER_OFF;
  write(dev, &state, 1);
}


void putItem_visible(int posx, int posy, int ch) {
  int offset;
  int repx, repy;
  int pixel1 = makepixel(0,0,0);
  int pixel2 = makepixel(0,255,0);
  int fy, fx;
  char *temp = findItem(ch);

  fx = 110;
  fy = 110;
    for(repy=posy; repy < posy+fy; repy++) {
      for(repx = posx; repx < posx+fx; repx++) {
        offset = repy * fvs.xres * (32/8) + repx * (32/8);
        lseek(frame_fd, offset, SEEK_SET);

        if(temp[(repy-posy)*fx+(repx-posx)] == 1) {
          write(frame_fd, &pixel2,(32/8));
	} else {
          //write(frame_fd, &pixel1,(32/8));
	}
      }
    }
}

void putItem(int posx, int posy, int ch) {
  int offset;
  int repx, repy;
  int pixel1 = makepixel(0,0,0);
  int pixel2 = makepixel(0,255,0);
  int fy, fx;
  char *temp = findItem(ch);

  fx = 110;
  fy = 110;
    for(repy=posy; repy < posy+fy; repy++) {
      offset = repy * fvs.xres * (32/8) + posx * (32/8);
      if(lseek(frame_fd, offset, SEEK_SET) < 0) {
        perror("LSeek Error!");
        exit(1);
      }
      for(repx = posx; repx < posx+fx; repx++) {
        if(temp[(repy-posy)*fx+(repx-posx)] == 1) {
          write(frame_fd, &pixel2,(32/8));
	} else {
          write(frame_fd, &pixel1,(32/8));
	}
      }
    }
}


void putChar(int posx, int posy, int ch) {
  int offset;
  int repx, repy;
  int pixel1 = makepixel(0,0,0);
  int pixel2 = makepixel(0,255,0);
  int fy, fx;
  char *temp = findChar(ch);

  fx = 24;
  fy = 29;
    for(repy=posy; repy < posy+fy; repy++) {
      offset = repy * fvs.xres * (32/8) + posx * (32/8);
      if(lseek(frame_fd, offset, SEEK_SET) < 0) {
        perror("LSeek Error!");
        exit(1);
      }
      for(repx = posx; repx < posx+fx; repx++) {
        if(temp[(repy-posy)*fx+(repx-posx)] == 1) {
          write(frame_fd, &pixel2,(32/8));
	} else {
          write(frame_fd, &pixel1,(32/8));
	}
      }
    }
}

void putString(int posx, int posy, char* str, int len) {
  int i;

  for(i=0; i<len; i++) {
    putChar(posx+(i*26), posy, str[i]);
  }
}

void drawRect(int pixel,int posx1,int posx2,int posy1,int posy2) {
    int offset;
    int repx, repy;

    for(repy=posy1; repy < posy2; repy++) {
        offset = repy * fvs.xres * (32/8) + posx1 * (32/8);
        if(lseek(frame_fd, offset, SEEK_SET) < 0) {
            perror("LSeek Error!");
            exit(1);
        }
        for(repx = posx1; repx <= posx2; repx++)
            write(frame_fd, &pixel,(32/8));
    }
}

void drawHoleRect(int pixel, int posx1, int posx2, int posy1, int posy2, int thick) {
    int offset;
    int repx, repy;

    for(repy=posy1; repy < posy2; repy++) {
        for(repx = posx1; repx <= posx2; repx++) {
            if ((posy1+thick > repy || repy > posy2-thick)||(posx1+thick > repx || repx > posx2-thick))  {
              offset = repy * fvs.xres * (32/8) + repx * (32/8);
	      lseek(frame_fd, offset, SEEK_SET);
	      write(frame_fd, &pixel,(32/8));
	    }
	}
    }
}

void clearLCD() {
    drawRect(makepixel(0,0,0), 0, 1024, 0, 600);
}

void draw7seg(int pixel, int dx, int dy, int t_, int w_, int num) {
    char flag[8] = {};
    switch(num) {
	case 0: sprintf(flag, "%s", "1111110");
	break;
	case 1: sprintf(flag, "%s", "0110000");
	break;
	case 2: sprintf(flag, "%s", "1101101");
	break;
	case 3: sprintf(flag, "%s", "1111001");
	break;
	case 4: sprintf(flag, "%s", "0110011");
	break;
	case 5: sprintf(flag, "%s", "1011011");
	break;
	case 6: sprintf(flag, "%s", "1011111");
	break;
	case 7: sprintf(flag, "%s", "1110010");
	break;
	case 8: sprintf(flag, "%s", "1111111");
	break;
	case 9: sprintf(flag, "%s", "1111011");
	break;
    }

    if(flag[0] == '1') drawRect(pixel, dx+t_, dx + t_+w_, dy+0, dy+t_);
    if(flag[1] == '1') drawRect(pixel, dx+t_+w_, dx+t_+w_+t_, dy+t_, dy+t_+w_);
    if(flag[2] == '1') drawRect(pixel, dx+t_+w_, dx+t_+w_+t_, dy+t_+w_+t_, dy+t_+w_+t_+w_);
    if(flag[3] == '1') drawRect(pixel, dx+t_, dx+t_+w_, dy+t_+w_+t_+w_, dy+t_+w_+t_+w_+t_);
    if(flag[4] == '1') drawRect(pixel, dx+0, dx+t_, dy+t_+w_+t_, dy+t_+w_+t_+w_);
    if(flag[5] == '1') drawRect(pixel, dx+0, dx+t_, dy+t_, dy+t_+w_);
    if(flag[6] == '1') drawRect(pixel, dx+t_, dx+t_+w_, dy+t_+w_, dy+t_+w_+t_);
} 

void drawKeypad(int pixel, int dx_, int dy_, int width_, int itv_, int* n) {
    int i, j;
    int posx1,posx2,posy1,posy2;
    int dist = width_/13;

    for(i = 0; i < 3; i++) {
      for(j = 0; j < 3; j++) {
	posx1 = i*(width_+itv_) + dx_;
	posx2 = i*(width_+itv_) + dx_ + width_;
	posy1 = j*(width_+itv_) + dy_;
	posy2 = j*(width_+itv_) + dy_ + width_;
        drawRect(pixel, posx1, posx2, posy1, posy2);
	draw7seg(makepixel(0,0,255), posx1+(dist*5), posy1+dist, dist, dist*5, (j*3)+(i+1));
      }
    }

    i=3;
    j=2;
    posx1 = i*(width_+itv_) + dx_;
    posx2 = i*(width_+itv_) + dx_ + width_;
    posy1 = j*(width_+itv_) + dy_;
    posy2 = j*(width_+itv_) + dy_ + width_;
    drawRect(pixel, posx1, posx2, posy1, posy2);
    draw7seg(makepixel(0,0,255), posx1+(dist*5), posy1+dist, dist, dist*5, 0);

    for(i = 0; i < 4; i++) {
      posx1 = i*(width_+itv_) + dx_;
      posx2 = i*(width_+itv_) + dx_ + width_;
      posy1 = dy_ - itv_ - width_ - width_;
      posy2 = dy_ + width_ - itv_ - width_ - width_;
      drawRect(pixel, posx1, posx2, posy1, posy2);
      draw7seg(makepixel(0,0,255), posx1+(dist*5), posy1+dist, dist, dist*5, n[i]);
    }
}






//
//
//  redraw function
//
//

void redrawitem() {
  int i;

  for(i = 0; i < 5; i++) {
    drawRect(makepixel(0,0,0),0,120, 120*i, 120*(i+1));
    if(shm_data->selItem == i+1) {
      drawHoleRect(makepixel(0,0,255),0,120, 120*i, 120*(i+1), 5);
    } else {
      drawHoleRect(makepixel(0,255,0),0,120, 120*i, 120*(i+1), 5);
    }
    putItem(5,(i*120)+5,shm_data->item[i+1]);
  }
}

#endif
