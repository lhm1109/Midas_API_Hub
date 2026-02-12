#ifndef __DB_HSPT_DB_H__
#define __DB_HSPT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_BNGR;

class CDB_HSPT
{
public:
	CDB_HSPT();
	virtual ~CDB_HSPT();
	CDBDoc* m_pDoc;

public:
	void Add(T_HSPT_K Key,T_HSPT_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr);
	BOOL Del(T_HSPT_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_HSPT_K Key,T_HSPT_D& rData)
		{return m_hspt.Lookup(Key.keymap,rData);}

	BOOL Get(T_HSPT_K Key,T_HSPT_D& rData);
		//{return m_hspt.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_hspt.GetCount();}
	POSITION GetStart();
		//{return m_hspt.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HSPT_K& rKey,T_HSPT_D& rData);
		//{m_hspt.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_HSPT_KEY,T_HSPT_KEY,T_HSPT_D,T_HSPT_D&>m_hspt;
};

#endif