#pragma once
#include <raylib.h>
#include <string>
#include <vector>

struct CutsceneCommand {
   enum class Type: char {dialogue};

   Type type;
   size_t NPCID = 0;
   std::string dialogue;
};

struct Cutscene {
   enum class Condition: char {start, finish, death};

   Condition condition;
   std::vector<CutsceneCommand> commands;
};

bool isPlayingCutscene();
size_t getCutsceneCount();
size_t pushCutscene(Cutscene &cutscene);

void playStartCutscene(std::vector<size_t> &cutscenes);
void playFinishCutscene(std::vector<size_t> &cutscenes);
void playDeathCutscene(std::vector<size_t> &cutscenes);

void updateCutscene();
void renderCutscene();
