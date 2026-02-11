#ifndef __DB_SMPTU_DB_H__
#define __DB_SMPTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SMPTU
{
public:
	CDB_SMPTU()
	{
		m_smpt.InitHashTable(HASHSIZESMPT);
	}
	virtual ~CDB_SMPTU(){};

public:
	void Add(T_UDRD_KEY Key,T_SMPT_UDRD_D& rData)
		{m_smpt.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_smpt.RemoveKey(Key);}
	void DelAll()
		{m_smpt.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SMPT_UDRD_D& rData)
		{return m_smpt.Lookup(Key,rData);}
	int GetCount()
		{return m_smpt.GetCount();}
	POSITION GetStart()
		{return m_smpt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SMPT_UDRD_D& rData)
		{m_smpt.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_SMPT_UDRD_D,T_SMPT_UDRD_D&>m_smpt;
};

#endif