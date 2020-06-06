#include "framework.h"
#include "crocodile.h"

enum CROCODILE_STATE
{
    CROC_NULL,
    CROC_IDLE,
    CROC_RUN,
    CROC_WALK,
    CROC_HIT,
    CROC_ATK,
    CROC_EMPTY1,
    CROC_DIE,
    WCROC_SWIM,
    WCROC_ATK,
    WCROC_DIE,
};

#ifdef DLL_INJECT
void injector::f_game::inject_crocodile()
{

}
#endif
