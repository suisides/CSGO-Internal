#include "includes.h"

Hack::~Hack()
{
	this->FontF->Release();
	this->LineL->Release();
}

void Hack::Init()
{
	client = (uintptr_t)GetModuleHandle("client.dll");
	engine = (uintptr_t)GetModuleHandle("engine.dll");
	entList = (EntList*)(client + (uintptr_t)offsets::dwEntityList);
	localEnt = entList->ents[0].ent;
}

void Hack::Update()
{
	memcpy(&viewMatrix, (PBYTE*)(client + (uintptr_t)offsets::dwViewMatrix), sizeof(viewMatrix));
}

void Hack::UpdateKeystates()
{
	settings.box2D = settings.bFeatureStates[settings.bFeatureStates[0]];
	settings.status2D = settings.bFeatureStates[settings.bFeatureStates[1]];
	settings.box3D = settings.bFeatureStates[settings.bFeatureStates[2]];
	settings.statusText = settings.bFeatureStates[settings.bFeatureStates[3]];
	settings.snaplines = settings.bFeatureStates[settings.bFeatureStates[4]];
	settings.rcsCrosshair = settings.bFeatureStates[settings.bFeatureStates[5]];
	settings.velEsp = settings.bFeatureStates[settings.bFeatureStates[6]];
	settings.headlineEsp = settings.bFeatureStates[settings.bFeatureStates[7]];
	settings.showTeammates = settings.bFeatureStates[settings.bFeatureStates[8]];
}

vec3 Hack::GetOrigin(Ent* ent)
{
	return ent->vecOrigin;
}

vec2 Hack::GetViewOffset(Ent* ent)
{
	return ent->vecViewOffset;
}

int Hack::GetHealth(Ent* ent)
{
	return ent->iHealth;
}

int Hack::GetTeam(Ent* ent)
{
	return ent->iTeamNum;
}

float Hack::GetDistance(Ent* ent)
{
	//ent pos
	vec3 entPos = ent->vecOrigin;

	//self pos
	vec3 selfPos = localEnt->vecOrigin;

	float x = ABS(entPos.x - selfPos.x);
	float y = ABS(entPos.y - selfPos.y);
	float z = ABS(entPos.z - selfPos.z);


	//pythagoras
	return sqrt(x * x + y * y + z * z);

}

Ent* Hack::GetClosestEnemy()
{
	float distance = FLT_MAX;
	Ent* closestEnemy;
	for (size_t i = 0; i < 32; i++)
	{
		Ent* curEnt = entList->ents[i].ent;
		if (!CheckValidEnt(curEnt))
			continue;

		if (GetDistance(curEnt) < distance)
		{
			distance = GetDistance(curEnt);
			closestEnemy = curEnt;
		}
	}
	return closestEnemy;
}

vec3 Hack::GetClosestEnemyPos()
{
	float distance = FLT_MAX;
	vec3 pos;
	for (size_t i = 0; i < 32; i++)
	{
		Ent* curEnt = entList->ents[i].ent;
		if (!CheckValidEnt(curEnt))
			continue;

		if (GetDistance(curEnt) < distance)
		{
			distance = GetDistance(curEnt);
			pos = curEnt->vecOrigin;
		}
	}
	return pos;
}

bool Hack::CheckValidEnt(Ent* ent)
{
	if (ent == nullptr) return false;
	if (ent == localEnt) return false;
	if (ent->iHealth < 1) return false;
	if (ent->isDormant) return false;
	return true;
}

bool Hack::WorldToScreen(vec3 pos, vec2& screen)
{
	vec4 clipCoords;
	clipCoords.x = pos.x * viewMatrix[0] + pos.y * viewMatrix[1] + pos.z * viewMatrix[2] + viewMatrix[3];
	clipCoords.y = pos.x * viewMatrix[4] + pos.y * viewMatrix[5] + pos.z * viewMatrix[6] + viewMatrix[7];
	clipCoords.z = pos.x * viewMatrix[8] + pos.y * viewMatrix[9] + pos.z * viewMatrix[10] + viewMatrix[11];
	clipCoords.w = pos.x * viewMatrix[12] + pos.y * viewMatrix[13] + pos.z * viewMatrix[14] + viewMatrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;
}

vec3 Hack::GetBonePos(Ent* ent, int bone)
{
	uintptr_t bonePtr = ent->boneMatrix;
	vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);
	return bonePos;
}

vec3 Hack::TransformVec(vec3 src, vec3 ang, float d)
{
	vec3 newPos;
	newPos.x = src.x + (cosf(TORAD(ang.y)) * d);
	newPos.y = src.y + (sinf(TORAD(ang.y)) * d);
	newPos.z = src.z + (tanf(TORAD(ang.x)) * d);
	return newPos;
}

void Hack::AimAt(vec3 dst)
{
	static vec3* viewAngles = (vec3*)(*(uint32_t*)(localEnt)+(offsets::dwClientState_ViewAngles));

	vec3 origin = GetOrigin(localEnt);
	vec2 viewOffset = GetViewOffset(localEnt);
	vec3 myPos = localEnt->vecOrigin;

	vec3 deltaVec = { dst.x - localEnt->vecOrigin.x,dst.y - localEnt->vecOrigin.y,dst.z - localEnt->vecOrigin.z };
	float deltaVecLength = sqrt(deltaVec.x * deltaVec.x + deltaVec.y * deltaVec.y + deltaVec.z * deltaVec.z);

	float pitch = asin(deltaVec.z / deltaVecLength) * (180 / PI);
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);

	viewAngles->x = -pitch;
	viewAngles->y = yaw;
}


