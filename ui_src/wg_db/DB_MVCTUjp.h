#ifndef __DB_MVCTUjp_DB_H__
#define __DB_MVCTUjp_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MVCTUjp
{
public:
	CDB_MVCTUjp()
	{
		m_mvctjp.InitHashTable(HASHSIZEMVCTjp);
	}
	virtual ~CDB_MVCTUjp(){};

public:
	void Add(T_UDRD_KEY Key,T_MVCTjp_UDRD_D& rData)
		{m_mvctjp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mvctjp.RemoveKey(Key);}
	void DelAll()
		{m_mvctjp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MVCTjp_UDRD_D& rData)
		{return m_mvctjp.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MVCTjp_UDRD_D,T_MVCTjp_UDRD_D&>m_mvctjp;
};

#endif