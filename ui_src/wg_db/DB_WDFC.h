#ifndef __DB_WDFC_DB_H__
#define __DB_WDFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_WDFC
{
public:
	CDB_WDFC();
	virtual ~CDB_WDFC();

public:
	T_WDFC_K m_nLastNum;
	T_WDFC_K m_nStartNum;

public:
	void Add(T_WDFC_K Key,T_WDFC_D& rData);
	BOOL Del(T_WDFC_K Key);

public:
	BOOL Get(T_WDFC_K Key,T_WDFC_D& rData)
		{return m_wvep.Lookup(Key,rData);}
	int GetCount()
		{return m_wvep.GetCount();}
	POSITION GetStart()
		{return m_wvep.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WDFC_K& rKey,T_WDFC_D& rData)
		{m_wvep.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_WDFC_K Key);
	BOOL DelList(T_WDFC_K Key);
	void DelAllList();
	BOOL GetList(T_WDFC_K Key,WdfcList*& rpList);
	BOOL GetListFromBaseDB(T_WDFC_K Key,WdfcList*& rpList){return m_wveplist.Lookup(Key,rpList);}
	int GetCountList(){return m_wveplist.GetCount();}
	void AddListItem(T_WDFC_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_WDFC_K Key, int nCmd, int nKey);
	
protected:
	CMap<T_WDFC_K,T_WDFC_K,T_WDFC_D,T_WDFC_D&>m_wvep;
	CMap<T_WDFC_K,T_WDFC_K,WdfcList*,WdfcList*>m_wveplist;
};

#endif
