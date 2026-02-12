#ifndef __DB_PORE_DB_H__
#define __DB_PORE_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_POLC;

class CDB_PORE
{
public:
	CDB_PORE();
	virtual ~CDB_PORE();
	CDBDoc* m_pDoc;

public:
	T_PORE_K m_nStartNum;
	T_PORE_K m_nLastNum;

public:
	void Add(T_PORE_K Key,T_PORE_D& rData,CDB_POLC* pPolc);
	BOOL Del(T_PORE_K Key,CDB_POLC* pPolc);

public:
	BOOL Get(T_PORE_K Key,T_PORE_D& rData);
		//{return m_pore.Lookup(Key,rData);}
	int GetCount();
		//{return m_pore.GetCount();}
	POSITION GetStart();
		//{return m_pore.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PORE_K& rKey,T_PORE_D& rData);
		//{m_pore.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PORE_K,T_PORE_K,T_PORE_D,T_PORE_D&>m_pore;
};

#endif