#ifndef __DB_CGTV_DB_H__
#define __DB_CGTV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CGTV
{
public:
	CDB_CGTV();
	virtual ~CDB_CGTV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CGTV_K Key, T_CGTV_D& rData);
	BOOL Del(T_CGTV_K Key);

public:
	BOOL Get(T_CGTV_K Key, T_CGTV_D& rData);
	//{return m_cgtv.Lookup(Key,rData);}
	int GetCount();
	//{return m_cgtv.GetCount();}
	POSITION GetStart();
	//{return m_cgtv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_CGTV_K& rKey, T_CGTV_D& rData);
	//{m_cgtv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGTV_K, T_CGTV_K, T_CGTV_D, T_CGTV_D&>m_cgtv;
};

#endif

