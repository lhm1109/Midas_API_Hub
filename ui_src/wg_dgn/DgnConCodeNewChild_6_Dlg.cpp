// DgnConCodeNewChild_6_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_6_Dlg.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "DgnDataCtrl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild6Dlg dialog


CDgnConCodeNewChild6Dlg::CDgnConCodeNewChild6Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild6Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild6Dlg)
// 	m_pData = pData;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dMrft = 0.0;
	m_iIs13920Sel = 0;
	m_bTorsionDesign = FALSE;
	m_nPMCurveMethod = 0;
    m_bBCJointDgn = TRUE; 
    m_iSCWBLast   = 0 ; 
    m_bStrongColmWeakBeam = FALSE; //Consider SCWB on Last Floor or Not
	m_bConsiderBECheck = FALSE;
	m_nBEEBarDesignMethod = 1;

	m_aDuctileDesign.RemoveAll();
	m_aDuctileDesign.Add(IDC_DGN_IS204_N25);
	m_aDuctileDesign.Add(IDC_DGN_IS204_N26);
	m_aDuctileDesign.Add(IDC_DGN_IS204_N27);
    m_aDuctileDesign.Add(IDC_DGN_IS456MAT_GROUP4);
    //m_aDuctileDesign.Add(IDC_DGN_IS456_BCJOINT_DGN);
    m_aDuctileDesign.Add(IDC_DGN_IS456_SCWB_LAFT_FLR);
    m_aDuctileDesign.Add(IDC_DGN_IS456_SCWB_LAFT_FLR2);
	m_aDuctileDesign.Add(IDC_DGN_IS456MAT_GROUP5);
	m_aDuctileDesign.Add(IDC_DGN_IS456_CONSIDER_BE_CHK);
	m_aDuctileDesign.Add(IDC_DGN_IS456_BE_MA_RDO);
	m_aDuctileDesign.Add(IDC_DGN_IS456_BE_MB_RDO);
 
	m_aShearforDesign.RemoveAll();
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR0);
	m_aShearforDesign.Add(IDC_DGN_EQ_FACTOR12);
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

	m_aMatProperty.RemoveAll();
	m_aMatProperty.Add(IDC_DGN_IS456MAT_GROUP);
	m_aMatProperty.Add(IDC_DGN_RCCTRL_IS456MAT_STATIC);
	m_aMatProperty.Add(IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR);
	m_aMatProperty.Add(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX);
	m_aMatProperty.Add(IDC_DGN_RCCTRL_IS456MAT_SUBGRDREXFACTOR);
	m_aMatProperty.Add(IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT);

	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_TORSION);
	
	m_aPMCurveMethod.RemoveAll();
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_P_RADIO);
	m_aPMCurveMethod.Add(IDC_DGN_PM_CURVE_METHOD_MP_RADIO);
}

CDgnConCodeNewChild6Dlg::~CDgnConCodeNewChild6Dlg()
{
}

void CDgnConCodeNewChild6Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild6Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Check(pDX, IDC_DGN_RCCTRL_TORSION, m_bTorsionDesign);

	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	DDX_Radio(pDX, IDC_DGN_IS204_N26, m_iIs13920Sel);

	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7 , m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);

	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_SOILFACTOR , m_dedtFOS);
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_SUBGRDREXFACTOR, m_dedtSubRex); //Pinakin added for IS456 MAT design
	DDX_Control(pDX, IDC_DGN_RCCTRL_IS456MAT_STATIC_SUBGRADEREX_MODULUS_UNIT, m_ctrlUnit); //Pinakin added for IS456 MAT design

	DDX_Radio(pDX, IDC_DGN_PM_CURVE_METHOD_P_RADIO, m_nPMCurveMethod);
    //DDX_Check(pDX, IDC_DGN_IS456_BCJOINT_DGN,  m_bBCJointDgn );
    DDX_Check(pDX, IDC_DGN_IS456_SCWB_LAFT_FLR, m_iSCWBLast  );  
    DDX_Check(pDX, IDC_DGN_IS456_SCWB_LAFT_FLR2,  m_bStrongColmWeakBeam); //Consider SCWB on Last Floor or Not

	DDX_Check(pDX, IDC_DGN_IS456_CONSIDER_BE_CHK, m_bConsiderBECheck); //Consider boundary element design
	DDX_Radio(pDX, IDC_DGN_IS456_BE_MA_RDO, m_nBEEBarDesignMethod);  //Design Method for boundary element
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild6Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild6Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
    ON_BN_CLICKED(IDC_DGN_IS204_N26, OnBnClickedDgnIs13920_1993)
    ON_BN_CLICKED(IDC_DGN_IS204_N27, OnBnClickedDgnIs13920_2016)
    ON_BN_CLICKED(IDC_DGN_PM_CURVE_METHOD_P_RADIO, OnBnClickedDgnPmCurveMethodPRadio)
    ON_BN_CLICKED(IDC_DGN_PM_CURVE_METHOD_MP_RADIO, OnBnClickedDgnPmCurveMethodMpRadio)  
    ON_BN_CLICKED(IDC_DGN_IS456_SCWB_LAFT_FLR, OnBnClickedDgnIs456ScwbLaftFlr)
	ON_BN_CLICKED(IDC_DGN_IS456_CONSIDER_BE_CHK, OnBnConsiderBEDesign)
	ON_BN_CLICKED(IDC_DGN_IS456_BE_MA_RDO, OnBnClickedMethodARadio)
	ON_BN_CLICKED(IDC_DGN_IS456_BE_MB_RDO, OnBnClickedMethodBRadio)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild6Dlg message handlers

