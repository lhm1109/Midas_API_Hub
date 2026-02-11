#ifndef __DB_RIGD_DB_H__
#define __DB_RIGD_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_BNGR;

class CDB_RIGD
{
public:
	CDB_RIGD();
	CDB_RIGD(CDB_RIGD& src);
	CDB_RIGD& operator=(CDB_RIGD& src);
	virtual ~CDB_RIGD();
	CDBDoc* m_pDoc;

public:
	void Add(T_RIGD_K Key,T_RIGD_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr);
	BOOL Del(T_RIGD_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr);
	void AddItem(T_RIGD_K Key,T_NODE_K KeySlave,CDB_NODE* pNode);
	BOOL DelItem(T_RIGD_K Key,T_NODE_K KeySlave,CDB_NODE* pNode);

public:
	BOOL GetFromBaseDB(T_RIGD_K Key,T_RIGD_D& rData)
		{return m_rigd.Lookup(Key.keymap,rData);}

	BOOL Get(T_RIGD_K Key,T_RIGD_D& rData);
		//{return m_rigd.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_rigd.GetCount();}
	POSITION GetStart();
		//{return m_rigd.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RIGD_K& rKey,T_RIGD_D& rData);
		//{m_rigd.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_RIGD_KEY,T_RIGD_KEY,T_RIGD_D,T_RIGD_D&>m_rigd;
};

#endif