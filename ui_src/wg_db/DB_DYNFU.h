#ifndef __DB_DYNFU_DB_H__
#define __DB_DYNFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_DYNFU
{
public:
	CDB_DYNFU()
	{
		m_dynf.InitHashTable(HASHSIZEDYNF);
	}
	virtual ~CDB_DYNFU(){};

public:   
	void Add(T_UDRD_KEY Key,T_DYNF_UDRD_D& rData)
		{m_dynf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dynf.RemoveKey(Key);}
	void DelAll()
		{m_dynf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DYNF_UDRD_D& rData)
		{return m_dynf.Lookup(Key,rData);}
	int GetCount()
		{return m_dynf.GetCount();}
	POSITION GetStart()
		{return m_dynf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DYNF_UDRD_D& rData)
		{m_dynf.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DYNF_UDRD_D,T_DYNF_UDRD_D&>m_dynf;
};

#endif
