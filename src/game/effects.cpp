#include "framework.h"
#include "effects.h"
#include "control.h"

void Splash(ITEM_INFO* item)
{
    int x, y, z;
    int wh;
    short room_number;

    x = item->pos.xPos;
    y = item->pos.yPos;
    z = item->pos.zPos;
    room_number = item->roomNumber;
    GetFloor(x, y, z, &room_number);
    if (RWATER(room_number))
    {
        wh = GetWaterHeight(x, y, z, room_number);
        splash_setup.y = wh;
        splash_setup.x = x;
        splash_setup.z = z;
        splash_setup.inner_rad = 32;
        splash_setup.inner_size = 8;
        splash_setup.inner_radvel = 320;
        splash_setup.inner_yvel = -item->fallspeed * 40;
        splash_setup.inner_y = 48;
        splash_setup.middle_rad = 32;
        splash_setup.middle_size = 480;
        splash_setup.middle_radvel = -item->fallspeed * 20;
        splash_setup.middle_yvel = 32;
        splash_setup.middle_y = 128;
        splash_setup.outer_rad = 544;
        SetupSplash(&splash_setup);
    }
}

#ifdef DLL_INJECT
void injector::f_game::inject_effects()
{
    inject(0x004372A0, Splash);
}
#endif
