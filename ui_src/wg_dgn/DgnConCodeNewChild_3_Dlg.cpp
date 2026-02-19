// DgnConCodeNewChild_3_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_3_Dlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild3Dlg dialog


CDgnConCodeNewChild3Dlg::CDgnConCodeNewChild3Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild3Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild3Dlg)
	m_bStrongColmWeakBeam = FALSE;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dConShearStrengthRatio = 0.0;

	m_dMrft = 0.0;

	m_aSelectFrameType.RemoveAll();
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI0);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI1);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI2);
	m_aSelectFrameType.Add(IDC_DGN_EQ_ACI3);
	
	m_aStrongColmWeakBeam.RemoveAll();
	m_aStrongColmWeakBeam.Add(IDC_DGN_COLM_WEAK_BEAM_CHK);

	m_aShearforDesign.RemoveAll();
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR0);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR12);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR13);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR21);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR14);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR11);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR1);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR2);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR3);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR4);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR5);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR6);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR7);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR8);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR9);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR10);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR20);
	
	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

	m_arCtrlSDR.RemoveAll();
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR11);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR1);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR2);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR3);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR4);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR5);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR6);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR7);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR8);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR9);
	m_arCtrlSDR.Add(IDC_DGN_EQ_FACTOR10);
}

void CDgnConCodeNewChild3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild3Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_COLM_WEAK_BEAM_CHK,  m_bStrongColmWeakBeam);

	DDX_Radio(pDX, IDC_DGN_EQ_ACI1, m_iFrameType);
	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);

	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7 , m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR14, m_dConShearStrengthRatio);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild3Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild3Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI1, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI2, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_ACI3, OnChangeFrameType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild3Dlg message handlers

BOOL CDgnConCodeNewChild3Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild3Dlg::InitCtrl()
{

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if(!m_bExistData)
	{
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
	}
	else
	{
		m_dMrft = m_pData->dMrft;
		m_iAlphaType1 = m_pData->iAlphaType;
		m_dAlpha1 = m_pData->dAlpha1;
		m_dAlpha2 = m_pData->dAlpha2;
		m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
		m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
	}

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);
	
	ResizeChildDlg();

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild3Dlg::OnDgnRcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ResizeChildDlg();

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild3Dlg::OnChangeFrameType()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	
	m_iAlphaType1 = 0;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);

	ResizeChildDlg();

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild3Dlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild3Dlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);  m_iAlphaType1 = 0;

	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dConShearStrengthRatio = 0.0;

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild3Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_dMrft        = m_pData->dMrft     ;
	m_iAlphaType1  = m_pData->iAlphaType;
	m_dAlpha1      = m_pData->dAlpha1   ;
	m_dAlpha2      = m_pData->dAlpha2   ;
	m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
	m_bStrongColmWeakBeam    = m_pData->bStrongColmWeakBeam   ;
}

void CDgnConCodeNewChild3Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);

	OnChangeFrameType();
}

void CDgnConCodeNewChild3Dlg::ResizeChildDlg()
{
	CDlgUtil::CtrlShowHide(this, m_aSelectFrameType, m_bSpecialEQ);
	CDlgUtil::CtrlShowHide(this, m_aStrongColmWeakBeam, m_bSpecialEQ);
	CDlgUtil::CtrlShowHide(this, m_aShearforDesign , m_bSpecialEQ);
	
	UINT nIDRef = IDC_DGN_RCCTRL_SPECIAL, nIDTgt = IDC_DGN_RCCTRL_MRFT_STATIC; 
	if(m_bSpecialEQ)
	{
		if(m_iFrameType==0) nIDRef = IDC_DGN_EQ_FACTOR20;
		else nIDRef = IDC_DGN_EQ_FACTOR0;

		ReAlingShearDesignCtrl();
	}  
	
	CRect RectRef, RectTgt;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);

	int nDistY = RectRef.bottom - RectTgt.top + RectTgt.Height()/2;

	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);


	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);
	
	CDconChildDialog::ResizeChildDlg();
	//Invalidate();
}

