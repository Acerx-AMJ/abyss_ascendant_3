#include "menu_state.hpp"
#include "asset.hpp"
#include "customize_state.hpp"
#include "file.hpp"
#include "game_state.hpp"
#include "input.hpp"
#include <math.h>

MenuState::MenuState() {
   Font font = getFont("slackey");

   playButton = Text::make(font, "PLAY", 50.0f),
   editorButton = Text::make(font, "EDITOR", 50.0f),
   customizeButton = Text::make(font, "CUSTOMIZE", 50.0f),
   optionsButton = Text::make(font, "OPTIONS", 50.0f);
   quitButton = Text::make(font, "QUIT", 50.0f);
   titleButtons.addElements({playButton, editorButton, customizeButton, optionsButton, quitButton});

   for (size_t i = 0; i < chapterCount; ++i) {
      if (i != 0 && !getChapterData(i - 1).completed) {
         chapterButtons[i] = Text::make(font, toRomanNumeral(i+1) + ": ???", 50.0f);
         chapterButtons[i]->disabled = true;
      }
      else {
         chapterButtons[i] = Text::make(font, toRomanNumeral(i+1) + ": " + getChapterCodeName(i), 50.0f);
      }
   }

   endlessButton = Text::make(font, "ENDLESS", 50.0f);
   freePlayButton = Text::make(font, "FREE-PLAY", 50.0f);
   backButton = Text::make(font, "BACK", 50.0f);
   gameModeSelectionButtons.addElements({chapterButtons[0], chapterButtons[1], chapterButtons[2],
      chapterButtons[3], chapterButtons[4], chapterButtons[5], chapterButtons[6], endlessButton,
      freePlayButton, backButton});

   for (size_t i = 0; i < levelButtonCount; ++i) {
      lvlButtons[i] = Text::make(font, "", 50.0f);
   }

   levelButtons.addElements({lvlButtons[0], lvlButtons[1], lvlButtons[2], lvlButtons[3], lvlButtons[4],
      lvlButtons[5], lvlButtons[6], lvlButtons[7], lvlButtons[8], lvlButtons[9]});

   initAnimationIfExists(starTexture, "star");
   initAnimationIfExists(timerTexture, "timer");

   updateResponsiveness();
}

void MenuState::updateResponsiveness() {
   float cr = getCubicRatio();

   playButton->position = {cr * 125.0f, cr * 300.0f};
   editorButton->position = {cr * 125.0f, cr * 400.0f};
   customizeButton->position = {cr * 125.0f, cr * 500.0f};
   optionsButton->position = {cr * 125.0f, cr * 600.0f};
   quitButton->position = {cr * 125.0f, cr * 700.0f};

   for (size_t i = 0; i < chapterCount; ++i) {
      chapterButtons[i]->position = {cr * 125.0f, cr * (225.0f + 75.0f * i)};
   }

   endlessButton->position = {cr * 125.0f, cr * 750.0f};
   freePlayButton->position = {cr * 125.0f, cr * 825.0f};
   backButton->position = {cr * 125.0f, cr * 900.0f};

   for (size_t i = 0; i < levelButtonCount; ++i) {
      lvlButtons[i]->position = {cr * 125.0f, cr * (225.0f + 75.0f * i)};
   }
}

State *MenuState::change() {
   if (shouldCustomize) {
      return new CustomizeState();
   }
   else if (shouldPlayLevel) {
      GameState *game = new GameState();
      game->setup(getLevel(levelID));
      return game;
   }

   return nullptr;
}

// Update

void MenuState::update() {
   switch (phase) {
   case Phase::title:             updateTitleState();             break;
   case Phase::gameModeSelection: updateGameModeSelectionState(); break;
   case Phase::levelSelection:    updateLevelSelectionState();    break;
   }
}

void MenuState::updateTitleState() {
   titleButtons.update();

   if (playButton->clicked) {
      phase = Phase::gameModeSelection;
   }

   if (editorButton->clicked) {

   }

   if (customizeButton->clicked) {
      shouldCustomize = true;
      fadingOut = true;
   }

   if (optionsButton->clicked) {

   }

   if (quitButton->clicked) {
      fadingOut = true;
   }
}

