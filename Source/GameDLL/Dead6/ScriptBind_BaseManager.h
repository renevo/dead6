////////////////////////////////////////////////////
// C&C: The Dead 6 - Core File
// Copyright (C), RenEvo Software & Designs, 2007
//
// ScriptBind_BaseManager.h
//
// Purpose: Script binding for the base manager
//
// File History:
//	- 7/22/07 : File created - KAK
////////////////////////////////////////////////////

#ifndef _D6C_SCRIPTBIND_BASEMANAGER_H_
#define _D6C_SCRIPTBIND_BASEMANAGER_H_

#include <IScriptSystem.h>
#include <ScriptHelpers.h>

struct IBaseManager;
struct ISystem;

class CScriptBind_BaseManager : public CScriptableBase
{
public:
	////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////
	CScriptBind_BaseManager(ISystem *pSystem);
private:
	CScriptBind_BaseManager(CScriptBind_BaseManager const&) {}
	CScriptBind_BaseManager& operator =(CScriptBind_BaseManager const&) {return *this;}

public:
	////////////////////////////////////////////////////
	// Destructor
	////////////////////////////////////////////////////
	virtual ~CScriptBind_BaseManager(void);

	////////////////////////////////////////////////////
	// AttachTo
	//
	// Purpose: Attaches binding to a base manager
	////////////////////////////////////////////////////
	virtual void AttachTo(IBaseManager *pBaseManager);

public:
	////////////////////////////////////////////////////
	// FindBuilding
	//
	// Purpose: Return a controller's script table
	//	by using the team and class names
	//
	// In:	szTeam - Owning team name
	//		szClass - Building class name
	//
	// Returns controller's script table or nil on error
	////////////////////////////////////////////////////
	virtual int FindBuilding(IFunctionHandler *pH, char const* szTeam, char const* szClass);

	////////////////////////////////////////////////////
	// SetBasePower
	//
	// Purpose: Set the power on all buildings
	//
	// In:	nTeamID - Team that owns the base
	//		bState - TRUE to turn power on, FALSE to turn
	//			power off
	////////////////////////////////////////////////////
	virtual int SetBasePower(IFunctionHandler *pH, int nTeamID, bool bState);

protected:
	////////////////////////////////////////////////////
	// RegisterGlobals
	//
	// Purpose: Registers any global values to the script
	//	system
	////////////////////////////////////////////////////
	void RegisterGlobals(void);

	////////////////////////////////////////////////////
	// RegisterMethods
	//
	// Purpose: Registers any binding methods to the script
	//	system
	////////////////////////////////////////////////////
	void RegisterMethods(void);

	IBaseManager *m_pBaseManager;
	ISystem *m_pSystem;
	IScriptSystem *m_pSS;
};

#endif //_D6C_SCRIPTBIND_BASEMANAGER_H_
