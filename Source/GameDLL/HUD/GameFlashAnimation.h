/*************************************************************************
Crytek Source File.
Copyright (C), Crytek Studios, 2001-2005.
-------------------------------------------------------------------------
$Id$
$DateTime$
Description:

-------------------------------------------------------------------------
History:
- 07:11:2005: Created by Julien Darre

*************************************************************************/
#ifndef __GAMEFLASHANIMATION_H__
#define __GAMEFLASHANIMATION_H__

//-----------------------------------------------------------------------------------------------------

#include <list>
#include "FlashAnimation.h"

// Forward declarations
class CGameFlashLogic;

enum EGameFlashDock
{
	eGFD_Stretch	= (1 << 0),
	eGFD_Center		= (1 << 1),
	eGFD_Left			= (1 << 3),
	eGFD_Right		= (1 << 4),
};

enum EFlashAnimFlags
{
	eFAF_ThisHandler	= (1 << 5),
	eFAF_Visible			= (1 << 6),
	eFAF_ManualRender	= (1 << 7),
	eFAF_Default			= (eFAF_ThisHandler|eFAF_Visible)
};

class CGameFlashAnimation : public CFlashAnimation
{
public:
	CGameFlashAnimation();
	virtual ~	CGameFlashAnimation();

	void Init(const char *strFileName, EGameFlashDock docking = eGFD_Center, uint32 flags = eFAF_Default);
	bool Load(const char *strFileName, EGameFlashDock docking = eGFD_Center, uint32 flags = eFAF_Default);
	bool Reload(bool forceUnload=false);
	virtual void Unload();
	void SetDock(uint32 eGFDock);
	uint32 GetDock() const;
	uint32 GetFlags() const;
	void AddVariable(const char *strControl,const char *strVariable,	const char *strToken,float fScale,float fOffset);
	void ReInitVariables();
	void GetMemoryStatistics(ICrySizer * s);

private:
	string	m_fileName;
	uint32	m_dock;
	uint32	m_flags;

	typedef DynArray<CGameFlashLogic *> TGameFlashLogicsList;
	TGameFlashLogicsList m_gameFlashLogicsList;
};

#endif //__GAMEFLASHANIMATION_H__