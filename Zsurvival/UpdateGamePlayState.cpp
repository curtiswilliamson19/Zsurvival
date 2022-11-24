#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"

void UpdateGamePlayState()
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	switch (gameState.playState)
	{
	case STATE_START:

		//for (int id_obj : Play::CollectAllGameObjectIDs())
			//Play::DestroyGameObject(id_obj);

		//Play::DestroyGameObjectsByType(TYPE_ZOMBIE);
		//Play::DestroyGameObjectsByType(TYPE_PLAYER);

		gameState.playState = STATE_APPEAR;
		return;
	case STATE_APPEAR:
		Play::CreateGameObject(TYPE_PLAYER, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 50, "survivor");
		obj_player.pos = { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 };
		Play::SetSprite(obj_player, "survivor", 0);
		gameState.playState = STATE_PLAY;
		break;

	case STATE_PLAY:

		UpdatePlayer();
		UpdatePlayerMovement('A', 'D', 'W', 'S');
		UpdateZombie();
		UpdateZombieMovement();

		if (Play::KeyPressed(VK_DELETE))
		{
			gameState.playState = STATE_DEAD;
		}
		break;
	case STATE_DEAD:
		Play::DestroyGameObjectsByType(TYPE_ZOMBIE);
		Play::DestroyGameObjectsByType(TYPE_PLAYER);
		gameState.playState = STATE_WAIT;
		break;
	case STATE_WIN:
		gameState.playState = STATE_WAIT;
		break;
	case STATE_WAIT:

		if (Play::KeyPressed(VK_SPACE) == true)
		{
			gameState.playState = STATE_START;
			gameState.score = 0;
		}
		break;

	} // End of switch

	UpdatePlayer();

}