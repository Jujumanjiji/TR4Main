#include "framework.h"
#include "loadsave.h"
#include "gameflow.h"
#include "input.h"
#include "output.h"
#include "specific.h"
#include "libgpu.h"
#include "timer.h"
#include "text.h"
#include "utils.h"

struct SAVEGAME_INFO /// maybe unaccurate since fog are included...
{
    LARA_INFO lara_data;
    int cutscene_triggered;
    char unknown1[10];
    char unknown2[10];
    char unknown3[10];
    BYTE current_level;
    bool unknown4;
    STATS level;
    STATS game;
    short weapon_back_object;
    short weapon_back_anim;
    short weapon_back_frame;
    short weapon_back_state_current;
    short weapon_back_state_next;
    CVECTOR nothing; // it's some fog value there !
    bool auto_target;
};

void WriteSG(char* pointer, int size)
{
    SGcount += size;
    for (; size > 0; size--)
        *SGpoint++ = *pointer++;
}

void ReadSG(char* pointer, int size)
{
    SGcount += size;
    for (; size > 0; size--)
        *pointer++ = *SGpoint++;
}

#ifdef DLL_INJECT
void injector::inject_loadsave()
{
    ///this->inject(0x0045A370, InitSaveGame);



    this->inject(0x0045B000, WriteSG);
    this->inject(0x0045BD80, ReadSG);
}
#endif