void MenuState::updateGameModeSelectionState() {
   gameModeSelectionButtons.update();

   for (size_t i = 0; i < chapterCount; ++i) {
      if (chapterButtons[i]->hovering) {
         chapterInfo = chapterButtons[i]->text;
         chapterUnlocked = !chapterButtons[i]->disabled;
         chapterID = i;
      }

      if (chapterButtons[i]->clicked) {
         chapterID = i;
         phase = Phase::levelSelection;
         levelSelected = false;
         levelIndex = 0;
         levelButtons.index = 0;
         return;
      }
   }
   endlessSelected = false;
   freePlaySelected = false;

   if (gameModeSelectionButtons.anySelected() && gameModeSelectionButtons.index <= chapterCount) {
      size_t i = gameModeSelectionButtons.index - 1;
      chapterInfo = chapterButtons[i]->text;
      chapterUnlocked = !chapterButtons[i]->disabled;
      chapterID = i;
   }
   else if (endlessButton->hovering) {
      chapterInfo = endlessButton->text;
      endlessSelected = true;
      chapterUnlocked = false;
   }
   else if (freePlayButton->hovering) {
      chapterInfo = freePlayButton->text;
      freePlaySelected = true;
      chapterUnlocked = false;
   }

   if (endlessButton->clicked) {

   }

   if (freePlayButton->clicked) {

   }

   if (backButton->clicked || handleKeyPressWithSound(KEY_ESCAPE)) {
      phase = Phase::title;
   }
}

void MenuState::updateLevelSelectionState() {
   ChapterData &data = getChapterData(chapterID);
   size_t last = std::min(levelButtonCount, data.levels.size() + 1);
   levelButtons.updateKey(levelButtons.up);
   levelButtons.updateKey(levelButtons.down);
   levelButtons.updateKey(levelButtons.tab);

   if (levelButtons.up.pressed || (levelButtons.tab.pressed && (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)))) {
      levelButtons.index = (levelButtons.index == 0 ? last : levelButtons.index - 1);

      if (levelButtons.index == 1 && levelIndex > 0) {
         levelButtons.index += 1;
         levelIndex -= 1;
      }

      if (levelButtons.index == last) {
         levelIndex = (data.levels.size() < levelButtonCount ? 0 : data.levels.size() - levelButtonCount + 1);
      }
   }

   if (levelButtons.down.pressed || (levelButtons.tab.pressed && (!IsKeyDown(KEY_LEFT_SHIFT) && !IsKeyDown(KEY_RIGHT_SHIFT)))) {
      levelButtons.index = (levelButtons.index + 1) % (last + 1);
   
      if (levelButtons.index == last && levelIndex + levelButtonCount - 1 < data.levels.size()) {
         levelButtons.index -= 1;
         levelIndex += 1;
      }

      if (levelButtons.index == 1) {
         levelIndex = 0;
      }
   }

   size_t selectedButton = levelButtonCount;
   size_t hoveredButton = levelButtonCount;

   for (size_t i = 0; i + levelIndex <= data.levels.size() && i < levelButtonCount; ++i) {
      Text *button = lvlButtons[i];
      bool selected = (i + 1 == levelButtons.index);

      if (i + levelIndex == data.levels.size()) {
         if (updateLevelButton(button, "BACK", selected)) {
            phase = Phase::gameModeSelection;
         }
         continue;
      }
      Level &level = getLevel(data.levels[levelIndex + i]);
      button->ID = level.ID;
      if (updateLevelButton(button, level.name, selected)) {
         shouldPlayLevel = true;
         levelID = level.ID;
         fadingOut = true;
      }

      if (selected) {
         selectedButton = i;
      }

      if (button->hovering) {
         hoveredButton = i;
      }
   }

   if (selectedButton != levelButtonCount) {
      levelID = lvlButtons[selectedButton]->ID;
      levelSelected = true;
   }
   else if (hoveredButton != levelButtonCount) {
      levelID = lvlButtons[hoveredButton]->ID;
      levelSelected = true;
   }

   float scroll = GetMouseWheelMove();
   if (scroll >= 0.5f && levelIndex > 0) {
      levelIndex -= 1;
   }
   else if (scroll <= -0.5f && levelIndex + levelButtonCount <= data.levels.size()) {
      levelIndex += 1;
   }

   if (handleKeyPressWithSound(KEY_ESCAPE)) {
      phase = Phase::gameModeSelection;
   }
}

bool MenuState::updateLevelButton(Text *button, const std::string &text, bool selected) {
   button->text = text;
   if (selected) {
      button->update(true, IsKeyDown(KEY_ENTER), IsKeyPressed(KEY_ENTER));
   }
   else {
      button->update();
   }
   return button->clicked;
}

// Render

