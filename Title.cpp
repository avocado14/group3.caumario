#include<bangtal.h>
#include <stdio.h>

SceneID titleScene;
ObjectID GameButton[6];

char path[256];

extern SceneID scene_g1, scene1_g2, scene_g4, scene_g6;

extern SoundID bgm_g6;

extern ObjectID createObject(const char* name, SceneID scene, int x, int y, bool shown, float size);

void Title_mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	
	if (object == GameButton[0])
		enterScene(scene_g1);

	else if (object == GameButton[1])
		enterScene(scene1_g2);
	
	else if (object == GameButton[3])
		enterScene(scene_g4);

	else if (object == GameButton[5]) {
		enterScene(scene_g6);
		playSound(bgm_g6);
	}

}


void Title_main() {

	titleScene = createScene("타이틀(임시)", "image/Title/배경.png");


	for (int i = 0; i < 6; i++) {
		sprintf_s(path, "image/Title/game%d.png", i+1);
		GameButton[i] = createObject(path, titleScene, 200 + 150 * i, 300, true, 1.3f);
	}
}