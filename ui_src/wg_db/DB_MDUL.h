#ifndef __DB_MDUL_DB_H__
#define __DB_MDUL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STOR;

class CDB_MDUL
{
public:
	CDB_MDUL();
	virtual ~CDB_MDUL();
	CDBDoc* m_pDoc;

public:
	T_MDUL_K m_nStartNum;
	T_MDUL_K m_nLastNum;

public:
	void Add(T_MDUL_K Key,T_MDUL_D& rData,CDB_STOR* pStor);
	BOOL Del(T_MDUL_K Key,CDB_STOR* pStor);

public:
	BOOL Get(T_MDUL_K Key,T_MDUL_D& rData);
		//{return m_mdul.Lookup(Key,rData);}
	int GetCount();
		//{return m_mdul.GetCount();}
	POSITION GetStart();
		//{return m_mdul.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MDUL_K& rKey,T_MDUL_D& rData);
		//{m_mdul.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MDUL_K,T_MDUL_K,T_MDUL_D,T_MDUL_D&>m_mdul;
};

#endif

