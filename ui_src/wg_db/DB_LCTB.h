#ifndef __DB_LCTB_DB_H__
#define __DB_LCTB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_LCTB
{
public:
	CDB_LCTB();
	virtual ~CDB_LCTB();
	CDBDoc* m_pDoc;
//  int m_nLctbType;
//  int m_nCmd;

public:
	T_LCTB_K m_nStartNum;
	T_LCTB_K m_nLastNum;

public:
	void Add(T_LCTB_K Key,T_LCTB_D& rData);
	BOOL Del(T_LCTB_K Key);

public:
	int GetCountFromBaseDB() { return m_lctb.GetCount(); }
	BOOL Get(T_LCTB_K Key,T_LCTB_D& rData);
		//{return m_lctb.Lookup(Key,rData);}
	int GetCount();
		//{return m_lctb.GetCount();}
	POSITION GetStart();
		//{return m_lctb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LCTB_K& rKey,T_LCTB_D& rData);
		//{m_lctb.GetNextAssoc(rNextPosition,rKey,rData);}
	
	CMap<T_LCTB_K, T_LCTB_K, UINT, UINT>* GetLctbMap();

protected:
	CMap<T_LCTB_K,T_LCTB_K,T_LCTB_D,T_LCTB_D&>m_lctb;
};

#endif
