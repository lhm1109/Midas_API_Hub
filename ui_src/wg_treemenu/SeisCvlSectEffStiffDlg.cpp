// SeisCvlSectEffStiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisCvlSectEffStiffDlg.h"

#include "..\wg_base\TreeMenuMode.h"
#include "..\MIT_frx\MTreeChildImpl.h"

#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\UnitCtrl.h"
#include "..\wg_db\BrdgSeisEvalResult.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_dgn\BrdgSeisEvalKISTEC2015.h"
#include "..\wg_dgn\BrdgSeisEvalKISTEC2019.h"
#include "..\wg_dgn\BrdgSeisEvalKALIS2023.h"
#include "..\wg_dgn\BrdgSeisEvalKEC2012.h"

#include "..\wg_main\wg_mainRes2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlSectEffStiffDlg dialog


CSeisCvlSectEffStiffDlg::CSeisCvlSectEffStiffDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CSeisCvlSectEffStiffDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeisCvlSectEffStiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDoc = CDBDoc::GetDocPoint();

	m_SectK = 0;
	m_bPreDgnCode = FALSE;

	m_bUserDefined = FALSE;
	m_dIeffRatMod[0] = 0.0;
	m_dIeffRatMod[1] = 0.0;

	m_nDesignCode = 0;
}

void CSeisCvlSectEffStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlSectEffStiffDlg)
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MPHI_CMB,    m_cmbMPhi);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_SECT_EDT,    m_edtSect);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_POS_EDT,     m_edtPos);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_I_LONGI_EDT, m_edtIyL);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_I_TRANS_EDT, m_edtIyT);

	DDX_Control(pDX, IDC_TM_SEIS_CVL_I_LONGI_MOD_EDT, m_edtIyLMod);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_I_TRANS_MOD_EDT, m_edtIyTMod);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MY_LONGI_EDT   , m_edtMyL);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_ME_LONGI_EDT   , m_edtMeL);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MY_TRANS_EDT   , m_edtMyT);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_ME_TRANS_EDT   , m_edtMeT);

	DDX_Control(pDX, IDC_TM_SEIS_CVL_MY_LONGI_UNT, m_untMyL);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_ME_LONGI_UNT, m_untMeL);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_MY_TRANS_UNT, m_untMyT);
	DDX_Control(pDX, IDC_TM_SEIS_CVL_ME_TRANS_UNT, m_untMeT);

	DDX_Check(pDX, IDC_TM_SEIS_CVL_UPDATE_CHECK, m_bUserDefined);

	DDX_Control(pDX, IDC_TM_SEIS_CVL_LIST, m_list);

	//}}AFX_DATA_MAP
}

void CSeisCvlSectEffStiffDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

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
		DoUnitChange();
		break;
	case D_UPDATE_SEL_ADD: 
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CSeisCvlSectEffStiffDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if(nCount==0) return; 
	ASSERT(nCount==1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch(nCmd)
		{
		case(UR_MPCC_ADD):
		case(UR_MPCC_DEL): 
			InitCombo();
			MakeItemEx();
			break;
		case(UR_BSED_ADD):
		case(UR_BSED_DEL):
			AlignCtrl();
			break;

		default:
			break;
		}
	}

	RedrawWindow();
}

void CSeisCvlSectEffStiffDlg::DoUnitChange()
{
	Data2Dlg();
}

void CSeisCvlSectEffStiffDlg::InitUnit()
{
	m_edtIyL   .SetUnitType(D_UNITSYS_NONE);
	m_edtIyT   .SetUnitType(D_UNITSYS_NONE);
	m_edtIyLMod.SetUnitType(D_UNITSYS_NONE);
	m_edtIyTMod.SetUnitType(D_UNITSYS_NONE);

	m_edtMyL.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtMyT.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtMeL.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_edtMeT.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_untMyL.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_untMyT.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_untMeL.SetUnitType(D_UNITSYS_BASE_MOMENT);
	m_untMeT.SetUnitType(D_UNITSYS_BASE_MOMENT);
}

