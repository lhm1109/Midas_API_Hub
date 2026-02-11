#ifndef __DB_PRCSU_DB_H__
#define __DB_PRCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PRCSU
{
public:
	CDB_PRCSU()
	{
		m_prcs.InitHashTable(HASHSIZEPRCS);
	}
	virtual ~CDB_PRCSU(){};

public:
	void Add(T_UDRD_KEY Key,T_PRCS_UDRD_D& rData)
		{m_prcs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_prcs.RemoveKey(Key);}
	void DelAll()
		{m_prcs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PRCS_UDRD_D& rData)
		{return m_prcs.Lookup(Key,rData);}
	int GetCount()
		{return m_prcs.GetCount();}
	POSITION GetStart()
		{return m_prcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PRCS_UDRD_D& rData)
		{m_prcs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PRCS_UDRD_D,T_PRCS_UDRD_D&>m_prcs;
};

#endif