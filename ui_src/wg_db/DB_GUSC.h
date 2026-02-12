#ifndef __DB_GUSC_DB_H__
#define __DB_GUSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;
class CDB_GUSC
{
public:
	CDB_GUSC();
	virtual ~CDB_GUSC();
	CDBDoc* m_pDoc;

public:
	T_GUSC_K m_nStartNum;
	T_GUSC_K m_nLastNum;

public:
	void Add(T_GUSC_K Key, T_GUSC_D& rData, CDB_SECT* pSect);
	BOOL Del(T_GUSC_K Key, CDB_SECT* pSect);

public:
	BOOL Get(T_GUSC_K Key, T_GUSC_D& rData);
	//{return m_gusc.Lookup(Key,rData);}
	int GetCount();
	//{return m_gusc.GetCount();}
	POSITION GetStart();
	//{return m_gusc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_GUSC_K& rKey, T_GUSC_D& rData);
	//{m_gusc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GUSC_K,T_GUSC_K,T_GUSC_D,T_GUSC_D&>m_gusc;
};

#endif