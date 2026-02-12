#ifndef __DB_PNLA_DB_H__
#define __DB_PNLA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_STLD;
class CDB_LDGR;
class CDB_GRUP;

class CDB_PNLA
{
public:
	CDB_PNLA();
	virtual ~CDB_PNLA();
	CDBDoc* m_pDoc;

public:
	T_MATL_K m_nLastNum;
	T_MATL_K m_nStartNum;

public:
	void Add(T_PNLA_K Key,T_PNLA_D& rData,CDB_NODE* pNode,CDB_STLD* pStld,CDB_LDGR* pLdgr,CDB_GRUP* pGrup);
	BOOL Del(T_PNLA_K Key,CDB_NODE* pNode,CDB_STLD* pStld,CDB_LDGR* pLdgr,CDB_GRUP* pGrup);

public:
	BOOL Get(T_PNLA_K Key,T_PNLA_D& rData);
		//{return m_pnla.Lookup(Key,rData);}
	int GetCount();
		//{return m_pnla.GetCount();}
	POSITION GetStart();
		//{return m_pnla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PNLA_K& rKey,T_PNLA_D& rData);
		//{m_pnla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PNLA_K,T_PNLA_K,T_PNLA_D,T_PNLA_D&>m_pnla;
};

#endif