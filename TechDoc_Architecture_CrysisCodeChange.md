[Back](TechDoc_Architecture.md)

All changes to the Crysis SDK codebase are logged at this location. All changes made in the source files are commented using the following special syntax:

```
// [D6] <Reason for change>
...
// [/D6]
```

Below, seperated by files, are the explinations for all changes made. Development team: please keep this file up-to-date with all changes made including a detailed description and the line number.

# Changes #

## !IMovementController.h ##

Lines 567 - 580:
```
	// [D6] Lock steering movement request
	bool HasLockedSteering(void) const
	{
		return CheckFlag(eMRF_LockedSteering);
	}

	void SetLockedSteering(bool bLocked)
	{
		if (true == bLocked)
			SetFlag(eMRF_LockedSteering);
		else
			ClearFlag(eMRF_LockedSteering);
	}
	// [/D6]
```

## !IEntity.h ##

Lines 378 - 380:
```
	// [D6] Special flag - Used to mark if an entity is an interface to a building
    ENTITY_FLAG_ISINTERFACE				 = BIT(4),
    // [/D6]
```

## Game.cpp ##

Lines 45 - 48:
```
// [D6] Use our game factory
//#include "GameFactory.h"
#include "CD6GameFactory.h"
// [/D6]
```

Lines 185 - 188:
```
	// [D6] Use our game factory!
	//InitGameFactory(m_pFramework);
	InitD6GameFactory(m_pFramework);
	// [/D6]
```

## Game.h ##

Lines 160 - 162:
```
	// [D6] Made virutal...
	virtual CGameRules *GetGameRules() const;
	// [/D6]
```

## GameDll.cpp ##

Lines 20 - 23:
```
// [D6] Include our platform too
#include <d6platform_impl.h>
#include "CD6Game.h"
// [/D6]
```

Lines 25 - 28:
```
// [D6] Set up extern
CD6CoreGlobalEnvironment CD6CoreGlobalEnvironment::m_Instance;
CD6CoreGlobalEnvironment* g_D6Core = &CD6CoreGlobalEnvironment::GetInstance();
// [/D6]
```

Lines 45 - 47:
```
		// [D6] Init our platform
		g_D6Core->D6CoreModuleInitISystem(pGameFramework->GetISystem());
		// [/D6]
```

Lines 51 - 55:
```
		// [D6] Run our game, not theirs!
		CD6Game *pGame = new ((void*)pGameBuffer) CD6Game();
		g_D6Core->pD6Game = pGame;
		return pGame;
		// [/D6]
```

## GameRules.cpp ##

Lines 214 - 219:
```
	// [D6] This is handled by the team manager now
	assert(g_D6Core->pTeamManager);
	g_D6Core->pTeamManager->PostInitClient(channelId);
	/*for (TEntityTeamIdMap::const_iterator tit=m_entityteams.begin(); tit!=m_entityteams.end(); ++tit)
		GetGameObject()->InvokeRMIWithDependentObject(ClSetTeam(), SetTeamParams(tit->first, tit->second), eRMI_ToClientChannel, tit->first, channelId);*/
	// [/D6]
```

Lines 769 - 792:
```
		// [D6] Use team manager to get this info
		/*EntityId teamId = GetTeam(shooter);
		TPlayerTeamIdMap::const_iterator tit=m_playerteams.find(teamId);
		if (tit!=m_playerteams.end())
		{
			for (TPlayers::const_iterator it=tit->second.begin(); it!=tit->second.end(); ++it)
			{
				if(temporary)
					GetGameObject()->InvokeRMI(ClTempRadarEntity(), params, eRMI_ToClientChannel, GetChannelId(*it));
				else
					GetGameObject()->InvokeRMI(ClTaggedEntity(), params, eRMI_ToClientChannel, GetChannelId(*it));
			}
		}*/
		TeamID nTeamID = GetTeam(shooter);
		TPlayers players;
		GetTeamPlayers(nTeamID, players);
		for (TPlayers::iterator itPlayer = players.begin(); itPlayer != players.end(); itPlayer++)
		{
			if(temporary)
				GetGameObject()->InvokeRMI(ClTempRadarEntity(), params, eRMI_ToClientChannel, GetChannelId(*itPlayer));
			else
				GetGameObject()->InvokeRMI(ClTaggedEntity(), params, eRMI_ToClientChannel, GetChannelId(*itPlayer));
		}
		// [/D6]
```

