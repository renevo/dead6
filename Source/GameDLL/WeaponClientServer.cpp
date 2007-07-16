/*************************************************************************
	Crytek Source File.
	Copyright (C), Crytek Studios, 2001-2004.
	-------------------------------------------------------------------------
	$Id$
	$DateTime$

	-------------------------------------------------------------------------
	History:
	- 15:2:2006   12:50 : Created by M�rcio Martins

*************************************************************************/
#include "StdAfx.h"
#include "Weapon.h"
#include "Actor.h"
#include "Game.h"
#include "GameRules.h"

/*
#define CHECK_OWNER_REQUEST()	\
	{ \
		uint16 channelId=m_pGameFramework->GetGameChannelId(pNetChannel);	\
		IActor *pOwnerActor=GetOwnerActor(); \
		if (!pOwnerActor || pOwnerActor->GetChannelId()!=channelId) \
		{ \
			CryLogAlways("[gamenet] Disconnecting %s. Bogus weapon action '%s' request! %s %d!=%d (%s!=%s)", \
			pNetChannel->GetName(), __FUNCTION__, pOwnerActor?pOwnerActor->GetEntity()->GetName():"null", \
			pOwnerActor?pOwnerActor->GetChannelId():0, channelId,\
			pOwnerActor?pOwnerActor->GetEntity()->GetName():"null", \
			m_pGameFramework->GetIActorSystem()->GetActorByChannelId(channelId)?m_pGameFramework->GetIActorSystem()->GetActorByChannelId(channelId)->GetEntity()->GetName():"null"); \

			return false; \
		} \
	} \
*/

#define CHECK_OWNER_REQUEST()	\
	{ \
		uint16 channelId=m_pGameFramework->GetGameChannelId(pNetChannel);	\
		IActor *pOwnerActor=GetOwnerActor(); \
		if (pOwnerActor && (pOwnerActor->GetChannelId()!=channelId && GetISystem()->IsDemoMode() != 2)) \
			return true; \
	} \


//------------------------------------------------------------------------
int CWeapon::NetGetCurrentFireMode() const
{
	return m_fmId;
}

//------------------------------------------------------------------------
void CWeapon::NetSetCurrentFireMode(int id)
{
	SetCurrentFireMode(id);
}

//------------------------------------------------------------------------
int CWeapon::NetGetCurrentAmmoCount() const
{
	if (!m_fm)
		return 0;

	return GetAmmoCount(m_fm->GetAmmoType());
}

//------------------------------------------------------------------------
void CWeapon::NetSetCurrentAmmoCount(int count)
{
	if (!m_fm)
		return;

	SetAmmoCount(m_fm->GetAmmoType(), count);
}

//------------------------------------------------------------------------
void CWeapon::NetShoot(const Vec3 &hit, int predictionHandle)
{
	if (m_fm)
		m_fm->NetShoot(hit, predictionHandle);
}

//------------------------------------------------------------------------
void CWeapon::NetShootEx(const Vec3 &pos, const Vec3 &dir, const Vec3 &vel, const Vec3 &hit, int predictionHandle)
{
	if (m_fm)
		m_fm->NetShootEx(pos, dir, vel, hit, predictionHandle);
}

//------------------------------------------------------------------------
void CWeapon::NetStartFire(EntityId shooterId)
{
	if (m_fm)
		m_fm->NetStartFire(shooterId);
}

//------------------------------------------------------------------------
void CWeapon::NetStopFire(EntityId shooterId)
{
	if (m_fm)
		m_fm->NetStopFire(shooterId);
}

//------------------------------------------------------------------------
void CWeapon::NetStartMeleeAttack(EntityId shooterId, bool weaponMelee)
{
	if (weaponMelee && m_melee)
		m_melee->NetStartFire(shooterId);
	else if (m_fm)
		m_fm->NetStartFire(shooterId);
}

