#ifndef __DB_RLFC_DB_H__
#define __DB_RLFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RLFC
{
public:
	CDB_RLFC();
	virtual ~CDB_RLFC();
	CDBDoc* m_pDoc;

public:
	T_RLFC_K m_nStartNum;
	T_RLFC_K m_nLastNum;

public:
	void Add(T_RLFC_K Key,T_RLFC_D& rData);
	BOOL Del(T_RLFC_K Key);

public:
	BOOL Get(T_RLFC_K Key,T_RLFC_D& rData);
		//{return m_RLFC.Lookup(Key,rData);}
	int GetCount();
		//{return m_RLFC.GetCount();}
	POSITION GetStart();
		//{return m_RLFC.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RLFC_K& rKey,T_RLFC_D& rData);
		//{m_RLFC.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RLFC_K,T_RLFC_K,T_RLFC_D,T_RLFC_D&>m_RLFC;
};

#endif
