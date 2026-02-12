#ifndef __DB_MBTV_DB_H__
#define __DB_MBTV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MBTV
{
public:
	CDB_MBTV();
	virtual ~CDB_MBTV();
	CDBDoc* m_pDoc;

public:
	void Add(T_MBTV_K Key,T_MBTV_D& rData);
	BOOL Del(T_MBTV_K Key);

public:
	BOOL Get(T_MBTV_K Key,T_MBTV_D& rData);
		//{return m_mbtv.Lookup(Key,rData);}
	int GetCount();
		//{return m_mbtv.GetCount();}
	POSITION GetStart();
		//{return m_mbtv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MBTV_K& rKey,T_MBTV_D& rData);
		//{m_mbtv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MBTV_K,T_MBTV_K,T_MBTV_D,T_MBTV_D&>m_mbtv;
};

#endif

