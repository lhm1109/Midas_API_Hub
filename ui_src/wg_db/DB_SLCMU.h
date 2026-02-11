#ifndef __DB_SLCMU_DB_H__
#define __DB_SLCMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SLCMU
{
public:
	CDB_SLCMU()
	{
		m_slcm.InitHashTable(HASHSIZESLCM);
	}
	virtual ~CDB_SLCMU(){};

public:
	void Add(T_UDRD_KEY Key,T_SLCM_UDRD_D& rData)
		{m_slcm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_slcm.RemoveKey(Key);}
	void DelAll()
		{m_slcm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SLCM_UDRD_D& rData)
		{return m_slcm.Lookup(Key,rData);}
	int GetCount()
		{return m_slcm.GetCount();}
	POSITION GetStart()
		{return m_slcm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SLCM_UDRD_D& rData)
		{m_slcm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SLCM_UDRD_D,T_SLCM_UDRD_D&>m_slcm;
};

#endif