#ifndef __DB_GUST_DB_H__
#define __DB_GUST_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STOR;
class CDB_GUST
{
public:
	CDB_GUST();
	virtual ~CDB_GUST();
	CDBDoc* m_pDoc;

public:
	T_GUST_K m_nStartNum;
	T_GUST_K m_nLastNum;

public:
	void Add(T_GUST_K Key, T_GUST_D& rData, CDB_STOR* pStor);
	BOOL Del(T_GUST_K Key, CDB_STOR* pStor);

public:
	BOOL Get(T_GUST_K Key, T_GUST_D& rData);
	//{return m_gust.Lookup(Key,rData);}
	int GetCount();
	//{return m_gust.GetCount();}
	POSITION GetStart();
	//{return m_gust.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUST_K& rKey, T_GUST_D& rData);
	//{m_gust.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUST_K,T_GUST_K,T_GUST_D,T_GUST_D&>m_gust;
};

#endif