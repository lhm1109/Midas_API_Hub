#ifndef __DB_CGTV_SSMU_DB_H__
#define __DB_CGTV_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGTV_SSMU
{
public:
	CDB_CGTV_SSMU()
	{
		m_cgtv_ssm.InitHashTable(HASHSIZECGTVSSM);
	}
	virtual ~CDB_CGTV_SSMU(){};

public:
	// cgtv_ssm
	void Add(T_UDRD_KEY Key,T_CGTV_SSM_UDRD_D& rData)
		{m_cgtv_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgtv_ssm.RemoveKey(Key);}
	void DelAll()
		{m_cgtv_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGTV_SSM_UDRD_D& rData)
		{return m_cgtv_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_cgtv_ssm.GetCount();}
	POSITION GetStart()
		{return m_cgtv_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGTV_SSM_UDRD_D& rData)
		{m_cgtv_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGTV_SSM_UDRD_D,T_CGTV_SSM_UDRD_D&>m_cgtv_ssm;
};

#endif
