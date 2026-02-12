#ifndef __DB_WNAT_DB_H__
#define __DB_WNAT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_WNAT
{
public:
	CDB_WNAT();
	virtual ~CDB_WNAT();
	CDBDoc* m_pDoc;
	
public:
	void Add(T_WNAT_K Key,T_WNAT_D& rData,CDB_STLD* pStld);
	BOOL Del(T_WNAT_K Key,CDB_STLD* pStld);
	
public:
	BOOL Get(T_WNAT_K Key,T_WNAT_D& rData);
	//{return m_wnat.Lookup(Key,rData);}
	int GetCount();
	//{return m_wnat.GetCount();}
	POSITION GetStart();
	//{return m_wnat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_WNAT_K& rKey,T_WNAT_D& rData);
	//{m_wnat.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	CMap<T_WNAT_K,T_WNAT_K,T_WNAT_D,T_WNAT_D&>m_wnat;
};

#endif