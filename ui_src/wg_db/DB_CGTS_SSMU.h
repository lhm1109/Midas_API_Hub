#ifndef __DB_CGTS_SSMU_DB_H__
#define __DB_CGTS_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGTS_SSMU
{
public:
	CDB_CGTS_SSMU()
	{
		m_cgts_ssm.InitHashTable(HASHSIZECGTSSSM);
	}
	virtual ~CDB_CGTS_SSMU(){};

public:
	// cgts_ssm
	void Add(T_UDRD_KEY Key,T_CGTS_SSM_UDRD_D& rData)
		{m_cgts_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgts_ssm.RemoveKey(Key);}
	void DelAll()
		{m_cgts_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGTS_SSM_UDRD_D& rData)
		{return m_cgts_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_cgts_ssm.GetCount();}
	POSITION GetStart()
		{return m_cgts_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGTS_SSM_UDRD_D& rData)
		{m_cgts_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGTS_SSM_UDRD_D,T_CGTS_SSM_UDRD_D&>m_cgts_ssm;
};

#endif
