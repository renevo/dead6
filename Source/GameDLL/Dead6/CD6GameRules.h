////////////////////////////////////////////////////
// C&C: The Dead 6 - Core File
// Copyright (C), RenEvo Software & Designs, 2007
//
// CD6GameRules.h
//
// Purpose: Dead6 Core Game rules
//
// File History:
//	- 7/22/07 : File created - KAK
////////////////////////////////////////////////////

#ifndef _D6C_CD6GAMERULES_H_
#define _D6C_CD6GAMERULES_H_

#include "GameRules.h"

////////////////////////////////////////////////////
class CD6GameRules : public CGameRules
{
public:
	////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////
	CD6GameRules(void);
private:
	CD6GameRules(CD6GameRules const&) {}
	CD6GameRules& operator =(CD6GameRules const&) {return *this;}

public:
	////////////////////////////////////////////////////
	// Destructor
	////////////////////////////////////////////////////
	virtual ~CD6GameRules(void);

	////////////////////////////////////////////////////
	// ClearAllTeams
	//
	// Purpose: Clears all teams that are currently
	//	loaded in the gamerules and removes all
	//	actor/player team definitions that may be loaded
	////////////////////////////////////////////////////
	virtual void ClearAllTeams(void);

public:
	// Overloaded GameRules calls for team management
	virtual int CreateTeam(const char *name);
	virtual void RemoveTeam(int teamId);
	virtual const char *GetTeamName(int teamId) const;
	virtual int GetTeamId(const char *name) const;
	virtual int GetTeamCount() const;
	virtual int GetTeamPlayerCount(int teamId, bool inGame=false) const;
	virtual EntityId GetTeamPlayer(int teamId, int idx);
	virtual void GetTeamPlayers(int teamId, TPlayers &players);
	virtual void SetTeam(int teamId, EntityId entityId);
	virtual int GetTeam(EntityId entityId) const;
	virtual void SetTeamDefaultSpawnGroup(int teamId, EntityId spawnGroupId);
	virtual EntityId GetTeamDefaultSpawnGroup(int teamId);
};

#endif //_D6C_CD6GAMERULES_H_