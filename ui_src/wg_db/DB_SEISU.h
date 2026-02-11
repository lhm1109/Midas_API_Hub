#ifndef __DB_SEISU_DB_H__
#define __DB_SEISU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SEISU
{
public:
	CDB_SEISU()
	{
		m_seis.InitHashTable(HASHSIZESEIS);
	}
	virtual ~CDB_SEISU(){};

public:
	void Add(T_UDRD_KEY Key,T_SEIS_UDRD_D& rData)
		{m_seis.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_seis.RemoveKey(Key);}
	void DelAll()
		{m_seis.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SEIS_UDRD_D& rData)
		{return m_seis.Lookup(Key,rData);}
	int GetCount()
		{return m_seis.GetCount();}
	POSITION GetStart()
		{return m_seis.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SEIS_UDRD_D& rData)
		{m_seis.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SEIS_UDRD_D,T_SEIS_UDRD_D&>m_seis;
};

#endif