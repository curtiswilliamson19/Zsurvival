#include "ZombieHealth.h"
#include "MainGame.h"

//Handles zombie health, calls function that removes zombie from vector and destroys object if health <= 0
void UpdateZombieHealth(int id)
{
	for (int i = 0; i < gameState.vZombies.size(); i++)
	{
		if (id == gameState.vZombies[i].getId())
		{
			if (gameState.vZombies[i].getHealth() > 0)
			{
				gameState.vZombies[i].decreaseHealth(50);
			}

			if (gameState.vZombies[i].getHealth() <= 0)
			{
				RemoveZombieFromVector(id);
				gameState.score += 100;
			}
		}
	}
}
