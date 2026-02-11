#ifndef __DB_HDPP_DB_H__
#define __DB_HDPP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_SECT;

class CDB_HDPP
{
public:
	CDB_HDPP();
	virtual ~CDB_HDPP();
	CDBDoc* m_pDoc;

public:
	void Add(T_HDPP_K Key,T_HDPP_D& rData,CDB_SECT* pSect);
	BOOL Del(T_HDPP_K Key,CDB_SECT* pSect);

public:
	BOOL Get(T_HDPP_K Key,T_HDPP_D& rData);
		//{return m_hdpp.Lookup(Key,rData);}
	int GetCount();
		//{return m_hdpp.GetCount();}
	POSITION GetStart();
		//{return m_hdpp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HDPP_K& rKey,T_HDPP_D& rData);
		//{m_hdpp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HDPP_K,T_HDPP_K,T_HDPP_D,T_HDPP_D&>m_hdpp;
};

#endif

