#ifndef __DB_BODFU_DB_H__
#define __DB_BODFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_BODFU
{
public:
	CDB_BODFU()
	{
		m_bodf.InitHashTable(HASHSIZEBODF);
	}
	virtual ~CDB_BODFU(){};

public:
	void Add(T_UDRD_KEY Key,T_BODF_UDRD_D& rData)
		{m_bodf.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_bodf.RemoveKey(Key);}
	void DelAll()
		{m_bodf.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_BODF_UDRD_D& rData)
		{return m_bodf.Lookup(Key,rData);}
	int GetCount()
		{return m_bodf.GetCount();}
	POSITION GetStart()
		{return m_bodf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_BODF_UDRD_D& rData)
		{m_bodf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_BODF_UDRD_D,T_BODF_UDRD_D&>m_bodf;
};

#endif