//------------------------------------------------------------------------
void CWeapon::NetMeleeAttack(bool weaponMelee, const Vec3 &pos, const Vec3 &dir)
{
	if (weaponMelee && m_melee)
	{
		m_melee->NetShootEx(pos, dir, ZERO, ZERO, 0);
		if (IsServer())
			m_pGameplayRecorder->Event(GetOwner(), GameplayEvent(eGE_WeaponMelee, 0, 0, (void *)GetEntityId()));
	}
	else if (m_fm)
	{
		m_fm->NetShootEx(pos, dir, ZERO, ZERO, 0);
		if (IsServer())
			m_pGameplayRecorder->Event(GetOwner(), GameplayEvent(eGE_WeaponMelee, 0, 0, (void *)GetEntityId()));
	}
}

//------------------------------------------------------------------------
void CWeapon::NetZoom(float fov)
{
	if (CActor *pOwner=GetOwnerActor())
	{
		if (pOwner->IsClient())
			return;

		SActorParams *pActorParams = pOwner->GetActorParams();
		if (!pActorParams)
			return;

		pActorParams->viewFoVScale = fov;
	}
}

//------------------------------------------------------------------------
void CWeapon::RequestShoot(IEntityClass* pAmmoType, const Vec3 &pos, const Vec3 &dir, const Vec3 &vel, const Vec3 &hit, int predictionHandle, bool forceExtended)
{
	IActor *pActor=m_pGameFramework->GetClientActor();

	if ((!pActor || pActor->IsClient()) && IsClient())
	{
		if (pActor)
			pActor->GetGameObject()->Pulse('bang');
		GetGameObject()->Pulse('bang');

		if (IsServerSpawn(pAmmoType) || forceExtended)
		{
			GetGameObject()->InvokeRMI(CWeapon::SvRequestShootEx(), SvRequestShootExParams(pos, dir, vel, hit, predictionHandle), eRMI_ToServer);
		}
		else
			GetGameObject()->InvokeRMI(CWeapon::SvRequestShoot(), SvRequestShootParams(pos, dir, hit, predictionHandle), eRMI_ToServer);
	}
	else if (!IsClient() && IsServer())
	{
		if (IsServerSpawn(pAmmoType) || forceExtended)
		{
			GetGameObject()->InvokeRMI(CWeapon::ClShoot(), ClShootParams(pos+dir*5.0f, predictionHandle), eRMI_ToAllClients);
			NetShootEx(pos, dir, vel, hit, predictionHandle);
		}
		else
		{
			GetGameObject()->InvokeRMI(CWeapon::ClShoot(), ClShootParams(hit, predictionHandle), eRMI_ToAllClients);
			NetShoot(hit, predictionHandle);
		}
	}
}

//------------------------------------------------------------------------
void CWeapon::RequestMeleeAttack(bool weaponMelee, const Vec3 &pos, const Vec3 &dir)
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if ((!pActor || pActor->IsClient()) && IsClient())
		GetGameObject()->InvokeRMI(CWeapon::SvRequestMeleeAttack(), RequestMeleeAttackParams(weaponMelee, pos, dir), eRMI_ToServer);
	else if (!IsClient() && IsServer())
	{
		GetGameObject()->InvokeRMI(CWeapon::ClMeleeAttack(), RequestMeleeAttackParams(weaponMelee, pos, dir), eRMI_ToAllClients);
		NetMeleeAttack(weaponMelee, pos, dir);
	}
}

//------------------------------------------------------------------------
void CWeapon::RequestStartFire()
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if ((!pActor || pActor->IsClient()) && IsClient())
		GetGameObject()->InvokeRMI(CWeapon::SvRequestStartFire(), EmptyParams(), eRMI_ToServer);
	else if (!IsClient() && IsServer())
		GetGameObject()->InvokeRMI(CWeapon::ClStartFire(), EmptyParams(), eRMI_ToAllClients);
}

//------------------------------------------------------------------------
void CWeapon::RequestStartMeleeAttack(bool weaponMelee)
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if ((!pActor || pActor->IsClient()) && IsClient())
		GetGameObject()->InvokeRMI(CWeapon::SvRequestStartMeleeAttack(), RequestStartMeleeAttackParams(weaponMelee), eRMI_ToServer);
	else if (!IsClient() && IsServer())
	{
		GetGameObject()->InvokeRMI(CWeapon::ClStartMeleeAttack(), RequestStartMeleeAttackParams(weaponMelee), eRMI_ToAllClients);
		NetStartMeleeAttack(GetOwnerId(), weaponMelee);
	}
}

