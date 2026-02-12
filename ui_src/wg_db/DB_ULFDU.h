#ifndef __DB_ULFDU_DB_H__
#define __DB_ULFDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ULFDU
{
public:
	CDB_ULFDU()
	{
		m_ulfd.InitHashTable(HASHSIZEULFD);
	}
	virtual ~CDB_ULFDU(){};

public:
	void Add(T_UDRD_KEY Key,T_ULFD_UDRD_D& rData)
		{m_ulfd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ulfd.RemoveKey(Key);}
	void DelAll()
		{m_ulfd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_ULFD_UDRD_D& rData)
		{return m_ulfd.Lookup(Key,rData);}
	int GetCount()
		{return m_ulfd.GetCount();}
	POSITION GetStart()
		{return m_ulfd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_ULFD_UDRD_D& rData)
		{m_ulfd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_ULFD_UDRD_D,T_ULFD_UDRD_D&>m_ulfd;
};

#endif