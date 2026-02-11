#ifndef __DB_SMPT_DB_H__
#define __DB_SMPT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_SMPT
{
public:
	CDB_SMPT();
	virtual ~CDB_SMPT();
	CDBDoc* m_pDoc;

public:
	T_SMPT_K m_nStartNum;
	T_SMPT_K m_nLastNum;

public:
	void Add(T_SMPT_K Key,T_SMPT_D& rData,CDB_NODE* pNode);
	BOOL Del(T_SMPT_K Key,CDB_NODE* pNode);

public:
	BOOL GetListFromBaseDB(T_SMPT_K Key,SmptList*& rpList){return m_smptlist.Lookup(Key,rpList);}

	BOOL Get(T_SMPT_K Key,T_SMPT_D& rData);
		//{return m_smpt.Lookup(Key,rData);}
	int GetCount();
		//{return m_smpt.GetCount();}
	POSITION GetStart();
		//{return m_smpt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SMPT_K& rKey,T_SMPT_D& rData);
		//{m_smpt.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_SMPT_K Key);
	BOOL DelList(T_SMPT_K Key);
	void DelAllList();
	BOOL GetList(T_SMPT_K Key,SmptList*& rpList);//{return m_smptlist.Lookup(Key,rpList);}
	int GetCountList();//{return m_smptlist.GetCount();}
	void AddListItem(T_SMPT_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_SMPT_K Key, int nCmd, int nKey);

protected:
	CMap<T_SMPT_K,T_SMPT_K,T_SMPT_D,T_SMPT_D&>m_smpt;
	CMap<T_SMPT_K,T_SMPT_K,SmptList*,SmptList*>m_smptlist;
};

#endif
