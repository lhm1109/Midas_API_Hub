#ifndef __DB_SECF_DB_H__
#define __DB_SECF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;
class CDB_BNGR;

class CDB_SECF
{
public:
	CDB_SECF();
	virtual ~CDB_SECF();
	CDBDoc* m_pDoc;

public:
	void Add(T_SECF_K Key,T_SECF_D& rData,CDB_SECT* pSect,CDB_BNGR* pBngr);
	BOOL Del(T_SECF_K Key,CDB_SECT* pSect,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_SECF_K Key,T_SECF_D& rData)
		{return m_secf.Lookup(Key.keymap,rData);}

	BOOL Get(T_SECF_K Key,T_SECF_D& rData);
		//{return m_secf.Lookup(Key,rData);}
	int GetCount();
		//{return m_secf.GetCount();}
	POSITION GetStart();
		//{return m_secf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SECF_K& rKey,T_SECF_D& rData);
		//{m_secf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SECF_KEY,T_SECF_KEY,T_SECF_D,T_SECF_D&>m_secf;
};

#endif