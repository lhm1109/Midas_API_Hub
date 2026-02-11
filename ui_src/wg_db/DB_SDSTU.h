#ifndef __DB_SDSTU_DB_H__
#define __DB_SDSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SDSTU
{
public:
	CDB_SDSTU()
	{
		m_sdst.InitHashTable(HASHSIZESDST);
	}
	virtual ~CDB_SDSTU(){};

public:
	void Add(T_UDRD_KEY Key,T_SDST_UDRD_D& rData)
		{m_sdst.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_sdst.RemoveKey(Key);}
	void DelAll()
		{m_sdst.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SDST_UDRD_D& rData)
		{return m_sdst.Lookup(Key,rData);}
	int GetCount()
		{return m_sdst.GetCount();}
	POSITION GetStart()
		{return m_sdst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SDST_UDRD_D& rData)
		{m_sdst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SDST_UDRD_D,T_SDST_UDRD_D&>m_sdst;
};

#endif