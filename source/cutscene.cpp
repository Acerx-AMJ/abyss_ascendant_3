#include "cutscene.hpp"
#include "asset.hpp"
#include "render.hpp"
#include <vector>

static std::vector<std::vector<CutsceneCommand>> cutscenes;
static size_t cutsceneID = 0;
static size_t cutsceneAction = 0;

void playCutscene(size_t cutsceneID) {
   cutscenes.push_back({});
   cutscenes.push_back({
      {CutsceneCommand::Type::dialogue, 0, "Hey! this is my dialogue"},
      {CutsceneCommand::Type::dialogue, 0, "this is message 2"},
      {CutsceneCommand::Type::dialogue, 0, "and this is my final message..."},
      {CutsceneCommand::Type::dialogue, 0, "stay safe"}
   });
   ::cutsceneID = cutsceneID;
}

bool isPlayingCutscene() {
   return cutsceneID != 0;
}

void updateCutscene() {
   if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      cutsceneAction += 1;

      if (cutsceneAction >= cutscenes[cutsceneID].size()) {
         cutsceneID = 0;
         cutsceneAction = 0;
      }
   }
}

void renderCutscene() {
   if (cutsceneID != 0 && cutsceneAction < cutscenes[cutsceneID].size()) {
      drawTextCentered(getFont("slackey"), getScreenCenter(), cutscenes[cutsceneID][cutsceneAction].dialogue.c_str(), 20.0f, WHITE);
   }
}
