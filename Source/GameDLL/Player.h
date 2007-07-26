/*************************************************************************
  Crytek Source File.
  Copyright (C), Crytek Studios, 2001-2004.
 -------------------------------------------------------------------------
  $Id$
  $DateTime$
  Description: Implements the player.
  
 -------------------------------------------------------------------------
  History:
  - 29:9:2004: Created by Filippo De Luca

*************************************************************************/
#ifndef __PLAYER_H__
#define __PLAYER_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "Actor.h"
#include "NanoSuit.h"
#include <IActionMapManager.h>
#include "Network/NetActionSync.h"

class CPlayerMovement;
class CPlayerRotation;
struct IPlayerInput;
class CHUD;
class CVehicleClient;
struct IDebugHistory;
struct IDebugHistoryManager;

struct SPlayerStats : public SActorStats
{
	float onGroundWBoots;

	float inMovement;//for how long the player is moving
	float inRest;//for how long the player is standing still

	bool landed;
	bool jumped;

	float jumpLock;
	
	float leanAmount;
	
	float shakeAmount; 

	Vec3 physCamOffset;
	float heightPivot;

	// falling things
	float fallSpeed;
		
	bool isFiring;

	bool isThirdPerson;
	
	bool isWalkingOnWater;
	
	CCoherentValue<uint8> followCharacterHead;
	bool isStandingUp;

	CCoherentValue<uint8> firstPersonBody;

	//useful for smoothing out the view
	float smoothViewZ;
	uint8 smoothZType;

	bool animationControlled;

	//cheating stuff
	uint8 flyMode;//0 no fly, 1 fly mode, 2 fly mode + noclip
	EntityId spectatorTarget;	// which player we are watching
	uint8 spectatorMode;
	// 0=off, 
	// 1=on,no-move,
	// 2=on,move
	// 3=on,nomove,a fake spectatormode used by FlowPlayerStagingNode.cpp
	// SNH: added the CActor::EActorSpectatorMode enum for the above and more...
	
	Vec3 upVector;
	Vec3 groundNormal;

	Vec3 FPWeaponPos;
	Ang3 FPWeaponAngles;
	Vec3 FPWeaponPosOffset;
	Ang3 FPWeaponAnglesOffset;

	Vec3 FPSecWeaponPos;
	Ang3 FPSecWeaponAngles;
	Vec3 FPSecWeaponPosOffset;
	Ang3 FPSecWeaponAnglesOffset;

	Vec3 gBootsSpotNormal;
	
	//
	Ang3 angularVel;
	
	Ang3 angularImpulse;
	float angularImpulseDeceleration;
	float angularImpulseTime;
	float angularImpulseTimeMax;

	//
	float thrusterSprint;

	//ladder
	bool isOnLadder;
	uint ladderMaterial;

	Vec3	ladderTop;
	Vec3	ladderBottom;
	Vec3  ladderOrientation;
	Vec3  ladderUpDir;
	bool  ladderMovingUp;
	bool  ladderMovingDown;
	int		ladderAction;		// actually a ELadderActionType

	Quat  playerRotation;

	bool	bSprinting;
	bool  bLookingAtFriendlyAI;

	//
	float viewFov;

	//Force crouch
	float forceCrouchTime;

	SPlayerStats()
	{
		memset(this,0,sizeof(SPlayerStats));
		new (this) SActorStats();

		mass = 80.0f;

		inAir = 1.0f;

		firstPersonBody = 2;

		upVector.Set(0,0,1);
		zeroGUp.Set(0,0,1);

		thrusterSprint = 1.0f;
		bSprinting = false;

		groundMaterialIdx = -1;
		
		inFreefall = -1;

		ladderMaterial = 0;

		forceCrouchTime = 0;
		bLookingAtFriendlyAI = false;

		inWater = -1000.0f;
		waterLevel = 10.0f;

		spectatorMode = 0;
		spectatorTarget = 0;
	}

	void Serialize( TSerialize ser, unsigned aspects );
};

struct SPlayerParams : public SActorParams
{
	float sprintMultiplier;
	float strafeMultiplier;
	float backwardMultiplier;
	float grabMultiplier;
	float afterburnerMultiplier;

	float inertia;
	float inertiaAccel;
		
	float jumpHeight;

	float slopeSlowdown;//cant find a better name for this
		
	float leanShift;
	float leanAngle;

	float thrusterImpulse;
	float thrusterStabilizeImpulse;

	float gravityBootsMultipler;