void CSeisCvlSectEffStiffDlg::InitCtrl()
{
	BOOL bBrdgSeisEvalResult = FALSE;
	CBrdgSeisEvalResult* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();
	if (pResult)
	{
		bBrdgSeisEvalResult = pResult->ExistResult();
	}

	BOOL bEnable = bBrdgSeisEvalResult; // (m_SyefD.nDgnType != 0);
	GetDlgItem(IDC_TM_SEIS_CVL_MY_LONGI_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_ME_LONGI_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_MY_TRANS_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_ME_TRANS_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_MY_LONGI_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_ME_LONGI_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_MY_TRANS_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_ME_TRANS_EDT)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_MY_LONGI_UNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_ME_LONGI_UNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_MY_TRANS_UNT)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_ME_TRANS_UNT)->EnableWindow(bEnable);

	GetDlgItem(IDC_TM_SEIS_CVL_UPDATE_BTN     )->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_UPDATE_CHECK   )->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_I_LONGI_MOD_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_IYY_MOD_STC    )->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_I_TRANS_MOD_STC)->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_IZZ_MOD_STC    )->EnableWindow(bEnable);
	GetDlgItem(IDC_TM_SEIS_CVL_I_LONGI_MOD_EDT)->EnableWindow(bEnable && m_bUserDefined);
	GetDlgItem(IDC_TM_SEIS_CVL_I_TRANS_MOD_EDT)->EnableWindow(bEnable && m_bUserDefined);

 
 	GetDlgItem(IDC_TM_SEIS_CVL_ADD      )->EnableWindow(bEnable);
 	GetDlgItem(IDC_TM_SEIS_CVL_MODIFY   )->EnableWindow(bEnable);

	int nSyefType = GetSyefType();
// 	if (m_nDesignCode == KALIS_2023 && nSyefType == 2)
// 		GetDlgItem(IDC_TM_SEIS_CVL_APPLY_BTN)->EnableWindow(bEnable);
// 	else
// 		GetDlgItem(IDC_TM_SEIS_CVL_APPLY_BTN)->EnableWindow(TRUE);

	MTreeChildImpl* pParent = dynamic_cast<MTreeChildImpl*>(GetParent()->GetParent());
	if (m_nDesignCode == KALIS_2023 && nSyefType == 2)
		pParent->GetExecuteBtn()->EnableWindow(bEnable);
	else
		pParent->GetExecuteBtn()->EnableWindow(TRUE);	
}

void CSeisCvlSectEffStiffDlg::InitCombo()
{
	m_cmbMPhi.ResetContent();

	CArray<T_MPCC_K,T_MPCC_K> aMpccK;
	m_pDoc->m_pAttrCtrl2->GetMpccKeyList(aMpccK);

	T_MPCC_D MpccD; MpccD.Initialize();

	for(int i=0; i<aMpccK.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(aMpccK[i], MpccD)) continue;
		m_cmbMPhi.SetItemData(m_cmbMPhi.AddString(MpccD.strName), aMpccK[i]);
	}

	m_cmbMPhi.SetCurSel(0);
}

