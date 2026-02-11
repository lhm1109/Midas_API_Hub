#ifndef __DB_THEF_DB_H__
#define __DB_THEF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THEF
{
public:
	CDB_THEF();
	virtual ~CDB_THEF();
	CDBDoc* m_pDoc;

public:
	T_THEF_K m_nStartNum;
	T_THEF_K m_nLastNum;

public:
	void Add(T_THEF_K Key,T_THEF_D& rData,CDB_THIS* pThis);
	BOOL Del(T_THEF_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_THEF_K Key,T_THEF_D& rData);
		//{return m_thef.Lookup(Key,rData);}
	int GetCount();
		//{return m_thef.GetCount();}
	POSITION GetStart();
		//{return m_thef.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THEF_K& rKey,T_THEF_D& rData);
		//{m_thef.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THEF_K,T_THEF_K,T_THEF_D,T_THEF_D&>m_thef;
};

#endif