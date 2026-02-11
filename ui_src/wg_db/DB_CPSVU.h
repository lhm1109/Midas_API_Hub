#ifndef __DB_CPSVU_DB_H__
#define __DB_CPSVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPSVU
{
public:
	CDB_CPSVU()
	{
		m_cpsv.InitHashTable(HASHSIZECPSV);
	}
	virtual ~CDB_CPSVU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPSV_UDRD_D& rData)
		{m_cpsv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpsv.RemoveKey(Key);}
	void DelAll()
		{m_cpsv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPSV_UDRD_D& rData)
		{return m_cpsv.Lookup(Key,rData);}
	int GetCount()
		{return m_cpsv.GetCount();}
	POSITION GetStart()
		{return m_cpsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPSV_UDRD_D& rData)
		{m_cpsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPSV_UDRD_D,T_CPSV_UDRD_D&>m_cpsv;
};

#endif