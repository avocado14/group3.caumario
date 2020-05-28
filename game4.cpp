#include<bangtal.h>
#include <iostream>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<ctime>
using namespace std;


SceneID scene_g4;
ObjectID g4c1, g4jumpbutton,g4startbutton, g4restartbutton,g4obj[20] ;
TimerID g4timer1, g4timer2, g4obmove;
SoundID g4theme;

const char* g4objfile[5] =
{ "image/game4/c2.png" ,"image/game4/c3.png" ,"image/game4/c4.png" ,"image/game4/c5.png" ,"image/game4/c6.png"  };
int g4objnumber[20] ={ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19 };
//0123 c2   4567 c3   891011 c4  12131415 c5  16171819 c6
int movetype;
int g4objx[20], g4objy[20];
bool alive = true;

int g4c1y=210,g4gravity=20, g4c1yjump2start;
bool g4isjumping1, g4isjumping2,g4isBottom,g4jumping1process=false, g4jumping2process = false;



ObjectID g4createObject(const char* image, SceneID scene, int x, int y, bool shown) {
    ObjectID object = createObject(image);
    locateObject(object, scene, x, y);
    if (shown) {
        showObject(object);
    }
    return(object);
}

void g4death() {
    if (g4c1y < 290) {
        for (int i = 0; i < 4; i++) {
            if (g4objx[i]<270&& g4objx[i]>150) {
                stopTimer(g4timer1);
                stopTimer(g4timer2);
                stopTimer(g4obmove);
                showMessage("dead");
                alive = false;
            }
            else {

            }
        }
    }
}

void g4jump1() {
    
    //점프중 = y감소
    //점프끝 = y증가
    if (g4isjumping1)
    {
        g4c1y += g4gravity;
        locateObject(g4c1, scene_g4, 210, g4c1y);
    }
    else
    {
        g4c1y -= g4gravity;
        locateObject(g4c1, scene_g4, 210, g4c1y);
        
    }

    //Y가 계속해서 증가하는걸 막기위해 바닥을 지정.
    //Y가 증가한다는 것은 공룡이 아래로 내려온다는 뜻.
    if (g4c1y < 230)
    {
        g4c1y = 210;
        g4isBottom = true;
        g4jumping1process = false;
        g4jumping2process = false;
        stopTimer(g4timer1);
    }

    //점프의 맨 위를 찍으면 점프가 끝난 상황
    if (g4c1y > 410)
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
        }
        else
        {   g4c1y -= g4gravity;
            locateObject(g4c1, scene_g4, 210, g4c1y);
            
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
        if (g4c1y > g4c1yjump2start + 200)
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

void g4obstaclemove(ObjectID obstacle,SceneID scene,int movetype,int currentx,int currenty,int objID) {
    
    if (movetype == 1) {
        
        switch (objID) {
        case(0):
        {
            g4objy[objID] = 210;
            if (g4objx[objID] < 0) {
                srand((unsigned int)time(NULL));
                int num = rand();
                int rndvalue = num % 5;
                g4objx[objID] = 1300 + rndvalue * 20;
            }
            else
                g4objx[objID] = g4objx[objID] - 10;

            locateObject(obstacle, scene, g4objx[objID], g4objy[objID]);
            break;
        }
        case(1):
        { g4objy[objID] = 210;
        if (g4objx[objID] < 0) {
            srand((unsigned int)time(NULL));
            int num = rand();
            int rndvalue = num % 5;
            g4objx[objID] = 1300 + rndvalue * 20;
        }
        else
            g4objx[objID] = g4objx[objID] - 10;

        locateObject(obstacle, scene, g4objx[objID], g4objy[objID]);
        break;
        }
        case(2):
        {
            g4objy[objID] = 210;
            if (g4objx[objID] < 0) {
                srand((unsigned int)time(NULL));
                int num = rand();
                int rndvalue = num % 5;
                g4objx[objID] = 1300 + rndvalue * 20;
            }
            else
                g4objx[objID] = g4objx[objID] - 10;

            locateObject(obstacle, scene, g4objx[objID], g4objy[objID]);
            break;
        }
        case(3):
        {
            g4objy[objID] = 210;
            if (g4objx[objID] < 0) {
                srand((unsigned int)time(NULL));
                int num = rand();
                int rndvalue = num % 5;
                g4objx[objID] = 1300 + rndvalue * 20;
            }
            else
                g4objx[objID] = g4objx[objID] - 10;

            locateObject(obstacle, scene, g4objx[objID], g4objy[objID]);
            break;
        }
        }
    }
}

void g4update() {
    //g4placeobstacle(g4obj[0], scene_g4, 1300, 210, true);
    
    for ( int i= 0; i < 4; i++) {
        g4obstaclemove(g4obj[i], scene_g4, 1, g4objx[i], g4objy[i],i);
    }
    g4death();
}

void g4gamestart() {
    for (int i = 0; i < 4; i++) {

        srand((unsigned int)time(NULL));          
        int num = rand();
        int rndvalue = num % 5;
        g4objx[i] = 1300 + 320*i+rndvalue * 20;
        g4objy[i] = 210;
        g4placeobstacle(g4obj[i], scene_g4, g4objx[i], g4objy[i], true);
    }
    startTimer(g4obmove);
}

void g4gamerestart() {
    g4gamestart();
    g4c1y = 210;
    locateObject(g4c1, scene_g4, 210,g4c1y);
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
    if (timer == g4obmove) {//점프 2
        
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


	scene_g4 = createScene("game4", "image/game4/background.png");
    g4jumpbutton = g4createObject("image/game4/확인.png", scene_g4, 800, 110, true);
    g4startbutton = g4createObject("image/game4/확인.png", scene_g4, 500, 110, true);
    g4restartbutton = g4createObject("image/game4/다시시작.png", scene_g4, 600, 500, true);
	g4c1 = g4createObject("image/game4/c1.png", scene_g4, 210, 210, true);
    for (int i = 0; i < 20; i++) {        
        g4obj[i] = g4createObject(g4objfile[g4objnumber[(i-(i%4))/4]], scene_g4, 1300, 210, true);
    }
	g4timer1 = createTimer(0.01f);
    g4timer2 = createTimer(0.01f);
    g4obmove = createTimer(0.01f);
    
    

}