#ifndef __DB_SECFU_DB_H__
#define __DB_SECFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECFU
{
public:
	CDB_SECFU()
	{
		m_secf.InitHashTable(HASHSIZESECF);
	}
	virtual ~CDB_SECFU(){};

public:
	void Add(T_UDRD_KEY Key,T_SECF_UDRD_D& rData)
		{m_secf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_secf.RemoveKey(Key);}
	void DelAll()
		{m_secf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SECF_UDRD_D& rData)
		{return m_secf.Lookup(Key,rData);}
	int GetCount()
		{return m_secf.GetCount();}
	POSITION GetStart()
		{return m_secf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SECF_UDRD_D& rData)
		{m_secf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SECF_UDRD_D,T_SECF_UDRD_D&>m_secf;
};

#endif
