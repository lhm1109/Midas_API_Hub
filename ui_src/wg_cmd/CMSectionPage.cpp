// CMSectionPage.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectionPage.h"

#include "..\wg_base\wg_base_Product.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_SectUtil.h"

#include "CMSectItemDlg.h"
#include "CMImportPropertyDlg.h"
#include "CMPropRenumDlg.h"
#include "CMPropertiesDlg.h"

// Coded by Seungjun('20061018)
#include "..\wg_db\IUsageCounter.h"

#define COLCOUNT 4

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSectionPageBase class

CCMSectionPageBase::CCMSectionPageBase(CDBDoc* pDoc, BOOL bDesign, CWnd* pParent/*=NULL*/)
{
	m_pDoc = pDoc;
	m_Data.Initialize();
	m_bDesign = bDesign;
	m_bVirtual = FALSE;
	m_bHideUDS = FALSE; // show all design sections
	m_pParent = (CCMPropertiesDlg*) pParent;

	//{{AFX_DATA_INIT(CCMSectionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCMSectionPageBase::SetVirtual(BOOL bVirtual)
{
	m_bVirtual = bVirtual;
}

//-----------------------------------------------------------------------------
// CCMSectionPageBase interface functions
POSITION CCMSectionPageBase::GetStartSect(CDBDoc* pDoc, BOOL bDesign, BOOL bVirtual)
{
	if (bDesign)
	{
		if(bVirtual)
			return pDoc->m_pAttrCtrl2->GetStartSecv();
		else
			return pDoc->m_pAttrCtrl->GetStartSectDesign();
	}
	else return pDoc->m_pAttrCtrl->GetStartSect();
}

void CCMSectionPageBase::GetNextSect(CDBDoc* pDoc, BOOL bDesign, BOOL bVirtual, POSITION &pos, T_SECT_K &Key, T_SECT_D &Data)
{
	if (bDesign)
	{
		if (bVirtual)
		{
			T_SECV_D VData;
			pDoc->m_pAttrCtrl2->GetNextSecv(pos, Key, VData);
			VData.ConvertToSect(Data);
		}
		else
			pDoc->m_pAttrCtrl->GetNextSectDesign(pos, Key, Data);
	}
	else pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
}

BOOL CCMSectionPageBase::GetSect(CDBDoc* pDoc, BOOL bDesign, BOOL bVirtual, T_SECT_K Key, T_SECT_D &Data)
{
	if (bDesign)
	{
		if (bVirtual)
		{
			T_SECV_D VData;
			if (!pDoc->m_pAttrCtrl2->GetSecv(Key, VData)) { ASSERT(0); return FALSE; }
			VData.ConvertToSect(Data);
			return TRUE;
		}
		else
			return pDoc->m_pAttrCtrl->GetSectDesign(Key, Data);
	}
	else return pDoc->m_pAttrCtrl->GetSect(Key, Data);
}

//-----------------------------------------------------------------------------
// CCMSectionPageBase implementation functions
void CCMSectionPageBase::SetHeaderTitle(T_UNIT_SYSTEM &UnitSystem, BOOL bInit)
{
	
	CString aTitle[] = { _LS(IDS_CMD_SECTION_id), _LS(IDS_CMD_SECTION_name),
															_LS(IDS_CMD_SECTION_type), _LS(IDS_CMD_SECTION_shape) };
	
	
//18japan  
//  static CString aTitle[] = {_T("ID"), _LS(IDS_WG_CMD__ADDD__Name), _LS(IDS_WG_CMD__ADDD__Type), _T("Shape")};
	int nColWidth[COLCOUNT];
	CString title;
	int i;
	LV_COLUMN lvcolumn;

	if (bInit)
	{
		DWORD dwStyle;
		dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	  dwStyle |= LVS_EX_FULLROWSELECT; 
	  ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);
	}

	// Calculate width for each column
	nColWidth[0] = 45; nColWidth[1] = 130; nColWidth[2] = 60; nColWidth[3] = 50;

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		// 제일 처음 시작일 때면 Column 추가
		// 아니면 Column Title만 변경

		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		if (i == 0) lvcolumn.fmt = LVCFMT_RIGHT;
		else lvcolumn.fmt = LVCFMT_LEFT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		if (bInit)
			m_List.InsertColumn(i,&lvcolumn);
		m_List.SetColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCMSectionPageBase::MakeItemEx(BOOL bUnitChanged)
{
	CWaitCursor Cursor;

	if(!bUnitChanged)
		m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSect();
	if(m_bVirtual)
		nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSecv();


	if(nItemCount == 0) return;

	LV_ITEM lvitem;
	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_SECT_K Key;
		T_SECT_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		//*^^*pos = m_pDoc->m_pAttrCtrl->GetStartSect();
		pos = GetStartSect(m_pDoc, m_bDesign, m_bVirtual);
		while(pos !=NULL)
		{
		  //*^^*m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
			GetNextSect(m_pDoc, m_bDesign, m_bVirtual, pos, Key, Data);
		  KeyBuf[nCount++] = Key;
		}
		qsort(KeyBuf, nItemCount, sizeof(T_SECT_K), CCompFunc::UINTAsc);

		int ix = 0;
		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key=KeyBuf[nCount];
			//*^^*m_pDoc->m_pAttrCtrl->GetSect(Key,Data);
			GetSect(m_pDoc, m_bDesign, m_bVirtual, Key, Data);
			if (m_bDesign && m_bHideUDS && IsUnmodifiedDesignSection(Key, Data))
				continue;
			
			for(int i = 0; i < COLCOUNT; i++)
			{
				lvitem.iItem=ix;
				lvitem.iSubItem=i;
				str = DataToStr(i, Key, Data);
				lvitem.pszText=str.GetBuffer(0);
				lvitem.mask=LVIF_TEXT ;
			  if(!bUnitChanged)
			  {
				  if( i == 0 )
					  m_List.InsertItem(&lvitem);
				  else
					  m_List.SetItem(&lvitem);
			  }
			  else
			  {
				  m_List.SetItemText(lvitem.iItem,lvitem.iSubItem,lvitem.pszText);
			  }
			  str.ReleaseBuffer();
			}
			ix++;
		}
		delete []KeyBuf;
	}
}

