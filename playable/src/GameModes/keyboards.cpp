#include "GameModes/keyboards.h"    // Various keyboards

#include "global.h"                 // Globals
#include "GameModes/inputFunctions.h" // Various keyboard functions
#include "ErrorHandle/error.h"      // quit

static void none(unsigned char key) {if (key||true) return;}

/** To Add a Key Board: Create press and hold functions, add to getKeyboardFunct **/
void (*getKeyboardFunct(bool hold))(unsigned char) {
    switch (gameMode) {
        case startMode: return (hold)?       none:menuKeyPress;
        case simMode  : return (hold)? simKeyHold:simKeyPress;
        default       : return (hold)?       none:simKeyPress;
    }
}

#include <stdio.h>
void simKeyPress(unsigned char key) {

    switch(key) {
        case 'w': case 's': case 'a': case 'd': case 'e': case 'z': case 'q': // Allows usage on my second keyboard (Because using it presses f23 and causes issues
                keyStates[key] = true; simKeyHold(key); return;

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
//    printf("%c, %d", key, key);
    //if (key != '\0') quit(INPUT_ERROR);
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
        case 13: newGameMode(simMode); return; // enter
        case 'f': toggleFullScreen(); return;
        case 'l': toggleLights();     return;

        default: return;

    }
}













