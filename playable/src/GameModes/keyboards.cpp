#include "GameModes/keyboards.h"    // Various keyboards

#include "global.h"                 // Globals
#include "GameModes/inputFunctions.h" // Various keyboard functions
#include "Generation/mutations.h"   // Mutation
#include "ErrorHandle/error.h"      // quit

static void none(unsigned char key) {if (key||true) return;}

/** To Add a Key Board: Create press and hold functions, add to getKeyboardFunct **/
void (*getKeyboardFunct(bool hold))(unsigned char) {
    switch (gameMode) {
        case startMode: return (hold)?       none:menuKeyPress;
        case simMode  : return (hold)? simKeyHold:simKeyPress;
        case mutMode  : return (hold)?       none:mutKeyPress;

        default       : return (hold)?       none:simKeyPress;
    }
}

void simKeyPress(unsigned char key) {
    switch(key) {
        case ESC: quit(NORMAL_EXIT);                  return;
        case 'f': toggleFullScreen();                 return;
        case 'i': toggleDisplay();                    return;

        case ' ': if (display) globalData[skipE].b = true;           return;
        case  13: if (display) globalData[goThroughGenE].b = true; setPlayBackSpeed(8192); return;
        case 'b': if (display) globalData[howToE].b = false;         return;
        case '1': if (display) globalData[instructionsE].b ^= true;  return;
        case '2': if (display) globalData[debugE].b ^= true;         return;
        case '3': if (display) globalData[graphE].b ^= true;         return;

        case ']': processSlowDown += 3; return;
        case '[': processSlowDown -= processSlowDown <= 0 ? 0 : 5; return;
        case '+': if (display) changePlayBack(true);  return;
        case '-': if (display) changePlayBack(false); return;
        case 'l': if (display) toggleLights();        return;
        case 'p': if (display) togglePerson();        return;
        case 'o': if (display) resetCamera();         return;
        default: return;
    }
}

void simKeyHold(unsigned char key) {
    if (key != '\0') quit(INPUT_ERROR);
    if (keyStates['w']) cameraMov.x =  playerSpeed;
    if (keyStates['s']) cameraMov.x = -playerSpeed;

    if (keyStates['a']) cameraMov.y = -playerSpeed;
    if (keyStates['d']) cameraMov.y =  playerSpeed;

    if (keyStates['e']) cameraMov.z =  playerSpeed;
    if (keyStates['z']) cameraMov.z = -playerSpeed; // 2 Ways to move down
    if (keyStates['q']) cameraMov.z = -playerSpeed;
    return;
}

void menuKeyPress(unsigned char key) {
    switch(key) {
        case ESC: quit(NORMAL_EXIT);  return;
        case 'b': newGameMode(simMode);  return;
        case 'f': toggleFullScreen(); return;
        case 'l': toggleLights();     return;

        default: return;

    }
}

void mutKeyPress(unsigned char key) {
    switch(key) {
        case ESC:              quit(NORMAL_EXIT);     return;
        case 'f':              toggleFullScreen();    return;
//        case 'i':              toggleDisplay();       return;

        case '+': if (display) changePlayBack(true);  return;
        case '-': if (display) changePlayBack(false); return;
        case 'l': if (display) toggleLights();        return;
        case 'p': if (display) togglePerson();        return;
        case 'o': if (display) resetCamera();         return;

        case '1': if (display) relocateNodes(   specimen[0].genome, 100);        return;
        case '2': if (display) shiftNodes(      specimen[0].genome, 100, 0.1);   return;
        case '3': if (display) addNode(         specimen[0].genome, 50, 20, 20); return;
        case '4': if (display) removeNode(      specimen[0].genome, 100);        return;
        case '5': if (display) addConnection(   specimen[0].genome, 100);        return;
        case '6': if (display) removeConnection(specimen[0].genome, 100);        return;
        case '7': if (display) swapConnection(  specimen[0].genome, 100);        return;

        default: return;
    }
}














