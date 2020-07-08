#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <linux/fb.h>
#include "../include/fpga_test.h"
#include "../include/font.h"
#include "../include/lcd.h"
#include "../include/draw.h"

//touch include
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <signal.h>

//ipc include
#include <sys/ipc.h>
#include <sys/shm.h>


unsigned char quit = 0;
void user_signal1(int sig) {quit = 1;}


 
void draw(int sel);
int checkpw(int selquiz);


//	__   __    __    __ ___  __
//	||\  ||\   ||\   || ||\  ||
//	|| \ || \  ||_\  || || \ ||
//	||  \||  \ ||  \ || ||  \||

int main(int argc, char** argv)
{
    // ipc variable
    shm_id = shmget((key_t)KEY_NUM, MEM_SIZE, 0666 | IPC_CREAT);
    if(shm_id == -1) {
      perror("shmget error");
      exit(0);
    }
    shm_data = (SHM_DATA*)shmat(shm_id, NULL, 0);
    if(shm_data == (SHM_DATA*)-1) {
      perror("shmat error  : ");
      exit(0);
    }
    initSHM();

    // motor variable
    int dev_motor;
    
    // buzzer variable
    int time;
    int bz_cnt = 0;
    int dev_buzzer;
    unsigned char state;
    ssize_t ret_bz;

    // touch variable
    int fd, ret;
    int x, y;
    int cnt = 0;
    pid_t pid;
    const char* evdev_path = "/dev/input/by-path/platform-imx-i2c.2-event";
    struct input_event iev[3];

    // push switch variable
    unsigned char push_sw_buf[IOM_PUSH_SWITCH_MAX_BUTTON];
    int dev_ps;
    int clicked = 0;
    int chattering = 0;
    // lcd variable
    int draw_f = 1;
    int check;
    // loop index
    int i, j, k;


    //***** MOTOR INIT *****
    dev_motor = open(STEP_MOTOR_DEVICE, O_WRONLY);

    //***** BUZZER INIT *****
    dev_buzzer = open(BUZZER_DEVICE, O_RDWR);

    //***** LCD INIT *****
    if((frame_fd = open("/dev/fb0",O_RDWR))<0) {
        perror("Frame Buffer Open Error!");
        exit(1);
    }
 
    if((check=ioctl(frame_fd,FBIOGET_VSCREENINFO,&fvs))<0) {
        perror("Get Information Error - VSCREENINFO!");
        exit(1);
    }
 
    if(fvs.bits_per_pixel != 32) {
        perror("Unsupport Mode. 32Bpp Only.");
        exit(1);
    }
 
    if(lseek(frame_fd, 0, SEEK_SET) < 0) {  
        perror("LSeek Error.");
        exit(1);
    }

    //***** PUSH SWITCH INIT *****
    dev_ps = open(PUSH_SWITCH_DEVICE, O_RDONLY);
    assert2(dev_ps >= 0, "Device open error", PUSH_SWITCH_DEVICE);

    (void)signal(SIGINT, user_signal1);
    printf("Press <ctrl+c> to quit.\n");


    //***** TOUCH INIT *****
    fd = open(evdev_path, O_RDONLY);
    if (fd < 0) {
      perror("error: could not open evdev");
      return -1;
    }


    pid = fork();
    if(pid == -1)
    {
      printf("can't fork, erro\n");
      exit(0);
    }
    else if(pid == 0){//0 = children pross
      while(1)
      {
        ret = read(fd, iev, sizeof(struct input_event)*3);
        if(ret < 0) {
          perror("error: could not read input event");
          break;
        }

        if(iev[0].type == 1 && iev[1].type == 3 && iev[2].type == 3)
        {
          printf("touch!!!!\n");
          printf("x = %d, y = %d \n",iev[1].value,iev[2].value);
	  printf("touch dismod = %d\n", shm_data->dismod);

	  if((shm_data->dismod != LOCK)&&(iev[1].value<120)) {
	    for(i=0; i<5; i++) {
	      if((0<=iev[1].value&&iev[1].value<120)
			&&(i*120<=iev[2].value&&iev[2].value<(i+1)*120)) {

		if(shm_data->selItem == i+1) {
		  shm_data->selItem = 0;
		} else {
		  shm_data->selItem = i+1;
		}
		printf("%d item is %d!\n", shm_data->selItem, shm_data->item[shm_data->selItem]);
	          //shm_data->dismod = LOCK;
	          //draw(shm_data->dismod);
	      }
	    }
	    redrawitem();
	  }

          switch(shm_data->dismod) {
                // HOME HOME HOME HOME HOME
            case HOME:
	      for(i=0; i<5; i++) {
		if((315<=iev[1].value&&iev[1].value<400)
			&&((i*85)+80<=iev[2].value&&iev[2].value<(i+1)*85+60)) {
		  printf("%d door lock touch!\n", i);
		  shm_data->selquiz = i;
	          shm_data->dismod = LOCK;
	          draw(shm_data->dismod);
		}
	      }
	      if((850<=iev[1].value&&iev[1].value<941)
		  &&(490<=iev[2].value&&iev[2].value<600)) {
		printf("touch fire\n");
		for(i=0; i<5; i++) {
		  if(shm_data->item[i+1] == KEY) {
		    break;
		  } else if(shm_data->item[i+1] == EMPTY) {
		    shm_data->item[i+1] = KEY;

//--------buzzer
        state = BUZZER_ON;
        while(bz_cnt < 49){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(2024);
        }       bz_cnt = 0;
        while(bz_cnt < 29){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(3401);
        }       bz_cnt = 0;
        state = BUZZER_OFF;
        ret_bz = write(dev_buzzer, &state, 1);
//-----------------


		    redrawitem();
		    break;
		  }
		}	
	      }
	    break;
            case RIGHT:
		// touch laptop
	      if((245<=iev[1].value&&iev[1].value<355)
		  &&(220<=iev[2].value&&iev[2].value<293)) {
	        printf("Laptop touch!\n");

		if((shm_data->quizflag[LAPTOP] == FIRST)&&(CD == shm_data->item[shm_data->selItem])){
		  shm_data->quizflag[LAPTOP] = SECOND;
		  drawLaptop();
		} else if(shm_data->quizflag[LAPTOP] == SECOND) {
		  shm_data->findquiz = LAPTOP;
		  shm_data->dismod = QUIZ;
		  clearLCD();
		  draw(shm_data->dismod);
		}
	      }
		// touch book case
	      if((430<=iev[1].value&&iev[1].value<570)
		  &&(220<=iev[2].value&&iev[2].value<298)) {
		printf("touch book case\n");
		for(i=0; i<5; i++) {
		  if(shm_data->item[i+1] == CD) {
		    break;
		  } else if(shm_data->item[i+1] == EMPTY) {
		    shm_data->item[i+1] = CD;

//--------buzzer
        state = BUZZER_ON;
        while(bz_cnt < 49){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(2024);
        }       bz_cnt = 0;
        while(bz_cnt < 29){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(3401);
        }       bz_cnt = 0;
        state = BUZZER_OFF;
        ret_bz = write(dev_buzzer, &state, 1);
//-----------------


		    redrawitem();
		    break;
		  }
		}	
	      }
	    break;
            case BACK:
		// touch HELP
	      if((300<=iev[1].value&&iev[1].value<410)
		  &&(150<=iev[2].value&&iev[2].value<260)) {
	        printf("HELP touch!\n");

		shm_data->findquiz = HELP;
		shm_data->dismod = QUIZ;
		clearLCD();
		draw(shm_data->dismod);
	      }

		// touch wall
	      if((790<=iev[1].value&&iev[1].value<1010)
		  &&(150<=iev[2].value&&iev[2].value<260)) {
	        printf("wall touch!\n");

		if((shm_data->quizflag[WALL] == FIRST)&&(DRIVER == shm_data->item[shm_data->selItem])){
		  shm_data->quizflag[WALL] = SECOND;
		  drawWall();
		} else if(shm_data->quizflag[WALL] == SECOND) {

		  for(i=0; i<5; i++) {
		    if(shm_data->item[i+1] == KNIFE) {
		      break;
		    } else if(shm_data->item[i+1] == EMPTY) {
		      shm_data->item[i+1] = KNIFE;

//--------buzzer
        state = BUZZER_ON;
        while(bz_cnt < 49){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(2024);
        }       bz_cnt = 0;
        while(bz_cnt < 29){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(3401);
        }       bz_cnt = 0;
        state = BUZZER_OFF;
        ret_bz = write(dev_buzzer, &state, 1);
//-----------------


		      redrawitem();
		      break;
		    }
		  }	
		  drawWall();
		}
	      }
	    break;
            case LEFT:
		// touch photo
	      if((750<=iev[1].value&&iev[1].value<950)
		  &&(100<=iev[2].value&&iev[2].value<350)) {
	        printf("Photo touch!\n");

		if((shm_data->quizflag[PHOTO] == FIRST)&&(KNIFE == shm_data->item[shm_data->selItem])){
		  shm_data->quizflag[PHOTO] = SECOND;
		  drawphoto();
		} else if(shm_data->quizflag[PHOTO] == SECOND) {
		  shm_data->findquiz = PHOTO;
		  shm_data->dismod = QUIZ;
		  clearLCD();
		  draw(shm_data->dismod);
		}
	      }

		// touch TV
	      if((240<=iev[1].value&&iev[1].value<550)
		  &&(200<=iev[2].value&&iev[2].value<430)) {
	        printf("TV touch!\n");

		if((shm_data->quizflag[TV] == FIRST)&&(REMOTE == shm_data->item[shm_data->selItem])){
		  shm_data->quizflag[TV] = SECOND;
		  drawTV();
		} else if(shm_data->quizflag[TV] == SECOND) {
		  shm_data->findquiz = TV;
		  shm_data->dismod = QUIZ;
		  clearLCD();
		  draw(shm_data->dismod);
		}
	      }

		// touch left drawer
	      if((165<=iev[1].value&&iev[1].value<395)
		  &&(515<=iev[2].value&&iev[2].value<585)) {
	        printf("left drawer touch!\n");

		if((shm_data->quizflag[LDRAWER] == FIRST)&&(KEY == shm_data->item[shm_data->selItem])){
		  shm_data->quizflag[LDRAWER] = SECOND;
		  drawTVtable();
		} else if(shm_data->quizflag[LDRAWER] == SECOND) {
		  for(i=0; i<5; i++) {
		    if(shm_data->item[i+1] == DRIVER) {
		      break;
		    } else if(shm_data->item[i+1] == EMPTY) {
		      shm_data->item[i+1] = DRIVER;

//--------buzzer
        state = BUZZER_ON;
        while(bz_cnt < 49){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(2024);
        }       bz_cnt = 0;
        while(bz_cnt < 29){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(3401);
        }       bz_cnt = 0;
        state = BUZZER_OFF;
        ret_bz = write(dev_buzzer, &state, 1);
//-----------------


		      redrawitem();
		      drawRect(makepixel(0,0,0),165,395,515,585);// left drawer
		      break;
		    }
		  }	
		}
	      }
		// touch right drawer
	      if((405<=iev[1].value&&iev[1].value<635)
		  &&(515<=iev[2].value&&iev[2].value<585)) {
	        printf("right drawer touch!\n");

		if((shm_data->quizflag[RDRAWER] == FIRST)&&(KEY == shm_data->item[shm_data->selItem])){
		  shm_data->quizflag[RDRAWER] = SECOND;
		  drawTVtable();
		} else if(shm_data->quizflag[RDRAWER] == SECOND) {
		  shm_data->findquiz = RDRAWER;
		  shm_data->dismod = QUIZ;
		  clearLCD();
		  draw(shm_data->dismod);
		}
	      }


		// touch remote
	      if((555<=iev[1].value&&iev[1].value<635)
		  &&(460<=iev[2].value&&iev[2].value<500)) {
		printf("touch remote\n");
		for(i=0; i<5; i++) {
		  if(shm_data->item[i+1] == REMOTE) {
		    break;
		  } else if(shm_data->item[i+1] == EMPTY) {
		    shm_data->item[i+1] = REMOTE;

//--------buzzer
        state = BUZZER_ON;
        while(bz_cnt < 49){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(2024);
        }       bz_cnt = 0;
        while(bz_cnt < 29){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(3401);
        }       bz_cnt = 0;
        state = BUZZER_OFF;
        ret_bz = write(dev_buzzer, &state, 1);
//-----------------


		    redrawitem();
		    drawRect(makepixel(0,0,0),555,635,465,480); // remote controller
		    break;
		  }
		}	
	      }

            break;
	    
	    case QUIZ:
	      if((400<=iev[1].value&&iev[1].value<1024)
		  &&(0<=iev[2].value&&iev[2].value<600)) {
		if(shm_data->findquiz == PHOTO) {
		  shm_data->dismod = LEFT;
		  draw(shm_data->dismod);
		} else if(shm_data->findquiz == RDRAWER) {
		  shm_data->dismod = LEFT;
		  draw(shm_data->dismod);
		} else if(shm_data->findquiz == TV) {
		  shm_data->dismod = LEFT;
		  draw(shm_data->dismod);
		} else if(shm_data->findquiz == LAPTOP) {
		  shm_data->dismod = RIGHT;
		  draw(shm_data->dismod);
		} else if(shm_data->findquiz == HELP) {
		  shm_data->dismod = BACK;
		  draw(shm_data->dismod);
		}
	      } 
	    break;

	    case LOCK:
	      if((30<=iev[1].value&&iev[1].value<355)
		  &&(500<=iev[2].value&&iev[2].value<570)) {
		
		clearLCD();
		draw(HOME);
		shm_data->dismod = HOME;

		checkpw(shm_data->selquiz);
		if(shm_data->lock[shm_data->selquiz] == 2) { // correct password

//------buzzer
        state = BUZZER_ON;

        while(bz_cnt < 65){
                bz_cnt++;

                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(3816);
        }       bz_cnt = 0;
        while(bz_cnt < 75){
                bz_cnt++;
                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(3030);
        }       bz_cnt = 0;
        while(bz_cnt < 90){
                bz_cnt++;

                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(2551);
        }       bz_cnt = 0;
        while(bz_cnt < 150){
                bz_cnt++;

                state = BUZZER_TOGGLE(state);
                ret_bz = write(dev_buzzer, &state, 1);
                usleep(1908);
        }       bz_cnt = 0;

        state = BUZZER_OFF;
        ret_bz = write(dev_buzzer, &state, 1);
//------------


		  open_door_db(dev_motor, 1.5, 4);
		  //for(i=0; i<5; i++) {
		  //  drawRect(makepixel(0,0,0), (i*7)+276,(i+1)*7+276,(shm_data->selquiz*85)+90, (shm_data->selquiz+1)*85+50);
		  //  drawRect(makepixel(0,255,0), 295,299,(shm_data->selquiz*85)+90, (shm_data->selquiz+1)*85+50);
		  //  usleep(400000);
		  //}
		  
		  shm_data->lock[shm_data->selquiz] = 0;

		  if(shm_data->lock[0] || shm_data->lock[1] || shm_data->lock[2] || shm_data->lock[3] || shm_data->lock[4]) {
		  } else {

		    usleep(1000000);
		    drawEnding();
//-------------buzzer
	tone_1(dev_buzzer, state, NOTE_G3, 32);
	tone_1(dev_buzzer, state, NOTE_G3, 32);
	tone_1(dev_buzzer, state, NOTE_C4, 32);
	tone_1(dev_buzzer, state, NOTE_E4, 32);
	tone_1(dev_buzzer, state, NOTE_G4, 32);

	tone_1(dev_buzzer, state, NOTE_C5, 32);
	tone_1(dev_buzzer, state, NOTE_E5, 32);
	tone_1(dev_buzzer, state, NOTE_G5, 8);

	tone_1(dev_buzzer, state, NOTE_E5, 8);


	tone_1(dev_buzzer, state, NOTE_GS3, 32);
	tone_1(dev_buzzer, state, NOTE_C4,  32);
	tone_1(dev_buzzer, state, NOTE_DS4, 32);
	tone_1(dev_buzzer, state, NOTE_GS4, 32);
	tone_1(dev_buzzer, state, NOTE_C5,  32);
	tone_1(dev_buzzer, state, NOTE_DS5, 32);
	tone_1(dev_buzzer, state, NOTE_GS5, 8);
	tone_1(dev_buzzer, state, NOTE_DS5, 8);


	tone_1(dev_buzzer, state, NOTE_AS3, 32);
	tone_1(dev_buzzer, state, NOTE_D4,  32);
	tone_1(dev_buzzer, state, NOTE_F4,  32);
	tone_1(dev_buzzer, state, NOTE_AS4, 32);
	tone_1(dev_buzzer, state, NOTE_D5,  32);
	tone_1(dev_buzzer, state, NOTE_F5,  32);
	tone_1(dev_buzzer, state, NOTE_AS5, 8);
	tone_1(dev_buzzer, state, 0, 256);
	tone_1(dev_buzzer, state, NOTE_AS5, 32);
	tone_1(dev_buzzer, state, 0, 256);
	tone_1(dev_buzzer, state, NOTE_AS5, 32);
	tone_1(dev_buzzer, state, 0, 256);
	tone_1(dev_buzzer, state, NOTE_AS5, 32);
	tone_1(dev_buzzer, state, 0, 256);
	tone_1(dev_buzzer, state, NOTE_C6, 4);
//------------------

		    shm_data->dismod = END;
		    //drawEnding();
		  }

		} else {	// wrong password


//------------buzzer
		  state = BUZZER_ON;

        	  while(bz_cnt < 65){
        	        bz_cnt++;

        	        state = BUZZER_TOGGLE(state);
        	        ret_bz = write(dev_buzzer, &state, 1);
        	        usleep(3816);
		  }       bz_cnt = 0;

        	  while(bz_cnt < 16){
        	        bz_cnt++;
        	        state = BUZZER_OFF;
        	        ret_bz = write(dev_buzzer, &state, 1);
        	        usleep(3816);
        	  }       bz_cnt = 0;
        	  while(bz_cnt < 65){
        	        bz_cnt++;
	
        	        state = BUZZER_TOGGLE(state);
        	        ret_bz = write(dev_buzzer, &state, 1);
        	        usleep(3816);
        	  }       bz_cnt = 0;

        	  state = BUZZER_OFF;
        	  ret_bz = write(dev_buzzer, &state, 1);
//--------------


		}
	      }
	    break;
	  }







        }
        else if(iev[0].type == 0 && iev[1].type == 1 && iev[2].type == 0)
        {
          //printf("hands off!!!\n");
        }
        else if(iev[0].type == 0 && iev[1].type == 3 && iev[2].type == 0 ||\
                        iev[0].type == 3 && iev[1].type == 3 && iev[2].type == 0)
        {
          //printf("touching...\n");
        }
      }
    } else {



//#############################################
//###	__  ___  _ _____ __ ______ _____    ###
//###	||  /||  / || || ||   ||   ||	    ###
//###	|| / || /  ||\\  ||   ||   ||===    ###
//###	||/  ||/   || \\ ||   ||   ||___    ###
//#############################################
    clearLCD();
    draw(HOME);
    //drawEnding();
    while (!quit) {

      usleep(400000);
      read(dev_ps, &push_sw_buf, sizeof(push_sw_buf));
      clicked = 0;
      for (i = 0; i < IOM_PUSH_SWITCH_MAX_BUTTON; i++) {
	if (push_sw_buf[i] == 1) {
	  clicked += i+1;
	}
      }
      if(clicked == 0) {
	chattering = 0;
      } else {
	if (chattering == 0) {
	  chattering = 1;

	  switch(shm_data->dismod) {
		// HOME HOME HOME HOME HOME
	    case HOME:
	    case RIGHT:
	    case BACK:
	    case LEFT:
	      if(clicked == 4) {	// if click LEFT
		if(shm_data->dismod == 0) {
		  shm_data->dismod = LEFT;
		} else {
		  shm_data->dismod--;
		}
	        draw(shm_data->dismod);
	      } else if(clicked == 6) {	// if click RIGHT
		if(shm_data->dismod == LEFT) {
		  shm_data->dismod = HOME;
		} else {
		  shm_data->dismod++;
		}
	        draw(shm_data->dismod);
	      }
	    break;
		// QUIZ QUIZ QUIZ QUIZ QUIZ
	    case LOCK:
	      if(0 < clicked && clicked <= 9) {
		for(i=0; i<3; i++) {
		  shm_data->n[i] = shm_data->n[i+1];
		}
		shm_data->n[3] = clicked;
	      } else if (clicked == 45) {
		for(i=0; i<3; i++) {
		  shm_data->n[i] = shm_data->n[i+1];
		}
		shm_data->n[3] = 0;
	      } else if (clicked == 16) {
		checkpw(shm_data->selquiz);
		clearLCD();
		draw(HOME);
		shm_data->dismod = HOME;
		continue;
	      }
	      drawKeypad(makepixel(0,255,0), 50, 255, 60, 15, shm_data->n);	//DoorLock Keypad
	    break;
	  }
	  printf("click dismod = %d\n", shm_data->dismod);

	  printf("[%d]\n", clicked);
        } else {
        }
      }
    }


  } // touch else }
  
  close(frame_fd);
  return 0;
}




