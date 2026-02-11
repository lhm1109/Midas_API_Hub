#ifndef __DB_TSGRU_DB_H__
#define __DB_TSGRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TSGRU
{
public:
	CDB_TSGRU()
	{
		m_tsgr.InitHashTable(HASHSIZETSGR);
	}
	virtual ~CDB_TSGRU(){};

public:
	void Add(T_UDRD_KEY Key,T_TSGR_UDRD_D& rData)
		{m_tsgr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tsgr.RemoveKey(Key);}
	void DelAll()
		{m_tsgr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TSGR_UDRD_D& rData)
		{return m_tsgr.Lookup(Key,rData);}
	int GetCount()
		{return m_tsgr.GetCount();}
	POSITION GetStart()
		{return m_tsgr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TSGR_UDRD_D& rData)
		{m_tsgr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TSGR_UDRD_D,T_TSGR_UDRD_D&>m_tsgr;
};

#endif