void CSeisCvlSectEffStiffDlg::AlignCtrl()
{
	CArray<UINT, UINT> aControl0; aControl0.RemoveAll();
	aControl0.Add(IDC_TM_SEIS_CVL_APPLY_BTN);
	aControl0.Add(IDC_TM_SEIS_CVL_CLOSE_BTN);

	CArray<UINT, UINT> aControl1; aControl1.RemoveAll();
	CArray<UINT, UINT> aControl2; aControl2.RemoveAll();

	CDlgUtil::GetCtrlIDByIncRect(this, aControl1, IDC_TM_SEIS_CVL_EFF_STIFF_MOD_GRP, TRUE);
	CDlgUtil::GetCtrlIDByIncRect(this, aControl2, IDC_TM_TPSC_GROUP_OPERATION      , TRUE);
	aControl2.Add(IDC_TM_SEIS_CVL_LIST);

	CRect rRef;
	CRect rToMove;
	int nDistY;

	int nSyefType = GetSyefType();

	BOOL bBrdgSeisEvalResult = FALSE;
	CBrdgSeisEvalResult* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();
	if (pResult)
	{
		bBrdgSeisEvalResult = pResult->ExistResult();
	}

	if (m_nDesignCode == KALIS_2023 && (bBrdgSeisEvalResult || nSyefType == 2))
	{
		CtrlShowHide(aControl1, SW_SHOW);
		CtrlShowHide(aControl2, SW_SHOW	);

		// Yield Effective Stiffness Group
		CRect rectResize;
		GetDlgItem(IDC_TM_SEIS_CVL_PROP_GRP)->GetWindowRect(rectResize);
		GetDlgItem(IDC_TM_SEIS_CVL_EFF_STIFF_MOD_GRP)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + 8;
		ScreenToClient(rectResize);
		GetDlgItem(IDC_TM_SEIS_CVL_PROP_GRP)->MoveWindow(rectResize);

		GetDlgItem(IDC_TM_SEIS_CVL_EFF_STIFF_MOD_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_TPSC_GROUP_OPERATION)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistY(this, aControl2, nDistY);

		GetDlgItem(IDC_TM_SEIS_CVL_LIST)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_SEIS_CVL_APPLY_BTN)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 10;
		CDlgUtil::CtrlMoveDistY(this, aControl0, nDistY);
 	}
	else
	{
		CtrlShowHide(aControl1, SW_HIDE);
		CtrlShowHide(aControl2, SW_HIDE);

		// Yield Effective Stiffness Group
		CRect rectResize;
		GetDlgItem(IDC_TM_SEIS_CVL_PROP_GRP)->GetWindowRect(rectResize);
		GetDlgItem(IDC_TM_SEIS_CVL_EFF_STIFF_GRP)->GetWindowRect(rRef);
		rectResize.bottom = rRef.bottom + 8;
		ScreenToClient(rectResize);
		GetDlgItem(IDC_TM_SEIS_CVL_PROP_GRP)->MoveWindow(rectResize);

		GetDlgItem(IDC_TM_SEIS_CVL_EFF_STIFF_GRP)->GetWindowRect(rRef);
		GetDlgItem(IDC_TM_SEIS_CVL_APPLY_BTN)->GetWindowRect(rToMove);
		nDistY = rRef.bottom - rToMove.top + 15;
		CDlgUtil::CtrlMoveDistY(this, aControl0, nDistY);

	}

}

void CSeisCvlSectEffStiffDlg::SetHeaderTitle()
{
	CString aTitle[] = { _T("Sect. Name"), _T("Longi."), _T("Trans."), _T("Key")}; //Key는 안보임.
	int nColWidth[] = { 80, 59 ,59, 0};

	CString title;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_list.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_list.GetSafeHwnd(), dwStyle);

	for (int i = 0; i < 4; i++)
	{
		CString strTitle = aTitle[i];
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		if (i == 0) lvcolumn.fmt = LVCFMT_LEFT;
		else     lvcolumn.fmt = LVCFMT_RIGHT;

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = strTitle.GetBuffer(0);
		m_list.InsertColumn(i + 1, &lvcolumn);
		strTitle.ReleaseBuffer();
	}
}

