#pragma once

#include <vector>

#include "Entity.h"

uint32_t EntityManager_Count();

void EntityManager_Add(const Entity &entity);
void EntityManager_Update();
void EntityManager_Draw(SpriteBatch &spriteBatch);