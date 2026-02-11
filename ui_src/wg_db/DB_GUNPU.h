#ifndef __DB_GUNPU_DB_H__
#define __DB_GUNPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GUNPU
{
public:
	CDB_GUNPU()
	{
		m_gunp.InitHashTable(HASHSIZEGUNP);
	}
	virtual ~CDB_GUNPU(){};

public:
	void Add(T_UDRD_KEY Key,T_GUNP_UDRD_D& rData)
		{m_gunp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gunp.RemoveKey(Key);}
	void DelAll()
		{m_gunp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GUNP_UDRD_D& rData)
		{return m_gunp.Lookup(Key,rData);}
	int GetCount()
		{return m_gunp.GetCount();}
	POSITION GetStart()
		{return m_gunp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GUNP_UDRD_D& rData)
		{m_gunp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GUNP_UDRD_D,T_GUNP_UDRD_D&>m_gunp;
};

#endif