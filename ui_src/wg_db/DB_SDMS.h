#ifndef __DB_SDMS_DB_H__
#define __DB_SDMS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_SDMS
{
public:
	CDB_SDMS();
	virtual ~CDB_SDMS();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDMS_K Key,T_SDMS_D& rData,CDB_SECT* pSect);
	BOOL Del(T_SDMS_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_SDMS_K Key,T_SDMS_D& rData);
		//{return m_sdms.Lookup(Key,rData);}
	int GetCount();
		//{return m_sdms.GetCount();}
	POSITION GetStart();
		//{return m_sdms.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDMS_K& rKey,T_SDMS_D& rData);
		//{m_sdms.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDMS_K,T_SDMS_K,T_SDMS_D,T_SDMS_D&>m_sdms;
};

#endif

