#ifndef __DB_CSCSU_DB_H__
#define __DB_CSCSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CSCSU
{
public:
	CDB_CSCSU()
	{
		m_cscs.InitHashTable(HASHSIZECSCS);
	}
	virtual ~CDB_CSCSU(){};

public:
	void Add(T_UDRD_KEY Key,T_CSCS_UDRD_D& rData)
		{m_cscs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cscs.RemoveKey(Key);}
	void DelAll()
		{m_cscs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CSCS_UDRD_D& rData)
		{return m_cscs.Lookup(Key,rData);}
	int GetCount()
		{return m_cscs.GetCount();}
	POSITION GetStart()
		{return m_cscs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CSCS_UDRD_D& rData)
		{m_cscs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CSCS_UDRD_D,T_CSCS_UDRD_D&>m_cscs;
};

#endif