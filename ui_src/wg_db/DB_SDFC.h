#ifndef __DB_SDFC_DB_H__
#define __DB_SDFC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDFC
{
public:
	CDB_SDFC();
	virtual ~CDB_SDFC();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDFC_K Key,T_SDFC_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDFC_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDFC_K Key,T_SDFC_D& rData);
		//{return m_sdfc.Lookup(Key,rData);}
	int GetCount();
		//{return m_sdfc.GetCount();}
	POSITION GetStart();
		//{return m_sdfc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDFC_K& rKey,T_SDFC_D& rData);
		//{m_sdfc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDFC_K,T_SDFC_K,T_SDFC_D,T_SDFC_D&>m_sdfc;
};

#endif

