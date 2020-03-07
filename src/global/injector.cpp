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

void injector::inject(ADDRESS_STRUCT addr) // from: address, to: function
{
    if (addr.address != NULL)
    {
        ((JMP*)(addr.address))->opCode = 0xE9;
        ((JMP*)(addr.address))->offset = (DWORD)(addr.function) - ((DWORD)(addr.address) + sizeof(JMP));
    }
}

///====================================///
///             3DSYSTEM               ///
///====================================///

void injector::inject_3dsystem()
{
    inject_3d_gen();                          // 0%
    inject_3d_gen_a();                        // 100%
}

///====================================///
///                GAME                ///
///====================================///

void injector::inject_game()
{
    // main:
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
    inject_health();
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
    inject_setup();
}
