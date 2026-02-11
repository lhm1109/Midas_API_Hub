#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdDampCheckInfoDlg.h"
#include "CmdIsolatorGBStressLmtTableDlg.h"
#include "ExportFunc.h"
#include "CMLoadCombDlg.h"
#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\StageInfo.h"
#include "..\wg_db\DampDgnMgr_CH.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "CmdDampLoadCombTableDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCmdDampCheckInfoDlg::CCmdDampCheckInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdDampCheckInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdDampCheckInfoDlg)
	m_nLmtType = 0;
	m_nSeisStor = 0;
	m_nUpStor = 0;
	m_nLcom = 0;
	m_bInit = FALSE;
	//}}AFX_DATA_INIT
}

void CCmdDampCheckInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdDampCheckInfoDlg)
	DDX_Control(pDX, IDC_CMD_DAMP_RX_LOAD_COMBO, m_rxCombox);
	DDX_Control(pDX, IDC_CMD_DAMP_RZ_LOAD_COMBO, m_rzCombox);
	DDX_Control(pDX, IDC_CMD_DAMP_GRAVITY_LCOM_COMBO, m_LcomCombox);
	DDX_Control(pDX, IDC_CMD_DAMP_RX_COE_EDIT, m_wndRxCoeEdit);
	DDX_Control(pDX, IDC_CMD_DAMP_RZ_COE_EDIT, m_wndRzCoeEdit);
}

BEGIN_MESSAGE_MAP(CCmdDampCheckInfoDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_DAMP_OK_BTN, OnOK)
	ON_BN_CLICKED(IDC_CMD_DAMP_CANCEL_BTN, OnCancel)
	ON_BN_CLICKED(IDC_CMD_DAMP_LOOK_LOAD_BTN, OnGeneralLcomBtn)
END_MESSAGE_MAP()

BOOL CCmdDampCheckInfoDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();
	SetLcomGCmb();
	SetRsCmb();
	Data2Dlg();

	return TRUE;
}

void CCmdDampCheckInfoDlg::Data2Dlg()
{
	T_DPCI_D DpciD; DpciD.Initialize();

	if (m_pDoc->m_pAttrCtrl->GetDpci(DpciD))
	{
		CDlgUtil::CobxSetCurSelItemData(m_LcomCombox, DpciD.GravityCase);
	}

	int nCount = m_rxCombox.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		m_rxCombox.SetCheck(i, FALSE);
		if (m_aRx[1][i] == 0)
		{
			for (int j = 0; j < DpciD.aSplcCaseX.GetSize(); j++)
			{
				if(m_aRx[0][i] == DpciD.aSplcCaseX[j])
				{
					m_rxCombox.SetCheck(i, TRUE);
					break;
				}
			}
		}
		else
		{
			for (int j = 0; j < DpciD.aThisCaseX.GetSize(); j++)
			{
				if(m_aRx[0][i] == DpciD.aThisCaseX[j])
				{
					m_rxCombox.SetCheck(i, TRUE);
					break;
				}
			}
		}
	}

	nCount = m_rzCombox.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if ((m_aRz[1][i] == 0 && m_aRz[0][i] == DpciD.SplcCaseZ) || (m_aRz[1][i] == 1 && m_aRz[0][i] == DpciD.ThisCaseZ))
		{
			m_rzCombox.SetCheck(i, TRUE);
			m_rzCombox.SetCurSel(i);
		}
		else
		{
			m_rzCombox.SetCheck(i, FALSE);
		}
	}
	if (CDampDesignMgr_CH::Instance()->GetDampIsoCategoryType() == 0 && CDampDesignMgr_CH::Instance()->GetDampIsoCodeType() == 0)
	{
		DpciD.dRxCoe = 1.0;
		DpciD.dRzCoe = 1.0;
		m_wndRxCoeEdit.EnableWindow(FALSE);
		m_wndRzCoeEdit.EnableWindow(FALSE);
	}
	CString csVal;
	csVal.Format(_T("%g"), DpciD.dRxCoe);
	m_wndRxCoeEdit.SetWindowText(csVal);
	csVal.Format(_T("%g"), DpciD.dRzCoe);
	m_wndRzCoeEdit.SetWindowText(csVal);
}

BOOL CCmdDampCheckInfoDlg::Dlg2Data()
{
	m_Data.Initialize();
	m_Data.GravityCase = CDlgUtil::CobxGetCurSelItemData(m_LcomCombox, m_LcomCombox.GetCurSel());

	int nCount = m_rxCombox.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_rxCombox.GetCheck(i))
		{
			if (m_aRx[1][i] == 0)
			{
				m_Data.aSplcCaseX.Add(m_aRx[0][i]);
			}
			else
			{
				m_Data.aThisCaseX.Add(m_aRx[0][i]);
			}
		}
	}

	nCount = m_rzCombox.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		if (m_rzCombox.GetCheck(i))
		{
			if (m_aRz[1][i] == 0)
			{
				m_Data.SplcCaseZ = m_aRz[0][i];
			}
			else
			{
				m_Data.ThisCaseZ = m_aRz[0][i];
			}
		}
	}
	m_wndRxCoeEdit.GetEditValue(m_Data.dRxCoe);
	m_wndRzCoeEdit.GetEditValue(m_Data.dRzCoe);

	return TRUE;
}

