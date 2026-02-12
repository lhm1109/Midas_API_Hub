#ifndef __DB_IMPF_DB_H__
#define __DB_IMPF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_IMPF
{
public:
	CDB_IMPF();
	virtual ~CDB_IMPF();
	CDBDoc* m_pDoc;

public:
	void Add(T_IMPF_K Key,T_IMPF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_IMPF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_IMPF_K Key,T_IMPF_D& rData);
		//{return m_impf.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_impf.GetCount();}
	POSITION GetStart();
		//{return m_impf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_IMPF_K& rKey,T_IMPF_D& rData);
		//{m_impf.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_IMPF_KEY,T_IMPF_KEY,T_IMPF_D,T_IMPF_D&>m_impf;
};

#endif