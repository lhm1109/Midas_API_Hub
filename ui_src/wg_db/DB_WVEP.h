#ifndef __DB_WVEP_DB_H__
#define __DB_WVEP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WVEP
{
public:
	CDB_WVEP();
	virtual ~CDB_WVEP();

public:
	T_WVEP_K m_nLastNum;
	T_WVEP_K m_nStartNum;

public:
	void Add(T_WVEP_K Key,T_WVEP_D& rData);
	BOOL Del(T_WVEP_K Key);

public:
	BOOL Get(T_WVEP_K Key,T_WVEP_D& rData)
		{return m_wvep.Lookup(Key,rData);}
	int GetCount()
		{return m_wvep.GetCount();}
	POSITION GetStart()
		{return m_wvep.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WVEP_K& rKey,T_WVEP_D& rData)
		{m_wvep.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_WVEP_K Key);
	BOOL DelList(T_WVEP_K Key);
	void DelAllList();
	BOOL GetList(T_WVEP_K Key,WvepList*& rpList);
	BOOL GetListFromBaseDB(T_WVEP_K Key,WvepList*& rpList){return m_wveplist.Lookup(Key,rpList);}
	int GetCountList(){return m_wveplist.GetCount();}
	void AddListItem(T_WVEP_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_WVEP_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_WVEP_K,T_WVEP_K,T_WVEP_D,T_WVEP_D&>m_wvep;
	CMap<T_WVEP_K,T_WVEP_K,WvepList*,WvepList*>m_wveplist;
};

#endif