void CCMSectionPageBase::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_SECT_K Key, KeyBak;
	T_SECT_D Data, DataBak;

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		if (m_bDesign && !m_bVirtual)
		{
		  switch(nCmd)
		  {
		  case(UR_SECD_ADD):
					// 삽입될 위치(nRow)를 찾아 넣는다.
					pViewBuff->GetSecd(nKey, Key, Data);
					InsertItem(Key, Data);
					break;
		  case(UR_SECD_DEL):
					// 삭제될 위치(nRow)를 찾아서 지운다.
					pViewBuff->GetSecd(nKey, Key, Data);
					DeleteItem(Key, Data);
					break;
			case(UR_SECD_MFD):
					// 여기서는 저장만 하고 UR_SECT_MFS에서 처리한다.
					pViewBuff->GetSecd(nKey, Key, Data);
					KeyBak = Key;
					DataBak = Data;
					bMFD = TRUE;
					break;
			case(UR_SECD_MFS):
					// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
					ASSERT(bMFD);
					pViewBuff->GetSecd(nKey, Key, Data);
					ModifyItem(Key, KeyBak, DataBak);
					// 플래그 수정
					bMFD = FALSE;
					break;
		  default:
			  break;
		  }
		}
		else if (m_bDesign && m_bVirtual)
		{
			T_SECV_D VData;
			switch (nCmd)
			{
			case(UR_SECV_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetSecv(nKey, Key, VData); 
				VData.ConvertToSect(Data);
				InsertItem(Key, Data);
				break;
			case(UR_SECV_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetSecv(nKey, Key, VData);
				VData.ConvertToSect(Data);
				DeleteItem(Key, Data);
				break;
			case(UR_SECV_MFD):
				// 여기서는 저장만 하고 UR_SECT_MFS에서 처리한다.
				pViewBuff->GetSecv(nKey, Key, VData);
				VData.ConvertToSect(Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
			case(UR_SECV_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetSecv(nKey, Key, VData);
				VData.ConvertToSect(Data);
				ModifyItem(Key, KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;
			default:
				break;
			}
		}
		else
		{
		  switch(nCmd)
		  {
		  case(UR_SECT_ADD):
					// 삽입될 위치(nRow)를 찾아 넣는다.
					pViewBuff->GetSect(nKey, Key, Data);
					InsertItem(Key, Data);
					break;
		  case(UR_SECT_DEL):
					// 삭제될 위치(nRow)를 찾아서 지운다.
					pViewBuff->GetSect(nKey, Key, Data);
					DeleteItem(Key, Data);
					break;
			case(UR_SECT_MFD):
					// 여기서는 저장만 하고 UR_SECT_MFS에서 처리한다.
					pViewBuff->GetSect(nKey, Key, Data);
					KeyBak = Key;
					DataBak = Data;
					bMFD = TRUE;
					break;
			case(UR_SECT_MFS):
					// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
					ASSERT(bMFD);
					pViewBuff->GetSect(nKey, Key, Data);
					ModifyItem(Key, KeyBak, DataBak);
					// 플래그 수정
					bMFD = FALSE;
					break;
		  default:
			  break;
		  }
		}
	} // end of while
}

void CCMSectionPageBase::UnitChanged()
{
	T_UNIT_SYSTEM UnitSystem;
	SetHeaderTitle(UnitSystem);
	MakeItemEx(TRUE);
}

BOOL CCMSectionPageBase::InsertItem(T_SECT_K Key, T_SECT_D &Data)
{
	if (m_bDesign && m_bHideUDS && IsUnmodifiedDesignSection(Key, Data))
		return TRUE;

	LVITEM lvitem;
	CString str;
	int nItem = m_List.GetItemCount();
	for(int i = 0; i < COLCOUNT; i++)
	{
		lvitem.iItem = nItem;
		lvitem.iSubItem=i;
		str = DataToStr(i, Key, Data);
		lvitem.pszText=str.GetBuffer(0);
		lvitem.mask=LVIF_TEXT;

		if( i == 0 ) nItem = m_List.InsertItem(&lvitem);
		else m_List.SetItem(&lvitem);

	  str.ReleaseBuffer();
	}

	// [GEN-1808] 업데이트 후 리스트 스크롤 유지
	m_List.EnsureVisible(m_List.GetItemCount() - 1, TRUE); // Scroll down to the bottom
	m_List.EnsureVisible(nItem, TRUE);// scroll back up just enough to show said item on top

	return TRUE;
}

BOOL CCMSectionPageBase::DeleteItem(T_SECT_K Key, T_SECT_D &Data)
{
	CString str;
	str.Format(_T("%6d"), Key);

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = str.GetBuffer(0);
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	str.ReleaseBuffer();
	return TRUE;
}

BOOL CCMSectionPageBase::ModifyItem(T_SECT_K KeyOld, T_SECT_K Key, T_SECT_D &Data)
{
	// HideUDS가 True인 상태에서 새로 수정된 것이 UDS이면 삭제만 하면 됨
	// 삭제시 Data는 의미 없음
	if (m_bDesign && m_bHideUDS && IsUnmodifiedDesignSection(Key, Data))
		return DeleteItem(KeyOld, Data);

	CString str;
	str.Format(_T("%6d"), KeyOld);

	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_STRING;
	FindInfo.psz = str.GetBuffer(0);
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = DataToStr(i, Key, Data);
			m_List.SetItemText(nItem, i, str);
		}
	}
	else
	{
		// 기존 List에 없는 경우 -> HideUDS가 TRUE인 상태에서 Sect4Design이 변경된 경우
		InsertItem(Key, Data);
	}
	str.ReleaseBuffer();
	return TRUE;
}

CString CCMSectionPageBase::DataToStr(int i, T_SECT_K Key, T_SECT_D &Data)
{
	CString str;

	if(i==0)str.Format(_T("%6d"),Key);
	else if(i==1) str = Data.SName;
	else if(i==2) 
	{
		switch(Data.nStype)
		{
		case D_SECT_TYPE_REGULAR: 
			{
				if (Data.SectBefore.SectI.DBName == _T("")) str = _LS(IDS_WG_CMD__ADD2__User);
				else str =  _LS(IDS_WG_CMD__ADD2__DB);
			}
			break;
		case D_SECT_TYPE_USER:           str = _LS(IDS_WG_CMD__ADDD__Value);       break;
		case D_SECT_TYPE_SRC:            str = _LS(IDS_WG_CMD__ADDD_SECT_SRC);     break;
		case D_SECT_TYPE_COMBINED:       str = _LS(IDS_WG_CMD__ADDD__Combined);    break;
		case D_SECT_TYPE_TAPERED:        str = _LS(IDS_WG_CMD__ADDD__Tapered);     break;
		case D_SECT_TYPE_CONSTRUCTION:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_HIBEAM:         str = _LS(IDS_WG_CMD__ADDD__Hi_Beam);     break;
		case D_SECT_TYPE_COLDFORMED:     str = _LS(IDS_WG_CMD__ADDD__Cold_Formed); break;
		case D_SECT_TYPE_COMPO_B:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_I:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_TUB:      str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_B:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_I:   str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_STLG_TUB: str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CI:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_CT:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_PC:       str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_COMPO_G:        str = _LS(IDS_WG_CMD__ADDD__Composite);   break;
		case D_SECT_TYPE_PSC:            str = _LS(IDS_WG_CMD__ADDD__PSC);         break;
		case D_SECT_TYPE_STLG_B:         str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		case D_SECT_TYPE_STLG_I:         str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		case D_SECT_TYPE_STLG_MCELL:     str = _LS(IDS_WG_CMD__ADDD__STLGIRDER);   break;
		}
	}
	else if(i==3) 
	{
		str = Data.SectBefore.Shape;
		CSectUtil::CustomizeShapeNameByLanguage(Data.nStype, str, FALSE);
		/*
		//if (CLocaleLib::IsEnglish())  // 영어권만 (일본은 H로 씀)
		#if defined(_US)
			str.Replace('H', 'I');
		#endif
		*/
	}
	else str = _LS(IDS_WG_CMD__ADDD__Error);

	return str;
}

BOOL CCMSectionPageBase::IsUnmodifiedDesignSection(T_SECT_K Key, T_SECT_D& rData)
{
	BOOL bDesignSect = FALSE;
	if (rData.nStype == D_SECT_TYPE_REGULAR ||
			rData.nStype == D_SECT_TYPE_SRC     ||
			(rData.nStype == D_SECT_TYPE_TAPERED && rData.SectBefore.nStype == D_SECT_TYPE_REGULAR))
			bDesignSect = TRUE;
	if (!bDesignSect) return TRUE;

	T_SECT_D Sect;
	if (!m_pDoc->m_pAttrCtrl->GetSect(Key, Sect)) { ASSERT(0); return FALSE; }
	if (Sect.nStype != rData.nStype) return FALSE;
	if (Sect.SName != rData.SName) return FALSE;
	if (Sect.SectBefore.Shape != rData.SectBefore.Shape) return FALSE;
	if (Sect.SectBefore.SectI.BuiltUpFlag != rData.SectBefore.SectI.BuiltUpFlag) return FALSE;
	if (Sect.SectBefore.SectI.DBName != rData.SectBefore.SectI.DBName) return FALSE;
	if (Sect.SectBefore.SectI.SName != rData.SectBefore.SectI.SName) return FALSE;

	for (int i = 0; i < 6; i++)
		if (Sect.SectBefore.SectI.Size[i] != rData.SectBefore.SectI.Size[i]) return FALSE;
	if (rData.nStype == D_SECT_TYPE_TAPERED || rData.nStype == D_SECT_TYPE_SRC)
	{
		for (int i = 0; i < 6; i++)
			if (Sect.SectBefore.SectJ.Size[i] != rData.SectBefore.SectJ.Size[i]) return FALSE;
	}
	if (rData.nStype == D_SECT_TYPE_TAPERED)
	{
		if (Sect.SectBefore.nLytype != rData.SectBefore.nLytype) return FALSE;
		if (Sect.SectBefore.nLztype != rData.SectBefore.nLztype) return FALSE;
	}
	if (rData.nStype == D_SECT_TYPE_SRC)
	{
		if (Sect.SectBefore.Matl_Density != rData.SectBefore.Matl_Density) return FALSE;
		if (Sect.SectBefore.Matl_Elast != rData.SectBefore.Matl_Elast) return FALSE;
		if (rData.SectBefore.Matl_Poisson_C != rData.SectBefore.Matl_Poisson_C) return FALSE;
		if (rData.SectBefore.Matl_Poisson_S != rData.SectBefore.Matl_Poisson_S) return FALSE;
		if (rData.SectBefore.nSRCtype != rData.SectBefore.nSRCtype) return FALSE;
	}
	return TRUE;  // 수정 안된 것, Design에 사용되지 않는 것은 TRUE 리턴
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectionPageBase message handlers

BOOL CCMSectionPageBase::OnInitDialog(CWnd* pwndPreview) 
{
	// TODO: Add extra initialization here
	T_UNIT_SYSTEM UnitSystem;
	SetHeaderTitle(UnitSystem, TRUE);
	MakeItemEx();
	
	// attach section preview window to dialog page
	ASSERT(pwndPreview);
	m_wndSecView.Init(pwndPreview);
	m_wndSecView.SetCentroidFlag(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMSectionPageBase::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMSectItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetMode(m_bDesign);
	dlg.SetVirual(m_bVirtual);
	dlg.DoModal();
}

void CCMSectionPageBase::OnModify() 
{
	// TODO: Add your control notification handler code here
	T_SECT_K Key;
	T_SECT_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}
 
	Key = _tstol(m_List.GetItemText(iItem, 0));
	//*^^*if (!m_pDoc->m_pAttrCtrl->GetSect(Key, Data)) return;
	if (!GetSect(m_pDoc, m_bDesign, m_bVirtual, Key, Data)) return;

	CCMSectItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetMode(m_bDesign);
	dlg.SetVirual(m_bVirtual);
	dlg.SetModifyData(Key, Data);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMSectionPageBase::OnDelete() 
{
	// TODO: Add your control notification handler code here
	T_SECT_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}
	Key = _tstol(m_List.GetItemText(iItem, 0));
	m_pDoc->m_pDataCtrl->DelSect(Key);

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);
}

void CCMSectionPageBase::OnCopy() 
{
	// TODO: Add your control notification handler code here
	T_SECT_K Key;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}
	Key = _tstol(m_List.GetItemText(iItem, 0));
	m_pDoc->m_pDataCtrl->CopySect(Key);
}