void CSeisCvlSectEffStiffDlg::MakeItemEx()
{
	m_list.DeleteAllItems();

	T_SYEF_D SyefD;
	T_MPCC_D MpccD;

	int nIndex = 0;
	CArray<T_SYEF_K, T_SYEF_K> rKeyList;
	m_pDoc->m_pAttrCtrl2->GetSyefKeyList(rKeyList);
	for (int i = 0; i < rKeyList.GetSize(); i++)
	{
		T_SYEF_K SyefK = rKeyList[i];
		T_MPCC_K MpccK = SyefK;

		if(!m_pDoc->m_pAttrCtrl2->GetSyef(SyefK, SyefD)) continue;
		if(SyefD.nDgnType != 2) continue;
		if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) continue;

		T_SECT_D SectD; SectD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetSect(MpccD.SectK, SectD)) { ASSERT(0); return; }

		T_EFFStif_D Data;
		Data.strName = SectD.SName;
		Data.MpccK = MpccK;
		Data.MpccKPre = MpccK;
		Data.dEffL = SyefD.dI_EffL;
		Data.dEffT = SyefD.dI_EffT;;

		InsertItem(nIndex, Data);
		nIndex++;
	}
}

BOOL CSeisCvlSectEffStiffDlg::InsertItem(int nIndex, T_EFFStif_D& Data)
{
	LVITEM lvitem;
	CString str;
	int nItem;

	lvitem.iItem = nIndex;
	for (int i = 0; i < 4; i++)
	{
		lvitem.iSubItem = i;
		str = DataToStr(i, Data);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		if (i == 0)
		{
			nItem = m_list.InsertItem(&lvitem);
		}
		else m_list.SetItem(&lvitem);
		str.ReleaseBuffer();
	}

	return TRUE;
}

BOOL CSeisCvlSectEffStiffDlg::ModifyItem(int nIndex, T_EFFStif_D& Data)
{
	int nItem = nIndex;
	if (nItem != -1)
	{
		CString str;
		for (int i = 0; i < 4; i++)
		{
			str = DataToStr(i, Data);
			m_list.SetItemText(nItem, i, str);
		}
	}
	return TRUE;
}

CString CSeisCvlSectEffStiffDlg::DataToStr(int i, T_EFFStif_D& Data)
{
	CString str = _T("");

	switch (i)
	{
	case 0:
		str = Data.strName;
		break;
	case 1:
		str.Format(_T("%g"), Data.dEffL);
		break;
	case 2:
		str.Format(_T("%g"), Data.dEffT);
		break;
	case 3:
		str.Format(_T("%d"), Data.MpccK);
		break;
	default:
		ASSERT(0); break;
	}

	return str;
}


