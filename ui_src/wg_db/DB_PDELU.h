#ifndef __DB_PDELU_DB_H__
#define __DB_PDELU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PDELU
{
public:
	CDB_PDELU()
	{
		m_pdel.InitHashTable(HASHSIZEPDEL);
	}
	virtual ~CDB_PDELU(){};

public:
	void Add(T_UDRD_KEY Key,T_PDEL_UDRD_D& rData)
		{m_pdel.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pdel.RemoveKey(Key);}
	void DelAll()
		{m_pdel.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PDEL_UDRD_D& rData)
		{return m_pdel.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_PDEL_UDRD_D,T_PDEL_UDRD_D&>m_pdel;
};

#endif