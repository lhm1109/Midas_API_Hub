#ifndef __DB_PTNS_DB_H__
#define __DB_PTNS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;

class CDB_PTNS
{
public:
	CDB_PTNS();
	virtual ~CDB_PTNS();
	CDBDoc* m_pDoc;

public:
	void Add(T_PTNS_K Key,T_PTNS_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_PTNS_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_PTNS_K Key,T_PTNS_D& rData)
		{return m_ptns.Lookup(Key.keymap,rData);}

	BOOL Get(T_PTNS_K Key,T_PTNS_D& rData);
		//{return m_ptns.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_ptns.GetCount();}
	POSITION GetStart();
		//{return m_ptns.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PTNS_K& rKey,T_PTNS_D& rData);
		//{m_ptns.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_PTNS_KEY,T_PTNS_KEY,T_PTNS_D,T_PTNS_D&>m_ptns;
};

#endif