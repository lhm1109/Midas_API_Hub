#ifndef __DB_MHTP_DB_H__
#define __DB_MHTP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_MHTP
{
public:
	CDB_MHTP();
	virtual ~CDB_MHTP();

public:
	T_MHTP_K m_nStartNum;
	T_MHTP_K m_nLastNum;

public:
	void Add(T_MHTP_K Key,T_MHTP_D& rData);
	BOOL Del(T_MHTP_K Key);

public:
	BOOL Get(T_MHTP_K Key,T_MHTP_D& rData)
		{return m_mhtp.Lookup(Key,rData);}
	int GetCount()
		{return m_mhtp.GetCount();}
	POSITION GetStart()
		{return m_mhtp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MHTP_K& rKey,T_MHTP_D& rData)
		{m_mhtp.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_MHTP_K,T_MHTP_K,T_MHTP_D,T_MHTP_D&> m_mhtp;
};

#endif
