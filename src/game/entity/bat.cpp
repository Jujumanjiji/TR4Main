#include "framework.h"
#include "bat.h"

void InitialiseBat(short itemNumber)
{
    
}

void BatControl(short itemNumber)
{
    
}

#ifdef DLL_INJECT
void injector::f_game::inject_bat()
{
    inject(0x00402A10, InitialiseBat);
    inject(0x00402A70, BatControl);
}
#endif
