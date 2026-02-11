#ifndef __DB_SCGDU_DB_H__
#define __DB_SCGDU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SCGDU
{
public:
	CDB_SCGDU()
	{
		m_scgd.InitHashTable(HASHSIZESCGD);
	}
	virtual ~CDB_SCGDU(){};

public:
	void Add(T_UDRD_KEY Key,T_SCGD_UDRD_D& rData)
	{m_scgd.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_scgd.RemoveKey(Key);}
	void DelAll()
	{m_scgd.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SCGD_UDRD_D& rData)
	{return m_scgd.Lookup(Key,rData);}
	int GetCount()
	{return m_scgd.GetCount();}
	POSITION GetStart()
	{return m_scgd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SCGD_UDRD_D& rData)
	{m_scgd.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SCGD_UDRD_D,T_SCGD_UDRD_D&> m_scgd;
};

#endif
