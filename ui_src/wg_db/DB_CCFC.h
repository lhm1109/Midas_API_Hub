#ifndef __DB_CCFC_DB_H__
#define __DB_CCFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CCFC
{
public:
	CDB_CCFC();
	virtual ~CDB_CCFC();
	CDBDoc* m_pDoc;

public:
	T_CCFC_K m_nStartNum;
	T_CCFC_K m_nLastNum;

public:
	void Add(T_CCFC_K Key,T_CCFC_D& rData);
	BOOL Del(T_CCFC_K Key);

public:
	BOOL Get(T_CCFC_K Key,T_CCFC_D& rData);
		//{return m_ccfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_ccfc.GetCount();}
	POSITION GetStart();
		//{return m_ccfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CCFC_K& rKey,T_CCFC_D& rData);
		//{m_ccfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CCFC_K,T_CCFC_K,T_CCFC_D,T_CCFC_D&>m_ccfc;
};

#endif