	Vec3	hudOffset;
	Ang3	hudAngleOffset;

	//
	char animationAppendix[32];

	SPlayerParams()
	{
 		memset(this,0,sizeof(SPlayerParams));
		new (this) SActorParams();

		sprintMultiplier = 1.25f;
		strafeMultiplier = 0.9f;
		backwardMultiplier = 0.9f;
		grabMultiplier = 0.5f;
		afterburnerMultiplier = 1.5f;

		inertia = 7.0f;
		inertiaAccel = 11.0f;
		
		jumpHeight = 1.0f;

		slopeSlowdown = 2.1f;
		
		leanShift = 0.5f;
		leanAngle = 11.0f;
		
		thrusterImpulse = 5.0f;
		thrusterStabilizeImpulse = 1.0f;

		gravityBootsMultipler = 0.7f;

		strcpy(animationAppendix,"nw");
	}
};

struct IPlayerEventListener
{
	enum ESpecialMove
	{
		eSM_Jump = 0,
		eSM_SpeedSprint,
		eSM_StrengthJump,
	};

	virtual void OnEnterVehicle(IActor* pActor, const char* strVehicleClassName, const char* strSeatName, bool bThirdPerson) {};
	virtual void OnExitVehicle(IActor* pActor) {};
	virtual void OnToggleThirdPerson(IActor* pActor, bool bThirdPerson) {};
	virtual void OnItemDropped(IActor* pActor, EntityId itemId) {};
	virtual void OnItemPickedUp(IActor* pActor, EntityId itemId) {};
	virtual void OnItemUsed(IActor* pActor, EntityId itemId) {};
	virtual void OnStanceChanged(IActor* pActor, EStance stance) {};
	virtual void OnSpecialMove(IActor* pActor, ESpecialMove move) {};
	virtual void OnDeath(IActor* pActor, bool bIsGod) {};
};

class CPlayerView;

class CPlayer :
	public CActor
{
	friend class CPlayerMovement;
	friend class CPlayerRotation;
	friend class CPlayerInput;
	friend class CPlayerView;
	friend class CNetPlayerInput;

public:

	enum EPlayerSounds //most of these sounds are supposed to be played, when the nanosuit is not in use
	{
		ESound_Player_First,
		ESound_Run,
		ESound_StopRun,
		ESound_Jump,
		ESound_Fall_Drop,
		ESound_Melee,
		ESound_Fear,
		ESound_Choking,
		ESound_Hit_Wall,
		ESound_UnderwaterBreathing,
		ESound_Drowning,
		ESound_DiveIn,
		ESound_DiveOut,
		ESound_Underwater,
		ESound_Player_Last
	};

	enum EClientPostEffect //first person screen effects for the client player
	{
		EEffect_ChromaShift = 1,
		EEffect_AlienInterference,
		EEffect_WaterDroplets
	};

	enum ELadderState
	{
		eLS_Exit,
		eLS_Climb
	};

	enum ELadderDirection
	{
		eLDIR_Up,
		eLDIR_Down,
		eLDIR_Stationary
	};

	enum ELadderActionType
	{
		eLAT_None,
		eLAT_ReachedEnd,
		eLAT_Jump,
		eLAT_StrafeLeft,
		eLAT_StrafeRight,
		eLAT_Use,
		eLAT_Die,
	};

	static const int ASPECT_NANO_SUIT_SETTING = eEA_GameClientStatic;
	static const int ASPECT_NANO_SUIT_ENERGY = eEA_GameServerDynamic;

	static const int ASPECT_HEALTH				= eEA_GameServerStatic;
	static const int ASPECT_FROZEN				= eEA_GameServerStatic;
	static const int ASPECT_CURRENT_ITEM	= eEA_GameClientStatic;

	CPlayer();
	virtual ~CPlayer();

	virtual void Draw(bool draw);
	virtual void BindInputs( IAnimationGraphState * pAGState );
	//marcok: GC hack
	virtual bool ShouldSwim();
	virtual bool IsSwimming() {	return m_bSwimming; }
	virtual bool Init( IGameObject * pGameObject );
	virtual void PostInit( IGameObject * pGameObject );
	virtual void InitClient( int channelId );
	virtual void InitLocalPlayer();
	virtual void ProcessEvent(SEntityEvent& event);
	virtual void SetAuthority( bool auth );
	virtual void SerializeXML( XmlNodeRef& node, bool bLoading );
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot);
	virtual void PrePhysicsUpdate();
	virtual void UpdateView(SViewParams &viewParams);
	virtual void PostUpdateView(SViewParams &viewParams);
	virtual void UpdateFirstPersonEffects(float frameTime);
	virtual void GetMemoryStatistics(ICrySizer * s);
	virtual void UpdateFootSteps(float frameTime);
	virtual int32 GetArmor() const;
	virtual int32 GetMaxArmor() const;

	virtual IEntity *LinkToVehicle(EntityId vehicleId);
	virtual IEntity *LinkToEntity(EntityId entityId);
	virtual void LinkToMountedWeapon(EntityId weaponId);

	virtual void SupressViewBlending() { m_viewBlending = false; };

	Vec3 GetLastRequestedVelocity() const { return m_lastRequestedVelocity; }
	void SetDeathTimer() { m_fDeathTime = gEnv->pTimer->GetFrameStartTime().GetSeconds(); }

	static  const char* GetActorClassType() { return "CPlayer"; }
	virtual const char* GetActorClass() const { return CPlayer::GetActorClassType(); }

  virtual bool IsCloaked() const;

