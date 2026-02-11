#ifndef __DB_SCMTU_DB_H__
#define __DB_SCMTU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_SCMTU
{
public:
	CDB_SCMTU()
	{
		m_SCMT.InitHashTable(HASHSIZESCMT);
	}
	virtual ~CDB_SCMTU(){};

public:
	void Add(T_UDRD_KEY Key,T_SCMT_UDRD_D& rData)
	{m_SCMT.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_SCMT.RemoveKey(Key);}
	void DelAll()
	{m_SCMT.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SCMT_UDRD_D& rData)
	{return m_SCMT.Lookup(Key,rData);}
	int GetCount()
	{return m_SCMT.GetCount();}
	POSITION GetStart()
	{return m_SCMT.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SCMT_UDRD_D& rData)
	{m_SCMT.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SCMT_UDRD_D,T_SCMT_UDRD_D&>m_SCMT;
};

#endif
