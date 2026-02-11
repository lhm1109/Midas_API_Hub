#ifndef __DB_GUMT_DB_H__
#define __DB_GUMT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_MATL;
class CDB_GUMT
{
public:
	CDB_GUMT();
	virtual ~CDB_GUMT();
	CDBDoc* m_pDoc;

public:
	T_GUMT_K m_nStartNum;
	T_GUMT_K m_nLastNum;

public:
	void Add(T_GUMT_K Key, T_GUMT_D& rData, CDB_MATL* pMatl);
	BOOL Del(T_GUMT_K Key, CDB_MATL* pMatl);

public:
	BOOL Get(T_GUMT_K Key, T_GUMT_D& rData);
	//{return m_gumt.Lookup(Key,rData);}
	int GetCount();
	//{return m_gumt.GetCount();}
	POSITION GetStart();
	//{return m_gumt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUMT_K& rKey, T_GUMT_D& rData);
	//{m_gumt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUMT_K,T_GUMT_K,T_GUMT_D,T_GUMT_D&>m_gumt;
};

#endif