#ifndef __DB_GSTYU_DB_H__
#define __DB_GSTYU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GSTYU
{
public:
	CDB_GSTYU()
	{
		m_gsty.InitHashTable(HASHSIZEGSTY);
	}
	virtual ~CDB_GSTYU(){};

public:
	void Add(T_UDRD_KEY Key,T_GSTY_UDRD_D& rData)
		{m_gsty.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_gsty.RemoveKey(Key);}
	void DelAll()
		{m_gsty.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_GSTY_UDRD_D& rData)
		{return m_gsty.Lookup(Key,rData);}
	int GetCount()
		{return m_gsty.GetCount();}
	POSITION GetStart()
		{return m_gsty.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GSTY_UDRD_D& rData)
		{m_gsty.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GSTY_UDRD_D,T_GSTY_UDRD_D&>m_gsty;
};

#endif