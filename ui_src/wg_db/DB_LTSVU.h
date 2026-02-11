#ifndef __DB_LTSVU_DB_H__
#define __DB_LTSVU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_LTSVU
{
public:
	CDB_LTSVU()
	{
		m_ltsv.InitHashTable(HASHSIZELTSV);
	}
	virtual ~CDB_LTSVU(){};

public:
	void Add(T_UDRD_KEY Key,T_LTSV_UDRD_D& rData)
		{m_ltsv.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_ltsv.RemoveKey(Key);}
	void DelAll()
		{m_ltsv.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_LTSV_UDRD_D& rData)
		{return m_ltsv.Lookup(Key,rData);}
	int GetCount()
		{return m_ltsv.GetCount();}
	POSITION GetStart()
		{return m_ltsv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_LTSV_UDRD_D& rData)
		{m_ltsv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_LTSV_UDRD_D,T_LTSV_UDRD_D&>m_ltsv;
};

#endif