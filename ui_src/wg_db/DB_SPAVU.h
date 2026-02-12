#ifndef __DB_SPAVU_DB_H__
#define __DB_SPAVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SPAVU
{
public:
	CDB_SPAVU()
	{
		m_spav.InitHashTable(HASHSIZESPAV);
	}
	virtual ~CDB_SPAVU(){};

public:
	void Add(T_UDRD_KEY Key,T_SPAV_UDRD_D& rData)
		{m_spav.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_spav.RemoveKey(Key);}
	void DelAll()
		{m_spav.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SPAV_UDRD_D& rData)
		{return m_spav.Lookup(Key,rData);}
	int GetCount()
		{return m_spav.GetCount();}
	POSITION GetStart()
		{return m_spav.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SPAV_UDRD_D& rData)
		{m_spav.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SPAV_UDRD_D,T_SPAV_UDRD_D&>m_spav;
};

#endif
