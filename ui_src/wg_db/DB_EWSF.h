#ifndef __DB_EWSF_DB_H__
#define __DB_EWSF_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;
class CDB_ELEM;
class CDB_BNGR;

class CDB_EWSF
{
public:
	CDB_EWSF();
	virtual ~CDB_EWSF();
	CDBDoc* m_pDoc;

public:
	void Add(T_EWSF_K Key,T_EWSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL Del(T_EWSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	void AddDesign(T_EWSF_K Key,T_EWSF_D& rData,CDB_ELEM* pElem,CDB_BNGR* pBngr);
	BOOL DelDesign(T_EWSF_K Key,CDB_ELEM* pElem,CDB_BNGR* pBngr);

public:
	///// ewsf
	BOOL GetFromBaseDB(T_EWSF_K Key,T_EWSF_D& rData)
		{return m_ewsf.Lookup(Key.keymap,rData);}
	BOOL Get(T_EWSF_K Key,T_EWSF_D& rData);
		//{return m_ewsf.Lookup(Key.keymap,rData);}
	int GetCount();
		//{return m_ewsf.GetCount();}
	POSITION GetStart();
		//{return m_ewsf.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_EWSF_K& rKey,T_EWSF_D& rData);
		//{m_ewsf.GetNextAssoc(rNextPosition,rKey.keymap,rData);}

	///// ewsd
	BOOL GetFromBaseDBDesign(T_EWSF_K Key,T_EWSF_D& rData)
		{return m_ewsf.Lookup(Key.keymap,rData);}
	BOOL GetDesign(T_EWSF_K Key,T_EWSF_D& rData);
	int GetCountDesign();
	POSITION GetStartDesign();
	void GetNextDesign(POSITION& rNextPosition,T_EWSF_K& rKey,T_EWSF_D& rData);

protected:
	CMap<T_EWSF_KEY,T_EWSF_KEY,T_EWSF_D,T_EWSF_D&>m_ewsf;
	CMap<T_EWSF_KEY,T_EWSF_KEY,T_EWSF_D,T_EWSF_D&>m_ewsfDesign;
};

#endif