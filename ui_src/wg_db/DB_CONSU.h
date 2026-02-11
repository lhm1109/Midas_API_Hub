#ifndef __DB_CONSU_DB_H__
#define __DB_CONSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_CONSU
{
public:
	CDB_CONSU()
	{
		m_cons.InitHashTable(HASHSIZECONS);
	}
	virtual ~CDB_CONSU(){};

public:   
	void Add(T_UDRD_KEY Key,T_CONS_UDRD_D& rData)
		{m_cons.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cons.RemoveKey(Key);}
	void DelAll()
		{m_cons.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CONS_UDRD_D& rData)
		{return m_cons.Lookup(Key,rData);}
	int GetCount()
		{return m_cons.GetCount();}
	POSITION GetStart()
		{return m_cons.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CONS_UDRD_D& rData)
		{m_cons.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CONS_UDRD_D,T_CONS_UDRD_D&>m_cons;
};

#endif
