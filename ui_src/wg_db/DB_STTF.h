#pragma once

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_STTF
{
public:
	CDB_STTF();
	virtual ~CDB_STTF();
	CDBDoc* m_pDoc;

public:
	T_STTF_K m_nStartNum;
	T_STTF_K m_nLastNum;

public:
	void Add(T_STTF_K Key,T_STTF_D& rData);
	BOOL Del(T_STTF_K Key);
	void Initialize();

public:
	BOOL Get(T_STTF_K Key,T_STTF_D& rData);
		//{return m_sttf.Lookup(Key,rData);}
	int GetCount();
		//{return m_sttf.GetCount();}
	POSITION GetStart();
		//{return m_sttf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STTF_K& rKey,T_STTF_D& rData);
		//{m_sttf.GetNextAssoc(rNextPosition,rKey,rData);}
protected:
	CMap<T_STTF_K,T_STTF_K,T_STTF_D,T_STTF_D&>m_sttf;
};

