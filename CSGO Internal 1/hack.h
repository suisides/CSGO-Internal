#pragma once

#define STR_MERGE_IMPL(a,b) a##b
#define STR_MERGE(a,b) STR_MERGE_IMPL(a,b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__) [size]
#define DEFINE_MEMBER_N(type,name,offset) struct{unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))
#define TORAD(x) ((x) * 0.01745329252)
#define W2S(x,y) hack->WorldToScreen(x,y)

struct vec2
{
	float x, y;
};

struct vec3
{
	float x, y, z;
};

struct vec4
{
	float x, y, z, w;
};


class Ent
{
public:
	union
	{
		DEFINE_MEMBER_N(bool, isDormant, 0xED);
		DEFINE_MEMBER_N(int, iHealth, 0x100);
		DEFINE_MEMBER_N(vec3, vecOrigin, 0x138);
		DEFINE_MEMBER_N(int, iTeamNum, 0xF4);
		DEFINE_MEMBER_N(int, boneMatrix, 0x26A8);
		DEFINE_MEMBER_N(int, armorValue, 0x117CC);
		DEFINE_MEMBER_N(vec3, aimPunchAngle, 0x303C);
		DEFINE_MEMBER_N(float, angEyeAnglesX, 0x117D0);
		DEFINE_MEMBER_N(float, angEyeAnglesY, 0x117D4);
		DEFINE_MEMBER_N(vec3, vecVelocity, 0x114);
		DEFINE_MEMBER_N(bool, bHasHelmet, 0x117C0);
		
	};
};

class EntListObj
{
public:
	struct Ent* ent;
	char padding[12];

};


class EntList
{
public:
	EntListObj ents[32];

};

class Hack
{
public:
	//offsets
	//=========
	uintptr_t dwEntityList = 0x4DD245C;
	uintptr_t dwViewMatrix = 0x4DC3D74;
	//=========

	uintptr_t engine;
	uintptr_t client;

	Ent* localEnt;
	EntList* entList;
	float viewMatrix[16];

	ID3DXLine* LineL;
	ID3DXFont* FontF;

	~Hack();

	vec2 crosshair2D;
	int crosshairSize = 4;

	void Init();
	void Update();

	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(vec3 pos, vec2& screen);
	vec3 GetBonePos(Ent* ent, int bone);
	vec3 TransformVec(vec3 src, vec3 ang, float d);

	struct Settings
	{
		//GUI shit
		//=======================
		int menuX = 100;
		int menuY = 100;
		int menuOutlineThickness = 4;
		const char* features[9]{ "Num 1. 2D Box:","Num 2. Healthbar info:","Num 3. 3D Box:","Num 4. Text info:","Num 5. Snaplines:","Num 6. RCS Crosshair:","Num 7. Velocity ESP:","Num 8. Traceline:","Num 9. Show Teammates:" };
		bool bFeatureStates[9] = {0,0,0,0,0,0,0,0,0};
		int selectedFeature = 0;

		int fontPaddingX = 15;
		int fontPaddingY = 15;

		int fontHeight = 14;

		int menuWidth = 300;
		int menuHeight = (2 * fontPaddingY) + ((fontHeight * std::size(features)) + (fontPaddingY * std::size(features)));
		//=======================


		bool showMenu = false; //insert
		bool box2D = false;//num1
		bool status2D = false;//num2
		bool box3D = false;//num3
		bool statusText = false;//num4
		bool snaplines = false;//num5
		bool rcsCrosshair = false;//num6
		bool velEsp = false;//num7
		bool headlineEsp = false;//num8
		bool showTeammates = false;//num9


	}settings;

};