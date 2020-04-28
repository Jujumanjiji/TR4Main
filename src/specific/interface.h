#pragma once

#define MAX_BUTTON 10
#define MAX_PAGE 6
#define NO_BUTTON -1
#define NO_LEVEL (char*)"-- No Level --"

#define memalloc(src, t, s) src = (t*)malloc(sizeof(t) * s);
#define meminit(src, t) memset(&src, 0, sizeof(t));
#define memfree(src) \
if (src != nullptr) \
{\
    free(src);\
    src = nullptr;\
}

#define CCLAMP(current, min, max, ret) if (current >= min && current <= max) return ret

enum MENU_ID
{
    M_TITLE,
    M_SELECT_LEVEL,
    M_LOAD,
    M_SAVE,
    M_OPTION,
    M_STATISTICS,
    M_PAUSE,
    MAX_GUI
};

enum TITLE_SEQUENCE_TYPE
{
    TS_IDLE,
    TS_SELECT_LEVEL,
    TS_LOAD,
    TS_OPTION,
};

enum MENU_FLAGS
{
    TK_RED = 1,
    TK_ADD = 2,
    TK_DEL = 4
};

typedef void(*do_draw)(void);
typedef int(*do_input)(void);

struct button_array
{
    int x;
    int y;
    char* str;
    unsigned short ff_flags;
    bool is_red;
    bool is_locked;  // cant move up or down if this one is defined !
    bool bypass_red; // only to change page !!!
    bool is_enabled;
};

struct button_struct
{
    button_array arrays[MAX_BUTTON];
};

struct gui_menu
{
    int32 current;
    int32 old;
    int32 next;
    int32 next_ignore; // ignore a certain button ?
    int32 initial;
    bool is_initial;
    /// max button in this gui !
    int32 min;
    int32 max;                     
    /// to check if this current gui have more button than max ! (normally used to switch between page !)
    int32 bprevious, bnext;
    int32 current_page;
    int32 default_x;
    int32 default_y;
    button_struct button[MAX_PAGE]; // contains all button !
    do_draw draw;
    do_input input;
};

class GAME_GUI
{
private:
    MENU_ID gui_front; // current
    MENU_ID gui_back;  // old
    gui_menu gui[MAX_GUI];

    int multiply_pos(int from, int to);
    int font_flags(int id);
    void draw_title_of_menu(MENU_ID id);
    void draw_button(int id, int x, int y, char* str, unsigned short ff_flags, bool is_red = false, bool is_locked = false);
    bool is_anylevel(void);
    char* replace_newgame(bool is_anylevel);
    void init_current_button(void);
    
    /// select level:
    void draw_sprite_up();
    void draw_sprite_down();
    void update_draw(do_draw draw);
    int update_input(do_input input);
public:
    MENU_ID get_current_gui() const;
    int get_page(int button_id);
    gui_menu* get_gui(MENU_ID current_gui);
    void set_current_gui(MENU_ID id);
    void set_previous_gui();
    void play_move_sound();

    int do_menu();
    int do_title_menu();
    int do_select_level_menu();
    
    void initialise_gui(void);
    void free_gui(void);
};
extern GAME_GUI game_gui;

extern int title_options(void);

#ifdef DLL_INJECT
#define TitleDBInput                VAR_U_(0x005358AC, int)
///#define CurrentSelection         VAR_I_(0x004AFAB0, DWORD, 1)
///#define OldSelection             VAR_U_(0x007FD248, DWORD)
///#define TitleStatus              VAR_I_(0x004BF67C, int, 0)
///#define TitleSequence            VAR_I_(0x004BF674, int, 0)
#define KeyRALT                     VAR_U_(0x00754324, unsigned char)
#define CharacterMenu               ARRAY_(0x004AFAA4, char, [12])

#define draw_title_logo   ((void(__cdecl*)(void)) 0x0048B130)
#define DoOptions       ((void(__cdecl*)(void)) 0x0047B170)
#endif
