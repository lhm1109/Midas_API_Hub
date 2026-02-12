#ifndef __DB_ETFC_DB_H__
#define __DB_ETFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_ETFC
{
public:
	CDB_ETFC();
	virtual ~CDB_ETFC();
	CDBDoc* m_pDoc;

public:
	T_ETFC_K m_nStartNum;
	T_ETFC_K m_nLastNum;

public:
	void Add(T_ETFC_K Key,T_ETFC_D& rData);
	BOOL Del(T_ETFC_K Key);

public:
	BOOL Get(T_ETFC_K Key,T_ETFC_D& rData);
		//{return m_etfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_etfc.GetCount();}
	POSITION GetStart();
		//{return m_etfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ETFC_K& rKey,T_ETFC_D& rData);
		//{m_etfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ETFC_K,T_ETFC_K,T_ETFC_D,T_ETFC_D&>m_etfc;
};

#endif
