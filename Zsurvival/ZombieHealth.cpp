#include "ZombieHealth.h"
#include "MainGame.h"

void UpdateZombieHealth(int id)
{
	for (int i = 0; i < gameState.vZombies.size(); i++)
	{
		if (gameState.vZombies[i].getHealth() > 0)
		{
			gameState.vZombies[i].decreaseHealth(50);
		}

		if (gameState.vZombies[i].getHealth() <= 0)
		{
			RemoveZombieFromVector(id);
		}
	}
}
