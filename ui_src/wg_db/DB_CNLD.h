#ifndef __DB_CNLD_DB_H__
#define __DB_CNLD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"
#include <map>

class CDBDoc;
class CDB_STLD;
class CDB_NODE;
class CDB_LDGR;


typedef CArray<T_CNLD_KEY, T_CNLD_KEY&> CnldTypeList;

class CDB_CNLD
{
public:
	CDB_CNLD();
	virtual ~CDB_CNLD();
	CDBDoc* m_pDoc;

public:
	void Add(T_CNLD_K Key,T_CNLD_D& rData,CDB_STLD* pStld,CDB_NODE* pNode, CDB_LDGR* pLdgr);
	BOOL Del(T_CNLD_K Key,CDB_STLD* pStld,CDB_NODE* pNode, CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_CNLD_K Key,T_CNLD_D& rData)
		{return m_cnld.Lookup(Key.keymap,rData);}

	BOOL Get(T_CNLD_K Key,T_CNLD_D& rData);
		//{return m_cnld.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_cnld.GetCount();}
	POSITION GetStart();
		//{return m_cnld.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_CNLD_K& rKey,T_CNLD_D& rData);
		//{m_cnld.GetNextAssoc(rNextPosition,rKey.keymap,rData);}
	void UpdateType(T_CNLD_K Key,T_CNLD_D& rData, BOOL bDelete);
	//Cnld Type
	BOOL GetType(const CString& Key,CnldTypeList& pData);
	int GetTypeCount();
	POSITION GetTypeStart();
	void GetTypeNext(POSITION& rNextPosition,CString& rKey,CnldTypeList*& pData);
protected:
	CMap<T_CNLD_KEY,T_CNLD_KEY,T_CNLD_D,T_CNLD_D&>m_cnld;
	CMap<CString,LPCTSTR,CnldTypeList*,CnldTypeList*>m_cnldType;//CnldTypeList T_CNLD_D:Groupk,LoadCaseKey are the same
};

#endif