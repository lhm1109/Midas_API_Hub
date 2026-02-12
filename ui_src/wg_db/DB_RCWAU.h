#ifndef __DB_RCWAU_DB_H__
#define __DB_RCWAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCWAU
{
public:
	CDB_RCWAU()
	{
		m_rcwa.InitHashTable(HASHSIZERCWA);
	}
	virtual ~CDB_RCWAU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCWA_UDRD_D& rData)
		{m_rcwa.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rcwa.RemoveKey(Key);}
	void DelAll()
		{m_rcwa.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCWA_UDRD_D& rData)
		{return m_rcwa.Lookup(Key,rData);}
	int GetCount()
		{return m_rcwa.GetCount();}
	POSITION GetStart()
		{return m_rcwa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RCWA_UDRD_D& rData)
		{m_rcwa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCWA_UDRD_D,T_RCWA_UDRD_D&>m_rcwa;
};

#endif