#ifndef __DB_ESSFU_DB_H__
#define __DB_ESSFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ESSFU
{
public:
	CDB_ESSFU()
	{
		m_essf.InitHashTable(HASHSIZEESSF);
	}
	virtual ~CDB_ESSFU(){};

public:
	void Add(T_UDRD_KEY Key,T_ESSF_UDRD_D& rData)
		{m_essf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_essf.RemoveKey(Key);}
	void DelAll()
		{m_essf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ESSF_UDRD_D& rData)
		{return m_essf.Lookup(Key,rData);}
	int GetCount()
		{return m_essf.GetCount();}
	POSITION GetStart()
		{return m_essf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ESSF_UDRD_D& rData)
		{m_essf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ESSF_UDRD_D,T_ESSF_UDRD_D&>m_essf;
};

#endif