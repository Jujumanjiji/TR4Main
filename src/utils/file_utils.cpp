#include "framework.h"
#include "file_utils.h"

uint8_t ReadByte()
{
    const uint8_t ret = *(uint8_t*)level_data;
    level_data += 1;
    return ret;
}

uint16_t ReadWord()
{
    const uint16_t ret = *(uint16_t*)level_data;
    level_data += 2;
    return ret;
}

uint32_t ReadDword()
{
    const uint32_t ret = *(uint32_t*)level_data;
    level_data += 4;
    return ret;
}

uint16_t FReadWord()
{
    uint16_t ret;
    fread(&ret, 2, 1, fp_level);
    return ret;
}

uint32_t FReadDword()
{
    uint32_t ret;
    fread(&ret, 4, 1, fp_level);
    return ret;
}

void ReadBytes(LPVOID dst, uint32_t count)
{
    memcpy(dst, level_data, count);
    level_data += count;
}

void FReadBytes(LPVOID dst, uint32_t count)
{
    if (dst == nullptr)
        fseek(fp_level, count, SEEK_CUR);
    else
        fread(dst, 1, count, fp_level);
}