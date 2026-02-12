#ifndef __DB_TDPL_DB_H__
#define __DB_TDPL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_LDGR;

class CDB_TDPL
{
public:
	CDB_TDPL();
	virtual ~CDB_TDPL();
	CDBDoc* m_pDoc;

public:
	void Add(T_TDPL_K Key,T_TDPL_D& rData,CDB_STLD* pStld,CDB_LDGR* pLdgr);
	BOOL Del(T_TDPL_K Key,CDB_STLD* pStld,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_TDPL_K Key,T_TDPL_D& rData)
		{ return m_tdpl.Lookup(Key.keymap,rData); }
	POSITION GetStartFromBaseDB()
		{return m_tdpl.GetStartPosition();}
	void GetNextFromBaseDB(POSITION& rNextPosition,T_TDPL_K& rKey,T_TDPL_D& rData)
		{m_tdpl.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

	BOOL Get(T_TDPL_K Key,T_TDPL_D& rData);
		//{return m_tdpl.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_tdpl.GetCount();}
	POSITION GetStart();
		//{return m_tdpl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_TDPL_K& rKey,T_TDPL_D& rData);
		//{m_tdpl.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_TDPL_KEY,T_TDPL_KEY,T_TDPL_D,T_TDPL_D&>m_tdpl;
};

#endif