Lines 1380 - 1389:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*TTeamIdMap::iterator it = m_teams.find(CONST_TEMP_STRING(name));
	if (it != m_teams.end())
		return it->second;

	m_teams.insert(TTeamIdMap::value_type(name, ++m_teamIdGen));
	m_playerteams.insert(TPlayerTeamIdMap::value_type(m_teamIdGen, TPlayers()));

	return m_teamIdGen;*/
	// [/D6]
```

Lines 1397 - 1411:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	//TTeamIdMap::iterator it = m_teams.find(CONST_TEMP_STRING(GetTeamName(teamId)));
	//if (it == m_teams.end())
	//	return;

	//m_teams.erase(it);

	//for (TEntityTeamIdMap::iterator eit=m_entityteams.begin(); eit != m_entityteams.end(); ++eit)
	//{
	//	if (eit->second == teamId)
	//		eit->second = 0; // 0 is no team
	//}

	//m_playerteams.erase(m_playerteams.find(teamId));
	// [/D6]
```

Lines 1417 - 1423:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*for (TTeamIdMap::const_iterator it = m_teams.begin(); it!=m_teams.end(); ++it)
	{
		if (teamId == it->second)
			return it->first;
	}*/
	// [/D6]
```

Lines 1431 - 1435:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*TTeamIdMap::const_iterator it=m_teams.find(CONST_TEMP_STRING(name));
	if (it!=m_teams.end())
		return it->second;*/
	// [/D6]
```

Lines 1443 - 1445:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	//return (int)m_teams.size();
	// [/D6]
```

Lines 1453 - 1477:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*if (!inGame)
	{
		TPlayerTeamIdMap::const_iterator it=m_playerteams.find(teamId);
		if (it!=m_playerteams.end())
			return (int)it->second.size();
		return 0;
	}
	else
	{
		TPlayerTeamIdMap::const_iterator it=m_playerteams.find(teamId);
		if (it!=m_playerteams.end())
		{
			int count=0;

			const TPlayers &players=it->second;
			for (TPlayers::const_iterator pit=players.begin(); pit!=players.end(); ++pit)
				if (IsPlayerInGame(*pit))
					++count;

			return count;
		}
		return 0;
	}*/
	// [/D6]
```

Lines 1485 - 1497:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*int count=0;
	for (TChannelTeamIdMap::const_iterator it=m_channelteams.begin(); it!=m_channelteams.end(); ++it)
	{
		if (teamId==it->second)
		{
			if (!inGame || IsChannelInGame(it->first))
				++count;
		}
	}

	return count;*/
	// [/D6]
```

Lines 1505 - 1512:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*TPlayerTeamIdMap::const_iterator it=m_playerteams.find(teamId);
	if (it!=m_playerteams.end())
	{
		if (idx>=0 && idx<it->second.size())
			return it->second[idx];
	}*/
	// [/D6]
```

Lines 1520 - 1525:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*players.resize(0);
	TPlayerTeamIdMap::const_iterator it=m_playerteams.find(teamId);
	if (it!=m_playerteams.end())
		players=it->second;*/
	// [/D6]
