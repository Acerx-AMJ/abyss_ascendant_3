#pragma once
#include <vector>

struct DamageEvent {
   size_t entityID = 0;
   float damage = 0.0f;
};

struct HealEvent {
   size_t entityID = 0;
   float heal = 0.0f;
};

void pushDamageEvent(DamageEvent event);
void pushHealEvent(HealEvent event);

std::vector<DamageEvent> &getDamageEvents();
std::vector<HealEvent> &getHealEvents();

void clearEvents();
