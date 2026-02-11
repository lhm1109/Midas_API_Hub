#ifndef __DB_DIAGU_DB_H__
#define __DB_DIAGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DIAGU
{
public:
	CDB_DIAGU()
	{
		m_diag.InitHashTable(HASHSIZEDIAG);
	}
	virtual ~CDB_DIAGU(){};

public:
	void Add(T_UDRD_KEY Key,T_DIAG_UDRD_D& rData)
		{m_diag.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_diag.RemoveKey(Key);}
	void DelAll()
		{m_diag.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DIAG_UDRD_D& rData)
		{return m_diag.Lookup(Key,rData);}
	int GetCount()
		{return m_diag.GetCount();}
	POSITION GetStart()
		{return m_diag.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DIAG_UDRD_D& rData)
		{m_diag.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DIAG_UDRD_D,T_DIAG_UDRD_D&>m_diag;
};

#endif