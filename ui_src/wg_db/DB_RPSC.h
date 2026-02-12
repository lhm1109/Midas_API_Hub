#ifndef __DB_RPSC_DB_H__
#define __DB_RPSC_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_SECT;

class CDB_RPSC
{
public:
	CDB_RPSC();
	virtual ~CDB_RPSC();

public:
	void Add(T_RPSC_K Key,T_RPSC_D& rData,CDB_SECT* pSect);
	BOOL Del(T_RPSC_K Key,CDB_SECT* pSect);
	void AddDesign(T_RPSC_K Key,T_RPSC_D& rData,CDB_SECT* pSect);
	BOOL DelDesign(T_RPSC_K Key,CDB_SECT* pSect);

public:
	// rpsc
	BOOL Get(T_RPSC_K Key,T_RPSC_D& rData)
		{return m_rpsc.Lookup(Key,rData);}
	int GetCount()
		{return m_rpsc.GetCount();}
	POSITION GetStart()
		{return m_rpsc.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_RPSC_K& rKey,T_RPSC_D& rData)
		{m_rpsc.GetNextAssoc(rNextPosition,rKey,rData);}

	// rpsd
	BOOL GetDesign(T_RPSC_K Key,T_RPSC_D& rData)
		{return m_rpscDesign.Lookup(Key,rData);}
	int GetCountDesign()
		{return m_rpscDesign.GetCount();}
	POSITION GetStartDesign()
		{return m_rpscDesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_RPSC_K& rKey,T_RPSC_D& rData)
		{m_rpscDesign.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_RPSC_K,T_RPSC_K,T_RPSC_D,T_RPSC_D&>m_rpsc;
	CMap<T_RPSC_K,T_RPSC_K,T_RPSC_D,T_RPSC_D&>m_rpscDesign;
};

#endif