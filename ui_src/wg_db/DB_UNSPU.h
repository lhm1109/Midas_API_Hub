#ifndef __DB_UNSPU_DB_H__
#define __DB_UNSPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_UNSPU
{
public:
	CDB_UNSPU()
	{
		m_unsp.InitHashTable(HASHSIZEUNSP);
	}
	virtual ~CDB_UNSPU(){};

public:
	void Add(T_UNSP_K Key,T_UNSP_UDRD_D& rData)
		{m_unsp.SetAt(Key,rData);}
	BOOL Del(T_UNSP_K Key)
		{return m_unsp.RemoveKey(Key);}
	void DelAll()
		{m_unsp.RemoveAll();}
	BOOL Get(T_UNSP_K Key,T_UNSP_UDRD_D& rData)
		{return m_unsp.Lookup(Key,rData);}
	int GetCount()
		{return m_unsp.GetCount();}
	POSITION GetStart()
		{return m_unsp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UNSP_K& rKey,T_UNSP_UDRD_D& rData)
		{m_unsp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UNSP_K,T_UNSP_K,T_UNSP_UDRD_D,T_UNSP_UDRD_D&>m_unsp;
};

#endif