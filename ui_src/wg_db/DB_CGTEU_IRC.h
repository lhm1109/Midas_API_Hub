#ifndef __DB_CGTEU_IRC_DB_H__
#define __DB_CGTEU_IRC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CGTEU_IRC
{
public:
	CDB_CGTEU_IRC()
	{
		m_cgte_IRC.InitHashTable(HASHSIZE_IRC_CGTE);
	}
	virtual ~CDB_CGTEU_IRC(){};

public:
	void Add(T_UDRD_KEY Key,T_CGTE_UDRD_IRC_D& rData)
		{m_cgte_IRC.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cgte_IRC.RemoveKey(Key);}
	void DelAll()
		{m_cgte_IRC.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CGTE_UDRD_IRC_D& rData)
		{return m_cgte_IRC.Lookup(Key,rData);}
	int GetCount()
		{return m_cgte_IRC.GetCount();}
	POSITION GetStart()
		{return m_cgte_IRC.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CGTE_UDRD_IRC_D& rData)
		{m_cgte_IRC.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CGTE_UDRD_IRC_D,T_CGTE_UDRD_IRC_D&>m_cgte_IRC;
};

#endif