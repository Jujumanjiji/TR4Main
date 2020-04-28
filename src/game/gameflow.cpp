#include "framework.h"
#include "gameflow.h"
#include "camera.h"
#include "control.h"
#include "directx.h"
#include "deltapak.h"
#include "draw.h"
#include "file.h"
#include "fmvs.h"
#include "health.h"
#include "inventory_2d.h"
#include "items.h"
#include "interface.h"
#include "libgpu.h"
#include "lot.h"
#include "loadsave.h"
#include "sound.h"
#include "specific.h"
#include "text.h"
#include "tomb4fx.h"

#define FMV_COPYRIGHT_INTRO 0
#define FMV_GAME_INTRO 1
#define PIX_ENGLISH "pix\\title_load\\english.bmp"
#define PIX_FRENCH "pix\\title_load\\french.bmp"
#define PIX_ITALIAN "pix\\title_load\\italian.bmp"
#define PIX_JAPANESE "pix\\title_load\\japanese.bmp"
#define PIX_SPANISH "pix\\title_load\\spanish.bmp"
#define PIX_GERMAN "pix\\title_load\\german.bmp"
#define PIX_TITLE "pix\\title.bmp"

void DoBootScreen(uint32 language_id)
{
    switch (language_id)
    {
    case LNG_ENGLISH:
    case LNG_UK:
    case LNG_US:
        DXCopyBitmap(App.lpBackBuffer, PIX_ENGLISH);
        S_DumpScreen();
        DXCopyBitmap(App.lpBackBuffer, PIX_ENGLISH);
        break;
    case LNG_FRENCH:
        DXCopyBitmap(App.lpBackBuffer, PIX_FRENCH);
        S_DumpScreen();
        DXCopyBitmap(App.lpBackBuffer, PIX_FRENCH);
        break;
    case LNG_GERMAN: // ALLEMAND
        DXCopyBitmap(App.lpBackBuffer, PIX_GERMAN);
        S_DumpScreen();
        DXCopyBitmap(App.lpBackBuffer, PIX_GERMAN);
        break;
    case LNG_ITALIAN:
        DXCopyBitmap(App.lpBackBuffer, PIX_ITALIAN);
        S_DumpScreen();
        DXCopyBitmap(App.lpBackBuffer, PIX_ITALIAN);
        break;
    case LNG_SPANISH: // ESPAGNOL
        DXCopyBitmap(App.lpBackBuffer, PIX_SPANISH);
        S_DumpScreen();
        DXCopyBitmap(App.lpBackBuffer, PIX_SPANISH);
        break;
    case LNG_JAPAN:
        DXCopyBitmap(App.lpBackBuffer, PIX_JAPANESE);
        S_DumpScreen();
        DXCopyBitmap(App.lpBackBuffer, PIX_JAPANESE);
        break;
    }
}

/*
void DoGameflow(void)
{
    
}
*/

