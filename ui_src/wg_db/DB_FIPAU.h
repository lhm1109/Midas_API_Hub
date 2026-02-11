#ifndef __DB_FIPAU_DB_H__
#define __DB_FIPAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FIPAU
{
public:
	CDB_FIPAU()
	{
		m_fipa.InitHashTable(HASHSIZEFIPA);
	}
	virtual ~CDB_FIPAU(){};

public:
	void Add(T_UDRD_KEY Key,T_FIPA_UDRD_D& rData)
		{m_fipa.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fipa.RemoveKey(Key);}
	void DelAll()
		{m_fipa.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FIPA_UDRD_D& rData)
		{return m_fipa.Lookup(Key,rData);}
	int GetCount()
		{return m_fipa.GetCount();}
	POSITION GetStart()
		{return m_fipa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FIPA_UDRD_D& rData)
		{m_fipa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FIPA_UDRD_D,T_FIPA_UDRD_D&>m_fipa;
};

#endif