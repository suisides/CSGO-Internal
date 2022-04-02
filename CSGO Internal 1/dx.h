#pragma once
#include "includes.h"

typedef HRESULT(APIENTRY* tEndScene)(LPDIRECT3DDEVICE9 pDevice);

BOOL CALLBACK enumWind(HWND handle, LPARAM lp);

bool GetD3D9Device(void** pTable, size_t size);

static HWND window;

HWND GetProcessWindow();

extern int windowHeight, windowWidth;

extern LPDIRECT3DDEVICE9 pDevice;