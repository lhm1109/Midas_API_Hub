#ifndef __DB_RKLC_DB_H__
#define __DB_RKLC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_RKLC
{
public:
	CDB_RKLC();
	virtual ~CDB_RKLC();
	CDBDoc* m_pDoc;

public:
	void Add(T_RKLC_K Key,T_RKLC_D& rData);
	BOOL Del(T_RKLC_K Key);

public:
	BOOL Get(T_RKLC_K Key,T_RKLC_D& rData);
		//{return m_rklc.Lookup(Key,rData);}
	int GetCount();
		//{return m_rklc.GetCount();}
	POSITION GetStart();
		//{return m_rklc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RKLC_K& rKey,T_RKLC_D& rData);
		//{m_rklc.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RKLC_K,T_RKLC_K,T_RKLC_D,T_RKLC_D&>m_rklc;
};

#endif
