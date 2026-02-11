#ifndef __DB_HMTP_DB_H__
#define __DB_HMTP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_HMTP
{
public:
	CDB_HMTP();
	virtual ~CDB_HMTP();
	CDBDoc* m_pDoc;

public:
	T_HMTP_K m_nStartNum;
	T_HMTP_K m_nLastNum;

public:
	void Add(T_HMTP_K Key,T_HMTP_D& rData);
	BOOL Del(T_HMTP_K Key);

public:
	BOOL Get(T_HMTP_K Key,T_HMTP_D& rData);
		//{return m_hmtp.Lookup(Key,rData);}
	int GetCount();
		//{return m_hmtp.GetCount();}
	POSITION GetStart();
		//{return m_hmtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_HMTP_K& rKey,T_HMTP_D& rData);
		//{m_hmtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_HMTP_K,T_HMTP_K,T_HMTP_D,T_HMTP_D&>m_hmtp;
};

#endif
