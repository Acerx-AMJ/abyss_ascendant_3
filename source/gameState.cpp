#include "state/gameState.hpp"
#include "SRU/random.hpp"
#include "SRU/render.hpp"
#include "SRU/util.hpp"
#include "entities/entity.hpp"
#include "systems/events.hpp"

GameState::GameState() {
   killEntities();
   spawnEntity({EntityType::enemy, 0, WHITE, randomV2(V2(), getWindowSize()), V2(50.0f), V2(), 100.0f, 100.0f});
   spawnEntity({EntityType::enemy, 0, WHITE, randomV2(V2(), getWindowSize()), V2(50.0f), V2(), 100.0f, 100.0f});
   spawnEntity({EntityType::enemy, 0, WHITE, randomV2(V2(), getWindowSize()), V2(50.0f), V2(), 100.0f, 100.0f});
   spawnEntity({EntityType::enemy, 0, WHITE, randomV2(V2(), getWindowSize()), V2(50.0f), V2(), 100.0f, 100.0f});
   spawnEntity({EntityType::enemy, 0, WHITE, randomV2(V2(), getWindowSize()), V2(50.0f), V2(), 100.0f, 100.0f});
   spawnEntity({EntityType::player, 0, WHITE, randomV2(V2(), getWindowSize()), V2(50.0f), V2(), 150.0f, 100.0f});
}

State *GameState::change() {
   return nullptr;
}

void GameState::update() {
   updateEntities();
   clearEvents();
}

void GameState::render() {
   renderEntities();
}
