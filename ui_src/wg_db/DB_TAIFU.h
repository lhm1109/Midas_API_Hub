#ifndef __DB_TAIFU_DB_H__
#define __DB_TAIFU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_TAIFU
{
public:
	CDB_TAIFU()
	{
		m_taif.InitHashTable(HASHSIZETAIF);
	}
	virtual ~CDB_TAIFU() {};

public:
	void Add(T_UDRD_KEY Key, T_TAIF_UDRD_D& rData)
	{
		m_taif.SetAt(Key, rData);
	}
	BOOL Del(T_UDRD_KEY Key)
	{
		return m_taif.RemoveKey(Key);
	}
	void DelAll()
	{
		m_taif.RemoveAll();
	}
	BOOL Get(T_UDRD_KEY Key, T_TAIF_UDRD_D& rData)
	{
		return m_taif.Lookup(Key, rData);
	}

protected:
	CMap<T_UDRD_KEY, T_UDRD_KEY, T_TAIF_UDRD_D, T_TAIF_UDRD_D&>m_taif;
};

#endif