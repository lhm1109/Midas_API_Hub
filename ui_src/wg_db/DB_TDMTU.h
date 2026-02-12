#ifndef __DB_TDMTU_DB_H__
#define __DB_TDMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDMTU
{
public:
	CDB_TDMTU()
	{
		m_tdmt.InitHashTable(HASHSIZETDMT);
	}
	virtual ~CDB_TDMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDMT_UDRD_D& rData)
		{m_tdmt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdmt.RemoveKey(Key);}
	void DelAll()
		{m_tdmt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDMT_UDRD_D& rData)
		{return m_tdmt.Lookup(Key,rData);}
	int GetCount()
		{return m_tdmt.GetCount();}
	POSITION GetStart()
		{return m_tdmt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDMT_UDRD_D& rData)
		{m_tdmt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDMT_UDRD_D,T_TDMT_UDRD_D&>m_tdmt;
};

#endif