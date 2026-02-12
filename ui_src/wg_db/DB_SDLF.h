#ifndef __DB_SDLF_DB_H__
#define __DB_SDLF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;

class CDB_SDLF
{
public:
	CDB_SDLF();
	virtual ~CDB_SDLF();
	CDBDoc* m_pDoc;

public:
	void Add(T_SDLF_K Key,T_SDLF_D& rData,CDB_ELEM* pElem);
	BOOL Del(T_SDLF_K Key,CDB_ELEM* pElem);

public:
	BOOL Get(T_SDLF_K Key,T_SDLF_D& rData);
		//{return m_sdlf.Lookup(Key,rData);}
	int GetCount();
		//{return m_sdlf.GetCount();}
	POSITION GetStart();
		//{return m_sdlf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_SDLF_K& rKey,T_SDLF_D& rData);
		//{m_sdlf.GetNextAssoc(rNextPosition,rKey,rData);}

protected:
	CMap<T_SDLF_K,T_SDLF_K,T_SDLF_D,T_SDLF_D&>m_sdlf;
};

#endif

