#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSplcSubCalcEccnDlg.h"

#include "../wg_base/DlgUtil.h"

#include "../wg_db/wg_db_DBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCMSplcSubCalcEccnDlg::CCMSplcSubCalcEccnDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSplcSubCalcEccnDlg::IDD, pParent)
{
	m_dPercent4PlanDimEccn = 5.0;
	m_dMultiple4TopStoryEccn = 2.0;
	m_dRatio4AnalEccn = 1.0;
	m_bConsiderEccnbyAnal = false;

	m_nDlgType = EN_STATIC;
	m_nDgnCode = EN_DGN_CODE_NONE;

	m_pDoc = NULL;
	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_arNotFixedEccn.RemoveAll();
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_STC3);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_STC4);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_MULTIPLE_EDT);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_STC5);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_PIC);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_CONSIDER_CHK);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_CONSIDER_GRP);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_STC10);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_RATIO_EDT);
	m_arNotFixedEccn.Add(IDC_WG_CMD_CALC_ECCN_STC11);

	m_arConsiderEccn4Anal.RemoveAll();
	m_arConsiderEccn4Anal.Add(IDC_WG_CMD_CALC_ECCN_STC10);
	m_arConsiderEccn4Anal.Add(IDC_WG_CMD_CALC_ECCN_RATIO_EDT);
	m_arConsiderEccn4Anal.Add(IDC_WG_CMD_CALC_ECCN_STC11);
}

CCMSplcSubCalcEccnDlg::~CCMSplcSubCalcEccnDlg()
{
	m_Bitmap.DeleteObject();
}

void CCMSplcSubCalcEccnDlg::GetInputData(OUT double& dPercent4PlanDimEccn, OUT double& dMultiple4TopStoryEccn, OUT bool& bConsiderEccnbyAnal, OUT double& dRatio4AnalEccn)
{
	dPercent4PlanDimEccn = m_dPercent4PlanDimEccn;
	dMultiple4TopStoryEccn = m_dMultiple4TopStoryEccn;
	bConsiderEccnbyAnal = m_bConsiderEccnbyAnal;
	dRatio4AnalEccn = m_dRatio4AnalEccn;
}

void CCMSplcSubCalcEccnDlg::SetDlgType(UINT nType)
{
	m_nDlgType = nType;
}

UINT CCMSplcSubCalcEccnDlg::GetDgnCodeSellected()
{
	return m_nDgnCode;
}

void CCMSplcSubCalcEccnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WG_CMD_CALC_ECCN_PERCENT_EDT, m_edtPercent);
	DDX_Control(pDX, IDC_WG_CMD_CALC_ECCN_MULTIPLE_EDT, m_edtMultiple);
	DDX_Control(pDX, IDC_WG_CMD_CALC_ECCN_RATIO_EDT, m_edtRatio);
	DDX_Control(pDX, IDC_WG_CMD_CALC_ECCN_CONSIDER_CHK, m_chkConsiderEccnbyAnal);
	DDX_Control(pDX, IDC_WG_CMD_CALC_ECCN_CODE_CMB, m_cmbDesignCode);
	DDX_Control(pDX, IDC_WG_CMD_CALC_ECCN_PIC, m_wndBmp);

}

BEGIN_MESSAGE_MAP(CCMSplcSubCalcEccnDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSplcSubCalcEccnDlg)
	ON_BN_CLICKED(IDC_WG_CMD_CALC_ECCN_CONSIDER_CHK, OnChkConsiderEccnbyAnal)
	ON_CBN_SELCHANGE(IDC_WG_CMD_CALC_ECCN_CODE_CMB, AdjustDlg4DgnCode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMSplcSubCalcEccnDlg::Data2Dlg()
{
	CString strVal;
	strVal.Format(_T("%.2f"), m_dPercent4PlanDimEccn);
	m_edtPercent.SetWindowText(strVal);

	strVal.Format(_T("%.2f"), m_dMultiple4TopStoryEccn);
	m_edtMultiple.SetWindowText(strVal);

	strVal.Format(_T("%.2f"), m_dRatio4AnalEccn);
	m_edtRatio.SetWindowText(strVal);

	CheckCtrlEnable();
	m_chkConsiderEccnbyAnal.SetCheck(m_bConsiderEccnbyAnal ? BST_CHECKED : BST_UNCHECKED);
}

BOOL CCMSplcSubCalcEccnDlg::Dlg2Data()
{
	CString strVal;

	m_edtPercent.GetWindowText(strVal);
	const auto dEccentricity = _ttof(strVal);
	if (dgn::LT0(dEccentricity))
	{
		AfxMessageBox(_LS(IDS_CMD_DBAR_SPLC_ECCN_ACCIDENTAL_ERROR)); return FALSE;
	}
	m_dPercent4PlanDimEccn = dEccentricity;

	m_edtMultiple.GetWindowText(strVal);
	const auto dMultiple = _ttof(strVal);
	if (dgn::LT0(dMultiple))
	{
		AfxMessageBox(_LS(IDS_CMD_DBAR_SPLC_ECCN_FACTOR_ERROR)); return FALSE;
	}
	m_dMultiple4TopStoryEccn = dMultiple;

	m_edtRatio.GetWindowText(strVal);
	const auto dRatio = _ttof(strVal);
	if (dgn::LT0(dRatio))
	{
		AfxMessageBox(_LS(IDS_CMD_DBAR_SPLC_ECCN_FACTOR_ERROR)); return FALSE;
	}
	m_dRatio4AnalEccn = dRatio;

	if (m_chkConsiderEccnbyAnal.GetCheck())
		m_bConsiderEccnbyAnal = true;
	else
		m_bConsiderEccnbyAnal = false;

	GetDgnCodeEnum(m_nDgnCode);

	return TRUE;
}

BOOL CCMSplcSubCalcEccnDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	this->GetWindowRect(&m_InitDlgRect);
	GetDlgItem(IDC_WG_CMD_CALC_ECCN_TYPE_GRP)->GetWindowRect(m_InitGroupRect);
	GetDlgItem(IDOK)->GetWindowRect(&m_InitOkRect);
	GetDlgItem(IDCANCEL)->GetWindowRect(&m_InitCancelRect);
	
	ScreenToClient(&m_InitDlgRect);
	ScreenToClient(&m_InitGroupRect);
	ScreenToClient(&m_InitOkRect);
	ScreenToClient(&m_InitCancelRect);

	InitCombo();
	SetBitmap();

	const auto bPostMode = m_pDoc->IsPostMode();
	if (!bPostMode)
	{
		m_chkConsiderEccnbyAnal.EnableWindow(FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_arConsiderEccn4Anal, FALSE);
	}

	AdjustDlg4DgnCode();

	Data2Dlg();

	UpdateData(FALSE);

	return TRUE;
}

