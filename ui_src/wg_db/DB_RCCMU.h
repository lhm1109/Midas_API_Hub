#ifndef __DB_RCCMU_DB_H__
#define __DB_RCCMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_RCCMU
{
public:
	CDB_RCCMU()
	{
		m_RCCM.InitHashTable(HASHSIZERCCM);
	}
	virtual ~CDB_RCCMU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCCM_UDRD_D& rData)
	{m_RCCM.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_RCCM.RemoveKey(Key);}
	void DelAll()
	{m_RCCM.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCCM_UDRD_D& rData)
	{return m_RCCM.Lookup(Key,rData);}
	int GetCount()
	{return m_RCCM.GetCount();}
	POSITION GetStart()
	{return m_RCCM.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RCCM_UDRD_D& rData)
	{m_RCCM.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCCM_UDRD_D,T_RCCM_UDRD_D&>m_RCCM;
};

#endif
