#ifndef __DB_IMPFU_DB_H__
#define __DB_IMPFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_IMPFU
{
public:
	CDB_IMPFU()
	{
		m_impf.InitHashTable(HASHSIZEIMPF);
	}
	virtual ~CDB_IMPFU(){};

public:   
	void Add(T_UDRD_KEY Key,T_IMPF_UDRD_D& rData)
		{m_impf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_impf.RemoveKey(Key);}
	void DelAll()
		{m_impf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_IMPF_UDRD_D& rData)
		{return m_impf.Lookup(Key,rData);}
	int GetCount()
		{return m_impf.GetCount();}
	POSITION GetStart()
		{return m_impf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_IMPF_UDRD_D& rData)
		{m_impf.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_IMPF_UDRD_D,T_IMPF_UDRD_D&>m_impf;
};

#endif
