#ifndef __DB_MFACU_DB_H__
#define __DB_MFACU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MFACU
{
public:
	CDB_MFACU()
	{
		m_mfac.InitHashTable(HASHSIZEMFAC);
	}
	virtual ~CDB_MFACU(){};

public:
	void Add(T_UDRD_KEY Key,T_MFAC_UDRD_D& rData)
		{m_mfac.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mfac.RemoveKey(Key);}
	void DelAll()
		{m_mfac.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MFAC_UDRD_D& rData)
		{return m_mfac.Lookup(Key,rData);}
	int GetCount()
		{return m_mfac.GetCount();}
	POSITION GetStart()
		{return m_mfac.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MFAC_UDRD_D& rData)
		{m_mfac.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MFAC_UDRD_D,T_MFAC_UDRD_D&>m_mfac;
};

#endif