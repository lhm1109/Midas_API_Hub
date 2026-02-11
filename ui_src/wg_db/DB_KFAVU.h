#ifndef __DB_KFAVU_DB_H__
#define __DB_KFAVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_KFAVU
{
public:
	CDB_KFAVU()
	{
		m_kfav.InitHashTable(HASHSIZEKFAV);
	}
	virtual ~CDB_KFAVU(){};

public:
	void Add(T_UDRD_KEY Key,T_KFAV_UDRD_D& rData)
		{m_kfav.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_kfav.RemoveKey(Key);}
	void DelAll()
		{m_kfav.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_KFAV_UDRD_D& rData)
		{return m_kfav.Lookup(Key,rData);}
	int GetCount()
		{return m_kfav.GetCount();}
	POSITION GetStart()
		{return m_kfav.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_KFAV_UDRD_D& rData)
		{m_kfav.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_KFAV_UDRD_D,T_KFAV_UDRD_D&>m_kfav;
};

#endif