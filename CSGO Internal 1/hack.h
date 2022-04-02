#pragma once

#define STR_MERGE_IMPL(a,b) a##b
#define STR_MERGE(a,b) STR_MERGE_IMPL(a,b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__) [size]
#define DEFINE_MEMBER_N(type,name,offset) struct{unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))
#define TORAD(x) ((x)* 0.01745329252)
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
	uintptr_t dwEntityList = 0x4DD245C;
	uintptr_t dwViewMatrix = 0x4DC3D74;

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
		bool box2D = true;//num1
		bool status2D = true;//num2
		bool box3D = true;//num3
		bool statusText = true;//num4
		bool snaplines = true;//num5
		bool rcsCrosshair = true;//num6
		bool velEsp = true;//num7
		bool headlineEsp = true;//num8
		bool showTeammates = true;//num9


	}settings;

};