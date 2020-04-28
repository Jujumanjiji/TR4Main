#pragma once

#ifdef DLL_INJECT
#define DXCopyBitmap ((HRESULT(__cdecl*)(LPDIRECTDRAWSURFACE4 lpSurface, LPCSTR fileName)) 0x00480850)
#endif