//	ILINE bool FeetUnderWater() const { return m_bFeetUnderWater; }

protected:
	virtual IActorMovementController * CreateMovementController();
	virtual void SetIK( const SActorFrameMovementParams& );

public:
  struct SAlienInterferenceParams
  {
    SAlienInterferenceParams() : maxdist(0.f) {}
    SAlienInterferenceParams(float dist) : maxdist(dist) {}

    float maxdist;
  };

	struct UnfreezeParams
	{
		UnfreezeParams() {};
		UnfreezeParams(float dlt): delta(dlt) {};

		float delta;
		void SerializeWith(TSerialize ser)
		{
			ser.Value("delta", delta, 'frzn');
		}
	};
	struct LadderParams
	{
		LadderParams() : topPos(ZERO), bottomPos(ZERO), reason(eLAT_None) {};
		LadderParams(Vec3 _topPos, Vec3 _bottomPos, ELadderActionType _reason)
			: topPos(_topPos), bottomPos(_bottomPos), reason(_reason) {};

		void SerializeWith(TSerialize ser)
		{
			ser.Value("topPos", topPos, 'wrld');
			ser.Value("bottomPos", bottomPos, 'wrld');
			ser.Value("reason", reason);
		}

		Vec3 topPos;
		Vec3 bottomPos;
		int reason;
	};
	struct HitAssistanceParams
	{
		HitAssistanceParams() : assistance(false) {};
		HitAssistanceParams(bool _assistance) : assistance(_assistance) {};

		void SerializeWith(TSerialize ser)
		{
			ser.Value("assistance", assistance, 'bool');
		}

		bool assistance;
	};


	struct EMPParams
	{
		EMPParams(): entering(false) {};
		EMPParams(bool enter): entering(enter) {};

		bool entering;

		void SerializeWith(TSerialize ser)
		{
			ser.Value("entering", entering, 'bool');
		}
	};

	DECLARE_SERVER_RMI_NOATTACH_FAST(SvRequestGrabOnLadder, LadderParams, eNRT_ReliableOrdered);
	DECLARE_SERVER_RMI_NOATTACH_FAST(SvRequestLeaveLadder, LadderParams, eNRT_ReliableOrdered);

	DECLARE_CLIENT_RMI_NOATTACH_FAST(ClGrabOnLadder, LadderParams, eNRT_ReliableOrdered);
	DECLARE_CLIENT_RMI_NOATTACH_FAST(ClLeaveLadder, LadderParams, eNRT_ReliableOrdered);

	DECLARE_SERVER_RMI_NOATTACH_FAST(SvRequestUnfreeze, UnfreezeParams, eNRT_ReliableUnordered);

	DECLARE_SERVER_RMI_NOATTACH(SvRequestStartDisarmMine, ItemIdParam, eNRT_ReliableUnordered);
	DECLARE_SERVER_RMI_NOATTACH(SvRequestEndDisarmMine, ItemIdParam, eNRT_ReliableUnordered);

	DECLARE_SERVER_RMI_NOATTACH_FAST(SvRequestHitAssistance, HitAssistanceParams, eNRT_ReliableUnordered);

	DECLARE_CLIENT_RMI_NOATTACH(ClEMP, EMPParams, eNRT_ReliableOrdered);

	//set/get actor status
	virtual void SetStats(SmartScriptTable &rTable);
	virtual void UpdateScriptStats(SmartScriptTable &rTable);
	virtual void UpdateStats(float frameTime);
	virtual void UpdateSwimStats(float frameTime);
	virtual void UpdateDrowning(float frameTime);
	virtual void SetParams(SmartScriptTable &rTable,bool resetFirst);
	virtual bool GetParams(SmartScriptTable &rTable);

	virtual float CalculatePseudoSpeed(bool wantSprint) const;

	// Accessed via function to allow game based modifiers to stance speed without multiplying the number of stances.
	virtual float GetStanceMaxSpeed(EStance stance) const;
	// Accessed via function to allow game based modifiers to stance speed without multiplying the number of stances.
	virtual float GetStanceNormalSpeed(EStance stance) const;

	virtual void ToggleThirdPerson();

	virtual int  IsGod();
	
	virtual void Revive( bool fromInit );
	virtual void Kill();

	//stances
	virtual Vec3	GetStanceViewOffset(EStance stance,float *pLeanAmt=NULL,bool withY = false) const;
	virtual bool IsThirdPerson() const;
	virtual void StanceChanged(EStance last);
  virtual bool TrySetStance(EStance stance);

	virtual void ResetAnimGraph();

	CNanoSuit *GetNanoSuit() const { return m_pNanoSuit; }
	void ActivateNanosuit(bool active);

	virtual void SetFlyMode(uint8 flyMode);
	virtual uint8 GetFlyMode() const { return m_stats.flyMode; };
	virtual void SetSpectatorMode(uint8 mode, EntityId targetId);
	virtual uint8 GetSpectatorMode() const { return m_stats.spectatorMode; };
	virtual EntityId GetSpectatorTarget() const { return m_stats.spectatorTarget; };
	virtual void SetSpectatorTarget(EntityId targetId);
	void MoveToSpectatorTargetPosition();

	virtual void SelectNextItem(int direction, bool keepHistory, const char *category);
	virtual void HolsterItem(bool holster);
	virtual void SelectLastItem(bool keepHistory);
	virtual void SelectItemByName(const char *name, bool keepHistory);
	virtual void SelectItem(EntityId itemId, bool keepHistory);

	virtual void RagDollize( bool fallAndPlay );
	virtual void HandleEvent( const SGameObjectEvent& event );

	virtual void UpdateAnimGraph(IAnimationGraphState * pState);
	virtual void PostUpdate(float frameTime);
	
	virtual void SetViewRotation( const Quat &rotation );
	virtual Quat GetViewRotation() const;
	virtual void EnableTimeDemo( bool bTimeDemo );

	virtual void SetViewAngleOffset(const Vec3 &offset) { if (!m_ignoreRecoil) m_viewAnglesOffset = offset; };
	virtual Vec3 GetViewAngleOffset() { return (Vec3)m_viewAnglesOffset; };

	virtual bool SetProfile(uint8 profile );

	virtual void Serialize( TSerialize ser, unsigned aspects );
	virtual void PostSerialize();
	//set/get actor params
	virtual void SetHealth( int health );
	virtual SActorStats *GetActorStats() { return &m_stats; };
	virtual const SActorStats *GetActorStats() const { return &m_stats; };
	virtual SActorParams *GetActorParams() { return &m_params; };
	virtual void PostPhysicalize();
	virtual void CameraShake(float angle,float shift,float duration,float frequency,Vec3 pos,int ID,const char* source="");
	virtual bool CreateCodeEvent(SmartScriptTable &rTable);
	ILINE virtual void VectorToLocal(Vec3 &v) {v = m_clientViewMatrix.GetInverted() * v;}
	ILINE virtual Matrix34 GetViewMatrix() const { return m_clientViewMatrix; }
	virtual void AddAngularImpulse(const Ang3 &angular,float deceleration, float duration);
	virtual void SetAngles(const Ang3 &angles);
	virtual Ang3 GetAngles();
	virtual void PlayAction(const char *action,const char *extension, bool looping=false);
	virtual void UpdateGrab(float frameTime);
	virtual float GetActorStrength() const;
	virtual void Freeze(bool freeze);
	virtual void ProcessBonesRotation(ICharacterInstance *pCharacter,float frameTime);
	virtual void ProcessIKLegs(ICharacterInstance *pCharacter,float frameTime);
	virtual void Landed(float fallSpeed);

	//Player can grab north koreans
	virtual int GetActorSpecies() { return eGCT_HUMAN; }

	//Support sleep 
	virtual bool CanSleep() {return true;}
	
	void UpdateParachute(float frameTime);
	void ChangeParachuteState(int8 newState);

	void ProcessCharacterOffset();

	const Quat& GetBaseQuat() { return m_baseQuat; }
	const Quat& GetViewQuat() { return m_viewQuat; }
	const Quat& GetViewQuatFinal() { return m_viewQuatFinal; }
	virtual void ResetAnimations();
	float GetMassFactor() const;
  virtual float GetFrozenAmount(bool stages=false) const; 
	IPlayerInput* GetPlayerInput() const {return m_pPlayerInput.get();}
		
	virtual void SwitchDemoModeSpectator(bool activate);
	bool IsTimeDemo() const { return m_timedemo; }
	void ForceFreeFall() { m_stats.inFreefall = true; }

	void RegisterPlayerEventListener	(IPlayerEventListener *pPlayerEventListener);
	void UnregisterPlayerEventListener(IPlayerEventListener *pPlayerEventListener);

	ILINE bool GravityBootsOn() const
	{
		return false; //(m_actions & ACTION_GRAVITYBOOTS && InZeroG() )?true:false;
	}

	void Stabilize(bool stabilize);
	ILINE bool GetStabilize() const { return m_bStabilize; }
	ILINE float GetSpeedLean() const { return m_fSpeedLean; }
	ILINE void SetSpeedLean(float speedLean) { m_fSpeedLean = speedLean; }
	
	ILINE void EnableParachute(bool enable) { m_parachuteEnabled=enable; };
	ILINE bool IsParachuteEnabled() const { return m_parachuteEnabled; };

	virtual bool UseItem(EntityId itemId);
	virtual bool PickUpItem(EntityId itemId, bool sound);
	virtual bool DropItem(EntityId itemId, float impulseScale=1.0f, bool slectNext=true, bool byDeath=false);

	ILINE const Vec3& GetEyeOffset() const { return m_eyeOffset; }
	ILINE const Vec3& GetWeaponOffset() const { return m_weaponOffset; }

	void UpdateUnfreezeInput(const Ang3 &deltaRotation, const Vec3 &deltaMovement, float mult);

	void SpawnParticleEffect(const char* effectName, const Vec3& pos, const Vec3& dir);
	void PlaySound(EPlayerSounds sound, bool play = true, bool param = false, const char* paramName = NULL, float paramValue = 0.0f);

	//Ladders
	bool IsLadderUsable();
	void RequestGrabOnLadder(ELadderActionType reason);
	void GrabOnLadder(ELadderActionType reason);
	void RequestLeaveLadder(ELadderActionType reason);
	void LeaveLadder(ELadderActionType reason);

	bool UpdateLadderAnimGraph(ELadderState eLS, ELadderDirection eLDIR, float time = 0.0f);
	void EvaluateMovementOnLadder(float &verticalSpeed, float animationTime);

	//Pick Up Items
	bool NeedToCrouch();

	// mines
	void StartDisarmMine(EntityId entityId);
	void EndDisarmMine(EntityId entityId);
	void RemoveMineEntity(EntityId entityId);

	//First person fists/hands actions
	void EnterFirstPersonSwimming();
	void ExitFirstPersonSwimming();
	void UpdateFirstPersonSwimming();
	void UpdateFirstPersonSwimmingEffects(bool exitWater, float velSqr);
	void UpdateFirstPersonFists();

	//Hit assistance
	bool HasHitAssistance();

	struct SStagingParams
	{
		SStagingParams() : 
			bActive(false), bLocked(false), vLimitDir(ZERO), vLimitRangeH(0.0f), vLimitRangeV(0.0f)
		{
		}

		bool  bActive;
		bool  bLocked;
		Vec3  vLimitDir;
		float vLimitRangeH;
		float vLimitRangeV;
		void Serialize(TSerialize ser)
		{
			assert( ser.GetSerializationTarget() != eST_Network );
			ser.BeginGroup("SStagingParams");
			ser.Value("bActive", bActive);
			ser.Value("bLocked", bLocked);
			ser.Value("vLimitDir", vLimitDir);
			ser.Value("vLimitRangeH", vLimitRangeH);
			ser.Value("vLimitRangeV", vLimitRangeV);
			ser.EndGroup();
		}
	};

	void StagePlayer(bool bStage, SStagingParams* pStagingParams = 0); 

