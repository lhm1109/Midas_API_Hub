#ifndef __DB_CRSTU_DB_H__
#define __DB_CRSTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CRSTU
{
public:
	CDB_CRSTU()
	{
		m_crst.InitHashTable(HASHSIZECRST);
	}
	virtual ~CDB_CRSTU(){};

public:
	void Add(T_UDRD_KEY Key,T_CRST_UDRD_D& rData)
		{m_crst.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_crst.RemoveKey(Key);}
	void DelAll()
		{m_crst.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CRST_UDRD_D& rData)
		{return m_crst.Lookup(Key,rData);}
	int GetCount()
		{return m_crst.GetCount();}
	POSITION GetStart()
		{return m_crst.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CRST_UDRD_D& rData)
		{m_crst.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CRST_UDRD_D,T_CRST_UDRD_D&>m_crst;
};

#endif