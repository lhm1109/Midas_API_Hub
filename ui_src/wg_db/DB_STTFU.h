#ifndef __DB_STTFU_DB_H__
#define __DB_STTFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STTFU
{
public:
	CDB_STTFU()
	{
		m_sttf.InitHashTable(HASHSIZESTTF);
	}
	virtual ~CDB_STTFU(){};

public:
	void Add(T_UDRD_KEY Key,T_STTF_UDRD_D& rData)
		{m_sttf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sttf.RemoveKey(Key);}
	void DelAll()
		{m_sttf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STTF_UDRD_D& rData)
		{return m_sttf.Lookup(Key,rData);}
	int GetCount()
		{return m_sttf.GetCount();}
	POSITION GetStart()
		{return m_sttf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STTF_UDRD_D& rData)
		{m_sttf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STTF_UDRD_D,T_STTF_UDRD_D&>m_sttf;
};

#endif