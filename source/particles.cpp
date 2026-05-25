#include "particles.hpp"
#include "asset.hpp"
#include "data.hpp"
#include "math.hpp"
#include "render.hpp"
#include "state.hpp"
#include <raymath.h>
#include <algorithm>
#include <vector>

std::vector<Particle> coinParticles;
std::vector<Particle> starParticles;
std::vector<Particle> confettiParticles;
std::vector<Particle> deathParticles;

void updateCluster(std::vector<Particle> &cluster) {
   for (Particle &particle: cluster) {
      particle.age += fixedUpdateDeltaTime;
      particle.position += particle.velocity * fixedUpdateDeltaTime;
      particle.velocity += particle.acceleration * fixedUpdateDeltaTime;
      particle.size += V2(particle.sizeVelocity * fixedUpdateDeltaTime);
      particle.rotation += particle.rotationVelocity * fixedUpdateDeltaTime;
   }

   cluster.erase(std::remove_if(cluster.begin(), cluster.end(), [](const Particle &p) -> bool {
      return p.age >= p.lifetime;
   }), cluster.end());
}

void updateParticles() {
   updateCluster(coinParticles);
   updateCluster(starParticles);
   updateCluster(confettiParticles);
   updateCluster(deathParticles);
}

void renderParticles(std::vector<Particle> &cluster) {
   for (Particle &particle: cluster) {
      Vector2 size = particle.size * getCubicRatio();
      if (particle.texture && particle.split != 0) {
         float unitX = (float)particle.texture->width / particle.split;
         float unitY = (float)particle.texture->height / particle.split;
         Rectangle source = {particle.splitX * unitX, particle.splitY * unitY, unitX, unitY};
         DrawTexturePro(*particle.texture, source, getRectangle(particle.position, size), getOrigin(size), particle.rotation, Fade(WHITE, 1.0f - particle.age / particle.lifetime));
      }
      else if (particle.texture) {
         DrawTexturePro(*particle.texture, getSource(*particle.texture), getRectangle(particle.position, size), getOrigin(size), particle.rotation, Fade(WHITE, 1.0f - particle.age / particle.lifetime));
      }
      else {
         Vector3 colorVector3 = getPlayerColor(particle.colorIndex) * 255.0f;
         Color colorTranslated = C4(colorVector3.x, colorVector3.y, colorVector3.z, (1.0f - particle.age / particle.lifetime) * 255.0f);
         DrawRectanglePro(getRectangle(particle.position, size), getOrigin(size), particle.rotation, colorTranslated);
      }
   }
}

void spawnParticles(const Particle &minimum, const Particle &maximum, size_t count, std::vector<Particle> &cluster) {
   cluster.reserve(cluster.size() + count);

   for (size_t i = 0; i < count; ++i) {
      Particle particle = {
         minimum.texture,
         randomV2(minimum.position, maximum.position),
         randomV2(minimum.velocity, maximum.velocity),
         randomV2(minimum.acceleration, maximum.acceleration),
         randomV2(minimum.size.x, maximum.size.x),
         randomFloat(minimum.sizeVelocity, maximum.sizeVelocity),
         randomFloat(minimum.rotation, maximum.rotation),
         randomFloat(minimum.rotationVelocity, maximum.rotationVelocity),
         randomFloat(minimum.lifetime, maximum.lifetime),
         0.0f,
         0,
         0,
         0,
         (minimum.texture ? 0 : rand() % getPlayerColorCount()),
      };
      cluster.push_back(particle);
   }
}

void spawnSplitParticles(const Particle &minimum, const Particle &maximum, size_t count, std::vector<Particle> &cluster) {
   cluster.reserve(cluster.size() + count);
   size_t split = std::sqrt(count);

   for (size_t i = 0; i < count; ++i) {
      Particle particle = {
         minimum.texture,
         randomV2(minimum.position, maximum.position),
         randomV2(minimum.velocity, maximum.velocity),
         randomV2(minimum.acceleration, maximum.acceleration),
         randomV2(minimum.size.x, maximum.size.x),
         randomFloat(minimum.sizeVelocity, maximum.sizeVelocity),
         randomFloat(minimum.rotation, maximum.rotation),
         randomFloat(minimum.rotationVelocity, maximum.rotationVelocity),
         randomFloat(minimum.lifetime, maximum.lifetime),
         0.0f,
         static_cast<int>(split),
         static_cast<int>(i % split),
         static_cast<int>(i / split),
         (minimum.texture ? 0 : rand() % getPlayerColorCount()),
      };
      cluster.push_back(particle);
   }
}

// presets

void spawnCoinParticles(const Vector2 &position, Texture *texture) {
   spawnParticles({
      texture,
      position - V2(5.0f) * getCubicRatio(),
      V2(-200.0f),
      V2(0.0f, 500.0f),
      V2(15.0f),
      0.95f,
      0.0f,
      -180.0f,
      0.6f
   }, {
      texture,
      position + V2(5.0f) * getCubicRatio(),
      V2(200.0f),
      V2(0.0f, 1000.0f),
      V2(20.0f),
      1.05f,
      360.0f,
      180.0f,
      0.8f
   }, 8, coinParticles);
}

void spawnStarParticles(const Vector2 &position) {
   float cr = getCubicRatio();
   spawnParticles({
      &getTexture("star"),
      position - V2(25.0f) * getCubicRatio(),
      V2(-500.0f) * cr,
      V2(0.0f, 500.0f) * cr,
      V2(25.0f),
      0.92f,
      0.0f,
      -180.0f,
      1.0f,
   }, {
      &getTexture("star"),
      position + V2(25.0f) * getCubicRatio(),
      V2(500.0f) * cr,
      V2(0.0f, 1000.0f) * cr,
      V2(35.0f),
      0.98f,
      360.0f,
      180.0f,
      2.0f,
   }, 25, starParticles);
}

void spawnConfetti() {
   float cr = getCubicRatio();
   spawnParticles({
      nullptr,
      V2(0.0f, -400.0f * getCubicRatio()),
      V2(-400.0f) * cr,
      V2(0.0f, 200.0f) * cr,
      V2(12.0f),
      0.92f,
      0.0f,
      -180.0f,
      6.0f,
   }, {
      nullptr,
      V2((float)GetScreenWidth(), 0.0f),
      V2(400.0f) * cr,
      V2(0.0f, 400.0f) * cr,
      V2(24.0f),
      0.98f,
      360.0f,
      180.0f,
      8.0f,
   }, 250, confettiParticles);
}

void spawnPlayerDeathParticles(const Vector2 &position, Texture *texture) {
   spawnSplitParticles({
      texture,
      position - V2(5.0f) * getCubicRatio(),
      V2(-300.0f),
      V2(0.0f, 500.0f),
      V2(10.0f),
      0.92f,
      0.0f,
      -180.0f,
      0.5f,
   }, {
      texture,
      position + V2(5.0f) * getCubicRatio(),
      V2(300.0f),
      V2(0.0f, 1000.0f),
      V2(15.0f),
      0.98f,
      360.0f,
      180.0f,
      0.7f
   }, 16, deathParticles);
}
