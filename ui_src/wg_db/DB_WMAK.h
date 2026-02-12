#ifndef __DB_WMAK_DB_H__
#define __DB_WMAK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_WMAK
{
public:
	CDB_WMAK();
	virtual ~CDB_WMAK();
	CDBDoc* m_pDoc;

public:
	T_WMAK_K m_nStartNum;
	T_WMAK_K m_nLastNum;

public:
	void Add(T_WMAK_K Key,T_WMAK_D& rData);
	BOOL Del(T_WMAK_K Key);

public:
	BOOL Get(T_WMAK_K Key,T_WMAK_D& rData);
		//{return m_wmak.Lookup(Key,rData);}
	int GetCount();
		//{return m_wmak.GetCount();}
	POSITION GetStart();
		//{return m_wmak.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WMAK_K& rKey,T_WMAK_D& rData);
		//{m_wmak.GetNextAssoc(rNextPosition,rKey,rData);}
	BOOL GetWmak(int nUserWallId, T_WMAK_K& rKey);
		//{return m_usrw.Lookup(nUserWallId, rKey);}
	BOOL Exist(int nUserWallId);
		//{ 
		//  T_WMAK_K KeyTemp;
		//  return m_usrw.Lookup(nUserWallId, KeyTemp);
		//}

protected:
	CMap<T_WMAK_K,T_WMAK_K,T_WMAK_D,T_WMAK_D&>m_wmak;
	CMap<int, int, T_WMAK_K, T_WMAK_K>m_usrw;
};

#endif
