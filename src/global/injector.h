#pragma once

extern void void_func(void);

struct ADDRESS_STRUCT
{
    LPVOID function_old;
    LPVOID function_new;
    DWORD address;
    bool oldSelected;

    ADDRESS_STRUCT()
    {
        this->address = NULL;
        this->function_old = void_func;
        this->function_new = void_func;
        this->oldSelected = true;
    }

    // empty the function !
    ADDRESS_STRUCT(DWORD address)
    {
        this->address = address;
        this->function_old = void_func;
        this->function_new = void_func;
        this->oldSelected = true;
    }

    // replace the old function with the new one !
    ADDRESS_STRUCT(DWORD address, LPVOID function_new)
    {
        this->address = address;
        this->function_old = void_func;
        this->function_new = function_new;
        this->oldSelected = false;
    }

    // replace the old function and set the old for backup !
    ADDRESS_STRUCT(DWORD address, LPVOID function_old, LPVOID function_new, bool oldSelected)
    {
        this->address = address;
        this->function_old = function_old;
        this->function_new = function_new;
        this->oldSelected = oldSelected;
    }

    // replace the old with the new one !
    ADDRESS_STRUCT(ADDRESS_STRUCT addr_old, ADDRESS_STRUCT addr_new)
    {
        this->address = addr_old.address;
        this->function_old = addr_old.function_old;
        this->function_new = addr_new.function_new;
        this->oldSelected = false;
    }

    // replace the old with the new one !
    ADDRESS_STRUCT(ADDRESS_STRUCT addr_old, ADDRESS_STRUCT addr_new, bool oldSelected)
    {
        this->address = addr_old.address;
        this->function_old = addr_old.function_old;
        this->function_new = addr_new.function_new;
        this->oldSelected = false;
    }
};

#define __DEFFUNC(address, rtrn, argument) ((rtrn(__cdecl*)argument) address)
#define __FUNCV(address, argument) ADDRESS_STRUCT(address, __DEFFUNC(address, void, argument))
#define __FUNCR(address, rtrn, argument) ADDRESS_STRUCT(address, __DEFFUNC(address, rtrn, argument))
#define _FUNCV(address, old_args1, new1) ADDRESS_STRUCT(ADDRESS_STRUCT(address, __DEFFUNC(address, void, old_args1)), ADDRESS_STRUCT(NULL, new1))
#define _FUNCR(address, old_rtrn1, old_args1, new1) ADDRESS_STRUCT(ADDRESS_STRUCT(address, __DEFFUNC(address, old_rtrn1, old_args1)), ADDRESS_STRUCT(NULL, new1))

class injector
{
private:
    void inject(ADDRESS_STRUCT addr);

    /// 3DSYSTEM
    void inject_3d_gen();
    void inject_3d_gen_a();

    /// GAME
    void inject_box();
    void inject_collide();
    void inject_control();
    void inject_debug();
    void inject_delstuff();
    void inject_deltapak();
    void inject_door();
    void inject_draw();
    void inject_effect2();
    void inject_effects();
    void inject_health();
    void inject_items();
    void inject_lara();
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
    void inject_sound();
    void inject_sphere();
    void inject_switchs();
    void inject_tomb4fx();
    void inject_traps();

    /// SPECIFIC
    void inject_drawprimitive();
    void inject_file();
    void inject_setup();

public:
    void inject_3dsystem();
    void inject_game();
    void inject_specific();
};

extern injector* inject;