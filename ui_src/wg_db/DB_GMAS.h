#ifndef __DB_GMAS_DB_H__
#define __DB_GMAS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_NODE;

class CDB_GMAS
{
public:
	CDB_GMAS();
	virtual ~CDB_GMAS();
	CDBDoc* m_pDoc;

public:
	void Add(T_GMAS_K Key,T_GMAS_D& rData,CDB_NODE* pNode);
	BOOL Del(T_GMAS_K Key,CDB_NODE* pNode);

public:
	BOOL Get(T_GMAS_K Key,T_GMAS_D& rData);
		//{return m_gmas.Lookup(Key,rData);}
	int GetCount();
		//{return m_gmas.GetCount();}
	POSITION GetStart();
		//{return m_gmas.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GMAS_K& rKey,T_GMAS_D& rData);
		//{m_gmas.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_GMAS_K,T_GMAS_K,T_GMAS_D,T_GMAS_D&> m_gmas;
};

#endif