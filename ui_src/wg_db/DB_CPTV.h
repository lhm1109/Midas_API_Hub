#ifndef __DB_CPTV_DB_H__
#define __DB_CPTV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_CPTV
{
public:
	CDB_CPTV();
	virtual ~CDB_CPTV();
	CDBDoc* m_pDoc;

public:
	void Add(T_CPTV_K Key, T_CPTV_D& rData);
	BOOL Del(T_CPTV_K Key);

public:
	BOOL Get(T_CPTV_K Key, T_CPTV_D& rData);
	//{return m_cptv.Lookup(Key,rData);}
	int GetCount();
	//{return m_cptv.GetCount();}
	POSITION GetStart();
	//{return m_cptv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_CPTV_K& rKey, T_CPTV_D& rData);
	//{m_cptv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_CPTV_K, T_CPTV_K, T_CPTV_D, T_CPTV_D&>m_cptv;
};

#endif

