#include "framework.h"
#include "interface.h"
#include "gameflow.h"
#include "savegame.h"
#include "sound.h"
#include "specific.h"
#include "text.h"

#define IN_MOVE_UP IN_FORWARD
#define IN_MOVE_DOWN IN_BACK

static int TitleCurrentLevel;
static int TitleSequence = TS_IDLE;
static int TitleStatus = 0;
GAME_GUI game_gui;

int GAME_GUI::multiply_pos(int from, int to)
{
    return from + (to * font_height);
}

int GAME_GUI::font_flags(int id)
{
    gui_menu* tgui = &gui[gui_current];
    button_struct* tbutton = &tgui->button[get_page(id)];
    button_array* ptr = &tbutton->arrays[id];

    if (tgui->current == id && !ptr->is_red)
        return TF_BLINK;
    else if (ptr->is_red)
        return TF_RED;
    else
        return TF_NORMAL;
}

void GAME_GUI::draw_title_of_menu(MENU_ID id)
{
    char* name = nullptr;

    switch (id)
    {
        case M_SELECT_LEVEL:
            name = &gfStringWad[gfStringOffsetReal[STR_SELECT_LEVEL]];
            break;
        case M_LOAD_TITLE:
        case M_LOAD:
            name = &gfStringWad[gfStringOffsetReal[STR_SELECT_GAME_TO_LOAD]];
            break;
        case M_SAVE:
            name = &gfStringWad[gfStringOffsetReal[STR_SELECT_GAME_TO_SAVE]];
            break;
    }

    PrintString(phd_centerx, phd_winymin + font_height, TF_ORANGE, name, FF_CENTER); // title of the current menu !
}

void GAME_GUI::draw_time(int hour, int minute, int second)
{
    
}

int GAME_GUI::get_page(int current)
{
    CCLAMP(current, 0,  9,  0);
    CCLAMP(current, 10, 19, 1);
    CCLAMP(current, 20, 29, 2);
    CCLAMP(current, 30, 39, 3);
    CCLAMP(current, 40, 49, 4);
    CCLAMP(current, 50, 59, 5);

    // if it's more than 6 page return to start
    return 0;
}

int GAME_GUI::update_title_input(gui_menu* tgui)
{
    button_struct* tbutton;
    button_array* ptr;

    if (CHK_EXI(TitleDBInput, IN_MOVE_UP))
    {
        if (tgui->current > tgui->min)
        {
            tgui->old = tgui->current;
            tgui->next = tgui->current - 1;

            tbutton = &tgui->button[tgui->current_page];
            ptr = &tbutton->arrays[tgui->next];
            if (ptr->is_enabled)
            {
                if (ptr->is_red && !ptr->is_locked)
                {
                    play_move_sound();
                    tgui->current -= 2;
                }
                else if (!ptr->is_red && !ptr->is_locked)
                {
                    play_move_sound();
                    tgui->current--;
                }
            }
        }
    }
    else
    if (CHK_EXI(TitleDBInput, IN_MOVE_DOWN))
    {
        if (tgui->current < tgui->max)
        {
            tgui->old = tgui->current;
            tgui->next = tgui->current + 1;

            tbutton = &tgui->button[tgui->current_page];
            ptr = &tbutton->arrays[tgui->next];
            if (ptr->is_enabled)
            {
                if (ptr->is_red && !ptr->is_locked)
                {
                    play_move_sound();
                    tgui->current += 2;
                }
                else if (!ptr->is_red && !ptr->is_locked)
                {
                    play_move_sound();
                    tgui->current++;
                }
            }
        }
    }

    if (CHK_EXI(TitleDBInput, IN_SELECT))
    {
        SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);

        switch (tgui->current)
        {
            case 0: // NEW GAME / SELECT LEVEL
                if (gameflow->play_any_level)
                {
                    TitleSequence = TS_SELECT_LEVEL;
                }
                else
                {
                    gfLevelStart = 1;
                    return 3; // start game
                }
                break;
            case 1: // LOAD GAME
                //FoundSaveGameInfo();
                TitleSequence = TS_IDLE;
                break;
            case 2: // OPTIONS
                TitleSequence = TS_IDLE;
                break;
            case 3: // EXIT GAME
                return 4; // exit the game !
        }
    }
    else
    if (CHK_EXI(TitleDBInput, IN_DESELECT) && gui_old != gui_current)
    {
        TitleSequence = TS_IDLE;
        S_SoundStopAllSamples();
        play_select_sound();
        set_previous_gui();
    }
    
    return 0; // loop
}