```

Lines 1532 - 1623:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*if (!gEnv->bServer )
	{
		assert(0);
		return;
	}

	int oldTeam = GetTeam(id);
	if (oldTeam==teamId)
		return;

	TEntityTeamIdMap::iterator it=m_entityteams.find(id);
	if (it!=m_entityteams.end())
		m_entityteams.erase(it);

	IActor *pActor=m_pActorSystem->GetActor(id);
	bool isplayer=pActor!=0;
	if (isplayer && oldTeam)
	{	
		TPlayerTeamIdMap::iterator pit=m_playerteams.find(oldTeam);
		assert(pit!=m_playerteams.end());
		stl::find_and_erase(pit->second, id);
	}

	if (teamId)
	{
		m_entityteams.insert(TEntityTeamIdMap::value_type(id, teamId));

		if (isplayer)
		{
			TPlayerTeamIdMap::iterator pit=m_playerteams.find(teamId);
			assert(pit!=m_playerteams.end());
			pit->second.push_back(id);

			UpdateObjectivesForPlayer(GetChannelId(id), teamId);
		}
	}

	if(IActor *pClient = g_pGame->GetIGameFramework()->GetClientActor())
	{
		if(GetTeam(pClient->GetEntityId()) == teamId)
		{
			if(id == pClient->GetGameObject()->GetWorldQuery()->GetLookAtEntityId())
			{
				if(g_pGame->GetHUD())
				{
					g_pGame->GetHUD()->GetCrosshair()->SetUsability(0);
					g_pGame->GetHUD()->GetCrosshair()->SetUsability(1);
				}
			}
		}
	}

	if(isplayer)
	{
		ReconfigureVoiceGroups(id,oldTeam,teamId);

		int channelId=GetChannelId(id);

		TChannelTeamIdMap::iterator it=m_channelteams.find(channelId);
		if (it!=m_channelteams.end())
		{
			if (!teamId)
				m_channelteams.erase(it);
			else
				it->second=teamId;
		}
		else if(teamId)
			m_channelteams.insert(TChannelTeamIdMap::value_type(channelId, teamId));

		if (pActor->IsClient())
			m_pRadio->SetTeam(GetTeamName(teamId));
	}

	ScriptHandle handle(id);
	CallScript(m_serverStateScript, "OnSetTeam", handle, teamId);

	if (gEnv->bClient)
	{
		ScriptHandle handle(id);
		CallScript(m_clientStateScript, "OnSetTeam", handle, teamId);
	}
	
	// if this is a spawn group, update it's validity
	if (m_spawnGroups.find(id)!=m_spawnGroups.end())
		CheckSpawnGroupValidity(id);

	GetGameObject()->InvokeRMIWithDependentObject(ClSetTeam(), SetTeamParams(id, teamId), eRMI_ToRemoteClients, id);

	if (IEntity *pEntity=m_pEntitySystem->GetEntity(id))
		m_pGameplayRecorder->Event(pEntity, GameplayEvent(eGE_ChangedTeam, 0, (float)teamId));*/
	// [/D6]
```

Lines 1629 - 1633:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*TEntityTeamIdMap::const_iterator it = m_entityteams.find(entityId);
	if (it != m_entityteams.end())
		return it->second;*/
	// [/D6]
```

Lines 1641 - 1645:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*TChannelTeamIdMap::const_iterator it = m_channelteams.find(channelId);
	if (it != m_channelteams.end())
		return it->second;*/
	// [/D6]
```

Lines 2407 - 2418:
```
		// [D6] Let team manager handle this
		/*TTeamIdEntityIdMap::iterator next;
		for (TTeamIdEntityIdMap::iterator dit=m_teamdefaultspawns.begin(); dit!=m_teamdefaultspawns.end(); dit=next)
		{
			next=dit;
			++next;
			if (dit->second==groupId)
				m_teamdefaultspawns.erase(dit);
		}*/
		assert(g_D6Core->pTeamManager);
		g_D6Core->pTeamManager->RemoveDefaultSpawnGroupFromTeams(groupId);
		// [/D6]
```

Lines 2502 - 2509:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*TTeamIdEntityIdMap::iterator it=m_teamdefaultspawns.find(teamId);
	
	if (it!=m_teamdefaultspawns.end())
		it->second=spawnGroupId;
	else
		m_teamdefaultspawns.insert(TTeamIdEntityIdMap::value_type(teamId, spawnGroupId));*/
	// [/D6]
