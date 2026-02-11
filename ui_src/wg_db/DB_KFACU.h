#ifndef __DB_KFACU_DB_H__
#define __DB_KFACU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_KFACU
{
public:
	CDB_KFACU()
	{
		m_kfac.InitHashTable(HASHSIZEKFAC);
	}
	virtual ~CDB_KFACU(){};

public:
	void Add(T_UDRD_KEY Key,T_KFAC_UDRD_D& rData)
		{m_kfac.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_kfac.RemoveKey(Key);}
	void DelAll()
		{m_kfac.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_KFAC_UDRD_D& rData)
		{return m_kfac.Lookup(Key,rData);}
	int GetCount()
		{return m_kfac.GetCount();}
	POSITION GetStart()
		{return m_kfac.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_KFAC_UDRD_D& rData)
		{m_kfac.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_KFAC_UDRD_D,T_KFAC_UDRD_D&>m_kfac;
};

#endif