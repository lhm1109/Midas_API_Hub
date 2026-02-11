#ifndef __DB_STRN_SSM_DB_H__
#define __DB_STRN_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_STRN_SSM
{
public:
	CDB_STRN_SSM();
	virtual ~CDB_STRN_SSM();

public:
	void Add(T_STRN_SSM_K Key,T_STRN_SSM_D& rData,CDB_SECT* pSect);
	BOOL Del(T_STRN_SSM_K Key,CDB_SECT* pSect);
	void AddDesign(T_STRN_SSM_K Key,T_STRN_SSM_D& rData,CDB_SECT* pSect);
	BOOL DelDesign(T_STRN_SSM_K Key,CDB_SECT* pSect);

public:
	// STRN_SSM
	BOOL Get(T_STRN_SSM_K Key,T_STRN_SSM_D& rData)
		{return m_strn_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_strn_ssm.GetCount();}
	POSITION GetStart()
		{return m_strn_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_STRN_SSM_K& rKey,T_STRN_SSM_D& rData)
		{m_strn_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_STRN_SSM_K,T_STRN_SSM_K,T_STRN_SSM_D,T_STRN_SSM_D&>m_strn_ssm;
};

#endif