#ifndef __DB_MNTH_DB_H__
#define __DB_MNTH_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_MNTH
{
public:
	CDB_MNTH();
	virtual ~CDB_MNTH();
	CDBDoc* m_pDoc;

public:
	void Add(T_MNTH_K Key,T_MNTH_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MNTH_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_MNTH_K Key,T_MNTH_D& rData);
		//{return m_mnth.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_mnth.GetCount();}
	POSITION GetStart();
		//{return m_mnth.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MNTH_K& rKey,T_MNTH_D& rData);
		//{m_mnth.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_MNTH_K,T_MNTH_K,T_MNTH_D,T_MNTH_D&>m_mnth;
};

#endif