#ifndef __DB_BPLTU_DB_H__
#define __DB_BPLTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_BPLTU
{
public:
	CDB_BPLTU()
	{
		m_BPLT.InitHashTable(HASHSIZEBPLT);
	}
	virtual ~CDB_BPLTU(){};

public:
	void Add(T_UDRD_KEY Key,T_BPLT_UDRD_D& rData)
		{m_BPLT.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_BPLT.RemoveKey(Key);}
	void DelAll()
		{m_BPLT.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BPLT_UDRD_D& rData)
		{return m_BPLT.Lookup(Key,rData);}
	int GetCount()
		{return m_BPLT.GetCount();}
	POSITION GetStart()
		{return m_BPLT.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BPLT_UDRD_D& rData)
		{m_BPLT.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_BPLT_UDRD_D,T_BPLT_UDRD_D&>m_BPLT;
};

#endif
