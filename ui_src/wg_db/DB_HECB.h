#ifndef __DB_HECB_DB_H__
#define __DB_HECB_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_HECB
{
public:
	CDB_HECB();
	virtual ~CDB_HECB();
	CDBDoc* m_pDoc;

public:
	void Add(T_HECB_K Key,T_HECB_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_HECB_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_HECB_K Key,T_HECB_D& rData)
		{return m_hecb.Lookup(Key.keymap,rData);}

	BOOL Get(T_HECB_K Key,T_HECB_D& rData);
		//{return m_hecb.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_hecb.GetCount();}
	POSITION GetStart();
		//{return m_hecb.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HECB_K& rKey,T_HECB_D& rData);
		//{m_hecb.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_HECB_KEY,T_HECB_KEY,T_HECB_D,T_HECB_D&>m_hecb;
};

#endif