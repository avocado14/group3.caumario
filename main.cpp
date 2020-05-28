#include<bangtal.h>

extern SceneID titleScene;

extern void Title_main();
extern void Title_mouseCallback(ObjectID object, int x, int y, MouseAction action);

extern void Game1_main();
extern void Game1_mouseCallback(ObjectID object, int x, int y, MouseAction action);
extern void Game1_timerCallback(TimerID timer);
extern void Game1_soundCallback(SoundID sound);
extern void Game1_keyboardCallback(KeyCode code, KeyState state);

extern void Game2_main();
extern void Game2_mouseCallback(ObjectID object, int x, int y, MouseAction action);
extern void Game2_timerCallback(TimerID timer);

extern void Game4_main();
extern void Game4_mouseCallback(ObjectID object, int x, int y, MouseAction action);
extern void Game4_timerCallback(TimerID timer);
extern void Game4_soundCallback(SoundID sound);

extern void Game6_main();
extern void Game6_mouseCallback(ObjectID object, int x, int y, MouseAction action);
extern void Game6_timerCallback(TimerID timer);
extern void Game6_keyboardCallback(KeyCode code, KeyState state);



void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	
	Title_mouseCallback(object, x, y, action);
	Game1_mouseCallback(object, x, y, action);
	Game2_mouseCallback(object, x, y, action);
	Game4_mouseCallback(object, x, y, action);
	Game6_mouseCallback(object, x, y, action);

}

void timerCallback(TimerID timer) {
	
	Game1_timerCallback(timer);
	Game2_timerCallback(timer);
	Game4_timerCallback(timer);
	Game6_timerCallback(timer);

}

void soundCallback(SoundID sound) {

	Game1_soundCallback(sound);
	Game4_soundCallback(sound);

}

void keyboardCallback(KeyCode code, KeyState state) {

	Game1_keyboardCallback(code, state);
	Game6_keyboardCallback(code, state);

}


int main() {

	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);
	setSoundCallback(soundCallback);
	setKeyboardCallback(keyboardCallback);


	Title_main();
	Game1_main();
	Game2_main();
	Game4_main();
	Game6_main();


	startGame(titleScene);

}