int GAME_GUI::update_select_level_input(gui_menu* tgui)
{
    int32 to_ignore = NO_BUTTON;
    button_struct* tbutton;
    button_array* ptr;

    if (CHK_EXI(TitleDBInput, IN_MOVE_UP))
    {
        if (tgui->current >= tgui->min)
        {
            tgui->old = tgui->current;
            tgui->next = tgui->current - 1;
            if ((tgui->bprevious != NO_BUTTON && tgui->next <= tgui->bprevious) || (tgui->next_ignore != NO_BUTTON && tgui->next == tgui->next_ignore))
            {
                tgui->current_page--;
                if (tgui->next == tgui->next_ignore)
                {
                    play_move_sound();
                    tgui->current -= 2;
                    return 0; // ignore the code after (not needed !)
                }
            }

            tbutton = &tgui->button[tgui->current_page];
            ptr = &tbutton->arrays[tgui->next];
            if (ptr->is_enabled) // button is enabled
            {
                if (ptr->is_red && !ptr->is_locked)         // not locked and red ? then miss 1 button
                {
                    play_move_sound();
                    tgui->current -= 2;
                }
                else if (!ptr->is_red && !ptr->is_locked)   // not locked and not red ? then next button !
                {
                    play_move_sound();
                    tgui->current--;
                }
            }
        }
    }
    else
    if (CHK_EXI(TitleDBInput, IN_MOVE_DOWN))
    {
        if (tgui->current <= tgui->max)
        {
            tgui->old = tgui->current;
            tgui->next = tgui->current + 1;
            if ((tgui->bnext != NO_BUTTON && tgui->next >= tgui->bnext) || (tgui->next_ignore != NO_BUTTON && tgui->next == tgui->next_ignore))
            {
                tgui->current_page++;
                if (tgui->next == tgui->next_ignore)
                {
                    play_move_sound();
                    tgui->current += 2;
                    return 0; // ignore the code after (not needed !)
                }
            }

            tbutton = &tgui->button[tgui->current_page];
            ptr = &tbutton->arrays[tgui->next];
            if (ptr->is_enabled) // button is enabled
            {
                if (ptr->is_red && !ptr->is_locked)         // not locked and red ? then miss 1 button
                {
                    play_move_sound();
                    tgui->current += 2;
                }
                else if (!ptr->is_red && !ptr->is_locked)   // not locked and not red ? then next button !
                {
                    play_move_sound();
                    tgui->current++;
                }
            }
        }
    }

    if (CHK_EXI(TitleDBInput, IN_SELECT))
    {
        play_select_sound();
        gfLevelStart = tgui->current;
        return 3;
    }
    else
    if (CHK_EXI(TitleDBInput, IN_DESELECT) && gui_old != gui_current)
    {
        TitleSequence = TS_IDLE;
        S_SoundStopAllSamples();
        play_select_sound();
        set_previous_gui();
    }

    return 0; // loop
}

int GAME_GUI::update_load_from_title_input(gui_menu* tgui)
{
    if (CHK_EXI(TitleDBInput, IN_MOVE_UP))
    {

    }
    else
    if (CHK_EXI(TitleDBInput, IN_MOVE_DOWN))
    {

    }


    if (CHK_EXI(TitleDBInput, IN_SELECT))
    {
        play_select_sound();
    }
    else
    if (CHK_EXI(TitleDBInput, IN_DESELECT) && gui_old != gui_current)
    {
        TitleSequence = TS_IDLE;
        S_SoundStopAllSamples();
        play_select_sound();
        set_previous_gui();
    }

    return NO_LOAD;
}

