#ifndef __DB_NTMP_DB_H__
#define __DB_NTMP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_STLD;
class CDB_NODE;
class CDB_LDGR;

class CDB_NTMP
{
public:
	CDB_NTMP();
	virtual ~CDB_NTMP();
	CDBDoc* m_pDoc;

public:
	void Add(T_NTMP_K Key,T_NTMP_D& rData,CDB_STLD* pStld,CDB_NODE* pNode,CDB_LDGR* pLdgr);
	BOOL Del(T_NTMP_K Key,CDB_STLD* pStld,CDB_NODE* pNode,CDB_LDGR* pLdgr);

public:
	BOOL GetFromBaseDB(T_NTMP_K Key,T_NTMP_D& rData)
		{return m_ntmp.Lookup(Key.keymap,rData);}

	BOOL Get(T_NTMP_K Key,T_NTMP_D& rData);
		//{return m_ntmp.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_ntmp.GetCount();}
	POSITION GetStart();
		//{return m_ntmp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_NTMP_K& rKey,T_NTMP_D& rData);
		//{m_ntmp.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

protected:
	CMap<T_NTMP_KEY,T_NTMP_KEY,T_NTMP_D,T_NTMP_D&>m_ntmp;
};

#endif