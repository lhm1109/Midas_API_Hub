#ifndef __DB_CSCS_DB_H__
#define __DB_CSCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_CSCS
{
public:
	CDB_CSCS();
	CDB_CSCS(CDB_CSCS& src);
	CDB_CSCS& operator=(CDB_CSCS& src);
	virtual ~CDB_CSCS();

public:
	T_CSCS_K m_nLastNum;
	T_CSCS_K m_nStartNum;

public:
	void Add(T_CSCS_K Key,T_CSCS_D& rData,CDB_SECT* pSect);
	BOOL Del(T_CSCS_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_CSCS_K Key,T_CSCS_D& rData)
		{return m_cscs.Lookup(Key,rData);}
	int GetCount()
		{return m_cscs.GetCount();}
	POSITION GetStart()
		{return m_cscs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CSCS_K& rKey,T_CSCS_D& rData)
		{m_cscs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CSCS_K,T_CSCS_K,T_CSCS_D,T_CSCS_D&>m_cscs;
};

#endif
