#ifndef __DB_CAMBU_DB_H__
#define __DB_CAMBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CAMBU
{
public:
	CDB_CAMBU()
	{
		m_camb.InitHashTable(HASHSIZECAMB);
	}
	virtual ~CDB_CAMBU(){};

public:
	void Add(T_UDRD_KEY Key,T_CAMB_UDRD_D& rData)
		{m_camb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_camb.RemoveKey(Key);}
	void DelAll()
		{m_camb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CAMB_UDRD_D& rData)
		{return m_camb.Lookup(Key,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CAMB_UDRD_D,T_CAMB_UDRD_D&>m_camb;
};

#endif