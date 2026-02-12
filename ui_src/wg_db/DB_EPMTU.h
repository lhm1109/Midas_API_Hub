#ifndef __DB_EPMTU_DB_H__
#define __DB_EPMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_EPMTU
{
public:
	CDB_EPMTU()
	{
		m_epmt.InitHashTable(HASHSIZEEPMT);
	}
	virtual ~CDB_EPMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_EPMT_UDRD_D& rData)
		{m_epmt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_epmt.RemoveKey(Key);}
	void DelAll()
		{m_epmt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_EPMT_UDRD_D& rData)
		{return m_epmt.Lookup(Key,rData);}
	int GetCount()
		{return m_epmt.GetCount();}
	POSITION GetStart()
		{return m_epmt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_EPMT_UDRD_D& rData)
		{m_epmt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_EPMT_UDRD_D,T_EPMT_UDRD_D&>m_epmt;
};

#endif