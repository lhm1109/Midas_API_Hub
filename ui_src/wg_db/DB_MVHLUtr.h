#ifndef __DB_MVHLUtr_DB_H__
#define __DB_MVHLUtr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVHLUtr
{
public:
	CDB_MVHLUtr()
	{
		m_mvhltr.InitHashTable(HASHSIZEMVHL);
	}
	virtual ~CDB_MVHLUtr(){};

public:
	void Add(T_UDRD_KEY Key,T_MVHLtr_UDRD_D& rData)
		{m_mvhltr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvhltr.RemoveKey(Key);}
	void DelAll()
		{m_mvhltr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVHLtr_UDRD_D& rData)
		{return m_mvhltr.Lookup(Key,rData);}
	int GetCount()
		{return m_mvhltr.GetCount();}
	POSITION GetStart()
		{return m_mvhltr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MVHLtr_UDRD_D& rData)
		{m_mvhltr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVHLtr_UDRD_D,T_MVHLtr_UDRD_D&>m_mvhltr;
};

#endif