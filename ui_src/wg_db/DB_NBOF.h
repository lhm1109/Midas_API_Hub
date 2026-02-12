#ifndef __DB_NBOF_DB_H__
#define __DB_NBOF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LDGR;

class CDB_NBOF
{
public:
	CDB_NBOF();
	virtual ~CDB_NBOF();
	CDBDoc* m_pDoc;

public:
	T_NBOF_K m_nLastNum;
	T_NBOF_K m_nStartNum;

public:
	void Add(T_NBOF_K Key,T_NBOF_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr);
	BOOL Del(T_NBOF_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr);
	void AddItem(T_NBOF_K Key,T_NODE_K KeyNode);
	BOOL DelItem(T_NBOF_K Key,T_NODE_K KeyNode);

public:
	BOOL Get(T_NBOF_K Key,T_NBOF_D& rData);
		//{return m_nbof.Lookup(Key,rData);}
	int GetCount();
		//{return m_nbof.GetCount();}
	POSITION GetStart();
		//{return m_nbof.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NBOF_K& rKey,T_NBOF_D& rData);
		//{m_nbof.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_NBOF_K,T_NBOF_K,T_NBOF_D,T_NBOF_D&>m_nbof;
};

#endif