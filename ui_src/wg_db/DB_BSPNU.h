#ifndef __DB_BSPNU_DB_H__
#define __DB_BSPNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BSPNU
{
public:
	CDB_BSPNU()
	{
		m_bspn.InitHashTable(HASHSIZEBSPN);
	}
	virtual ~CDB_BSPNU(){};

public:
	void Add(T_UDRD_KEY Key,T_BSPN_UDRD_D& rData)
		{m_bspn.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bspn.RemoveKey(Key);}
	void DelAll()
		{m_bspn.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BSPN_UDRD_D& rData)
		{return m_bspn.Lookup(Key,rData);}
	int GetCount()
		{return m_bspn.GetCount();}
	POSITION GetStart()
		{return m_bspn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BSPN_UDRD_D& rData)
		{m_bspn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BSPN_UDRD_D,T_BSPN_UDRD_D&>m_bspn;
};

#endif