#ifndef __DB_JDFDU_DB_H__
#define __DB_JDFDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_JDFDU
{
public:
	CDB_JDFDU()
	{
		m_JDFD.InitHashTable(HASHSIZEJDFD);
	}
	virtual ~CDB_JDFDU(){};

public:
	void Add(T_UDRD_KEY Key,T_JDFD_UDRD_D& rData)
		{m_JDFD.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_JDFD.RemoveKey(Key);}
	void DelAll()
		{m_JDFD.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_JDFD_UDRD_D& rData)
		{return m_JDFD.Lookup(Key,rData);}
	int GetCount()
		{return m_JDFD.GetCount();}
	POSITION GetStart()
		{return m_JDFD.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_JDFD_UDRD_D& rData)
		{m_JDFD.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_JDFD_UDRD_D,T_JDFD_UDRD_D&>m_JDFD;
};

#endif