```

Lines 2515 - 2519:
```
	// [D6] Code is handled by CD6GameRules and Team Manager
	/*TTeamIdEntityIdMap::iterator it=m_teamdefaultspawns.find(teamId);
	if (it!=m_teamdefaultspawns.end())
		return it->second;*/
	// [/D6]
```

Lines 2886 - 2900:
```
					// [D6] Use team manager
					/*TPlayerTeamIdMap::const_iterator tit=m_playerteams.find(teamId);
					if (tit!=m_playerteams.end())
					{
						for (TPlayers::const_iterator it=tit->second.begin(); it!=tit->second.end(); ++it)
							GetGameObject()->InvokeRMIWithDependentObject(ClChatMessage(), params, eRMI_ToClientChannel, *it, GetChannelId(*it));
					}*/
					TPlayers players;
					GetTeamPlayers(teamId, players);
					for (TPlayers::const_iterator itPlayer = players.begin(); itPlayer != players.end(); itPlayer++)
					{
						if (CanReceiveChatMessage(type, sourceId, *itPlayer))
							GetGameObject()->InvokeRMIWithDependentObject(ClChatMessage(), params, eRMI_ToClientChannel, *itPlayer, GetChannelId(*itPlayer));
					}
					// [/D6]
```

Lines 3199 - 3217:
```
	// [D6] Use team manager
	/*CGameRules *pGameRules=g_pGame->GetGameRules();
	if (!pGameRules->m_entityteams.empty())
	{
		CryLogAlways("// Teams //");
		for (TTeamIdMap::const_iterator tit=pGameRules->m_teams.begin(); tit!=pGameRules->m_teams.end(); ++tit)
		{
			CryLogAlways("Team: %s  (id: %d)", tit->first.c_str(), tit->second);
			for (TEntityTeamIdMap::const_iterator eit=pGameRules->m_entityteams.begin(); eit!=pGameRules->m_entityteams.end(); ++eit)
			{
				if (eit->second==tit->second)
				{
					IEntity *pEntity=gEnv->pEntitySystem->GetEntity(eit->first);
					CryLogAlways("    -> Entity: %s  class: %s  (eid: %d %08x)", pEntity->GetName(), pEntity->GetClass()->GetName(), pEntity->GetId(), pEntity->GetId());
				}
			}
		}
	}*/
	// [/D6]
```

Lines 3235 - 3253:
```
	// [D6] Let team manager handle this
	/*CGameRules *pGameRules=g_pGame->GetGameRules();
	if (!pGameRules->m_objectives.empty())
	{
		CryLogAlways("// Teams //");
		for (TTeamIdMap::const_iterator tit=pGameRules->m_teams.begin(); tit!=pGameRules->m_teams.end(); ++tit)
		{
			if (TObjectiveMap *pObjectives=pGameRules->GetTeamObjectives(tit->second))
			{
				for (TObjectiveMap::const_iterator it=pObjectives->begin(); it!=pObjectives->end(); ++it)
					CryLogAlways("  -> Objective: %s  teamId: %d  status: %s  (eid: %d %08x)", it->first.c_str(), tit->second,
						status[CLAMP(it->second.status, 0, CHUDMissionObjective::LAST)], it->second.entityId, it->second.entityId);
			}
		}
	}*/

	assert(g_D6Core->pTeamManager);
	g_D6Core->pTeamManager->CmdDebugObjectives(pArgs, status);
	// [/D6]
