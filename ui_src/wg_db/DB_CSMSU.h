#ifndef __DB_CSMSU_DB_H__
#define __DB_CSMSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CSMSU
{
public:
	CDB_CSMSU()
	{
		m_csms.InitHashTable(HASHSIZECSMS);
	}
	virtual ~CDB_CSMSU(){};

public:
	void Add(T_UDRD_KEY Key,T_CSMS_UDRD_D& rData)
		{m_csms.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_csms.RemoveKey(Key);}
	void DelAll()
		{m_csms.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CSMS_UDRD_D& rData)
		{return m_csms.Lookup(Key,rData);}
	int GetCount()
		{return m_csms.GetCount();}
	POSITION GetStart()
		{return m_csms.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CSMS_UDRD_D& rData)
		{m_csms.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CSMS_UDRD_D,T_CSMS_UDRD_D&>m_csms;
};

#endif