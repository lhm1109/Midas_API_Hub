#ifndef __DB_THSL_DB_H__
#define __DB_THSL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THSL
{
public:
	CDB_THSL();
	virtual ~CDB_THSL();
	CDBDoc* m_pDoc;

public:
	T_THSL_K m_nStartNum;
	T_THSL_K m_nLastNum;

public:
	void Add(T_THSL_K Key,T_THSL_D& rData,CDB_THIS* pThis);
	BOOL Del(T_THSL_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_THSL_K Key,T_THSL_D& rData);
		//{return m_thsl.Lookup(Key,rData);}
	int GetCount();
		//{return m_thsl.GetCount();}
	POSITION GetStart();
		//{return m_thsl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THSL_K& rKey,T_THSL_D& rData);
		//{m_thsl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THSL_K,T_THSL_K,T_THSL_D,T_THSL_D&>m_thsl;
};

#endif