#ifndef __DB_PORG_DB_H__
#define __DB_PORG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_POLC;

class CDB_PORG
{
public:
	CDB_PORG();
	virtual ~CDB_PORG();
	CDBDoc* m_pDoc;

public:
	T_PORG_K m_nStartNum;
	T_PORG_K m_nLastNum;

public:
	void Add(T_PORG_K Key,T_PORG_D& rData,CDB_POLC* pPolc);
	BOOL Del(T_PORG_K Key,CDB_POLC* pPolc);

public:
	BOOL Get(T_PORG_K Key,T_PORG_D& rData);
		//{return m_porg.Lookup(Key,rData);}
	int GetCount();
		//{return m_porg.GetCount();}
	POSITION GetStart();
		//{return m_porg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PORG_K& rKey,T_PORG_D& rData);
		//{m_porg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PORG_K,T_PORG_K,T_PORG_D,T_PORG_D&>m_porg;
};

#endif