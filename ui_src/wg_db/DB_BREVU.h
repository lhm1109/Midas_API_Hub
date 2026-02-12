#ifndef __DB_BREVU_DB_H__
#define __DB_BREVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BREVU
{
public:
	CDB_BREVU()
	{
		m_brev.InitHashTable(HASHSIZEBREV);
	}
	virtual ~CDB_BREVU(){};

public:
	void Add(T_UDRD_KEY Key,T_BREV_UDRD_D& rData)
	{m_brev.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_brev.RemoveKey(Key);}
	void DelAll()
	{m_brev.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BREV_UDRD_D& rData)
	{return m_brev.Lookup(Key,rData);}
	int GetCount()
	{return m_brev.GetCount();}
	POSITION GetStart()
	{return m_brev.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BREV_UDRD_D& rData)
	{m_brev.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_BREV_UDRD_D,T_BREV_UDRD_D&>m_brev;
};

#endif

