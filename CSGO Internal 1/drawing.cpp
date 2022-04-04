#include "includes.h"

extern Hack* hack;

RECT rect;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color)
{
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color)
{
	
	if(!hack->LineL)
	D3DXCreateLine(pDevice, &hack->LineL);
	
	D3DXVECTOR2 Line[2];

	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	//hack->LineL->SetAntialias(1);	 ------->fps drops
	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, color);
}

void DrawLine(vec2 src, vec2 dst, int thickness, D3DCOLOR color)
{
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawLineO(vec2 src, vec2 dst, int thickness, D3DCOLOR color)
{
	DrawLine(src.x, src.y, dst.x, dst.y, thickness + 3, D3DCOLOR_ARGB(255, 0, 0, 0));
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(vec2 top, vec2 bot, int thickness, D3DCOLOR color)
{
	int height = ABS(top.y - bot.y);
	vec2 tl, tr;
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;

	vec2 bl, br;
	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;

	DrawLineO(tl, tr, thickness, color);
	DrawLineO(bl, br, thickness, color);
	DrawLineO(tl, bl, thickness, color);
	DrawLineO(tr, br, thickness, color);
}

void DrawEspBox3D(vec3 top, vec3 bot, float a, int width, int thickness, D3DCOLOR color)
{
	int height3D = ABS(top.z - bot.z);
	vec3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.z = b2.z = b3.z = b4.z = bot.z;

	b1.x = bot.x + (cos(TORAD(a + 45)) * width);
	b1.y = bot.y + (sin(TORAD(a + 45)) * width);

	b2.x = bot.x + (cos(TORAD(a + 135)) * width);
	b2.y = bot.y + (sin(TORAD(a + 135)) * width);

	b3.x = bot.x + (cos(TORAD(a + 225)) * width);
	b3.y = bot.y + (sin(TORAD(a + 225)) * width); 

	b4.x = bot.x + (cos(TORAD(a + 315)) * width);
	b4.y = bot.y + (sin(TORAD(a + 315)) * width);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;
	
	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;
	
	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;


	vec2 b1_2, b2_2, b3_2, b4_2, t1_2, t2_2, t3_2, t4_2;
	if(W2S(b1,b1_2)&& W2S(b2, b2_2)&& W2S(b3, b3_2)&& W2S(b4, b4_2)&& W2S(t1, t1_2)&& W2S(t2, t2_2)&& W2S(t3, t3_2)&& W2S(t4, t4_2))
	{
		//columns
		DrawLineO(t1_2, b1_2, thickness, color);
		DrawLineO(t2_2, b2_2, thickness, color);
		DrawLineO(t3_2, b3_2, thickness, color);
		DrawLineO(t4_2, b4_2, thickness, color);

		//top base
		DrawLineO(t1_2, t2_2, thickness, color);
		DrawLineO(t2_2, t3_2, thickness, color);
		DrawLineO(t3_2, t4_2, thickness, color);
		DrawLineO(t4_2, t1_2, thickness, color);

		//bottom base
		DrawLineO(b1_2, b2_2, thickness, color);
		DrawLineO(b2_2, b3_2, thickness, color);
		DrawLineO(b3_2, b4_2, thickness, color);
		DrawLineO(b4_2, b1_2, thickness, color);


	}

}

void DrawTextC(const char* text, float x, float y, D3DCOLOR color)
{
	if (!hack->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas", &hack->FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, BLACK(255));

	SetRect(&rect, x, y, x, y);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}

void DrawTextL(const char* text, float x, float y, D3DCOLOR color)
{
	if (!hack->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas", &hack->FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_LEFT | DT_NOCLIP, color);
}

void DrawRect(int x, int y, int w, int h, int thickness, D3DCOLOR color)
{
	DrawLine(x, y, x + w, y, thickness, color);//top
	DrawLine(x+w, y, x + w, y + h, thickness, color);//right
	DrawLine(x+w, y+h, x, y + h, thickness, color);//bottom
	DrawLine(x, y+h, x, y, thickness, color);//left
}

void DrawRect(vec2 src, int w, int h, int thickness, D3DCOLOR color)
{
	DrawRect(src.x, src.y, w, h, thickness, color);
}

void DrawMenu()
{
	DrawFilledRect(hack->settings.menuX, hack->settings.menuY, hack->settings.menuWidth, hack->settings.menuHeight, D3DCOLOR_ARGB(255, 100, 100, 100));//menu background
	DrawRect(hack->settings.menuX - (hack->settings.menuOutlineThickness / 2), hack->settings.menuY - (hack->settings.menuOutlineThickness / 2), hack->settings.menuWidth, hack->settings.menuHeight,hack->settings.menuOutlineThickness, D3DCOLOR_ARGB(255, 0, 0, 0));
	for (size_t i = 0; i < std::size(hack->settings.features); i++)
	{
		int fX = hack->settings.menuX + hack->settings.fontPaddingX;								 //20 = font padding, 14 = font height;
		int fY = hack->settings.menuY + ((i) * hack->settings.fontPaddingY) + ((i + 1) * hack->settings.fontHeight);
			D3DCOLOR Fcolor;
		
			if (hack->settings.bFeatureStates[i])
				Fcolor = D3DCOLOR_ARGB(255, 0, 255, 0);
			else
				Fcolor = D3DCOLOR_ARGB(255, 255, 0, 0);
		DrawTextL(hack->settings.features[i], fX, fY, Fcolor);//feature name
		if(hack->settings.selectedFeature == i)
		{
			DrawRect(fX - 3, fY-1, 175, hack->settings.fontHeight + 3, 3, D3DCOLOR_ARGB(255, 199, 199, 199));//select box
		}
	}
}