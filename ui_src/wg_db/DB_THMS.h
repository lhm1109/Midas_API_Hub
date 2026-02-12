#ifndef __DB_THMS_DB_H__
#define __DB_THMS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_THIS;
class CDB_NODE;

class CDB_THMS
{
public:
	CDB_THMS();
	virtual ~CDB_THMS();
	CDBDoc* m_pDoc;

public:
	void Add(T_THMS_K Key,T_THMS_D& rData,CDB_THIS* pThis, CDB_NODE* pNode);
	BOOL Del(T_THMS_K Key,CDB_THIS* pThis, CDB_NODE* pNode);

public:
	BOOL Get(T_THMS_K Key,T_THMS_D& rData);
		//{return m_thms.Lookup(Key,rData);}
	int GetCount();
		//{return m_thms.GetCount();}
	POSITION GetStart();
		//{return m_thms.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_THMS_K& rKey,T_THMS_D& rData);
		//{m_thms.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_THMS_KEY,T_THMS_KEY,T_THMS_D,T_THMS_D&>m_thms;
};

#endif