#ifndef __DB_PNLDU_DB_H__
#define __DB_PNLDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PNLDU
{
public:
	CDB_PNLDU()
	{
		m_pnld.InitHashTable(HASHSIZEPNLD);
	}
	virtual ~CDB_PNLDU(){};

public:
	void Add(T_UDRD_KEY Key,T_PNLD_UDRD_D& rData)
		{m_pnld.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pnld.RemoveKey(Key);}
	void DelAll()
		{m_pnld.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PNLD_UDRD_D& rData)
		{return m_pnld.Lookup(Key,rData);}
	int GetCount()
		{return m_pnld.GetCount();}
	POSITION GetStart()
		{return m_pnld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PNLD_UDRD_D& rData)
		{m_pnld.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PNLD_UDRD_D,T_PNLD_UDRD_D&>m_pnld;
};

#endif