#ifndef __DB_ASPMU_DB_H__
#define __DB_ASPMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ASPMU
{
public:
	CDB_ASPMU()
	{
		m_aspm.InitHashTable(HASHSIZEASPM);
	}
	virtual ~CDB_ASPMU(){};

public:
	void Add(T_UDRD_KEY Key,T_ASPM_UDRD_D& rData)
		{m_aspm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_aspm.RemoveKey(Key);}
	void DelAll()
		{m_aspm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ASPM_UDRD_D& rData)
		{return m_aspm.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ASPM_UDRD_D,T_ASPM_UDRD_D&>m_aspm;
};

#endif