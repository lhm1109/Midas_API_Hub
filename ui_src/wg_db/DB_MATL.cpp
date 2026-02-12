#include "stdafx.h"
#include "DB_ELEM.h"
#include "DB_MATL.h"
#include "DBDoc.h"
#include "DispOptColorObj.h"
#include "DispOptMgr.h"

#include "..\wg_base\RandomColorUtil.h"   // for CRandomColorUtil

#include "ClassMatl.h"
#include "UnitCtrl.h"

CDB_MATL::CDB_MATL()
{
	m_nStartNum=1;
	m_nLastNum=0;
	m_matl.InitHashTable(HASHSIZEMATL);
	m_matldesign.InitHashTable(HASHSIZEMATL);
	m_matllist.InitHashTable(HASHSIZELISTMATL);
	m_matlcolor.InitHashTable(HASHSIZELISTMATL);
	m_pDoc = CDBDoc::GetDocPoint();
}

CDB_MATL::~CDB_MATL()
{
	DelAllList();
}

CDB_MATL::CDB_MATL(CDB_MATL& src)
{
	*this = src;
}

CDB_MATL& CDB_MATL::operator= (CDB_MATL& src)
{
	POSITION pos;

	m_pDoc = src.m_pDoc;
	m_nStartNum=src.m_nStartNum;
	m_nLastNum=src.m_nLastNum;

	T_MATL_K KeyMatl;
	T_MATL_D DataMatl;
	T_COLOR_D Color;
	m_matl.RemoveAll();
	pos=src.m_matl.GetStartPosition();
	while(pos != NULL)
	{
		src.m_matl.GetNextAssoc(pos,KeyMatl,DataMatl);
		m_matl.SetAt(KeyMatl, DataMatl);
	}
	T_MATD_D DataMatd;
	pos=src.m_matldesign.GetStartPosition();
	while(pos != NULL)
	{
		src.m_matldesign.GetNextAssoc(pos,KeyMatl,DataMatd);
		m_matldesign.SetAt(KeyMatl, DataMatd);
	}
	//CMap<T_MATL_K,T_MATL_K,T_COLOR_D,T_COLOR_D&>m_matlcolor;
	pos=src.m_matlcolor.GetStartPosition();
	while(pos != NULL)
	{
		src.m_matlcolor.GetNextAssoc(pos,KeyMatl,Color);
		m_matlcolor.SetAt(KeyMatl, Color);
	}

	DelAllList();
	MatlList* pList;
	pos=src.m_matllist.GetStartPosition();
	while(pos != NULL)
	{
		src.m_matllist.GetNextAssoc(pos,KeyMatl,pList);
		MatlList* pListNew = new MatlList;
		POSITION pos2=pList->GetHeadPosition();
		while(pos2 != NULL)
		{
			T_LIST_DATA ListData=pList->GetNext(pos2);
			pListNew->AddTail(ListData);
		}
		m_matllist.SetAt(KeyMatl, pListNew);
	}

	return *this;
}

BOOL CDB_MATL::Get(T_MATL_K Key,T_MATL_D& rData)
{
	BOOL bGet = m_matl.Lookup(Key,rData);
//   if (bGet)
//   {
//     T_UNIT_INDEX CurUnitIndex;
//     m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnitIndex);
//     if (CurUnitIndex.nBase_Temper != D_UNITSYS_TEMPER_INDEX_C)
//       CClassMatl::ConvertThermalCoefToFahr(rData);
//   }
	return bGet;
}

void CDB_MATL::GetNext(POSITION& rNextPosition,T_MATL_K& rKey,T_MATL_D& rData)
{
	m_matl.GetNextAssoc(rNextPosition,rKey,rData);
//   T_UNIT_INDEX CurUnitIndex;
//   m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnitIndex);
//   if (CurUnitIndex.nBase_Temper != D_UNITSYS_TEMPER_INDEX_C)
//     CClassMatl::ConvertThermalCoefToFahr(rData);
}

