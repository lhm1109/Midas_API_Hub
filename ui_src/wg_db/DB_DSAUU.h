#ifndef __DB_DSAUU_DB_H__
#define __DB_DSAUU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DSAUU
{
public:
	CDB_DSAUU()
	{
		m_dsau.InitHashTable(HASHSIZEDSAU);
	}
	virtual ~CDB_DSAUU(){};

public:
	void Add(T_UDRD_KEY Key,T_DSAU_UDRD_D& rData)
		{m_dsau.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_dsau.RemoveKey(Key);}
	void DelAll()
		{m_dsau.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_DSAU_UDRD_D& rData)
		{return m_dsau.Lookup(Key,rData);}
	int GetCount()
		{return m_dsau.GetCount();}
	POSITION GetStart()
		{return m_dsau.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DSAU_UDRD_D& rData)
		{m_dsau.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DSAU_UDRD_D,T_DSAU_UDRD_D&>m_dsau;
};

#endif