#ifndef __DB_RPSV_SSM_DB_H__
#define __DB_RPSV_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECV;

class CDB_RPSV_SSM
{
public:
	CDB_RPSV_SSM();
	virtual ~CDB_RPSV_SSM();

public:
	void Add(T_RPSV_SSM_K Key,T_RPSV_SSM_D& rData, CDB_SECV* pSecv);
	BOOL Del(T_RPSV_SSM_K Key, CDB_SECV* pSecv);

public:
	// rpsv_ssm
	BOOL Get(T_RPSV_SSM_K Key,T_RPSV_SSM_D& rData)
		{return m_rpsv_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_rpsv_ssm.GetCount();}
	POSITION GetStart()
		{return m_rpsv_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RPSV_SSM_K& rKey,T_RPSV_SSM_D& rData)
		{m_rpsv_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RPSV_SSM_K,T_RPSV_SSM_K,T_RPSV_SSM_D,T_RPSV_SSM_D&>m_rpsv_ssm;
};

#endif