void DoLevel(unsigned char name, unsigned char audio)
{
    SetFade(255, 0);

    if (gfGameMode != 4)
        memset(&savegame_game, 0, sizeof(STATS));

    S_LoadLevelFile((char*)name);
    SetFogColor(gfFogColor.r, gfFogColor.g, gfFogColor.b);
    InitialiseFXArray(TRUE);
    InitialiseLOT(TRUE);
    ClearFXFogBulbs();
    InitSpotCamSequences();
    InitialisePickUpDisplay();
    //S_InitialiseScreen(); // return nothing !
    SOUND_Stop();
    bDisableLaraControl = FALSE;

    if (gfGameMode == 4)
    {
        sgRestoreGame();
        gfRequiredStartPos = 0;
        gfInitialiseGame = 0;
        gfFogColor.b = gfFogColor2.b;
        gfFogColor.g = gfFogColor2.g;
        gfFogColor.r = gfFogColor2.r;
        if (App.settingVolumetric)
            SetFogColor(gfFogColor.r, gfFogColor.g, gfFogColor.b);
    }
    else
    {
        if (gfInitialiseGame != 0)
        {
            GameTimer = 0;
            gfRequiredStartPos = 0;
            gfInitialiseGame = 0;
            CutSceneTriggered = 0;
            FmvSceneTriggered = 0;
        }
        else
        {
            LoadGame();
        }

        if (CHK_EXI(gfLevelFlags, SLEV_REMOVE_AMULET))
            lara.questitems &= ~1;
        savegame_game.timer = 0;
        CurrentAtmosphere = audio;
    }

    S_CDPlay(CurrentAtmosphere, 1);
    IsAtmospherePlaying = 1;
    ScreenFadedOut = 0;
    ScreenFading = 0;
    ScreenFadeBack = 0;
    dScreenFade = 255;
    ScreenFade = 255;

    if (gfCutsceneID == 0 || CutSceneTriggered & (1 << gfCutsceneID))
    {
        cutseq_num = 0;
        gfCutsceneID = 0;
        SetScreenFadeIn(16);
    }
    else
    {
        cutseq_num = gfCutsceneID;
        gfCutsceneID = 0;
        ScreenFadedOut = 1;
    }

    InitialiseCamera();
    bUseSpotCam = 0;
    gfGameMode = 0;
    gfLevelStart = 0;
    nframes = 2;
    framecount = 0;
    gfStatus = ControlPhase(nframes, FALSE);
    byte_5358AC = 0;
        
    while (gfStatus == 0)
    {
        GPU_BeginScene();

        if (gfLegendTime != 0 && !DestFadeScreenHeight && !FadeScreenHeight && !cutseq_num)
        {
            PrintString(phd_winwidth >> 1, phd_winymax - font_height, 2, &gfStringWad[gfStringOffsetReal[gfLegend]], FF_CENTER);
            gfLegendTime--;
        }

        nframes = DrawPhaseGame();
        handle_cutseq_triggering();

        if (DEL_playingamefmv)
        {
            DEL_playingamefmv = 0;

            // hardcoded:
            S_CDStop();
            S_PlayFMV(7);
            MoveLaraForFMV(54179, -8192, 50899, -32703);
        }

        if (gfLevelStart)
        {
            gfStatus = 3;
            break;
        }

        int status = 0;
        gfStatus = ControlPhase(nframes, FALSE);
        if (gfStatus != 0)
        {
            if (status == 0)
            {
                if (lara_item->hit_points >= 0)
                    break;
                status = gfStatus;
                SetFade(0, 255);
                gfStatus = 0;
            }
        }

        if (status != 0)
        {
            gfStatus = 0;
            if (status && dword_672E60 == 2)
            {
                gfStatus = status;
                if (gfStatus)
                    break;
            }
        }
        else
        {
            if (gfStatus)
                break;
        }
    }

    S_SoundStopAllSamples();
    S_CDStop();

    if (gfStatus == 3) // Play FMV
    {
        int fmvStatus = 0;
        char fmv0, fmv1;

        fmv0 = fmv_to_play[0];
        fmv1 = fmv_to_play[1];

        if (fmv0 < 0)
        {
            if ((fmv0 & 0x7F) == 9 && gfLevelStart != 10)
            {
                fmv_to_play[0] = fmv0 = 0;
            }
            
            if ((fmv0 & 0x7F) == 8 && gfLevelStart != 22)
            {
                if (fmv0)
                    S_PlayFMV(fmv0 & 0x7F);
            }
        }
        
        if (fmv0 != 0)
            fmvStatus = S_PlayFMV(fmv0 & 0x7F);
        else
            fmv0 = 0;

        if (fmv1 != 0 && fmvStatus != 2) // 2 = fmv is playing..
            S_PlayFMV(fmv1 & 0x7F);
    }

    num_fmvs = 0;
    fmv_to_play[1] = 0;
    fmv_to_play[0] = 0;
    lara.examine1 = 0;
    lara.examine2 = 0;
    lara.examine3 = 0;

    PrepareCutscene();

    if (gfStatus == 3 && gfLevelStart == 39)
    {
        gfStatus = 1;
        bDoCredits = true;
    }
    else if (gfStatus == 3 && gfLevelStart == 25 && gfResetHubEnabled)
    {
        lara.skidoo = NO_ITEM;
    }

    TrInput = IN_NONE;
    reset_flag = FALSE;
}