int GAME_GUI::do_title_menu()
{
    draw_title_logo();
    set_current_gui(M_TITLE, M_TITLE);
    init_current_button();
    gui_menu* tgui = &gui[gui_current];
    tgui->default_x = phd_centerx;
    tgui->default_y = phd_winymax - (4 * font_height);
    draw_button(0, tgui->default_x, multiply_pos(tgui->default_y, 0), replace_newgame(is_anylevel()),  FF_CENTER);
    draw_button(1, tgui->default_x, multiply_pos(tgui->default_y, 1), &gfStringWad[gfStringOffsetReal[STR_TITLE_LOAD_GAME]], FF_CENTER, gameflow->load_save_enabled ? false : true);
    draw_button(2, tgui->default_x, multiply_pos(tgui->default_y, 2), &gfStringWad[gfStringOffsetReal[STR_OPTIONS]],         FF_CENTER);
    draw_button(3, tgui->default_x, multiply_pos(tgui->default_y, 3), &gfStringWad[gfStringOffsetReal[STR_EXIT]],            FF_CENTER);
    tgui->next_ignore = NO_BUTTON;
    tgui->min = 0;
    tgui->max = 3;
    tgui->bprevious = NO_BUTTON;
    tgui->bnext = NO_BUTTON;

    return update_title_input(tgui);
}

