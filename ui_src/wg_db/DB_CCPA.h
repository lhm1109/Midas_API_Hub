#ifndef __DB_CCPA_DB_H__
#define __DB_CCPA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_CCPA
{
public:
	CDB_CCPA();
	virtual ~CDB_CCPA();

public:
	T_CCPA_K m_nStartNum;
	T_CCPA_K m_nLastNum;

public:
	void Add(T_CCPA_K Key, T_CCPA_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_CCPA_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_CCPA_K Key,T_CCPA_D& rData)
	{return m_ccpa.Lookup(Key,rData);}
	int GetCount()
	{return m_ccpa.GetCount();}
	POSITION GetStart()
	{return m_ccpa.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CCPA_K& rKey,T_CCPA_D& rData)
	{m_ccpa.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CCPA_K,T_CCPA_K,T_CCPA_D,T_CCPA_D&>m_ccpa;
};

#endif