#pragma once
#include "data.hpp"

struct Enemy {
   void init(Rectangle bounds);
   void update(struct Map &map, Rectangle playerBounds, bool &playerDied); // fixedUpdate
   void render();

   EnemyAI ai;
   Rectangle bounds;
   TextureAA3 texture;
   Vector2 size;
   Vector2 position;
   Vector2 direction;
   float speed = 0.0f;
   bool collisionX = false;
   bool collisionY = false;
};
