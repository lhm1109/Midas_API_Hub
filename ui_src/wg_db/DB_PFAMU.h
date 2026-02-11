#ifndef __DB_PFAMU_DB_H__
#define __DB_PFAMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_PFAMU
{
public:
	CDB_PFAMU()
	{
		m_mapData.InitHashTable(HASHSIZE_PFAM);
	}
	virtual ~CDB_PFAMU(){};

public:
	void Add(T_PFAM_K Key,T_PFAM_UDRD_D& rData)
		{m_mapData.SetAt(Key,rData);}
	BOOL Del(T_PFAM_K Key)
		{return m_mapData.RemoveKey(Key);}
	void DelAll()
		{m_mapData.RemoveAll();}
	BOOL Get(T_PFAM_K Key, T_PFAM_UDRD_D& rData)
		{return m_mapData.Lookup(Key,rData);}
	int GetCount()
		{return m_mapData.GetCount();}
	POSITION GetStart()
		{return m_mapData.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PFAM_K& rKey, T_PFAM_UDRD_D& rData)
		{m_mapData.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_PFAM_K,T_PFAM_K, T_PFAM_UDRD_D, T_PFAM_UDRD_D&>m_mapData;
};

#endif