void CCMSectionPageBase::OnUpdateDB(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		UnitChanged();
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCMSectionPageBase::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
		m_Data.Initialize();
	else
	{
		m_Key = _tstol(m_List.GetItemText(iItem, 0));
		//*^^*if (!m_pDoc->m_pAttrCtrl->GetSect(m_Key, m_Data)) return;
		if (!GetSect(m_pDoc, m_bDesign, m_bVirtual, m_Key, m_Data)) return;
		ShowSizeTip(m_Data);
	}
	m_wndSecView.SetDataSource(&m_Data);
	m_wndSecView.Invalidate();

	*pResult = 0;
}

void CCMSectionPageBase::OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	T_SECT_K Key;
	T_SECT_D Data;

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return; // no message, just return
 
	Key = _tstol(m_List.GetItemText(iItem, 0));
	//if (!m_pDoc->m_pAttrCtrl->GetSect(Key, Data)) return;
	if (!GetSect(m_pDoc, m_bDesign, m_bVirtual, Key, Data)) return;

	CCMSectItemDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.SetMode(m_bDesign);
	dlg.SetVirual(m_bVirtual);
	dlg.SetModifyData(Key, Data);
	dlg.DoModal();

	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	
	*pResult = 0;
}



/////////////////////////////////////////////////////////////////////////////
// CCMSectionPage dialog

