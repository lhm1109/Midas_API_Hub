#ifndef __DB_MATL_DB_H__
#define __DB_MATL_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDB_ELEM;
class CDBDoc;

class CDB_MATL
{
public:
	CDB_MATL();
	CDB_MATL(CDB_MATL& src);
	CDB_MATL& operator=(CDB_MATL& src);
	virtual ~CDB_MATL();

public:
	T_MATL_K m_nLastNum;
	T_MATL_K m_nStartNum;

public:
	void Add(T_MATL_K Key,T_MATL_D& rData);
	BOOL Del(T_MATL_K Key);
	void AddDesign(T_MATL_K Key,T_MATD_D& rData);
	BOOL DelDesign(T_MATL_K Key);

public:
	BOOL Get(T_MATL_K Key,T_MATL_D& rData);
		//{return m_matl.Lookup(Key,rData);}
	int GetCount()
		{return (int)m_matl.GetCount();}
	POSITION GetStart()
		{return m_matl.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_MATL_K& rKey,T_MATL_D& rData);
		//{m_matl.GetNextAssoc(rNextPosition,rKey,rData);}

	BOOL GetDesign(T_MATL_K Key,T_MATD_D& rData)
		{return m_matldesign.Lookup(Key,rData);}
	POSITION GetStartDesign()
		{return m_matldesign.GetStartPosition();}
	void GetNextDesign(POSITION& rNextPosition,T_MATL_K& rKey,T_MATD_D& rData)
		{m_matldesign.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_MATL_K Key);
	BOOL DelList(T_MATL_K Key);
	void DelAllList();
	BOOL GetList(T_MATL_K Key,MatlList*& rpList){return m_matllist.Lookup(Key,rpList);}
	int GetCountList(){return (int)m_matllist.GetCount();}
	void AddListItem(T_MATL_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_MATL_K Key, int nCmd, int nKey);

protected:
	CMap<T_MATL_K,T_MATL_K,T_MATL_D,T_MATL_D&>m_matl;
	CMap<T_MATL_K,T_MATL_K,T_MATD_D,T_MATD_D&>m_matldesign;
	CMap<T_MATL_K,T_MATL_K,MatlList*,MatlList*>m_matllist;
	CDBDoc* m_pDoc;
public:
	CMap<T_MATL_K,T_MATL_K,T_COLOR_D,T_COLOR_D&>m_matlcolor;
};

#endif