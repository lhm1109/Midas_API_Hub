#ifndef __DB_RCCOU_DB_H__
#define __DB_RCCOU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RCCOU
{
public:
	CDB_RCCOU()
	{
		m_rcco.InitHashTable(HASHSIZERCCO);
	}
	virtual ~CDB_RCCOU(){};

public:
	void Add(T_UDRD_KEY Key,T_RCCO_UDRD_D& rData)
		{m_rcco.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rcco.RemoveKey(Key);}
	void DelAll()
		{m_rcco.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RCCO_UDRD_D& rData)
		{return m_rcco.Lookup(Key,rData);}
	int GetCount()
		{return m_rcco.GetCount();}
	POSITION GetStart()
		{return m_rcco.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RCCO_UDRD_D& rData)
		{m_rcco.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RCCO_UDRD_D,T_RCCO_UDRD_D&>m_rcco;
};

#endif