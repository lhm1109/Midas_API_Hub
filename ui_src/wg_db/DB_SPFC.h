#ifndef __DB_SPFC_DB_H__
#define __DB_SPFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SPFC
{
public:
	CDB_SPFC();
	virtual ~CDB_SPFC();
	CDBDoc* m_pDoc;

public:
	T_SPFC_K m_nStartNum;
	T_SPFC_K m_nLastNum;

public:
	void Add(T_SPFC_K Key,T_SPFC_D& rData);
	BOOL Del(T_SPFC_K Key);
	void Initialize();

public:
	BOOL Get(T_SPFC_K Key,T_SPFC_D& rData);
		//{return m_spfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_spfc.GetCount();}
	POSITION GetStart();
		//{return m_spfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPFC_K& rKey,T_SPFC_D& rData);
		//{m_spfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPFC_K,T_SPFC_K,T_SPFC_D,T_SPFC_D&>m_spfc;
};

#endif
