#ifndef __DB_RKPOU_DB_H__
#define __DB_RKPOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RKPOU
{
public:
	CDB_RKPOU()
	{
		m_rkpo.InitHashTable(HASHSIZERKPO);
	}
	virtual ~CDB_RKPOU(){};

public:
	void Add(T_UDRD_KEY Key,T_RKPO_UDRD_D& rData)
		{m_rkpo.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rkpo.RemoveKey(Key);}
	void DelAll()
		{m_rkpo.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RKPO_UDRD_D& rData)
		{return m_rkpo.Lookup(Key,rData);}
	int GetCount()
		{return m_rkpo.GetCount();}
	POSITION GetStart()
		{return m_rkpo.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RKPO_UDRD_D& rData)
		{m_rkpo.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RKPO_UDRD_D,T_RKPO_UDRD_D&>m_rkpo;
};

#endif