#ifndef __DB_POFC_DB_H__
#define __DB_POFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_NODE;

class CDB_POFC
{
public:
	CDB_POFC();
	virtual ~CDB_POFC();
	CDBDoc* m_pDoc;

public:
	T_POFC_K m_nStartNum;
	T_POFC_K m_nLastNum;

public:
	void Add(T_POFC_K Key, T_POFC_D& rData);
	BOOL Del(T_POFC_K Key);

public:
	BOOL Get(T_POFC_K Key,T_POFC_D& rData);
		//{return m_pofc.Lookup(Key,rData);}
	int GetCount();
		//{return m_pofc.GetCount();}
	POSITION GetStart();
		//{return m_pofc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_POFC_K& rKey,T_POFC_D& rData);
		//{m_pofc.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_POFC_K Key);
	BOOL DelList(T_POFC_K Key);
	void DelAllList();
	BOOL GetList(T_POFC_K Key, PofcList*& rpList);//{return m_pofclist.Lookup(Key,rpList);}
	int GetCountList();//{return m_pofclist.GetCount();}
	void AddListItem(T_POFC_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_POFC_K Key, int nCmd, int nKey);

protected:
	CMap<T_POFC_K,T_POFC_K,T_POFC_D,T_POFC_D&>m_pofc;
	CMap<T_POFC_K,T_POFC_K, PofcList*,PofcList*>m_pofclist;
};

#endif
