#ifndef __DB_BLCF_DB_H__
#define __DB_BLCF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DN_JUD.h"

class CDB_ELEM;

class CDB_BLCF
{
public:
	CDB_BLCF();
	virtual ~CDB_BLCF();

public:
	T_BLCF_K m_nStartNum;
	T_BLCF_K m_nLastNum;

public:
	void Add(T_BLCF_K Key,T_BLCF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_BLCF_K Key,CDB_ELEM* pElem);
	void AddItem(T_BLCF_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem);
	BOOL DelItem(T_BLCF_K Key,T_ELEM_K KeyElem,CDB_ELEM* pElem);

public:
	BOOL Get(T_BLCF_K Key,T_BLCF_D& rData)
		{return m_blcf.Lookup(Key,rData);}
	int GetCount()
		{return m_blcf.GetCount();}
	POSITION GetStart()
		{return m_blcf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BLCF_K& rKey,T_BLCF_D& rData)
		{m_blcf.GetNextAssoc(rNextPosition,rKey,rData);}
	
	void AddList(T_BLCF_K Key);
	BOOL DelList(T_BLCF_K Key);
	void DelAllList();
	BOOL GetList(T_BLCF_K Key,BlcfList*& rpList){return m_blcflist.Lookup(Key,rpList);}
	int GetCountList(){return m_blcflist.GetCount();}
	void AddListItem(T_BLCF_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_BLCF_K Key, int nCmd, int nKey);

protected:
	CMap<T_BLCF_K,T_BLCF_K,T_BLCF_D,T_BLCF_D&>m_blcf;
	CMap<T_BLCF_K,T_BLCF_K,BlcfList*,BlcfList*>m_blcflist;
};

#endif