#ifndef __DB_STFNU_DB_H__
#define __DB_STFNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STFNU
{
public:
	CDB_STFNU()
	{
		m_stfn.InitHashTable(HASHSIZESTFN);
	}
	virtual ~CDB_STFNU(){};

public:
	void Add(T_UDRD_KEY Key,T_STFN_UDRD_D& rData)
		{m_stfn.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stfn.RemoveKey(Key);}
	void DelAll()
		{m_stfn.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STFN_UDRD_D& rData)
		{return m_stfn.Lookup(Key,rData);}
	int GetCount()
		{return m_stfn.GetCount();}
	POSITION GetStart()
		{return m_stfn.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STFN_UDRD_D& rData)
		{m_stfn.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STFN_UDRD_D,T_STFN_UDRD_D&>m_stfn;
};

#endif