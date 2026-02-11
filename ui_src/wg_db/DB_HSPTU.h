#ifndef __DB_HSPTU_DB_H__
#define __DB_HSPTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_HSPTU
{
public:
	CDB_HSPTU()
	{
		m_hspt.InitHashTable(HASHSIZEHSPT);
	}
	virtual ~CDB_HSPTU(){};

public:   
	void Add(T_UDRD_KEY Key,T_HSPT_UDRD_D& rData)
		{m_hspt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hspt.RemoveKey(Key);}
	void DelAll()
		{m_hspt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HSPT_UDRD_D& rData)
		{return m_hspt.Lookup(Key,rData);}
	int GetCount()
		{return m_hspt.GetCount();}
	POSITION GetStart()
		{return m_hspt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HSPT_UDRD_D& rData)
		{m_hspt.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HSPT_UDRD_D,T_HSPT_UDRD_D&>m_hspt;
};

#endif
