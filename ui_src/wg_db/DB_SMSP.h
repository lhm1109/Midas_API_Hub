#ifndef __DB_SMSP_DB_H__
#define __DB_SMSP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SMSP
{
public:
	CDB_SMSP();
	virtual ~CDB_SMSP();
	CDBDoc* m_pDoc;

public:
	void Add(T_SMSP_K Key,T_SMSP_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_SMSP_K Key,CDB_ELEM* pElem);  

public:
	BOOL Get(T_SMSP_K Key,T_SMSP_D& rData);
	//{return m_Smsp.Lookup(Key,rData);}
	int GetCount();
	//{return m_Smsp.GetCount();}
	POSITION GetStart();
	//{return m_Smsp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SMSP_K& rKey,T_SMSP_D& rData);
	//{m_Smsp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SMSP_K,T_SMSP_K,T_SMSP_D,T_SMSP_D&>m_Smsp;
};

#endif
