#include "framework.h"
#include "loadsave.h"
#include "inventory.h"
#include "gameflow.h"
#include "input.h"
#include "output.h"
#include "specific.h"
#include "libgpu.h"
#include "timer.h"
#include "text.h"
#include "utils.h"

/*
int LoadSaveThread(int key_pressed, int skipMonoScreen)
{
    if (!skipMonoScreen)
        CreateMonoScreen();

    int key, selected_id;
    in_inventory = 1;
    FoundLoadSaveCount();

    key = 0;
    while (true)
    {
        GPU_BeginScene();
        if (key)
            dbinput = IN_NONE;
        else
            S_UpdateInput();
        SetDebounce = 1;
        draw_outlines();
        selected_id = do_loadsave_mode(key_pressed);
        UpdatePulseColour();
        S_OutputPolyList();
        S_DumpScreen();

        if (selected_id >= 0)
        {
            if (key_pressed & IN_SAVE)
            {
                sgSaveGame();
                WriteSaveGame(selected_id);
                FoundLoadSaveCount(); // reset save count
                break;
            }
            else
            {
                key = selected_id + 1;
                ReadSaveGame(selected_id);
                SetFade(0, 255); // black to transparent
                selected_id = -1;
            }
        }

        if (key && FadeType == 2)
        {
            selected_id = key - 1;
            break;
        }
        else if (CHK_ANY(TrInput, IN_OPTION))
        {
            break;
        }
        else if (thread_started)
        {
            break;
        }
    }

    TIME_Init();
    if (!skipMonoScreen)
        RemoveMonoScreen();

    in_inventory = 0;
    return selected_id;
}

void sgRestoreGame(void)
{
    InitSaveGame(savegame_current_level & 0x7F, FALSE);
    GameTimer = savegame_level.timer;
    gfCurrentLevel = savegame_current_level & 0x7F;
    RestoreLevelData(TRUE);
    RestoreLaraData(TRUE);
}

int LoadGameInfo(void)
{
    return LoadSaveThread(IN_LOAD, TRUE) < 0 ? -1 : 1;
}

int SaveGameInfo(void)
{
    TrInput = IN_NONE;
    dbinput = IN_NONE;
    return LoadSaveThread(IN_SAVE, TRUE) < 0 ? -1 : 1;
}
*/

#ifdef DLL_INJECT
void injector::inject_loadsave()
{
    //this->inject(0x0047CD20, LoadSaveThread);
    //this->inject(0x0045B040, sgRestoreGame);
    //this->inject(0x0043B710, LoadGameInfo);
    //this->inject(0x0043B730, SaveGameInfo);
    //this->inject(0x0045B230, RestoreLevelData);
}
#endif