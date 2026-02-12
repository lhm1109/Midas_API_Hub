#ifndef __DB_CSPA_DB_H__
#define __DB_CSPA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CSPA
{
public:
	CDB_CSPA();
	virtual ~CDB_CSPA();
	CDBDoc* m_pDoc;

public:
	T_CSPA_K m_nStartNum;
	T_CSPA_K m_nLastNum;

public:
	void Add(T_CSPA_K Key, T_CSPA_D& rData);
	BOOL Del(T_CSPA_K Key);

public:
	BOOL Get(T_CSPA_K Key, T_CSPA_D& rData);
	//{return m_cspa.Lookup(Key,rData);}
	int GetCount();
	//{return m_cspa.GetCount();}
	POSITION GetStart();
	//{ return m_cspa.GetStartPosition(); }
	void GetNext(POSITION& rNextPosition, T_CSPA_K& rKey, T_CSPA_D& rData);
	//{ m_cspa.GetNextAssoc(rNextPosition, rKey, rData); }

protected:
	CMap<T_CSPA_K, T_CSPA_K, T_CSPA_D, T_CSPA_D&>m_cspa;
};

#endif