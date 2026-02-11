#ifndef __DB_TDGRU_DB_H__
#define __DB_TDGRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDGRU
{
public:
	CDB_TDGRU()
	{
		m_tdgr.InitHashTable(HASHSIZETDGR);
	}
	virtual ~CDB_TDGRU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDGR_UDRD_D& rData)
		{m_tdgr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdgr.RemoveKey(Key);}
	void DelAll()
		{m_tdgr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDGR_UDRD_D& rData)
		{return m_tdgr.Lookup(Key,rData);}
	int GetCount()
		{return m_tdgr.GetCount();}
	POSITION GetStart()
		{return m_tdgr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDGR_UDRD_D& rData)
		{m_tdgr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDGR_UDRD_D,T_TDGR_UDRD_D&>m_tdgr;
};

#endif