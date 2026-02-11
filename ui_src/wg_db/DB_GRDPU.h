#ifndef __DB_GRDPU_DB_H__
#define __DB_GRDPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GRDPU
{
public:
	CDB_GRDPU()
	{
		m_grdp.InitHashTable(HASHSIZEGRDP);
	}
	virtual ~CDB_GRDPU(){};

public:
	void Add(T_UDRD_KEY Key,T_GRDP_UDRD_D& rData)
		{m_grdp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_grdp.RemoveKey(Key);}
	void DelAll()
		{m_grdp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GRDP_UDRD_D& rData)
		{return m_grdp.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GRDP_UDRD_D,T_GRDP_UDRD_D&>m_grdp;
};

#endif