#ifndef __DB_STFN_SSMU_DB_H__
#define __DB_STFN_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STFN_SSMU
{
public:
	CDB_STFN_SSMU()
	{
		m_stfn_ssm.InitHashTable(HASHSIZESTFNSSM);
	}
	virtual ~CDB_STFN_SSMU(){};

public:
	// stfn_ssm
	void Add(T_UDRD_KEY Key,T_STFN_SSM_UDRD_D& rData)
		{m_stfn_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stfn_ssm.RemoveKey(Key);}
	void DelAll()
		{m_stfn_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STFN_SSM_UDRD_D& rData)
		{return m_stfn_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_stfn_ssm.GetCount();}
	POSITION GetStart()
		{return m_stfn_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STFN_SSM_UDRD_D& rData)
		{m_stfn_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STFN_SSM_UDRD_D,T_STFN_SSM_UDRD_D&>m_stfn_ssm;
};

#endif
