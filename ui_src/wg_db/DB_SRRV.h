#ifndef __DB_SRRV_DB_H__
#define __DB_SRRV_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_SRRV
{
public:
	CDB_SRRV();
	virtual ~CDB_SRRV();
	CDBDoc* m_pDoc;

public:
	void Add(T_SRRV_K Key, T_SRRV_D& rData);
	BOOL Del(T_SRRV_K Key);

public:
	BOOL Get(T_SRRV_K Key, T_SRRV_D& rData);
	//{return m_srrv.Lookup(Key,rData);}
	int GetCount();
	//{return m_srrv.GetCount();}
	POSITION GetStart();
	//{return m_srrv.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_SRRV_K& rKey, T_SRRV_D& rData);
	//{m_srrv.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SRRV_K, T_SRRV_K, T_SRRV_D, T_SRRV_D&>m_srrv;
};

#endif