void CSeisCvlSectEffStiffDlg::Data2Dlg()
{
	if(m_pDoc->m_pAttrCtrl2->GetCountMpcc() == 0) return;

	T_MPCC_K MpccK;
	T_MPCC_D MpccD; MpccD.Initialize();
	MpccK = m_cmbMPhi.GetItemData(m_cmbMPhi.GetCurSel());
	if(!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) { ASSERT(0); return; }

	m_SyefD.Initialize();
	m_SyefK = MpccK;
	if(!m_pDoc->m_pAttrCtrl2->GetSyef(MpccK, m_SyefD)) { ASSERT(0); m_SyefD.Initialize();}

	CString strSect;
	CString strPos;

	// 무조건 정의된 것을 보여주자
	//if(MpccD.EvgpK == 0) // None
	{
		T_SECT_D SectD; SectD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetSect(MpccD.SectK, SectD)) { ASSERT(0); return; }

		m_SectK = MpccD.SectK;

		strSect = SectD.SName;

		if     (MpccD.nPosition == 0) strPos = _LS(IDS_WG_TREEMENU_SECT_EFF_STIFF_I);
		else if(MpccD.nPosition == 1) strPos = _LS(IDS_WG_TREEMENU_SECT_EFF_STIFF_M);
		else                          strPos = _LS(IDS_WG_TREEMENU_SECT_EFF_STIFF_J);
	}

	m_edtSect.SetWindowText(strSect);
	m_edtPos.SetWindowText(strPos);

	double dIeffRat[2];
	dIeffRat[0] = 0.0;
	dIeffRat[1] = 0.0;

	T_BRDGEVAL_RESULT Result[2]; //Longi. Trans.

	CBrdgSeisEval* pSeisEval;
	if      (m_nDesignCode == KISTEC_2015) pSeisEval = new CBrdgSeisEvalKISTEC2015;
	else if (m_nDesignCode == KISTEC_2019) pSeisEval = new CBrdgSeisEvalKISTEC2019;
	else if (m_nDesignCode == KALIS_2023)  pSeisEval = new CBrdgSeisEvalKALIS2023;
	else if (m_nDesignCode == KEC_2012)    pSeisEval = new CBrdgSeisEvalKEC2012;
	else                                   pSeisEval = new CBrdgSeisEvalKISTEC2015;

	if(pSeisEval != nullptr)
	{
		BOOL bRecal = TRUE;
		if (m_nDesignCode == KALIS_2023)
		{
			BOOL bBrdgSeisEvalResult = FALSE;
			CBrdgSeisEvalResult* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();
			if (pResult)
			{
				bBrdgSeisEvalResult = pResult->ExistResult();
			}
			if (bBrdgSeisEvalResult || m_SyefD.nDgnType==2) bRecal = FALSE;
			
		}

		pSeisEval->CalcIeff(MpccD, dIeffRat, Result, bRecal);
		delete pSeisEval;
		pSeisEval = nullptr;
	}

	double dtMyL = (m_SyefD.nDgnType==2)? m_SyefD.dMyL : Result[0].MPhiD.StateD.dMoment[4];
	double dtMeL = (m_SyefD.nDgnType==2)? m_SyefD.dMeL : Result[0].EvalD.EvalD.dMe_pd;
	double dtMyT = (m_SyefD.nDgnType==2)? m_SyefD.dMyT : Result[1].MPhiD.StateD.dMoment[4];
	double dtMeT = (m_SyefD.nDgnType==2)? m_SyefD.dMeT : Result[1].EvalD.EvalD.dMe_pd;

	m_edtIyL.SetEditUnit(dIeffRat[0]); 
	m_edtIyT.SetEditUnit(dIeffRat[1]);
	m_edtMyL.SetEditUnit(dtMyL);
	m_edtMeL.SetEditUnit(dtMeL);
	m_edtMyT.SetEditUnit(dtMyT);
	m_edtMeT.SetEditUnit(dtMeT);

	m_dIeffRatMod[0] = 0.0;
	m_dIeffRatMod[1] = 0.0;
	if      (fabs(dtMeL) >=    fabs(dtMyL)) m_dIeffRatMod[0] = dIeffRat[0];
	else if (fabs(dtMeL) >=0.5*fabs(dtMyL)) m_dIeffRatMod[0] = (1 + dIeffRat[0])/2.0;
	else                                    m_dIeffRatMod[0] = 1.0; // = EI

	if      (fabs(dtMeT) >=    fabs(dtMyT)) m_dIeffRatMod[1] = dIeffRat[1];
	else if (fabs(dtMeT) >=0.5*fabs(dtMyT)) m_dIeffRatMod[1] = (1 + dIeffRat[1])/2.0;
	else                                    m_dIeffRatMod[1] = 1.0; // = EI

	if (m_SyefD.nDgnType == 2)
	{
		m_edtIyLMod.SetEditUnit(m_SyefD.dI_EffL);
		m_edtIyTMod.SetEditUnit(m_SyefD.dI_EffT);
	}
	else
	{
		m_edtIyLMod.SetWindowText(_T("-"));
		m_edtIyTMod.SetWindowText(_T("-"));
	}
	m_bUserDefined = m_SyefD.bUserDefined;

	UpdateData(FALSE);

	InitCtrl();
}

