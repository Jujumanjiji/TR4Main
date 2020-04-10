#include "framework.h"
#include "file_utils.h"

int8 ReadByte()
{
    const int8 ret = *(int8*)level_data;
    level_data += 1;
    return ret;
}

uint8 ReadUByte()
{
    const uint8 ret = *(uint8*)level_data;
    level_data += 1;
    return ret;
}

sint8 ReadSByte()
{
    const sint8 ret = *(sint8*)level_data;
    level_data += 1;
    return ret;
}

int16 ReadWord()
{
    const int16 ret = *(int16*)level_data;
    level_data += sizeof(int16);
    return ret;
}

sint16 ReadSWord()
{
    const sint16 ret = *(sint16*)level_data;
    level_data += sizeof(sint16);
    return ret;
}

uint16 ReadUWord()
{
    const uint16 ret = *(uint16*)level_data;
    level_data += sizeof(uint16);
    return ret;
}

int32 ReadDword()
{
    const int32 ret = *(int32*)level_data;
    level_data += sizeof(int32);
    return ret;
}

sint32 ReadSDword()
{
    const sint32 ret = *(sint32*)level_data;
    level_data += sizeof(sint32);
    return ret;
}

uint32 ReadUDword()
{
    const uint32 ret = *(uint32*)level_data;
    level_data += sizeof(uint32);
    return ret;
}

uint16 FReadWord()
{
    uint16 ret;
    fread(&ret, sizeof(uint16), 1, fp_level);
    return ret;
}

uint32 FReadDword()
{
    uint32 ret;
    fread(&ret, sizeof(uint32), 1, fp_level);
    return ret;
}

void ReadBytes(LPVOID dst, uint32 count)
{
    memcpy(dst, level_data, count);
    level_data += count;
}

void FReadBytes(LPVOID dst, uint32 count)
{
    if (dst == nullptr)
        fseek(fp_level, count, SEEK_CUR);
    else
        fread(dst, sizeof(char), count, fp_level);
}