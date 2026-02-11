#ifndef __DB_ESEQU_DB_H__
#define __DB_ESEQU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ESEQU
{
public:
	CDB_ESEQU()
	{
		m_eseq.InitHashTable(HASHSIZEESEQ);
	}
	virtual ~CDB_ESEQU(){};

public:
	void Add(T_UDRD_KEY Key,T_ESEQ_UDRD_D& rData)
		{m_eseq.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_eseq.RemoveKey(Key);}
	void DelAll()
		{m_eseq.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ESEQ_UDRD_D& rData)
		{return m_eseq.Lookup(Key,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_ESEQ_UDRD_D,T_ESEQ_UDRD_D&>m_eseq;
};

#endif