//------------------------------------------------------------------------
void CWeapon::RequestZoom(float fov)
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if ((!pActor || pActor->IsClient()) && IsClient())
		GetGameObject()->InvokeRMI(CWeapon::SvRequestZoom(), ZoomParams(fov), eRMI_ToServer);
	else if (!IsClient() && IsServer())
	{
		GetGameObject()->InvokeRMI(CWeapon::ClZoom(), ZoomParams(fov), eRMI_ToAllClients);
		NetZoom(fov);
	}
}


//------------------------------------------------------------------------
void CWeapon::RequestStopFire()
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if ((!pActor || pActor->IsClient()) && IsClient())
		GetGameObject()->InvokeRMI(CWeapon::SvRequestStopFire(), EmptyParams(), eRMI_ToServer);
	else if (!IsClient() && IsServer())
		GetGameObject()->InvokeRMI(CWeapon::ClStopFire(), EmptyParams(), eRMI_ToAllClients);
}

//------------------------------------------------------------------------
void CWeapon::RequestReload()
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if ((!pActor || pActor->IsClient()) && IsClient())
		GetGameObject()->InvokeRMI(SvRequestReload(), EmptyParams(), eRMI_ToServer);
	else if (!IsClient() && IsServer())
		GetGameObject()->InvokeRMI(CWeapon::ClReload(), EmptyParams(), eRMI_ToAllClients);
}

//-----------------------------------------------------------------------
void CWeapon::RequestCancelReload()
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if ((!pActor || pActor->IsClient()) && IsClient())
		GetGameObject()->InvokeRMI(SvRequestCancelReload(), EmptyParams(), eRMI_ToServer);
	else if (!IsClient() && IsServer())
		GetGameObject()->InvokeRMI(CWeapon::ClCancelReload(), EmptyParams(), eRMI_ToAllClients);

}

