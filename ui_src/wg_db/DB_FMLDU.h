#ifndef __DB_FMLDU_DB_H__
#define __DB_FMLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FMLDU
{
public:
	CDB_FMLDU()
	{
		m_fmld.InitHashTable(HASHSIZEFMLD);
	}
	virtual ~CDB_FMLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_FMLD_UDRD_D& rData)
		{m_fmld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fmld.RemoveKey(Key);}
	void DelAll()
		{m_fmld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FMLD_UDRD_D& rData)
		{return m_fmld.Lookup(Key,rData);}
	int GetCount()
		{return m_fmld.GetCount();}
	POSITION GetStart()
		{return m_fmld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FMLD_UDRD_D& rData)
		{m_fmld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FMLD_UDRD_D,T_FMLD_UDRD_D&>m_fmld;
};

#endif