void CDB_MATL::Add(T_MATL_K Key,T_MATL_D& rData)
{
	T_MATL_D data;
	if(!m_matl.Lookup(Key, data))
	{
		T_COLOR_D DataColor;
		DataColor.Initialize();
		
		CRandomColorUtil randomColorUtil;

		if (CDispOptMgr::GetFlagAssignRandomColor())
		{
//       DataColor.WireFrame  = randomColorUtil.GetRandomColor();
//       DataColor.HiddenFill = randomColorUtil.GetRandomColor();
//       DataColor.HiddenEdge = randomColorUtil.GetRandomColor();
			randomColorUtil.GetRandomColor(DataColor.HiddenFill, DataColor.HiddenEdge, DataColor.WireFrame);
		}
		else
		{
			COptColorObjNames COCN;
			/*DataColor.WireFrame = CDBDoc::GetDocPoint()->m_pDispOpt->GetColor(COCN.m_ElemGlobalWire)->m_DefVal;*/
			DataColor.WireFrame = RGB(131, 131, 131);
			/*DataColor.HiddenFill = CDBDoc::GetDocPoint()->m_pDispOpt->GetColor(COCN.m_ElemGlobalHidd)->m_DefVal;*/
			DataColor.HiddenFill = RGB(178, 178, 178);
			/*DataColor.HiddenEdge = CDBDoc::GetDocPoint()->m_pDispOpt->GetColor(COCN.m_ElemGlobalHideEdge)->m_DefVal;*/
			DataColor.HiddenEdge = RGB(131, 131, 131);
		}
		m_matlcolor.SetAt(Key, DataColor);
	}
	m_matl.SetAt(Key,rData);
	if(Key == m_nStartNum)
	{
		while(TRUE)
		{
			m_nStartNum++;
			if(!Get(m_nStartNum,data))break;
		}
	}
	if(Key > m_nLastNum)m_nLastNum=Key;
}

BOOL CDB_MATL::Del(T_MATL_K Key)
{
	BOOL ret=m_matl.RemoveKey(Key);
	ASSERT(ret);
	if(ret)
	{
		VERIFY(m_matlcolor.RemoveKey(Key));
		if(Key < m_nStartNum)m_nStartNum=Key;
		if(Key==m_nLastNum)
		{
			T_MATL_K key;
			T_MATL_D data;
			if(Get(m_nLastNum-1,data))
			{
				m_nLastNum--;
			}
			else
			{
				m_nLastNum=0;
				POSITION pos=GetStart();
				while(pos != NULL)
				{
					GetNext(pos,key,data);
					if(key > m_nLastNum)m_nLastNum=key;
				}
			}
		}
	}
	return ret;
}

void CDB_MATL::AddDesign(T_MATL_K Key,T_MATD_D& rData)
{
	T_MATD_D data;
	m_matldesign.SetAt(Key,rData);
}

BOOL CDB_MATL::DelDesign(T_MATL_K Key)
{
	return m_matldesign.RemoveKey(Key);
}

void CDB_MATL::AddList(T_MATL_K Key)
{
	MatlList* pList = new MatlList;
	m_matllist.SetAt(Key, pList);
}

BOOL CDB_MATL::DelList(T_MATL_K Key)
{
	MatlList* pList;
	BOOL bExist=m_matllist.Lookup(Key,pList);
	ASSERT(bExist);
	if(!bExist)return FALSE;
	m_matllist.RemoveKey(Key);
	delete pList;
	return TRUE;
}

void CDB_MATL::DelAllList()
{
	T_MATL_K Key;
	MatlList* pList;
	POSITION pos=m_matllist.GetStartPosition();
	while(pos != NULL)
	{
		m_matllist.GetNextAssoc(pos,Key,pList);
		delete pList;
	}
	m_matllist.RemoveAll();
}

void CDB_MATL::AddListItem(T_MATL_K Key, int nCmd, int nKey)
{
	MatlList* pList;
	if(!GetList(Key, pList))return;

	T_LIST_DATA ListData;
	ListData.nCmd=nCmd;
	ListData.nKey=nKey;
	pList->AddTail(ListData);
}

BOOL CDB_MATL::DelListItem(T_MATL_K Key, int nCmd, int nKey)
{
	MatlList* pList;
	if(!GetList(Key, pList))return TRUE;
	if(pList->GetCount()<=0)return TRUE;

	T_LIST_DATA ListData;
	POSITION pos=pList->GetHeadPosition(), posPrv;
	while(pos != NULL)
	{
		posPrv=pos;
		ListData=pList->GetNext(pos);
		if(ListData.nCmd == nCmd && ListData.nKey == nKey)
		{
			pList->RemoveAt(posPrv);
			return TRUE;
		}
	}
	return FALSE;
}