int GAME_GUI::do_select_level_menu()
{
    draw_title_of_menu(M_SELECT_LEVEL);
    set_current_gui(M_SELECT_LEVEL, M_TITLE);
    init_current_button();
    gui_menu* tgui = &gui[gui_current];
    tgui->default_x = phd_centerx;

    /// PAGE 0
    // lock title button so the player will not try to go in negative button and rekt itself :D
    // also the default start button is 1 because the player will be already in the title level !
    draw_button(0, tgui->default_x, multiply_pos(phd_winymin, 3), &gfStringWad[gfStringOffsetReal[gfLevelName[0]]], FF_CENTER, true, true); // title screen
    draw_button(1, tgui->default_x, multiply_pos(phd_winymin, 4), &gfStringWad[gfStringOffsetReal[gfLevelName[1]]], FF_CENTER);  // angkor wat
    draw_button(2, tgui->default_x, multiply_pos(phd_winymin, 5), &gfStringWad[gfStringOffsetReal[gfLevelName[2]]], FF_CENTER);  // race of the iris
    draw_button(3, tgui->default_x, multiply_pos(phd_winymin, 6), &gfStringWad[gfStringOffsetReal[gfLevelName[3]]], FF_CENTER);  // the tomb of seth
    draw_button(4, tgui->default_x, multiply_pos(phd_winymin, 7), &gfStringWad[gfStringOffsetReal[gfLevelName[4]]], FF_CENTER);  // test level
    draw_button(5, tgui->default_x, multiply_pos(phd_winymin, 8), &gfStringWad[gfStringOffsetReal[gfLevelName[5]]], FF_CENTER);  // burial chambers
    draw_button(6, tgui->default_x, multiply_pos(phd_winymin, 9), &gfStringWad[gfStringOffsetReal[gfLevelName[6]]], FF_CENTER);  // valley of the kings
    draw_button(7, tgui->default_x, multiply_pos(phd_winymin, 10), &gfStringWad[gfStringOffsetReal[gfLevelName[7]]], FF_CENTER); // kv5
    draw_button(8, tgui->default_x, multiply_pos(phd_winymin, 11), &gfStringWad[gfStringOffsetReal[gfLevelName[8]]], FF_CENTER); // temple of karnak
    draw_button(9, tgui->default_x, multiply_pos(phd_winymin, 12), &gfStringWad[gfStringOffsetReal[gfLevelName[9]]], FF_CENTER); // the great hypostyle hall
    if (tgui->current_page == 0)
    {
        tgui->next_ignore = NO_BUTTON;
        tgui->min = 0;
        tgui->max = 9;
        tgui->bprevious = NO_BUTTON;
        tgui->bnext = 10;
        draw_sprite_down();
    }

    /// PAGE 1
    draw_button(10, tgui->default_x, multiply_pos(phd_winymin, 3), &gfStringWad[gfStringOffsetReal[gfLevelName[10]]], FF_CENTER);  // sacred lake
    draw_button(11, tgui->default_x, multiply_pos(phd_winymin, 4), NO_LEVEL, FF_CENTER, true);                                            // yes
    draw_button(12, tgui->default_x, multiply_pos(phd_winymin, 5), &gfStringWad[gfStringOffsetReal[gfLevelName[12]]], FF_CENTER);  // tomb of semerkhet
    draw_button(13, tgui->default_x, multiply_pos(phd_winymin, 6), &gfStringWad[gfStringOffsetReal[gfLevelName[13]]], FF_CENTER);  // guardian of semerkhet
    draw_button(14, tgui->default_x, multiply_pos(phd_winymin, 7), &gfStringWad[gfStringOffsetReal[gfLevelName[14]]], FF_CENTER);  // desert railroad
    draw_button(15, tgui->default_x, multiply_pos(phd_winymin, 8), &gfStringWad[gfStringOffsetReal[gfLevelName[15]]], FF_CENTER);  // alexandria
    draw_button(16, tgui->default_x, multiply_pos(phd_winymin, 9), &gfStringWad[gfStringOffsetReal[gfLevelName[16]]], FF_CENTER);  // coastal ruins
    draw_button(17, tgui->default_x, multiply_pos(phd_winymin, 10), &gfStringWad[gfStringOffsetReal[gfLevelName[17]]], FF_CENTER); // pharos, temple of isis
    draw_button(18, tgui->default_x, multiply_pos(phd_winymin, 11), &gfStringWad[gfStringOffsetReal[gfLevelName[18]]], FF_CENTER); // cleopatra's palaces
    draw_button(19, tgui->default_x, multiply_pos(phd_winymin, 12), &gfStringWad[gfStringOffsetReal[gfLevelName[19]]], FF_CENTER); // catacomb
    if (tgui->current_page == 1)
    {
        tgui->next_ignore = NO_BUTTON;
        tgui->min = 10;
        tgui->max = 19;
        tgui->bprevious = 9;
        tgui->bnext = 20;
        draw_sprite_up();
        draw_sprite_down();
    }

    /// PAGE 2
    draw_button(20, tgui->default_x, multiply_pos(phd_winymin, 3), &gfStringWad[gfStringOffsetReal[gfLevelName[20]]], FF_CENTER);  // temple of poseidon
    draw_button(21, tgui->default_x, multiply_pos(phd_winymin, 4), &gfStringWad[gfStringOffsetReal[gfLevelName[21]]], FF_CENTER);  // the lost library
    draw_button(22, tgui->default_x, multiply_pos(phd_winymin, 5), &gfStringWad[gfStringOffsetReal[gfLevelName[22]]], FF_CENTER);  // hall of demetrius
    draw_button(23, tgui->default_x, multiply_pos(phd_winymin, 6), &gfStringWad[gfStringOffsetReal[gfLevelName[23]]], FF_CENTER);  // city of the dead
    draw_button(24, tgui->default_x, multiply_pos(phd_winymin, 7), &gfStringWad[gfStringOffsetReal[gfLevelName[24]]], FF_CENTER);  // trenches
    draw_button(25, tgui->default_x, multiply_pos(phd_winymin, 8), &gfStringWad[gfStringOffsetReal[gfLevelName[25]]], FF_CENTER);  // chambers of tulun
    draw_button(26, tgui->default_x, multiply_pos(phd_winymin, 9), &gfStringWad[gfStringOffsetReal[gfLevelName[26]]], FF_CENTER);  // street bazaar
    draw_button(27, tgui->default_x, multiply_pos(phd_winymin, 10), &gfStringWad[gfStringOffsetReal[gfLevelName[27]]], FF_CENTER); // citadel gate
    draw_button(28, tgui->default_x, multiply_pos(phd_winymin, 11), &gfStringWad[gfStringOffsetReal[gfLevelName[28]]], FF_CENTER); // citadel
    draw_button(29, tgui->default_x, multiply_pos(phd_winymin, 12), &gfStringWad[gfStringOffsetReal[gfLevelName[29]]], FF_CENTER); // the sphinx complex
    if (tgui->current_page == 2)
    {
        tgui->next_ignore = NO_BUTTON;
        tgui->min = 20;
        tgui->max = 29;
        tgui->bprevious = 19;
        tgui->bnext = 30;
        draw_sprite_up();
        draw_sprite_down();
    }

    /// PAGE 3
    draw_button(30, tgui->default_x, multiply_pos(phd_winymin, 3), NO_LEVEL, FF_CENTER, true);                                     // no
    draw_button(31, tgui->default_x, multiply_pos(phd_winymin, 4), &gfStringWad[gfStringOffsetReal[gfLevelName[31]]], FF_CENTER);  // underneath the sphinx
    draw_button(32, tgui->default_x, multiply_pos(phd_winymin, 5), &gfStringWad[gfStringOffsetReal[gfLevelName[32]]], FF_CENTER);  // menkaure's pyramid
    draw_button(33, tgui->default_x, multiply_pos(phd_winymin, 6), &gfStringWad[gfStringOffsetReal[gfLevelName[33]]], FF_CENTER);  // inside mankaure's pyramid
    draw_button(34, tgui->default_x, multiply_pos(phd_winymin, 7), &gfStringWad[gfStringOffsetReal[gfLevelName[34]]], FF_CENTER);  // the mastabas
    draw_button(35, tgui->default_x, multiply_pos(phd_winymin, 8), &gfStringWad[gfStringOffsetReal[gfLevelName[35]]], FF_CENTER);  // the great pyramid
    draw_button(36, tgui->default_x, multiply_pos(phd_winymin, 9), &gfStringWad[gfStringOffsetReal[gfLevelName[36]]], FF_CENTER);  // khufu's queens pyramids
    draw_button(37, tgui->default_x, multiply_pos(phd_winymin, 10), &gfStringWad[gfStringOffsetReal[gfLevelName[37]]], FF_CENTER); // inside the great pyramid
    draw_button(38, tgui->default_x, multiply_pos(phd_winymin, 11), &gfStringWad[gfStringOffsetReal[gfLevelName[38]]], FF_CENTER); // temple of horus part 1
    draw_button(39, tgui->default_x, multiply_pos(phd_winymin, 12), &gfStringWad[gfStringOffsetReal[gfLevelName[39]]], FF_CENTER); // temple of horus part 2
    if (tgui->current_page == 3)
    {
        tgui->next_ignore = 30;         // skip the button 30 (and switch page !)
        tgui->min = 30;
        tgui->max = 39;
        // not used because next_ignore have priority
        // this bprevious not switch between page if a red button is first in list !
        tgui->bprevious = NO_BUTTON;
        tgui->bnext = NO_BUTTON;
        draw_sprite_up();
    }

    return update_select_level_input(tgui);
}

