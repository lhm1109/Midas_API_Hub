#ifndef __DB_GSPR_DB_H__
#define __DB_GSPR_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;
class CDB_GSTP;
class CDB_BNGR;

class CDB_GSPR
{
public:
	CDB_GSPR();
	virtual ~CDB_GSPR();
	CDBDoc* m_pDoc;

public:
	void Add(T_GSPR_K Key,T_GSPR_D& rData,CDB_GSTP* pGstp,CDB_NODE* pNode,CDB_BNGR* pBngr);
	BOOL Del(T_GSPR_K Key,CDB_GSTP* pGstp,CDB_NODE* pNode,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_GSPR_K Key,T_GSPR_D& rData)
		{return m_gspr.Lookup(Key.keymap,rData);}

	BOOL Get(T_GSPR_K Key,T_GSPR_D& rData);
		//{return m_gspr.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_gspr.GetCount();}
	POSITION GetStart();
		//{return m_gspr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GSPR_K& rKey,T_GSPR_D& rData);
		//{m_gspr.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_GSPR_KEY,T_GSPR_KEY,T_GSPR_D,T_GSPR_D&>m_gspr;
};

#endif