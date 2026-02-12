#ifndef __DB_ESSF_DB_H__
#define __DB_ESSF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_ESSF
{
public:
	CDB_ESSF();
	virtual ~CDB_ESSF();
	CDBDoc* m_pDoc;

public:
	void Add(T_ESSF_K Key,T_ESSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_ESSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	BOOL GetFromBaseDB(T_ESSF_K Key,T_ESSF_D& rData)
		{return m_essf.Lookup(Key.keymap,rData);}
	
	BOOL Get(T_ESSF_K Key,T_ESSF_D& rData);
		//{return m_essf.Lookup(Key,rData);}
	int GetCount();
		//{return m_essf.GetCount();}
	POSITION GetStart();
		//{return m_essf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_ESSF_K& rKey,T_ESSF_D& rData);
		//{m_essf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_ESSF_KEY,T_ESSF_KEY,T_ESSF_D,T_ESSF_D&>m_essf;
};

#endif