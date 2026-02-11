#ifndef __DB_CPCV_DB_H__
#define __DB_CPCV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CPCV
{
public:
	CDB_CPCV();
	virtual ~CDB_CPCV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPCV_K Key,T_CPCV_D& rData);
	BOOL Del(T_CPCV_K Key);

public:
	BOOL Get(T_CPCV_K Key,T_CPCV_D& rData);
		//{return m_cpcv.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpcv.GetCount();}
	POSITION GetStart();
		//{return m_cpcv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPCV_K& rKey,T_CPCV_D& rData);
		//{m_cpcv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPCV_K,T_CPCV_K,T_CPCV_D,T_CPCV_D&>m_cpcv;
};

#endif

