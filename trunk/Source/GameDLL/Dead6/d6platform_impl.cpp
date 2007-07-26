////////////////////////////////////////////////////
// C&C: The Dead 6 - Core File
// Copyright (C), RenEvo Studios, 2007
//
// d6platform_impl.cpp
//
// Purpose: Platform implementation for core setup
//
// File History:
//	- 7/21/07 : File created - KAK
////////////////////////////////////////////////////

#include "stdafx.h"
#include <d6platform_impl.h>

// Core files
#include "CBaseManager.h"
#include "CTeamManager.h"

////////////////////////////////////////////////////
CD6CoreGlobalEnvironment::CD6CoreGlobalEnvironment(void)
{
	pSystem = NULL;
	pSystemGE = NULL;
	pBaseManager = NULL;
	pTeamManager = NULL;
}

////////////////////////////////////////////////////
CD6CoreGlobalEnvironment::~CD6CoreGlobalEnvironment(void)
{
	SAFE_DELETE(pBaseManager);
	SAFE_DELETE(pTeamManager);
}

////////////////////////////////////////////////////
void CD6CoreGlobalEnvironment::D6CoreModuleInitISystem(ISystem *pSystem)
{
	// Set it up
	pSystem = pSystem;
	pSystemGE = pSystem->GetGlobalEnvironment();
	pBaseManager = new CBaseManager;
	pTeamManager = new CTeamManager;
};