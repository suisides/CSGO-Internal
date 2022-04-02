#include "includes.h"

//data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;


//hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice)
{
    if (!pDevice)
        pDevice = o_pDevice;

    //settings / hotkeys
    if (GetAsyncKeyState(VK_NUMPAD1) & 1)//2d esp
        hack->settings.box2D = !hack->settings.box2D;

    if (GetAsyncKeyState(VK_NUMPAD2) & 1)//2d esp healthbars
        hack->settings.status2D = !hack->settings.status2D;

    if (GetAsyncKeyState(VK_NUMPAD3) & 1)//3d esp
        hack->settings.box3D = !hack->settings.box3D;

    if (GetAsyncKeyState(VK_NUMPAD4) & 1)//text status
        hack->settings.statusText = !hack->settings.statusText;

    if (GetAsyncKeyState(VK_NUMPAD5) & 1)//snaplines
        hack->settings.snaplines = !hack->settings.snaplines;

    if (GetAsyncKeyState(VK_NUMPAD6) & 1)//rcs crosshair
        hack->settings.rcsCrosshair = !hack->settings.rcsCrosshair;

    if (GetAsyncKeyState(VK_NUMPAD7) & 1)//vel Esp
        hack->settings.velEsp = !hack->settings.velEsp;

    if (GetAsyncKeyState(VK_NUMPAD8) & 1)//headline esp
        hack->settings.headlineEsp = !hack->settings.headlineEsp;

    if (GetAsyncKeyState(VK_NUMPAD9) & 1)//show Teammates
        hack->settings.showTeammates = !hack->settings.showTeammates;

    //drawing


    if (hack->settings.rcsCrosshair) 
    {
        vec2 l, r, t, b;
        l = r = t = b = hack->crosshair2D;
        l.x -= hack->crosshairSize;
        r.x += hack->crosshairSize;
        b.y += hack->crosshairSize;
        t.y -= hack->crosshairSize;
        DrawLine(l, r, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
        DrawLine(t, b, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    for(int i = 1; i < 32; i++)
    {
        Ent* curEnt = hack->entList->ents[i].ent;
        if (!hack->CheckValidEnt(curEnt))
            continue;
    
        D3DCOLOR color;
        if (curEnt->iTeamNum == hack->localEnt->iTeamNum)
            color = D3DCOLOR_ARGB(255, 0, 255, 0);
        else
            color = D3DCOLOR_ARGB(255, 255, 0, 0);

        if (!hack->settings.showTeammates && (curEnt->iTeamNum == hack->localEnt->iTeamNum))
            continue;

        vec3 entHead3D = hack->GetBonePos(curEnt, 8);
        entHead3D.z += 5;
        vec2 entPos2D, entHead2D;
        if (hack->WorldToScreen(curEnt->vecOrigin, entPos2D))
        {
            if (hack->settings.snaplines)
            {
                DrawLine(entPos2D.x, entPos2D.y, windowWidth / 2, windowHeight, 2, color);
            }

            if (hack->WorldToScreen(entHead3D, entHead2D))
            {
                
                //2D esp
                /*if (hack->settings.box2D)
                {
                    DrawEspBox2D(entPos2D, entHead2D, 2, color);
                }*/
                
                //healthbars
                if (hack->settings.status2D && hack->settings.box2D)
                {
                    int height = ABS(entPos2D.y - entHead2D.y);
                    int dX = (entPos2D.x - entHead2D.x);

                    float healthPerc = curEnt->iHealth / 100.f;
                    float armorPerc = curEnt->armorValue / 100.f;

                    vec2 botHealth, topHealth, botArmor, topArmor;
                    int healthHeight = height * healthPerc;
                    int armorHeight = height * armorPerc;

                    botHealth.y = botArmor.y = entPos2D.y;

                    botHealth.x = entPos2D.x - (height / 4) - 2;
                    botArmor.x = entPos2D.x + (height / 4) + 2;

                    topHealth.y = entHead2D.y + height - healthHeight;
                    topArmor.y = entHead2D.y + height - armorHeight;

                    topHealth.x = entPos2D.x - (height / 4) - 2 - (dX * healthPerc);
                    topArmor.x = entPos2D.x + (height / 4) + 2 - (dX * healthPerc);

                    DrawLine(botHealth, topHealth, 2, D3DCOLOR_ARGB(255, 46, 139, 87));
                    DrawLine(botArmor, topArmor, 2, D3DCOLOR_ARGB(255, 30, 144, 255));
                }

                //3D esp
                if (hack->settings.box3D)
                {
                    DrawEspBox3D(entHead3D, curEnt->vecOrigin, curEnt->angEyeAnglesY, 25, 2, color);
                }
                
                //headline
                if(hack->settings.headlineEsp)
                {
                    vec3 head3D = hack->GetBonePos(curEnt, 8);
                    vec3 entAngles;
                    entAngles.x = curEnt->angEyeAnglesX;
                    entAngles.y = curEnt->angEyeAnglesY;
                    entAngles.z = 0;
                    vec3 endPoint = hack->TransformVec(head3D, entAngles, 60);
                    vec2 endPoint2D, head2D;
                    hack->WorldToScreen(head3D, head2D);
                    if (hack->WorldToScreen(endPoint, endPoint2D))
                    {
                        DrawLine(head2D, endPoint2D, 2, color);
                    }
                }
            }
        }
    }


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
        hack->Update();
        vec3 pAng = hack->localEnt->aimPunchAngle;
        hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
        hack->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);
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