CCMSectionPage::CCMSectionPage(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CChildDialog(CCMSectionPage::IDD, pParent), CCMSectionPageBase(pDoc, FALSE, pParent)
{
	//{{AFX_DATA_INIT(CCMSectionPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CCMSectionPage::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectionPage)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_SP_SEC_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SECT_SIZE_TIP, m_editTip);

}

/////////////////////////////////////////////////////////////////////////////
// CCMSectionPage implementation functions

#define CDialog CChildDialog
BEGIN_MESSAGE_MAP(CCMSectionPage, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CCMSectionPage)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_ADD, OnAdd)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_MOD, OnModify)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_DEL, OnDelete)
	ON_BN_CLICKED(IDC_CMD_MP_BTN_IMPORT, OnImport)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SP_SEC_LIST, OnItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_SP_SEC_LIST, OnDblClickListItem)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_COPY, OnCopy)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_RENUM, OnCmdBtnRenum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectionPage message handlers

BOOL CCMSectionPage::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_PREVIEW);
	m_editTip.SetReadOnly();
	m_editTip.ShowWindow(SW_SHOW);
	return CCMSectionPageBase::OnInitDialog(pWnd);
}

void CCMSectionPage::OnAdd() 
{
	// TODO: Add your control notification handler code here
	CCMSectionPageBase::OnAdd();

}

