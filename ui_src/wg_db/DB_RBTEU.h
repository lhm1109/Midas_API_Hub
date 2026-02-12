#ifndef __DB_RBTEU_DB_H__
#define __DB_RBTEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RBTEU
{
public:
	CDB_RBTEU()
	{
		m_rbte.InitHashTable(HASHSIZERBTE);
	}
	virtual ~CDB_RBTEU(){};

public:
	void Add(T_UDRD_KEY Key,T_RBTE_UDRD_D& rData)
		{m_rbte.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rbte.RemoveKey(Key);}
	void DelAll()
		{m_rbte.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RBTE_UDRD_D& rData)
		{return m_rbte.Lookup(Key,rData);}
	int GetCount()
		{return m_rbte.GetCount();}
	POSITION GetStart()
		{return m_rbte.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RBTE_UDRD_D& rData)
		{m_rbte.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RBTE_UDRD_D,T_RBTE_UDRD_D&>m_rbte;
};

#endif