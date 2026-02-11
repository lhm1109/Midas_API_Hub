#ifndef __DB_RPSC_SSM_DB_H__
#define __DB_RPSC_SSM_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_RPSC_SSM
{
public:
	CDB_RPSC_SSM();
	virtual ~CDB_RPSC_SSM();

public:
	void Add(T_RPSC_SSM_K Key,T_RPSC_SSM_D& rData,CDB_SECT* pSect);
	BOOL Del(T_RPSC_SSM_K Key,CDB_SECT* pSect);
	void AddDesign(T_RPSC_SSM_K Key,T_RPSC_SSM_D& rData,CDB_SECT* pSect);
	BOOL DelDesign(T_RPSC_SSM_K Key,CDB_SECT* pSect);

public:
	// rpsc_ssm
	BOOL Get(T_RPSC_SSM_K Key,T_RPSC_SSM_D& rData)
		{return m_rpsc_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_rpsc_ssm.GetCount();}
	POSITION GetStart()
		{return m_rpsc_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RPSC_SSM_K& rKey,T_RPSC_SSM_D& rData)
		{m_rpsc_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

	// rpsd_ssm
	BOOL GetDesign(T_RPSC_SSM_K Key,T_RPSC_SSM_D& rData)
		{return m_rpsc_ssmDesign.Lookup(Key,rData);}
	int GetCountDesign()
		{return m_rpsc_ssmDesign.GetCount();}
	POSITION GetStartDesign()
		{return m_rpsc_ssmDesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_RPSC_SSM_K& rKey,T_RPSC_SSM_D& rData)
		{m_rpsc_ssmDesign.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RPSC_SSM_K,T_RPSC_SSM_K,T_RPSC_SSM_D,T_RPSC_SSM_D&>m_rpsc_ssm;
	CMap<T_RPSC_SSM_K,T_RPSC_SSM_K,T_RPSC_SSM_D,T_RPSC_SSM_D&>m_rpsc_ssmDesign;
};

#endif