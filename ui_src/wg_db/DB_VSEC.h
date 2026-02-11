#ifndef __DB_VSEC_DB_H__
#define __DB_VSEC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDB_NODE;

class CDB_VSEC
{
public:
	CDB_VSEC();
	virtual ~CDB_VSEC();

public:
	T_VSEC_K m_nStartNum;
	T_VSEC_K m_nLastNum;

public:
	void Add(T_VSEC_K Key,T_VSEC_D& rData,CDB_ELEM* pElem,CDB_NODE* pNode);
	BOOL Del(T_VSEC_K Key,CDB_ELEM* pElem,CDB_NODE* pNode);
	void AddItem(T_VSEC_K Key,T_NODE_K KeyNode, T_ELEM_K KeyElem, CDB_NODE* pNode, CDB_ELEM* pElem);
	BOOL DelItem(T_VSEC_K Key,T_NODE_K KeyNode, T_ELEM_K KeyElem, CDB_NODE* pNode, CDB_ELEM* pElem);

public:
	BOOL Get(T_VSEC_K Key,T_VSEC_D& rData)
		{return m_vsec.Lookup(Key,rData);}
	int GetCount()
		{return m_vsec.GetCount();}
	POSITION GetStart()
		{return m_vsec.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_VSEC_K& rKey,T_VSEC_D& rData)
		{m_vsec.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_VSEC_K Key);
	BOOL DelList(T_VSEC_K Key);
	void DelAllList();
	BOOL GetList(T_VSEC_K Key,VsecList*& rpList){return m_vseclist.Lookup(Key,rpList);}
	int GetCountList(){return m_vseclist.GetCount();}
	void AddListItem(T_VSEC_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_VSEC_K Key, int nCmd, int nKey);

protected:
	CMap<T_VSEC_K,T_VSEC_K,T_VSEC_D,T_VSEC_D&>m_vsec;
	CMap<T_VSEC_K,T_VSEC_K,VsecList*,VsecList*>m_vseclist;
};

#endif