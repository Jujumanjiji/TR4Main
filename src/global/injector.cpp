#include "framework.h"
#include "injector.h"

injector g_inject;

///====================================///
///           EMPTY_FUNCTION           ///
///====================================///

void void_func(void)
{

}

///====================================///
///           INJECT_COMMAND           ///
///====================================///

void inject(DWORD addr, LPVOID function) // from: address, to: function
{
    if (addr != NULL)
    {
        ((JMP*)(addr))->opCode = 0xE9;
        ((JMP*)(addr))->offset = (DWORD)(function) - ((DWORD)(addr) + sizeof(JMP));
    }
}

void injector::inject_3dsystem()
{
    _3dsystem.inject_3d_gen();
    _3dsystem.inject_3d_gen_a();
}

void injector::inject_game()
{
    // principal function
    _game.inject_box();
    _game.inject_camera();
    _game.inject_collide();
    _game.inject_control();
    _game.inject_debris();
    _game.inject_debug();
    _game.inject_delstuff();
    _game.inject_deltapak();
    _game.inject_door();
    _game.inject_draw();
    _game.inject_effect2();
    _game.inject_effects();
    _game.inject_gameflow();
    _game.inject_hair();
    _game.inject_health();
    _game.inject_input();
    _game.inject_inventory();
    _game.inject_items();
    _game.inject_lara();
    _game.inject_lara1gun();
    _game.inject_lara2gun();
    _game.inject_larafire();
    _game.inject_laraflare();
    _game.inject_laramisc();
    _game.inject_larautils();
    _game.inject_lot();
    _game.inject_people();
    _game.inject_pickup();
    _game.inject_puzzle();
    _game.inject_rope();
    _game.inject_savegame();
    _game.inject_sound();
    _game.inject_sphere();
    _game.inject_switchs();
    _game.inject_tomb4fx();
    _game.inject_traps();

    // vehicles
    _game.inject_motorbike();

    // entities
    _game.inject_ahmet();
    _game.inject_baboon();
}

void injector::inject_specific()
{
    _specific.inject_setup();
}