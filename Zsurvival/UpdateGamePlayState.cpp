#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Projectile.h"

void UpdateGamePlayState()
{
	GameObject& obj_player = Play::GetGameObjectByType(TYPE_PLAYER);

	switch (gameState.playState)
	{
	case STATE_START:
		gameState.playState = STATE_APPEAR;
		return;
	case STATE_APPEAR:
		Play::CreateGameObject(TYPE_PLAYER, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, 50, "survivor");
		MovePlayerSpriteOrigin();
		obj_player.pos = { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 };
		Play::SetSprite(obj_player, "survivor", 0);
		gameState.playState = STATE_PLAY;
		break;

	case STATE_PLAY:
		Play::DrawFontText("64px", "SCORE " + std::to_string((int)gameState.score), { DISPLAY_WIDTH - 70, 20 }, Play::CENTRE);
		Play::DrawFontText("64px", "AMMO " + std::to_string((int)player.getAmmo()), {55, 20}, Play::CENTRE);

		UpdatePlayer();
		UpdatePlayerMovement('A', 'D', 'W', 'S');
		UpdateZombie();
		UpdateZombieMovement();
		UpdateProjectile();
		UpdateAmmo();
		
		if (Play::KeyPressed(VK_DELETE))
		{
			gameState.playState = STATE_DEAD;
		}
		break;
	case STATE_DEAD:
		Play::DestroyGameObjectsByType(TYPE_ZOMBIE);
		Play::DestroyGameObjectsByType(TYPE_PLAYER);
		Play::DestroyGameObjectsByType(TYPE_AMMO);

		gameState.playState = STATE_WAIT;
		break;
	case STATE_WIN:
		gameState.playState = STATE_WAIT;
		break;
	case STATE_WAIT:
		Play::DrawFontText("64px", "GAME OVER", { DISPLAY_WIDTH / 2, 320 }, Play::CENTRE);
		Play::DrawFontText("64px", "HIGH SCORE: " + std::to_string((int)gameState.score), { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 }, Play::CENTRE);
		Play::DrawFontText("64px", "PRESS SPACE BAR TO PLAY AGAIN", { DISPLAY_WIDTH / 2, 400}, Play::CENTRE);

		if (Play::KeyPressed(VK_SPACE) == true)
		{
			gameState.playState = STATE_APPEAR;
			gameState.score = 0;
			player.setAmmo(10);
			gameState.vZombies.clear();
		}
		break;

	}

	UpdatePlayer();

}