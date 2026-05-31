#include "cutscene.hpp"
#include "asset.hpp"
#include "render.hpp"

static std::vector<Cutscene> cutscenes;
static size_t cutsceneID = 0;
static size_t cutsceneAction = 0;
static bool playingCutscene = false;

size_t getCutsceneCount() {
   return cutscenes.size();
}

size_t pushCutscene(Cutscene &cutscene) {
   cutscenes.push_back(cutscene);
   return cutscenes.size() - 1;
}

void playStartCutscene(std::vector<size_t> &cutscenes) {
   for (size_t index: cutscenes) {
      Cutscene &cutscene = ::cutscenes[index];
      if (cutscene.condition == Cutscene::Condition::start) {
         playingCutscene = (cutsceneID < cutscenes.size() && !cutscene.commands.empty());
         cutsceneID = index;
         break;
      }
   }
}

void playFinishCutscene(std::vector<size_t> &cutscenes) {
   for (size_t index: cutscenes) {
      Cutscene &cutscene = ::cutscenes[index];
      if (cutscene.condition == Cutscene::Condition::finish) {
         playingCutscene = (cutsceneID < cutscenes.size() && !cutscene.commands.empty());
         cutsceneID = index;
         break;
      }
   }
}

void playDeathCutscene(std::vector<size_t> &cutscenes) {
   for (size_t index: cutscenes) {
      Cutscene &cutscene = ::cutscenes[index];
      if (cutscene.condition == Cutscene::Condition::death) {
         playingCutscene = (cutsceneID < cutscenes.size() && !cutscene.commands.empty());
         cutsceneID = index;
         break;
      }
   }
}

void playCutscene(size_t cutsceneID) {
   ::cutsceneID = cutsceneID;
}

bool isPlayingCutscene() {
   return playingCutscene;
}

void updateCutscene() {
   if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      cutsceneAction += 1;

      if (cutsceneAction >= cutscenes[cutsceneID].commands.size()) {
         playingCutscene = false;
         cutsceneID = 0;
         cutsceneAction = 0;
      }
   }
}

void renderCutscene() {
   if (playingCutscene) {
      drawTextCentered(getFont("slackey"), getScreenCenter(), cutscenes[cutsceneID].commands[cutsceneAction].dialogue.c_str(), 35.0f, WHITE);
   }
}
