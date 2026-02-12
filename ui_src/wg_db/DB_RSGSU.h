#ifndef __DB_RSGSU_DB_H__
#define __DB_RSGSU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RSGSU
{
public:
	CDB_RSGSU()
	{
		m_rsgs.InitHashTable(HASHSIZERSGS);
	}
	virtual ~CDB_RSGSU(){};

public:
	void Add(T_UDRD_KEY Key,T_RSGS_UDRD_D& rData)
		{m_rsgs.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rsgs.RemoveKey(Key);}
	void DelAll()
		{m_rsgs.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RSGS_UDRD_D& rData)
		{return m_rsgs.Lookup(Key,rData);}
	int GetCount()
		{return m_rsgs.GetCount();}
	POSITION GetStart()
		{return m_rsgs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RSGS_UDRD_D& rData)
		{m_rsgs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RSGS_UDRD_D,T_RSGS_UDRD_D&>m_rsgs;
};

#endif