#ifndef __DB_LFCA_DB_H__
#define __DB_LFCA_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;

class CDB_LFCA
{
public:
	CDB_LFCA();
	virtual ~CDB_LFCA();
	CDBDoc* m_pDoc;

public:
	void Add(T_LFCA_K Key,T_LFCA_D& rData);
	BOOL Del(T_LFCA_K Key);

public:
	BOOL Get(T_LFCA_K Key,T_LFCA_D& rData);
		//{return m_lfca.Lookup(Key,rData);}
	int GetCount();
		//{return m_lfca.GetCount();}
	POSITION GetStart();
		//{return m_lfca.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LFCA_K& rKey,T_LFCA_D& rData);
		//{m_lfca.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LFCA_KEY,T_LFCA_KEY,T_LFCA_D,T_LFCA_D&>m_lfca;
};

#endif