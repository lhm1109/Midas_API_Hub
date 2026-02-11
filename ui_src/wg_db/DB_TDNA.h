#ifndef __DB_TDNA_DB_H__
#define __DB_TDNA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_TDGR;

class CDB_TDNA
{
public:
	CDB_TDNA();
	virtual ~CDB_TDNA();
	CDBDoc* m_pDoc;

public:
	T_TDNA_K m_nLastNum;
	T_TDNA_K m_nStartNum;

public:
	void Add(T_TDNA_K Key,T_TDNA_D& rData,CDB_ELEM* pElem,CDB_TDGR* pTdgr);
	BOOL Del(T_TDNA_K Key,CDB_ELEM* pElem,CDB_TDGR* pTdgr);

public:
	BOOL GetFromBaseDB(T_TDNA_K Key,T_TDNA_D& rData);
	BOOL Get(T_TDNA_K Key,T_TDNA_D& rData);
		//{return m_tdna.Lookup(Key,rData);}
	int GetCount();
	int GetCountFromBaseDB();
		//{return m_tdna.GetCount();}
	POSITION GetStart();
	POSITION GetStartFromBaseDB();
	void GetNextFromBaseDB(POSITION& rNextPosition,T_TDNA_K& rKey,T_TDNA_D& rData);
		//{return m_tdna.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDNA_K& rKey,T_TDNA_D& rData);
		//{m_tdna.GetNextAssoc(rNextPosition,rKey,rData);}

public:
	BOOL IsUsedTdnt(T_TDNT_K TdntKey);

protected:
	CMap<T_TDNA_K,T_TDNA_K,T_TDNA_D,T_TDNA_D&>m_tdna;
	CMap<T_TDNT_K,T_TDNT_K,int,int> m_tdntCount;
};

#endif