// TODO: for later !
int GAME_GUI::do_load_menu(void)
{
    draw_title_of_menu(M_LOAD_TITLE);
    set_current_gui(M_LOAD_TITLE, M_TITLE);
    init_current_button();
    gui_menu* tgui = &gui[gui_current];
    tgui->default_x = phd_centerx;
    tgui->default_y = int(phd_winwidth * 0.0015625);
    

    return update_load_from_title_input(tgui);
}

int GAME_GUI::do_menu()
{
    int status = 0;
    int id = -1;
    
    switch (TitleSequence)
    {
    case TS_IDLE:
        status = game_gui.do_title_menu();
        break;
    case TS_SELECT_LEVEL:
        status = game_gui.do_select_level_menu();
        break;
    case TS_LOAD:  // TODO: recreate load menu !
        TitleSequence = TS_IDLE;
        S_LogValue("TS_LOAD: not implemented !");       // check update_title_input() for selection !
        break;
    case TS_OPTION:// TODO: recreate option menu !
        TitleSequence = TS_IDLE;
        S_LogValue("TS_OPTION: not implemented !");     // check update_title_input() for selection !
        break;
    }

    if (thread_started)
    {
        return 4; // exit game
    }
    else
    if (status != 0)
    {
        TitleStatus = status;
        TitleCurrentLevel = gfLevelStart;
        status = 0;
        SetFade(0, 255);
    }
    else if (status != 3 && status != 4) // not new game or exit game !
    {
        TitleStatus = 0;
    }
    
    return TitleStatus;
}

void GAME_GUI::draw_sprite_up()
{
    PrintString(phd_winxmin + 32, multiply_pos(phd_winymin, 3), TF_ORANGE, &CharacterMenu[4], FF_NONE);
    PrintString(phd_winxmax - 64, multiply_pos(phd_winymin, 3), TF_ORANGE, &CharacterMenu[4], FF_NONE);
}

void GAME_GUI::draw_sprite_down()
{
    PrintString(phd_winxmin + 32, multiply_pos(phd_winymin, 10), TF_ORANGE, &CharacterMenu[0], FF_NONE);
    PrintString(phd_winxmax - 64, multiply_pos(phd_winymin, 10), TF_ORANGE, &CharacterMenu[0], FF_NONE);
}

