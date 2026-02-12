#ifndef __DB_CPCVU_DB_H__
#define __DB_CPCVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPCVU
{
public:
	CDB_CPCVU()
	{
		m_cpcv.InitHashTable(HASHSIZECPCV);
	}
	virtual ~CDB_CPCVU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPCV_UDRD_D& rData)
		{m_cpcv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpcv.RemoveKey(Key);}
	void DelAll()
		{m_cpcv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPCV_UDRD_D& rData)
		{return m_cpcv.Lookup(Key,rData);}
	int GetCount()
		{return m_cpcv.GetCount();}
	POSITION GetStart()
		{return m_cpcv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPCV_UDRD_D& rData)
		{m_cpcv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPCV_UDRD_D,T_CPCV_UDRD_D&>m_cpcv;
};

#endif