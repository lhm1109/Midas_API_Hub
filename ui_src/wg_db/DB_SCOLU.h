#ifndef __DB_SCOLU_DB_H__
#define __DB_SCOLU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SCOLU
{
public:
	CDB_SCOLU()
	{
		m_scol.InitHashTable(HASHSIZESCOL);
	}
	virtual ~CDB_SCOLU(){};

public:
	void Add(T_UDRD_KEY Key,T_SCOL_UDRD_D& rData)
	{m_scol.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
	{return m_scol.RemoveKey(Key);}
	void DelAll()
	{m_scol.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_SCOL_UDRD_D& rData)
	{return m_scol.Lookup(Key,rData);}
	int GetCount()
	{return m_scol.GetCount();}
	POSITION GetStart()
	{return m_scol.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_SCOL_UDRD_D& rData)
	{m_scol.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_UDRD_KEY,T_UDRD_KEY,T_SCOL_UDRD_D,T_SCOL_UDRD_D&>m_scol;
};

#endif