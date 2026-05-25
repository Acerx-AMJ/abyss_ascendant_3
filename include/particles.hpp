#pragma once
#include <raylib.h>
#include <vector>

struct Particle {
   Texture2D *texture;
   Vector2 position, velocity, acceleration;
   Vector2 size;
   float sizeVelocity = 1.0f;
   float rotation = 0.0f;
   float rotationVelocity = 0.0f;
   float lifetime = 0.0f;
   float age = 0.0f;
   int split = 0;
   int splitX = 0;
   int splitY = 0;
   size_t colorIndex = 0; // from player colors
};

extern std::vector<Particle> coinParticles;
extern std::vector<Particle> starParticles;
extern std::vector<Particle> confettiParticles;
extern std::vector<Particle> deathParticles;

void updateParticles();
void renderParticles(std::vector<Particle> &cluster);

// presets

void spawnCoinParticles(const Vector2 &position, Texture *texture);
void spawnStarParticles(const Vector2 &position);
void spawnConfetti();
void spawnPlayerDeathParticles(const Vector2 &position, Texture *texture);
