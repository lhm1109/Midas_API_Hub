#ifndef __DB_RBMSU_DB_H__
#define __DB_RBMSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RBMSU
{
public:
	CDB_RBMSU()
	{
		m_rbms.InitHashTable(HASHSIZERBMS);
	}
	virtual ~CDB_RBMSU(){};

public:
	void Add(T_UDRD_KEY Key,T_RBMS_UDRD_D& rData)
		{m_rbms.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rbms.RemoveKey(Key);}
	void DelAll()
		{m_rbms.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RBMS_UDRD_D& rData)
		{return m_rbms.Lookup(Key,rData);}
	int GetCount()
		{return m_rbms.GetCount();}
	POSITION GetStart()
		{return m_rbms.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RBMS_UDRD_D& rData)
		{m_rbms.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RBMS_UDRD_D,T_RBMS_UDRD_D&>m_rbms;
};

#endif