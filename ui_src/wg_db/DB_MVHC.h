#ifndef __DB_MVHC_DB_H__
#define __DB_MVHC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVHC
{
public:
	CDB_MVHC();
	virtual ~CDB_MVHC();
	CDBDoc* m_pDoc;

public:
	T_MVHC_K m_nStartNum;
	T_MVHC_K m_nLastNum;

public:
	void Add(T_MVHC_K Key,T_MVHC_D& rData);
	BOOL Del(T_MVHC_K Key);
	void Initialize();

public:
	BOOL Get(T_MVHC_K Key,T_MVHC_D& rData);
		//{return m_mvhc.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvhc.GetCount();}
	POSITION GetStart();
		//{return m_mvhc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVHC_K& rKey,T_MVHC_D& rData);
		//{m_mvhc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVHC_K,T_MVHC_K,T_MVHC_D,T_MVHC_D&>m_mvhc;
};

#endif
