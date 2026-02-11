#ifndef __DB_MBTP_DB_H__
#define __DB_MBTP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_MBTP
{
public:
	CDB_MBTP();
	virtual ~CDB_MBTP();
	CDBDoc* m_pDoc;

public:
	void Add(T_MBTP_K Key,T_MBTP_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_MBTP_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_MBTP_K Key,T_MBTP_D& rData);
		//{return m_mbtp.Lookup(Key,rData);}
	int GetCount();
		//{return m_mbtp.GetCount();}
	POSITION GetStart();
		//{return m_mbtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MBTP_K& rKey,T_MBTP_D& rData);
		//{m_mbtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MBTP_K,T_MBTP_K,T_MBTP_D,T_MBTP_D&>m_mbtp;
};

#endif

