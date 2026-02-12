#ifndef __DB_PORDU_DB_H__
#define __DB_PORDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PORDU
{
public:
	CDB_PORDU()
	{
		m_pord.InitHashTable(HASHSIZEPORD);
	}
	virtual ~CDB_PORDU(){};

public:
	void Add(T_UDRD_KEY Key,T_PORD_UDRD_D& rData)
		{m_pord.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_pord.RemoveKey(Key);}
	void DelAll()
		{m_pord.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_PORD_UDRD_D& rData)
		{return m_pord.Lookup(Key,rData);}
	int GetCount()
		{return m_pord.GetCount();}
	POSITION GetStart()
		{return m_pord.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_PORD_UDRD_D& rData)
		{m_pord.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_PORD_UDRD_D,T_PORD_UDRD_D&>m_pord;
};

#endif