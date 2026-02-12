#ifndef __DB_REBC_DB_H__
#define __DB_REBC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_REBC
{
public:
	CDB_REBC();
	CDB_REBC(CDB_REBC& src);
	virtual ~CDB_REBC();
	CDB_REBC& operator=(CDB_REBC& src);
	CDBDoc* m_pDoc;

public:
	void Add(T_REBC_K Key,T_REBC_D& rData,CDB_SECT* pSect);
	BOOL Del(T_REBC_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_REBC_K Key,T_REBC_D& rData);
		//{return m_rebc.Lookup(Key,rData);}
	int GetCount();
		//{return m_rebc.GetCount();}
	POSITION GetStart();
		//{return m_rebc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_REBC_K& rKey,T_REBC_D& rData);
		//{m_rebc.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_REBC_KEY Key);
	BOOL DelList(T_REBC_KEY Key);
	void DelAllList();
	BOOL GetList(T_REBC_K Key,RebcList*& rpList){return m_rebclist.Lookup(Key.keymap,rpList);}
	int GetCountList(){return m_rebclist.GetCount();}
	void AddListItem(T_REBC_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_REBC_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_REBC_KEY,T_REBC_KEY,T_REBC_D,T_REBC_D&>m_rebc;
	CMap<T_REBC_KEY,T_REBC_KEY,RebcList*,RebcList*> m_rebclist;
};

#endif

