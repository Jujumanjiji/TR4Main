#pragma once

extern void void_func(void);

#ifdef DLL_INJECT
extern void inject(DWORD addr, LPVOID function);

class injector
{
private:
    struct f_3dsystem
    {
        void inject_3d_gen();
        void inject_3d_gen_a();
    };

    struct f_game
    {
        void inject_box();
        void inject_camera();
        void inject_collide();
        void inject_control();
        void inject_debris();
        void inject_debug();
        void inject_delstuff();
        void inject_deltapak();
        void inject_door();
        void inject_draw();
        void inject_effect2();
        void inject_effects();
        void inject_gameflow();
        void inject_hair();
        void inject_health();
        void inject_input();
        void inject_inventory();
        void inject_items();
        void inject_lara();
        void inject_larautils();
        void inject_lara1gun();
        void inject_lara2gun();
        void inject_larafire();
        void inject_laraflare();
        void inject_laramisc();
        void inject_lot();
        void inject_people();
        void inject_pickup();
        void inject_puzzle();
        void inject_rope();
        void inject_savegame();
        void inject_sound();
        void inject_sphere();
        void inject_switchs();
        void inject_tomb4fx();
        void inject_traps();

        /// vehicles
        void inject_motorbike();

        /// entities
        void inject_ahmet();
        void inject_baboon();
        void inject_bat();
        void inject_crocodile();
        void inject_mutant();
    };

    struct f_specific
    {
        void inject_setup();
    };

    f_3dsystem _3dsystem;
    f_game _game;
    f_specific _specific;
public:
    void inject_3dsystem();
    void inject_game();
    void inject_specific();
};

extern injector g_inject;
#endif