void CCMSectionPage::OnModify() 
{
	// TODO: Add your control notification handler code here
	CCMSectionPageBase::OnModify();
}

void CCMSectionPage::OnDelete() 
{
	// TODO: Add your control notification handler code here
	CCMSectionPageBase::OnDelete();
}

void CCMSectionPage::OnImport()
{
#if defined(_MGEN_ORG) || defined(_MGEN_US)
	CFileDialog fd(TRUE, D_MODEL_FILE_EXT, NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES,
					D_MODEL_FILE_FILTER_EX, NULL);
#else
	CFileDialog fd(TRUE, D_MODEL_FILE_EXT, NULL, 
								 OFN_FILEMUSTEXIST | OFN_LONGNAMES, 
								 D_MODEL_FILE_FILTER, NULL);
#endif
	if (fd.DoModal() == IDOK)
	{
		CString csFileName = fd.GetPathName();
		CString strMgbExt = _T(".") + fd.GetFileExt();
		BOOL bIsMgb = _T(".mgb") == strMgbExt;
		CCMImportPropertyDlg dlg;
		dlg.InitPropertyData(csFileName, 1, bIsMgb);
		dlg.SetInitPos(D_INIT_POS_RT);
		dlg.DoModal();
	}	
}

void CCMSectionPage::OnCopy() 
{
	CCMSectionPageBase::OnCopy();
}