BOOL CDgnConCodeNewChild6Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild6Dlg::InitCtrl()
{
	CString strCode = CDBLib::GetConvertCodeName(m_pData->DesignCode);

	m_dedtFOS.SetUnitType(D_UNITSYS_NONE);
	m_dedtSubRex.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_ctrlUnit.SetUnitType(D_UNITSYS_BASE_DENSITY);

	if(!m_bExistData)
	{
		m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
		CheckDlgButton(m_bConsiderBECheck, FALSE);
		GetDlgItem(IDC_DGN_IS456_BE_MA_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_IS456_BE_MB_RDO)->EnableWindow(FALSE);
		CButton* radioBtn = (CButton*)GetDlgItem(IDC_DGN_IS456_BE_MA_RDO);
		radioBtn->SetCheck(TRUE);
	}
	else
	{
		m_dMrft = m_pData->dMrft;
		m_iAlphaType1 = m_pData->iAlphaType;
		m_dAlpha1 = m_pData->dAlpha1;
		m_dAlpha2 = m_pData->dAlpha2;
		m_iIs13920Sel = m_pData->iSubCode;
		m_bTorsionDesign = m_pData->bTorsionDesign;
		m_nPMCurveMethod = m_pData->nPMCurveMethod;
        m_bBCJointDgn    = m_pData->bBCJointDgn;
        m_iSCWBLast      = m_pData->nSCWBDgnMethod;
        m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam; //Consider SCWB on Last Floor or Not
		m_bConsiderBECheck = m_pData->bConsiderBEDesign;
		m_nBEEBarDesignMethod = m_pData->nBEEBarDesignMethod - 1;
	}
	m_dedtFOS.SetEditUnit(m_pData->dFOS);
	m_dedtSubRex.SetEditUnit(m_pData->dSubRex);
    if (m_iIs13920Sel == 0) OnBnClickedDgnIs13920_1993();
    else OnBnClickedDgnIs13920_2016();
	CButton* ctrlBECheck = (CButton*)GetDlgItem(IDC_DGN_IS456_CONSIDER_BE_CHK);
	ctrlBECheck->SetCheck(m_bConsiderBECheck);
	OnBnConsiderBEDesign();
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, 0, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);
	
	//OnChangeTorsion();

	ResizeChildDlg();

	SetEQCtrl(strCode);
    OnBnClickedDgnIs456ScwbLaftFlr();
	//SetBndrElemClimitDesc(strCode);
	//EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild6Dlg::OnDgnRcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ResizeChildDlg();

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
}

void CDgnConCodeNewChild6Dlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild6Dlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);  m_iAlphaType1 = 0;

	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild6Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_dMrft        = m_pData->dMrft     ;
	m_iAlphaType1  = m_pData->iAlphaType;
	m_dAlpha1      = m_pData->dAlpha1   ;
	m_dAlpha2      = m_pData->dAlpha2   ;
	m_bTorsionDesign = m_pData->bTorsionDesign;

	m_iIs13920Sel  = m_pData->iSubCode;
	m_dedtFOS.SetEditUnit(m_pData->dFOS);
	m_dedtSubRex.SetEditUnit(m_pData->dSubRex);
	m_nPMCurveMethod = m_pData->nPMCurveMethod;
    m_bBCJointDgn    = m_pData->bBCJointDgn;
    m_iSCWBLast      = m_pData->nSCWBDgnMethod;
    m_bStrongColmWeakBeam = m_pData->bStrongColmWeakBeam; //Consider SCWB on Last Floor or Not
	m_bConsiderBECheck = m_pData->bConsiderBEDesign;
	m_nBEEBarDesignMethod = m_pData->nBEEBarDesignMethod - 1;
}


