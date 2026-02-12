#ifndef __DB_MDFS_DB_H__
#define __DB_MDFS_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_MDFS
{
public:
	CDB_MDFS();
	virtual ~CDB_MDFS();
	CDBDoc* m_pDoc;

public:
	void Add(T_MDFS_K Key,T_MDFS_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MDFS_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_MDFS_K Key,T_MDFS_D& rData);
		//{return m_mdfs.Lookup(Key,rData);}
	int GetCount();
		//{return m_mdfs.GetCount();}
	POSITION GetStart();
		//{return m_mdfs.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MDFS_K& rKey,T_MDFS_D& rData);
		//{m_mdfs.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MDFS_K,T_MDFS_K,T_MDFS_D,T_MDFS_D&>m_mdfs;
};

#endif

