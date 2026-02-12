#ifndef __DB_DLCMU_DB_H__
#define __DB_DLCMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DLCMU
{
public:
	CDB_DLCMU()
	{
		m_dlcm.InitHashTable(HASHSIZEDLCM);
	}
	virtual ~CDB_DLCMU(){};

public:
	void Add(T_UDRD_KEY Key,T_DLCM_UDRD_D& rData)
		{m_dlcm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dlcm.RemoveKey(Key);}
	void DelAll()
		{m_dlcm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DLCM_UDRD_D& rData)
		{return m_dlcm.Lookup(Key,rData);}
	int GetCount()
		{return m_dlcm.GetCount();}
	POSITION GetStart()
		{return m_dlcm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DLCM_UDRD_D& rData)
		{m_dlcm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DLCM_UDRD_D,T_DLCM_UDRD_D&>m_dlcm;
};

#endif