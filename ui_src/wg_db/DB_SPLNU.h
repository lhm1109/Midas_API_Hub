#ifndef __DB_SPLNU_DB_H__
#define __DB_SPLNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPLNU
{
public:
	CDB_SPLNU()
	{
		m_spln.InitHashTable(HASHSIZESPLN);
	}
	virtual ~CDB_SPLNU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPLN_UDRD_D& rData)
		{m_spln.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_spln.RemoveKey(Key);}
	void DelAll()
		{m_spln.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPLN_UDRD_D& rData)
		{return m_spln.Lookup(Key,rData);}
	int GetCount()
		{return m_spln.GetCount();}
	POSITION GetStart()
		{return m_spln.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPLN_UDRD_D& rData)
		{m_spln.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPLN_UDRD_D,T_SPLN_UDRD_D&>m_spln;
};

#endif