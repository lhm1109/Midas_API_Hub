#ifndef __DB_CFTR_DB_H__
#define __DB_CFTR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_GRUP;

class CDB_CFTR
{
public:
	CDB_CFTR();
	virtual ~CDB_CFTR();
	CDBDoc* m_pDoc;

public:
	T_CFTR_K m_nStartNum;
	T_CFTR_K m_nLastNum;

public:
	void Add(T_CFTR_K Key,T_CFTR_D& rData,CDB_GRUP* pGrup);
	BOOL Del(T_CFTR_K Key,CDB_GRUP* pGrup);

public:
	BOOL Get(T_CFTR_K Key,T_CFTR_D& rData);
		//{return m_cftr.Lookup(Key,rData);}
	int GetCount();
		//{return m_cftr.GetCount();}
	POSITION GetStart();
		//{return m_cftr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CFTR_K& rKey,T_CFTR_D& rData);
		//{m_cftr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CFTR_K,T_CFTR_K,T_CFTR_D,T_CFTR_D&>m_cftr;
};

#endif