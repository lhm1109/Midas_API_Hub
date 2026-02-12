#ifndef __DB_XFAC_DB_H__
#define __DB_XFAC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_SEIS.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_XFAC
{
public:
	CDB_XFAC();
	virtual ~CDB_XFAC();
	CDBDoc* m_pDoc;

public:
	void Add(T_XFAC_K Key,T_XFAC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_XFAC_K Key,CDB_ELEM* pElem);

public:
	BOOL GetFromBaseDB(T_XFAC_K Key,T_XFAC_D& rData)
		{return m_xfac.Lookup(Key,rData);}

	BOOL Get(T_XFAC_K Key,T_XFAC_D& rData);
		//{return m_xfac.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_xfac.GetCount();}
	POSITION GetStart();
		//{return m_xfac.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_XFAC_K& rKey,T_XFAC_D& rData);
		//{m_xfac.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_XFAC_K,T_XFAC_K,T_XFAC_D,T_XFAC_D&> m_xfac;
};

#endif