void CDgnConCodeNewChild3Dlg::ReAlingShearDesignCtrl()
{
	// Shear for Design 위치를 위해서 사용한 Ctrl 들은 우선 안보이게 함
	GetDlgItem(IDC_DGN_EQ_FACTOR16)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_EQ_FACTOR17)->ShowWindow(SW_HIDE);	

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
	int iStatus	= GetDataStatus(strCode);

	CWnd* pSDR = GetDlgItem(IDC_DGN_EQ_FACTOR11);
	CWnd* pSDRHide = GetDlgItem(IDC_DGN_EQ_FACTOR16);
	CWnd* pSDRShow = GetDlgItem(IDC_DGN_EQ_FACTOR17);
	CRect SDRRect, SDRHideRect, SDRShowRect;
	pSDR->GetWindowRect(SDRRect);
	pSDRHide->GetWindowRect(SDRHideRect);
	pSDRShow->GetWindowRect(SDRShowRect);

	int DY_SDR = 0;
	if((iStatus==10 && m_iFrameType==0) || (iStatus==18 && m_iFrameType==0) || (iStatus==20 && m_iFrameType==0) || iStatus==15)
	{
		DY_SDR = SDRShowRect.top - SDRRect.top;
		GetDlgItem(IDC_DGN_EQ_FACTOR0)->ShowWindow(SW_HIDE);		
		GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_SHOW);		
	}
	else
	{
		DY_SDR = SDRHideRect.top - SDRRect.top;
		GetDlgItem(IDC_DGN_EQ_FACTOR0)->ShowWindow(SW_SHOW);		
		GetDlgItem(IDC_DGN_EQ_FACTOR20)->ShowWindow(SW_HIDE);		
	}

	CDlgUtil::CtrlMoveDistY(this, m_arCtrlSDR, DY_SDR, TRUE);
}

void CDgnConCodeNewChild3Dlg::SetEQCtrl(const CString& strCode)
{
	CString strTxt=_T("");
	int iStatus	= GetDataStatus(strCode);

	if(iStatus==9 || iStatus==10 || iStatus==11 || iStatus==12 || iStatus==13 || 
		iStatus==14 || iStatus==15 || iStatus==16 || iStatus==18 || iStatus==20)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(FALSE);

		if(m_iAlphaType1==0 || m_iAlphaType1==1)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
		}
		else if(m_iAlphaType1==2)
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR6)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR7)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR9)->EnableWindow(TRUE);
			GetDlgItem(IDC_DGN_EQ_FACTOR10)->EnableWindow(TRUE);
		}
	}

	CString strQ=_T("");
	if(iStatus==9)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_AIJ));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_AIJ));
	}
	else if(iStatus==10 || iStatus==18 || iStatus==20)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_ACI));
		if(m_iFrameType==0) //Speical
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_SPECIAL_ACI));
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
		}
		else //Intermediate, Ordinary
		{
			GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_ACI));
			GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
		}
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
	}
	else if(iStatus==12)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_KSCE));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_KSCE));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_KSCE));
	}
	else if(iStatus==11 || iStatus==13 || iStatus==14)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_ACI));
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
	}
	else if(iStatus==15)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_TWN));
	}
	else if(iStatus==16)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_IS));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_IS));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_IS));
	}

	if(strQ!=_T(""))
	{
		CString strMax=_T(""), strMin=_T(""), strQ1=_T(""), strQ2=_T("");
		strQ1.Format(_T("%s1"), strQ);
		strQ2.Format(_T("%s2"), strQ);
		strMax.Format(_T("MAX(%s,%s)"), strQ1,strQ2);
		strMin.Format(_T("MIN(%s,%s)"), strQ1,strQ2);
		GetDlgItem(IDC_DGN_EQ_FACTOR1)->SetWindowText(strMax);
		GetDlgItem(IDC_DGN_EQ_FACTOR2)->SetWindowText(strMin);
		GetDlgItem(IDC_DGN_EQ_FACTOR3)->SetWindowText(strQ1);
		GetDlgItem(IDC_DGN_EQ_FACTOR4)->SetWindowText(strQ2);
	}

	// Add by ZINU.('08.03.20). NO:3292, Multiply R by Vc (Vu1 > 0.5*Vu2).
	// ACI318-Series only Special Moment Frame, TWN-USD92.
	if((iStatus==10 && m_iFrameType==0) || (iStatus==18 && m_iFrameType==0) || (iStatus==20 && m_iFrameType==0) ||iStatus==15)
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_EQ_FACTOR14)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_DGN_EQ_FACTOR21)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_DGN_EQ_FACTOR13)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EQ_FACTOR14)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_EQ_FACTOR21)->ShowWindow(SW_HIDE);
	}
}

BOOL CDgnConCodeNewChild3Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft       = m_dMrft;
	m_pData->iAlphaType  = m_iAlphaType1;
	m_pData->dAlpha1     = m_dAlpha1;
	m_pData->dAlpha2     = m_dAlpha2;
	m_pData->dConShearStrengthRatio = m_dConShearStrengthRatio;
	m_pData->bStrongColmWeakBeam    = m_bStrongColmWeakBeam;

	CDconChildDialog::Dlg2Data();
	return TRUE;
}