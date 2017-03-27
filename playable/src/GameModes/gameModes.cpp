#include "GameModes/gameModes.h"

#include "Glut/basic.h"              // KeyOperations
#include "global.h"                  // GameMode
#include "GameModes/Simulate/all.h"  // Mode
#include "GameModes/Editor/editor.h" // Mode
#include "GameModes/MainMenu/menu.h" // Mode
#include "GameModes/Mutation/mut.h"  // Mode

void renderScene(void) {
    callKeyboard('\0', true);
    switch (gameMode) {
        case startMode:
            startUpMode();
            return;
        case simMode:
            simulationMode();
            return;
        case editMode:
            editorMode();
            return;
        case mutMode:
            mutateMode();
            return;
        default : return;
    }
}















