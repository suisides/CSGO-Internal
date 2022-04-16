#include "includes.h"
#include <sstream>
#include <string>

//data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;
int i_fontSize = 14;
vec3 pAng;
vec2 snaplineWindow;

//hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
    if (!pDevice)
        pDevice = o_pDevice;
      
    DrawTextC("pipipupu's hack", 100, 50, D3DCOLOR_ARGB(255, 123, 123, 123));//watermark

    if (!hack->localEnt) 
        oEndScene(pDevice);

    //settings | hotkeys
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)//2d esp
    {
        hack->settings.box2D = hack->settings.bFeatureStates[0] = !hack->settings.box2D;
        if (hack->settings.box3D && hack->settings.box2D)
        {
            hack->settings.box3D = false;
            hack->settings.bFeatureStates[2] = false;
        }
    }
    
    if (GetAsyncKeyState(VK_NUMPAD2) & 1)//2d esp healthbars
    {  
        hack->settings.status2D = hack->settings.bFeatureStates[1] = !hack->settings.status2D;
    }

    if (GetAsyncKeyState(VK_NUMPAD3) & 1)//3d esp
    {
        hack->settings.box3D = hack->settings.bFeatureStates[2] = !hack->settings.box3D;
        if (hack->settings.box3D && hack->settings.box2D)
        {
            hack->settings.box2D = false;
            hack->settings.bFeatureStates[0] = false;
        }
    }
    
    if (GetAsyncKeyState(VK_NUMPAD4) & 1)//text status
    {
        hack->settings.statusText = hack->settings.bFeatureStates[3] = !hack->settings.statusText;
    }
    
    if (GetAsyncKeyState(VK_NUMPAD5) & 1)//snaplines
    {
        hack->settings.snaplines = hack->settings.bFeatureStates[4] = !hack->settings.snaplines;
    }
    
    if (GetAsyncKeyState(VK_NUMPAD6) & 1)//rcs crosshair
    {
        hack->settings.rcsCrosshair = hack->settings.bFeatureStates[5] = !hack->settings.rcsCrosshair;
    }
    
    if (GetAsyncKeyState(VK_NUMPAD7) & 1)//vel Esp
    {
        hack->settings.velEsp = hack->settings.bFeatureStates[6] = !hack->settings.velEsp;
    }
    
    if (GetAsyncKeyState(VK_NUMPAD8) & 1)//headline esp
    {
        hack->settings.headlineEsp = hack->settings.bFeatureStates[7] = !hack->settings.headlineEsp;
    }
    
    if (GetAsyncKeyState(VK_NUMPAD9) & 1)//show Teammates
    {
        hack->settings.showTeammates = hack->settings.bFeatureStates[8] = !hack->settings.showTeammates;
    }

    if (GetAsyncKeyState(VK_MULTIPLY) &1)
    {
        hack->settings.aimbot = hack->settings.bFeatureStates[9] = !hack->settings.aimbot;
    }
    
    if (GetAsyncKeyState(VK_UP) & 1 && hack->settings.showMenu)//show Teammates
    {
        hack->settings.selectedFeature--;
        if (hack->settings.selectedFeature < 0)
            hack->settings.selectedFeature = 8;
    }
    
    if (GetAsyncKeyState(VK_DOWN) & 1 && hack->settings.showMenu)//show Teammates
    {
        hack->settings.selectedFeature++;
        if (hack->settings.selectedFeature > 8)
            hack->settings.selectedFeature = 0;
    }

    if (GetAsyncKeyState(VK_RETURN) & 1 && hack->settings.showMenu)
    {
        hack->settings.bFeatureStates[hack->settings.selectedFeature] = !hack->settings.bFeatureStates[hack->settings.selectedFeature]; // change color

        hack->UpdateKeystates();
    }
    
    if (GetAsyncKeyState(VK_INSERT) & 1)//show Teammates
    hack->settings.showMenu = !hack->settings.showMenu;
    
    //drawing
    
    
    

    if (hack->settings.rcsCrosshair) 
    {
        vec2 l, r, t, b;
        l = r = t = b = hack->crosshair2D;
        l.x -= hack->crosshairSize;
        r.x += hack->crosshairSize;
        b.y += hack->crosshairSize;
        t.y -= hack->crosshairSize;
        DrawLineO(l, r, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
        DrawLineO(t, b, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    for (int i = 1; i < 32; i++);

    if (GetAsyncKeyState(VK_XBUTTON1))
        hack->AimAt(hack->GetClosestEnemyPos());

    if (hack->settings.showMenu)
        DrawMenu();

    oEndScene(pDevice);
}



DWORD WINAPI HackThread(HMODULE hModule)
{
    //hook
    if(GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
    {
        memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

        oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
    }
    
    hack = new Hack();
    hack->Init();


    //hack loop
    while (!GetAsyncKeyState(VK_END))
    {
        if (hack->localEnt) 
        {
            hack->Update();
            pAng = hack->localEnt->aimPunchAngle;
            hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
            hack->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);
        }
    }

    //unhook
    Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

    Sleep(1000);

    //uninject
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
        return TRUE;
}