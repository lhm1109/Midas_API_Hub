#ifndef __DB_PSSFU_DB_H__
#define __DB_PSSFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PSSFU
{
public:
	CDB_PSSFU()
	{
		m_pssf.InitHashTable(HASHSIZEPSSF);
	}
	virtual ~CDB_PSSFU(){};

public:
	void Add(T_UDRD_KEY Key,T_PSSF_UDRD_D& rData)
		{m_pssf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pssf.RemoveKey(Key);}
	void DelAll()
		{m_pssf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PSSF_UDRD_D& rData)
		{return m_pssf.Lookup(Key,rData);}
	int GetCount()
		{return m_pssf.GetCount();}
	POSITION GetStart()
		{return m_pssf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PSSF_UDRD_D& rData)
		{m_pssf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PSSF_UDRD_D,T_PSSF_UDRD_D&>m_pssf;
};

#endif