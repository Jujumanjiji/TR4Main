#pragma once

#pragma pack(push, 1)
typedef struct {
    BYTE opCode;	// must be 0xE9;
    DWORD offset;	// jump offset
} JMP;
#pragma pack(pop)

class injector
{
private:
    void inject(DWORD from, LPVOID to);

    /// 3DSYSTEM
    void inject_3d_gen();
    void inject_3d_gen_a();

    /// GAME
	void inject_box();
    void inject_draw();
	void inject_lot();
	void inject_sound();

    /// SPECIFIC
	void inject_drawprimitive();
	void inject_file();
    void inject_setup();

	/// OTHER
	void inject_entity();

public:
    void inject_3dsystem();
    void inject_game();
    void inject_specific();
};

extern injector* inject;