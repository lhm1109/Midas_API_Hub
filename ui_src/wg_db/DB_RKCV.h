#ifndef __DB_RKCV_DB_H__
#define __DB_RKCV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_RKCV
{
public:
	CDB_RKCV();
	virtual ~CDB_RKCV();
	CDBDoc* m_pDoc;

public:
	void Add(T_RKCV_K Key,T_RKCV_D& rData);
	BOOL Del(T_RKCV_K Key);

public:
	BOOL Get(T_RKCV_K Key,T_RKCV_D& rData);
		//{return m_rkcv.Lookup(Key,rData);}
	int GetCount();
		//{return m_rkcv.GetCount();}
	POSITION GetStart();
		//{return m_rkcv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RKCV_K& rKey,T_RKCV_D& rData);
		//{m_rkcv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RKCV_K,T_RKCV_K,T_RKCV_D,T_RKCV_D&>m_rkcv;
};

#endif

