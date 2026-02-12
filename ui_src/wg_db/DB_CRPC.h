#ifndef __DB_CRPC_DB_H__
#define __DB_CRPC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_LDGR;

class CDB_CRPC
{
public:
	CDB_CRPC();
	virtual ~CDB_CRPC();
	CDBDoc* m_pDoc;

public:
	void Add(T_CRPC_K Key,T_CRPC_D& rData,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_CRPC_K Key,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_CRPC_K Key,T_CRPC_D& rData)
		{return m_crpc.Lookup(Key.keymap,rData);}

	BOOL Get(T_CRPC_K Key,T_CRPC_D& rData);
		//{return m_crpc.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_crpc.GetCount();}
	POSITION GetStart();
		//{return m_crpc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CRPC_K& rKey,T_CRPC_D& rData);
		//{m_crpc.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_CRPC_KEY,T_CRPC_KEY,T_CRPC_D,T_CRPC_D&>m_crpc;
};

#endif