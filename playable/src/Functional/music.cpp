#include "Functional/music.h"
#include <stdio.h>


    #include <windows.h>
    #include "mmsystem.h"              // Audio
    void playSong(const char * name, const char * alias) {
        char str[100] = "";
        sprintf(str, "open %s type mpegvideo alias %s", name, alias);

        MCIERROR me = mciSendString(str, NULL, 0, 0);

        sprintf(str, "play %s repeat", alias);
        if (!me) me =  mciSendString(str, NULL, 0, 0);
        return;
    }

    void endSong(const char * alias) {
        char str[100] = "";
        sprintf(str, "close %s", alias);

        mciSendString(str, NULL, 0, 0);
        return;
    }

//#else
//    void playSong(const char * name, const char * alias) {
//        if (name||alias||true) return;
//    }
//    void endSong(const char * alias) {
//        if (alias||true) return;
//    }
//
//#endif
