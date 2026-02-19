// CNotifyManager.cpp: implementation of the CNotifyManager class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_dgn.h"
#include "CNotifyManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
I_NotifyCommand::I_NotifyCommand()
{
	m_bIsMakeClone = FALSE;
	m_Command = GetRegisteredCommandID();
	m_Hint    = 0;
	m_Param1  = 0;
	m_Param2  = 0;
}

UINT I_NotifyCommand::GetRegisteredCommandID()
{  
	if(! m_bIsMakeClone)
	{
		static UINT _CurCommandID = 0;
		_CurCommandID++;
		return _CurCommandID;
	}
	
	return m_Command;
}



CNotifyCmd_Rebar* CNotifyCmd_Rebar::Instance()
{
	static CNotifyCmd_Rebar theNotifyRebar;
	return &theNotifyRebar;
}

CNotifyCmd_Rebar* CNotifyCmd_Rebar::MakeInstance()
{
	Instance()->SetMakeClone();
		CNotifyCmd_Rebar* pCmd = new CNotifyCmd_Rebar();      
	Instance()->ResetMakeClone();
	return pCmd;
}

CNotifyCmd_RebarTable* CNotifyCmd_RebarTable::Instance()
{
	static CNotifyCmd_RebarTable theNotifyRebar;
	return &theNotifyRebar;
}

CNotifyCmd_RebarTable* CNotifyCmd_RebarTable::MakeInstance()
{
	Instance()->SetMakeClone();
		CNotifyCmd_RebarTable* pCmd = new CNotifyCmd_RebarTable();      
	Instance()->ResetMakeClone();
	return pCmd;
}

CNotifyManager::CNotifyManager()
{

}

CNotifyManager::~CNotifyManager()
{

}

CNotifyManager* CNotifyManager::Instance()
{
	static CNotifyManager  theNotifyMgr;
	return &theNotifyMgr;
}

void CNotifyManager::PropagateNotify(I_NotifyCommand* pNotifyCmd)
{
	int nReceiver = m_arNotifyReceivers.GetSize();
	for(int i = 0; i < nReceiver ; i++)
	{
		 m_arNotifyReceivers[i]->OnNotifyCommand(pNotifyCmd);            
	}
}

void CNotifyManager::RemoveNotifyBuffer()
{
	int nNotifies = m_arCurNotifies.GetSize();
	for(int i = 0; i < nNotifies ; i++)
	{
		delete m_arCurNotifies[i];
	}

	m_arCurNotifies.RemoveAll();  
}

void CNotifyManager::On_BeginCommand()
{
	RemoveNotifyBuffer();
}

void CNotifyManager::AddNotify(I_NotifyCommand* pNotifyCmd)
{
	int nNotifies = m_arCurNotifies.GetSize();
	for(int i = 0; i < nNotifies ; i++)
	{
		delete m_arCurNotifies[i];
	}
}

void CNotifyManager::On_EndCommand()
{
	int nNotifies = m_arCurNotifies.GetSize();
	for(int i = 0; i < nNotifies ; i++)
	{
		PropagateNotify(m_arCurNotifies[i]);
	}
}
