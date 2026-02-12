#ifndef __DB_SPLC_DB_H__
#define __DB_SPLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SPLC
{
public:
	CDB_SPLC();
	virtual ~CDB_SPLC();
	CDBDoc* m_pDoc;

public:
	T_SPLC_K m_nStartNum;
	T_SPLC_K m_nLastNum;

public:
	void Add(T_SPLC_K Key,T_SPLC_D& rData);
	BOOL Del(T_SPLC_K Key);
	void Initialize();

public:
	BOOL Get(T_SPLC_K Key,T_SPLC_D& rData);
		//{return m_splc.Lookup(Key,rData);}
	int GetCount();
		//{return m_splc.GetCount();}
	POSITION GetStart();
		//{return m_splc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SPLC_K& rKey,T_SPLC_D& rData);
		//{m_splc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SPLC_K,T_SPLC_K,T_SPLC_D,T_SPLC_D&>m_splc;
};

#endif
