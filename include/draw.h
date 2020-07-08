#ifndef __DRAW__H__
#define __DRAW__H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//#############################################
//###   ____   ____  ___    __  ___  __     ###
//###   ||  \\ || || ||\\   ||  /||  /      ###
//###   ||  || ||\\  ||_\\  || / || /       ###
//###   ||__// || \\ ||  \\ ||/  ||/        ###
//#############################################
void drawEnding() {
  int i, j;
  int posx = 0;
  int posy = 250;
  int spr_x;
  int spr_y = 350;
  int vic_x;
  int vic_y;

  int arr[] = {
1,0,0,0,1,0,1,1,1,0,0,1,1,1,0,1,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,0,0,1,
1,0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,0,
1,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,0,0,0,1,0,0,
1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,0,1,0,0,
1,0,0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,0,
};

  srand(time(NULL));

  clearLCD();

  // 100 trees
  for(i=0; i<30; i++) {
    if(i%2) {
      putItem_visible(34*i, (rand()%180),30);
      putItem_visible(34*i, (rand()%180)+440,30);
    } else {
      putItem_visible(34*i, (rand()%180),31);
      putItem_visible(34*i, (rand()%180)+440,31);
    }
  }

  // house
  putItem_visible(posx,posy,32);
  putItem_visible(posx,posy+110,33);
  putItem_visible(posx+110,posy,34);
  putItem_visible(posx+110,posy+110,35);
  drawRect(makepixel(0,0,0), posx + 87,posx + 136,posy + 129,posy + 192);

  // sprite
  for(i = 0; i<30; i++){
    usleep(200000);
    spr_x = 254 + i*22;

    if(16<i && i<27) {
      drawRect(makepixel(0,0,0), spr_x-22, spr_x-22+110, 330, 440-(i-16)*10);
    } else if(i<=16) {
      drawRect(makepixel(0,0,0), spr_x-22, spr_x-22+110, 330, 440);
    }

    if(i%4 == 0) {
      putItem_visible(spr_x,330,40);
    } else if(i%4 == 1) {
      putItem_visible(spr_x,330,41);
    } else if(i%4 == 2) {
      putItem_visible(spr_x,330,42);
    } else if(i%4 == 3) {
      putItem_visible(spr_x,330,43);
    }


    drawRect(makepixel(0,255,0),0,1024,i*10,(i+1)*10);
    drawRect(makepixel(0,255,0),0,1024,600-(i+1)*10,600-i*10);
  }


  // victory
  for(j=0; j<5; j++) {
    drawRect(makepixel(0,0,0), 5+22, 32+22, (j*27)+300, (j+1)*27+300);
    drawRect(makepixel(0,0,0), 950+22, 977+22, (j*27)+300, (j+1)*27+300);
    for(i=0; i<34; i++) {
      if(arr[j*34+i]==1) {
	drawRect(makepixel(0,255,0), i*27+32+22, (i+1)*27+32+22, (j*27)+300, (j+1)*27+300);
      } else {
	drawRect(makepixel(0,0,0), i*27+32+22, (i+1)*27+32+22, (j*27)+300, (j+1)*27+300);
      }
    }
  }
  for(i=0;i<36;i++){
    drawRect(makepixel(0,0,0), i*27+5+22, (i+1)*27+5+22, 300-27, 300);
    drawRect(makepixel(0,0,0), i*27+5+22, (i+1)*27+5+22, 408+27, 408+54);
  }

}

void drawHome() {
	int i;
	int pixel=makepixel(0,255,0);
	


        drawRect(makepixel(0,0,0),0,1024,0,600);    //BG
        drawHoleRect(pixel,295,735,55,600,5);      //DOOR
        drawRect(pixel,300,730,595,600);             //DOOR BOTTOM
        for(i=0;i<5;i++) {                              //DOOR LOCK
          drawHoleRect(pixel,250,275,(i*85)+80,(i+1)*85+60,3); // left door lock
          drawHoleRect(pixel,315,400,(i*85)+80,(i+1)*85+60,3); // right door lock
          if(shm_data->lock[i] == 1) {
            drawRect(pixel,276,314,(i*85)+90,(i+1)*85+50);     // center door lock
          } else if(shm_data->lock[i] == 2) {
            drawRect(pixel,276,314,(i*85)+90,(i+1)*85+50);     // center door lock
          } else if(shm_data->lock[i] == 0) {
            drawRect(pixel,311,314,(i*85)+90,(i+1)*85+50);     // center door lock
	  }
          drawKeypad(pixel, 350, (i*85)+105,8,2,shm_data->n);    // door lock key pad
        }
	putItem(790, 380, 11); // draw fire
	putItem(900, 380, 12); // draw fire
	putItem(790, 490, 13); // draw fire
	drawRect(makepixel(0,255,0),900,941,490,600);//draw fire
	//drawHoleRect(makepixel(0,0,255), 850,941,430,600,5);
}

