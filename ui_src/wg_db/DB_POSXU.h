#ifndef __DB_POSXU_DB_H__
#define __DB_POSXU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_POSXU
{
public:
	CDB_POSXU()
	{
		m_posx.InitHashTable(HASHSIZEPOSX);
	}
	virtual ~CDB_POSXU(){};

public:
	void Add(T_UDRD_KEY Key,T_POSX_UDRD_D& rData)
		{m_posx.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_posx.RemoveKey(Key);}
	void DelAll()
		{m_posx.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_POSX_UDRD_D& rData)
		{return m_posx.Lookup(Key,rData);}
	int GetCount()
		{return m_posx.GetCount();}
	POSITION GetStart()
		{return m_posx.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_POSX_UDRD_D& rData)
		{m_posx.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_POSX_UDRD_D,T_POSX_UDRD_D&>m_posx;
};

#endif