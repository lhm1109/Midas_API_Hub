#ifndef __DB_POVC_DB_H__
#define __DB_POVC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_POVC
{
public:
	CDB_POVC();
	virtual ~CDB_POVC();
	CDBDoc* m_pDoc;

public:
	void Add(T_POVC_K Key, T_POVC_D& rData);
	BOOL Del(T_POVC_K Key);

public:
	BOOL Get(T_POVC_K Key, T_POVC_D& rData);
	//{return m_povc.Lookup(Key,rData);}
	int GetCount();
	//{return m_povc.GetCount();}
	POSITION GetStart();
	//{return m_povc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition, T_POVC_K& rKey, T_POVC_D& rData);
	//{m_povc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_POVC_K, T_POVC_K, T_POVC_D, T_POVC_D&>m_povc;
};

#endif

