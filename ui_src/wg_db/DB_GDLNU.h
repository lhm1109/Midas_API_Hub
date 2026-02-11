#ifndef __DB_GDLNU_DB_H__
#define __DB_GDLNU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_GDLNU
{
public:
	CDB_GDLNU()
	{
		m_gdln.InitHashTable(HASHSIZEGDLN);
	}
	virtual ~CDB_GDLNU()
	{
		DelAll();   // new 사용데이터
	};

public:
	void Add(T_UDRD_KEY Key,T_GDLN_UDRD_D& rData)
		{m_gdln.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{
			T_GDLN_UDRD_D data;
			if(!Get(Key, data))return FALSE;
			delete data.data.pXGrid;
			delete data.data.pYGrid;
			return m_gdln.RemoveKey(Key);
		}
	void DelAll()
		{
			T_UDRD_KEY key;
			T_GDLN_UDRD_D data;
			POSITION pos=GetStart();
			while(pos != NULL)
			{
				GetNext(pos, key, data);
				Del(key);
			}
		}
	BOOL Get(T_UDRD_KEY Key,T_GDLN_UDRD_D& rData)
		{return m_gdln.Lookup(Key,rData);}
	int GetCount()
		{return m_gdln.GetCount();}
	POSITION GetStart()
		{return m_gdln.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_GDLN_UDRD_D& rData)
		{m_gdln.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_GDLN_UDRD_D,T_GDLN_UDRD_D&>m_gdln;
};

#endif