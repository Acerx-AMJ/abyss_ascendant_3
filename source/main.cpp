#include "state/loadingState.hpp"
#include <raylib.h>

constexpr int minWindowWidth = 800;
constexpr int minWindowHeight = 600;

int main() {
   SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
   InitWindow(minWindowWidth, minWindowHeight, "Abyss Ascendant 3");
   SetWindowMinSize(minWindowWidth, minWindowHeight);
   InitAudioDevice();
   SetExitKey(KEY_NULL);
   SetTraceLogLevel(LOG_ERROR);

   State *current = new LoadingState();

   while (!WindowShouldClose()) {
      if (current->shouldChangeState()) {
         State *newState = current->change();
         delete current;
         current = newState;
      }

      if (!current) {
         break;
      }

      current->updateStateLogic();
      current->renderState();
   }

   CloseWindow();
   CloseAudioDevice();
}
