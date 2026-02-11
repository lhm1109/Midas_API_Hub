#ifndef __DB_MSTRU_DB_H__
#define __DB_MSTRU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MSTRU
{
public:
	CDB_MSTRU()
	{
		m_mstr.InitHashTable(HASHSIZEMSTR);
	}
	virtual ~CDB_MSTRU(){};

public:
	void Add(T_UDRD_KEY Key,T_MSTR_UDRD_D& rData)
		{m_mstr.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mstr.RemoveKey(Key);}
	void DelAll()
		{m_mstr.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MSTR_UDRD_D& rData)
		{return m_mstr.Lookup(Key,rData);}
	int GetCount()
		{return m_mstr.GetCount();}
	POSITION GetStart()
		{return m_mstr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MSTR_UDRD_D& rData)
		{m_mstr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MSTR_UDRD_D,T_MSTR_UDRD_D&>m_mstr;
};

#endif