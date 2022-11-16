#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

void UpdatePlayer() 
{

	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);
	obj_player.scale = 0.35f;
	Play::DrawObjectRotated(obj_player);
}
