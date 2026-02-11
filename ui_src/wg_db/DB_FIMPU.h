#ifndef __DB_FIMPU_DB_H__
#define __DB_FIMPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_FIMPU
{
public:
	CDB_FIMPU()
	{
		m_fimp.InitHashTable(HASHSIZEFIMP);
	}
	virtual ~CDB_FIMPU(){};

public:
	void Add(T_UDRD_KEY Key,T_FIMP_UDRD_D& rData)
		{m_fimp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_fimp.RemoveKey(Key);}
	void DelAll()
		{m_fimp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_FIMP_UDRD_D& rData)
		{return m_fimp.Lookup(Key,rData);}
	int GetCount()
		{return m_fimp.GetCount();}
	POSITION GetStart()
		{return m_fimp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_FIMP_UDRD_D& rData)
		{m_fimp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_FIMP_UDRD_D,T_FIMP_UDRD_D&>m_fimp;
};

#endif