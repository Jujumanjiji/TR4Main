#include "framework.h"
#include "injector.h"

injector* inject;

///====================================///
///           EMPTY_FUNCTION           ///
///====================================///

void void_func(void)
{

}

///====================================///
///           INJECT_COMMAND           ///
///====================================///

void injector::inject(DWORD addr, LPVOID function) // from: address, to: function
{
    if (addr != NULL)
    {
        ((JMP*)(addr))->opCode = 0xE9;
        ((JMP*)(addr))->offset = (DWORD)(function) - ((DWORD)(addr) + sizeof(JMP));
    }
}

///====================================///
///             3DSYSTEM               ///
///====================================///

void injector::inject_3dsystem()
{
    inject_3d_gen();
    inject_3d_gen_a();
}

///====================================///
///                GAME                ///
///====================================///

void injector::inject_game()
{
    inject_box();
    inject_collide();
    inject_control();
    inject_debug();
    inject_delstuff();
    inject_deltapak();
    inject_door();
    inject_draw();
    inject_effect2();
    inject_effects();
    inject_hair();
    inject_health();
    inject_inventory();
    inject_items();
    inject_lara();
    inject_lara_utils();
    inject_lara1gun();
    inject_lara2gun();
    inject_larafire();
    inject_laraflare();
    inject_laramisc();
    inject_lot();
    inject_people();
    inject_pickup();
    inject_puzzle();
    inject_rope();
    inject_sound();
    inject_sphere();
    inject_switchs();
    inject_tomb4fx();
    inject_traps();
}

///====================================///
///             SPECIFIC               ///
///====================================///

void injector::inject_specific()
{
    inject_drawprimitive();
    inject_file();
    inject_loadsave();
    inject_setup();
}
