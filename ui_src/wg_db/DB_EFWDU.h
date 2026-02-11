#ifndef __DB_EFWDU_DB_H__
#define __DB_EFWDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EFWDU
{
public:
	CDB_EFWDU()
	{
		m_EFWD.InitHashTable(HASHSIZEEFWD);
	}
	virtual ~CDB_EFWDU(){};

public:
	// EFWD
	void Add(T_UDRD_KEY Key,T_EFWD_UDRD_D& rData)
	{m_EFWD.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_EFWD.RemoveKey(Key);}
	void DelAll()
	{m_EFWD.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EFWD_UDRD_D& rData)
	{return m_EFWD.Lookup(Key,rData);}
	int GetCount()
	{return m_EFWD.GetCount();}
	POSITION GetStart()
	{return m_EFWD.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EFWD_UDRD_D& rData)
	{m_EFWD.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_EFWD_UDRD_D,T_EFWD_UDRD_D&>m_EFWD;
};

#endif
