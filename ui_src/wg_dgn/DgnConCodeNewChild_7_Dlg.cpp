// DgnConCodeNewChild_7_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_7_Dlg.h"
#include "DgnCodeCtrl.h"
#include "DgnDataCtrl.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild7Dlg dialog


CDgnConCodeNewChild7Dlg::CDgnConCodeNewChild7Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild7Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild7Dlg)
// 	m_pData = pData;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dMrft = 0.0;

	m_nBeamEndLocRC = 1;
	m_nColmEndLocRC = 1;
	m_nBeamEndLocRC_ST = 0;
	m_nColmEndLocRC_ST = 0;

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

	m_aSelectMethod.RemoveAll();
	m_aSelectMethod.Add(IDC_DGN_COLM_METHOD0);
	m_aSelectMethod.Add(IDC_DGN_COLM_METHOD1);
	m_aSelectMethod.Add(IDC_DGN_COLM_METHOD2);
	m_aSelectMethod.Add(IDC_DGN_COLM_METHOD3);

	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC0);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC1);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC5);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC2);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC17);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC6);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC7);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC8);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC3);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC4);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT1_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT2_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT3_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT1_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT2_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT3_RDO);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC19);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC20);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC21);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC22);
	m_aMomentRedistr.Add(IDC_WG_JUD_STATIC23);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT1_ST_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT2_ST_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT3_ST_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT1_ST_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT2_ST_RDO);
	m_aMomentRedistr.Add(IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT3_ST_RDO);
}

void CDgnConCodeNewChild7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild7Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	DDX_Radio(pDX, IDC_DGN_COLM_METHOD1, m_iColmMethod);

	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7 , m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);

	DDX_Radio(pDX, IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT1_RDO, m_nBeamEndLocRC);
	DDX_Radio(pDX, IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT1_RDO, m_nColmEndLocRC);
	DDX_Radio(pDX, IDC_JUD_CALC_SECT_BEAM_ENDLOC_RC_OPT1_ST_RDO, m_nBeamEndLocRC_ST);
	DDX_Radio(pDX, IDC_JUD_CALC_SECT_COLM_ENDLOC_RC_OPT1_ST_RDO, m_nColmEndLocRC_ST);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild7Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild7Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild7Dlg message handlers

BOOL CDgnConCodeNewChild7Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild7Dlg::InitCtrl()
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
		m_nBeamEndLocRC = m_pData->nBeamEndLocRC;
		m_nColmEndLocRC = m_pData->nColmEndLocRC;
		m_nBeamEndLocRC_ST = m_pData->nBeamEndLocRC_ST;
		m_nColmEndLocRC_ST = m_pData->nColmEndLocRC_ST;
	}



	//SetComboBox();
	//SetShowExposure(strCode);

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iColmMethod, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);
	
	//OnChangeTorsion();

	ResizeChildDlg();

	SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
	//EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild7Dlg::OnDgnRcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ResizeChildDlg();

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
}

void CDgnConCodeNewChild7Dlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild7Dlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);  m_iAlphaType1 = 0;

	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild7Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_dMrft                  = m_pData->dMrft     ;
	m_iAlphaType1            = m_pData->iAlphaType;
	m_dAlpha1                = m_pData->dAlpha1   ;
	m_dAlpha2                = m_pData->dAlpha2   ;

	m_nBeamEndLocRC		= m_pData->nBeamEndLocRC;
	m_nColmEndLocRC		= m_pData->nColmEndLocRC;
	m_nBeamEndLocRC_ST = m_pData->nBeamEndLocRC_ST;
	m_nColmEndLocRC_ST = m_pData->nColmEndLocRC_ST;

}

void CDgnConCodeNewChild7Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);

	ResizeChildDlg();

	SetEQCtrl(strCode);
	m_pDgnDataCtrl->Get_DgnConMrftDef(strCode, m_dMrft);
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iColmMethod, m_iAlphaType1, m_dAlpha1, m_dAlpha2);

	UpdateData(FALSE);
	
}
void CDgnConCodeNewChild7Dlg::ResizeChildDlg()
{ 
	// PMS 5795-49 : Select Method for Columns and Braces 컨트롤은 일문일 때 보여주지 않도록 처리
	BOOL bJP=FALSE;	// 통합되면서 해외랑 같이 쓰도록 함

	int nConCode = CDgnCodeCtrl::GetConCodeID(m_pData->DesignCode);
	BOOL bHideSelectMehtod = nConCode==AIJ_WSD99 && bJP;

	CDlgUtil::CtrlShowHide(this, m_aShearforDesign , m_bSpecialEQ);
	if(bHideSelectMehtod) CDlgUtil::CtrlShowHide(this, m_aSelectMethod, FALSE);

	UINT nIDRef = m_bSpecialEQ ? IDC_DGN_EQ_FACTOR0 : IDC_DGN_RCCTRL_SPECIAL;
	UINT nIDTgt = bHideSelectMehtod ? IDC_DGN_RCCTRL_MRFT_STATIC : IDC_DGN_COLM_METHOD0; 
	
	CRect RectRef, RectTgt, RectGap;
	GetDlgItem(nIDRef)->GetWindowRect(&RectRef);
	GetDlgItem(nIDTgt)->GetWindowRect(&RectTgt);
	//GetDlgItem(IDC_DGN_RCCTRL_MRFT_STATIC)->GetWindowRect(&RectGap);

	int nDistY = RectRef.bottom - RectTgt.top + globalUtils.ScaleByDPI(10);

	if(!bHideSelectMehtod) CDlgUtil::CtrlMoveDistY(this, m_aSelectMethod, nDistY);
	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);
	
	CDconChildDialog::ResizeChildDlg();
	//Invalidate();
}

void CDgnConCodeNewChild7Dlg::SetEQCtrl(const CString& strCode)
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

BOOL CDgnConCodeNewChild7Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft       = m_dMrft;
	m_pData->iAlphaType  = m_iAlphaType1;
	m_pData->dAlpha1     = m_dAlpha1;
	m_pData->dAlpha2     = m_dAlpha2;

	m_pData->nBeamEndLocRC = m_nBeamEndLocRC;
	m_pData->nColmEndLocRC = m_nColmEndLocRC;
	m_pData->nBeamEndLocRC_ST = m_nBeamEndLocRC_ST;
	m_pData->nColmEndLocRC_ST = m_nColmEndLocRC_ST;

	CDconChildDialog::Dlg2Data();
	return TRUE;
}