#ifndef __DB_PMFM_DB_H__
#define __DB_PMFM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MATL;

class CDB_PMFM
{
public:
	CDB_PMFM();
	virtual ~CDB_PMFM();

public:
	void Add(T_PMFM_K Key,T_PMFM_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_PMFM_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_PMFM_K Key,T_PMFM_D& rData)
		{return m_pmfm.Lookup(Key,rData);}
	int GetCount()
		{return m_pmfm.GetCount();}
	POSITION GetStart()
		{return m_pmfm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PMFM_K& rKey,T_PMFM_D& rData)
		{m_pmfm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_PMFM_K,T_PMFM_K,T_PMFM_D,T_PMFM_D&>m_pmfm;
};

#endif