//------------------------------------------------------------------------
void CWeapon::RequestFireMode(int fmId)
{
	IActor *pActor=m_pGameFramework->GetClientActor();
	if (!pActor || pActor->IsClient())
	{
		if (IsServer())
			SetCurrentFireMode(fmId);	// serialization will fix the rest.
		else
			GetGameObject()->InvokeRMI(SvRequestFireMode(), SvRequestFireModeParams(fmId), eRMI_ToServer);
	}
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestStartFire)
{
	CHECK_OWNER_REQUEST();

	GetGameObject()->InvokeRMI(CWeapon::ClStartFire(), params, eRMI_ToOtherClients, 
		m_pGameFramework->GetGameChannelId(pNetChannel));

	CActor *pActor=GetActorByNetChannel(pNetChannel);
	IActor *pLocalActor=m_pGameFramework->GetClientActor();
	bool isLocal = pLocalActor && pActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

	if (!isLocal)
		NetStartFire(pActor?pActor->GetEntityId():0);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestStopFire)
{
	CHECK_OWNER_REQUEST();

	GetGameObject()->InvokeRMI(CWeapon::ClStopFire(), params, eRMI_ToOtherClients, 
		m_pGameFramework->GetGameChannelId(pNetChannel));

	CActor *pActor=GetActorByNetChannel(pNetChannel);
	IActor *pLocalActor=m_pGameFramework->GetClientActor();
	bool isLocal = pLocalActor && pActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

	if (!isLocal)
		NetStopFire(pActor?pActor->GetEntityId():0);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClStartFire)
{
	NetStartFire(GetOwnerId());

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClStopFire)
{
	NetStopFire(GetOwnerId());

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestShoot)
{
	CHECK_OWNER_REQUEST();

	bool ok=true;
	CActor *pActor=GetActorByNetChannel(pNetChannel);
	if (!pActor || pActor->GetHealth()<=0)
		ok=false;

	if (ok)
	{
		if (IActor *pOwner=GetOwnerActor())
			pOwner->GetGameObject()->Pulse('bang');
		GetGameObject()->Pulse('bang');

		static ray_hit rh;

		IEntity* pEntity = NULL;
		if ( gEnv->pPhysicalWorld->RayWorldIntersection(params.pos, params.dir*4096.0f, ent_all & ~ent_terrain, rwi_stop_at_pierceable|rwi_ignore_back_faces, &rh, 1) )
			pEntity = gEnv->pEntitySystem->GetEntityFromPhysics(rh.pCollider);
		if (pEntity)
		{
			if(INetContext* pNC = gEnv->pGame->GetIGameFramework()->GetNetContext())
			{
				if(pNC->IsBound(pEntity->GetId()))
				{
					AABB bbox; pEntity->GetWorldBounds(bbox);
					bool hit0 = bbox.GetRadius() < 1.0f; // this (radius*2) must match the value in CompressionPolicy.xml ("hit0")
					Vec3 hitLocal = pEntity->GetWorldTM().GetInvertedFast() * rh.pt;
					//GetGameObject()->InvokeRMI(CWeapon::ClShootX(), ClShootXParams(pEntity->GetId(), hit0, hitLocal, params.predictionHandle),
					//	eRMI_ToOtherClients|eRMI_NoLocalCalls, m_pGameFramework->GetGameChannelId(pNetChannel));
					GetGameObject()->InvokeRMIWithDependentObject(CWeapon::ClShootX(), ClShootXParams(pEntity->GetId(), hit0, hitLocal, params.predictionHandle),
						eRMI_ToOtherClients|eRMI_NoLocalCalls, pEntity->GetId(), m_pGameFramework->GetGameChannelId(pNetChannel));

				}
			}
		}
		else
			GetGameObject()->InvokeRMI(CWeapon::ClShoot(), ClShootParams(params.hit, params.predictionHandle), 
				eRMI_ToOtherClients|eRMI_NoLocalCalls, m_pGameFramework->GetGameChannelId(pNetChannel));

		IActor *pLocalActor=m_pGameFramework->GetClientActor();
		bool isLocal = pLocalActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

		if (!isLocal)
			NetShoot(params.hit, params.predictionHandle);
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestShootEx)
{
	CHECK_OWNER_REQUEST();

	bool ok=true;
	CActor *pActor=GetActorByNetChannel(pNetChannel);
	if (!pActor || pActor->GetHealth()<=0)
		ok=false;

	if (ok)
	{
		if (IActor *pOwner=GetOwnerActor())
			pOwner->GetGameObject()->Pulse('bang');
		GetGameObject()->Pulse('bang');

		GetGameObject()->InvokeRMI(CWeapon::ClShoot(), ClShootParams(params.pos+params.dir*5.0f, params.predictionHandle),
			eRMI_ToOtherClients|eRMI_NoLocalCalls, m_pGameFramework->GetGameChannelId(pNetChannel));

		IActor *pLocalActor=m_pGameFramework->GetClientActor();
		bool isLocal = pLocalActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

		if (!isLocal)
			NetShootEx(params.pos, params.dir, params.vel, params.hit, params.predictionHandle);
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClShoot)
{
	NetShoot(params.hit, params.predictionHandle);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClShootX)
{
	if (IEntity* pEntity = gEnv->pEntitySystem->GetEntity(params.eid))
	{
		Vec3 hit = pEntity->GetWorldTM() * params.hit;
		NetShoot(hit, params.predictionHandle);
	}
	else
	{
		GameWarning("ClShootX: invalid entity id %.8x", params.eid);
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestStartMeleeAttack)
{
	CHECK_OWNER_REQUEST();

	GetGameObject()->InvokeRMI(CWeapon::ClStartMeleeAttack(), params, eRMI_ToOtherClients, 
		m_pGameFramework->GetGameChannelId(pNetChannel));

	CActor *pActor=GetActorByNetChannel(pNetChannel);
	IActor *pLocalActor=m_pGameFramework->GetClientActor();
	bool isLocal = pLocalActor && pActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

	if (!isLocal)
		NetStartMeleeAttack(GetOwnerId(), params.wmelee);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClStartMeleeAttack)
{
	NetStartMeleeAttack(GetOwnerId(), params.wmelee);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestMeleeAttack)
{
	CHECK_OWNER_REQUEST();

	bool ok=true;
	CActor *pActor=GetActorByNetChannel(pNetChannel);
	if (!pActor || pActor->GetHealth()<=0)
		ok=false;

	if (ok)
	{
		GetGameObject()->InvokeRMI(CWeapon::ClMeleeAttack(), params,
			eRMI_ToOtherClients|eRMI_NoLocalCalls, m_pGameFramework->GetGameChannelId(pNetChannel));

		IActor *pLocalActor=m_pGameFramework->GetClientActor();
		bool isLocal = pLocalActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

		if (!isLocal)
			NetMeleeAttack(params.wmelee, params.pos, params.dir);

		m_pGameplayRecorder->Event(GetOwner(), GameplayEvent(eGE_WeaponMelee, 0, 0, (void *)GetEntityId()));
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClMeleeAttack)
{
	NetMeleeAttack(params.wmelee, params.pos, params.dir);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestZoom)
{
	CHECK_OWNER_REQUEST();

	bool ok=true;
	CActor *pActor=GetActorByNetChannel(pNetChannel);
	if (!pActor || pActor->GetHealth()<=0)
		ok=false;

	if (ok)
	{
		GetGameObject()->InvokeRMI(CWeapon::ClZoom(), params,
			eRMI_ToOtherClients|eRMI_NoLocalCalls, m_pGameFramework->GetGameChannelId(pNetChannel));

		IActor *pLocalActor=m_pGameFramework->GetClientActor();
		bool isLocal = pLocalActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

		if (!isLocal)
			NetZoom(params.fov);

		int event=eGE_ZoomedOut;
		if (params.fov<0.99f)
			event=eGE_ZoomedIn;
		m_pGameplayRecorder->Event(GetOwner(), GameplayEvent(event, 0, 0, (void *)GetEntityId()));
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClZoom)
{
	NetZoom(params.fov);

	return true;
}


//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestFireMode)
{
	CHECK_OWNER_REQUEST();

	SetCurrentFireMode(params.id);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClSetFireMode)
{
	SetCurrentFireMode(params.id);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestReload)
{
	CHECK_OWNER_REQUEST();
	
	bool ok=true;
	CActor *pActor=GetActorByNetChannel(pNetChannel);
	if (!pActor || pActor->GetHealth()<=0)
		ok=false;

	if (ok)
	{
		GetGameObject()->InvokeRMI(CWeapon::ClReload(), params, eRMI_ToOtherClients|eRMI_NoLocalCalls, m_pGameFramework->GetGameChannelId(pNetChannel));

		IActor *pLocalActor=m_pGameFramework->GetClientActor();
		bool isLocal = pLocalActor && (pLocalActor->GetChannelId() == pActor->GetChannelId());

		if (!isLocal)
			Reload();

		m_pGameplayRecorder->Event(GetOwner(), GameplayEvent(eGE_WeaponReload, 0, 0, (void *)GetEntityId()));
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClReload)
{
	if (m_fm)
	{
		if(m_zm)
			m_fm->Reload(m_zm->GetCurrentStep());
		else
			m_fm->Reload(false);
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, SvRequestCancelReload)
{
	CHECK_OWNER_REQUEST();

	if(m_fm)
	{
		m_fm->CancelReload();
		GetGameObject()->InvokeRMI(CWeapon::ClCancelReload(), params, eRMI_ToOtherClients|eRMI_NoLocalCalls, m_pGameFramework->GetGameChannelId(pNetChannel));
	}

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClCancelReload)
{
	if(m_fm)
		m_fm->CancelReload();

	return true;
}
//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClStartLocking)
{
	if (m_fm)
		m_fm->StartLocking(params.entityId);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClLock)
{
	if (m_fm)
		m_fm->Lock(params.entityId);

	return true;
}

//------------------------------------------------------------------------
IMPLEMENT_RMI(CWeapon, ClUnlock)
{
	if (m_fm)
		m_fm->Unlock();

	return true;
}