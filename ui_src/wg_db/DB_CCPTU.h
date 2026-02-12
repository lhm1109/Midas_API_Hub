#ifndef __DB_CCPTU_DB_H__
#define __DB_CCPTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CCPTU
{
public:
	CDB_CCPTU()
	{
		m_ccpt.InitHashTable(HASHSIZECCPT);
	}
	virtual ~CDB_CCPTU(){};

public:
	void Add(T_UDRD_KEY Key,T_CCPT_UDRD_D& rData)
		{m_ccpt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ccpt.RemoveKey(Key);}
	void DelAll()
		{m_ccpt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CCPT_UDRD_D& rData)
		{return m_ccpt.Lookup(Key,rData);}
	int GetCount()
		{return m_ccpt.GetCount();}
	POSITION GetStart()
		{return m_ccpt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CCPT_UDRD_D& rData)
		{m_ccpt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CCPT_UDRD_D,T_CCPT_UDRD_D&>m_ccpt;
};

#endif