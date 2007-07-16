/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2004.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description: Weapon System

-------------------------------------------------------------------------
History:
- 18:10:2005   17:41 : Created by M�rcio Martins

*************************************************************************/
#ifndef __WEAPONSYSTEM_H__
#define __WEAPONSYSTEM_H__

#if _MSC_VER > 1000
# pragma once
#endif


#include <IItemSystem.h>
#include <ILevelSystem.h>
#include <IWeapon.h>
#include "Item.h"
#include "TracerManager.h"
#include "VectorMap.h"
#include "AmmoParams.h"

class CGame;
class CProjectile;
struct ISystem;

struct SProjectileQuery
{
	AABB        box;
	const char* ammoName;
	IEntity     **pResults;
	int         nCount;
	SProjectileQuery()
	{
		pResults = 0;
		nCount = 0;
		ammoName = 0;
	}
};

class CWeaponSystem : public ILevelSystemListener
{
	typedef std::map<string, IFireMode		*(*)()>								TFireModeRegistry;
	typedef std::map<string, IZoomMode		*(*)()>								TZoomModeRegistry;
	typedef std::map<string, IGameObjectExtensionCreatorBase *>	TProjectileRegistry;
	typedef std::map<EntityId, CProjectile *>				TProjectileMap;
	typedef VectorMap<IEntityClass*, const SAmmoParams*>		TAmmoTypeParams;
	typedef std::vector<string>											TFolderList;
	typedef std::vector<IEntity*>	TIEntityVector;

public:
	CWeaponSystem(CGame *pGame, ISystem *pSystem);
	virtual ~CWeaponSystem();

	void Update(float frameTime);
	void Release();

	void GetMemoryStatistics( ICrySizer * );

	void Reload();

	// ILevelSystemListener
	virtual void OnLevelNotFound(const char *levelName) {};
	virtual void OnLoadingStart(ILevelInfo *pLevel) {};
	virtual void OnLoadingComplete(ILevel *pLevel);
	virtual void OnLoadingError(ILevelInfo *pLevel, const char *error) {};
	virtual void OnLoadingProgress(ILevelInfo *pLevel, int progressAmount) {};
	//~ILevelSystemListener

	IFireMode *CreateFireMode(const char *name);
	void RegisterFireMode(const char *name, IFireMode *(*)());
	
	IZoomMode *CreateZoomMode(const char *name);
	void RegisterZoomMode(const char *name, IZoomMode *(*)());

	CProjectile *SpawnAmmo(IEntityClass* pAmmoType, bool isRemote=false);
	bool IsServerSpawn(IEntityClass* pAmmoType) const;
	void RegisterProjectile(const char *name, IGameObjectExtensionCreatorBase *pCreator);
	const SAmmoParams* GetAmmoParams(IEntityClass* pAmmoType) const;

	void AddProjectile(IEntity *pEntity, CProjectile *pProjectile);
	void RemoveProjectile(CProjectile *pProjectile);
	CProjectile *GetProjectile(EntityId entityId);
	int	QueryProjectiles(SProjectileQuery& q);

	CTracerManager &GetTracerManager() { return m_tracerManager; };

	void Scan(const char *folderName);
	bool ScanXML(XmlNodeRef &root, const char *xmlFile);

  static void DebugGun(IConsoleCmdArgs *args = 0);
	static void RefGun(IConsoleCmdArgs *args = 0);

private:  
	CGame				*m_pGame;
	ISystem			*m_pSystem;
	IItemSystem	*m_pItemSystem;

	CTracerManager			m_tracerManager;

	TFireModeRegistry		m_fmregistry;
	TZoomModeRegistry		m_zmregistry;
	TProjectileRegistry	m_projectileregistry;
	TAmmoTypeParams			m_ammoparams;
	TProjectileMap			m_projectiles;

	TFolderList					m_folders;
	bool								m_reloading;
	bool								m_recursing;

	ICVar								*m_pPrecache;
	TIEntityVector			m_queryResults;//for caching queries results
};



#endif //__WEAPONSYSTEM_H__