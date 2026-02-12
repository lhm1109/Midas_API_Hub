#ifndef __DB_MVHCU_DB_H__
#define __DB_MVHCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVHCU
{
public:
	CDB_MVHCU()
	{
		m_mvhc.InitHashTable(HASHSIZEMVHC);
	}
	virtual ~CDB_MVHCU(){};

public:
	void Add(T_UDRD_KEY Key,T_MVHC_UDRD_D& rData)
		{m_mvhc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvhc.RemoveKey(Key);}
	void DelAll()
		{m_mvhc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVHC_UDRD_D& rData)
		{return m_mvhc.Lookup(Key,rData);}
	int GetCount()
		{return m_mvhc.GetCount();}
	POSITION GetStart()
		{return m_mvhc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVHC_UDRD_D& rData)
		{m_mvhc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVHC_UDRD_D,T_MVHC_UDRD_D&>m_mvhc;
};

#endif