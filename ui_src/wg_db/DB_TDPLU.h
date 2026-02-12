#ifndef __DB_TDPLU_DB_H__
#define __DB_TDPLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDPLU
{
public:
	CDB_TDPLU()
	{
		m_tdpl.InitHashTable(HASHSIZETDPL);
	}
	virtual ~CDB_TDPLU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDPL_UDRD_D& rData)
		{m_tdpl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdpl.RemoveKey(Key);}
	void DelAll()
		{m_tdpl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDPL_UDRD_D& rData)
		{return m_tdpl.Lookup(Key,rData);}
	int GetCount()
		{return m_tdpl.GetCount();}
	POSITION GetStart()
		{return m_tdpl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDPL_UDRD_D& rData)
		{m_tdpl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDPL_UDRD_D,T_TDPL_UDRD_D&>m_tdpl;
};

#endif