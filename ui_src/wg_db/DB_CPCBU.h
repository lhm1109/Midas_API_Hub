#ifndef __DB_CPCBU_DB_H__
#define __DB_CPCBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPCBU
{
public:
	CDB_CPCBU()
	{
		m_cpcb.InitHashTable(HASHSIZECPCB);
	}
	virtual ~CDB_CPCBU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPCB_UDRD_D& rData)
		{m_cpcb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cpcb.RemoveKey(Key);}
	void DelAll()
		{m_cpcb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPCB_UDRD_D& rData)
		{return m_cpcb.Lookup(Key,rData);}
	int GetCount()
		{return m_cpcb.GetCount();}
	POSITION GetStart()
		{return m_cpcb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPCB_UDRD_D& rData)
		{m_cpcb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPCB_UDRD_D,T_CPCB_UDRD_D&>m_cpcb;
};

#endif