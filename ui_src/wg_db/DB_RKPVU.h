#ifndef __DB_RKPVU_DB_H__
#define __DB_RKPVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RKPVU
{
public:
	CDB_RKPVU()
	{
		m_rkpv.InitHashTable(HASHSIZERKPV);
	}
	virtual ~CDB_RKPVU(){};

public:
	void Add(T_UDRD_KEY Key,T_RKPV_UDRD_D& rData)
		{m_rkpv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rkpv.RemoveKey(Key);}
	void DelAll()
		{m_rkpv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RKPV_UDRD_D& rData)
		{return m_rkpv.Lookup(Key,rData);}
	int GetCount()
		{return m_rkpv.GetCount();}
	POSITION GetStart()
		{return m_rkpv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RKPV_UDRD_D& rData)
		{m_rkpv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RKPV_UDRD_D,T_RKPV_UDRD_D&>m_rkpv;
};

#endif