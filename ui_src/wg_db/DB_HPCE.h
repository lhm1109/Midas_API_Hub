#ifndef __DB_HPCE_DB_H__
#define __DB_HPCE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_HPCE
{
public:
	CDB_HPCE();
	virtual ~CDB_HPCE();
	CDBDoc* m_pDoc;

public:
	T_HPCE_K m_nStartNum;
	T_HPCE_K m_nLastNum;

public:
	void Add(T_HPCE_K Key,T_HPCE_D& rData,CDB_NODE* pNode);
	BOOL Del(T_HPCE_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_HPCE_K Key,T_HPCE_D& rData);
		//{return m_hpce.Lookup(Key,rData);}
	int GetCount();
		//{return m_hpce.GetCount();}
	POSITION GetStart();
		//{return m_hpce.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HPCE_K& rKey,T_HPCE_D& rData);
		//{m_hpce.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HPCE_K,T_HPCE_K,T_HPCE_D,T_HPCE_D&>m_hpce;
};

#endif

