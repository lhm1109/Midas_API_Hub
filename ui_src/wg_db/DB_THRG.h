#ifndef __DB_THRG_DB_H__
#define __DB_THRG_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THRG
{
public:
	CDB_THRG();
	virtual ~CDB_THRG();
	CDBDoc* m_pDoc;

public:
	T_THRG_K m_nStartNum;
	T_THRG_K m_nLastNum;

public:
	void Add(T_THRG_K Key,T_THRG_D& rData,CDB_THIS* pThis);
	BOOL Del(T_THRG_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_THRG_K Key,T_THRG_D& rData);
		//{return m_thrg.Lookup(Key,rData);}
	int GetCount();
		//{return m_thrg.GetCount();}
	POSITION GetStart();
		//{return m_thrg.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THRG_K& rKey,T_THRG_D& rData);
		//{m_thrg.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THRG_K,T_THRG_K,T_THRG_D,T_THRG_D&>m_thrg;
};

#endif