private:
	void AnimationControlled(bool activate);
	bool ShouldUsePhysicsMovement();
  void Debug();
	typedef std::list<IPlayerEventListener *> TPlayerEventListeners;
	TPlayerEventListeners m_playerEventListeners;

protected:
	Vec3		m_modelOffset;
	float		m_groundElevation;
	Ang3 		m_viewAnglesOffset;//used for recoil
	Ang3		m_headAngles;//head angles in local space, for bones rotation

	// bool IsMaterialBootable(int matId) const;
	ILINE bool IsMaterialBootable(int matId) const { return true; }

  void InitInterference();

	CNanoSuit *m_pNanoSuit;

	Ang3		m_angleOffset;	// Used only by the view system. (retained state)

	Quat	m_viewQuat;//view quaternion
	Quat	m_viewQuatFinal;//the final exact view quaternion (with recoil added)
	Quat	m_baseQuat;//base rotation quaternion, rotating on the Z axis

	Matrix34 m_clientViewMatrix;

	Vec3		m_eyeOffset;	// View system - used to interpolate to goal eye offset
												//the offset from the entity origin to eyes, its not the real offset vector but its referenced to player view direction.

	Vec3		m_eyeOffsetView; //this is exclusive for CPlayerView to use, do not touch it outside CPlayerView

	Vec3		m_weaponOffset;

	float		m_viewRoll;

	Vec3		m_upVector;//using gravity boots (or when prone) is the normal of the surface where the player is standing on, otherwise is the default (0,0,1)

	Vec3		m_bobOffset;
	
	Vec3		m_FPWeaponLastDirVec;
	Vec3		m_FPWeaponOffset;
	Ang3		m_FPWeaponAngleOffset;

	// updated by PlayerMovement for tracking time based acceleration
	Vec3		m_velocity;
	// updated by PlayerMovement to allow for turning faster than we can in one frame
	Quat    m_turnTarget;
	Vec3		m_lastRequestedVelocity;
	Vec3    m_lastKnownPosition;

	SPlayerStats		m_stats;
	SPlayerParams		m_params;
	
	std::auto_ptr<IPlayerInput> m_pPlayerInput;

	// for foot/leg ik
	Vec3 m_feetWpos[2];

	// compatibility with old code: which actions are set
	int m_actions;

	// TODO: wish i could figure out a way of removing this
	// was the rotation forced this frame?
	bool m_forcedRotation;
	bool m_viewBlending;
	bool m_ignoreRecoil;
	bool m_timedemo;

	//Demo Mode
	bool m_bDemoModeSpectator;

	// animation graph input ids
	IAnimationGraph::InputID m_inputAction;
	IAnimationGraph::InputID m_inputItem;
	IAnimationGraph::InputID m_inputUsingLookIK;
	IAnimationGraph::InputID m_inputAiming;
	IAnimationGraph::InputID m_inputVehicleName;
	IAnimationGraph::InputID m_inputVehicleSeat;
	
	// probably temporary, feel free to figure out better place
	float m_lastAnimControlled;
	Quat m_lastAnimContRot;
	Vec3 m_lastAnimContPos;
	bool m_bSwimming;
	bool m_bSprinting;
	bool m_bUnderwater;
	bool m_bStabilize;
	float m_fSpeedLean;
	bool m_bCrawling;
	float m_fDeathTime;
	float m_drownEffectDelay;
	float m_underwaterBubblesDelay;

	// used by parachute. 
	int			m_nParachuteSlot;
	float		m_fParachuteMorph; //0..1 to play morph targets
	bool		m_parachuteEnabled;
	float		m_openParachuteTimer;
	bool    m_openingParachute;

	CVehicleClient* m_pVehicleClient;

	//sound loops that are played when we don't have a nanosuit
	float		 m_sprintTimer;
	tSoundID m_sounds[ESound_Player_Last];
	//client / localActor active first person effects
	std::vector<EClientPostEffect> m_clientPostEffects;

  typedef std::map<IEntityClass*, const SAlienInterferenceParams> TAlienInterferenceParams;
  static TAlienInterferenceParams m_interferenceParams;
      
  IDebugHistoryManager*   m_pDebugHistory;
  IDebugHistory*          m_pMovementDebug;
  IDebugHistory*          m_pDeltaXDebug;
  IDebugHistory*          m_pDeltaYDebug;

	float										m_mineDisarmStartTime;
	EntityId								m_mineBeingDisarmed;
	bool										m_disarmingMine;
  bool                    m_bSpeedSprint;
	bool										m_bHasAssistance;

	IGameObjectExtension*		m_pVoiceListener;
	IGameObjectExtension*		m_pInteractor;

	SStagingParams m_stagingParams;
};


#endif //__PLAYER_H__