void CCmdDampCheckInfoDlg::OnGeneralLcomBtn()
{
	//CreateOrActivateDlg((CDBDoc*)CDBDoc::GetDocPoint(), CCMLoadCombDlg::IDD, NULL);
	CCmdDampLoadCombTableDlg dlg;
	Dlg2Data();
	dlg.SetParamData(m_Data);
	dlg.DoModal();
}

void CCmdDampCheckInfoDlg::OnOK()
{
	if (!Dlg2Data())	return;
	if(!CDBDoc::GetDocPoint()->m_pDataCtrl->AddDpci(m_Data)) return;
	CDialogMove::OnOK();
}

void CCmdDampCheckInfoDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCmdDampCheckInfoDlg::SetLcomGCmb()
{
	m_LcomCombox.ResetContent();
	CArray<T_LCOM_K, T_LCOM_K> aLcomKey;
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_GENERAL, aLcomKey);
	if (aLcomKey.GetCount() < 1) 	return;
	
	T_LCOM_D LcomD;
	for (int i = 0; i < aLcomKey.GetCount(); i++)
	{
		LcomD.Initialize();
		if (!m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_GENERAL, aLcomKey[i], LcomD))
		{
			ASSERT(0);
			continue;
		}
		m_LcomCombox.SetItemData(m_LcomCombox.AddString(LcomD.LoadCombName), aLcomKey[i]);
	}
	m_LcomCombox.SetCurSel(0);
}

void CCmdDampCheckInfoDlg::SetRsCmb()
{
	for (int i = 0; i < 2; i++)
	{
		m_aRx[i].RemoveAll();
		m_aRz[i].RemoveAll();
	}
	CDC* pRxDC = m_rxCombox.GetDC();
	CDC* pRzDC = m_rzCombox.GetDC();
	int maxRxTextLength = 0;
	int maxRzTextLength = 0;

	T_SPLC_K_LIST aSplcK;
	T_SPLC_D SplcD;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	m_rxCombox.ResetContent();
	m_rzCombox.ResetContent();
	for (int i = 0; i < aSplcK.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD);
		if (SplcD.nDirection == 1)
		{
			m_rxCombox.AddString(SplcD.LoadCaseName);
			m_aRx[0].Add(aSplcK[i]);
			m_aRx[1].Add(0);
			maxRxTextLength = max(maxRxTextLength, pRxDC->GetTextExtent(SplcD.LoadCaseName).cx);
		}
		else if(SplcD.nDirection == 2)
		{
			m_rzCombox.AddString(SplcD.LoadCaseName);
			m_aRz[0].Add(aSplcK[i]);
			m_aRz[1].Add(0);
			maxRzTextLength = max(maxRzTextLength, pRzDC->GetTextExtent(SplcD.LoadCaseName).cx);
		}
	}
	/** This */
	T_THGA_K ThgaK;
	T_THGA_D ThgaD;
	T_THIS_D ThisD;
	CArray<T_THGA_K, T_THGA_K> aKey;
	POSITION pos = m_pDoc->m_pAttrCtrl->GetStartThga();
	while (pos != NULL)
	{
		m_pDoc->m_pAttrCtrl->GetNextThga(pos, ThgaK, ThgaD);
		m_pDoc->m_pAttrCtrl->GetThis(ThgaK, ThisD);
		if (ThgaD.nThisFuncKey[2] == 0)
		{
			m_rxCombox.AddString(ThisD.LoadCaseName);
			m_aRx[0].Add(ThgaK);
			m_aRx[1].Add(1);
			maxRxTextLength = max(maxRxTextLength, pRxDC->GetTextExtent(ThisD.LoadCaseName).cx);
		}
		else
		{
			m_rzCombox.AddString(ThisD.LoadCaseName);
			m_aRz[0].Add(ThgaK);
			m_aRz[1].Add(1);
			maxRzTextLength = max(maxRzTextLength, pRzDC->GetTextExtent(ThisD.LoadCaseName).cx);
		}
	}
	m_rxCombox.SetCurSel(0);
	m_rzCombox.SetCurSel(0);

	//Set Dynamic Length
	m_rxCombox.SetDroppedWidth(maxRxTextLength + 20);
	m_rzCombox.SetDroppedWidth(maxRzTextLength + 20);
}

void CCmdDampCheckInfoDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	switch (lHint)
	{
	case D_UPDATE_DGN_FORCE_CHANGED:
		SetLcomGCmb();
		m_LcomCombox.SetCurSel(0);
	default:
		break;
	}
}

