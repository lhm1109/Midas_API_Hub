#ifndef __DB_TDMFU_DB_H__
#define __DB_TDMFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TDMFU
{
public:
	CDB_TDMFU()
	{
		m_tdmf.InitHashTable(HASHSIZETDMF);
	}
	virtual ~CDB_TDMFU(){};

public:
	void Add(T_UDRD_KEY Key,T_TDMF_UDRD_D& rData)
		{m_tdmf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_tdmf.RemoveKey(Key);}
	void DelAll()
		{m_tdmf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_TDMF_UDRD_D& rData)
		{return m_tdmf.Lookup(Key,rData);}
	int GetCount()
		{return m_tdmf.GetCount();}
	POSITION GetStart()
		{return m_tdmf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_TDMF_UDRD_D& rData)
		{m_tdmf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_TDMF_UDRD_D,T_TDMF_UDRD_D&>m_tdmf;
};

#endif