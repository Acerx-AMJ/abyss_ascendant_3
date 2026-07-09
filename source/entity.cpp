#include "entities/entity.hpp"
#include "SRU/assets.hpp"
#include "SRU/random.hpp"
#include "SRU/render.hpp"
#include "SRU/util.hpp"
#include "systems/events.hpp"
#include <algorithm>

static std::vector<Entity> entities;

void spawnEntity(Entity entity) {
   entity.ID = entities.size();
   entities.push_back(entity);
}

void killEntities() {
   entities.clear();
}

void updateEntity(Entity &entity) {
   switch (entity.type) {
   case EntityType::enemy:
      entity.direction = Vector2Lerp(entity.direction, randomV2(-1.0f, 1.0f), 0.1f);
      entity.position += Vector2Normalize(entity.direction) * entity.speed * GetFrameTime();
      entity.position = Vector2Clamp(entity.position, V2(), getWindowSize() - entity.size);
      entity.flagForDeletion = entity.died;
      break;
   case EntityType::player: {
      float directionX = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
      float directionY = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
      entity.direction = Vector2Normalize({directionX, directionY});
      entity.position += entity.direction * entity.speed * GetFrameTime();
      entity.position = Vector2Clamp(entity.position, V2(), getWindowSize() - entity.size);
      entity.flagForDeletion = entity.died;
      break;
   }}
}

void updateEntities() {
   for (Entity &entity: entities) {
      updateEntity(entity);
   }

   entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity &e) -> bool {
      return e.flagForDeletion;
   }), entities.end());

   for (DamageEvent &event: getDamageEvents()) {
      entities[event.entityID].health -= event.damage;
   }

   for (HealEvent &event: getHealEvents()) {
      entities[event.entityID].health += event.heal;
   }

   for (Entity &entity: entities) {
      if (entity.health <= 0.0f) {
         entity.died = true;
      }
   }
}

void renderEntity(Entity &entity) {
   switch (entity.type) {
   case EntityType::enemy:
      drawTexture(getTexture("lock"), entity.position, entity.size, entity.color);
      break;
   case EntityType::player:
      drawTexture(getTexture("player1"), entity.position, entity.size, entity.color);
      break;
   }
}

void renderEntities() {
   for (Entity &entity: entities) {
      renderEntity(entity);
   }
}

Entity &getEntity(size_t ID) {
   return entities[ID];
}

std::vector<Entity> &getEntityContainer() {
   return entities;
}