void DoTitle(unsigned char name, unsigned char audio)
{
    bCreditDone = false;
    SetFade(255, 0);
    num_fmvs = 0;
    fmv_to_play[1] = 0;
    fmv_to_play[0] = 0;
    ResetHUB(TRUE);

    memset(&savegame_level, 0, sizeof(STATS));
    memset(&savegame_game, 0, sizeof(STATS));

    S_LoadLevelFile((char*)name);

    game_gui.initialise_gui(); // TODO: move to DoGameflow() !
    GLOBAL_invitemlastchosen = NO_ITEM;
#ifndef FEATURE_OLD_TITLE
    InitSpotCamSequences();
#endif
    title_controls_locked_out = false;
    InitialiseFXArray(TRUE);
    InitialiseLOT(TRUE);
    SetFogColor(gfFogColor.r, gfFogColor.g, gfFogColor.b);
    ClearFXFogBulbs();
    InitialisePickUpDisplay();
    //S_InitialiseScreen();
    SOUND_Stop();
    S_CDPlay(audio, TRUE);
    IsAtmospherePlaying = FALSE;
    //S_SetReverbTypes(1);
    InitialiseCamera(); // it crash (division by zero if no camera is initialised with old title feature!) so it's called by default!

    if (bDoCredits)
    {
        cutseq_num = 28;                            // CUT_SPECIAL1
        SetFadeClip(32, 1);
        ScreenFadedOut = 1;
        ScreenFade = 255;
        dScreenFade = 255;
        S_CDPlay(98, 1);
    }
    else
    {
#ifndef FEATURE_OLD_TITLE
        trigger_title_spotcam(CUTSCENE_TITLE);
#endif
        ScreenFadedOut = 0;
        ScreenFade = 0;
        dScreenFade = 0;
        ScreenFadeBack = 0;
        ScreenFadeSpeed = 8;
        ScreenFading = 0;
    }

#ifndef FEATURE_OLD_TITLE
    bUseSpotCam = TRUE;
#endif

    lara_item->mesh_bits = 0;
    gfGameMode = 1;
    gfLevelStart = 0;
    nframes = 2;

#ifdef FEATURE_OLD_TITLE
    // load the title image and print it:
    DXCopyBitmap(App.lpBackBuffer, PIX_TITLE);
    S_DumpScreen();
    DXCopyBitmap(App.lpBackBuffer, PIX_TITLE);
#endif

    gfStatus = ControlPhase(nframes, FALSE);
    while (gfStatus == 0)
    {
        GPU_BeginScene();
        gfStatus = title_options();
        if (gfStatus != 0)
            break;
#ifndef FEATURE_OLD_TITLE
        handle_cutseq_triggering(); // no cutscene in the old title version !
        nframes = DrawPhaseGame();
#endif
        gfStatus = ControlPhase(nframes, FALSE);
    }

    if (gfStatus == 4) // game is closing ?
        game_gui.free_gui();

    S_SoundStopAllSamples();
    S_CDStop();
    bUseSpotCam = FALSE;
    bDisableLaraControl = FALSE;
    if (gfLevelStart == 1 && gfStatus != 2)
        S_PlayFMV(12);
    if (gfStatus != 4)
        PrepareCutscene();
    TrInput = IN_NONE;
}

#ifdef DLL_INJECT
void injector::f_game::inject_gameflow()
{
    inject(0x004808E0, DoBootScreen);
    ///inject(0x00451770, DoGameflow);
    inject(0x00452710, DoLevel);
    inject(0x00451E30, DoTitle);
}
#endif
