// DgnConCodeNewChild_5_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_5_Dlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "DgnDataCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild5Dlg dialog


CDgnConCodeNewChild5Dlg::CDgnConCodeNewChild5Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild5Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild5Dlg)
	m_bStrongColmWeakBeam = FALSE;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dMrft = 0.0;
	m_dTrft = 0.0;
	m_dConShearStrengthRatio = 0.0;
	m_nPMCurveMethod = 0;

	m_aStrongColmWeakBeam.RemoveAll();
	m_aStrongColmWeakBeam.Add(IDC_DGN_COLM_WEAK_BEAM_CHK);

	m_aShearforDesign.RemoveAll();
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR12);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR13);
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
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR21);

	m_aSCWBDesign.RemoveAll();
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_GRP);
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO);
	m_aSCWBDesign.Add(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO);

	m_aRdoSCWBMethod.RemoveAll();
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_DGN_STREN_RDO);
	m_aRdoSCWBMethod.Add(IDC_DGN_SCWB_METHOD_NOR_STREN_RDO);
	m_aRdoSCWBMethod.FreeExtra();

	m_aTorsionDesign.RemoveAll();
	m_aTorsionDesign.Add(IDC_DGN_TORSION_GROUP);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TORSION);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_STATIC);
	m_aTorsionDesign.Add(IDC_DGN_RCCTRL_TRFT_FACTOR);

	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

	m_aPMCurveMethod.RemoveAll();
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD);	
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_P_RADIO);	
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_MP_RADIO);	

    m_pDgnDataCtrl = new CDgnDataCtrl;
}

CDgnConCodeNewChild5Dlg::~CDgnConCodeNewChild5Dlg()
{
    _SAFE_DELETE(m_pDgnDataCtrl);
}

void CDgnConCodeNewChild5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild5Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION, m_bTorsionDesign);
	DDX_Check(pDX, IDC_DGN_COLM_WEAK_BEAM_CHK,  m_bStrongColmWeakBeam);

	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7 , m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR14, m_dConShearStrengthRatio);

	DDX_Text(pDX, IDC_DGN_RCCTRL_TRFT_FACTOR, m_dTrft);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);

	DDX_Control(pDX,  IDC_DGN_RCCTRL_TORSION, m_btnTorsion);
	DDX_Radio(pDX, IDC_DGN_PM_CURVE_METHOD_P_RADIO, m_nPMCurveMethod);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild5Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild5Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_TORSION, OnChangeTorsion)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild5Dlg message handlers

BOOL CDgnConCodeNewChild5Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild5Dlg::InitCtrl()
{

	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if(!m_bExistData)
	{
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
		m_pDgnDataCtrl->Get_DgnConTrftDef(strCode, m_dTrft); //m_dTrft = 1.0;//TW에서만 사용
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
		if (m_pData->DesignCode==_T("TWN-USD100")) // PMS:4385-PARKHJ-20111025 : 대만대리점 요청사항.
			CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, 1);  // 1: Norminal Strength
	}
	else
	{
		m_dMrft = m_pData->dMrft;
		m_dTrft = m_pData->dTrft;
		m_iAlphaType1 = m_pData->iAlphaType;
		m_dAlpha1 = m_pData->dAlpha1;
		m_dAlpha2 = m_pData->dAlpha2;
		m_bTorsionDesign = m_pData->bTorsionDesign;
		m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam;
		m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
		CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
		m_nPMCurveMethod = m_pData->nPMCurveMethod;
	}

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, 0, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);
	
	OnChangeTorsion();

	ResizeChildDlg();

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild5Dlg::OnDgnRcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ResizeChildDlg();

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild5Dlg::OnChangeAlphaType()
{
    UpdateData(TRUE);

    CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);
    SetEQCtrl(strCode);

    UpdateData(FALSE);
}

void CDgnConCodeNewChild5Dlg::OnUpdateByCode()
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

void CDgnConCodeNewChild5Dlg::OnChangeTorsion()
{
	if(m_btnTorsion.GetCheck())
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_RCCTRL_TRFT_FACTOR)->EnableWindow(FALSE);
	}
}

void CDgnConCodeNewChild5Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();
																	
	m_dMrft                  = m_pData->dMrft                 ;
	m_dTrft                  = m_pData->dTrft                 ;
	m_iAlphaType1            = m_pData->iAlphaType            ;
	m_dAlpha1                = m_pData->dAlpha1               ;
	m_dAlpha2                = m_pData->dAlpha2               ;
	m_bTorsionDesign         = m_pData->bTorsionDesign        ;
	m_dConShearStrengthRatio = m_pData->dConShearStrengthRatio;
	m_bStrongColmWeakBeam    = m_pData->bStrongColmWeakBeam   ;
	m_nPMCurveMethod		 = m_pData->nPMCurveMethod;
    CDlgUtil::CtrlRadioSetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
}

void CDgnConCodeNewChild5Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	UpdateData(FALSE);

	ResizeChildDlg();

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild5Dlg::ResizeChildDlg()
{ 
	CDlgUtil::CtrlShowHide(this, m_aStrongColmWeakBeam, m_bSpecialEQ);
	CDlgUtil::CtrlShowHide(this, m_aShearforDesign , m_bSpecialEQ);
	CDlgUtil::CtrlShowHide(this, m_aSCWBDesign , m_bSpecialEQ);

	UINT nIDRef = IDC_DGN_RCCTRL_SPECIAL, nIDTgt = IDC_DGN_RCCTRL_TORSION; 
    if (m_bSpecialEQ)
    {
         nIDRef = IDC_DGN_SCWB_METHOD_GRP;
    }
	
	CRect RectRef, RectTgt;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);

	int nDistY = RectRef.bottom - RectTgt.top + RectTgt.Height()/2;

	CDlgUtil::CtrlMoveDistY(this, m_aTorsionDesign, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);

	if(m_pData->DesignCode == _T("TWN-USD100"))
	{
		GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectRef);
		GetDlgItem(IDC_DGN_PM_CURVE_METHOD)->GetWindowRect(&RectTgt);

		nDistY = RectRef.bottom - RectTgt.top + globalUtils.ScaleByDPI(8);

		CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
	}

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);
	
	CDconChildDialog::ResizeChildDlg();
}

void CDgnConCodeNewChild5Dlg::SetEQCtrl(const CString& strCode)
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

BOOL CDgnConCodeNewChild5Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft       = m_dMrft;
	m_pData->dTrft       = m_dTrft;
	m_pData->iAlphaType  = m_iAlphaType1;
	m_pData->dAlpha1     = m_dAlpha1;
	m_pData->dAlpha2     = m_dAlpha2;
	m_pData->bTorsionDesign = m_bTorsionDesign;
	m_pData->dConShearStrengthRatio = m_dConShearStrengthRatio;
	m_pData->bStrongColmWeakBeam  = m_bStrongColmWeakBeam;

	CDlgUtil::CtrlRadioGetCheck(this, m_aRdoSCWBMethod, m_pData->nSCWBDgnMethod);
	m_pData->nPMCurveMethod	= m_nPMCurveMethod;
	CDconChildDialog::Dlg2Data();
	return TRUE;
}