#ifndef __DB_DLCM_DB_H__
#define __DB_DLCM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_DLCM
{
public:
	CDB_DLCM();
	virtual ~CDB_DLCM();
	CDBDoc* m_pDoc;

public:
	T_DLCM_K m_nStartNum;
	T_DLCM_K m_nLastNum;

public:
	void Add(T_DLCM_K Key, T_DLCM_D& rData);
	BOOL Del(T_DLCM_K Key);

public:
	BOOL Get(T_DLCM_K Key, T_DLCM_D& rData);
		//{return m_dlcm.Lookup(Key,rData);}
	int GetCount();
		//{return m_dlcm.GetCount();}
	POSITION GetStart();
		//{return m_dlcm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_DLCM_K& rKey,T_DLCM_D& rData);
		//{m_dlcm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_DLCM_K,T_DLCM_K,T_DLCM_D,T_DLCM_D&>m_dlcm;
};

#endif