//=======================================
//#   FUNCTION      #     FUNCTION      #
//=======================================

int checkpw(int sq) {
  int i;
  int bingo = 1;

  printf("%d QUIZ\n", sq);
  printf("PASSWORD : ");
  for(i=0; i<4; i++) {
    printf("%d",shm_data->n[i]);

    if(shm_data->n[i] != shm_data->answer[sq][i]) {
      bingo = 0;
    }

    shm_data->n[i] = 45;
  }
  if(bingo == 1) {
    shm_data->lock[sq] = 2;
  }
  printf("\n");
}

void initnum(int *num, char *str, int n)
{
  int i;
  for(i=0; i<n; i++) {
    num[i] = str[i] - '0';
  }
}

int open_door_db(int dev, double time, int speed){
	int i,j;
        unsigned char state[3];

        memset(state, 0, sizeof(state));
        state[0] = STEP_MOTOR_ON;
        state[1] = STEP_MOTOR_DIR_RIGHT;
        state[2] = (unsigned char)speed;
        usleep(100);

        write(dev, state, 3);



//---- open door lock motion
	for(i=0; i<5; i++) {
	  drawRect(makepixel(0,0,0), (i*7)+276,(i+1)*7+276,(shm_data->selquiz*85)+90, (shm_data->selquiz+1)*85+50);
	  drawRect(makepixel(0,255,0), 295,299,(shm_data->selquiz*85)+90, (shm_data->selquiz+1)*85+50);
	
          usleep(300000);
	}

        memset(state, 0, sizeof(state));
        state[0] = STEP_MOTOR_OFF;
        usleep(100);

        write(dev, state, 3);

        return 0;
}




