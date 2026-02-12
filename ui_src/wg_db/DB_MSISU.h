#ifndef __DB_MSISU_DB_H__
#define __DB_MSISU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MSISU
{
public:
	CDB_MSISU()
	{
		m_msis.InitHashTable(HASHSIZEMSIS);
	}
	virtual ~CDB_MSISU(){};

public:
	void Add(T_UDRD_KEY Key,T_MSIS_UDRD_D& rData)
		{m_msis.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_msis.RemoveKey(Key);}
	void DelAll()
		{m_msis.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MSIS_UDRD_D& rData)
		{return m_msis.Lookup(Key,rData);}
	int GetCount()
		{return m_msis.GetCount();}
	POSITION GetStart()
		{return m_msis.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MSIS_UDRD_D& rData)
		{m_msis.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MSIS_UDRD_D,T_MSIS_UDRD_D&>m_msis;
};

#endif