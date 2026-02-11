#ifndef __DB_DORCU_DB_H__
#define __DB_DORCU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DORCU
{
public:
	CDB_DORCU()
	{
		m_dorc.InitHashTable(HASHSIZEDORC);
	}
	virtual ~CDB_DORCU(){};

public:
	void Add(T_UDRD_KEY Key,T_DORC_UDRD_D& rData)
		{m_dorc.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dorc.RemoveKey(Key);}
	void DelAll()
		{m_dorc.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DORC_UDRD_D& rData)
		{return m_dorc.Lookup(Key,rData);}
	int GetCount()
		{return m_dorc.GetCount();}
	POSITION GetStart()
		{return m_dorc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DORC_UDRD_D& rData)
		{m_dorc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DORC_UDRD_D,T_DORC_UDRD_D&>m_dorc;
};

#endif