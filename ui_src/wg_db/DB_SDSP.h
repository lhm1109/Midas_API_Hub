#ifndef __DB_SDSP_DB_H__
#define __DB_SDSP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_NODE;
class CDB_LDGR;

class CDB_SDSP
{
public:
	CDB_SDSP();
	virtual ~CDB_SDSP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDSP_K Key,T_SDSP_D& rData,CDB_STLD* pStld,CDB_NODE* pNode, CDB_LDGR* pLdgr);
	BOOL Del(T_SDSP_K Key,CDB_STLD* pStld,CDB_NODE* pNode, CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_SDSP_K Key,T_SDSP_D& rData)
		{return m_sdsp.Lookup(Key.keymap,rData);}

	BOOL Get(T_SDSP_K Key,T_SDSP_D& rData);
		//{return m_sdsp.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_sdsp.GetCount();}
	POSITION GetStart();
		//{return m_sdsp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDSP_K& rKey,T_SDSP_D& rData);
		//{m_sdsp.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_SDSP_KEY,T_SDSP_KEY,T_SDSP_D,T_SDSP_D&>m_sdsp;
};

#endif
