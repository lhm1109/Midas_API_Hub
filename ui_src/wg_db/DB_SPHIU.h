#ifndef __DB_SPHIU_DB_H__
#define __DB_SPHIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPHIU
{
public:
	CDB_SPHIU()
	{
		m_sphi.InitHashTable(HASHSIZESPHI);
	}
	virtual ~CDB_SPHIU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPHI_UDRD_D& rData)
		{m_sphi.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sphi.RemoveKey(Key);}
	void DelAll()
		{m_sphi.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPHI_UDRD_D& rData)
		{return m_sphi.Lookup(Key,rData);}
	int GetCount()
		{return m_sphi.GetCount();}
	POSITION GetStart()
		{return m_sphi.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPHI_UDRD_D& rData)
		{m_sphi.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPHI_UDRD_D,T_SPHI_UDRD_D&>m_sphi;
};

#endif