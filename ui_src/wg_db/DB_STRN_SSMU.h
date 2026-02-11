#ifndef __DB_STRN_SSMU_DB_H__
#define __DB_STRN_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STRN_SSMU
{
public:
	CDB_STRN_SSMU()
	{
		m_strn_ssm.InitHashTable(HASHSIZESTRNSSM);
	}
	virtual ~CDB_STRN_SSMU(){};

public:
	// strn_ssm
	void Add(T_UDRD_KEY Key,T_STRN_SSM_UDRD_D& rData)
		{m_strn_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_strn_ssm.RemoveKey(Key);}
	void DelAll()
		{m_strn_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STRN_SSM_UDRD_D& rData)
		{return m_strn_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_strn_ssm.GetCount();}
	POSITION GetStart()
		{return m_strn_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STRN_SSM_UDRD_D& rData)
		{m_strn_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STRN_SSM_UDRD_D,T_STRN_SSM_UDRD_D&>m_strn_ssm;
};

#endif