void CDgnConCodeNewChild6Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	UpdateData(FALSE);

	ResizeChildDlg();

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
}
void CDgnConCodeNewChild6Dlg::ResizeChildDlg()
{ 
	CDlgUtil::CtrlShowHide(this, m_aDuctileDesign , m_bSpecialEQ);
	CDlgUtil::CtrlShowHide(this, m_aShearforDesign , m_bSpecialEQ);
	
	UINT nIDRef = IDC_DGN_RCCTRL_SPECIAL, nIDTgt = IDC_DGN_IS456MAT_GROUP; 
	if(m_bSpecialEQ) nIDRef = IDC_DGN_EQ_FACTOR0;
	
	CRect RectRef, RectTgt, RectTorsion;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
	GetDlgItem(IDC_DGN_RCCTRL_TORSION)->GetWindowRect(&RectTorsion); // 간격을 넣기 위해

	int nDistY = RectRef.bottom - RectTgt.top + RectTorsion.Height()/2;

	CDlgUtil::CtrlMoveDistY(this, m_aMatProperty, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);

	if(m_pData->DesignCode == _T("IS456:2000"))
	{
		GetDlgItem(IDC_DGN_RCCTRL_TORSION)->GetWindowRect(&RectRef);
		GetDlgItem(IDC_DGN_PM_CURVE_METHOD)->GetWindowRect(&RectTgt);

		nDistY = RectRef.bottom - RectTgt.top + globalUtils.ScaleByDPI(8);

		CDlgUtil::CtrlMoveDistY(this, m_aPMCurveMethod, nDistY);
	}

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);
	
	CDconChildDialog::ResizeChildDlg();
	//Invalidate();
}

void CDgnConCodeNewChild6Dlg::SetEQCtrl(const CString& strCode)
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
		//GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
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
		//GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_ACI));
	}
	else if(iStatus==15)
	{
		strQ.Format(_T("%s"), _LS(IDS_DGN_TEXT_SUFS_SHEAR_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR5)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA1_TWN));
		GetDlgItem(IDC_DGN_EQ_FACTOR8)->SetWindowText(_LS(IDS_DGN_TEXT_SUFS_ALPHA2_TWN));
		//GetDlgItem(IDC_DGN_EQ_FACTOR13)->SetWindowText(_LS(IDS_DGN_TEXT_R_VC_SHEAR_TWN));
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
}

BOOL CDgnConCodeNewChild6Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft       = m_dMrft;
	m_pData->iAlphaType  = m_iAlphaType1;
	m_pData->dAlpha1     = m_dAlpha1;
	m_pData->dAlpha2     = m_dAlpha2;

    CDlgUtil::CtrlRadioGetCheck(this, m_aDuctileDesign, m_iIs13920Sel);
    
	m_pData->iSubCode    = m_iIs13920Sel-1;
	m_pData->dFOS        = m_dedtFOS.GetEditValue();;
	m_pData->dSubRex		 = m_dedtSubRex.GetEditValue();
	m_pData->bTorsionDesign = m_bTorsionDesign;
	m_pData->nPMCurveMethod = m_nPMCurveMethod>1 ? 1:m_nPMCurveMethod;
    //SCWB only for IS13920 2016 Ductile Selection
    //Joint Design To be done only if IS13920-2016 + SCWB Design is selected
    m_pData->bBCJointDgn     =  m_pData->iSubCode == 0 ? FALSE : m_iSCWBLast > 0 ? TRUE : FALSE ;
    m_pData->nSCWBDgnMethod  =  m_pData->iSubCode == 0 ? 0     : m_iSCWBLast   ;
    m_pData->dStorColmWeakBeamFactor = 1.4;
    m_pData->bStrongColmWeakBeam   = m_bStrongColmWeakBeam;//Consider SCWB on Last Floor or Not
    m_pData->bCheckBeamDeflect = TRUE;

	m_pData->bConsiderBEDesign = m_bConsiderBECheck;
	m_pData->nBEEBarDesignMethod = m_nBEEBarDesignMethod + 1;
    
	CDconChildDialog::Dlg2Data();
	return TRUE;
}

