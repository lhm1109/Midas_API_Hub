#ifndef __DB_UTBLU_DB_H__
#define __DB_UTBLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UTBLU
{
public:
	CDB_UTBLU()
	{
		m_utbl.InitHashTable(HASHSIZEUTBL);
	}
	virtual ~CDB_UTBLU(){};

public:
	void Add(T_UDRD_KEY Key,T_UTBL_UDRD_D& rData)
		{m_utbl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_utbl.RemoveKey(Key);}
	void DelAll()
		{m_utbl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_UTBL_UDRD_D& rData)
		{return m_utbl.Lookup(Key,rData);}
	int GetCount()
		{return m_utbl.GetCount();}
	POSITION GetStart()
		{return m_utbl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_UTBL_UDRD_D& rData)
		{m_utbl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_UTBL_UDRD_D,T_UTBL_UDRD_D&>m_utbl;
};

#endif