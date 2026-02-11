#ifndef __DB_PTMP_DB_H__
#define __DB_PTMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_ELEM;
class CDB_LDGR;

class CDB_PTMP
{
public:
	CDB_PTMP();
	virtual ~CDB_PTMP();
	CDBDoc* m_pDoc;

public:
	void Add(T_PTMP_K Key,T_PTMP_D& rData,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);
	BOOL Del(T_PTMP_K Key,CDB_STLD* pStld,CDB_ELEM* pElem,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_PTMP_K Key,T_PTMP_D& rData)
		{return m_ptmp.Lookup(Key.keymap,rData);}

	BOOL Get(T_PTMP_K Key,T_PTMP_D& rData);
		//{return m_ptmp.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_ptmp.GetCount();}
	POSITION GetStart();
		//{return m_ptmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_PTMP_K& rKey,T_PTMP_D& rData);
		//{m_ptmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_PTMP_KEY,T_PTMP_KEY,T_PTMP_D,T_PTMP_D&>m_ptmp;
};

#endif