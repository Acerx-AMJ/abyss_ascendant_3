#pragma once
#include "data.hpp"
#include "navig.hpp"
#include "state.hpp"

constexpr size_t levelButtonCount = chapterCount + 3;

struct MenuState: public State {
   MenuState();
   ~MenuState() = default;

   void updateResponsiveness() override;
   State *change() override;

   void update() override;
   void updateTitleState();
   void updateGameModeSelectionState();
   void updateLevelSelectionState();

   bool updateLevelButton(Text *button, const std::string &text, bool selected);

   void render() override;
   void renderTitleState();
   void renderGameModeSelectionState();
   void renderLevelSelectionState();

private:

   enum class Phase {title, gameModeSelection, levelSelection};
   Phase phase = Phase::title;

   Navigation titleButtons, gameModeSelectionButtons, levelButtons;
   Text *playButton, *editorButton, *customizeButton, *optionsButton, *quitButton, *endlessButton,
      *freePlayButton, *backButton;
   Text *lvlButtons[levelButtonCount];
   Text *chapterButtons[chapterCount];

   TextureAA3 starTexture, timerTexture;
   std::string chapterInfo;
   size_t chapterID = 0;
   bool chapterUnlocked = false;

   size_t levelID = 0;
   size_t levelIndex = 0;
   bool levelSelected = false;
   bool draggingScrollbar = false;
   Vector2 dragOffset;
   size_t dragStartIndex = 0;

   bool endlessSelected = false;
   bool freePlaySelected = false;
   bool shouldPlayLevel = false;
   bool shouldCustomize = false;
};
