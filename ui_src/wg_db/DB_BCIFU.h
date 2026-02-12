#ifndef __DB_BCIFU_DB_H__
#define __DB_BCIFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BCIFU
{
public:
	CDB_BCIFU()
	{
		m_bcif.InitHashTable(HASHSIZEBCIF);
	}
	virtual ~CDB_BCIFU(){};

public:
	void Add(T_UDRD_KEY Key,T_BCIF_UDRD_D& rData)
		{m_bcif.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bcif.RemoveKey(Key);}
	void DelAll()
		{m_bcif.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BCIF_UDRD_D& rData)
		{return m_bcif.Lookup(Key,rData);}
	int GetCount()
		{return m_bcif.GetCount();}
	POSITION GetStart()
		{return m_bcif.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BCIF_UDRD_D& rData)
		{m_bcif.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BCIF_UDRD_D,T_BCIF_UDRD_D&>m_bcif;
};

#endif