#ifndef __DB_CRANU_DB_H__
#define __DB_CRANU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CRANU
{
public:
	CDB_CRANU()
	{
		m_cran.InitHashTable(HASHSIZECRAN);
	}
	virtual ~CDB_CRANU(){};

public:
	void Add(T_UDRD_KEY Key,T_CRAN_UDRD_D& rData)
		{m_cran.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cran.RemoveKey(Key);}
	void DelAll()
		{m_cran.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CRAN_UDRD_D& rData)
		{return m_cran.Lookup(Key,rData);}
	int GetCount()
		{return m_cran.GetCount();}
	POSITION GetStart()
		{return m_cran.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CRAN_UDRD_D& rData)
		{m_cran.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CRAN_UDRD_D,T_CRAN_UDRD_D&>m_cran;
};

#endif