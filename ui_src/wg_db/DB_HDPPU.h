#ifndef __DB_HDPPU_DB_H__
#define __DB_HDPPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_HDPPU
{
public:
	CDB_HDPPU()
	{
		m_hdpp.InitHashTable(HASHSIZEHDPP);
	}
	virtual ~CDB_HDPPU(){};

public:
	void Add(T_UDRD_KEY Key,T_HDPP_UDRD_D& rData)
		{m_hdpp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_hdpp.RemoveKey(Key);}
	void DelAll()
		{m_hdpp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_HDPP_UDRD_D& rData)
		{return m_hdpp.Lookup(Key,rData);}
	int GetCount()
		{return m_hdpp.GetCount();}
	POSITION GetStart()
		{return m_hdpp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_HDPP_UDRD_D& rData)
		{m_hdpp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_HDPP_UDRD_D,T_HDPP_UDRD_D&>m_hdpp;
};

#endif