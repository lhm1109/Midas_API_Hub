#ifndef __DB_THSFU_DB_H__
#define __DB_THSFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_THSFU
{
public:
	CDB_THSFU()
	{
		m_thsf.InitHashTable(HASHSIZETHSF);
	}
	virtual ~CDB_THSFU(){};

public:
	void Add(T_UDRD_KEY Key,T_THSF_UDRD_D& rData)
		{m_thsf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_thsf.RemoveKey(Key);}
	void DelAll()
		{m_thsf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_THSF_UDRD_D& rData)
		{return m_thsf.Lookup(Key,rData);}
	int GetCount()
		{return m_thsf.GetCount();}
	POSITION GetStart()
		{return m_thsf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_THSF_UDRD_D& rData)
		{m_thsf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_THSF_UDRD_D,T_THSF_UDRD_D&>m_thsf;
};

#endif