#ifndef __DB_MHAT_DB_H__
#define __DB_MHAT_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_MATL;

class CDB_MHAT
{
public:
	CDB_MHAT();
	virtual ~CDB_MHAT();
	CDBDoc* m_pDoc;

public:
	void Add(T_MHAT_K Key,T_MHAT_D& rData,CDB_MATL* pMatl);
	BOOL Del(T_MHAT_K Key,CDB_MATL* pMatl);

public:
	BOOL Get(T_MHAT_K Key,T_MHAT_D& rData);
		//{return m_mhat.Lookup(Key,rData);}
	int GetCount();
		//{return m_mhat.GetCount();}
	POSITION GetStart();
		//{return m_mhat.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MHAT_K& rKey,T_MHAT_D& rData);
		//{m_mhat.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MHAT_K,T_MHAT_K,T_MHAT_D,T_MHAT_D&>m_mhat;
};

#endif