void CSeisCvlSectEffStiffDlg::Dlg2SyefData(BOOL bDgnType1/*=FALSE*/)
{
	T_SECF_D SecfD; SecfD.Initialize();
	if (bDgnType1)
	{
		SecfD.dScaleFactor[4] = m_edtIyL.GetEditValue();
		SecfD.dScaleFactor[5] = m_edtIyT.GetEditValue();

		m_SyefD.nDgnType = 1;
		m_SyefD.bUserDefined = FALSE;
		m_SyefD.dI_EffL = 0.0;
		m_SyefD.dI_EffT = 0.0;
		m_SyefD.dMyL = 0.0;
		m_SyefD.dMeL = 0.0;
		m_SyefD.dMyT = 0.0;
		m_SyefD.dMeT = 0.0;
	}
	else
	{
		if (m_SyefD.nDgnType == 2)
		{
			SecfD.dScaleFactor[4] = m_edtIyLMod.GetEditValue();
			SecfD.dScaleFactor[5] = m_edtIyTMod.GetEditValue();
		}
		else
		{
			SecfD.dScaleFactor[4] = m_edtIyL.GetEditValue();
			SecfD.dScaleFactor[5] = m_edtIyT.GetEditValue();
		}

		m_SyefD.nDgnType = 2;
		m_SyefD.bUserDefined = m_bUserDefined;
		m_SyefD.dI_EffL = SecfD.dScaleFactor[4];
		m_SyefD.dI_EffT = SecfD.dScaleFactor[5];
		m_SyefD.dMyL = m_edtMyL.GetEditValue();
		m_SyefD.dMeL = m_edtMeL.GetEditValue();
		m_SyefD.dMyT = m_edtMyT.GetEditValue();
		m_SyefD.dMeT = m_edtMeT.GetEditValue();
	}

	if (!m_pDoc->m_pDataCtrl->ModifySyef(m_SyefK, m_SyefK, m_SyefD)) { ASSERT(0); }
}

int CSeisCvlSectEffStiffDlg::GetSyefType()
{
	T_SYEF_D SyefD;
	int nIndex = 0;
	CArray<T_SYEF_K, T_SYEF_K> rKeyList;
	m_pDoc->m_pAttrCtrl2->GetSyefKeyList(rKeyList);
	for (int i = 0; i < rKeyList.GetSize(); i++)
	{
		T_SYEF_K SyefK = rKeyList[i];

		if (!m_pDoc->m_pAttrCtrl2->GetSyef(SyefK, SyefD)) continue;
		if (nIndex < SyefD.nDgnType)
			nIndex = SyefD.nDgnType;
	}

	return nIndex;
}


BEGIN_MESSAGE_MAP(CSeisCvlSectEffStiffDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CSeisCvlSectEffStiffDlg)
	ON_CBN_SELCHANGE(IDC_TM_SEIS_CVL_MPHI_CMB, OnTmSectEffStiffMPhiCmb)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MPHI_BTN,    OnTmSectEffStiffMPhiBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_APPLY_BTN,   OnTmSectEffStiffApplyBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_CLOSE_BTN,   OnTmSectEffStiffCloseBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_UPDATE_BTN,  OnTmSectEffStiffUpdateBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_UPDATE_CHECK, OnTmSectEffStiffUserDefinedChk)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_SEIS_CVL_LIST, OnTmEvalGrupColumnItemChanged)

	ON_BN_CLICKED(IDC_TM_SEIS_CVL_ADD,    OnTmEvalGrupColumnAddBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_MODIFY, OnTmEvalGrupColumnModBtn)
	ON_BN_CLICKED(IDC_TM_SEIS_CVL_DELETE, OnTmEvalGrupColumnDelBtn)



	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeisCvlSectEffStiffDlg message handlers

BOOL CSeisCvlSectEffStiffDlg::OnInitDialog() 
{
 	CMenuBarChildDlg::OnInitDialog();

	T_BSED_D BsedD; BsedD.Initialize();
	m_pDoc->m_pAttrCtrl2->GetDgnBsed(BsedD);
	m_nDesignCode = BsedD.nDesignCode;

	InitUnit();
	InitCombo();
	AlignCtrl();
	SetHeaderTitle();
	MakeItemEx();

	Data2Dlg();

	OnTmSectEffStiffUserDefinedChk();

	GetDlgItem(IDC_TM_SEIS_CVL_APPLY_BTN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_TM_SEIS_CVL_CLOSE_BTN)->ShowWindow(SW_HIDE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisCvlSectEffStiffDlg::OnTmSectEffStiffMPhiCmb()
{
	UpdateData(TRUE);

	Data2Dlg();
}

void CSeisCvlSectEffStiffDlg::OnTmSectEffStiffMPhiBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEIS_CVL_MPHI_CURVE,0));
}

