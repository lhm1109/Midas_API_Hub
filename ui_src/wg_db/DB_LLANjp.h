#ifndef __DB_LLANjp_DB_H__
#define __DB_LLANjp_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_LLANjp
{
public:
	CDB_LLANjp();
	virtual ~CDB_LLANjp();
	CDBDoc* m_pDoc;

public:
	T_LLANjp_K m_nStartNum;
	T_LLANjp_K m_nLastNum;


public:
	void Add(T_LLANjp_K Key, T_LLANjp_D& rData, CDB_ELEM* pElem);
	BOOL Del(T_LLANjp_K Key, CDB_ELEM* pElem);

public:
	//////////////////////////////////////////////////////////////////////////
	BOOL GetFromBaseDB(T_LLANjp_D_OLD& rData)
		{return m_llanjp_Old.Lookup(1,rData);}
	BOOL Get_Old(T_LLANjp_D_OLD& rData)
	    {return m_llanjp_Old.Lookup(1,rData);}
	//////////////////////////////////////////////////////////////////////////

	BOOL Get(T_LLANjp_K Key,T_LLANjp_D& rData);
		//{return m_llanjp.Lookup(1,rData);}
	int GetCount();
		//{return m_llanjp.GetCount();}
	POSITION GetStart();
		//{return m_llanfr.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_LLANjp_K& rKey,T_LLANjp_D& rData);
		//{m_llanfr.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_LLANjp_K,T_LLANjp_K,T_LLANjp_D,T_LLANjp_D&>m_llanjp;
	CMap<T_LLANjp_K,T_LLANjp_K,T_LLANjp_D_OLD, T_LLANjp_D_OLD&>m_llanjp_Old;
};

#endif