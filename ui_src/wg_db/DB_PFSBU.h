#ifndef __DB_PFSBU_DB_H__
#define __DB_PFSBU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PFSBU
{
public:
	CDB_PFSBU()
	{
		m_mapData.InitHashTable(HASHSIZE_PFSB);
	}
	virtual ~CDB_PFSBU(){};

public:
	void Add(T_PFSB_K Key,T_PFSB_UDRD_D& rData)
		{m_mapData.SetAt(Key,rData);}
	BOOL Del(T_PFSB_K Key)
		{return m_mapData.RemoveKey(Key);}
	void DelAll()
		{m_mapData.RemoveAll();}
	BOOL Get(T_PFSB_K Key,T_PFSB_UDRD_D& rData)
		{return m_mapData.Lookup(Key,rData);}
	int GetCount()
		{return m_mapData.GetCount();}
	POSITION GetStart()
		{return m_mapData.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFSB_K& rKey,T_PFSB_UDRD_D& rData)
		{m_mapData.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_PFSB_K,T_PFSB_K,T_PFSB_UDRD_D,T_PFSB_UDRD_D&> m_mapData;
};

#endif