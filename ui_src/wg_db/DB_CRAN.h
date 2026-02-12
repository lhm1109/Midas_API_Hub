#ifndef __DB_CRAN_DB_H__
#define __DB_CRAN_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_CRAN
{
public:
	CDB_CRAN();
	virtual ~CDB_CRAN();
	CDBDoc* m_pDoc;

public:
	T_CRAN_K m_nStartNum;
	T_CRAN_K m_nLastNum;

public:
	void Add(T_CRAN_K Key,T_CRAN_D& rData,CDB_NODE* pNode);
	BOOL Del(T_CRAN_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_CRAN_K Key,T_CRAN_D& rData);
		//{return m_cran.Lookup(Key,rData);}
	int GetCount();
		//{return m_cran.GetCount();}
	POSITION GetStart();
		//{return m_cran.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CRAN_K& rKey,T_CRAN_D& rData);
		//{m_cran.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CRAN_K,T_CRAN_K,T_CRAN_D,T_CRAN_D&>m_cran;
};

#endif

