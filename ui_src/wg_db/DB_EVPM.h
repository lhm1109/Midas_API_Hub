#ifndef __DB_EVPM_DB_H__
#define __DB_EVPM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_EVPM
{
public:
	CDB_EVPM();
	virtual ~CDB_EVPM();
	CDBDoc* m_pDoc;

public:
	T_EVPM_K m_nStartNum;
	T_EVPM_K m_nLastNum;

public:
	void Add(T_EVPM_K Key,T_EVPM_D& rData);
	BOOL Del(T_EVPM_K Key);

public:
	BOOL Get(T_EVPM_K Key,T_EVPM_D& rData);
		//{return m_evpm.Lookup(Key,rData);}
	int GetCount();
		//{return m_evpm.GetCount();}
	POSITION GetStart();
		//{return m_evpm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EVPM_K& rKey,T_EVPM_D& rData);
		//{m_evpm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_EVPM_K,T_EVPM_K,T_EVPM_D,T_EVPM_D&>m_evpm;
};

#endif