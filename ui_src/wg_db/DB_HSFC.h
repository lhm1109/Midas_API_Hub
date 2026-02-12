#ifndef __DB_HSFC_DB_H__
#define __DB_HSFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_HSFC
{
public:
	CDB_HSFC();
	virtual ~CDB_HSFC();
	CDBDoc* m_pDoc;

public:
	T_HSFC_K m_nStartNum;
	T_HSFC_K m_nLastNum;

public:
	void Add(T_HSFC_K Key,T_HSFC_D& rData);
	BOOL Del(T_HSFC_K Key);

public:
	BOOL Get(T_HSFC_K Key,T_HSFC_D& rData);
		//{return m_hsfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_hsfc.GetCount();}
	POSITION GetStart();
		//{return m_hsfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HSFC_K& rKey,T_HSFC_D& rData);
		//{m_hsfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HSFC_K,T_HSFC_K,T_HSFC_D,T_HSFC_D&>m_hsfc;
};

#endif
