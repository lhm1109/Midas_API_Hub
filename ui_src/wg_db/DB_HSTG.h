#ifndef __DB_HSTG_DB_H__
#define __DB_HSTG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GRUP;
class CDB_BNGR;
class CDB_LDGR;

class CDB_HSTG
{
public:
	CDB_HSTG();
	virtual ~CDB_HSTG();

public:
	T_HSTG_K m_nStartNum;
	T_HSTG_K m_nLastNum;
	T_HSTG_K m_nLastStag;

public:
	void Add(T_HSTG_K Key,T_HSTG_D& rData,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr);
	BOOL Del(T_HSTG_K Key,CDB_GRUP* pGrup,CDB_BNGR* pBngr,CDB_LDGR* pLdgr);

public:
	BOOL Get(T_HSTG_K Key,T_HSTG_D& rData)
		{return m_hstg.Lookup(Key,rData);}
	int GetCount()
		{return m_hstg.GetCount();}
	POSITION GetStart()
		{return m_hstg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HSTG_K& rKey,T_HSTG_D& rData)
		{m_hstg.GetNextAssoc(rNextPosition,rKey,rData);}

	T_HSTG_K GetLastHstg() { return m_nLastStag; }

protected:
	CMap<T_HSTG_K,T_HSTG_K,T_HSTG_D,T_HSTG_D&>m_hstg;
};

#endif