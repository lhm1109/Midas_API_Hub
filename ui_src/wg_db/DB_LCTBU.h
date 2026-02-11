#ifndef __DB_LCTBU_DB_H__
#define __DB_LCTBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LCTBU
{
public:
	CDB_LCTBU()
	{
		m_lctb.InitHashTable(HASHSIZELCTB);
	}
	virtual ~CDB_LCTBU(){};

public:
	void Add(T_UDRD_KEY Key,T_LCTB_UDRD_D& rData)
		{m_lctb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lctb.RemoveKey(Key);}
	void DelAll()
		{m_lctb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LCTB_UDRD_D& rData)
		{return m_lctb.Lookup(Key,rData);}
	int GetCount()
		{return m_lctb.GetCount();}
	POSITION GetStart()
		{return m_lctb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LCTB_UDRD_D& rData)
		{m_lctb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LCTB_UDRD_D,T_LCTB_UDRD_D&>m_lctb;
};

#endif