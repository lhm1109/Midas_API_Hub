#ifndef __DB_THSLU_DB_H__
#define __DB_THSLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THSLU
{
public:
	CDB_THSLU()
	{
		m_thsl.InitHashTable(HASHSIZETHSL);
	}
	virtual ~CDB_THSLU(){};

public:
	void Add(T_UDRD_KEY Key,T_THSL_UDRD_D& rData)
		{m_thsl.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thsl.RemoveKey(Key);}
	void DelAll()
		{m_thsl.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THSL_UDRD_D& rData)
		{return m_thsl.Lookup(Key,rData);}
	int GetCount()
		{return m_thsl.GetCount();}
	POSITION GetStart()
		{return m_thsl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THSL_UDRD_D& rData)
		{m_thsl.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THSL_UDRD_D,T_THSL_UDRD_D&>m_thsl;
};

#endif