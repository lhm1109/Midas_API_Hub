#ifndef __DB_MRFTU_DB_H__
#define __DB_MRFTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MRFTU
{
public:
	CDB_MRFTU()
	{
		m_mrft.InitHashTable(HASHSIZEMRFT);
	}
	virtual ~CDB_MRFTU(){};

public:
	void Add(T_UDRD_KEY Key,T_MRFT_UDRD_D& rData)
		{m_mrft.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mrft.RemoveKey(Key);}
	void DelAll()
		{m_mrft.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MRFT_UDRD_D& rData)
		{return m_mrft.Lookup(Key,rData);}
	int GetCount()
		{return m_mrft.GetCount();}
	POSITION GetStart()
		{return m_mrft.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MRFT_UDRD_D& rData)
		{m_mrft.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MRFT_UDRD_D,T_MRFT_UDRD_D&>m_mrft;
};

#endif