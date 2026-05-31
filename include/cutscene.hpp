#pragma once
#include <raylib.h>
#include <string>

struct CutsceneCommand {
   enum class Type: char {dialogue};

   Type type;
   size_t NPCID = 0;
   std::string dialogue;
};

void playCutscene(size_t cutsceneID);
bool isPlayingCutscene();

void updateCutscene();
void renderCutscene();
