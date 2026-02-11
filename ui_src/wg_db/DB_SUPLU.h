#ifndef __DB_SUPLU_DB_H__
#define __DB_SUPLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SUPLU
{
public:
	CDB_SUPLU()
	{
		m_supl.InitHashTable(HASHSIZESUPL);
	}
	virtual ~CDB_SUPLU(){};

public:
	void Add(T_UDRD_KEY Key,T_SUPL_UDRD_D& rData)
	{m_supl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_supl.RemoveKey(Key);}
	void DelAll()
	{m_supl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SUPL_UDRD_D& rData)
	{return m_supl.Lookup(Key,rData);}
	int GetCount()
	{return m_supl.GetCount();}
	POSITION GetStart()
	{return m_supl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SUPL_UDRD_D& rData)
	{m_supl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SUPL_UDRD_D,T_SUPL_UDRD_D&>m_supl;
};

#endif

