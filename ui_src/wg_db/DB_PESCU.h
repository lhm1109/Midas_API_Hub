#ifndef __DB_PESCU_DB_H__
#define __DB_PESCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PESCU
{
public:
	CDB_PESCU()
	{
		m_pesc.InitHashTable(HASHSIZEPESC);
	}
	virtual ~CDB_PESCU(){};

public:
	void Add(T_UDRD_KEY Key,T_PESC_UDRD_D& rData)
		{m_pesc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pesc.RemoveKey(Key);}
	void DelAll()
		{m_pesc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PESC_UDRD_D& rData)
		{return m_pesc.Lookup(Key,rData);}
	int GetCount()
		{return m_pesc.GetCount();}
	POSITION GetStart()
		{return m_pesc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PESC_UDRD_D& rData)
		{m_pesc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PESC_UDRD_D,T_PESC_UDRD_D&>m_pesc;
};

#endif