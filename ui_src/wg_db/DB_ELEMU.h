#ifndef __DB_ELEMU_DB_H__
#define __DB_ELEMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEMU
{
public:
	CDB_ELEMU()
	{
		m_elem.InitHashTable(HASHSIZEELEM);
	}
	virtual ~CDB_ELEMU(){};

public:
	void Add(T_UDRD_KEY Key,T_ELEM_UDRD_D& rData)
		{m_elem.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_elem.RemoveKey(Key);}
	void DelAll()
		{m_elem.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ELEM_UDRD_D& rData)
		{return m_elem.Lookup(Key,rData);}
	int GetCount()
		{return m_elem.GetCount();}
	POSITION GetStart()
		{return m_elem.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ELEM_UDRD_D& rData)
		{m_elem.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ELEM_UDRD_D,T_ELEM_UDRD_D&>m_elem;
};

#endif