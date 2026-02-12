#ifndef __DB_RKCVU_DB_H__
#define __DB_RKCVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RKCVU
{
public:
	CDB_RKCVU()
	{
		m_rkcv.InitHashTable(HASHSIZERKCV);
	}
	virtual ~CDB_RKCVU(){};

public:
	void Add(T_UDRD_KEY Key,T_RKCV_UDRD_D& rData)
		{m_rkcv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rkcv.RemoveKey(Key);}
	void DelAll()
		{m_rkcv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RKCV_UDRD_D& rData)
		{return m_rkcv.Lookup(Key,rData);}
	int GetCount()
		{return m_rkcv.GetCount();}
	POSITION GetStart()
		{return m_rkcv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RKCV_UDRD_D& rData)
		{m_rkcv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RKCV_UDRD_D,T_RKCV_UDRD_D&>m_rkcv;
};

#endif