//============================
//#         RIGHT            #
//============================

void drawLaptop(){
  //laptop
  drawHoleRect(makepixel(0,255,0),245,355,220,293,7); // laptop display
  drawRect(makepixel(0,255,0),240,360,290,298);	//laptop under side
  if(shm_data->quizflag[LAPTOP] == SECOND) {
    drawRect(makepixel(0,255,0), 255, 345, 230, 283); // laptop
    drawRect(makepixel(0,0,0),260,340,235,238); // string
    drawRect(makepixel(0,0,0),270,340,252,255); // string
    drawRect(makepixel(0,0,0),260,310,270,273); // string
  }
}

void drawRight() {
  drawRect(makepixel(0,0,0),0,1023,0,600);	//BG

  drawLaptop();
  
  //hanger
  drawRect(makepixel(0,255,0),700,950,100,140);	//hanger body
  drawRect(makepixel(0,180,50),740,745,140,170);	//hanger branch 1
  drawRect(makepixel(0,180,50),820,825,140,170);	//hanger branch 2
  drawRect(makepixel(0,180,50),900,905,140,170);	//hanger branch 3
  
  //bookcase
  drawRect(makepixel(0,255,0),555,570,230,298);	//book case right side
  drawRect(makepixel(0,255,0),430,445,230,298);	//book case left side
  drawRect(makepixel(0,150,250),446,470,240,298);	//book 1
  drawRect(makepixel(0,50,200),471,485,220,298);	//book 2
  drawRect(makepixel(0,200,100),486,500,220,298);	//book 3
  drawRect(makepixel(0,150,100),501,520,230,298);	//book 4
  //drawHoleRect(makepixel(0,0,255),430,570,220,298,5);	//test blue
  
  //table
  drawRect(makepixel(0,255,0),200,600,300,330);	//table upper side
  drawRect(makepixel(0,255,0),210,230,300,580);	//table left leg
  drawRect(makepixel(0,255,0),570,590,300,580);	//table right leg
  
  //chair
  drawRect(makepixel(0,255,0),320,480,260,450);	//back of the chair
  drawRect(makepixel(0,255,0),325,340,450,580);	//chair left leg
  drawRect(makepixel(0,255,0),460,475,450,580);	//chair right leg
}

//============================
//#         BACK             #
//============================

void drawWall() {
  int i;
 
  if (shm_data->quizflag[WALL] == FIRST) {
  putItem(790, 150, 14); // draw wall
  putItem(900, 150, 15); // draw wall
  } else if (shm_data->quizflag[WALL] == SECOND) {
    drawRect(makepixel(0,0,0),790,1010,150,260);	//draw wall
    putItem(845, 150, KNIFE); // draw knife
    drawHoleRect(makepixel(0,255,0),790+20,1010-20,150+8,260-10,5);	//draw wall
  }
  for(i=0; i<5; i++) {
    if(shm_data->item[i+1] == KNIFE) {
      drawRect(makepixel(0,0,0),795,1005,155,255);	//draw wall
      drawHoleRect(makepixel(0,255,0),790+20,1010-20,150+8,260-10,5);	//draw wall
    }
  }
}

