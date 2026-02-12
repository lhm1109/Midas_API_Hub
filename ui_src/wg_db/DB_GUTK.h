#ifndef __DB_GUTK_DB_H__
#define __DB_GUTK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIK;
class CDB_GUTK
{
public:
	CDB_GUTK();
	virtual ~CDB_GUTK();
	CDBDoc* m_pDoc;

public:
	T_GUTK_K m_nStartNum;
	T_GUTK_K m_nLastNum;

public:
	void Add(T_GUTK_K Key, T_GUTK_D& rData, CDB_THIK* pThik);
	BOOL Del(T_GUTK_K Key, CDB_THIK* pThik);

public:
	BOOL Get(T_GUTK_K Key, T_GUTK_D& rData);
	//{return m_gutk.Lookup(Key,rData);}
	int GetCount();
	//{return m_gutk.GetCount();}
	POSITION GetStart();
	//{return m_gutk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUTK_K& rKey, T_GUTK_D& rData);
	//{m_gutk.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUTK_K,T_GUTK_K,T_GUTK_D,T_GUTK_D&>m_gutk;
};

#endif