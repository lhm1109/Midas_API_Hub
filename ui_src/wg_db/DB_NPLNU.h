#ifndef __DB_NPLNU_DB_H__
#define __DB_NPLNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NPLNU
{
public:
	CDB_NPLNU()
	{
		m_npln.InitHashTable(HASHSIZENPLN);
	}
	virtual ~CDB_NPLNU(){};

public:
	void Add(T_UDRD_KEY Key,T_NPLN_UDRD_D& rData)
		{m_npln.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_npln.RemoveKey(Key);}
	void DelAll()
		{m_npln.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NPLN_UDRD_D& rData)
		{return m_npln.Lookup(Key,rData);}
	int GetCount()
		{return m_npln.GetCount();}
	POSITION GetStart()
		{return m_npln.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NPLN_UDRD_D& rData)
		{m_npln.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NPLN_UDRD_D,T_NPLN_UDRD_D&>m_npln;
};

#endif