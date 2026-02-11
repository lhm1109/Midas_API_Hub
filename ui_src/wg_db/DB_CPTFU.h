#ifndef __DB_CPTFU_DB_H__
#define __DB_CPTFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_CPTFU
{
public:
	CDB_CPTFU()
	{
		m_cptf.InitHashTable(HASHSIZECPTF);
	}
	virtual ~CDB_CPTFU(){};

public:
	void Add(T_UDRD_KEY Key,T_CPTF_UDRD_D& rData)
		{m_cptf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_cptf.RemoveKey(Key);}
	void DelAll()
		{m_cptf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_CPTF_UDRD_D& rData)
		{return m_cptf.Lookup(Key,rData);}
	int GetCount()
		{return m_cptf.GetCount();}
	POSITION GetStart()
		{return m_cptf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_CPTF_UDRD_D& rData)
		{m_cptf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_CPTF_UDRD_D,T_CPTF_UDRD_D&>m_cptf;
};

#endif