void CCMSplcSubCalcEccnDlg::OnCancel()
{
	CDialogMove::OnCancel();
}

void CCMSplcSubCalcEccnDlg::OnOK()
{
	if (!Dlg2Data()) return;

	CDialogMove::OnOK();
}

void CCMSplcSubCalcEccnDlg::CheckCtrlEnable()
{
	if (m_bConsiderEccnbyAnal)
	{
		CDlgUtil::CtrlEnableDisable(this, m_arConsiderEccn4Anal, TRUE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_arConsiderEccn4Anal, FALSE);
	}
}

void CCMSplcSubCalcEccnDlg::InitCombo()
{
	m_cmbDesignCode.ResetContent();
	if ( m_nDlgType == EN_STATIC ) m_cmbDesignCode.AddString(_T("Fixed Eccentricity"));
	m_cmbDesignCode.AddString(_T("NTC-2023 (Mexico)"));
	m_cmbDesignCode.SetCurSel(0);
}

void CCMSplcSubCalcEccnDlg::SetBitmap()
{
	m_Bitmap.DeleteObject();

	auto bLoad = m_Bitmap.LoadBitmap(IDB_DBAR_SPLC_SUB_CALC_ECCN_FORMULA);
	if (!bLoad) return;

	//CRect Rect;
	//GetDlgItem(IDC_WG_CMD_CALC_ECCN_PIC)->GetWindowRect(&Rect);
	//ScreenToClient(&Rect);

	//CDC* pDC = GetDC();
	//int nDpiX = GetDeviceCaps(pDC->m_hDC, LOGPIXELSX);
	//int nDpiY = GetDeviceCaps(pDC->m_hDC, LOGPIXELSY);

	//double dScaleX = nDpiX / 96.0;
	//double dScaleY = nDpiY / 96.0;

	//BITMAP BitmapInfo;
	//m_Bitmap.GetBitmap(&BitmapInfo);

	//int nOrgWidth = BitmapInfo.bmWidth;
	//int nOrgHeight = BitmapInfo.bmHeight;

	//int nCtrlWidth = Rect.Width();
	//int nCtrlHeight = Rect.Height();

	//double dPictureRatio = dgn::SafeDiv(static_cast<double>(nOrgWidth), static_cast<double>(nOrgHeight));

	//int nNewWidth = nCtrlWidth;
	//int nNewHeight = static_cast<int>(dgn::SafeDiv(static_cast<double>(nCtrlWidth), dPictureRatio));

	//if (nNewHeight > nCtrlHeight)
	//{
	//	nNewHeight = nCtrlHeight;
	//	nNewWidth = static_cast<int>(dPictureRatio * nCtrlHeight);
	//}

	//CBitmap ScaledBitmap;
	//ScaledBitmap.CreateCompatibleBitmap(pDC, nNewWidth, nNewHeight);

	//CDC srcDC, dstDC;
	//srcDC.CreateCompatibleDC(pDC);
	//dstDC.CreateCompatibleDC(pDC);

	//ReleaseDC(pDC);

	//CBitmap* pOldSrcBitmap = srcDC.SelectObject(&m_Bitmap);
	//CBitmap* pOldDstBitmap = dstDC.SelectObject(&ScaledBitmap);

	//dstDC.StretchBlt(0, 0, nNewWidth, nNewHeight, &srcDC, 0, 0, BitmapInfo.bmWidth, BitmapInfo.bmHeight, SRCCOPY);

	//srcDC.SelectObject(pOldSrcBitmap);
	//dstDC.SelectObject(pOldDstBitmap);

	//m_wndBmp.SetBitmap(HBITMAP(ScaledBitmap.Detach()));
	m_wndBmp.SetBitmap(HBITMAP(m_Bitmap.Detach()));

	//srcDC.DeleteDC();
	//dstDC.DeleteDC();
}

