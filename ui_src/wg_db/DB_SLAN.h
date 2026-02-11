#ifndef __DB_SLAN_DB_H__
#define __DB_SLAN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_SLAN
{
public:
	CDB_SLAN();
	virtual ~CDB_SLAN();
	CDBDoc* m_pDoc;

public:
	T_SLAN_K m_nStartNum;
	T_SLAN_K m_nLastNum;

public:
	void Add(T_SLAN_K Key,T_SLAN_D& rData,CDB_NODE* pNode);
	BOOL Del(T_SLAN_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_SLAN_K Key,T_SLAN_D& rData);
		//{return m_slan.Lookup(Key,rData);}
	int GetCount();
		//{return m_slan.GetCount();}
	POSITION GetStart();
		//{return m_slan.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLAN_K& rKey,T_SLAN_D& rData);
		//{m_slan.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SLAN_K,T_SLAN_K,T_SLAN_D,T_SLAN_D&>m_slan;
};

#endif

