#ifndef __DB_SERVU_DB_H__
#define __DB_SERVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SERVU
{
public:
	CDB_SERVU()
	{
		m_serv.InitHashTable(HASHSIZESERV);
	}
	virtual ~CDB_SERVU(){};

public:
	void Add(T_UDRD_KEY Key,T_SERV_UDRD_D& rData)
		{m_serv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_serv.RemoveKey(Key);}
	void DelAll()
		{m_serv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SERV_UDRD_D& rData)
		{return m_serv.Lookup(Key,rData);}
	int GetCount()
		{return m_serv.GetCount();}
	POSITION GetStart()
		{return m_serv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SERV_UDRD_D& rData)
		{m_serv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SERV_UDRD_D,T_SERV_UDRD_D&>m_serv;
};

#endif