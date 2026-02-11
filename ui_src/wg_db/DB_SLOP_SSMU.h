#ifndef __DB_SLOP_SSMU_DB_H__
#define __DB_SLOP_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLOP_SSMU
{
public:
	CDB_SLOP_SSMU()
	{
		m_slop_ssm.InitHashTable(HASHSIZESLOP_SSM);
	}
	virtual ~CDB_SLOP_SSMU(){};

public:
	// slop_ssm
	void Add(T_UDRD_KEY Key,T_SLOP_SSM_UDRD_D& rData)
	{m_slop_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_slop_ssm.RemoveKey(Key);}
	void DelAll()
	{m_slop_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLOP_SSM_UDRD_D& rData)
	{return m_slop_ssm.Lookup(Key,rData);}
	int GetCount()
	{return m_slop_ssm.GetCount();}
	POSITION GetStart()
	{return m_slop_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLOP_SSM_UDRD_D& rData)
	{m_slop_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLOP_SSM_UDRD_D,T_SLOP_SSM_UDRD_D&>m_slop_ssm;
};

#endif
