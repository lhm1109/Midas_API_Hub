#ifndef __DB_APHIU_DB_H__
#define __DB_APHIU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_APHIU
{
public:
	CDB_APHIU()
	{
		m_aphi.InitHashTable(HASHSIZEAPHI);
	}
	virtual ~CDB_APHIU(){};

public:
	void Add(T_UDRD_KEY Key,T_APHI_UDRD_D& rData)
		{m_aphi.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_aphi.RemoveKey(Key);}
	void DelAll()
		{m_aphi.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_APHI_UDRD_D& rData)
		{return m_aphi.Lookup(Key,rData);}
	int GetCount()
		{return m_aphi.GetCount();}
	POSITION GetStart()
		{return m_aphi.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_APHI_UDRD_D& rData)
		{m_aphi.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_APHI_UDRD_D,T_APHI_UDRD_D&>m_aphi;
};

#endif