#include<bangtal.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>


ObjectID createObject(const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	if (shown) {
		showObject(object);
	}
	return(object);
}