#include "state/loadingState.hpp"
#include "SRU/assets.hpp"
#include "SRU/audio.hpp"
#include "SRU/render.hpp"
#include "SRU/text.hpp"
#include "SRU/file.hpp"
#include "state/gameState.hpp"

constexpr float iconRotationSpeed = 360.0f; // degrees per second
constexpr float finalWaitTime = 1.0f;

LoadingState::LoadingState() {
   loadingText = "Loading Fonts... ";
   loadFont("slackey", "assets/fonts/slackey.ttf");
   loadTexture("loading", "assets/sprites/loading.png");
   getRandomLineFromFileInPlace(splash, "data/splash.txt");
}

State *LoadingState::change() {
   return new GameState();
}

void LoadingState::update() {
   iconRotation += DT * iconRotationSpeed;

   if (phase == Load::fonts) {
      loadFonts("assets/fonts");
      loadingText = "Loading Textures... ";
      phase = Load::textures;
   }
   else if (phase == Load::textures) {
      loadTextures("assets/sprites");
      loadingText = "Loading Shaders... ";
      phase = Load::shaders;
   }
   else if (phase == Load::shaders) {
      loadShaders("assets/shaders");
      loadingText = "Loading Sounds... ";
      phase = Load::sounds;
   }
   else if (phase == Load::sounds) {
      loadSounds("assets/sounds");
      loadingText = "Loading Game Data... ";
      phase = Load::data;
   }
   else if (phase == Load::data) {
      // loadData();
      // loadPlayerData();
      playSound("success");
      loadingText = "Loading Done!";
      phase = Load::count;
   }
   else if (phase == Load::count) {
      finalWaitTimer += DT;
      if (finalWaitTimer >= finalWaitTime) {
         quitState();
      }
   }
}

void LoadingState::render() {
   std::string finalLoadingText = loadingText;
   if (phase != Load::count) {
      finalLoadingText = TextFormat("%s%d/%d", loadingText.c_str(), (int)phase, (int)Load::count);
   }

   Font font = getFont("slackey");
   Texture icon = getTexture("loading");
   float splashFontSize = getFontSizeScaled(30.0f);
   float splashFontSpacing = fitSpacing(splashFontSize);

   drawTextCenteredResponsive(font, {0.5f, 0.375f}, finalLoadingText.c_str(), 60.0f);
   drawTextCenteredResponsive(font, {0.5f, 0.75f}, fitInside(splash, font, {GetScreenWidth() * 0.9f, GetScreenHeight() * 0.375f}, splashFontSize, splashFontSpacing).c_str(), 30.0f, GRAY);
   drawTextureCenteredResponsiveCubic(icon, {0.5f, 0.5f}, {0.1f, 0.1f}, WHITE, iconRotation);
}
