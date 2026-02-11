#ifndef __DB_HDGPU_DB_H__
#define __DB_HDGPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HDGPU
{
public:
	CDB_HDGPU()
	{
		m_hdgp.InitHashTable(HASHSIZEHDGP);
	}
	virtual ~CDB_HDGPU(){};

public:
	void Add(T_UDRD_KEY Key,T_HDGP_UDRD_D& rData)
		{m_hdgp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hdgp.RemoveKey(Key);}
	void DelAll()
		{m_hdgp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HDGP_UDRD_D& rData)
		{return m_hdgp.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HDGP_UDRD_D,T_HDGP_UDRD_D&>m_hdgp;
};

#endif