void CCMSplcSubCalcEccnDlg::GetDgnCodeEnum(UINT& enVal)
{
	CString strText;

	auto pComboBox = (CComboBox*)GetDlgItem(IDC_WG_CMD_CALC_ECCN_CODE_CMB);

	if (pComboBox)
	{
		int nSel = pComboBox->GetCurSel();
		if (nSel != CB_ERR) pComboBox->GetLBText(nSel, strText);
	}

	if ( strText == _T("Fixed Eccentricity")) enVal = EN_ECCN_DGN_CODE::EN_FIXED_ECCENTRICITY;
	else if ( strText == _T("NTC-2023 (Mexico)")) enVal = EN_ECCN_DGN_CODE::EN_DGN_CODE_NTCS_2023;
	else enVal = EN_ECCN_DGN_CODE::EN_DGN_CODE_NONE;
}

void CCMSplcSubCalcEccnDlg::ResetDilaog()
{
	CDlgUtil::CtrlShowHide(this, m_arNotFixedEccn, TRUE);

	if (!m_InitDlgRect.IsRectEmpty())
	{
		CRect CurrentRect;
		this->GetWindowRect(&CurrentRect);

		CurrentRect.right = CurrentRect.left + m_InitDlgRect.Width();
		CurrentRect.bottom = CurrentRect.top + m_InitDlgRect.Height();

		this->MoveWindow(&CurrentRect);
	}

	if (!m_InitGroupRect.IsRectEmpty())
	{
		GetDlgItem(IDC_WG_CMD_CALC_ECCN_TYPE_GRP)->MoveWindow(&m_InitGroupRect);
	}

	if (!m_InitOkRect.IsRectEmpty())
	{
		GetDlgItem(IDOK)->MoveWindow(&m_InitOkRect);
	}

	if (!m_InitCancelRect.IsRectEmpty())
	{
		GetDlgItem(IDCANCEL)->MoveWindow(&m_InitCancelRect);
	}

	return;
}

void CCMSplcSubCalcEccnDlg::AdjustDlg4DgnCode()
{
	CArray<UINT, UINT> arMoveCtrl;
	arMoveCtrl.RemoveAll();

	GetDgnCodeEnum(m_nDgnCode);
	switch (m_nDgnCode)
	{
	case EN_ECCN_DGN_CODE::EN_FIXED_ECCENTRICITY:
		{
			ResetDilaog();

			CDlgUtil::CtrlShowHide(this, m_arNotFixedEccn, FALSE);

			CRect rRef;
			CRect rToMove;
			int nDistY;

			arMoveCtrl.Add(IDC_WG_CMD_CALC_ECCN_TYPE_GRP);
			GetDlgItem(IDC_WG_CMD_CALC_ECCN_STC2)->GetWindowRect(rRef);
			GetDlgItem(IDC_WG_CMD_CALC_ECCN_PIC)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.bottom;

			CDlgUtil::CtrlResizeBottom(this, IDC_WG_CMD_CALC_ECCN_TYPE_GRP, nDistY);

			CRect CurrentRect;
			this->GetWindowRect(&CurrentRect);
			GetDlgItem(IDC_WG_CMD_CALC_ECCN_TYPE_GRP)->GetWindowRect(rRef);
			GetDlgItem(IDC_WG_CMD_CALC_ECCN_CONSIDER_GRP)->GetWindowRect(rToMove);
			nDistY = rRef.bottom - rToMove.bottom;

			arMoveCtrl.RemoveAll();
			arMoveCtrl.Add(IDOK);
			arMoveCtrl.Add(IDCANCEL);
			CDlgUtil::CtrlMoveDistY(this, arMoveCtrl, nDistY);

			CurrentRect.right = CurrentRect.left + m_InitDlgRect.Width();
			CurrentRect.bottom = CurrentRect.top + m_InitDlgRect.Height() + nDistY;

			this->MoveWindow(&CurrentRect);
		}
		break;
	case EN_ECCN_DGN_CODE::EN_DGN_CODE_NTCS_2023:
		{
			ResetDilaog();

			CDlgUtil::CtrlShowHide(this, m_arNotFixedEccn, TRUE);
		}
		break;
	default:
		{
			ResetDilaog();

			ASSERT(0); return;
		}
	}
}

void CCMSplcSubCalcEccnDlg::OnChkConsiderEccnbyAnal()
{
	const auto bPostMode = m_pDoc->IsPostMode();
	if (!bPostMode) return;

	if (m_chkConsiderEccnbyAnal.GetCheck())
		m_bConsiderEccnbyAnal = true;
	else
		m_bConsiderEccnbyAnal = false;

	CheckCtrlEnable();
}
