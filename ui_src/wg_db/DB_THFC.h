#ifndef __DB_THFC_DB_H__
#define __DB_THFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_THFC
{
public:
	CDB_THFC();
	virtual ~CDB_THFC();
	CDBDoc* m_pDoc;

public:
	T_THFC_K m_nStartNum;
	T_THFC_K m_nLastNum;

public:
	void Add(T_THFC_K Key,T_THFC_D& rData);
	BOOL Del(T_THFC_K Key);

public:
	BOOL Get(T_THFC_K Key,T_THFC_D& rData);
		//{return m_thfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_thfc.GetCount();}
	POSITION GetStart();
		//{return m_thfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THFC_K& rKey,T_THFC_D& rData);
		//{m_thfc.GetNextAssoc(rNextPosition,rKey,rData);}

	int GetThfcKeyList_Direct(CArray<T_THFC_K, T_THFC_K>& rThfcKeyList);  // (속도개선) 복사연산 없이 주소상의 데이터를 바로 읽어서 처리

protected:
	CMap<T_THFC_K,T_THFC_K,T_THFC_D,T_THFC_D&>m_thfc;
};

#endif
