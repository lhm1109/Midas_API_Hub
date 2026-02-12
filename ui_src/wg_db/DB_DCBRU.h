#ifndef __DB_DCBRU_DB_H__
#define __DB_DCBRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DCBRU
{
public:
	CDB_DCBRU()
	{
		m_dcbr.InitHashTable(HASHSIZEDCBR);
	}
	virtual ~CDB_DCBRU(){};

public:
	void Add(T_UDRD_KEY Key,T_DCBR_UDRD_D& rData)
		{m_dcbr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dcbr.RemoveKey(Key);}
	void DelAll()
		{m_dcbr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DCBR_UDRD_D& rData)
		{return m_dcbr.Lookup(Key,rData);}
	int GetCount()
		{return m_dcbr.GetCount();}
	POSITION GetStart()
		{return m_dcbr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DCBR_UDRD_D& rData)
		{m_dcbr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DCBR_UDRD_D,T_DCBR_UDRD_D&>m_dcbr;
};

#endif