#ifndef __DB_MFAC_DB_H__
#define __DB_MFAC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT_SEIS.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_MFAC
{
public:
	CDB_MFAC();
	virtual ~CDB_MFAC();
	CDBDoc* m_pDoc;

public:
	void Add(T_MFAC_K Key,T_MFAC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MFAC_K Key,CDB_ELEM* pElem);

public:
	BOOL GetFromBaseDB(T_MFAC_K Key,T_MFAC_D& rData)
		{return m_mfac.Lookup(Key,rData);}

	BOOL Get(T_MFAC_K Key,T_MFAC_D& rData);
		//{return m_mfac.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_mfac.GetCount();}
	POSITION GetStart();
		//{return m_mfac.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MFAC_K& rKey,T_MFAC_D& rData);
		//{m_mfac.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_MFAC_K,T_MFAC_K,T_MFAC_D,T_MFAC_D&>m_mfac;
};

#endif