MENU_ID GAME_GUI::get_current_gui() const
{
    return gui_current;
}

gui_menu* GAME_GUI::get_gui(MENU_ID current_gui)
{
    return &gui[current_gui];
}

void GAME_GUI::set_current_gui(MENU_ID now, MENU_ID old)
{
    gui_old = old;
    gui_current = now;
}

// for safe purpose, i dont want to have some problem with it later !
// switch to old gui
void GAME_GUI::set_previous_gui()
{
    gui_current = gui_old;
}

void GAME_GUI::play_move_sound()
{
    SoundEffect(SFX_MENU_CHOOSE, nullptr, SFX_ALWAYS);
}

void GAME_GUI::play_select_sound(void)
{
    SoundEffect(SFX_MENU_SELECT, nullptr, SFX_ALWAYS);
}

void GAME_GUI::draw_button(int id, int x, int y, char* str, unsigned short ff_flags, bool is_red, bool is_locked)
{
    gui_menu* tgui = &gui[gui_current];
    button_struct* button = &tgui->button[get_page(id)];
    button_array* ptr = &button->arrays[id];

    if (!ptr->is_enabled)
    {
        ptr->x = x;
        ptr->y = y;
        ptr->str = str;
        ptr->ff_flags = ff_flags;
        ptr->is_locked = is_locked;
        ptr->is_red = is_red;
        ptr->is_enabled = true;
    }

    if (ptr->is_enabled && tgui->current_page == get_page(id))
        PrintString(ptr->x, ptr->y, font_flags(id), ptr->str, ptr->ff_flags);
}

bool GAME_GUI::is_anylevel()
{
    return gameflow->play_any_level ? true : false;
}

char* GAME_GUI::replace_newgame(bool is_anylevel)
{
    if (is_anylevel)
        return &gfStringWad[gfStringOffsetReal[STR_SELECT_LEVEL]];
    else
        return &gfStringWad[gfStringOffsetReal[STR_TITLE_NEW_GAME]];
}

void GAME_GUI::init_current_button(void)
{
    gui_menu* tgui = &gui[gui_current];
    if (!tgui->is_initial)
    {
        tgui->current = tgui->initial;
        tgui->is_initial = true;
    }
}

void GAME_GUI::initialise_gui(void)
{
    gui_current = M_TITLE;
    gui_old = M_TITLE;
    
    for (int i = 0; i < MAX_GUI; i++)
    {
        gui_menu* tgui = &gui[i];
        tgui->default_x = phd_winxmin; // avoid problem of drawing
        tgui->default_y = phd_winymin; // 
        tgui->current = 0;
        tgui->old = 0;
        tgui->next = 0;
        tgui->next_ignore = NO_BUTTON;
        if (i == M_SELECT_LEVEL)
            tgui->initial = 1;
        else
            tgui->initial = 0;
        tgui->is_initial = false;
        tgui->min = 0;
        tgui->max = 0;
        tgui->bprevious = 0;
        tgui->bnext = 0;
        tgui->current_page = 0;

        int j, k;
        for (j = 0, k = 0; j < MAX_BUTTON, k < MAX_PAGE; j++, k++)
        {
            button_struct* button = &tgui->button[k];
            button_array* ptr = &button->arrays[j];
            ptr->is_enabled = false;
            ptr->is_red = false;
            ptr->is_locked = false;
            ptr->x = 0;
            ptr->y = 0;
            ptr->str = NULL;
            ptr->ff_flags = NULL;
        }
    }
}

void GAME_GUI::free_gui(void)
{
    
}

int title_options(void)
{
    int status;
    int levelback;

    status = TitleStatus;
    if (status != 0)
    {
        if (FadeType == 2)
        {
            levelback = TitleCurrentLevel;
            TitleCurrentLevel = 0;
            gfLevelStart = levelback;
            TitleStatus = 0;
            return TitleStatus;
        }
        TrInput = IN_NONE;
        TitleDBInput = IN_NONE;
    }
    
    if (!bDoCredits)
    {
        TitleStatus = game_gui.do_menu();
        return TitleStatus;
    }

    if (!DisplayCredit())
        bDoCredits = false;
    else
        status = 0;

    return status;
}