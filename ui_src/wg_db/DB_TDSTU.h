#ifndef __DB_TDSTU_DB_H__
#define __DB_TDSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_DESIGN.H"

class CDB_TDSTU
{
public:
	CDB_TDSTU()
	{
		m_tdst.InitHashTable(HASHSIZETDST);
	}
	virtual ~CDB_TDSTU(){};

public:
	void Add(T_UDRD_KEY Key, T_TDST_UDRD_D& rData)
		{m_tdst.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdst.RemoveKey(Key);}
	void DelAll()
		{m_tdst.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key, T_TDST_UDRD_D& rData)
		{return m_tdst.Lookup(Key,rData);}
	int GetCount()
		{return m_tdst.GetCount();}
	POSITION GetStart()
		{return m_tdst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_UDRD_KEY& rKey, T_TDST_UDRD_D& rData)
		{m_tdst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_TDST_UDRD_D, T_TDST_UDRD_D&> m_tdst;
};

#endif