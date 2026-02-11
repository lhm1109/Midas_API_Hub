#ifndef __DB_STRP_SSMU_DB_H__
#define __DB_STRP_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STRP_SSMU
{
public:
	CDB_STRP_SSMU()
	{
		m_strp_ssm.InitHashTable(HASHSIZESTRPSSM);
	}
	virtual ~CDB_STRP_SSMU(){};

public:
	// strp_ssm
	void Add(T_UDRD_KEY Key,T_STRP_SSM_UDRD_D& rData)
		{m_strp_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_strp_ssm.RemoveKey(Key);}
	void DelAll()
		{m_strp_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STRP_SSM_UDRD_D& rData)
		{return m_strp_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_strp_ssm.GetCount();}
	POSITION GetStart()
		{return m_strp_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STRP_SSM_UDRD_D& rData)
		{m_strp_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STRP_SSM_UDRD_D,T_STRP_SSM_UDRD_D&>m_strp_ssm;
};

#endif