void CSeisCvlSectEffStiffDlg::OnTmSectEffStiffUpdateBtn()
{
	m_bUserDefined = FALSE; // 우선은 계산된 Eff 적용.
	m_SyefD.nDgnType = 2;

	m_edtIyLMod.SetEditUnit(m_dIeffRatMod[0]);
	m_edtIyTMod.SetEditUnit(m_dIeffRatMod[1]);

	UpdateData(FALSE);
}

void CSeisCvlSectEffStiffDlg::OnTmSectEffStiffUserDefinedChk()
{
	UpdateData(TRUE);

	m_edtIyLMod.EnableWindow(m_bUserDefined);
	m_edtIyTMod.EnableWindow(m_bUserDefined);

	if (m_bUserDefined)
	{
		if (m_SyefD.nDgnType != 2)
		{
			double dIyL = m_edtIyL.GetEditValue();
			double dIyT = m_edtIyT.GetEditValue();

			m_edtIyLMod.SetEditUnit(dIyL);
			m_edtIyTMod.SetEditUnit(dIyT);
		}
	}
}

void CSeisCvlSectEffStiffDlg::OnTmEvalGrupColumnItemChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	T_EVGP_COLM_D EvgpColm;
	int nItem = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1)
	{
		*pResult = 0;
		return;
	}
	else
	{
		if (pNMListView->uNewState == 0 || pNMListView->uNewState == 1)
		{
			*pResult = 0;
			return;
		}

		nItem = pNMListView->iItem;
	}

	T_MPCC_K MpccK = _ttoi(m_list.GetItemText(nItem, 3));
	int i = 0;
	for (i = 0; i < m_cmbMPhi.GetCount(); i++)
	{
		if (MpccK != m_cmbMPhi.GetItemData(i)) continue;
		break;
	}
	m_cmbMPhi.SetCurSel(i);

	UpdateData(FALSE);

	*pResult = 0;

	Data2Dlg();
}

void CSeisCvlSectEffStiffDlg::OnTmEvalGrupColumnAddBtn()
{
	UpdateData(TRUE);

	Dlg2SyefData();

	T_MPCC_K MpccK = m_SyefK;
 	T_MPCC_D MpccD; MpccD.Initialize();
 	if (!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) { ASSERT(0); return; }

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSect(MpccD.SectK, SectD)) { ASSERT(0); return; }

	int nCount = m_list.GetItemCount();
	T_EFFStif_D Data;
	Data.strName = SectD.SName;
	Data.MpccK = MpccK;
	Data.MpccKPre = 0;
	Data.dEffL = m_SyefD.dI_EffL;
	Data.dEffT = m_SyefD.dI_EffT;;

	T_MPCC_D MpccLst; MpccLst.Initialize();

	BOOL bModify = FALSE;
	CString strListName;
	int iItem = 0;
	for (int i = 0; i < nCount; i++)
	{
		T_MPCC_K MpccKLst = _ttoi(m_list.GetItemText(i, 3));

		if (!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccKLst, MpccLst)) { ASSERT(0); return; }

		if (MpccD.SectK == MpccLst.SectK)
		{
			Data.MpccKPre = MpccKLst;
			bModify = TRUE;
			iItem = i;
			break;
		}
	}

	if (!bModify)
		InsertItem(nCount, Data);
	else
		ModifyItem(iItem, Data);

	
	if (Data.MpccKPre > 0 && Data.MpccK != Data.MpccKPre)
	{
		T_SYEF_K SyefKPre = Data.MpccKPre;
		T_SYEF_D SyefDPre;
		if (!m_pDoc->m_pAttrCtrl2->GetSyef(SyefKPre, SyefDPre)) { ASSERT(0);}

		SyefDPre.nDgnType = 1;
		if (!m_pDoc->m_pDataCtrl->ModifySyef(SyefKPre, SyefKPre, SyefDPre)) { ASSERT(0); }
	}

}

