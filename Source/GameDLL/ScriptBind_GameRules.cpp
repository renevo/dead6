/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2004.
-------------------------------------------------------------------------
$Id$
$DateTime$

-------------------------------------------------------------------------
History:
- 27:10:2004   11:29 : Created by M�rcio Martins

*************************************************************************/
#include "StdAfx.h"
#include "ScriptBind_GameRules.h"
#include "GameRules.h"
#include "Actor.h"
#include "Game.h"
#include "GameCVars.h"
#include "MPTutorial.h"

//------------------------------------------------------------------------
CScriptBind_GameRules::CScriptBind_GameRules(ISystem *pSystem, IGameFramework *pGameFramework)
: m_pSystem(pSystem),
	m_pSS(pSystem->GetIScriptSystem()),
	m_pGameFW(pGameFramework)
{
	Init(m_pSS, m_pSystem, 1);

	m_players.Create(m_pSS);
	m_teamplayers.Create(m_pSS);
	m_spawnlocations.Create(m_pSS);
	m_spawngroups.Create(m_pSS);
	m_spectatorlocations.Create(m_pSS);

	RegisterMethods();
	RegisterGlobals();
}

//------------------------------------------------------------------------
CScriptBind_GameRules::~CScriptBind_GameRules()
{
}

//------------------------------------------------------------------------
void CScriptBind_GameRules::RegisterGlobals()
{
	m_pSS->SetGlobalValue("TextMessageCenter", eTextMessageCenter);
	m_pSS->SetGlobalValue("TextMessageConsole", eTextMessageConsole);
	m_pSS->SetGlobalValue("TextMessageError", eTextMessageError);
	m_pSS->SetGlobalValue("TextMessageInfo", eTextMessageInfo);
	m_pSS->SetGlobalValue("TextMessageServer", eTextMessageServer);

	m_pSS->SetGlobalValue("ChatToTarget", eChatToTarget);
	m_pSS->SetGlobalValue("ChatToTeam", eChatToTeam);
	m_pSS->SetGlobalValue("ChatToAll", eChatToAll);

	m_pSS->SetGlobalValue("TextMessageToAll", eRMI_ToAllClients);
	m_pSS->SetGlobalValue("TextMessageToAllRemote", eRMI_ToRemoteClients);
	m_pSS->SetGlobalValue("TextMessageToClient", eRMI_ToClientChannel);

 	m_pSS->SetGlobalValue("eTE_TurretUnderAttack", eTE_TurretUnderAttack);
	m_pSS->SetGlobalValue("eTE_TurretDestroyed", eTE_TurretDestroyed);
 	m_pSS->SetGlobalValue("eTE_GameOverWin", eTE_GameOverWin);
 	m_pSS->SetGlobalValue("eTE_GameOverLose", eTE_GameOverLose);
 	m_pSS->SetGlobalValue("eTE_TACTankStarted", eTE_TACTankStarted);
 	m_pSS->SetGlobalValue("eTE_SingularityStarted", eTE_SingularityStarted);
 	m_pSS->SetGlobalValue("eTE_TACTankCompleted", eTE_TACTankCompleted);
	m_pSS->SetGlobalValue("eTE_TACLauncherCompleted", eTE_TACLauncherCompleted);
 	m_pSS->SetGlobalValue("eTE_SingularityCompleted", eTE_SingularityCompleted);
	m_pSS->SetGlobalValue("eTE_EnemyNearBase", eTE_EnemyNearBase);
	m_pSS->SetGlobalValue("eTE_Promotion", eTE_Promotion);
}

