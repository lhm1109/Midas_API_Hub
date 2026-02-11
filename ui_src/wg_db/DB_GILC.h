#ifndef __DB_GILC_DB_H__
#define __DB_GILC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_GILC
{
public:
	CDB_GILC();
	virtual ~CDB_GILC();
	CDBDoc* m_pDoc;

public:
	T_GILC_K m_nStartNum;
	T_GILC_K m_nLastNum;

public:
	void Add(T_GILC_K Key,T_GILC_D& rData);
	BOOL Del(T_GILC_K Key);
	void Initialize();

public:
	BOOL GetFromBaseDB(T_GILC_K Key,T_GILC_D& rData)
		{return m_gilc.Lookup(Key,rData);}
	int GetCountFromBaseDB()
		{return m_gilc.GetCount();}
	POSITION GetStartFromBaseDB()
		{return m_gilc.GetStartPosition();}
	void GetNextFromBaseDB(POSITION& rNextPosition,T_GILC_K& rKey,T_GILC_D& rData)
		{m_gilc.GetNextAssoc(rNextPosition,rKey,rData);}

	BOOL Get(T_GILC_K Key,T_GILC_D& rData);
		//{return m_gilc.Lookup(Key,rData);}
	int GetCount();
		//{return m_gilc.GetCount();}
	POSITION GetStart();
		//{return m_gilc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GILC_K& rKey,T_GILC_D& rData);
		//{m_gilc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GILC_K,T_GILC_K,T_GILC_D,T_GILC_D&>m_gilc;
};

#endif
