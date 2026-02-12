#ifndef __DB_RIPBU_DB_H__
#define __DB_RIPBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RIPBU
{
public:
	CDB_RIPBU()
	{
		m_ripb.InitHashTable(HASHSIZERIPB);
	}
	virtual ~CDB_RIPBU(){};

public:
	void Add(T_UDRD_KEY Key,T_RIPB_UDRD_D& rData)
		{m_ripb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ripb.RemoveKey(Key);}
	void DelAll()
		{m_ripb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RIPB_UDRD_D& rData)
		{return m_ripb.Lookup(Key,rData);}
	int GetCount()
		{return m_ripb.GetCount();}
	POSITION GetStart()
		{return m_ripb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RIPB_UDRD_D& rData)
		{m_ripb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RIPB_UDRD_D,T_RIPB_UDRD_D&>m_ripb;
};

#endif