#ifndef __DB_SSPS_DB_H__
#define __DB_SSPS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_SSPS
{
public:
	CDB_SSPS();
	virtual ~CDB_SSPS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SSPS_K Key,T_SSPS_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_SSPS_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_SSPS_K Key,T_SSPS_D& rData)
		{return m_ssps.Lookup(Key.keymap,rData);}

	BOOL Get(T_SSPS_K Key,T_SSPS_D& rData);
		//{return m_ssps.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_ssps.GetCount();}
	POSITION GetStart();
		//{return m_ssps.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SSPS_K& rKey,T_SSPS_D& rData);
		//{m_ssps.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_SSPS_KEY,T_SSPS_KEY,T_SSPS_D,T_SSPS_D&>m_ssps;
};

#endif
