#ifndef __DB_DNODU_DB_H__
#define __DB_DNODU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DNODU
{
public:
	CDB_DNODU()
	{
		m_dnod.InitHashTable(HASHSIZEDNOD);
	}
	virtual ~CDB_DNODU(){};

public:
	void Add(T_UDRD_KEY Key,T_DNOD_UDRD_D& rData)
		{m_dnod.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{
			T_DNOD_UDRD_D data;
			if(!Get(Key, data))return FALSE;
			delete data.data.parDnod;
			return m_dnod.RemoveKey(Key);
		}
	void DelAll()
		{
			T_UDRD_KEY key;
			T_DNOD_UDRD_D data;
			POSITION pos=GetStart();
			while(pos != NULL)
			{
				GetNext(pos, key, data);
				Del(key);
			}
		}
	BOOL Get(T_UDRD_KEY Key,T_DNOD_UDRD_D& rData)
		{return m_dnod.Lookup(Key,rData);}
	POSITION GetStart()
		{return m_dnod.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DNOD_UDRD_D& rData)
		{m_dnod.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DNOD_UDRD_D,T_DNOD_UDRD_D&>m_dnod;
};

#endif