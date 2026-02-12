#ifndef __DB_HDCTU_DB_H__
#define __DB_HDCTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HDCTU
{
public:
	CDB_HDCTU()
	{
		m_hdct.InitHashTable(HASHSIZEHDCT);
	}
	virtual ~CDB_HDCTU(){};

public:
	void Add(T_UDRD_KEY Key,T_HDCT_UDRD_D& rData)
		{m_hdct.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hdct.RemoveKey(Key);}
	void DelAll()
		{m_hdct.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HDCT_UDRD_D& rData)
		{return m_hdct.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HDCT_UDRD_D,T_HDCT_UDRD_D&>m_hdct;
};

#endif