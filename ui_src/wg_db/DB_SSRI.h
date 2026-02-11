#ifndef __DB_SSRI_DB_H__
#define __DB_SSRI_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_SSRI
{
public:
	CDB_SSRI();
	virtual ~CDB_SSRI();

public:
	T_SSRI_K m_nStartNum;
	T_SSRI_K m_nLastNum;

public:
	void Add(T_SSRI_K Key, T_SSRI_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SSRI_K Key, CDB_ELEM* pElem);

public:
	BOOL Get(T_SSRI_K Key,T_SSRI_D& rData)
	{return m_ssri.Lookup(Key,rData);}
	int GetCount()
	{return m_ssri.GetCount();}
	POSITION GetStart()
	{return m_ssri.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SSRI_K& rKey,T_SSRI_D& rData)
	{m_ssri.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SSRI_K,T_SSRI_K,T_SSRI_D,T_SSRI_D&>m_ssri;
};

#endif