```

Lines 3334 - 3577:
```
		// [D6] Check if system exists
		IEntity *pShooter=m_pEntitySystem->GetEntity(explosionInfo.shooterId);
		IEntity *pWeapon=m_pEntitySystem->GetEntity(explosionInfo.weaponId);
		explosion.SetValue("shooter", pShooter?pShooter->GetScriptTable():(IScriptTable *)0);
		explosion.SetValue("weapon", pWeapon?pWeapon->GetScriptTable():(IScriptTable *)0);
		explosion.SetValue("materialId", 0);
		explosion.SetValue("damage", explosionInfo.damage);
		explosion.SetValue("min_radius", explosionInfo.minRadius);
		explosion.SetValue("radius", explosionInfo.radius);
		explosion.SetValue("pressure", explosionInfo.pressure);
		explosion.SetValue("hole_size", explosionInfo.hole_size);
		explosion.SetValue("effect", explosionInfo.pParticleEffect?explosionInfo.pParticleEffect->GetName():"");
		explosion.SetValue("effectScale", explosionInfo.effect_scale);
		explosion.SetValue("effectClass", explosionInfo.effect_class.c_str());
		explosion.SetValue("typeId", explosionInfo.type);
		const char *type=GetHitType(explosionInfo.type);
		explosion.SetValue("type", type);
		explosion.SetValue("angle", explosionInfo.angle);
		
		explosion.SetValue("impact", explosionInfo.impact);
		explosion.SetValue("impact_velocity", explosionInfo.impact_velocity);
		explosion.SetValue("impact_normal", explosionInfo.impact_normal);
    explosion.SetValue("impact_targetId", ScriptHandle(explosionInfo.impact_targetId));		
	}
  
  SmartScriptTable temp;
  if (scriptExplosionInfo->GetValue("AffectedEntities", temp))
  {
    temp->Clear();
	}
	if (scriptExplosionInfo->GetValue("AffectedEntitiesObstruction", temp))
	{
		temp->Clear();
	}
}

//------------------------------------------------------------------------

void CGameRules::ShowScores(bool show)
{
	CallScript(m_script, "ShowScores", show);
}

//------------------------------------------------------------------------
void CGameRules::UpdateAffectedEntitiesSet(TExplosionAffectedEntities &affectedEnts, const pe_explosion *pExplosion)
{
	if (pExplosion)
	{
		for (int i=0; i<pExplosion->nAffectedEnts; ++i)
		{ 
			if (IEntity *pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(pExplosion->pAffectedEnts[i]))
			{ 
				if (IScriptTable *pEntityTable = pEntity->GetScriptTable())
				{
					IPhysicalEntity* pEnt = pEntity->GetPhysics();
					if (pEnt)
					{
						float affected=gEnv->pPhysicalWorld->IsAffectedByExplosion(pEnt);

						AddOrUpdateAffectedEntity(affectedEnts, pEntity, affected);
					}
				}
			}
		}
	}
}

//------------------------------------------------------------------------
void CGameRules::AddOrUpdateAffectedEntity(TExplosionAffectedEntities &affectedEnts, IEntity* pEntity, float affected)
{
	TExplosionAffectedEntities::iterator it=affectedEnts.find(pEntity);
	if (it!=affectedEnts.end())
	{
		if (it->second<affected)
			it->second=affected;
	}
	else
		affectedEnts.insert(TExplosionAffectedEntities::value_type(pEntity, affected));
}

//------------------------------------------------------------------------
void CGameRules::CommitAffectedEntitiesSet(SmartScriptTable &scriptExplosionInfo, TExplosionAffectedEntities &affectedEnts)
{
	CScriptSetGetChain explosion(scriptExplosionInfo);

	SmartScriptTable affected;
	SmartScriptTable affectedObstruction;

	if (scriptExplosionInfo->GetValue("AffectedEntities", affected) && 
		scriptExplosionInfo->GetValue("AffectedEntitiesObstruction", affectedObstruction))
	{
		if (affectedEnts.empty())
		{
			affected->Clear();
			affectedObstruction->Clear();
		}
		else
		{
			int k=0;      
			for (TExplosionAffectedEntities::const_iterator it=affectedEnts.begin(),end=affectedEnts.end(); it!=end; ++it)
			{
				float obstruction = 1.0f-it->second;
				affected->SetAt(++k, it->first->GetScriptTable());
				affectedObstruction->SetAt(k, obstruction);
			}
		}
	}
}

