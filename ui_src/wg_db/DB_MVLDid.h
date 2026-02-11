#ifndef __DB_MVLDid_DB_H__
#define __DB_MVLDid_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MVLDid
{
public:
	CDB_MVLDid();
	virtual ~CDB_MVLDid();
	CDBDoc* m_pDoc;

public:
	T_MVLDid_K m_nStartNum;
	T_MVLDid_K m_nLastNum;

public:
	void Add(T_MVLDid_K Key,T_MVLDid_D& rData);
	BOOL Del(T_MVLDid_K Key);

public:
	BOOL Get(T_MVLDid_K Key,T_MVLDid_D& rData);
		//{return m_mvldid.Lookup(Key,rData);}
	int GetCount();
		//{return m_mvldid.GetCount();}
	POSITION GetStart();
		//{return m_mvldid.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MVLDid_K& rKey,T_MVLDid_D& rData);
		//{m_mvldid.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MVLDid_K,T_MVLDid_K,T_MVLDid_D,T_MVLDid_D&>m_mvldid;
};

#endif
