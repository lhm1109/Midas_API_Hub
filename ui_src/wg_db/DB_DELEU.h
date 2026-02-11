#ifndef __DB_DELEU_DB_H__
#define __DB_DELEU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_DELEU
{
public:
	CDB_DELEU()
	{
		m_dele.InitHashTable(HASHSIZEDELE);
	}
	virtual ~CDB_DELEU(){};

public:
	void Add(T_UDRD_KEY Key,T_DELE_UDRD_D& rData)
		{m_dele.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{
			T_DELE_UDRD_D data;
			if(!Get(Key, data))return FALSE;
			delete data.data.parDele;
			return m_dele.RemoveKey(Key);
		}
	void DelAll()
		{
			T_UDRD_KEY key;
			T_DELE_UDRD_D data;
			POSITION pos=GetStart();
			while(pos != NULL)
			{
				GetNext(pos, key, data);
				Del(key);
			}
		}
	BOOL Get(T_UDRD_KEY Key,T_DELE_UDRD_D& rData)
		{return m_dele.Lookup(Key,rData);}
	POSITION GetStart()
		{return m_dele.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_DELE_UDRD_D& rData)
		{m_dele.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_DELE_UDRD_D,T_DELE_UDRD_D&>m_dele;
};

#endif