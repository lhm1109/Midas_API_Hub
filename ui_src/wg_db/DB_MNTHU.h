#ifndef __DB_MNTHU_DB_H__
#define __DB_MNTHU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
	 
class CDB_MNTHU
{
public:
	CDB_MNTHU()
	{
		m_mnth.InitHashTable(HASHSIZEMNTH);
	}
	virtual ~CDB_MNTHU(){};

public:   
	void Add(T_UDRD_KEY Key,T_MNTH_UDRD_D& rData)
		{m_mnth.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mnth.RemoveKey(Key);}
	void DelAll()
		{m_mnth.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MNTH_UDRD_D& rData)
		{return m_mnth.Lookup(Key,rData);}
	int GetCount()
		{return m_mnth.GetCount();}
	POSITION GetStart()
		{return m_mnth.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MNTH_UDRD_D& rData)
		{m_mnth.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MNTH_UDRD_D,T_MNTH_UDRD_D&>m_mnth;
};

#endif