//#############################################
//###	____   ____  ___    __  ___  __     ###
//###	||  \\ || || ||\\   ||  /||  /      ###
//###	||  || ||\\  ||_\\  || / || /       ###
//###	||__// || \\ ||  \\ ||/  ||/        ###
//#############################################
void draw(int sel) {
    int i;
    int tvq_mvy = 150;
    int number[50];

    switch(sel) {
	// START DISPLAY
      case HOME:
	drawHome();
	redrawitem();
      break;
      case RIGHT:
	drawRight();
	redrawitem();
      break;
      case BACK:
	drawBack();
	redrawitem();
      break;
      case LEFT:
	drawLeft();
	redrawitem();
      break;




	// QUIZ DISPLAY
      case QUIZ:
	drawRect(makepixel(0,0,0),400,1023,0,600);		//Quiz base obj
	drawHoleRect(makepixel(0,255,0), 400, 1023, 0, 600, 30);//Quiz Hole Rect

	switch(shm_data->findquiz) {
	  case PHOTO:
	    putString(450, 60, "THIS IS 5TH QUIZ", 16);
		//
	    initnum(number, "02370034400621", 14);
	    for(i=0; i<14; i++) {
	      if(i == 4 || i == 9){
	      } else {
		draw7seg(makepixel(0,255,0), (i*25)+500, 180, 3, 15, number[i]);
	      }
	    }
	    putString(600, 180, "+", 1);
	    putString(725, 180, "=", 1);
		//
	    initnum(number, "09500112200912", 14);
	    for(i=0; i<14; i++) {
	      if(i == 4 || i == 9){
	      } else {
		draw7seg(makepixel(0,255,0), (i*25)+500, 260, 3, 15, number[i]);
	      }
	    }
	    putString(600, 260, "+", 1);
	    putString(725, 260, "=", 1);
		//
	    initnum(number, "014401025", 9);
	    for(i=0; i<9; i++) {
	      if(i == 4){
	      } else {
	        draw7seg(makepixel(0,255,0), (i*25)+500, 440, 3, 15, number[i]);
	      }
	    }
	    putString(600, 440, "+", 1);
	    putString(725, 440, "=????", 5);




	  break;
	  case TV:
	    putString(450, 60, "THIS IS 2ND QUIZ", 16);

	    drawHoleRect(makepixel(0,255,0), 485, 911, 105 + tvq_mvy, 300 + tvq_mvy, 5);
	    drawRect(makepixel(0,255,0), 500, 896, 120 + tvq_mvy, 285 + tvq_mvy);

	    drawRect(makepixel(0,0,0), 533, 665, 153 + tvq_mvy, 186 + tvq_mvy);
	    drawRect(makepixel(0,0,0), 533, 566, 219 + tvq_mvy, 252 + tvq_mvy);
	    drawRect(makepixel(0,0,0), 632, 764, 219 + tvq_mvy, 252 + tvq_mvy);
	    drawRect(makepixel(0,0,0), 731, 764, 153 + tvq_mvy, 186 + tvq_mvy);
	    drawRect(makepixel(0,0,0), 830, 863, 153 + tvq_mvy, 186 + tvq_mvy);
	    drawRect(makepixel(0,0,0), 830, 863, 219 + tvq_mvy, 252 + tvq_mvy);
	  break;
	  case LAPTOP:
	    putString(450, 60, "THIS IS 4TH QUIZ", 16);
		//
	    initnum(number, "15560317403998", 14);
	    for(i=0; i<14; i++) {
	      if(i == 4 || i == 9){
	      } else {
		draw7seg(makepixel(0,255,0), (i*25)+500, 220, 3, 15, number[i]);
	      }
	    }
	    putString(600, 220, "+", 1);
	    putString(725, 220, "=", 1);
		//
	    initnum(number, "217406935", 9);
	    for(i=0; i<9; i++) {
	      if(i == 4){
	      } else {
	        draw7seg(makepixel(0,255,0), (i*25)+500, 440, 3, 15, number[i]);
	      }
	    }
	    putString(600, 440, "+", 1);
	    putString(725, 440, "=????", 5);


	  break;
	  case HELP:
	    putString(450, 60, "THIS IS 1ST QUIZ", 16);
	    //putString(450, (1*35)+60, "FIRST QUIZ", 10);
	    //putString(450, (2*35)+60, "SOLVE QUIZ", 10);
	    //putString(450, (3*35)+60, "UNLOCK DOORLOCK", 15);
	    //putString(450, (4*35)+60, "SOLVE QUIZ", 10);
	    //putString(450, (5*35)+60, "SOLVE QUIZ", 10);
	    putString(450, (6*35)+60, "PASSWORD", 8);
	    putString(450, (7*35)+60, "IS    0308", 10);
	    //putString(450, (8*35)+60, "SOLVE QUIZ", 10);
	    //putString(450, (9*35)+60, "SOLVE QUIZ", 10);
	    //putString(450, (10*35)+60, "SOLVE QUIZ", 10);
	    //putString(450, (11*35)+60, "SOLVE QUIZ", 10);
	    //putString(450, (12*35)+60, "SOLVE QUIZ", 10);
	  break;
	  case RDRAWER:
	    putString(450, 60, "THIS IS 3RD QUIZ", 16);

	    putString(450, (3*35)+60, "5+3 = 0028", 10);
	    putString(450, (4*35)+60, "9+1 = 0810", 10);
	    putString(450, (5*35)+60, "8+6 = 0214", 10);
	    putString(450, (6*35)+60, "5+4 = 0019", 10);
	    putString(450, (8*35)+60, "THEN", 4);
	    putString(450, (9*35)+60, "7+3 = ????", 10);
	  break;
	}
	redrawitem();

      break;

      case LOCK:
	drawRect(makepixel(0,0,0),0,400,0,600);			//BG
	drawHoleRect(makepixel(0,255,0),30,355,80,485,5);		//base DoorLock
	drawKeypad(makepixel(0,255,0), 50, 255, 60, 15, shm_data->n);	//DoorLock Keypad
	drawHoleRect(makepixel(0,255,0), 30,355,500,570,5);		//OK button
	putString(130, 520, "ENTER", 5);

	drawRect(makepixel(0,0,0),400,1023,0,600);		//Quiz base obj
	drawHoleRect(makepixel(0,255,0), 400, 1023, 0, 600, 30);//Quiz Hole Rect

	putString(450, 60, "FIND ITEM", 9);
	putString(450, (1*35)+60, "FIND QUIZ", 9);
	putString(450, (2*35)+60, "SOLVE QUIZ", 10);
	putString(450, (3*35)+60, "UNLOCK DOORLOCK", 15);
	//putString(450, (4*35)+60, "SOLVE QUIZ", 10);
	//putString(450, (5*35)+60, "SOLVE QUIZ", 10);
	//putString(450, (6*35)+60, "", 14);
	//putString(450, (7*35)+60, "IS    0308", 10);
	//putString(450, (8*35)+60, "SOLVE QUIZ", 10);
	//putString(450, (9*35)+60, "SOLVE QUIZ", 10);
	//putString(450, (10*35)+60, "SOLVE QUIZ", 10);
	//putString(450, (11*35)+60, "SOLVE QUIZ", 10);
	//putString(450, (12*35)+60, "SOLVE QUIZ", 10);

      break;
    }
}

