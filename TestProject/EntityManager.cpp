#include "EntityManager.h"

#include <algorithm>

std::vector<Entity> EntityManager_Entities;

bool EntityManager_IsUpdating = false;
std::vector<Entity> EntityManager_AddedEntities;

uint32_t EntityManager_Count()
{
	return (uint32_t)EntityManager_Entities.size();
}


void EntityManager_Add(const Entity &entity)
{
	if (!EntityManager_IsUpdating)
		EntityManager_Entities.push_back(entity);
	else
		EntityManager_AddedEntities.push_back(entity);
}

void EntityManager_Update()
{
	EntityManager_IsUpdating = true;

	for (Entity &entity : EntityManager_Entities)
		entity.update();

	EntityManager_IsUpdating = false;

	for (const Entity &entity : EntityManager_AddedEntities)
		EntityManager_Entities.push_back(entity);

	EntityManager_AddedEntities.clear();

	// remove any expired entities
	//EntityManager_Entities.erase(std::remove_if(EntityManager_Entities.begin(), EntityManager_Entities.end(),
	//[](const Entity &entity)
	//{
	//	return entity.isExpired;
	//}), EntityManager_Entities.end());
}

void EntityManager_Draw(SpriteBatch &spriteBatch)
{

}