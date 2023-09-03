#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
#include "MainGame.h"
#include "Blood.h"

//Handles drawing, animation and destruction of blood
void UpdateBlood()
{
	std::vector<int> vBlood = Play::CollectGameObjectIDsByType(TYPE_BLOOD);

	for (int id : vBlood)
	{
		GameObject& obj_blood = Play::GetGameObject(id);

		obj_blood.scale = 0.35f;

		Play::UpdateGameObject(obj_blood);

		Play::DrawObjectRotated(obj_blood);

		if (Play::IsAnimationComplete(obj_blood))
		{
			Play::DestroyGameObject(id);
		}
	}
}