//------------------------------------------------------------------------
void CScriptBind_GameRules::RegisterMethods()
{
#undef SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_CLASSNAME &CScriptBind_GameRules::

	SCRIPT_REG_TEMPLFUNC(IsServer, "");
	SCRIPT_REG_TEMPLFUNC(IsClient, "");
	SCRIPT_REG_TEMPLFUNC(CanCheat, "");

	SCRIPT_REG_TEMPLFUNC(SpawnPlayer, "channelId, name, className, pos, angles");
	SCRIPT_REG_TEMPLFUNC(ChangePlayerClass, "channelId, className, pos, angles");
	SCRIPT_REG_TEMPLFUNC(RevivePlayer, "playerId, pos, angles, teamId, clearInventory");
	SCRIPT_REG_TEMPLFUNC(RevivePlayerInVehicle, "playerId, vehicleId, seatId, teamId, clearInventory");
	SCRIPT_REG_TEMPLFUNC(RenamePlayer, "playerId, name");
	SCRIPT_REG_TEMPLFUNC(KillPlayer, "playerId, dropItem, ragdoll, shooterId, weaponId, damage, material, headshot, impulse");
	SCRIPT_REG_TEMPLFUNC(MovePlayer, "playerId, pos, angles");
	SCRIPT_REG_TEMPLFUNC(GetPlayerByChannelId, "channelId");
	SCRIPT_REG_TEMPLFUNC(GetPlayerCount, "");
	SCRIPT_REG_TEMPLFUNC(GetSpectatorCount, "");
	SCRIPT_REG_TEMPLFUNC(GetPlayers, "");
	SCRIPT_REG_TEMPLFUNC(IsPlayerInGame, "playerId");
	
	SCRIPT_REG_TEMPLFUNC(AddSpawnLocation, "entityId");
	SCRIPT_REG_TEMPLFUNC(RemoveSpawnLocation, "id");
	SCRIPT_REG_TEMPLFUNC(GetSpawnLocationCount, "");
	SCRIPT_REG_TEMPLFUNC(GetSpawnLocationByIdx, "idx");
	SCRIPT_REG_TEMPLFUNC(GetSpawnLocations, "");
	SCRIPT_REG_TEMPLFUNC(IsSpawnLocationSafe, "playerId, spawnLocationId, safeDistance, ignoreTeam");
	SCRIPT_REG_TEMPLFUNC(GetSpawnLocation, "playerId, teamId, safeDistance, ignoreTeam, includeNeutral");
	SCRIPT_REG_TEMPLFUNC(GetFirstSpawnLocation, "teamId");

	SCRIPT_REG_TEMPLFUNC(AddSpawnGroup, "groupId");
	SCRIPT_REG_TEMPLFUNC(AddSpawnLocationToSpawnGroup, "groupId, location");
	SCRIPT_REG_TEMPLFUNC(RemoveSpawnLocationFromSpawnGroup, "groupId, location");
	SCRIPT_REG_TEMPLFUNC(RemoveSpawnGroup, "groupId");
	SCRIPT_REG_TEMPLFUNC(GetSpawnLocationGroup, "spawnId");
	SCRIPT_REG_TEMPLFUNC(GetSpawnGroups, "");

	SCRIPT_REG_TEMPLFUNC(GetTeamDefaultSpawnGroup, "teamId");
	SCRIPT_REG_TEMPLFUNC(SetTeamDefaultSpawnGroup, "teamId, groupId");
	SCRIPT_REG_TEMPLFUNC(SetPlayerSpawnGroup, "playerId, groupId");

	SCRIPT_REG_TEMPLFUNC(AddSpectatorLocation, "location");
	SCRIPT_REG_TEMPLFUNC(RemoveSpectatorLocation, "id");
	SCRIPT_REG_TEMPLFUNC(GetSpectatorLocationCount, "");
	SCRIPT_REG_TEMPLFUNC(GetSpectatorLocation, "idx");
	SCRIPT_REG_TEMPLFUNC(GetSpectatorLocations, "");
	SCRIPT_REG_TEMPLFUNC(GetRandomSpectatorLocation, "");
	SCRIPT_REG_TEMPLFUNC(GetInterestingSpectatorLocation, "");
	SCRIPT_REG_TEMPLFUNC(GetNextSpectatorTarget, "playerId, change");

	SCRIPT_REG_TEMPLFUNC(AddMinimapEntity, "entityId, type, lifetime");
	SCRIPT_REG_TEMPLFUNC(RemoveMinimapEntity, "entityId");
	SCRIPT_REG_TEMPLFUNC(ResetMinimap, "");

	SCRIPT_REG_TEMPLFUNC(GetPing, "channelId");
	SCRIPT_REG_TEMPLFUNC(ResetEntities, "");

	SCRIPT_REG_TEMPLFUNC(ServerExplosion, "shooterId, weaponId, dmg, pos, dir, radius, angle, press, holesize, [effect], [effectScale]");
	SCRIPT_REG_TEMPLFUNC(ServerHit, "targetId, shooterId, weaponId, dmg, radius, materialId, partId, typeId, [pos], [dir], [normal]");

	SCRIPT_REG_TEMPLFUNC(CreateTeam, "name");
	SCRIPT_REG_TEMPLFUNC(RemoveTeam, "teamId");
	SCRIPT_REG_TEMPLFUNC(GetTeamName, "teamId");
	SCRIPT_REG_TEMPLFUNC(GetTeamId, "teamName");
	SCRIPT_REG_TEMPLFUNC(GetTeamCount, "");
	SCRIPT_REG_TEMPLFUNC(GetTeamPlayerCount, "teamId");
	SCRIPT_REG_TEMPLFUNC(GetTeamPlayers, "teamId");

	SCRIPT_REG_TEMPLFUNC(SetTeam, "teamId, playerId");
	SCRIPT_REG_TEMPLFUNC(GetTeam, "playerId");

	SCRIPT_REG_TEMPLFUNC(AddObjective, "teamId, objective, status, entityId");
	SCRIPT_REG_TEMPLFUNC(SetObjectiveStatus, "teamId, objective, status");
	SCRIPT_REG_TEMPLFUNC(SetObjectiveEntity, "teamId, objective, entityId");
	SCRIPT_REG_TEMPLFUNC(RemoveObjective, "teamId, objective");
	SCRIPT_REG_TEMPLFUNC(ResetObjectives, "");

	SCRIPT_REG_TEMPLFUNC(TextMessage, "type, msg");
	SCRIPT_REG_TEMPLFUNC(SendTextMessage, "type, msg");
	SCRIPT_REG_TEMPLFUNC(SendChatMessage, "type, sourceId, targetId, msg");

	SCRIPT_REG_TEMPLFUNC(ForbiddenAreaWarning, "active, timer, targetId");

	SCRIPT_REG_TEMPLFUNC(ResetGameTime, "");
	SCRIPT_REG_TEMPLFUNC(GetRemainingGameTime, "");
	SCRIPT_REG_TEMPLFUNC(IsTimeLimited, "");

	SCRIPT_REG_TEMPLFUNC(ResetRoundTime, "");
	SCRIPT_REG_TEMPLFUNC(GetRemainingRoundTime, "");
	SCRIPT_REG_TEMPLFUNC(IsRoundTimeLimited, "");

	SCRIPT_REG_TEMPLFUNC(ResetPreRoundTime, "");
	SCRIPT_REG_TEMPLFUNC(GetRemainingPreRoundTime, "");

	SCRIPT_REG_TEMPLFUNC(EndGame, "");
	SCRIPT_REG_TEMPLFUNC(NextLevel, "");

	SCRIPT_REG_TEMPLFUNC(RegisterHitMaterial, "materialName");
	SCRIPT_REG_TEMPLFUNC(GetHitMaterialId, "materialName");
	SCRIPT_REG_TEMPLFUNC(ResetHitMaterials, "");

	SCRIPT_REG_TEMPLFUNC(RegisterHitType, "type");
	SCRIPT_REG_TEMPLFUNC(GetHitTypeId, "type");
	SCRIPT_REG_TEMPLFUNC(GetHitType, "id");
	SCRIPT_REG_TEMPLFUNC(ResetHitTypes, "");

	SCRIPT_REG_TEMPLFUNC(ForceScoreboard, "force");
	SCRIPT_REG_TEMPLFUNC(FreezeInput, "freeze");

	SCRIPT_REG_TEMPLFUNC(ScheduleEntityRespawn, "entityId, unique, timer");
	SCRIPT_REG_TEMPLFUNC(AbortEntityRespawn, "entityId, destroyData");

	SCRIPT_REG_TEMPLFUNC(ScheduleEntityRemoval, "entityId, timer, visibility");
	SCRIPT_REG_TEMPLFUNC(AbortEntityRemoval, "entityId");

	SCRIPT_REG_TEMPLFUNC(SetSynchedGlobalValue, "key, value");
	SCRIPT_REG_TEMPLFUNC(GetSynchedGlobalValue, "key");

	SCRIPT_REG_TEMPLFUNC(SetSynchedEntityValue, "entityId, key, value");
	SCRIPT_REG_TEMPLFUNC(GetSynchedEntityValue, "entityId, key");

	SCRIPT_REG_TEMPLFUNC(ResetSynchedStorage, "");

	SCRIPT_REG_TEMPLFUNC(IsDemoMode, "");
	SCRIPT_REG_TEMPLFUNC(GetTimeLimit, "");
	SCRIPT_REG_TEMPLFUNC(GetPreRoundTime, "");
	SCRIPT_REG_TEMPLFUNC(GetRoundTime, "");
	SCRIPT_REG_TEMPLFUNC(GetSuddenDeathTime, "");
	SCRIPT_REG_TEMPLFUNC(GetRoundLimit, "");
	SCRIPT_REG_TEMPLFUNC(GetFragLimit, "");
	SCRIPT_REG_TEMPLFUNC(GetFragLead, "");
  SCRIPT_REG_TEMPLFUNC(GetFriendlyFireRatio, "");
  SCRIPT_REG_TEMPLFUNC(GetReviveTime, "");
	SCRIPT_REG_TEMPLFUNC(GetMinPlayerLimit, "");
	SCRIPT_REG_TEMPLFUNC(GetMinTeamLimit, "");

	SCRIPT_REG_TEMPLFUNC(IsFrozen, "entityId");
	SCRIPT_REG_TEMPLFUNC(FreezeEntity, "entityId, freeze, vapor");
	SCRIPT_REG_TEMPLFUNC(ShatterEntity, "entityId, pos, impulse");

  SCRIPT_REG_TEMPLFUNC(DebugCollisionDamage, "");

	SCRIPT_REG_TEMPLFUNC(RequestHitIndicator, "shooterId");
	SCRIPT_REG_TEMPLFUNC(RequestDamageIndicator, "shooterId");
	SCRIPT_REG_TEMPLFUNC(TutorialEvent, "type");

	SCRIPT_REG_TEMPLFUNC(GameOver, "localWinner");
	SCRIPT_REG_TEMPLFUNC(EnteredGame, "");
	SCRIPT_REG_TEMPLFUNC(EndGameNear, "entityId");
}

