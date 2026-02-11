#ifndef __DB_SSEIU_DB_H__
#define __DB_SSEIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SSEIU
{
public:
	CDB_SSEIU()
	{
		m_ssei.InitHashTable(HASHSIZESSEI);
	}
	virtual ~CDB_SSEIU(){};

public:
	void Add(T_UDRD_KEY Key,T_SSEI_UDRD_D& rData)
		{m_ssei.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ssei.RemoveKey(Key);}
	void DelAll()
		{m_ssei.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SSEI_UDRD_D& rData)
		{return m_ssei.Lookup(Key,rData);}
	int GetCount()
		{return m_ssei.GetCount();}
	POSITION GetStart()
		{return m_ssei.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SSEI_UDRD_D& rData)
		{m_ssei.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SSEI_UDRD_D,T_SSEI_UDRD_D&>m_ssei;
};

#endif