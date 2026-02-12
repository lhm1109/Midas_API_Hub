#ifndef __DB_TDGR_DB_H__
#define __DB_TDGR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;

class CDB_TDGR
{
public:
	CDB_TDGR();
	virtual ~CDB_TDGR();

public:
	T_TDGR_K m_nLastNum;
	T_TDGR_K m_nStartNum;

public:
	void Add(T_TDGR_K Key,T_TDGR_D& rData);
	BOOL Del(T_TDGR_K Key);

public:
	BOOL Get(T_TDGR_K Key,T_TDGR_D& rData)
		{return m_tdgr.Lookup(Key,rData);}
	int GetCount()
		{return m_tdgr.GetCount();}
	POSITION GetStart()
		{return m_tdgr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDGR_K& rKey,T_TDGR_D& rData)
		{m_tdgr.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_TDGR_K Key);
	BOOL DelList(T_TDGR_K Key);
	void DelAllList();
	BOOL GetList(T_TDGR_K Key,TdgrList*& rpList){return m_tdgrlist.Lookup(Key,rpList);}
	int GetCountList(){return m_tdgrlist.GetCount();}
	void AddListItem(T_TDGR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_TDGR_K Key, int nCmd, int nKey);

protected:
	CMap<T_TDGR_K,T_TDGR_K,T_TDGR_D,T_TDGR_D&>m_tdgr;
	CMap<T_TDGR_K,T_TDGR_K,TdgrList*,TdgrList*>m_tdgrlist;
};

#endif
