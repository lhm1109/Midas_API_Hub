#ifndef __DB_FLUEU_DB_H__
#define __DB_FLUEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FLUEU
{
public:
	CDB_FLUEU()
	{
		m_flue.InitHashTable(HASHSIZEFLUE);
	}
	virtual ~CDB_FLUEU(){};

public:
	void Add(T_UDRD_KEY Key,T_FLUE_UDRD_D& rData)
		{m_flue.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_flue.RemoveKey(Key);}
	void DelAll()
		{m_flue.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FLUE_UDRD_D& rData)
		{return m_flue.Lookup(Key,rData);}
	int GetCount()
		{return m_flue.GetCount();}
	POSITION GetStart()
		{return m_flue.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FLUE_UDRD_D& rData)
		{m_flue.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FLUE_UDRD_D,T_FLUE_UDRD_D&>m_flue;
};

#endif