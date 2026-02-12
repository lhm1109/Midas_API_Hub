#ifndef __DB_KFAV_DB_H__
#define __DB_KFAV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_KFAV
{
public:
	CDB_KFAV();
	virtual ~CDB_KFAV();
	CDBDoc* m_pDoc;

public:
	void Add(T_KFAV_K Key,T_KFAV_D& rData);
	BOOL Del(T_KFAV_K Key);

public:
	BOOL Get(T_KFAV_K Key,T_KFAV_D& rData);
		//{return m_kfav.Lookup(Key,rData);}
	int GetCount();
		//{return m_kfav.GetCount();}
	POSITION GetStart();
		//{return m_kfav.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_KFAV_K& rKey,T_KFAV_D& rData);
		//{m_kfav.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_KFAV_K,T_KFAV_K,T_KFAV_D,T_KFAV_D&>m_kfav;
};

#endif

