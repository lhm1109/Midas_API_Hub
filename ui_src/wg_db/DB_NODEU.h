#ifndef __DB_NODEU_DB_H__
#define __DB_NODEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NODEU
{
public:
	CDB_NODEU()
	{
		m_node.InitHashTable(HASHSIZENODE);
	}
	virtual ~CDB_NODEU(){};

public:
	void Add(T_UDRD_KEY Key,T_NODE_UDRD_D& rData)
		{m_node.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_node.RemoveKey(Key);}
	void DelAll()
		{m_node.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_NODE_UDRD_D& rData)
		{return m_node.Lookup(Key,rData);}
	int GetCount()
		{return m_node.GetCount();}
	POSITION GetStart()
		{return m_node.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_NODE_UDRD_D& rData)
		{m_node.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_NODE_UDRD_D,T_NODE_UDRD_D&>m_node;
};

#endif