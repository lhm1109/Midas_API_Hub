#ifndef __DB_EGLD_DB_H__
#define __DB_EGLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_EGLD
{
public:
	CDB_EGLD();
	virtual ~CDB_EGLD();
	CDBDoc* m_pDoc;
public:
	T_EGLD_K m_nStartNum;
	T_EGLD_K m_nLastNum;

public:
	void Add(T_EGLD_K Key,T_EGLD_D& rData,CDB_STLD* pStld);
	BOOL Del(T_EGLD_K Key,CDB_STLD* pStld);

public:
	BOOL Get(T_EGLD_K Key,T_EGLD_D& rData);
		//{return m_egld.Lookup(Key,rData);}
	int GetCount();
		//{return m_egld.GetCount();}
	POSITION GetStart();
		//{return m_egld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EGLD_K& rKey,T_EGLD_D& rData);
		//{m_egld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_EGLD_K,T_EGLD_K,T_EGLD_D,T_EGLD_D&>m_egld;
};

#endif