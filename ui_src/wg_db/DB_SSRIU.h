#ifndef __DB_SSRIU_DB_H__
#define __DB_SSRIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SSRIU
{
public:
	CDB_SSRIU()
	{
		m_ssri.InitHashTable(HASHSIZESSRI);
	}
	virtual ~CDB_SSRIU(){};

public:
	void Add(T_UDRD_KEY Key,T_SSRI_UDRD_D& rData)
		{m_ssri.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ssri.RemoveKey(Key);}
	void DelAll()
		{m_ssri.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SSRI_UDRD_D& rData)
		{return m_ssri.Lookup(Key,rData);}
	int GetCount()
		{return m_ssri.GetCount();}
	POSITION GetStart()
		{return m_ssri.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SSRI_UDRD_D& rData)
		{m_ssri.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SSRI_UDRD_D,T_SSRI_UDRD_D&>m_ssri;
};

#endif