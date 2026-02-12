#ifndef __DB_HNGTU_DB_H__
#define __DB_HNGTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HNGTU
{
public:
	CDB_HNGTU()
	{
		m_hngt.InitHashTable(HASHSIZEHNGT);
	}
	virtual ~CDB_HNGTU(){};

public:
	void Add(T_UDRD_KEY Key,T_HNGT_UDRD_D& rData)
		{m_hngt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hngt.RemoveKey(Key);}
	void DelAll()
		{m_hngt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HNGT_UDRD_D& rData)
		{return m_hngt.Lookup(Key,rData);}
	int GetCount()
		{return m_hngt.GetCount();}
	POSITION GetStart()
		{return m_hngt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HNGT_UDRD_D& rData)
		{m_hngt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HNGT_UDRD_D,T_HNGT_UDRD_D&>m_hngt;
};

#endif