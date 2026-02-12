#ifndef __DB_LCOM_DB_H__
#define __DB_LCOM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LCOM
{
public:
	CDB_LCOM();
	virtual ~CDB_LCOM();
	CDBDoc* m_pDoc;
	int m_nLcomType;
	int m_nCmd;

public:
	T_LCOM_K m_nStartNum;
	T_LCOM_K m_nLastNum;

public:
	void Add(T_LCOM_K Key,T_LCOM_D& rData);
	BOOL Del(T_LCOM_K Key);

public:
	int GetCountFromBaseDB() { return m_lcom.GetCount(); }
	BOOL Get(T_LCOM_K Key,T_LCOM_D& rData);
		//{return m_lcom.Lookup(Key,rData);}
	int GetCount();
		//{return m_lcom.GetCount();}
	POSITION GetStart();
		//{return m_lcom.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LCOM_K& rKey,T_LCOM_D& rData);
		//{m_lcom.GetNextAssoc(rNextPosition,rKey,rData);}
	
	CMap<T_LCOM_K, T_LCOM_K, UINT, UINT>* GetLcomMap();

protected:
	CMap<T_LCOM_K,T_LCOM_K,T_LCOM_D,T_LCOM_D&>m_lcom;
	CMap<T_LCOM_K, T_LCOM_K, UINT, UINT>* m_arLcomMaps[D_LCOMTYPE_MAX + 1];
};

#endif
