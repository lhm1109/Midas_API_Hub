#ifndef __DB_LDGR_DB_H__
#define __DB_LDGR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDBDoc;

struct T_CMDKEY_K
{
	operator unsigned long() { return (nCmd<<16) + (nKey); }
	BOOL operator == (const T_CMDKEY_K& ck) const { return nCmd == ck.nCmd && nKey == ck.nKey; }
	UINT nCmd;
	UINT nKey;
};

class CDB_LDGR
{
public:
	CDB_LDGR();
	virtual ~CDB_LDGR();
	CDBDoc* m_pDoc;

public:
	T_LDGR_K m_nLastNum;
	T_LDGR_K m_nStartNum;

public:
	void Add(T_LDGR_K Key,T_LDGR_D& rData);
	BOOL Del(T_LDGR_K Key);

public:
	BOOL Get(T_LDGR_K Key,T_LDGR_D& rData)
		{return m_ldgr.Lookup(Key,rData);}
	int GetCount()
		{return m_ldgr.GetCount();}
	POSITION GetStart()
		{return m_ldgr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LDGR_K& rKey,T_LDGR_D& rData)
		{m_ldgr.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_LDGR_K Key);
	BOOL DelList(T_LDGR_K Key);
	void DelAllList();
	BOOL GetList(T_LDGR_K Key,LdgrList*& rpList);
	BOOL GetListFromBaseDB(T_LDGR_K Key,LdgrList*& rpList){return m_ldgrlist.Lookup(Key,rpList);}
	int GetCountList(){return m_ldgrlist.GetCount();}
	void AddListItem(T_LDGR_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_LDGR_K Key, int nCmd, int nKey);

protected:
	CMap<T_LDGR_K,T_LDGR_K,T_LDGR_D,T_LDGR_D&>m_ldgr;
	CMap<T_LDGR_K,T_LDGR_K,LdgrList*,LdgrList*>m_ldgrlist;
	// Default Load Group에 대해 Del List Item 빨리 하기 위한 Map
	CMap<T_CMDKEY_K, T_CMDKEY_K, POSITION, POSITION>m_CmdKey2Pos; 
};

#endif
