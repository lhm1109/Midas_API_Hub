#pragma once

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;

class CDB_THSF
{
public:
	CDB_THSF();
	virtual ~CDB_THSF();
	CDBDoc* m_pDoc;

public:
	T_THSF_K m_nStartNum;
	T_THSF_K m_nLastNum;

public:
	void Add(T_THSF_K Key,T_THSF_D& rData,CDB_THIS* pThis);
	BOOL Del(T_THSF_K Key,CDB_THIS* pThis);

public:
	BOOL Get(T_THSF_K Key,T_THSF_D& rData);
		//{return m_thsf.Lookup(Key,rData);}
	int GetCount();
		//{return m_thsf.GetCount();}
	POSITION GetStart();
		//{return m_thsf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THSF_K& rKey,T_THSF_D& rData);
		//{m_thsf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THSF_K,T_THSF_K,T_THSF_D, T_THSF_D&>m_thsf;
};

