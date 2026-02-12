#ifndef __DB_STAG_DB_H__
#define __DB_STAG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GRUP;
class CDB_BNGR;
class CDB_LDGR;

class CDB_STAG
{
public:
	CDB_STAG();
	virtual ~CDB_STAG();

public:
	T_STAG_K m_nStartNum;
	T_STAG_K m_nLastNum;
	T_STAG_K m_nLastStag;

public:
	void Add(T_STAG_K Key,T_STAG_D& rData,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr);
	BOOL Del(T_STAG_K Key,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr);
	void AddItem(T_STAG_ITEM_UDRD_D& rItem,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr);
	BOOL DelItem(T_STAG_ITEM_UDRD_D& rItem,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr);

public:
	BOOL Get(T_STAG_K Key,T_STAG_D& rData)
		{return m_stag.Lookup(Key,rData);}
	int GetCount()
		{return m_stag.GetCount();}
	POSITION GetStart()
		{return m_stag.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STAG_K& rKey,T_STAG_D& rData)
		{m_stag.GetNextAssoc(rNextPosition,rKey,rData);}

	T_STAG_K GetLastStag() { return m_nLastStag; }

protected:
	CMap<T_STAG_K,T_STAG_K,T_STAG_D,T_STAG_D&>m_stag;
};

#endif