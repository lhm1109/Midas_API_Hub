#ifndef __DB_LAPL_DB_H__
#define __DB_LAPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_NODE;
class CDB_ELEM;
class CDB_LAGR;

class CDB_LAPL
{
public:
	CDB_LAPL();
	virtual ~CDB_LAPL();

public:
	T_LAPL_K m_nStartNum;
	T_LAPL_K m_nLastNum;

public:
	void Add(T_LAPL_K Key,T_LAPL_D& rData,CDB_NODE* pNode,CDB_ELEM* pElem,CDB_LAGR* pLagr);
	BOOL Del(T_LAPL_K Key,CDB_NODE* pNode,CDB_ELEM* pElem,CDB_LAGR* pLagr);
	void AddItem(T_LAPL_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem);
	BOOL DelItem(T_LAPL_K Key,T_NODE_K KeyNode,T_ELEM_K KeyElem,CDB_NODE* pNode,CDB_ELEM* pElem);

public:
	BOOL Get(T_LAPL_K Key,T_LAPL_D& rData)
		{return m_lapl.Lookup(Key,rData);}
	int GetCount()
		{return m_lapl.GetCount();}
	POSITION GetStart()
		{return m_lapl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LAPL_K& rKey,T_LAPL_D& rData)
		{m_lapl.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_LAPL_K Key);
	BOOL DelList(T_LAPL_K Key);
	void DelAllList();
	BOOL GetList(T_LAPL_K Key,LaplList*& rpList){return m_lapllist.Lookup(Key,rpList);}
	int GetCountList(){return m_lapllist.GetCount();}
	void AddListItem(T_LAPL_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_LAPL_K Key, int nCmd, int nKey);

protected:
	CMap<T_LAPL_K,T_LAPL_K,T_LAPL_D,T_LAPL_D&>m_lapl;
	CMap<T_LAPL_K,T_LAPL_K,LaplList*,LaplList*>m_lapllist;
};

#endif