#ifndef __DB_MTCS_DB_H__
#define __DB_MTCS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_MTCS
{
public:
	CDB_MTCS();
	CDB_MTCS(CDB_MTCS& src);
	CDB_MTCS& operator=(CDB_MTCS& src);
	virtual ~CDB_MTCS();
	CDBDoc* m_pDoc;

public:
	T_MTCS_K m_nStartNum;
	T_MTCS_K m_nLastNum;

public:
	void Add(T_MTCS_K Key,T_MTCS_D& rData);
	BOOL Del(T_MTCS_K Key);

public:
	BOOL Get(T_MTCS_K Key,T_MTCS_D& rData);
		//{return m_mtcs.Lookup(Key,rData);}
	int GetCount();
		//{return m_mtcs.GetCount();}
	POSITION GetStart();
		//{return m_mtcs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MTCS_K& rKey,T_MTCS_D& rData);
		//{m_mtcs.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_MTCS_K Key);
	BOOL DelList(T_MTCS_K Key);
	void DelAllList();
	BOOL GetList(T_MTCS_K Key,MtcsList*& rpList);
	int GetCountList();
	void AddListItem(T_MTCS_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_MTCS_K Key, int nCmd, int nKey);
	void AddListItem(CArray<T_MTCS_K, T_MTCS_K>& aNewMtcsK, 
									 CArray<T_MTCS_K, T_MTCS_K>& aOldMTCSK,
									 int nCmd, int nKey, BOOL bExist);
	BOOL DelListItem(CArray<T_MTCS_K, T_MTCS_K>& aOldMtcsK, 
									 int nCmd, int nKey);

protected:
	CMap<T_MTCS_K,T_MTCS_K,T_MTCS_D,T_MTCS_D&>m_mtcs;
	CMap<T_MTCS_K,T_MTCS_K,MtcsList*,MtcsList*>m_mtcslist;
};

#endif