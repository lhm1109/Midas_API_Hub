#ifndef __DB_SLANfr_DB_H__
#define __DB_SLANfr_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_SLANfr
{
public:
	CDB_SLANfr();
	virtual ~CDB_SLANfr();
	CDBDoc* m_pDoc;

public:
	T_SLANfr_K m_nStartNum;
	T_SLANfr_K m_nLastNum;

public:
	void Add(T_SLANfr_K Key,T_SLANfr_D& rData,CDB_NODE* pNode);
	BOOL Del(T_SLANfr_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_SLANfr_K Key,T_SLANfr_D& rData);
		//{return m_slanfr.Lookup(Key,rData);}
	int GetCount();
		//{return m_slanfr.GetCount();}
	POSITION GetStart();
		//{return m_slanfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SLANfr_K& rKey,T_SLANfr_D& rData);
		//{m_slanfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SLANfr_K,T_SLANfr_K,T_SLANfr_D,T_SLANfr_D&>m_slanfr;
};

#endif