//------------------------------------------------------------------------
void CGameRules::PrepCollision(int src, int trg, const SGameCollision& event, IEntity* pTarget)
{
	const EventPhysCollision* pCollision = event.pCollision;
	CScriptSetGetChain chain(m_collisionTable);

	chain.SetValue("normal", pCollision->n);
	chain.SetValue("pos", pCollision->pt);

	Vec3 dir(0, 0, 0);
	if (pCollision->vloc[src].GetLengthSquared() > 1e-6f)
	{
		dir = pCollision->vloc[src].GetNormalized();
		chain.SetValue("dir", dir);
	}
	else
	{
		chain.SetToNull("dir");
	}

	chain.SetValue("velocity", pCollision->vloc[src]);
	pe_status_living sl;
	if (pCollision->pEntity[src]->GetStatus(&sl) && sl.bSquashed)
	{
		chain.SetValue("target_velocity", pCollision->n*(200.0f*(1-src*2)));
		chain.SetValue("target_mass", pCollision->mass[trg]>0 ? pCollision->mass[trg] : 10000.0f);
	}
	else
	{
		chain.SetValue("target_velocity", pCollision->vloc[trg]);
		chain.SetValue("target_mass", pCollision->mass[trg]);
	}
	chain.SetValue("backface", pCollision->n.Dot(dir) >= 0);
	//chain.SetValue("partid", pCollision->partid[src]);
	//chain.SetValue("backface", pCollision->n.Dot(dir) >= 0);
	/*float deltaE = 0;
	if (pCollision->mass[0])
		deltaE += -pCollision->normImpulse*(pCollision->vloc[0]*pCollision->n + pCollision->normImpulse*0.5f/pCollision->mass[0]);
	if (pCollision->mass[1])
		deltaE +=  pCollision->normImpulse*(pCollision->vloc[1]*pCollision->n - pCollision->normImpulse*0.5f/pCollision->mass[1]);
	chain.SetValue("energy_loss", deltaE);*/

	//IEntity *pTarget = gEnv->pEntitySystem->GetEntityFromPhysics(pCollision->pEntity[trg]);

	//chain.SetValue("target_type", (int)pCollision->pEntity[trg]->GetType());

	if (pTarget)
	{
		ScriptHandle sh;
		sh.n = pTarget->GetId();

		if (pTarget->GetPhysics())
		{
			chain.SetValue("target_type", (int)pTarget->GetPhysics()->GetType());
		}

		chain.SetValue("target_id", sh);

		if (pTarget->GetScriptTable())
		{
			chain.SetValue("target", pTarget->GetScriptTable());
		}
		else
		{
			chain.SetToNull("target");  
		}
	}
	else
	{
		chain.SetToNull("target"); 
		chain.SetToNull("target_id");
	}


	if(pCollision->idmat[trg]==s_barbWireID)
		chain.SetValue("materialId",pCollision->idmat[trg]); //Pass collision with barbwire to script
	else
		chain.SetValue("materialId", pCollision->idmat[src]);
	//chain.SetValue("target_materialId", pCollision->idmat[trg]);

	//ISurfaceTypeManager *pSurfaceTypeManager = gEnv->p3DEngine->GetMaterialManager()->GetSurfaceTypeManager();
}

//------------------------------------------------------------------------
void CGameRules::Restart()
{
	if (gEnv->bServer)
		CallScript(m_script, "RestartGame", gEnv->pSystem->IsDevMode());
}

//------------------------------------------------------------------------
void CGameRules::NextLevel()
{
  if (!gEnv->bServer)
    return;

	ILevelRotation *pLevelRotation=m_pGameFramework->GetILevelSystem()->GetLevelRotation();
	if (!pLevelRotation->GetLength())
		Restart();
	else
		pLevelRotation->ChangeLevel();
}

