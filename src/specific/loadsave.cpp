#include "framework.h"
#include "loadsave.h"
#include "text.h"

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

#ifdef DLL_INJECT
void injector::inject_loadsave()
{
    
}
#endif