void CSeisCvlSectEffStiffDlg::OnTmEvalGrupColumnModBtn()
{
	UpdateData(TRUE);

	int iItem = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_ERROR_NO_SELECTED_SEIS_EVAL_COLM));
		return;
	}

	Dlg2SyefData();

	T_MPCC_K MpccK = m_SyefK;
	T_MPCC_D MpccD; MpccD.Initialize();
	if (!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) { ASSERT(0); return; }

	T_SECT_D SectD; SectD.Initialize();
	if (!m_pDoc->m_pAttrCtrl->GetSect(MpccD.SectK, SectD)) { ASSERT(0); return; }

	T_EFFStif_D Data;
	Data.strName = SectD.SName;
	Data.MpccK = MpccK;
	Data.MpccKPre = MpccK;
	Data.dEffL = m_SyefD.dI_EffL;
	Data.dEffT = m_SyefD.dI_EffT;

	ModifyItem(iItem, Data);
}

void CSeisCvlSectEffStiffDlg::OnTmEvalGrupColumnDelBtn()
{
	int iItem = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_ERROR_NO_SELECTED_SEIS_EVAL_COLM));
		return;
	}

	Dlg2SyefData(TRUE);

	m_list.DeleteItem(iItem);

	int nCount = m_list.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0)
		m_list.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CSeisCvlSectEffStiffDlg::Execute()
{
	OnTmSectEffStiffApplyBtn();
}

void CSeisCvlSectEffStiffDlg::OnTmSectEffStiffApplyBtn()
{

	T_SECF_D SecfD; 

	int nSyefType = GetSyefType();

	BOOL bBrdgSeisEvalResult = FALSE;
	CBrdgSeisEvalResult* pResult = m_pDoc->m_pPostCtrl->GetBrdgSeisEvalResult();
	if (pResult)
	{
		bBrdgSeisEvalResult = pResult->ExistResult();
	}

	if (m_nDesignCode == KALIS_2023 && (bBrdgSeisEvalResult || nSyefType == 2))
	{
		T_MPCC_D MpccD; MpccD.Initialize();

		int nCount = m_list.GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			T_MPCC_K MpccK = _ttoi(m_list.GetItemText(i, 3));
			if (!m_pDoc->m_pAttrCtrl2->GetMpcc(MpccK, MpccD)) { ASSERT(0); return; }

			T_SYEF_K SyefK = MpccK;
			T_SYEF_D SyefD;
			if (!m_pDoc->m_pAttrCtrl2->GetSyef(SyefK, SyefD)) { ASSERT(0); return; }

			SecfD.Initialize();
			SecfD.dScaleFactor[4] = SyefD.dI_EffL;
			SecfD.dScaleFactor[5] = SyefD.dI_EffT;

			if (!m_pDoc->m_pDataCtrl->AddSecf(MpccD.SectK, SecfD)) { ASSERT(0); }
		}
	}
	else
	{
		Dlg2SyefData(TRUE);

		SecfD.Initialize();
		SecfD.dScaleFactor[4] = m_edtIyL.GetEditValue();
		SecfD.dScaleFactor[5] = m_edtIyT.GetEditValue();

		if (!m_pDoc->m_pDataCtrl->AddSecf(m_SectK, SecfD)) { ASSERT(0); }
	}

}

void CSeisCvlSectEffStiffDlg::OnTmSectEffStiffCloseBtn()
{
	CTreeMenuBarBase::SetModeST(__TMMODE_TREEMENU__);
}