void CCMSectionPage::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	CCMSectionPageBase::OnUpdateDB(pSender, lHint, pHint);
}

void CCMSectionPage::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCMSectionPageBase::OnItemChanged(pNMHDR, pResult);
}

void CCMSectionPage::OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CCMSectionPageBase::OnDblClickListItem(pNMHDR, pResult);
}

//Add by KJH20021030
void CCMSectionPage::OnCmdBtnRenum() 
{
	CCMPropRenumDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	dlg.m_nPropNum = 1;
	dlg.DoModal();
}

void CCMSectionPage::OnActivatePage()
{
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
		m_Data.Initialize();
	else
	{
		m_Key = _tstol(m_List.GetItemText(iItem, 0));
		if (!GetSect(m_pDoc, m_bDesign, m_bVirtual, m_Key, m_Data)) return;
		ShowSizeTip(m_Data);
	}
}

void CCMSectionPage::SetShowTip(CString& csTip)
{
	m_editTip.ShowWindow(SW_SHOW);
	m_editTip.SetWindowText(csTip);
	m_csTip = csTip;
}

void CCMSectionPage::ShowSizeTip(T_SECT_D& rData)
{
	if (rData.nStype != D_SECT_TYPE_REGULAR)
	{
		SetShowTip(CString(_T("")));
		return;
	}

	int nShapeIndex = CSectUtil::GetShapeIndexFromNameReg(m_Data.SectBefore.Shape);
	if (nShapeIndex < 0) return;
	CStringArray aSizeTitle;
	CSectUtil::GetSizeTitleFromShapeIndexReg(nShapeIndex, aSizeTitle);
	int nCount = aSizeTitle.GetSize();

	int nSizeNum = 0;
	CString csSizeTip = _T("");
	T_SECT_SECTBASE_D* pSect = &rData.SectBefore.SectI;

	for (int i = 0; i < nCount; i++)
	{
		if (aSizeTitle[i] != _T(""))
		{
			CString csTemp;
			csTemp.Format(_T("%s : %g "), aSizeTitle[i], pSect->Size[nSizeNum]);
			csSizeTip += csTemp;
			nSizeNum++;
		}
	}
	SetShowTip(csSizeTip);
}

