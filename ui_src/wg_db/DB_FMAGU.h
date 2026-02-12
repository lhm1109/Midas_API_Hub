#ifndef __DB_FMAGU_DB_H__
#define __DB_FMAGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FMAGU
{
public:
	CDB_FMAGU()
	{
		m_fmag.InitHashTable(HASHSIZEFMAG);
	}
	virtual ~CDB_FMAGU(){};

public:
	void Add(T_UDRD_KEY Key,T_FMAG_UDRD_D& rData)
		{m_fmag.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fmag.RemoveKey(Key);}
	void DelAll()
		{m_fmag.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FMAG_UDRD_D& rData)
		{return m_fmag.Lookup(Key,rData);}
	int GetCount()
		{return m_fmag.GetCount();}
	POSITION GetStart()
		{return m_fmag.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FMAG_UDRD_D& rData)
		{m_fmag.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FMAG_UDRD_D,T_FMAG_UDRD_D&>m_fmag;
};

#endif