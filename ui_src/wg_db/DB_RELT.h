#ifndef __DB_RELT_DB_H__
#define __DB_RELT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_BNGR;

class CDB_RELT
{
public:
	CDB_RELT();
	virtual ~CDB_RELT();
	CDBDoc* m_pDoc;

public:
	T_RELT_K m_nStartNum;
	T_RELT_K m_nLastNum;

public:
	void Add(T_RELT_K Key,T_RELT_D& rData, CDB_BNGR* pBngr);
	BOOL Del(T_RELT_K Key, CDB_BNGR* pBngr);

public:
	BOOL Get(T_RELT_K Key,T_RELT_D& rData);
		//{return m_RELT.Lookup(Key,rData);}
	int GetCount();
		//{return m_RELT.GetCount();}
	POSITION GetStart();
		//{return m_RELT.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RELT_K& rKey,T_RELT_D& rData);
		//{m_RELT.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RELT_K,T_RELT_K,T_RELT_D,T_RELT_D&>m_RELT;
};

#endif
