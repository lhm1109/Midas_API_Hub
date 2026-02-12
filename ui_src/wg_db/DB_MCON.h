#ifndef __DB_MCON_DB_H__
#define __DB_MCON_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_BNGR;

class CDB_MCON
{
public:
	CDB_MCON();
	CDB_MCON(CDB_MCON& src);
	CDB_MCON& operator=(CDB_MCON& src);
	virtual ~CDB_MCON();
	CDBDoc* m_pDoc;

public:
	void Add(T_MCON_K Key,T_MCON_D& rData,CDB_NODE* pNode,CDB_BNGR* pBngr);
	BOOL Del(T_MCON_K Key,CDB_NODE* pNode,CDB_BNGR* pBngr);
	void AddItem(T_MCON_K Key,T_MCON_BASE& KeySlave,CDB_NODE* pNode);
	BOOL DelItem(T_MCON_K Key,T_MCON_BASE& KeySlave,CDB_NODE* pNode);

public:
	BOOL GetFromBaseDB(T_MCON_K Key,T_MCON_D& rData)
		{return m_mcon.Lookup(Key.keymap,rData);}

	BOOL Get(T_MCON_K Key,T_MCON_D& rData);
		//{return m_mcon.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_mcon.GetCount();}
	POSITION GetStart();
		//{return m_mcon.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MCON_K& rKey,T_MCON_D& rData);
		//{m_mcon.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_MCON_KEY,T_MCON_KEY,T_MCON_D,T_MCON_D&>m_mcon;
};

#endif