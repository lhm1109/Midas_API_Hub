#ifndef __DB_RKPV_DB_H__
#define __DB_RKPV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RKPV
{
public:
	CDB_RKPV();
	virtual ~CDB_RKPV();
	CDBDoc* m_pDoc;

public:
	void Add(T_RKPV_K Key,T_RKPV_D& rData);
	BOOL Del(T_RKPV_K Key);

public:
	BOOL Get(T_RKPV_K Key,T_RKPV_D& rData);
		//{return m_rkpv.Lookup(Key,rData);}
	int GetCount();
		//{return m_rkpv.GetCount();}
	POSITION GetStart();
		//{return m_rkpv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RKPV_K& rKey,T_RKPV_D& rData);
		//{m_rkpv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RKPV_K,T_RKPV_K,T_RKPV_D,T_RKPV_D&>m_rkpv;
};

#endif

