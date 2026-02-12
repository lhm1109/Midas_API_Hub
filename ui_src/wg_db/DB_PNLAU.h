#ifndef __DB_PNLAU_DB_H__
#define __DB_PNLAU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PNLAU
{
public:
	CDB_PNLAU()
	{
		m_pnla.InitHashTable(HASHSIZEPNLA);
	}
	virtual ~CDB_PNLAU(){};

public:
	void Add(T_UDRD_KEY Key,T_PNLA_UDRD_D& rData)
		{m_pnla.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pnla.RemoveKey(Key);}
	void DelAll()
		{m_pnla.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PNLA_UDRD_D& rData)
		{return m_pnla.Lookup(Key,rData);}
	int GetCount()
		{return m_pnla.GetCount();}
	POSITION GetStart()
		{return m_pnla.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PNLA_UDRD_D& rData)
		{m_pnla.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PNLA_UDRD_D,T_PNLA_UDRD_D&>m_pnla;
};

#endif