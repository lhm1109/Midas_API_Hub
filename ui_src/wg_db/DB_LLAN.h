#ifndef __DB_LLAN_DB_H__
#define __DB_LLAN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LLAN
{
public:
	CDB_LLAN();
	virtual ~CDB_LLAN();
	CDBDoc* m_pDoc;

public:
	T_LLAN_K m_nStartNum;
	T_LLAN_K m_nLastNum;

public:
	void Add(T_LLAN_K Key,T_LLAN_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_LLAN_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_LLAN_K Key,T_LLAN_D& rData);
		//{return m_llan.Lookup(Key,rData);}
	int GetCount();
		//{return m_llan.GetCount();}
	POSITION GetStart();
		//{return m_llan.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LLAN_K& rKey,T_LLAN_D& rData);
		//{m_llan.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LLAN_K,T_LLAN_K,T_LLAN_D,T_LLAN_D&>m_llan;
};

#endif

