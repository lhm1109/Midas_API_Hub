#ifndef __DB_RBSLU_DB_H__
#define __DB_RBSLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RBSLU
{
public:
	CDB_RBSLU()
	{
		m_rbsl.InitHashTable(HASHSIZERBSL);
	}
	virtual ~CDB_RBSLU(){};

public:
	void Add(T_UDRD_KEY Key,T_RBSL_UDRD_D& rData)
		{m_rbsl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rbsl.RemoveKey(Key);}
	void DelAll()
		{m_rbsl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RBSL_UDRD_D& rData)
		{return m_rbsl.Lookup(Key,rData);}
	int GetCount()
		{return m_rbsl.GetCount();}
	POSITION GetStart()
		{return m_rbsl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RBSL_UDRD_D& rData)
		{m_rbsl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RBSL_UDRD_D,T_RBSL_UDRD_D&>m_rbsl;
};

#endif