//------------------------------------------------------------------------
void CGameRules::ResetEntities()
{
	g_pGame->GetWeaponSystem()->GetTracerManager().Reset();

	ResetFrozen();

	while (!m_queuedExplosions.empty())
		m_queuedExplosions.pop();

	while (!m_queuedHits.empty())
		m_queuedHits.pop();
	m_processingHit=0;

	// remove voice groups too. They'll be recreated when players are put back on their teams after reset.
 	TTeamIdVoiceGroupMap::iterator it = m_teamVoiceGroups.begin();
 	TTeamIdVoiceGroupMap::iterator next;
 	for(; it != m_teamVoiceGroups.end(); it=next)
 	{
 		next = it; ++next;
 
		m_teamVoiceGroups.erase(it);
 	}

	m_respawns.clear();
	// [D6] No longer needed here
	/*m_entityteams.clear();
	m_teamdefaultspawns.clear()

	for (TPlayerTeamIdMap::iterator tit=m_playerteams.begin(); tit!=m_playerteams.end(); tit++)
		tit->second.resize(0);;*/
	// [/D6]
```

Lines 3897 - 3911:
```
				// [D6] Use team's player map
				/*TPlayerTeamIdMap::const_iterator tit=m_playerteams.find(teamId);
				if (tit!=m_playerteams.end())
				{
					for (TPlayers::const_iterator it=tit->second.begin(); it!=tit->second.end(); ++it)
						GetGameObject()->InvokeRMIWithDependentObject(ClRadioMessage(), params, eRMI_ToClientChannel, *it, GetChannelId(*it));
				}*/
				TPlayers players;
				GetTeamPlayers(teamId, players);
				for (TPlayers::iterator itPlayer = players.begin(); itPlayer != players.end();
					itPlayer++)
				{
					GetGameObject()->InvokeRMIWithDependentObject(ClRadioMessage(), params, eRMI_ToClientChannel, *itPlayer, GetChannelId(*itPlayer));
				}
				// [/D6]
```

Lines 4021 - 4026:
```
	// [D6] Following containers are no longer part of the GameRules class, migrated to Team Manager
	//s->AddContainer(m_teams);
	//s->AddContainer(m_entityteams);
	//s->AddContainer(m_teamdefaultspawns);
	//s->AddContainer(m_playerteams);
	// [/D6]
```

Lines 4040 - 4045:
```
	// [D6] Migrated to Team Manager
	/*for (TTeamIdMap::iterator iter = m_teams.begin(); iter != m_teams.end(); ++iter)
		s->Add(iter->first);*/
	/*for (TPlayerTeamIdMap::iterator iter = m_playerteams.begin(); iter != m_playerteams.end(); ++iter)
		s->AddContainer(iter->second);*/
	// [/D6]
```

## GameRulesClientServer.cpp ##

Lines 790 - 820:
```
	// [D6] Code is handled in team manager now
	assert(g_D6Core->pTeamManager);
	bool bResult = g_D6Core->pTeamManager->SetTeam(params.teamId, params.entityId);
	int oldTeam = GetTeam(params.entityId);
	if (oldTeam==params.teamId)
		return true;
	IActor *pActor=m_pActorSystem->GetActor(params.entityId);
	bool isplayer=pActor!=0;

	/*TEntityTeamIdMap::iterator it=m_entityteams.find(params.entityId);
	if (it!=m_entityteams.end())
		m_entityteams.erase(it);

	if (isplayer && oldTeam)
	{
		TPlayerTeamIdMap::iterator pit=m_playerteams.find(oldTeam);
		assert(pit!=m_playerteams.end());
		stl::find_and_erase(pit->second, params.entityId);
	}

	if (params.teamId)
	{
		m_entityteams.insert(TEntityTeamIdMap::value_type(params.entityId, params.teamId));
		if (isplayer)
		{
			TPlayerTeamIdMap::iterator pit=m_playerteams.find(params.teamId);
			assert(pit!=m_playerteams.end());
			pit->second.push_back(params.entityId);
		}
	}*/
	// [/D6]
```

[Back](TechDoc_Architecture.md)