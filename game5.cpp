#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>

#define PLAYER_SPEED				20
#define FIRE_SPEED				    25
#define KOOPA_FIRE_SPEED            15
#define KFIRE_COOLTIME              5.0f
#define KBALL_COOLTIME              5.0f
#define KRAIN_COOLTIME              30.0f
#define PLAYER_ANIMATION_TIME		0.05f
#define GAP                         20

SceneID scene_g5;
ObjectID player_g5, koopa;
ObjectID startButton_g5;
ObjectID fire1[4], fire2[4];
ObjectID koopaFire1, koopaFire2, koopaFire1Ball;
ObjectID HPbar, HPbarLine, HPbarBlank, HPright;
ObjectID playerHP[3], playerFrame, koopaFrame;
TimerID playTimer_g5, jumpTimer_g5, koopaJumpTimer, fire1MoveTimer, fire2MoveTimer, koopaDamageTimer, playerDamageTimer, deadCheckTimer;
TimerID kFireMoveTimer1, kFireMoveTimer2, kBallMoveTimer;
TimerID kFireCoolTimer1, kFireCoolTimer2, kBallCoolTimer, koopaJumpCoolTimer;
SoundID bgm_g5, jumpSound_g5, playerFireSound1, playerFireSound2, koopaFireSound1, koopaFireSound2, koopaFireSound3; 
SoundID koopaDamageSound1, koopaDamageSound2, playerDamageSound;


int dx_g5 = 0, dy_g5 = 0;
bool isJumping = false;

float playerSize_g5 = 100;
float koopaSizeX = 230, koopaSizeY = 300;
float fireSize = 45;

int playerX_g5 = 100, playerY_g5 = 150, playerBottom = playerY_g5, playerTop = 450, gravity_g5 = 25, koopaTop = 500;
int playerLife = 3;
int playerCount = 0;
bool playerDead = false;
bool isRising_g5, isLanded = true;

int koopaX = 950, koopaY = playerY_g5;
int koopaCount = 0;
int koopaHP = 500;

//마리오 불 공격
int fire1X[4], fire1Y, fire2X[4], fire2Y;
int fire1Num = 0, fire2Num = 0;
bool attacking1 = false, attacking2 = false;

//쿠파 불 공격
int kFire1X, kFire1Y;
int kFire2X, kFire2Y;
int kBallX, kBallY;
float kFireSizeX = 100, kFireSizeY = 50;
float kBallSize = 70;
bool kBallisRising;
bool koopaisRising, koopaisLanded;


extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

void endGame5() {

    stopTimer(playTimer_g5);
    stopTimer(kFireCoolTimer1);
    stopTimer(kFireCoolTimer2);
    stopTimer(kBallCoolTimer);
    stopTimer(koopaJumpCoolTimer);
    stopTimer(jumpTimer_g5);
    stopTimer(fire1MoveTimer);
    stopTimer(fire2MoveTimer);
    stopTimer(kFireMoveTimer1);
    stopTimer(kFireMoveTimer2);
    stopTimer(kBallMoveTimer);
    stopTimer(koopaDamageTimer);
    stopTimer(deadCheckTimer);

}

void jump_g5() {

    if (isRising_g5)    //상승
    {
        playerY_g5 += gravity_g5;
       
        if (playerY_g5 > playerTop)  {     //상승 끝나면 -> else
            playerY_g5 -= gravity_g5;
            isRising_g5 = false;
        }

        locateObject(playerX_g5, scene_g5, playerX_g5, playerY_g5);

        setTimer(jumpTimer_g5, 0.01f);
        startTimer(jumpTimer_g5);
    }

    else  //착지
    {
        playerY_g5 -= gravity_g5;

        if (playerY_g5 < playerBottom) {    //착지 끝나면
            playerY_g5 += gravity_g5;
            isLanded = true;
            stopTimer(jumpTimer_g5);
        }

        locateObject(playerX_g5, scene_g5, playerX_g5, playerY_g5);

        if (isLanded == false) {        //착지 전까지는 타이머 계속
            setTimer(jumpTimer_g5, 0.01f);
            startTimer(jumpTimer_g5);
        }
    }
}

