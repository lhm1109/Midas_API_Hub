#ifndef __DB_POEF_DB_H__
#define __DB_POEF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_POLC;

class CDB_POEF
{
public:
	CDB_POEF();
	virtual ~CDB_POEF();
	CDBDoc* m_pDoc;

public:
	T_POEF_K m_nStartNum;
	T_POEF_K m_nLastNum;

public:
	void Add(T_POEF_K Key,T_POEF_D& rData,CDB_POLC* pPolc);
	BOOL Del(T_POEF_K Key,CDB_POLC* pPolc);

public:
	BOOL Get(T_POEF_K Key,T_POEF_D& rData);
		//{return m_poef.Lookup(Key,rData);}
	int GetCount();
		//{return m_poef.GetCount();}
	POSITION GetStart();
		//{return m_poef.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_POEF_K& rKey,T_POEF_D& rData);
		//{m_poef.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_POEF_K,T_POEF_K,T_POEF_D,T_POEF_D&>m_poef;
};

#endif