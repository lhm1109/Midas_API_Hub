#ifndef __DB_TMATU_DB_H__
#define __DB_TMATU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TMATU
{
public:
	CDB_TMATU()
	{
		m_tmat.InitHashTable(HASHSIZETMAT);
	}
	virtual ~CDB_TMATU(){};

public:
	void Add(T_UDRD_KEY Key,T_TMAT_UDRD_D& rData)
		{m_tmat.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tmat.RemoveKey(Key);}
	void DelAll()
		{m_tmat.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TMAT_UDRD_D& rData)
		{return m_tmat.Lookup(Key,rData);}
	int GetCount()
		{return m_tmat.GetCount();}
	POSITION GetStart()
		{return m_tmat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TMAT_UDRD_D& rData)
		{m_tmat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TMAT_UDRD_D,T_TMAT_UDRD_D&>m_tmat;
};

#endif