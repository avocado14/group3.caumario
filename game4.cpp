#include<bangtal.h>
#include <iostream>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<ctime>
using namespace std;

#define g4_character_size_width 60 
#define g4_character_size_length 100 
#define g4_obj1_size_width 30
#define g4_obj1_size_length 100 
#define g4_obj2_size_width 80
#define g4_obj2_size_length 90 
#define g4_obj3_size_width 80 
#define g4_obj3_size_length 100 
#define g4_obj4_size_width  80   
#define g4_obj4_size_length  80

#define g4_character_jump_hight 250




SceneID scene_g4;
ObjectID g4c1, g4jumpbutton,g4startbutton, g4restartbutton,g4obj1[4], g4obj2[4], g4obj3[4], g4obj4[4], g4obj5[4],g4floor1,g4floor2;
TimerID g4timer1, g4timer2, g4obmove;
SoundID g4theme;

const char* g4objfile[5] =
{ "image/game4/선인/선인 애니메이션 1.png" ,"image/game4/c3.png" ,"image/game4/c4.png" ,"image/game4/c5.png" ,"image/game4/c6.png"  };
int g4objnumber[20] ={ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
//0123 지네   4567 c3   891011 c4  12131415 c5  16171819 c6
int movetype;
//-----------선인---------

double g4obj1x[4], g4obj1y[4];
double g4obj1difficulty;

//------------해골 거북이-------
double g4obj2x[4], g4obj2y[4];




//------------하늘 거북이-------

double g4obj3x[4], g4obj3y[4];
int g4obj3startx[4], g4obj3starty[4] ;


//------------점프 하는 놈-------

double g4obj4x[4], g4obj4y[4];


//------------마리오 점프----------
double g4c1y=210,g4gravity=20, g4c1yjump2start;
bool g4isjumping1, g4isjumping2,g4isBottom,g4jumping1process=false, g4jumping2process = false;
double g4c1jump1cache, g4c1jump2cache;






//----------------애니메이션------------
//---------마리오----
const char* g4c1animationfile[10] =
{ "image/game4/마리오/마리오 애니메이션1.png","image/game4/마리오/마리오 애니메이션2.png","image/game4/마리오/마리오 애니메이션3.png","image/game4/마리오/마리오 애니메이션4.png",
"image/game4/마리오/마리오 애니메이션5.png","image/game4/마리오/마리오 애니메이션6.png" ,"image/game4/마리오/마리오 애니메이션7.png" ,"image/game4/마리오/마리오 애니메이션8.png"
,"image/game4/마리오/마리오 애니메이션9.png" ,"image/game4/마리오/마리오 애니메이션10.png" };
const char* g4c1jumpanimationfile[3] =
{ "image/game4/마리오/마리오 애니메이션 점프 1.png","image/game4/마리오/마리오 애니메이션 점프 2.png","image/game4/마리오/마리오 애니메이션 점프 3.png" };
double g4c1animationcache, g4c1animationcache1;


//-------선인------------
double g4obj1animationcache, g4obj1animationcache1;//1이 계속 증가하는 값이고 0가 나머지 잠시 저장하는 변수
const char* g4obj1animationfile[4] =
{ "image/game4/선인/선인 애니메이션 1.png","image/game4/선인/선인 애니메이션 2.png","image/game4/선인/선인 애니메이션 3.png","image/game4/선인/선인 애니메이션 2.png" };


//---------해골 거북이-----------
double g4obj2animationcache, g4obj2animationcache1;
const char* g4obj2animationfile[10] =
{ "image/game4/해골 거북이/해골거북이애니메이션1.png","image/game4/해골 거북이/해골거북이애니메이션2.png","image/game4/해골 거북이/해골거북이애니메이션3.png","image/game4/해골 거북이/해골거북이애니메이션4.png" 
,"image/game4/해골 거북이/해골거북이애니메이션5.png" ,"image/game4/해골 거북이/해골거북이애니메이션6.png" ,"image/game4/해골 거북이/해골거북이애니메이션7.png" ,"image/game4/해골 거북이/해골거북이애니메이션8.png" 
,"image/game4/해골 거북이/해골거북이애니메이션9.png" ,"image/game4/해골 거북이/해골거북이애니메이션10.png" };


//---------하늘 거북이---------
double g4obj3animationcache, g4obj3animationcache1;
const char* g4obj3animationfile[7] =
{ "image/game4/하늘 거북이/하늘거북이 애니메이션 1.png","image/game4/하늘 거북이/하늘거북이 애니메이션 2.png","image/game4/하늘 거북이/하늘거북이 애니메이션 3.png","image/game4/하늘 거북이/하늘거북이 애니메이션 4.png",
"image/game4/하늘 거북이/하늘거북이 애니메이션 5.png","image/game4/하늘 거북이/하늘거북이 애니메이션 6.png","image/game4/하늘 거북이/하늘거북이 애니메이션 7.png", };

//-----------바닥----------
int g4floor1x=0, g4floor2x=2500;


ObjectID g4createObject(const char* image, SceneID scene, int x, int y, bool shown) {
    ObjectID object = createObject(image);
    locateObject(object, scene, x, y);
    if (shown) {
        showObject(object);
    }
    return(object);
}

void g4c1animation() {
    
    if((g4jumping1process == true)||(g4jumping2process ==true)){
    }
    else {
        g4c1animationcache = fmod(g4c1animationcache1, 10);
        if (g4c1animationcache >= 0 && g4c1animationcache < 1) {
            setObjectImage(g4c1, g4c1animationfile[0]);
        }
        else if (g4c1animationcache >= 1 && g4c1animationcache < 2) {
            setObjectImage(g4c1, g4c1animationfile[1]);
        }
        else if (g4c1animationcache >= 2 && g4c1animationcache < 3) {
            setObjectImage(g4c1, g4c1animationfile[2]);
        }
        else if (g4c1animationcache >= 3 && g4c1animationcache < 4) {
            setObjectImage(g4c1, g4c1animationfile[3]);
        }
        else if (g4c1animationcache >= 4 && g4c1animationcache < 5) {
            setObjectImage(g4c1, g4c1animationfile[4]);
        }
        else if (g4c1animationcache >= 5 && g4c1animationcache < 6) {
            setObjectImage(g4c1, g4c1animationfile[5]);
        }
        else if (g4c1animationcache >= 6 && g4c1animationcache < 7) {
            setObjectImage(g4c1, g4c1animationfile[6]);
        }
        else if (g4c1animationcache >= 7 && g4c1animationcache < 8) {
            setObjectImage(g4c1, g4c1animationfile[7]);
        }
        else if (g4c1animationcache >= 8 && g4c1animationcache < 9) {
            setObjectImage(g4c1, g4c1animationfile[8]);
        }
        else if (g4c1animationcache >= 9 && g4c1animationcache < 10) {
            setObjectImage(g4c1, g4c1animationfile[9]);
        }
        
        
    }
    g4c1animationcache1 = g4c1animationcache1 + 0.7;

}


void g4obj1animation() {
    for (int i = 0; i < 4; i++) {
        g4obj1animationcache = fmod(g4obj1animationcache1, 4);
        if (g4obj1animationcache >= 0 && g4obj1animationcache < 1) {
            setObjectImage(g4obj1[i], g4obj1animationfile[0]);
        }
        else  if (g4obj1animationcache >= 1 && g4obj1animationcache < 2) {
            setObjectImage(g4obj1[i], g4obj1animationfile[1]);
        }
        else  if (g4obj1animationcache >= 2 && g4obj1animationcache < 3) {
            setObjectImage(g4obj1[i], g4obj1animationfile[2]);
        }
        else  if (g4obj1animationcache >= 3 && g4obj1animationcache < 4) {
            setObjectImage(g4obj1[i], g4obj1animationfile[3]);
        }       

    }
    g4obj1animationcache1 = g4obj1animationcache1 + 0.7;
}

void g4obj2animation() {
    for (int i = 0; i < 4; i++) {
        g4obj2animationcache = fmod(g4obj2animationcache1, 10);
            if (g4obj2animationcache >= 0 && g4obj2animationcache < 1) {
                setObjectImage(g4obj2[i], g4obj2animationfile[0]);
            }
            else  if (g4obj2animationcache >= 1 && g4obj2animationcache < 2) {
                setObjectImage(g4obj2[i], g4obj2animationfile[1]);
            }
            else  if (g4obj2animationcache >= 2 && g4obj2animationcache < 3) {
                setObjectImage(g4obj2[i], g4obj2animationfile[2]);
            }
            else  if (g4obj2animationcache >= 3 && g4obj2animationcache < 4) {
                setObjectImage(g4obj2[i], g4obj2animationfile[3]);
            }
            else  if (g4obj2animationcache >= 4 && g4obj2animationcache < 5) {
                setObjectImage(g4obj2[i], g4obj2animationfile[4]);
            }
            else  if (g4obj2animationcache >= 5 && g4obj2animationcache < 6) {
                setObjectImage(g4obj2[i], g4obj2animationfile[5]);
            }
            else  if (g4obj2animationcache >= 6 && g4obj2animationcache < 7) {
                setObjectImage(g4obj2[i], g4obj2animationfile[6]);
            }
            else  if (g4obj2animationcache >= 7 && g4obj2animationcache < 8) {
                setObjectImage(g4obj2[i], g4obj2animationfile[7]);
            }
            else  if (g4obj2animationcache >= 8 && g4obj2animationcache < 9) {
                setObjectImage(g4obj2[i], g4obj2animationfile[8]);
            }
            else  if (g4obj2animationcache >= 9 && g4obj2animationcache < 10) {
                setObjectImage(g4obj2[i], g4obj2animationfile[9]);
            }   
    }
    g4obj2animationcache1 = g4obj2animationcache1 +0.5;
    }

void g4obj3animation() {
    for (int i = 0; i < 4; i++) {
        g4obj3animationcache = fmod(g4obj3animationcache1, 7);
        if (g4obj3animationcache >= 0 && g4obj3animationcache < 1) {
            setObjectImage(g4obj3[i], g4obj3animationfile[0]);
        }
        else  if (g4obj3animationcache >= 1 && g4obj3animationcache < 2) {
            setObjectImage(g4obj3[i], g4obj3animationfile[1]);
        }
        else  if (g4obj3animationcache >= 2 && g4obj3animationcache < 3) {
            setObjectImage(g4obj3[i], g4obj3animationfile[2]);
        }
        else  if (g4obj3animationcache >= 3 && g4obj3animationcache < 4) {
            setObjectImage(g4obj3[i], g4obj3animationfile[3]);
        }
        else  if (g4obj3animationcache >= 4 && g4obj3animationcache < 5) {
            setObjectImage(g4obj3[i], g4obj3animationfile[4]);
        }
        else  if (g4obj3animationcache >= 5 && g4obj3animationcache < 6) {
            setObjectImage(g4obj3[i], g4obj3animationfile[5]);
        }
        else  if (g4obj3animationcache >= 6 && g4obj3animationcache < 7) {
            setObjectImage(g4obj3[i], g4obj3animationfile[6]);
        }
       
    }
    g4obj3animationcache1 = g4obj3animationcache1 + 0.7;
}

void g4flooranimation() {
    if (g4floor1x < -2600) {
        g4floor1x = g4floor2x + 2500;
        
    }
    if (g4floor2x < -2600) {
        g4floor2x = g4floor1x + 2500;
        
    }
    else {
        g4floor1x = g4floor1x - 5;
        g4floor2x = g4floor2x - 5;
    }
}

void g4death() {
    
        for (int i = 0; i < 4; i++) {
            if ((210 < (g4obj1x[i] + g4_obj1_size_width)) && ((210 + g4_character_size_width) > g4obj1x[i]) &&
                (g4c1y < (g4obj1y[i] + g4_obj1_size_length)) && ((g4c1y + g4_character_size_length) > g4obj1y[i])){
                stopTimer(g4timer1);
                stopTimer(g4timer2);
                stopTimer(g4obmove);
                showMessage("dead");               
            }
            if ((210 < (g4obj2x[i] + g4_obj2_size_width)) && (210 + g4_character_size_width > g4obj2x[i]) &&
                (g4c1y < (g4obj2y[i] + g4_obj2_size_length)) && (g4c1y + g4_character_size_length > g4obj2y[i])) {
                stopTimer(g4timer1);
                stopTimer(g4timer2);
                stopTimer(g4obmove);
                showMessage("dead");
            }
            if ((210 < (g4obj3x[i] + g4_obj3_size_width)) && (210 + g4_character_size_width > g4obj3x[i]) &&
                (g4c1y < (g4obj3y[i] + g4_obj3_size_length)) && (g4c1y + g4_character_size_length > g4obj3y[i])) {
                stopTimer(g4timer1);
                stopTimer(g4timer2);
                stopTimer(g4obmove);
                showMessage("dead");
            }
            if ((210 < (g4obj4x[i] + g4_obj4_size_width)) && (210 + g4_character_size_width > g4obj4x[i]) &&
                (g4c1y < (g4obj4y[i] + g4_obj4_size_length)) && (g4c1y + g4_character_size_length > g4obj4y[i])) {
                stopTimer(g4timer1);
                stopTimer(g4timer2);
                stopTimer(g4obmove);
                showMessage("dead");
            }
            
        }
    
}
void g4jump11() {

    if (g4jumping1process)
    {
        g4c1y= -(1/40)*g4c1jump1cache* g4c1jump1cache+250;
        g4c1jump1cache = g4c1jump1cache + 10;
        locateObject(g4c1, scene_g4, 210, g4c1y);
        setObjectImage(g4c1, g4c1jumpanimationfile[0]);
    }
    

    //Y가 계속해서 증가하는걸 막기위해 바닥을 지정.
    //Y가 증가한다는 것은 공룡이 아래로 내려온다는 뜻.
    if (g4c1y < 110)
    {
        g4c1y = 110;
        g4isBottom = true;
        g4jumping1process = false;
        
        stopTimer(g4timer1);
        g4c1jump1cache = -100;
    }

    //점프의 맨 위를 찍으면 점프가 끝난 상황
    

}

void g4jump1() {
    
    //점프중 = y감소
    //점프끝 = y증가
    if (g4isjumping1)
    {
        g4c1y += g4gravity;
        locateObject(g4c1, scene_g4, 210, g4c1y);
        setObjectImage(g4c1, g4c1jumpanimationfile[0]);
    }
    else
    {
        g4c1y -= g4gravity;
        locateObject(g4c1, scene_g4, 210, g4c1y);
        setObjectImage(g4c1, g4c1jumpanimationfile[2]);
    }

    //Y가 계속해서 증가하는걸 막기위해 바닥을 지정.
    //Y가 증가한다는 것은 공룡이 아래로 내려온다는 뜻.
    if (g4c1y < 110)
    {
        g4c1y = 110;
        g4isBottom = true;
        g4jumping1process = false;
        g4jumping2process = false;
        stopTimer(g4timer1);
        g4c1jump1cache = -100;
    }

    //점프의 맨 위를 찍으면 점프가 끝난 상황
    if (g4c1y > 110+ g4_character_jump_hight)
    {
        g4isjumping1 = false;        
    }
   
}
void g4jump2() {

    //점프중 = y감소
    //점프끝 = y증가
    if (g4jumping2process) {
        if (g4isjumping2)
        {   g4c1y += g4gravity;
            locateObject(g4c1, scene_g4, 210, g4c1y);
            setObjectImage(g4c1, g4c1jumpanimationfile[1]);
        }
        else
        {   g4c1y -= g4gravity;
            locateObject(g4c1, scene_g4, 210, g4c1y);
            setObjectImage(g4c1, g4c1jumpanimationfile[2]);
            
        }

        //Y가 계속해서 증가하는걸 막기위해 바닥을 지정.
        //Y가 증가한다는 것은 공룡이 아래로 내려온다는 뜻.
        if (g4c1y < g4c1yjump2start - 1)
        {
            g4jumping2process = false;
            stopTimer(g4timer2);
            startTimer(g4timer1);
            g4jumping1process = true;
            g4isjumping1 = false;
        }

        //점프의 맨 위를 찍으면 점프가 끝난 상황
        if (g4c1y > g4c1yjump2start + g4_character_jump_hight)
        {
            g4isjumping2 = false;
        }        
    }
    
}

void g4placeobstacle(ObjectID obstacle, SceneID scene, int x, int y, bool shown) {
    if (shown) {
        showObject(obstacle);
    }
    else hideObject(obstacle);
    locateObject(obstacle, scene, x, y);
}

void g4obstaclemove(ObjectID obstacle,SceneID scene,int movetype,int objID) {
    
    if (movetype == 1) {
       g4obj1y[objID] = 110;
       if (g4obj1x[objID] < 0) {
           srand((unsigned int)time(NULL));
           int num = rand();
           int rndvalue = num % 5;
           g4obj1x[objID] = 1300 + rndvalue * 20;
       }
       else {
           g4obj1x[objID] = g4obj1x[objID] - 5;
       }
    }
    if (movetype == 2) {
        g4obj2y[objID] = 110;
        if (g4obj2x[objID] < 0) {
            srand((unsigned int)time(NULL));
            int num = rand();
            int rndvalue = num % 5;
            g4obj2x[objID] = 1300 + rndvalue * 20;
        }
        else {
            g4obj2x[objID] = g4obj2x[objID] - 10;
        }
    }
    if (movetype == 3) {       
        if (g4obj3x[objID] < 0) {
            //srand((unsigned int)time(NULL));
            int num = rand();
            int rndvalue = num % 5;
            
            //g4obj3startx[objID] = 1300 + rndvalue * 20;
           // g4obj3starty[objID] = 400 + rndvalue * 30;
            g4obj3x[objID] = 1300 + rndvalue * 20;
            
            g4obj3starty[objID] = 400 + rndvalue * 30;
            g4obj3y[objID] = g4obj3starty[objID];
        }
        else {
            g4obj3x[objID] = g4obj3x[objID] - 3;
            g4obj3y[objID] = 50*sin((0.1)*g4obj3x[objID])+ g4obj3starty[objID];
        }
    }
}

void g4update() {
    g4c1animation();
    g4obj1animation();
    g4obj2animation();
    g4obj3animation();
    g4flooranimation();
    g4death();
    //-------좌표이동
    for (int i = 0; i < 4; i++) {
        g4obstaclemove(g4obj1[i], scene_g4, 1, i);
    }
    for (int i = 0; i < 4; i++) {
        g4obstaclemove(g4obj2[i], scene_g4, 2, i);
    }
    for (int i = 0; i < 4; i++) {
        g4obstaclemove(g4obj3[i], scene_g4, 3, i);
    }
    //-----배치
    for (int i=0; i < 4; i++) {
        locateObject(g4obj1[i], scene_g4, g4obj1x[i], g4obj1y[i]);
    }
    for (int i = 0; i < 4; i++) {
        locateObject(g4obj2[i], scene_g4, g4obj2x[i], g4obj2y[i]);
    }
    for (int i = 0; i < 4; i++) {
        locateObject(g4obj3[i], scene_g4, g4obj3x[i], g4obj3y[i]);
    }
    for (int i = 0; i < 4; i++) {
        locateObject(g4obj4[i], scene_g4, g4obj4x[i], g4obj4y[i]);
    }    
    locateObject(g4floor1, scene_g4, g4floor1x, 0);
    locateObject(g4floor2, scene_g4, g4floor2x, 0);
}

void g4gamestart() {
    for (int i = 0; i < 4; i++) {
        srand((unsigned int)time(NULL));          
        int num = rand();
        int rndvalue = num % 5;
        g4obj1x[i] = 1300 + 320*i+rndvalue * 20;
        g4obj1y[i] = 110;
        g4placeobstacle(g4obj1[i], scene_g4, g4obj1x[i], g4obj1y[i], true);
    }
    for (int i = 0; i < 4; i++) {
        //srand((unsigned int)time(NULL));
        int num = rand();
        int rndvalue = num % 5;
        g4obj2x[i] = 1300 + 320 * i + rndvalue * 20;
        g4obj2y[i] = 110;
        g4placeobstacle(g4obj2[i], scene_g4, g4obj2x[i], g4obj2y[i], true);
    }
   
    for (int i = 0; i < 4; i++) {
        //srand((unsigned int)time(NULL));
        int num = rand();
        int rndvalue = num % 5;
        g4obj3x[i] = 1300 + 320 * i + rndvalue * 20;
        g4obj3starty[i] = 400 + rndvalue * 30;
        g4obj3y[i] = g4obj3starty[i];
        g4placeobstacle(g4obj3[i], scene_g4, g4obj3x[i], g4obj3y[i], true);
    }
    startTimer(g4obmove);
}

void g4gamerestart() {
    g4gamestart();
    g4c1y = 110;
    locateObject(g4c1, scene_g4, 210,g4c1y);
    g4jumping1process = false;
    g4jumping2process = false;
}

void Game4_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

	if(object == g4jumpbutton) {
      if (!g4jumping1process) {
            g4isjumping1 = true;
            g4isBottom = false;
            g4jumping1process = true;
            startTimer(g4timer1);
        }
        else {
            if (g4jumping1process && !g4jumping2process ) {
                g4isjumping2 = true;
                g4jumping2process = true;
                g4c1yjump2start = g4c1y;
                stopTimer(g4timer1);
                startTimer(g4timer2);
            }
        }
	}
    else if (object == g4startbutton) {
        g4gamestart();
    }
    else if (object == g4restartbutton) {
        g4gamerestart();
    }

}
void Game4_timerCallback(TimerID timer) {
	if (timer == g4timer1) {//점프 1
        
     g4jump1();
     setTimer(g4timer1, 0.01f);
     startTimer(g4timer1);
        
	}
    else if (timer == g4timer2) {//점프 2
        g4jump2(); 
        setTimer(g4timer2, 0.01f);
        startTimer(g4timer2);
    }
    if (timer == g4obmove) {//장애물 움직임
        setTimer(g4obmove, 0.01f);
        startTimer(g4obmove);
        g4update();
    }
    
}
void Game4_soundCallback(SoundID sound) {
	if (sound == g4theme) {

	}

}
void Game4_main() {


	scene_g4 = createScene("game4", "image/game4/배경.png");
    g4floor1 = g4createObject("image/game4/바닥.png", scene_g4, 0, 0, true);
    g4floor2 = g4createObject("image/game4/바닥.png", scene_g4, 2500, 0, true);
    
    g4startbutton = g4createObject("image/game4/확인.png", scene_g4, 500, 110, true);
    g4restartbutton = g4createObject("image/game4/다시시작.png", scene_g4, 600, 500, true);
	g4c1 = g4createObject("image/game4/마리오 애니메이션1.png", scene_g4, 210, 110, true);
    for (int i = 0; i < 4; i++) {        
        g4obj1[i] = g4createObject(g4objfile[0], scene_g4, 1300, 110, true);
    }
    for (int i = 0; i < 4; i++) {
        g4obj2[i] = g4createObject(g4objfile[1], scene_g4, 130, 110, true);
    }
    for (int i = 0; i < 4; i++) {
        g4obj3[i] = g4createObject(g4objfile[2], scene_g4, 500, 410, true);
    }
    for (int i = 0; i < 4; i++) {
        g4obj4[i] = g4createObject(g4objfile[3], scene_g4, 1300, 110, true);
    }
    g4jumpbutton = g4createObject("image/game4/확인.png", scene_g4, 800, 110, true);
	g4timer1 = createTimer(0.01f);
    g4timer2 = createTimer(0.01f);
    g4obmove = createTimer(0.01f);
    
    

}