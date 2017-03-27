#include "GameModes/MainMenu/menuInputFuncts.h"

#include "GameModes/mice.h" // Bound
#include "preproccessor.h" // simMode
#include "GameModes/inputFunctions.h" // newGameMode
#include "ErrorHandle/error.h"
#include "global.h"

void menuOptions() {
    int numOptions = 7;
    double ox = wx * 0.15;
    double oy = wy * 0.25;
    int lastx = 240;
    int letterHeight = 30;

    for (int j = 0; j < numOptions; j++) {
        int x1 = ox - lastx / 2  - 20;
        int y1 = (oy - letterHeight + (j + 1) * letterHeight - letterHeight) * 1.015;
        int x2 = ox + lastx / 2;
        int y2 = (oy - letterHeight + (j + 2) * letterHeight - letterHeight / 2) * 0.985;

        if (bound(mousePos.x, mousePos.y, x1, y1, x2, y2)) {
            switch(j) {
                case 0: newGameMode(simMode);break;
                case 1: break;
                case 2: break;
                case 3: break;
                case 4: break;
                case 5: break;
                case 6: quit(NORMAL_EXIT); break;
                default: break;
            }
            break;
        }
    }
    return;
}
