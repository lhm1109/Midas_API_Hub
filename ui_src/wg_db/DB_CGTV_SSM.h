#ifndef __DB_CGTV_SSM_DB_H__
#define __DB_CGTV_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGTV_SSM
{
public:
	CDB_CGTV_SSM();
	virtual ~CDB_CGTV_SSM();

public:
	void Add(T_CGTV_SSM_K Key,T_CGTV_SSM_D& rData);
	BOOL Del(T_CGTV_SSM_K Key);

public:
	// cgtv_ssm
	BOOL Get(T_CGTV_SSM_K Key,T_CGTV_SSM_D& rData)
		{return m_cgtv_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_cgtv_ssm.GetCount();}
	POSITION GetStart()
		{return m_cgtv_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_CGTV_SSM_K& rKey,T_CGTV_SSM_D& rData)
		{m_cgtv_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CGTV_SSM_K, T_CGTV_SSM_K,T_CGTV_SSM_D,T_CGTV_SSM_D&>m_cgtv_ssm;
};

#endif