void koopaJump() {

    if (koopaisRising)    //상승
    {
        koopaY += gravity_g5;

        if (koopaY > koopaTop) {     //상승 끝나면 -> else
            koopaY -= gravity_g5;
            koopaisRising = false;
        }

        locateObject(koopa, scene_g5, koopaX, koopaY);

        setTimer(koopaJumpTimer, 0.01f);
        startTimer(koopaJumpTimer);
    }

    else  //착지
    {
        koopaY -= gravity_g5;

        if (koopaY < playerBottom) {    //착지 끝나면
            koopaY += gravity_g5;
            koopaisLanded = true;
            stopTimer(koopaJumpTimer);
        }

        locateObject(koopa, scene_g5, koopaX, koopaY);

        if (koopaisLanded == false) {        //착지 전까지는 타이머 계속
            setTimer(koopaJumpTimer, 0.01f);
            startTimer(koopaJumpTimer);
        }
    }
}

void attack1() {

    playSound(playerFireSound1);

    for (int i = 0; i < 4; i++) {
        fire1X[i] = playerX_g5 + 50 - 40 * i;
        fire1Y = playerY_g5 + 20;
        locateObject(fire1[i], scene_g5, fire1X[i], fire1Y);
    }
    showObject(fire1[0]);

    fire1Num = 0;

    startTimer(fire1MoveTimer);

}

void attack2() {

    playSound(playerFireSound2);

    for (int i = 0; i < 4; i++) {
        fire2X[i] = playerX_g5 + 50 - 40 * i;
        fire2Y = playerY_g5 + 20;
        locateObject(fire2[i], scene_g5, fire2X[i], fire2Y);

    }
    showObject(fire2[0]);

    fire2Num = 0;

    startTimer(fire2MoveTimer);

}

void koopaDamage() {

    //데미지 입는 애니메이션
    koopaCount = 0;
    setTimer(koopaDamageTimer, 0.05f);
    startTimer(koopaDamageTimer);

    koopaHP -= 20;  //(20/500) 25번 때리면 죽음
    
    //체력바 줄어들기 (횐색바 왼쪽으로 옮기기)
    //x : hp + 640 
    locateObject(HPbarBlank, scene_g5, koopaHP + 640, 640);

    if (koopaHP == 0) {
        showMessage("클리어");
        endGame5();
        hideObject(koopa);
    }

}

void playerDamage() {

    playSound(playerDamageSound);

    //데미지 입는 애니메이션
    playerCount = 0;
    setTimer(playerDamageTimer, 0.05f);
    startTimer(playerDamageTimer);

    playerLife--;

    if (playerLife >= 0) {
        for (int i = playerLife; i < 3; i++) {
            hideObject(playerHP[i]);
        }
    }

    if (playerLife <= 0) {

        for (int i = 0; i < 3; i++) {
            hideObject(playerHP[i]);
        }

        showMessage("게임오버");
        endGame5();
    }

}


//플레이어 향해서 불 발사
void koopaFire1Attack() {

    playSound(koopaFireSound1);

    kFire1X = koopaX;
    kFire1Y = playerY_g5;

    locateObject(koopaFire1, scene_g5, kFire1X, kFire1Y);
    showObject(koopaFire1);

    startTimer(kFireMoveTimer1);

}

void koopaFire2Attack() {

    playSound(koopaFireSound2);

    kFire2X = koopaX;
    kFire2Y = playerY_g5;

    locateObject(koopaFire2, scene_g5, kFire2X, kFire2Y);
    showObject(koopaFire2);

    startTimer(kFireMoveTimer2);

}

//튕기는 불공 바닥으로 던지기
void koopaBallAttack() {
    
    playSound(koopaFireSound3);

    kBallX = koopaX;
    kBallY = koopaY + 200;

    locateObject(koopaFire1Ball, scene_g5, kBallX, kBallY);
    showObject(koopaFire1Ball);

    startTimer(kBallMoveTimer);

}



void Game5_mouseCallback(ObjectID object, int x, int y, MouseAction action) {

   if (object == startButton_g5) {
        startTimer(playTimer_g5);
        hideObject(startButton_g5);
        startTimer(kFireCoolTimer1);
        startTimer(kFireCoolTimer2);
        startTimer(kBallCoolTimer);
        startTimer(koopaJumpCoolTimer);
        startTimer(deadCheckTimer);
    }

}


