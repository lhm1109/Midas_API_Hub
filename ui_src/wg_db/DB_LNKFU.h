#ifndef __DB_LNKFU_DB_H__
#define __DB_LNKFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LNKFU
{
public:
	CDB_LNKFU()
	{
		m_lnkf.InitHashTable(HASHSIZELNKF);
	}
	virtual ~CDB_LNKFU(){};

public:
	void Add(T_UDRD_KEY Key,T_LNKF_UDRD_D& rData)
		{m_lnkf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_lnkf.RemoveKey(Key);}
	void DelAll()
		{m_lnkf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LNKF_UDRD_D& rData)
		{return m_lnkf.Lookup(Key,rData);}
	int GetCount()
		{return m_lnkf.GetCount();}
	POSITION GetStart()
		{return m_lnkf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LNKF_UDRD_D& rData)
		{m_lnkf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LNKF_UDRD_D,T_LNKF_UDRD_D&>m_lnkf;
};

#endif