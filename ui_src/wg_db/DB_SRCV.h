#ifndef __DB_SRCV_DB_H__
#define __DB_SRCV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SRCV
{
public:
	CDB_SRCV();
	virtual ~CDB_SRCV();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRCV_K Key, T_SRCV_D& rData);
	BOOL Del(T_SRCV_K Key);

public:
	BOOL Get(T_SRCV_K Key, T_SRCV_D& rData);
	//{return m_srcv.Lookup(Key,rData);}
	int GetCount();
	//{return m_srcv.GetCount();}
	POSITION GetStart();
	//{return m_srcv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRCV_K& rKey, T_SRCV_D& rData);
	//{m_srcv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRCV_K, T_SRCV_K, T_SRCV_D, T_SRCV_D&>m_srcv;
};

#endif