/////////////////////////////////////////////////////////////////////////////
// CCMSectPage4Design dialog

CCMSectPage4Design::CCMSectPage4Design(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSectPage4Design::IDD, pParent), CCMSectionPageBase(0, TRUE)
{
	//{{AFX_DATA_INIT(CCMSectPage4Design)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMSectPage4Design::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectPage4Design)
	DDX_Control(pDX, IDC_CMD_SP_CHK_HIDE, m_chkHideUDS);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CMD_SP_SEC_LIST, m_List);
	DDX_Control(pDX, IDC_CMD_SP_PREVIEW,  m_wndSecView);
}


BEGIN_MESSAGE_MAP(CCMSectPage4Design, CDialogMove)
	//{{AFX_MSG_MAP(CCMSectPage4Design)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_MOD, OnModify)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_RESET, OnReset)
	ON_BN_CLICKED(IDC_CMD_SP_BTN_RESET_ALL, OnResetAll)	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SP_SEC_LIST, OnItemChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_CMD_SP_SEC_LIST, OnDblClickListItem)
	ON_BN_CLICKED(IDC_CMD_SP_CHK_HIDE, OnHideUDS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectPage4Design message handlers
BOOL CCMSectPage4Design::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CDialogMove::OnInitDialog();
	
	GetDlgItem(IDC_CMD_SP_CHK_HIDE)->ShowWindow(!m_bVirtual);
	//GetDlgItem(IDC_CMD_SP_BTN_RESET)->ShowWindow(!m_bVirtual);
	//GetDlgItem(IDC_CMD_SP_BTN_RESET_ALL)->ShowWindow(!m_bVirtual);

	// TODO: Add extra initialization here
	CWnd* pWnd = GetDlgItem(IDC_CMD_SP_PREVIEW);
	m_chkHideUDS.SetCheck(m_bHideUDS);

	return CCMSectionPageBase::OnInitDialog(pWnd);
}

void CCMSectPage4Design::OnModify() 
{	
	CCMSectionPageBase::OnModify();
	// Coded by Seungjun ('20061018)
	IUsageCounter::Use(_T("SECD"));
}

void CCMSectPage4Design::OnReset() 
{
	if (m_bVirtual)
	{
		VirtualReset();
		return;
	}

	T_SECT_K Key;
	T_SECT_D Data;
	CArray<T_SECT_K, T_SECT_K> aSectK;
	CArray<T_SECT_D, T_SECT_D&> aSectD;
	aSectK.RemoveAll();
	aSectD.RemoveAll();

	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}
 
	Key = _tstol(m_List.GetItemText(iItem, 0));    
	if(!m_pDoc->m_pAttrCtrl->GetSect(Key, Data)) return;
	aSectK.Add(Key);
	aSectD.Add(Data);
	if(aSectK.GetSize() > 0) m_pDoc->m_pDataCtrl->AddSecd(aSectK, aSectD); 

	// Coded by Seungjun ('20061018)
	IUsageCounter::Use(_T("SECD"));
}