void Game5_timerCallback(TimerID timer) {
    
    if (timer == jumpTimer_g5) {
        jump_g5();
    }

    if (timer == playTimer_g5) {
        if (playerX_g5 + dx_g5 > 1280 - playerSize_g5 || playerX_g5 + dx_g5 < 0 || playerY_g5 + dy_g5 > 720 - playerSize_g5 || playerY_g5 + dy_g5 < 0) {}	//테두리 나가면 이동 안시키기

        else {
            playerX_g5 += dx_g5; playerY_g5 += dy_g5;
            locateObject(player_g5, scene_g5, playerX_g5, playerY_g5);

        }

        if (playerX_g5 >= koopaX - playerSize_g5 && playerX_g5 <= koopaX + koopaSizeX &&
            playerY_g5 >= koopaY - playerSize_g5 && playerY_g5 <= koopaY + koopaSizeY) {     //마리오 쿠파랑 부딪히면
        
            playerLife = 0;
            playerDamage();

            return;
        }

        if (isJumping == true) {
            
            if (isLanded == true) {//바닥에 있을 때만 점프
                playSound(jumpSound_g5);
                
                isRising_g5 = true;
                isLanded = false;
                startTimer(jumpTimer_g5);
            }
        }

         setTimer(timer, PLAYER_ANIMATION_TIME);
         startTimer(timer);
        
    }

    if (timer == fire1MoveTimer) {

        //불 차례로 스폰
        if (fire1Num == 2)
            showObject(fire1[1]);
        else if (fire1Num == 4)
            showObject(fire1[2]);
        else if (fire1Num == 6)
            showObject(fire1[3]);

        if (fire1Num < 6)
            fire1Num++;

        //불 이동
        for (int i = 0; i < 4; i++) {
            fire1X[i] += FIRE_SPEED;
            locateObject(fire1[i], scene_g5, fire1X[i], fire1Y);
        }

        //불 없어질 조건
        if (fire1X[0] > 1200)               //화면 밖으로 나가면 공격 끝
            attacking1 = false;

        if (koopaX - fire1X[0] >= -koopaSizeX && koopaX - fire1X[0] <= fireSize &&
            fire1Y - koopaY >= -30 && fire1Y - koopaY <= koopaSizeY){    //불이 쿠파랑 부딪히면

            koopaDamage();
            playSound(koopaDamageSound1);

            attacking1 = false;
        }

        //공격 끝날 시
        if (attacking1 == false) {

            for (int i = 0; i < 4; i++)
                hideObject(fire1[i]);
        }

        //공격 지속
        else {
            setTimer(fire1MoveTimer, 0.05f);
            startTimer(fire1MoveTimer);
        }

    }

    if (timer == fire2MoveTimer) {

        if (fire2Num == 2)
            showObject(fire2[1]);
        else if (fire2Num == 4)
            showObject(fire2[2]);
        else if (fire2Num == 6)
            showObject(fire2[3]);

        if (fire2Num < 6)
            fire2Num++;

        
        for (int i = 0; i < 4; i++) {      
            fire2X[i] += FIRE_SPEED;
            locateObject(fire2[i], scene_g5, fire2X[i], fire2Y);
        }

        
        if (fire2X[0] > 1200)               //화면 밖으로 나가면 공격 끝
            attacking2 = false;

        if (koopaX - fire2X[0] >= -koopaSizeX && koopaX - fire2X[0] <= fireSize &&
            fire2Y - koopaY >= -30 && fire2Y - koopaY <= koopaSizeY) {    //불이 쿠파랑 부딪히면

            koopaDamage();
            playSound(koopaDamageSound2);

            attacking2 = false;
        }

        
        if (attacking2 == false) {

            for (int i = 0; i < 4; i++)
                hideObject(fire2[i]);
        }
        
        else {
            setTimer(fire2MoveTimer, 0.05f);
            startTimer(fire2MoveTimer);
        }

    }

    if (timer == koopaDamageTimer) {
        
        if (koopaCount % 2 == 0)
            setObjectImage(koopa, "image/game5/맞은쿠파.png");
        else
            setObjectImage(koopa, "image/game5/쿠파.png");
        
        if (koopaCount < 3) {
            koopaCount++;
            setTimer(koopaDamageTimer, 0.05f);
            startTimer(koopaDamageTimer);
        }
    }

    if (timer == playerDamageTimer) {

        if (playerCount % 2 == 0)
            setObjectImage(player_g5, "image/game5/맞은마리오.png");
        else
            setObjectImage(player_g5, "image/game5/마리오.png");

        if (playerCount < 3) {
            playerCount++;
            setTimer(playerDamageTimer, 0.05f);
            startTimer(playerDamageTimer);
        }
    }

    if (timer == kFireMoveTimer1) {

        kFire1X -= KOOPA_FIRE_SPEED;
        locateObject(koopaFire1, scene_g5, kFire1X, kFire1Y);

        if (kFire1X < 10)        //화면 밖으로 나가면 사라짐
            hideObject(koopaFire1);

        else if (kFire1X >= playerX_g5 - kFireSizeX + GAP && kFire1X <= playerX_g5 + playerSize_g5 - GAP &&
                 kFire1Y >= playerY_g5 - kFireSizeY + GAP && kFire1Y <= playerY_g5 + playerSize_g5 - GAP) {     //마리오랑 부딪히면

            playerDamage();

            hideObject(koopaFire1);
        }

        else {
            setTimer(kFireMoveTimer1, 0.05f);
            startTimer(kFireMoveTimer1);
        }

    }

    if (timer == kFireMoveTimer2) {

        kFire2X -= KOOPA_FIRE_SPEED + 10;
        locateObject(koopaFire2, scene_g5, kFire2X, kFire2Y);

        if (kFire2X < 10)        //화면 밖으로 나가면 사라짐
            hideObject(koopaFire2);

        else if (kFire2X >= playerX_g5 - kFireSizeX + GAP && kFire2X <= playerX_g5 + playerSize_g5 - GAP &&
                 kFire2Y >= playerY_g5 - kFireSizeY + GAP && kFire2Y <= playerY_g5 + playerSize_g5 - GAP) {     //마리오랑 부딪히면

            playerDamage();

            hideObject(koopaFire2);

       }

        else {
            setTimer(kFireMoveTimer2, 0.05f);
            startTimer(kFireMoveTimer2);
        }

    }

    if (timer == kBallMoveTimer) {

        kBallX -= KOOPA_FIRE_SPEED + 5;

        if (kBallisRising == false)
            kBallY -= KOOPA_FIRE_SPEED - 5;

        else
            kBallY += KOOPA_FIRE_SPEED - 5;

        if (kBallY <= playerBottom)         //바닥에 닿으면 위로 튕기기
            kBallisRising = true;

        locateObject(koopaFire1Ball, scene_g5, kBallX, kBallY);

        if (kBallX < 10 || kBallY > 600)        //화면 밖으로 나가면 사라짐
            hideObject(koopaFire1Ball);

        else if (kBallX >= playerX_g5 - kBallSize + GAP && kBallX <= playerX_g5 + playerSize_g5 - GAP &&
                 kBallY >= playerY_g5 - kBallSize + GAP && kBallY <= playerY_g5 + playerSize_g5 - GAP) {     //마리오랑 부딪히면

            playerDamage();

            hideObject(koopaFire1Ball);
        }

        else {
            setTimer(kBallMoveTimer, 0.05f);
            startTimer(kBallMoveTimer);
        }
    }

    if (timer == koopaJumpTimer) {
        koopaJump();
    }

    if (timer == koopaJumpCoolTimer) {
        koopaisRising = true;
        koopaisLanded = false;
        setTimer(koopaJumpTimer, 0.01f);
        startTimer(koopaJumpTimer);
        setTimer(koopaJumpCoolTimer, 4.0f);
        startTimer(koopaJumpCoolTimer);
    }

    if (timer == kFireCoolTimer1) {
        koopaFire1Attack();
        setTimer(kFireCoolTimer1, KFIRE_COOLTIME);
        startTimer(kFireCoolTimer1);
    
    }

    if (timer == kFireCoolTimer2) {
        koopaFire2Attack();
        setTimer(kFireCoolTimer2, KFIRE_COOLTIME);
        startTimer(kFireCoolTimer2);

    }

    if (timer == kBallCoolTimer) {
        kBallisRising = false;
        koopaBallAttack();
        setTimer(kBallCoolTimer, KBALL_COOLTIME);
        startTimer(kBallCoolTimer);
    }
 
}

