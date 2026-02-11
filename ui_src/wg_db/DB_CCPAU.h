#ifndef __DB_CCPAU_DB_H__
#define __DB_CCPAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CCPAU
{
public:
	CDB_CCPAU()
	{
		m_ccpa.InitHashTable(HASHSIZECCPA);
	}
	virtual ~CDB_CCPAU(){};

public:
	void Add(T_UDRD_KEY Key,T_CCPA_UDRD_D& rData)
		{m_ccpa.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ccpa.RemoveKey(Key);}
	void DelAll()
		{m_ccpa.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CCPA_UDRD_D& rData)
		{return m_ccpa.Lookup(Key,rData);}
	int GetCount()
		{return m_ccpa.GetCount();}
	POSITION GetStart()
		{return m_ccpa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CCPA_UDRD_D& rData)
		{m_ccpa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CCPA_UDRD_D,T_CCPA_UDRD_D&>m_ccpa;
};

#endif