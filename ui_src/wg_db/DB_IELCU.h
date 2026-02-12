#ifndef __DB_IELCU_DB_H__
#define __DB_IELCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_IELCU
{
public:
	CDB_IELCU()
	{
		m_ielc.InitHashTable(HASHSIZEIELC);
	}
	virtual ~CDB_IELCU(){};

public:   
	void Add(T_UDRD_KEY Key,T_IELC_UDRD_D& rData)
		{m_ielc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ielc.RemoveKey(Key);}
	void DelAll()
		{m_ielc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IELC_UDRD_D& rData)
		{return m_ielc.Lookup(Key,rData);}
	int GetCount()
		{return m_ielc.GetCount();}
	POSITION GetStart()
		{return m_ielc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IELC_UDRD_D& rData)
		{m_ielc.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IELC_UDRD_D,T_IELC_UDRD_D&>m_ielc;
};

#endif
