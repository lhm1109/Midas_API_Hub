#ifndef __DB_MLSPU_DB_H__
#define __DB_MLSPU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MLSPU
{
public:
	CDB_MLSPU()
	{
		m_mlsp.InitHashTable(HASHSIZEMLSP);
	}
	virtual ~CDB_MLSPU(){};

public:
	void Add(T_UDRD_KEY Key,T_MLSP_UDRD_D& rData)
		{m_mlsp.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_mlsp.RemoveKey(Key);}
	void DelAll()
		{m_mlsp.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_MLSP_UDRD_D& rData)
		{return m_mlsp.Lookup(Key,rData);}
	int GetCount()
		{return m_mlsp.GetCount();}
	POSITION GetStart()
		{return m_mlsp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_MLSP_UDRD_D& rData)
		{m_mlsp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_MLSP_UDRD_D,T_MLSP_UDRD_D&>m_mlsp;
};

#endif