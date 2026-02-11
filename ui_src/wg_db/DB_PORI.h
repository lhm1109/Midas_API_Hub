#ifndef __DB_PORI_DB_H__
#define __DB_PORI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_POLC;

class CDB_PORI
{
public:
	CDB_PORI();
	virtual ~CDB_PORI();
	CDBDoc* m_pDoc;

public:
	T_PORI_K m_nStartNum;
	T_PORI_K m_nLastNum;

public:
	void Add(T_PORI_K Key,T_PORI_D& rData,CDB_POLC* pPolc);
	BOOL Del(T_PORI_K Key,CDB_POLC* pPolc);

public:
	BOOL Get(T_PORI_K Key,T_PORI_D& rData);
		//{return m_pori.Lookup(Key,rData);}
	int GetCount();
		//{return m_pori.GetCount();}
	POSITION GetStart();
		//{return m_pori.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PORI_K& rKey,T_PORI_D& rData);
		//{m_pori.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PORI_K,T_PORI_K,T_PORI_D,T_PORI_D&>m_pori;
};

#endif