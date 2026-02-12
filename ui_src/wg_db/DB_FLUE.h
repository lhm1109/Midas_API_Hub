#pragma once

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_FLUE
{
public:
	CDB_FLUE();
	virtual ~CDB_FLUE();
	CDBDoc* m_pDoc;

public:
	T_FLUE_K m_nStartNum;
	T_FLUE_K m_nLastNum;

public:
	void Add(T_FLUE_K Key,T_FLUE_D& rData);
	BOOL Del(T_FLUE_K Key);
	void Initialize();

public:
	BOOL Get(T_FLUE_K Key,T_FLUE_D& rData);
		//{return m_flue.Lookup(Key,rData);}
	int GetCount();
		//{return m_flue.GetCount();}
	POSITION GetStart();
		//{return m_flue.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_FLUE_K& rKey,T_FLUE_D& rData);
		//{m_flue.GetNextAssoc(rNextPosition,rKey,rData);}
protected:
	CMap<T_FLUE_K,T_FLUE_K,T_FLUE_D,T_FLUE_D&>m_flue;
};