void Game5_keyboardCallback(KeyCode code, KeyState state) {

    if (code == 83) {		// RIGHT
        dx_g5 += (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
    }
    else if (code == 82) {		// LEFT
        dx_g5 -= (state == KeyState::KEYBOARD_PRESSED ? PLAYER_SPEED : -PLAYER_SPEED);
    }

    if (code == 84) {			// Jump 
        isJumping += (state == KeyState::KEYBOARD_PRESSED ? 1 : -1);
    }

    if (code == 75) {           // 불 발사 : 스페이스바
        if (state == KeyState::KEYBOARD_PRESSED) {
            
            if (attacking1 == false) {
                attack1();
                attacking1 = true;
            }

            else if (attacking2 == false) {
                attack2();
                attacking2 = true;
            }
        }
    }
}



void Game5_soundCallback(SoundID sound) {

    if (sound == bgm_g5)
        playSound(bgm_g5);
}


void Game5_main() {

	scene_g5 = createScene("쿠파 성", "image/game5/쿠파성배경.png");

    startButton_g5 = createObject("image/game5/게임시작.png", scene_g5, 500, 100, true, 1.0f);

    //체력바 : 500 x 40
    HPbar = createObject("image/game5/체력바.png", scene_g5, 640, 640, true, 1.0f);
    HPbarBlank = createObject("image/game5/체력바흰색.png", scene_g5, 1140, 640, true, 1.0f);
    HPright = createObject("image/game5/체력바배경.png", scene_g5, 1140, 640, true, 1.0f);
    HPbarLine = createObject("image/game5/체력바테두리.png", scene_g5, 640, 640, true, 1.0f);

    playerFrame = createObject("image/game5/마리오그림.png", scene_g5, 240, 640, true, 1.3f);
    koopaFrame = createObject("image/game5/쿠파그림.png", scene_g5, 560, 640, true, 1.3f);

    for (int i = 0; i < 3; i++) {
        playerHP[i] = createObject("image/game5/하트.png", scene_g5, 320 + 55 * i, 640, true, 1.0f);
    }

	player_g5 = createObject("image/game5/마리오.png", scene_g5, playerX_g5, playerBottom, true, 1.0f);
	koopa = createObject("image/game5/쿠파.png", scene_g5, koopaX, koopaY, true, 1.0f);

    koopaFire1 = createObject("image/game5/쿠파불.png", scene_g5, 1, 1, false, 1.0f);
    koopaFire2 = createObject("image/game5/쿠파불.png", scene_g5, 1, 1, false, 1.0f);
    koopaFire1Ball = createObject("image/game5/쿠파불공.png", scene_g5, 1, 1, false, 1.0f);

    char path_g5[256];

    for (int i = 0; i < 4; i++) {
        sprintf_s(path_g5, "image/game5/불%d.png", i + 1);
        fire1[i] = createObject(path_g5, scene_g5, 1, 1, false, 0.8f);
        fire2[i] = createObject(path_g5, scene_g5, 1, 1, false, 0.8f);
    }

    playTimer_g5 = createTimer(PLAYER_ANIMATION_TIME);
    jumpTimer_g5 = createTimer(0.01f);
    fire1MoveTimer = createTimer(0.05f);
    fire2MoveTimer = createTimer(0.05f);
    koopaDamageTimer = createTimer(0.05f);
    playerDamageTimer = createTimer(0.05f);
    deadCheckTimer = createTimer(0.1f);

    koopaJumpTimer = createTimer(0.01f);
    kFireMoveTimer1 = createTimer(0.05f);
    kFireMoveTimer2 = createTimer(0.05f);
    kBallMoveTimer = createTimer(0.05f);

    koopaJumpCoolTimer = createTimer(4.0f);
    kFireCoolTimer1 = createTimer(2.0f);
    kFireCoolTimer2 = createTimer(2.0f + KFIRE_COOLTIME / 2);
    kBallCoolTimer = createTimer(3.0f);

    bgm_g5 = createSound("image/game5/쿠파성브금.mp3");

    jumpSound_g5 = createSound("sounds/공통/점프.mp3");

    playerFireSound1 = createSound("sounds/game5/마리오불공격1.wav");
    playerFireSound2 = createSound("sounds/game5/마리오불공격2.wav");
    koopaFireSound1 = createSound("sounds/game5/쿠파불공격1.mp3");
    koopaFireSound2 = createSound("sounds/game5/쿠파불공격2.mp3");
    koopaFireSound3 = createSound("sounds/game5/쿠파불공격3.mp3");

    koopaDamageSound1 = createSound("sounds/game5/쿠파데미지1.mp3");
    koopaDamageSound2 = createSound("sounds/game5/쿠파데미지2.mp3");
    playerDamageSound = createSound("sounds/game5/마리오데미지.mp3");
}