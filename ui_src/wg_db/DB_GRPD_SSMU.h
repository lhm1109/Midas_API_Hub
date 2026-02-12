#ifndef __DB_GRPD_SSMU_DB_H__
#define __DB_GRPD_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GRPD_SSMU
{
public:
	CDB_GRPD_SSMU()
	{
		m_grpd_ssm.InitHashTable(HASHSIZEGRPDSSM);
	}
	virtual ~CDB_GRPD_SSMU(){};

public:
	// grpd_ssm
	void Add(T_UDRD_KEY Key,T_GRPD_SSM_UDRD_D& rData)
		{m_grpd_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_grpd_ssm.RemoveKey(Key);}
	void DelAll()
		{m_grpd_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GRPD_SSM_UDRD_D& rData)
		{return m_grpd_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_grpd_ssm.GetCount();}
	POSITION GetStart()
		{return m_grpd_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GRPD_SSM_UDRD_D& rData)
		{m_grpd_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GRPD_SSM_UDRD_D,T_GRPD_SSM_UDRD_D&>m_grpd_ssm;
};

#endif
