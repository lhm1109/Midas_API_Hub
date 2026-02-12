#ifndef __DB_POSV_DB_H__
#define __DB_POSV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_POSV
{
public:
	CDB_POSV();
	virtual ~CDB_POSV();
	CDBDoc* m_pDoc;

public:
	void Add(T_POSV_K Key, T_POSV_D& rData);
	BOOL Del(T_POSV_K Key);

public:
	BOOL Get(T_POSV_K Key, T_POSV_D& rData);
	//{return m_posv.Lookup(Key,rData);}
	int GetCount();
	//{return m_posv.GetCount();}
	POSITION GetStart();
	//{return m_posv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_POSV_K& rKey, T_POSV_D& rData);
	//{m_posv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_POSV_K, T_POSV_K, T_POSV_D, T_POSV_D&>m_posv;
};

#endif

