#include "systems/events.hpp"

static std::vector<DamageEvent> damageEvents;
static std::vector<HealEvent> healEvents;

void pushDamageEvent(DamageEvent event) {
   damageEvents.push_back(event);
}

void pushHealEvent(HealEvent event) {
   healEvents.push_back(event);
}

std::vector<DamageEvent> &getDamageEvents() {
   return damageEvents;
}

std::vector<HealEvent> &getHealEvents() {
   return healEvents;
}

void clearEvents() {
   damageEvents.clear();
   healEvents.clear();
}
