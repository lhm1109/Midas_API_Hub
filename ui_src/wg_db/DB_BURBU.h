#ifndef __DB_BURBU_DB_H__
#define __DB_BURBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BURBU
{
public:
	CDB_BURBU()
	{
		m_burb.InitHashTable(HASHSIZEBURB);
	}
	virtual ~CDB_BURBU(){};

public:
	void Add(T_UDRD_KEY Key,T_BURB_UDRD_D& rData)
		{m_burb.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_burb.RemoveKey(Key);}
	void DelAll()
		{m_burb.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BURB_UDRD_D& rData)
		{return m_burb.Lookup(Key,rData);}
	int GetCount()
		{return m_burb.GetCount();}
	POSITION GetStart()
		{return m_burb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BURB_UDRD_D& rData)
		{m_burb.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BURB_UDRD_D,T_BURB_UDRD_D&>m_burb;
};

#endif