void CDgnConCodeNewChild6Dlg::OnBnClickedDgnIs13920_1993()
{
    // TODO: Add your control notification handler code here
	m_aDuctileDesign.RemoveAt( 7, 3 );
    CDlgUtil::CtrlRadioSetCheck(this,m_aDuctileDesign,1);
	m_aDuctileDesign.Add( IDC_DGN_IS456_CONSIDER_BE_CHK );
	m_aDuctileDesign.Add( IDC_DGN_IS456_BE_MA_RDO );
	m_aDuctileDesign.Add( IDC_DGN_IS456_BE_MB_RDO );
    //GetDlgItem(IDC_DGN_IS456_BCJOINT_DGN)->EnableWindow(FALSE);
    GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR)->EnableWindow(FALSE);  
    GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR2)->EnableWindow(FALSE);
	GetDlgItem( IDC_DGN_IS456_CONSIDER_BE_CHK )->EnableWindow(FALSE);
	GetDlgItem( IDC_DGN_IS456_BE_MA_RDO )->EnableWindow(FALSE);
	GetDlgItem( IDC_DGN_IS456_BE_MB_RDO )->EnableWindow(FALSE);
    m_iIs13920Sel = 0;
}

void CDgnConCodeNewChild6Dlg::OnBnClickedDgnIs13920_2016()
{
    // ENable only Check Box of SCWB
	m_aDuctileDesign.RemoveAt( 7, 3 );
    CDlgUtil::CtrlRadioSetCheck(this,m_aDuctileDesign,2);   
	m_aDuctileDesign.Add( IDC_DGN_IS456_CONSIDER_BE_CHK );
	m_aDuctileDesign.Add( IDC_DGN_IS456_BE_MA_RDO );
	m_aDuctileDesign.Add( IDC_DGN_IS456_BE_MB_RDO );
    //GetDlgItem(IDC_DGN_IS456_BCJOINT_DGN)->EnableWindow(TRUE);
    //GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR)->EnableWindow(TRUE);   GEN-2907 Disable
    GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR)->EnableWindow(TRUE);
    GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR2)->EnableWindow(FALSE);
	GetDlgItem( IDC_DGN_IS456_CONSIDER_BE_CHK )->EnableWindow(TRUE);
	GetDlgItem( IDC_DGN_IS456_BE_MA_RDO )->EnableWindow(m_bConsiderBECheck);
	GetDlgItem( IDC_DGN_IS456_BE_MB_RDO )->EnableWindow(m_bConsiderBECheck);
    m_iIs13920Sel = 1;
}


void CDgnConCodeNewChild6Dlg::OnBnClickedDgnPmCurveMethodPRadio()
{
    // TODO: Add your control notification handler code here
   /* if (m_iIs13920Sel = 0) OnBnClickedDgnIs13920_1993();
    else if (m_iIs13920Sel = 1) OnBnClickedDgnIs13920_2016();*/
    m_nPMCurveMethod = 0;
    
}


void CDgnConCodeNewChild6Dlg::OnBnClickedDgnPmCurveMethodMpRadio()
{
    // TODO: Add your control notification handler code here
   /* if (m_iIs13920Sel = 0) OnBnClickedDgnIs13920_1993();
    else if (m_iIs13920Sel = 1) OnBnClickedDgnIs13920_2016();*/
    m_nPMCurveMethod = 1;
}




void CDgnConCodeNewChild6Dlg::OnBnClickedDgnIs456ScwbLaftFlr()
{
    // IS13920 2016 GUI Fixes for Last Floor COnsideration
    bool IsCheck1Checked;
    CButton *ctlCheck1 = (CButton*)GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR);
    IsCheck1Checked = (ctlCheck1->GetCheck() == 1) ? true : false;
    if (IsCheck1Checked) GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR2)->EnableWindow(TRUE);
    else 
    {
        CButton *ctlCheck2 = (CButton*)GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR2);
        ctlCheck2->SetCheck(false);
        GetDlgItem(IDC_DGN_IS456_SCWB_LAFT_FLR2)->EnableWindow(FALSE);
    }
    
}

void CDgnConCodeNewChild6Dlg::OnBnConsiderBEDesign()
{
	m_bConsiderBECheck = IsDlgButtonChecked(IDC_DGN_IS456_CONSIDER_BE_CHK);
	CButton* radioBtn = (CButton*)GetDlgItem( IDC_DGN_IS456_BE_MA_RDO );
	radioBtn->SetCheck( TRUE );
	radioBtn = (CButton*)GetDlgItem(IDC_DGN_IS456_BE_MB_RDO);
	radioBtn->SetCheck(FALSE);
	GetDlgItem(IDC_DGN_IS456_BE_MA_RDO)->EnableWindow(m_bConsiderBECheck);
	GetDlgItem(IDC_DGN_IS456_BE_MB_RDO)->EnableWindow(m_bConsiderBECheck);
}

void CDgnConCodeNewChild6Dlg::OnBnClickedMethodARadio()
{
	m_nBEEBarDesignMethod = 1;
}

void CDgnConCodeNewChild6Dlg::OnBnClickedMethodBRadio()
{
	m_nBEEBarDesignMethod = 2;
}
