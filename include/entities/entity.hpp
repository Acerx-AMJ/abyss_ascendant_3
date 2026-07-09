#pragma once
#include "raylib.h"
#include <vector>

enum class EntityType: unsigned char {
   player, enemy
};

struct Entity {
   EntityType type;
   size_t ID = 0;

   Color color = WHITE;
   Vector2 position;
   Vector2 size;
   Vector2 direction;

   float speed = 0.0f;
   float health = 0.0f;

   bool died = false;
   bool flagForDeletion = false;
};

void spawnEntity(Entity entity);
void killEntities();

void updateEntity(Entity &entity);
void updateEntities();

void renderEntity(Entity &entity);
void renderEntities();

Entity &getEntity(size_t ID);
std::vector<Entity> &getEntityContainer();
