#pragma once

struct BABOON_RESPAWN_STRUCT
{
    int id;
    PHD_3DPOS pos;
    int count;
    int max_count; // used to limit the number of respawn !
};

class BABOON_RESPAWN
{
private:
    vector<BABOON_RESPAWN_STRUCT> baboonRespawnArray;
public:
    void Free(void);
    void Add(ITEM_INFO* item, int max_count);
    void Remove(int id);
    int GetBaboonFreePlace(void);
    BABOON_RESPAWN_STRUCT* GetBaboonRespawn(int id);
    int GetCount(int id);
    int GetCountMax(int id);
};


extern BABOON_RESPAWN BaboonRespawn;

extern void InitialiseBaboon(short itemNumber);
extern void BaboonControl(short itemNumber);
extern void BaboonSearchCrowbarSwitch(ITEM_INFO* item, short flags);
extern void BaboonRespawnFunction(short itemNumber);
extern void BaboonDieEffect(ITEM_INFO* item);

#ifdef DLL_INJECT
///#define InitialiseBaboon ((void(__cdecl*)(short itemNumber)) 0x00401DB0)
///#define BaboonControl ((void(__cdecl*)(short itemNumber)) 0x00401E50)
///#define BaboonSearchCrowbarSwitch ((void(__cdecl*)(ITEM_INFO *item, short flags)) 0x004027E0)
///#define BaboonRespawn ((void(__cdecl*)(short itemNumber)) 0x00402850)
///#define BaboonDieEffect ((void(__cdecl*)(ITEM_INFO* item)) 0x00402970)
#endif
