#ifndef __DB_MLFC_DB_H__
#define __DB_MLFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MLFC
{
public:
	CDB_MLFC();
	virtual ~CDB_MLFC();
	CDBDoc* m_pDoc;

public:
	T_MLFC_K m_nStartNum;
	T_MLFC_K m_nLastNum;

public:
	void Add(T_MLFC_K Key,T_MLFC_D& rData);
	BOOL Del(T_MLFC_K Key);

public:
	BOOL Get(T_MLFC_K Key,T_MLFC_D& rData);
		//{return m_mlfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_mlfc.GetCount();}
	POSITION GetStart();
		//{return m_mlfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MLFC_K& rKey,T_MLFC_D& rData);
		//{m_mlfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MLFC_K,T_MLFC_K,T_MLFC_D,T_MLFC_D&>m_mlfc;
};

#endif
