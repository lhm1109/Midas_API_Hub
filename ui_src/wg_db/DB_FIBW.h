#ifndef __DB_FIBW_DB_H__
#define __DB_FIBW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_FIBW
{
public:
	CDB_FIBW();
	virtual ~CDB_FIBW();
	CDBDoc* m_pDoc;

public:
	T_FIBW_K m_nStartNum;
	T_FIBW_K m_nLastNum;

public:
	void Add(T_FIBW_K Key,T_FIBW_D& rData);
	BOOL Del(T_FIBW_K Key);

public:
	BOOL Get(T_FIBW_K Key,T_FIBW_D& rData);
	T_FIBW_D* GetFibwPair(T_FIBW_K Key);
		//{return m_fibw.Lookup(Key,rData);}
	int GetCount();
		//{return m_fibw.GetCount();}
	POSITION GetStart();
		//{return m_fibw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FIBW_K& rKey,T_FIBW_D& rData);
	
	CMap<unsigned int, unsigned int, T_FIBW_D, T_FIBW_D&>::CPair* PGetFirstAssoc();
	CMap<unsigned int, unsigned int, T_FIBW_D, T_FIBW_D&>::CPair* PGetNext(CMap<T_FIBW_K, T_FIBW_K, T_FIBW_D, T_FIBW_D&>::CPair* pair);
		//{m_fibw.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_FIBW_K,T_FIBW_K,T_FIBW_D,T_FIBW_D&>m_fibw;
};

#endif
