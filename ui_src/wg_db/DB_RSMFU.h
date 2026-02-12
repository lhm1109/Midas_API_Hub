#ifndef __DB_RSMFU_DB_H__
#define __DB_RSMFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RSMFU
{
public:
	CDB_RSMFU()
	{
		m_rsmf.InitHashTable(HASHSIZERSMF);
	}
	virtual ~CDB_RSMFU(){};

public:
	void Add(T_UDRD_KEY Key,T_RSMF_UDRD_D& rData)
		{m_rsmf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rsmf.RemoveKey(Key);}
	void DelAll()
		{m_rsmf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RSMF_UDRD_D& rData)
		{return m_rsmf.Lookup(Key,rData);}
	int GetCount()
		{return m_rsmf.GetCount();}
	POSITION GetStart()
		{return m_rsmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RSMF_UDRD_D& rData)
		{m_rsmf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RSMF_UDRD_D,T_RSMF_UDRD_D&>m_rsmf;
};

#endif