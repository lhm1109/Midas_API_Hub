#ifndef __DB_NLNK_DB_H__
#define __DB_NLNK_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_NLLP;
class CDB_BNGR;

class CDB_NLNK
{
public:
	CDB_NLNK();
	virtual ~CDB_NLNK();
	CDBDoc* m_pDoc;

public:
	T_LCOM_K m_nStartNum;
	T_LCOM_K m_nLastNum;

public:
	void Add(T_NLNK_K Key,T_NLNK_D& rData,CDB_NODE* pNode,CDB_NLLP* pNllp,CDB_BNGR* pBngr);
	BOOL Del(T_NLNK_K Key,CDB_NODE* pNode,CDB_NLLP* pNllp,CDB_BNGR* pBngr);

public:
	BOOL Get(T_NLNK_K Key,T_NLNK_D& rData);
		//{return m_nlnk.Lookup(Key,rData);}
	int GetCount();
		//{return m_nlnk.GetCount();}
	POSITION GetStart();
		//{return m_nlnk.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NLNK_K& rKey,T_NLNK_D& rData);
		//{m_nlnk.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_NLNK_K Key);
	BOOL DelList(T_NLNK_K Key);
	void DelAllList();
	BOOL GetList(T_NLNK_K Key, ElnkList*& rpList);//{return m_nlnklist.Lookup(Key,rpList);}
	int GetCountList();//{return m_nlnklist.GetCount();}
	void AddListItem(T_NLNK_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_NLNK_K Key, int nCmd, int nKey);

protected:
	CMap<T_NLNK_K,T_NLNK_K,T_NLNK_D,T_NLNK_D&>m_nlnk;
	CMap<T_NLNK_K,T_NLNK_K, NlnkList*,NlnkList*>m_nlnklist;
};

#endif