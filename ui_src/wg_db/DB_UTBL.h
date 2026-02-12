#ifndef __DB_UTBL_DB_H__
#define __DB_UTBL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_UTBL
{
public:
	CDB_UTBL();
	virtual ~CDB_UTBL();
	CDBDoc* m_pDoc;

public:
	T_UTBL_K m_nLastNum;
	T_UTBL_K m_nStartNum;

public:
	void Add(T_UTBL_K Key,T_UTBL_D& rData);
	BOOL Del(T_UTBL_K Key);

public:
	BOOL Get(T_UTBL_K Key,T_UTBL_D& rData)
		{return m_utbl.Lookup(Key,rData);}
	int GetCount()
		{return m_utbl.GetCount();}
	POSITION GetStart()
		{return m_utbl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UTBL_K& rKey,T_UTBL_D& rData)
		{m_utbl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UTBL_K,T_UTBL_K,T_UTBL_D,T_UTBL_D&>m_utbl;
};

#endif