//------------------------------------------------------------------------
CGameRules *CScriptBind_GameRules::GetGameRules(IFunctionHandler *pH)
{
	return static_cast<CGameRules *>(m_pGameFW->GetIGameRulesSystem()->GetCurrentGameRules());
}

//------------------------------------------------------------------------
CActor *CScriptBind_GameRules::GetActor(EntityId id)
{
	return static_cast<CActor *>(m_pGameFW->GetIActorSystem()->GetActor(id));
}

//------------------------------------------------------------------------
void CScriptBind_GameRules::AttachTo(CGameRules *pGameRules)
{
	IScriptTable *pScriptTable = pGameRules->GetEntity()->GetScriptTable();

	if (pScriptTable)
	{
		SmartScriptTable thisTable(m_pSS);
		thisTable->Delegate(GetMethodsTable());

		pScriptTable->SetValue("game", thisTable);
	}
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsServer(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);
	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(gEnv->bServer);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsClient(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);
	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(gEnv->bClient);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::CanCheat(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);
	if (!pGameRules)
		return pH->EndFunction();

	if (g_pGame->GetIGameFramework()->CanCheat())
		return pH->EndFunction(1);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SpawnPlayer(IFunctionHandler *pH, int channelId, const char *name, const char *className, Vec3 pos, Vec3 angles)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = pGameRules->SpawnPlayer(channelId, name, className, pos, Ang3(angles));

	if (pActor)
		return pH->EndFunction(pActor->GetEntity()->GetScriptTable());

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ChangePlayerClass(IFunctionHandler *pH, int channelId, const char *className)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = pGameRules->ChangePlayerClass(channelId, className);

	if (pActor)
		return pH->EndFunction(pActor->GetEntity()->GetScriptTable());

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RevivePlayer(IFunctionHandler *pH, ScriptHandle playerId, Vec3 pos, Vec3 angles, int teamId, bool clearInventory)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = GetActor((EntityId)playerId.n);

	if (pActor)
		pGameRules->RevivePlayer(pActor, pos, Ang3(angles), teamId, clearInventory);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RevivePlayerInVehicle(IFunctionHandler *pH, ScriptHandle playerId, ScriptHandle vehicleId, int seatId, int teamId, bool clearInventory)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = GetActor((EntityId)playerId.n);

	if (pActor)
		pGameRules->RevivePlayerInVehicle(pActor, (EntityId)vehicleId.n, seatId, teamId, clearInventory);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RenamePlayer(IFunctionHandler *pH, ScriptHandle playerId, const char *name)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = GetActor((EntityId)playerId.n);

	if (pActor)
		pGameRules->RenamePlayer(pActor, name);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::KillPlayer(IFunctionHandler *pH, ScriptHandle playerId, bool dropItem, bool ragdoll,
																			ScriptHandle shooterId, ScriptHandle weaponId, float damage, int material, bool headshot, Vec3 impulse)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = GetActor((EntityId)playerId.n);

	if (pActor)
		pGameRules->KillPlayer(pActor, dropItem, ragdoll, (EntityId)shooterId.n, (EntityId)weaponId.n, damage, material, headshot, impulse);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::MovePlayer(IFunctionHandler *pH, ScriptHandle playerId, Vec3 pos, Vec3 angles)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = GetActor((EntityId)playerId.n);

	if (pActor)
		pGameRules->MovePlayer(pActor, pos, Ang3(angles));

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetPlayerByChannelId(IFunctionHandler *pH, int channelId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	CActor *pActor = pGameRules->GetActorByChannelId(channelId);
	if (pActor)
		return pH->EndFunction(pActor->GetEntity()->GetScriptTable());

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetPlayerCount(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	bool inGame=false;
	if (pH->GetParamCount()>0)
		pH->GetParam(1, inGame);

	return pH->EndFunction(pGameRules->GetPlayerCount(inGame));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpectatorCount(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	bool inGame=false;
	if (pH->GetParamCount()>0)
		pH->GetParam(1, inGame);

	return pH->EndFunction(pGameRules->GetSpectatorCount(inGame));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetPlayers(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();
 
	int count=pGameRules->GetPlayerCount();
	if (!count)
		return pH->EndFunction();

	int tcount=m_players->Count();

	int i=0;
	while(i<count)
	{
		IEntity *pEntity=gEnv->pEntitySystem->GetEntity(pGameRules->GetPlayer(i));
		if (pEntity)
		{
			IScriptTable *pEntityScript=pEntity->GetScriptTable();
			if (pEntityScript)
				m_players->SetAt(i+1, pEntityScript);
			else
				m_players->SetNullAt(i+1);
		}
		++i;
	}

	while(i<tcount)
		m_players->SetNullAt(++i);

	return pH->EndFunction(m_players);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsPlayerInGame(IFunctionHandler *pH, ScriptHandle playerId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules && pGameRules->IsPlayerInGame((EntityId)playerId.n))
		return pH->EndFunction(true);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AddSpawnLocation(IFunctionHandler *pH, ScriptHandle entityId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->AddSpawnLocation((EntityId)entityId.n);
	
	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RemoveSpawnLocation(IFunctionHandler *pH, ScriptHandle id)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->RemoveSpawnLocation((EntityId)id.n);
	
	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpawnLocationCount(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		return pH->EndFunction(pGameRules->GetSpawnLocationCount());

	return pH->EndFunction(0);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpawnLocationByIdx(IFunctionHandler *pH, int idx)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
	{
		EntityId id = pGameRules->GetSpawnLocation(idx);
		if (id)
			return pH->EndFunction(ScriptHandle(id));
	}
	
	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpawnLocations(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	int count=pGameRules->GetSpawnLocationCount();
	if (!count)
		return pH->EndFunction();

	int tcount=m_spawnlocations->Count();

	int i=0;
	while(i<count)
	{
		m_spawnlocations->SetAt(i, ScriptHandle(pGameRules->GetSpawnLocation(i)));
		++i;
	}

	while(i<tcount)
		m_spawnlocations->SetNullAt(++i);

	return pH->EndFunction(m_spawnlocations);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsSpawnLocationSafe(IFunctionHandler *pH, ScriptHandle playerId, ScriptHandle spawnLocationId, float safeDistance, bool ignoreTeam)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		return pH->EndFunction(pGameRules->IsSpawnLocationSafe(
			(EntityId)playerId.n, (EntityId)spawnLocationId.n, safeDistance, ignoreTeam));
	
	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpawnLocation(IFunctionHandler *pH, ScriptHandle playerId, float safeDistance, bool ignoreTeam, bool includeNeutral)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
	{
		EntityId groupId=0;
		if (pH->GetParamCount()>4 && pH->GetParamType(5)==svtPointer)
		{
			ScriptHandle groupIdHdl;
			pH->GetParam(5, groupIdHdl);
			groupId=(EntityId)groupIdHdl.n;
		}

		EntityId id=pGameRules->GetSpawnLocation((EntityId)playerId.n, safeDistance, ignoreTeam, includeNeutral, groupId);
		if (id)
			return pH->EndFunction(ScriptHandle(id));
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetFirstSpawnLocation(IFunctionHandler *pH, int teamId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
	{
		EntityId id=pGameRules->GetFirstSpawnLocation(teamId);
		if (id)
			return pH->EndFunction(ScriptHandle(id));
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AddSpawnGroup(IFunctionHandler *pH, ScriptHandle groupId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->AddSpawnGroup((EntityId)groupId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AddSpawnLocationToSpawnGroup(IFunctionHandler *pH, ScriptHandle groupId, ScriptHandle location)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->AddSpawnLocationToSpawnGroup((EntityId)groupId.n, (EntityId)location.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RemoveSpawnLocationFromSpawnGroup(IFunctionHandler *pH, ScriptHandle groupId, ScriptHandle location)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->RemoveSpawnLocationFromSpawnGroup((EntityId)groupId.n, (EntityId)location.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RemoveSpawnGroup(IFunctionHandler *pH, ScriptHandle groupId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->RemoveSpawnGroup((EntityId)groupId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpawnLocationGroup(IFunctionHandler *pH, ScriptHandle spawnId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
	{
		EntityId groupId=pGameRules->GetSpawnLocationGroup((EntityId)spawnId.n);
		if (groupId)
			return pH->EndFunction(ScriptHandle(groupId));
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpawnGroups(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	int teamId=-1;
	if (pH->GetParamCount()>0 && pH->GetParamType(1)==svtNumber)
		pH->GetParam(1, teamId);

	int count=pGameRules->GetSpawnGroupCount();
	if (!count)
		return pH->EndFunction();

	int tcount=m_spawngroups->Count();

	int i=0;
	int k=0;
	while(i<count)
	{
		EntityId groupId=pGameRules->GetSpawnGroup(i);
		if (teamId==-1 || teamId==pGameRules->GetTeam(groupId))
			m_spawngroups->SetAt(k++, ScriptHandle(groupId));
		++i;
	}

	while(i<tcount)
		m_spawngroups->SetNullAt(++i);

	return pH->EndFunction(m_spawngroups);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTeamDefaultSpawnGroup(IFunctionHandler *pH, int teamId)
{
	CGameRules *pGameRules=GetGameRules(pH);
	if (pGameRules)
	{
		EntityId id=pGameRules->GetTeamDefaultSpawnGroup(teamId);
		if (id)
			return pH->EndFunction(ScriptHandle(id));
	}
	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SetTeamDefaultSpawnGroup(IFunctionHandler *pH, int teamId, ScriptHandle groupId)
{
	CGameRules *pGameRules=GetGameRules(pH);
	if (pGameRules)
		pGameRules->SetTeamDefaultSpawnGroup(teamId, (EntityId)groupId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SetPlayerSpawnGroup(IFunctionHandler *pH, ScriptHandle playerId, ScriptHandle groupId)
{
	CGameRules *pGameRules=GetGameRules(pH);
	if (pGameRules)
		pGameRules->SetPlayerSpawnGroup((EntityId)playerId.n, (EntityId)groupId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AddSpectatorLocation(IFunctionHandler *pH, ScriptHandle location)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->AddSpectatorLocation((EntityId)location.n);

	return pH->EndFunction();

}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RemoveSpectatorLocation(IFunctionHandler *pH, ScriptHandle id)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->RemoveSpectatorLocation((EntityId)id.n);

	return pH->EndFunction();

}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpectatorLocationCount(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		return pH->EndFunction(pGameRules->GetSpectatorLocationCount());

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpectatorLocation(IFunctionHandler *pH, int idx)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		return pH->EndFunction(ScriptHandle(pGameRules->GetSpectatorLocation(idx)));

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSpectatorLocations(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	int count=pGameRules->GetSpectatorLocationCount();
	if (!count)
		return pH->EndFunction();

	int tcount=m_spectatorlocations->Count();

	int i=0;
	while(i<count)
	{
		m_spectatorlocations->SetAt(i, ScriptHandle(pGameRules->GetSpectatorLocation(i)));
		++i;
	}

	while(i<tcount)
		m_spectatorlocations->SetNullAt(++i);

	return pH->EndFunction(m_spectatorlocations);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetRandomSpectatorLocation(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
	{
		if (EntityId locationId=pGameRules->GetRandomSpectatorLocation())
			return pH->EndFunction(ScriptHandle(locationId));
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetInterestingSpectatorLocation(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
	{
		if (EntityId locationId=pGameRules->GetInterestingSpectatorLocation())
			return pH->EndFunction(ScriptHandle(locationId));
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetNextSpectatorTarget(IFunctionHandler *pH, ScriptHandle playerId, int change)
{
	if(change >= 1) change = 1;
	if(change <= 0) change = -1;
	CGameRules* pGameRules = GetGameRules(pH);
	if(pGameRules)
	{
		CPlayer* pPlayer = (CPlayer*)gEnv->pGame->GetIGameFramework()->GetIActorSystem()->GetActor((EntityId)playerId.n);
		if(pPlayer)
		{
			// get list of possible players (team mates or all players)
			CGameRules::TPlayers players;
			if(g_pGame->GetCVars()->g_spectate_TeamOnly == 0 || pGameRules->GetTeamCount() == 0)
			{
				pGameRules->GetPlayers(players);
			}
			else
			{
				int team = pGameRules->GetTeam((EntityId)playerId.n);
				pGameRules->GetTeamPlayers(team, players);
			}
			int numPlayers = players.size();

			// work out which one we are currently watching
			int index = 0;
			for(; index < players.size(); ++index)
				if(players[index] == pPlayer->GetSpectatorTarget())
					break;

			// loop through the players to find a valid one.
			bool found = false;
			if(numPlayers > 0)
			{
				int newTargetIndex = index;
				int numAttempts = numPlayers;
				do
				{
					newTargetIndex += change;
					--numAttempts;

					// wrap around
					if(newTargetIndex < 0)
						newTargetIndex = numPlayers-1;
					if(newTargetIndex >= numPlayers)
						newTargetIndex = 0;

					// skip ourself
					if(players[newTargetIndex] == playerId.n)
						continue;

					// skip dead players
					IActor* pActor = gEnv->pGame->GetIGameFramework()->GetIActorSystem()->GetActor(players[newTargetIndex]);
					if(!pActor || pActor->GetHealth() == 0)
						continue;				

					// skip spectating players
					if(((CPlayer*)pActor)->GetSpectatorMode() != CActor::eASM_None)
						continue;
					
					// otherwise this one will do.
					found = true;
				} while(!found && numAttempts > 0);

				if(found)
					return pH->EndFunction(players[newTargetIndex]);
			}
		}
	}
	return pH->EndFunction(0);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AddMinimapEntity(IFunctionHandler *pH, ScriptHandle entityId, int type, float lifetime)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->AddMinimapEntity((EntityId)entityId.n, type, lifetime);

	return pH->EndFunction();
}


//------------------------------------------------------------------------
int CScriptBind_GameRules::RemoveMinimapEntity(IFunctionHandler *pH, ScriptHandle entityId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->RemoveMinimapEntity((EntityId)entityId.n);

	return pH->EndFunction();
}


//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetMinimap(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->ResetMinimap();

	return pH->EndFunction();
}


//------------------------------------------------------------------------
int CScriptBind_GameRules::GetPing(IFunctionHandler *pH, int channelId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	INetChannel *pNetChannel = g_pGame->GetIGameFramework()->GetNetChannel(channelId);
	if (pNetChannel)
		return pH->EndFunction(pNetChannel->GetPing(true));

	return pH->EndFunction(0);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetEntities(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->ResetEntities();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ServerExplosion(IFunctionHandler *pH, ScriptHandle shooterId, ScriptHandle weaponId,
																					 float dmg, Vec3 pos, Vec3 dir, float radius, float angle, float pressure, float holesize)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!gEnv->bServer)
		return pH->EndFunction();

	ExplosionInfo info(shooterId.n, weaponId.n, dmg, pos, dir.GetNormalized(), radius, angle, pressure, holesize, 0);

	const char *effect="";
	float effectScale=1.0f;
	int type=0;

	if (pH->GetParamCount()>9 && pH->GetParamType(10)!=svtNull)
		pH->GetParam(10, effect);

	if (pH->GetParamCount()>10 && pH->GetParamType(11)!=svtNull)
		pH->GetParam(11, effectScale);

	if (pH->GetParamCount()>11 && pH->GetParamType(12)!=svtNull)
		pH->GetParam(12, type);

	IParticleEffect* pParticleEffect = gEnv->p3DEngine->FindParticleEffect(effect);
	info.SetEffect(pParticleEffect, effectScale, 0.0f);
	info.type = type;

	pGameRules->ServerExplosion(info);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ServerHit(IFunctionHandler *pH, ScriptHandle targetId, ScriptHandle shooterId, ScriptHandle weaponId,
																		 float dmg, float radius, int materialId, int partId, int typeId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!gEnv->bServer)
		return pH->EndFunction();

	HitInfo info(shooterId.n, targetId.n, weaponId.n, dmg, radius, materialId, partId, typeId);

	if (pH->GetParamCount()>8 && pH->GetParamType(9)!=svtNull)
		pH->GetParam(9, info.pos);

	if (pH->GetParamCount()>9 && pH->GetParamType(10)!=svtNull)
		pH->GetParam(10, info.dir);

	if (pH->GetParamCount()>10 && pH->GetParamType(11)!=svtNull)
		pH->GetParam(11, info.normal);

	pGameRules->ServerHit(info);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::CreateTeam(IFunctionHandler *pH, const char *name)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(pGameRules->CreateTeam(name));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RemoveTeam(IFunctionHandler *pH, int teamId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->RemoveTeam(teamId);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTeamName(IFunctionHandler *pH, int teamId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	const char *name=pGameRules->GetTeamName(teamId);
	if (name)
		return pH->EndFunction(name);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTeamId(IFunctionHandler *pH, const char *teamName)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	int id=pGameRules->GetTeamId(teamName);
	if (id)
		return pH->EndFunction(id);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTeamCount(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(pGameRules->GetTeamCount());
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTeamPlayerCount(IFunctionHandler *pH, int teamId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	bool inGame=false;
	if (pH->GetParamCount()>1)
		pH->GetParam(2, inGame);

	return pH->EndFunction(pGameRules->GetTeamPlayerCount(teamId, inGame));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTeamPlayers(IFunctionHandler *pH, int teamId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	int count=pGameRules->GetTeamPlayerCount(teamId);
	if (!count)
		return pH->EndFunction();

	int tcount=m_teamplayers->Count();

	int i=0;
	while(i<count)
	{
		IEntity *pEntity=gEnv->pEntitySystem->GetEntity(pGameRules->GetTeamPlayer(teamId, i));
		if (pEntity)
		{
			IScriptTable *pEntityScript=pEntity->GetScriptTable();
			if (pEntityScript)
				m_teamplayers->SetAt(i+1, pEntityScript);
			else
				m_teamplayers->SetNullAt(i+1);
		}
		++i;
	}

	while(i<tcount)
		m_teamplayers->SetNullAt(++i);

	return pH->EndFunction(m_teamplayers);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SetTeam(IFunctionHandler *pH, int teamId, ScriptHandle playerId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->SetTeam(teamId, (EntityId)playerId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTeam(IFunctionHandler *pH, ScriptHandle playerId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(pGameRules->GetTeam((EntityId)playerId.n));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AddObjective(IFunctionHandler *pH, int teamId, const char *objective, int status, ScriptHandle entityId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->AddObjective(teamId, objective, status, (EntityId)entityId.n);

	return pH->EndFunction();

}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SetObjectiveStatus(IFunctionHandler *pH, int teamId, const char *objective, int status)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->SetObjectiveStatus(teamId, objective, status);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SetObjectiveEntity(IFunctionHandler *pH, int teamId, const char *objective, ScriptHandle entityId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->SetObjectiveEntity(teamId, objective, (EntityId)entityId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RemoveObjective(IFunctionHandler *pH, int teamId, const char *objective)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->RemoveObjective(teamId, objective);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetObjectives(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->ResetObjectives();

	return pH->EndFunction();
}


//------------------------------------------------------------------------
int CScriptBind_GameRules::TextMessage(IFunctionHandler *pH, int type, const char *msg)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	if (pH->GetParamCount()>2)
	{
		string p[4];
		for (int i=0;i<pH->GetParamCount()-2;i++)
		{
			switch(pH->GetParamType(3+i))
			{
			case svtPointer:
				{
					ScriptHandle sh;
					pH->GetParam(3+i, sh);

					if (IEntity *pEntity=gEnv->pEntitySystem->GetEntity((EntityId)sh.n))
						p[i]=pEntity->GetName();
				}
				break;
			default:
				{
					ScriptAnyValue value;
					pH->GetParamAny(3+i, value);
					switch(value.GetVarType())
					{
					case svtNumber:
						p[i].Format("%g", value.number);
						break;
					case svtString:
						p[i]=value.str;
						break;
					case svtBool:
						p[i]=value.b?"true":"false";
						break;
					default:
						break;
					}
				}
				break;
			}
		}
		pGameRules->OnTextMessage((ETextMessageType)type, msg,
			p[0].empty()?0:p[0].c_str(),
			p[1].empty()?0:p[1].c_str(),
			p[2].empty()?0:p[2].c_str(),
			p[3].empty()?0:p[3].c_str()
			);
	}
	else
		pGameRules->OnTextMessage((ETextMessageType)type, msg);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SendTextMessage(IFunctionHandler *pH, int type, const char *msg)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	int to=eRMI_ToAllClients;
	int channelId=-1;

	if (pH->GetParamCount()>2)
		pH->GetParam(3, to);

	if (pH->GetParamCount()>3)
	{
		if (pH->GetParamType(4)==svtPointer)
		{
			ScriptHandle playerId;
			pH->GetParam(4, playerId);

			channelId=pGameRules->GetChannelId((EntityId)playerId.n);
		}
		else if (pH->GetParamType(4)==svtNumber)
			pH->GetParam(4, channelId);
	}

	if (pH->GetParamCount()>4)
	{
		string p[4];
		for (int i=0;i<pH->GetParamCount()-4;i++)
		{
			switch(pH->GetParamType(5+i))
			{
			case svtPointer:
				{
					ScriptHandle sh;
					pH->GetParam(5+i, sh);

					if (IEntity *pEntity=gEnv->pEntitySystem->GetEntity((EntityId)sh.n))
						p[i]=pEntity->GetName();
				}
				break;
			default:
				{
					ScriptAnyValue value;
					pH->GetParamAny(5+i, value);
					switch(value.GetVarType())
					{
					case svtNumber:
						p[i].Format("%g", value.number);
						break;
					case svtString:
						p[i]=value.str;
						break;
					case svtBool:
						p[i]=value.b?"true":"false";
						break;
					default:
						break;
					}			
				}
				break;
			}
		}
		pGameRules->SendTextMessage((ETextMessageType)type, msg, to, channelId, 
			p[0].empty()?0:p[0].c_str(),
			p[1].empty()?0:p[1].c_str(),
			p[2].empty()?0:p[2].c_str(),
			p[3].empty()?0:p[3].c_str()
			);
	}
	else
		pGameRules->SendTextMessage((ETextMessageType)type, msg, to, channelId);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SendChatMessage(IFunctionHandler *pH, int type, ScriptHandle sourceId, ScriptHandle targetId, const char *msg)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->SendChatMessage((EChatMessageType)type, (EntityId)sourceId.n, (EntityId)targetId.n, msg);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ForbiddenAreaWarning(IFunctionHandler *pH, bool active, int timer, ScriptHandle targetId)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->ForbiddenAreaWarning(active, timer, (EntityId)targetId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetGameTime(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->ResetGameTime();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetRemainingGameTime(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(pGameRules->GetRemainingGameTime());
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsTimeLimited(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules || !pGameRules->IsTimeLimited())
		return pH->EndFunction();

	return pH->EndFunction(true);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetRoundTime(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->ResetRoundTime();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetRemainingRoundTime(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(pGameRules->GetRemainingRoundTime());
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsRoundTimeLimited(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules || !pGameRules->IsRoundTimeLimited())
		return pH->EndFunction();

	return pH->EndFunction(true);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetPreRoundTime(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	pGameRules->ResetPreRoundTime();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetRemainingPreRoundTime(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (!pGameRules)
		return pH->EndFunction();

	return pH->EndFunction(pGameRules->GetRemainingPreRoundTime());
}


//------------------------------------------------------------------------
int	CScriptBind_GameRules::EndGame(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->OnEndGame();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::NextLevel(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pGameRules)
		pGameRules->NextLevel();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RegisterHitMaterial(IFunctionHandler *pH, const char *materialName)
{
	CGameRules *pGameRules=GetGameRules(pH);
	
	return pH->EndFunction(pGameRules->RegisterHitMaterial(materialName));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetHitMaterialId(IFunctionHandler *pH, const char *materialName)
{
	CGameRules *pGameRules=GetGameRules(pH);

	return pH->EndFunction(pGameRules->GetHitMaterialId(materialName));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetHitMaterials(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->ResetHitMaterials();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RegisterHitType(IFunctionHandler *pH, const char *type)
{
	CGameRules *pGameRules=GetGameRules(pH);

	return pH->EndFunction(pGameRules->RegisterHitType(type));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetHitTypeId(IFunctionHandler *pH, const char *type)
{
	CGameRules *pGameRules=GetGameRules(pH);

	return pH->EndFunction(pGameRules->GetHitTypeId(type));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetHitType(IFunctionHandler *pH, int id)
{
	CGameRules *pGameRules=GetGameRules(pH);

	return pH->EndFunction(pGameRules->GetHitType(id));
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetHitTypes(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->ResetHitTypes();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ForceScoreboard(IFunctionHandler *pH, bool force)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->ForceScoreboard(force);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::FreezeInput(IFunctionHandler *pH, bool freeze)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->FreezeInput(freeze);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ScheduleEntityRespawn(IFunctionHandler *pH, ScriptHandle entityId, bool unique, float timer)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->ScheduleEntityRespawn((EntityId)entityId.n, unique, timer);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AbortEntityRespawn(IFunctionHandler *pH, ScriptHandle entityId, bool destroyData)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->AbortEntityRespawn((EntityId)entityId.n, destroyData);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ScheduleEntityRemoval(IFunctionHandler *pH, ScriptHandle entityId, float timer, bool visibility)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->ScheduleEntityRemoval((EntityId)entityId.n, timer, visibility);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::AbortEntityRemoval(IFunctionHandler *pH, ScriptHandle entityId)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->AbortEntityRemoval((EntityId)entityId.n);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SetSynchedGlobalValue(IFunctionHandler *pH, int key)
{
	CGameRules *pGameRules=GetGameRules(pH);

	if (pH->GetParamCount()==2)
	{
		switch (pH->GetParamType(2))
		{
		case svtString:
			{
				const char *s=0;
				pH->GetParam(2, s);
				pGameRules->SetSynchedGlobalValue(key, string(s));
			}
			break;
		case svtPointer:
			{
				ScriptHandle e;
				pH->GetParam(2, e);
				pGameRules->SetSynchedGlobalValue(key, (EntityId)e.n);
			}
			break;
		case svtBool:
			{
				bool b;
				pH->GetParam(2, b);
				pGameRules->SetSynchedGlobalValue(key, b);
			}
			break;
		case svtNumber:
			{
				float f;
				int i;
				pH->GetParam(2, f);
				i=(int)f;
				if (f==i)
					pGameRules->SetSynchedGlobalValue(key, i);
				else
					pGameRules->SetSynchedGlobalValue(key, f);
			}
			break;
		default:
			assert(0);
		}
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSynchedGlobalValue(IFunctionHandler *pH, int key)
{
	CGameRules *pGameRules=GetGameRules(pH);

	int type=pGameRules->GetSynchedGlobalValueType(key);
	if (type==eSVT_None)
		return pH->EndFunction();

	switch (type)
	{
	case eSVT_Bool:
		{
			bool b;
			pGameRules->GetSynchedGlobalValue(key, b);
			return pH->EndFunction(b);
		}
		break;
	case eSVT_Float:
		{
			float f;
			pGameRules->GetSynchedGlobalValue(key, f);
			return pH->EndFunction(f);
		}
		break;
	case eSVT_Int:
		{
			int i;
			pGameRules->GetSynchedGlobalValue(key, i);
			return pH->EndFunction(i);
		}
		break;
	case eSVT_EntityId:
		{
			EntityId e;
			pGameRules->GetSynchedGlobalValue(key, e);
			return pH->EndFunction(ScriptHandle(e));
		}
		break;
	case eSVT_String:
		{
			static string s;
			pGameRules->GetSynchedGlobalValue(key, s);
			return pH->EndFunction(s.c_str());
		}
		break;
	default:
		assert(0);
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::SetSynchedEntityValue(IFunctionHandler *pH, ScriptHandle entityId, int key)
{
	CGameRules *pGameRules=GetGameRules(pH);
	
	EntityId id=(EntityId)entityId.n;

	if (pH->GetParamCount()==3)
	{
		switch (pH->GetParamType(3))
		{
		case svtString:
			{
				const char *s=0;
				pH->GetParam(3, s);
				pGameRules->SetSynchedEntityValue(id, key, string(s));
			}
			break;
		case svtPointer:
			{
				ScriptHandle e;
				pH->GetParam(3, e);
				pGameRules->SetSynchedEntityValue(id, key, (EntityId)e.n);
			}
			break;
		case svtBool:
			{
				bool b;
				pH->GetParam(3, b);
				pGameRules->SetSynchedEntityValue(id, key, b);
			}
			break;
		case svtNumber:
			{
				float f;
				int i;
				pH->GetParam(3, f);
				i=(int)f;
				if (f==i)
					pGameRules->SetSynchedEntityValue(id, key, i);
				else
					pGameRules->SetSynchedEntityValue(id, key, f);
			}
			break;
		default:
			assert(0);
		}
	}
	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSynchedEntityValue(IFunctionHandler *pH, ScriptHandle entityId, int key)
{
	CGameRules *pGameRules=GetGameRules(pH);

	EntityId id=(EntityId)entityId.n;
	int type=pGameRules->GetSynchedEntityValueType(id, key);
	if (type==eSVT_None)
		return pH->EndFunction();

	switch (type)
	{
	case eSVT_Bool:
		{
			bool b;
			pGameRules->GetSynchedEntityValue(id, key, b);
			return pH->EndFunction(b);
		}
		break;
	case eSVT_Float:
		{
			float f;
			pGameRules->GetSynchedEntityValue(id, key, f);
			return pH->EndFunction(f);
		}
		break;
	case eSVT_Int:
		{
			int i;
			pGameRules->GetSynchedEntityValue(id, key, i);
			return pH->EndFunction(i);
		}
		break;
	case eSVT_EntityId:
		{
			EntityId e;
			pGameRules->GetSynchedEntityValue(id, key, e);
			return pH->EndFunction(ScriptHandle(e));
		}
		break;
	case eSVT_String:
		{
			static string s;
			pGameRules->GetSynchedEntityValue(id, key, s);
			return pH->EndFunction(s.c_str());
		}
		break;
	default:
		assert(0);
	}

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ResetSynchedStorage(IFunctionHandler *pH)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->ResetSynchedStorage();

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsDemoMode(IFunctionHandler *pH)
{
	int demoMode = GetISystem()->IsDemoMode();
	return pH->EndFunction(demoMode);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetTimeLimit(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_timelimit);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetRoundTime(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_roundtime);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetPreRoundTime(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_preroundtime);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetSuddenDeathTime(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_suddendeathtime);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetRoundLimit(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_roundlimit);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetFragLimit(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_fraglimit);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetFragLead(IFunctionHandler *pH)
{
  return pH->EndFunction(g_pGameCVars->g_fraglead);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetFriendlyFireRatio(IFunctionHandler *pH)
{
  return pH->EndFunction(g_pGameCVars->g_friendlyfireratio);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetReviveTime(IFunctionHandler *pH)
{
  return pH->EndFunction(g_pGameCVars->g_revivetime);
}

int CScriptBind_GameRules::GetMinPlayerLimit(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_minplayerlimit);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::GetMinTeamLimit(IFunctionHandler *pH)
{
	return pH->EndFunction(g_pGameCVars->g_minteamlimit);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::IsFrozen(IFunctionHandler *pH, ScriptHandle entityId)
{
	CGameRules *pGameRules=GetGameRules(pH);
	if (pGameRules->IsFrozen((EntityId)entityId.n))
		return pH->EndFunction(1);
	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::FreezeEntity(IFunctionHandler *pH, ScriptHandle entityId, bool freeze, bool vapor)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->FreezeEntity((EntityId)entityId.n, freeze, vapor);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::ShatterEntity(IFunctionHandler *pH, ScriptHandle entityId, Vec3 pos, Vec3 impulse)
{
	CGameRules *pGameRules=GetGameRules(pH);
	pGameRules->ShatterEntity((EntityId)entityId.n, pos, impulse);

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::DebugCollisionDamage(IFunctionHandler *pH)
{
  return pH->EndFunction(g_pGameCVars->g_debugCollisionDamage);
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RequestHitIndicator(IFunctionHandler* pH, ScriptHandle shooterId)
{
	EntityId id = EntityId(shooterId.n);
	if(!id)
		return pH->EndFunction();
	CGameRules *pGameRules = GetGameRules(pH);
	if(!gEnv->bServer)
		return pH->EndFunction();

	CActor* pActor = pGameRules->GetActorByEntityId(id);
	if ( !pActor || !pActor->IsPlayer() )
		return pH->EndFunction();

	CGameRules::EntityParams params(id);
	pGameRules->GetGameObject()->InvokeRMIWithDependentObject(CGameRules::ClRequestHitIndicator(), params, eRMI_ToClientChannel, id, pGameRules->GetChannelId(id));

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::RequestDamageIndicator(IFunctionHandler* pH, ScriptHandle shooterId)
{
	EntityId id = EntityId(shooterId.n);
	if(!id)
		return pH->EndFunction();
	CGameRules *pGameRules = GetGameRules(pH);
	if(!gEnv->bServer)
		return pH->EndFunction();

	CActor* pActor = pGameRules->GetActorByEntityId(id);
	if ( !pActor || !pActor->IsPlayer() )
		return pH->EndFunction();

	CGameRules::EntityParams params(id);
	pGameRules->GetGameObject()->InvokeRMIWithDependentObject(CGameRules::ClRequestDamageIndicator(), params, eRMI_ToClientChannel, id, pGameRules->GetChannelId(id));

	return pH->EndFunction();
}

//------------------------------------------------------------------------
int CScriptBind_GameRules::TutorialEvent(IFunctionHandler* pH, int eventType)
{
	CMPTutorial* pTutorial = GetGameRules(pH)->GetMPTutorial();
	if(pTutorial)
	{
		pTutorial->TriggerEvent(static_cast<ETutorialEvent>(eventType));
	}

	return pH->EndFunction();
}

//-------------------------------------------------------------------------
int CScriptBind_GameRules::GameOver(IFunctionHandler* pH, int localWinner)
{
	CGameRules* pGameRules = g_pGame->GetGameRules();
	if (pGameRules)
		pGameRules->GameOver(localWinner);

	return pH->EndFunction();
}

//-------------------------------------------------------------------------
int CScriptBind_GameRules::EnteredGame(IFunctionHandler* pH)
{
	CGameRules* pGameRules = g_pGame->GetGameRules();
	if (pGameRules)
		pGameRules->EnteredGame();

	return pH->EndFunction();
}

//--------------------------------------------------------------------------
int CScriptBind_GameRules::EndGameNear(IFunctionHandler* pH, ScriptHandle entityId)
{
	CGameRules* pGameRules = g_pGame->GetGameRules();
	if(pGameRules)
		pGameRules->EndGameNear(EntityId(entityId.n));

	return pH->EndFunction();
}