void CCMSectPage4Design::OnResetAll() 
{
	if (m_bVirtual)
	{
		VirtualResetAll();
		return;
	}

	T_SECT_K Key;
	T_SECT_D Data;    
	CArray<T_SECT_K, T_SECT_K> aSectK;
	CArray<T_SECT_D, T_SECT_D&> aSectD;
	aSectK.RemoveAll();
	aSectD.RemoveAll();
	
	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountSect();
	if(nItemCount == 0) return;
	POSITION pos;  
	
	if(nItemCount)
	{        
		pos = m_pDoc->m_pAttrCtrl->GetStartSect();
		while(pos != NULL)
		{
			m_pDoc->m_pAttrCtrl->GetNextSect(pos, Key, Data);
			aSectK.Add(Key);
			aSectD.Add(Data);
		}    
		if(aSectK.GetSize() > 0) m_pDoc->m_pDataCtrl->AddSecd(aSectK, aSectD); 
	}

	// Coded by Seungjun ('20061018)
	IUsageCounter::Use(_T("SECD"));
}

void CCMSectPage4Design::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	CCMSectionPageBase::OnUpdateDB((CWnd*)pSender, lHint, pHint);
}

void CCMSectPage4Design::OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCMSectionPageBase::OnItemChanged(pNMHDR, pResult);
}

void CCMSectPage4Design::OnDblClickListItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CCMSectionPageBase::OnDblClickListItem(pNMHDR, pResult);

	MakeItemEx(FALSE);
}

void CCMSectPage4Design::OnHideUDS()
{
	if (m_chkHideUDS.GetCheck()) m_bHideUDS = TRUE;
	else m_bHideUDS = FALSE;
	MakeItemEx(FALSE);
}

void CCMSectPage4Design::SetVirtual(BOOL bVitual)
{
	CCMSectionPageBase::SetVirtual(bVitual);
}

void CCMSectPage4Design::VirtualReset()
{
	T_SECV_K Key;
	T_SECV_D Data;
	CArray<T_SECV_K, T_SECV_K> aSectK;
	CArray<T_SECV_D, T_SECV_D&> aSectD;
	aSectK.RemoveAll();
	aSectD.RemoveAll();

	int iItem = m_List.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_selected_section__nChoose_one_));
		return;
	}

	Key = _tstol(m_List.GetItemText(iItem, 0));

	if (!m_pDoc->m_pAttrCtrl2->GetSevo(Key, Data)) return;

	aSectK.Add(Key);
	aSectD.Add(Data);
	if (aSectK.GetSize() > 0) m_pDoc->m_pDataCtrl->AddSecv(aSectK, aSectD);
}

void CCMSectPage4Design::VirtualResetAll()
{
	T_SECV_K Key;
	T_SECV_D Data;
	CArray<T_SECV_K, T_SECV_K> aSectK;
	CArray<T_SECV_D, T_SECV_D&> aSectD;
	aSectK.RemoveAll();
	aSectD.RemoveAll();

	int nItemCount = m_pDoc->m_pAttrCtrl2->GetCountSevo();
	if (nItemCount == 0) return;
	POSITION pos;

	if (nItemCount)
	{
		pos = m_pDoc->m_pAttrCtrl2->GetStartSevo();
		while (pos != NULL)
		{
			m_pDoc->m_pAttrCtrl2->GetNextSevo(pos, Key, Data);
			aSectK.Add(Key);
			aSectD.Add(Data);
		}
		if (aSectK.GetSize() > 0) m_pDoc->m_pDataCtrl->AddSecv(aSectK, aSectD);
	}
}

