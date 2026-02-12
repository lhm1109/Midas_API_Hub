#ifndef __DB_CPSV_DB_H__
#define __DB_CPSV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CPSV
{
public:
	CDB_CPSV();
	virtual ~CDB_CPSV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPSV_K Key,T_CPSV_D& rData);
	BOOL Del(T_CPSV_K Key);

public:
	BOOL Get(T_CPSV_K Key,T_CPSV_D& rData);
		//{return m_cpsv.Lookup(Key,rData);}
	int GetCount();
		//{return m_cpsv.GetCount();}
	POSITION GetStart();
		//{return m_cpsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CPSV_K& rKey,T_CPSV_D& rData);
		//{m_cpsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPSV_K,T_CPSV_K,T_CPSV_D,T_CPSV_D&>m_cpsv;
};

#endif