void drawBack() {
  drawRect(makepixel(0,0,0),0,1023,0,600);	//BG
  
  //chairs
  drawRect(makepixel(0,255,0), 254,274,330,580);	//left chair base
  drawRect(makepixel(0,255,0), 254,404,450,480);	//left chair sheet
  drawRect(makepixel(0,255,0), 384,404,450,580);	//left chair legs
  
  drawRect(makepixel(0,255,0), 750,770,330,580);	//right chair base
  drawRect(makepixel(0,255,0), 620,770,450,480);	//right chair sheet
  drawRect(makepixel(0,255,0), 620,640,450,580);	//right chair legs
  
  //table
  drawRect(makepixel(0,255,0), 360,660,380,410);	//middle table upper side
  drawRect(makepixel(0,255,0), 492,532,380,580);	//middle table pillar
  drawRect(makepixel(0,255,0), 460,564,550,580);	//middle table under side

  drawWall();

  drawHoleRect(makepixel(0,255,0), 190,630,80,280, 10);
  drawRect(makepixel(0,0,0),342,457,62,107);
  drawHoleRect(makepixel(0,255,0),340,459,60,109, 3);
  putString(350,70,"HELP",4);
  putItem(300, 150, 20); // draw text paper
}

//============================
//#         LEFT             #
//============================
void drawphoto() {
  drawRect(makepixel(0,0,0),750,950,100,350); 
  drawHoleRect(makepixel(0,255,0),750,950,100,350,20); // Photo frame
  if(shm_data->quizflag[PHOTO] == FIRST) {
    drawRect(makepixel(0,255,0),820,825,100,350);	//main vertical line
    drawRect(makepixel(0,255,0),750,950,270,275);	//main horizonal line
    drawRect(makepixel(0,255,0),750,820,170,175);	//sub horizonal line 1
    drawRect(makepixel(0,255,0),890,950,300,305);	//sub horizonal line 2
    drawRect(makepixel(0,255,0),885,890,275,350);	//sub vertical line 1
    drawRect(makepixel(0,50,130),825,930,120,270);	//colored square 1
    drawRect(makepixel(0,100,200),770,820,275,331);	//colored square 2
    drawRect(makepixel(0,180,255),890,930,305,331);	//colored square 3
  } else if (shm_data->quizflag[PHOTO] == SECOND) {
    putItem(790, 160, 10); // draw text paper
  }
}

void drawTV() {
  drawHoleRect(makepixel(0,255,0),240,550,200,430,20); // TV display
  drawRect(makepixel(0,255,0),375,415,430,460);	//TV support neck 
  drawRect(makepixel(0,255,0),305,485,460,475);	//TV under support structure
  if(shm_data->quizflag[TV] == SECOND) {
    drawRect(makepixel(0,255,0),270,520,230,400); // TV
    drawRect(makepixel(0,0,0),390,500,260,280); // string
    drawRect(makepixel(0,0,0),320,420,300,320); // string
    drawRect(makepixel(0,0,0),290,500,350,370); // string
  }
}

void drawTVtable() {
  int i;
  drawRect(makepixel(0,255,0),140,660,480,495);	//TV Table upper side
  drawHoleRect(makepixel(0,255,0),150,650,500,600,10); // TV table under side
  drawRect(makepixel(0,255,0),165,395,515,585);	//TV Table Left drawer
  drawRect(makepixel(0,255,0),405,635,515,585);	//TV Table Right drawer
  drawRect(makepixel(0,255,0),555,635,465,480); // remote controller

  if (shm_data->quizflag[LDRAWER] == SECOND) {
    drawRect(makepixel(0,0,0),165,395,515,585);	//TV Table Left drawer
    drawRect(makepixel(0,255,0),200,260,565,585); // driver
    drawRect(makepixel(0,255,0),260,320,570,580); // driver
  }
  if (shm_data->quizflag[RDRAWER] == SECOND) {
    drawRect(makepixel(0,0,0),405,635,515,585);	//TV Table Right drawer
    drawRect(makepixel(0,255,0), 460,580,570,585); // quiz paper
  }

  for(i=0; i<5; i++) {
    if(shm_data->item[i+1] == REMOTE) {
      drawRect(makepixel(0,0,0),555,635,465,480); // remote controller
    }
    if(shm_data->item[i+1] == DRIVER) {
      drawRect(makepixel(0,0,0),165,395,515,585);	//TV Table Left drawer
    }
  }
}

void drawLeft() {
  drawRect(makepixel(0,0,0),0,1023,0,600);	//BG
  
  drawphoto();
  drawTV();
  drawTVtable();
}
#endif
