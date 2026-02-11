#ifndef __DB_SCLMU_DB_H__
#define __DB_SCLMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SCLMU
{
public:
	CDB_SCLMU()
	{
		m_sclm.InitHashTable(HASHSIZESCLM);
	}
	virtual ~CDB_SCLMU(){};

public:
	void Add(T_UDRD_KEY Key,T_SCLM_UDRD_D& rData)
		{m_sclm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sclm.RemoveKey(Key);}
	void DelAll()
		{m_sclm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SCLM_UDRD_D& rData)
		{return m_sclm.Lookup(Key,rData);}
	int GetCount()
		{return m_sclm.GetCount();}
	POSITION GetStart()
		{return m_sclm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SCLM_UDRD_D& rData)
		{m_sclm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SCLM_UDRD_D,T_SCLM_UDRD_D&>m_sclm;
};

#endif