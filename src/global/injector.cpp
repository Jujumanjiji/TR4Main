#include "framework.h"
#include "injector.h"

injector* inject;

///====================================///
///           INJECT_COMMAND           ///
///====================================///

void injector::inject(DWORD from, LPVOID to)
{
    if (from != NULL)
    {
        ((JMP*)(from))->opCode = 0xE9;
        ((JMP*)(from))->offset = (DWORD)(to)-((DWORD)(from)+sizeof(JMP));
    }
}

///====================================///
///             3DSYSTEM               ///
///====================================///

void injector::inject_3dsystem()
{
    this->inject_3d_gen();                          // 0%
    this->inject_3d_gen_a();                        // 100%
}

///====================================///
///                GAME                ///
///====================================///

void injector::inject_game()
{
	// other:
	this->inject_entity();

	// main:
	this->inject_box();
	this->inject_draw();
}

///====================================///
///             SPECIFIC               ///
///====================================///

void injector::inject_specific()
{
	this->inject_drawprimitive();
	this->inject_file();
    this->inject_setup();
}
