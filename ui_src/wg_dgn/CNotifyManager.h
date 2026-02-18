// CNotifyManager.h: interface for the CNotifyManager class.
// (060123) sshan Create
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CNOTIFYMANAGER_H__912B65AE_EEC1_45C0_A71A_33431119E982__INCLUDED_)
#define AFX_CNOTIFYMANAGER_H__912B65AE_EEC1_45C0_A71A_33431119E982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class I_NotifyCommand
{
protected:
	UINT  m_Command ;
	UINT  m_Hint    ;
	UINT  m_Param1  ;
	UINT  m_Param2  ;
	BOOL  m_bIsMakeClone;
 
	I_NotifyCommand();
public:
	UINT GetRegisteredCommandID();

	~I_NotifyCommand()
	{
			
	}

	virtual UINT  GetCommand()
	{
		return m_Command;
	}

	virtual UINT  GetHint   ()
	{
		return m_Hint;
	}

	virtual UINT GetParam1 ()
	{
		return m_Param1;
	}
	
	virtual UINT GetParam2 ()
	{
		return m_Param2;
	}
	
	virtual void SetHint   (UINT nHint  )
	{ 
		m_Hint    = nHint;
	}

	virtual void SetParam1 (UINT Param1 )
	{
		m_Param1 = Param1;
	}
	
	virtual void SetParam2 (UINT Param2 )
	{
		m_Param2 = Param2;
	}

	void SetMakeClone()
	{
		m_bIsMakeClone = TRUE ;
	}

	void ResetMakeClone()
	{
		m_bIsMakeClone = FALSE;
	}
};

//----------------------------------------------------------------
// Command 의 종류가 많아지면 별도 파일로 옮길것.
class CNotifyCmd_Rebar : public I_NotifyCommand
{
public:
	//--------------------------------------------------------------
	// Hint는 계속해서 추가 될 수 있다. 
	// 작업시 Sensitive Help에서 참조 가능하도록 
	// 변수로 설정한다. 
	UINT CMD_HINT_ADD;
	UINT CMD_HINT_DEL;
protected:
	CNotifyCmd_Rebar()
	{
		CMD_HINT_ADD = 1;
		CMD_HINT_DEL = 2;
	}
public:
	
	~CNotifyCmd_Rebar()
	{
		
	}

	static CNotifyCmd_Rebar* Instance();

	static CNotifyCmd_Rebar* MakeInstance();

};

class CNotifyCmd_RebarTable : public I_NotifyCommand
{
public:
	//--------------------------------------------------------------
	// Hint는 계속해서 추가 될 수 있다. 
	// 작업시 Sensitive Help에서 참조 가능하도록 
	// 변수로 설정한다. 
	CMapStringToString m_mapSelectedRebarName;

	UINT CMD_HINT_SELCHANGED;
	UINT CMD_HINT_UPDATETABLE;
protected:
	CNotifyCmd_RebarTable()
	{
		CMD_HINT_SELCHANGED  = 1;
		CMD_HINT_UPDATETABLE = 2;
	}
public:
	
	~CNotifyCmd_RebarTable()
	{
		
	}

	static CNotifyCmd_RebarTable* Instance();

	static CNotifyCmd_RebarTable* MakeInstance();

	void ResetSelection()
	{
		m_mapSelectedRebarName.RemoveAll();
	}
};

class I_NotifyReceiver;
class CNotifyManager  
{
friend class I_NotifyReceiver;
protected:
	CArray<I_NotifyCommand * , I_NotifyCommand *> m_arCurNotifies    ;

	CArray<I_NotifyReceiver* , I_NotifyReceiver*> m_arNotifyReceivers;

	CNotifyManager();

	void RegisterNotifyReceiver(I_NotifyReceiver* pReceiver)
	{
		m_arNotifyReceivers.Add(pReceiver);
	}
	void UnRegisterNotifyReceiver(I_NotifyReceiver* pReceiver)
	{
		int nSize = m_arNotifyReceivers.GetSize();
		for( int i = 0; i < nSize ; i++)
		{
			if(m_arNotifyReceivers[i] == pReceiver)
			{
				m_arNotifyReceivers.RemoveAt(i);
				break;
			} 
		}
	}
public:
	virtual ~CNotifyManager();
	static CNotifyManager* Instance();
	void PropagateNotify(I_NotifyCommand* pNotifyCmd);
	
	void On_BeginCommand();
	void AddNotify(I_NotifyCommand* pNotifyCmd);
	void On_EndCommand();
	void RemoveNotifyBuffer();
};

class I_NotifyReceiver
{
protected:
	
public:
	I_NotifyReceiver()
	{
		CNotifyManager::Instance()->RegisterNotifyReceiver(this);
	}

	~I_NotifyReceiver()
	{
		CNotifyManager::Instance()->UnRegisterNotifyReceiver(this);
	}

	virtual void OnNotifyCommand(I_NotifyCommand* pNotifyCmd) = 0;

};


#endif // !defined(AFX_CNOTIFYMANAGER_H__912B65AE_EEC1_45C0_A71A_33431119E982__INCLUDED_)
