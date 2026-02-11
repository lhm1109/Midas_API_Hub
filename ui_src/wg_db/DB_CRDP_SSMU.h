#ifndef __DB_CRDP_SSMU_DB_H__
#define __DB_CRDP_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CRDP_SSMU
{
public:
	CDB_CRDP_SSMU()
	{
		m_crdp_ssm.InitHashTable(HASHSIZECRDPSSM);
	}
	virtual ~CDB_CRDP_SSMU(){};

public:
	// crdp_ssm
	void Add(T_UDRD_KEY Key,T_CRDP_SSM_UDRD_D& rData)
		{m_crdp_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_crdp_ssm.RemoveKey(Key);}
	void DelAll()
		{m_crdp_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CRDP_SSM_UDRD_D& rData)
		{return m_crdp_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_crdp_ssm.GetCount();}
	POSITION GetStart()
		{return m_crdp_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CRDP_SSM_UDRD_D& rData)
		{m_crdp_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CRDP_SSM_UDRD_D,T_CRDP_SSM_UDRD_D&>m_crdp_ssm;
};

#endif