void MenuState::render() {
   switch (phase) {
   case Phase::title:             renderTitleState();             break;
   case Phase::gameModeSelection: renderGameModeSelectionState(); break;
   case Phase::levelSelection:    renderLevelSelectionState();    break;
   }
}

void MenuState::renderTitleState() {
   Font font = getFont("slackey");
   float cr = getCubicRatio();
   
   drawTextButtonBackground(600.0f, RED);
   drawTextSemiCentered(font, {cr * 25.0f, cr * 100.0f}, "      ABYSS\n ASCENDANT 3", 80.0f, WHITE);
   titleButtons.render();

   if (titleButtons.anySelected()) {
      drawPointer(getTexture("lotus"), titleButtons.getSelectedElement()->position, true);
   }
}

void MenuState::renderGameModeSelectionState() {
   Font font = getFont("slackey");
   float cr = getCubicRatio();

   drawTextButtonBackground(600.0f, RED);
   drawTextSemiCentered(font, {cr * 25.0f, cr * 100.0f}, "  SELECT GAME\n       MODE", 80.0f, WHITE);
   gameModeSelectionButtons.render();

   drawTextCentered(font, {cr * 1260.0f, cr * 150.0f}, chapterInfo.c_str(), 80.0f, WHITE);
   if (!endlessSelected && !freePlaySelected && chapterUnlocked) {
      ChapterData &data = getChapterData(chapterID);
      
      drawTextureAnimatedCentered(starTexture, {cr * 1000.0f, cr * 300.0f}, cubicSize(50.0f), WHITE);
      drawTextSemiCentered(font, {cr * 1040.0f, cr * 300.0f}, (data.completed ? TextFormat("%d/%d", data.starCount, data.levels.size() * 3) : TextFormat("%d/???", data.starCount)), 50.0f, WHITE);

      drawTextureAnimatedCentered(timerTexture, {cr * 1440.0f, cr * 300.0f}, cubicSize(50.0f), WHITE);
      drawTextSemiCentered(font, {cr * 1480.0f, cr * 300.0f}, (data.totalTime != std::numeric_limits<float>::max() ? TextFormat("%05.2f", data.totalTime) : "--.--"), 50.0f, WHITE);
   }

   if (gameModeSelectionButtons.anySelected()) {
      drawPointer(getTexture("lotus"), gameModeSelectionButtons.getSelectedElement()->position, true);
   }
}

void MenuState::renderLevelSelectionState() {
   ChapterData &data = getChapterData(chapterID);
   Font font = getFont("slackey");
   float cr = getCubicRatio();

   drawTextButtonBackground(550.0f, RED);

   float oneValue = GetScreenHeight() / float(data.levels.size() + 1);
   float offset = levelIndex * oneValue;
   float visible = fmin(data.levels.size() + 1, levelButtonCount) * oneValue;
   DrawRectangleV({550.0f * cr, 0.0f}, {50.0f * cr, 1.0f * GetScreenHeight()}, Fade(RED, 0.75f));
   DrawRectangleV({550.0f * cr, offset}, {50.0f * cr, visible}, WHITE);

   drawTextSemiCentered(font, {cr * 25.0f, cr * 100.0f}, "  SELECT LEVEL", 80.0f, WHITE);

   for (size_t i = 0; i + levelIndex <= data.levels.size() && i < levelButtonCount; ++i) {
      lvlButtons[i]->render();
   }

   if (levelSelected) {
      Level &level = getLevel(levelID);
      LevelData data = getLevelData(levelID);
      drawTextCentered(font, {cr * 1260.0f, cr * 150.0f}, level.name.c_str(), 80.0f, WHITE);

      drawTextureAnimatedCentered(starTexture, {cr * 1000.0f, cr * 300.0f}, cubicSize(50.0f), WHITE);
      drawTextSemiCentered(font, {cr * 1040.0f, cr * 300.0f}, TextFormat("%d/%d%c", data.stars, 3, (data.perfect ? '!' : '\0')), 50.0f, WHITE);

      drawTextureAnimatedCentered(timerTexture, {cr * 1440.0f, cr * 300.0f}, cubicSize(50.0f), WHITE);
      drawTextSemiCentered(font, {cr * 1480.0f, cr * 300.0f}, (data.time != std::numeric_limits<float>::max() ? TextFormat("%05.2f", data.time) : "--.--"), 50.0f, WHITE);
   }

   if (levelButtons.anySelected()) {
      drawPointer(getTexture("lotus"), levelButtons.getSelectedElement()->position, true);
   }
}
