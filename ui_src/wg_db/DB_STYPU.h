#ifndef __DB_STYPU_DB_H__
#define __DB_STYPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STYPU
{
public:
	CDB_STYPU()
	{
		m_styp.InitHashTable(HASHSIZESTYP);
	}
	virtual ~CDB_STYPU(){};

public:
	void Add(T_UDRD_KEY Key,T_STYP_UDRD_D& rData)
		{m_styp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_styp.RemoveKey(Key);}
	void DelAll()
		{m_styp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STYP_UDRD_D& rData)
		{return m_styp.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STYP_UDRD_D,T_STYP_UDRD_D&>m_styp;
};

#endif