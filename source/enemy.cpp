#include "enemy.hpp"
#include "map.hpp"
#include "math.hpp"
#include "player.hpp"
#include "render.hpp"
#include <raymath.h>

void Enemy::init(Rectangle bounds) {
   Vector2 origin = getOrigin(size);
   this->bounds = getRectangle(V2(bounds.x, bounds.y) + origin, V2(bounds.width, bounds.height) - size);

   switch (ai) {
   case EnemyAI::horizontal:
      direction.x = (chance(50) ? 1.0f : -1.0f);
      break;
   case EnemyAI::vertical:
      direction.y = (chance(50) ? 1.0f : -1.0f);
      break;
   case EnemyAI::diagonal:
      direction.x = (chance(50) ? 1.0f : -1.0f);
      direction.y = (chance(50) ? 1.0f : -1.0f);
      break;
   case EnemyAI::straight:
      break;
   }
}

void Enemy::update(Map &map, Rectangle playerBounds, bool &playerDied) {
   float directionX = 0.0f;
   float directionY = 0.0f;

   switch (ai) {
   case EnemyAI::horizontal:
      if (collisionX) {
         direction.x *= -1.0f;
      }
      directionX = direction.x;
      break;
   case EnemyAI::vertical:
      if (collisionY) {
         direction.y *= -1.0f;
      }
      directionY = direction.y;
      break;
   case EnemyAI::diagonal:
      if (collisionX) {
         direction.x *= -1.0f;
      }

      if (collisionY) {
         direction.y *= -1.0f;
      }
      directionX = direction.x;
      directionY = direction.y;
      break;
   case EnemyAI::straight:
      directionX = (playerBounds.x + playerBounds.width / 2.0f) - position.x;
      directionY = (playerBounds.y + playerBounds.height / 2.0f) - position.y;
      break;
   }

   collisionX = false;
   collisionY = false;
   direction = Vector2Normalize({directionX, directionY});

   if (direction.x != 0.0f) {
      position.x += direction.x * speed;
      if (position.x < bounds.x || position.x > bounds.x + bounds.width) {
         position.x = Clamp(position.x, bounds.x, bounds.x + bounds.width);
         collisionX = true;
      }

      Rectangle playerBounds = getRectangle(position - getOrigin(size), size);
      size_t startX = playerBounds.x / tileSize;
      size_t startY = playerBounds.y / tileSize;
      size_t endX = fmin(startX + 2, map.sizeX - 1);
      size_t endY = fmin(startY + 2, map.sizeY - 1);

      for (size_t y = startY; y <= endY; ++y)  {
         for (size_t x = startX; x <= endX; ++x) {
            Tile tile = map.tiles[y][x];
            if (tile.tileType == Tile::TileType::none || tile.type == Tile::Type::coin || tile.type == Tile::Type::finish) {
               continue;
            }

            Vector2 pos = V2(x, y);
            Vector2 size = V2(1.0f, 1.0f);

            if (tile.tileType == Tile::TileType::ghost) {
               pos = tile.rootPosition;
               tile = map.tiles[pos.y][pos.x];
               size = V2(tile.width, tile.height);
            }

            Rectangle tileBounds = getRectangle(pos * tileSize, size * tileSize);
            if (tile.type == Tile::Type::deadly) {
               tileBounds.x += deadlyTilePadding;
               tileBounds.y += deadlyTilePadding;
               tileBounds.width  -= deadlyTilePadding * 2;
               tileBounds.height -= deadlyTilePadding * 2;
            }

            if (CheckCollisionRecs(playerBounds, tileBounds)) {
               playerBounds.x = (tileBounds.x > playerBounds.x ? tileBounds.x - playerBounds.width : tileBounds.x + tileBounds.width);
               collisionX = true;
            }
         }
      }
      position.x = playerBounds.x + size.x / 2.0f;
   }

   if (direction.y != 0.0f) {
      position.y += direction.y * speed;
      if (position.y < bounds.y || position.y > bounds.y + bounds.height) {
         position.y = Clamp(position.y, bounds.y, bounds.y + bounds.height);
         collisionY = true;
      }

      Rectangle playerBounds = getRectangle(position - getOrigin(size), size);
      size_t startX = playerBounds.x / tileSize;
      size_t startY = playerBounds.y / tileSize;
      size_t endX = fmin(startX + 2, map.sizeX - 1);
      size_t endY = fmin(startY + 2, map.sizeY - 1);

      for (size_t y = startY; y <= endY; ++y)  {
         for (size_t x = startX; x <= endX; ++x) {
            Tile tile = map.tiles[y][x];
            if (tile.tileType == Tile::TileType::none || tile.type == Tile::Type::coin || tile.type == Tile::Type::finish) {
               continue;
            }

            Vector2 pos = V2(x, y);
            Vector2 size = V2(1.0f, 1.0f);

            if (tile.tileType == Tile::TileType::ghost) {
               pos = tile.rootPosition;
               tile = map.tiles[pos.y][pos.x];
               size = V2(tile.width, tile.height);
            }

            Rectangle tileBounds = getRectangle(pos * tileSize, size * tileSize);
            if (tile.type == Tile::Type::deadly) {
               tileBounds.x += deadlyTilePadding;
               tileBounds.y += deadlyTilePadding;
               tileBounds.width  -= deadlyTilePadding * 2;
               tileBounds.height -= deadlyTilePadding * 2;
            }

            if (CheckCollisionRecs(playerBounds, tileBounds)) {
               playerBounds.y = (tileBounds.y > playerBounds.y ? tileBounds.y - playerBounds.height : tileBounds.y + tileBounds.height);
               collisionY = true;
            }
         }
      }
      position.y = playerBounds.y + size.y / 2.0f;
   }

   if (CheckCollisionRecs(getRectangle(position - getOrigin(size), size), playerBounds)) {
      playerDied = true;
   }
}

void Enemy::render() {
   drawTextureAnimatedCentered(texture, position, size, WHITE);
}
