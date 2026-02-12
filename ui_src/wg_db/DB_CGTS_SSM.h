#ifndef __DB_CGTS_SSM_DB_H__
#define __DB_CGTS_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_CGTS_SSM
{
public:
	CDB_CGTS_SSM();
	virtual ~CDB_CGTS_SSM();

public:
	void Add(T_CGTS_SSM_K Key,T_CGTS_SSM_D& rData,CDB_SECT* pSect);
	BOOL Del(T_CGTS_SSM_K Key,CDB_SECT* pSect);

public:
	// cgts_ssm
	BOOL Get(T_CGTS_SSM_K Key,T_CGTS_SSM_D& rData)
		{return m_cgts_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_cgts_ssm.GetCount();}
	POSITION GetStart()
		{return m_cgts_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CGTS_SSM_K& rKey,T_CGTS_SSM_D& rData)
		{m_cgts_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGTS_SSM_K,T_CGTS_SSM_K,T_CGTS_SSM_D,T_CGTS_SSM_D&>m_cgts_ssm;
};

#endif