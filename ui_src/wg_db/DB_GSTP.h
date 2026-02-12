#ifndef __DB_GSTP_DB_H__
#define __DB_GSTP_DB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "DB_ST_DT.h"

class CDBDoc;

class CDB_GSTP
{
public:
	CDB_GSTP();
	virtual ~CDB_GSTP();
	CDBDoc* m_pDoc;

public:
	T_GSTP_K m_nStartNum;
	T_GSTP_K m_nLastNum;

public:
	void Add(T_GSTP_K Key,T_GSTP_D& rData);
	BOOL Del(T_GSTP_K Key);

public:
	BOOL GetListFromBaseDB(T_GSTP_K Key,GstpList*& rpList){return m_gstplist.Lookup(Key,rpList);}

	BOOL Get(T_GSTP_K Key,T_GSTP_D& rData)
		{return m_gstp.Lookup(Key,rData);}
	int GetCount()
		{return m_gstp.GetCount();}
	POSITION GetStart()
		{return m_gstp.GetStartPosition();}
	void GetNext(POSITION& rNextPosition,T_GSTP_K& rKey,T_GSTP_D& rData)
		{m_gstp.GetNextAssoc(rNextPosition,rKey,rData);}

	void AddList(T_GSTP_K Key);
	BOOL DelList(T_GSTP_K Key);
	void DelAllList();
	BOOL GetList(T_GSTP_K Key,GstpList*& rpList);//{return m_gstplist.Lookup(Key,rpList);}
	int GetCountList();//{return m_gstplist.GetCount();}
	void AddListItem(T_GSTP_K Key, int nCmd, int nKey);
	BOOL DelListItem(T_GSTP_K Key, int nCmd, int nKey);

protected:
	CMap<T_GSTP_K,T_GSTP_K,T_GSTP_D,T_GSTP_D&>m_gstp;
	CMap<T_GSTP_K,T_GSTP_K,GstpList*,GstpList*>m_gstplist;
};

#endif
