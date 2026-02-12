#ifndef __DB_LENGU_DB_H__
#define __DB_LENGU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LENGU
{
public:
	CDB_LENGU()
	{
		m_leng.InitHashTable(HASHSIZELENG);
	}
	virtual ~CDB_LENGU(){};

public:
	void Add(T_UDRD_KEY Key,T_LENG_UDRD_D& rData)
		{m_leng.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_leng.RemoveKey(Key);}
	void DelAll()
		{m_leng.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LENG_UDRD_D& rData)
		{return m_leng.Lookup(Key,rData);}
	int GetCount()
		{return m_leng.GetCount();}
	POSITION GetStart()
		{return m_leng.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LENG_UDRD_D& rData)
		{m_leng.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LENG_UDRD_D,T_LENG_UDRD_D&>m_leng;
};

#endif