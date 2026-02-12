#ifndef __DB_BEMW_DB_H__
#define __DB_BEMW_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_BEMW
{
public:
	CDB_BEMW();
	virtual ~CDB_BEMW();
	CDBDoc* m_pDoc;

public:
	void Add(T_BEMW_K Key,T_BEMW_D& rData);
	BOOL Del(T_BEMW_K Key);

public:
	BOOL Get(T_BEMW_K Key,T_BEMW_D& rData);
		//{return m_bemw.Lookup(Key,rData);}
	int GetCount();
		//{return m_bemw.GetCount();}
	POSITION GetStart();
		//{return m_bemw.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_BEMW_K& rKey,T_BEMW_D& rData);
	//{m_bemw.GetNextAssoc(rNextPosition,rKey,rData);}
	
protected:
	CMap<T_BEMW_K,T_BEMW_K,T_BEMW_D,T_BEMW_D&>m_bemw;
};

#endif