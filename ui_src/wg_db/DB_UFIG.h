#ifndef __DB_UFIG_DB_H__
#define __DB_UFIG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
//class CDB_GRUP;

class CDB_UFIG
{
public:
	CDB_UFIG();
	virtual ~CDB_UFIG();
	CDBDoc* m_pDoc;

public:
	T_UFIG_K m_nLastNum;
	T_UFIG_K m_nStartNum;

public:
	void Add(T_UFIG_K Key,T_UFIG_D& rData);
	BOOL Del(T_UFIG_K Key);
//   void Add(T_UFIG_K Key,T_UFIG_D& rData,CDB_GRUP* pGrup);
//   BOOL Del(T_UFIG_K Key,CDB_GRUP* pGrup);

public:
	BOOL Get(T_UFIG_K Key,T_UFIG_D& rData)
		{return m_ufig.Lookup(Key,rData);}
	int GetCount()
		{return m_ufig.GetCount();}
	POSITION GetStart()
		{return m_ufig.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UFIG_K& rKey,T_UFIG_D& rData)
		{m_ufig.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UFIG_K,T_UFIG_K,T_UFIG_D,T_UFIG_D&>m_ufig;
};

#endif
