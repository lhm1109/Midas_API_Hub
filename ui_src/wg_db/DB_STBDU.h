#ifndef __DB_STBDU_DB_H__
#define __DB_STBDU_DB_H__ 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_STBDU
{
public:
	CDB_STBDU()
	{
		m_stbd.InitHashTable(HASHSIZESTBD);
	}
	virtual ~CDB_STBDU(){};

public:
	void Add(T_UDRD_KEY Key,T_STBD_UDRD_D& rData)
		{m_stbd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_stbd.RemoveKey(Key);}
	void DelAll()
		{m_stbd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_STBD_UDRD_D& rData)
		{return m_stbd.Lookup(Key,rData);}
	int GetCount()
		{return m_stbd.GetCount();}
	POSITION GetStart()
		{return m_stbd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_STBD_UDRD_D& rData)
		{m_stbd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_STBD_UDRD_D,T_STBD_UDRD_D&>m_stbd;
};

#endif