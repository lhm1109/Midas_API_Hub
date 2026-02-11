#ifndef __DB_RPSC_SSMU_DB_H__
#define __DB_RPSC_SSMU_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_RPSC_SSMU
{
public:
	CDB_RPSC_SSMU()
	{
		m_rpsc_ssm.InitHashTable(HASHSIZERPSCSSM);
		m_rpsc_ssmDesign.InitHashTable(HASHSIZERPSCSSM);
	}
	virtual ~CDB_RPSC_SSMU(){};

public:
	// rpsc_ssm
	void Add(T_UDRD_KEY Key,T_RPSC_SSM_UDRD_D& rData)
		{m_rpsc_ssm.SetAt(Key,rData);}
	BOOL Del(T_UDRD_KEY Key)
		{return m_rpsc_ssm.RemoveKey(Key);}
	void DelAll()
		{m_rpsc_ssm.RemoveAll();}
	BOOL Get(T_UDRD_KEY Key,T_RPSC_SSM_UDRD_D& rData)
		{return m_rpsc_ssm.Lookup(Key,rData);}
	int GetCount()
		{return m_rpsc_ssm.GetCount();}
	POSITION GetStart()
		{return m_rpsc_ssm.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RPSC_SSM_UDRD_D& rData)
		{m_rpsc_ssm.GetNextAssoc(rNextPosition,rKey,rData);}

	// rpsc_ssm design
	void AddDesign(T_UDRD_KEY Key,T_RPSC_SSM_UDRD_D& rData)
		{m_rpsc_ssmDesign.SetAt(Key,rData);}
	BOOL DelDesign(T_UDRD_KEY Key)
		{return m_rpsc_ssmDesign.RemoveKey(Key);}
	void DelAllDesign()
		{m_rpsc_ssmDesign.RemoveAll();}
	BOOL GetDesign(T_UDRD_KEY Key,T_RPSC_SSM_UDRD_D& rData)
		{return m_rpsc_ssmDesign.Lookup(Key,rData);}
	int GetCountDesign()
		{return m_rpsc_ssmDesign.GetCount();}
	POSITION GetStartDesign()
		{return m_rpsc_ssmDesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_UDRD_KEY& rKey,T_RPSC_SSM_UDRD_D& rData)
		{m_rpsc_ssmDesign.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPSC_SSM_UDRD_D,T_RPSC_SSM_UDRD_D&>m_rpsc_ssm;
	 CMap<T_UDRD_KEY,T_UDRD_KEY,T_RPSC_SSM_UDRD_D,T_RPSC_SSM_UDRD_D&>m_rpsc_ssmDesign;
};

#endif
