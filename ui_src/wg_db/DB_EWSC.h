#ifndef __DB_EWSC_DB_H__
#define __DB_EWSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_EWSC
{
public:
	CDB_EWSC();
	virtual ~CDB_EWSC();
	CDBDoc* m_pDoc;

public:
	void Add(T_EWSC_K Key,T_EWSC_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_EWSC_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	void AddDesign(T_EWSC_K Key,T_EWSC_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL DelDesign(T_EWSC_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	///// ewsc
	BOOL GetFromBaseDB(T_EWSC_K Key,T_EWSC_D& rData)
	{return m_ewsc.Lookup(Key.keymap,rData);}
	BOOL Get(T_EWSC_K Key,T_EWSC_D& rData);
	int GetCount();
	POSITION GetStart();
	void GetNext(POSITION& rNextPosition,T_EWSC_K& rKey,T_EWSC_D& rData);

	///// ewsd
	BOOL GetFromBaseDBDesign(T_EWSC_K Key,T_EWSC_D& rData)
	{return m_ewscDesign.Lookup(Key.keymap,rData);}
	BOOL GetDesign(T_EWSC_K Key,T_EWSC_D& rData);
	int GetCountDesign();
	POSITION GetStartDesign();
	void GetNextDesign(POSITION& rNextPosition,T_EWSC_K& rKey,T_EWSC_D& rData);

protected:
	CMap<T_EWSC_KEY,T_EWSC_KEY,T_EWSC_D,T_EWSC_D&>m_ewsc;
	CMap<T_EWSC_KEY,T_EWSC_KEY,T_EWSC_D,T_EWSC_D&>m_ewscDesign;
};

#endif