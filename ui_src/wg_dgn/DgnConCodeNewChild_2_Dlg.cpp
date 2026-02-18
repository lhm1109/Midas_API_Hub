// DgnConCodeNewChild_2_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeNewChild_2_Dlg.h"

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
// CDgnConCodeNewChild2Dlg dialog


CDgnConCodeNewChild2Dlg::CDgnConCodeNewChild2Dlg(CDgnConCodeNewDlg* pParent /*=NULL*/, T_DCON_D* pData, BOOL bExistData/*=FALSE*/)
	: CDconChildDialog(CDgnConCodeNewChild2Dlg::IDD, pParent, pData, bExistData)
{
	//{{AFX_DATA_INIT(CDgnConCodeNewChild2Dlg)
// 	m_pData = pData;
	m_iAlphaType1 = -1;
	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	m_dMrft = 0.0;

 
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


	m_aMomentRedistr.RemoveAll();
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_STATIC);
	m_aMomentRedistr.Add(IDC_DGN_RCCTRL_MRFT_FACTOR);

    m_pDgnDataCtrl = new CDgnDataCtrl;
}

CDgnConCodeNewChild2Dlg::~CDgnConCodeNewChild2Dlg()
{
    _SAFE_DELETE(m_pDgnDataCtrl);
}

void CDgnConCodeNewChild2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDconChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeNewChild2Dlg)
	DDX_Check(pDX, IDC_DGN_RCCTRL_SPECIAL, m_bSpecialEQ);
	DDX_Radio(pDX, IDC_DGN_EQ_FACTOR1, m_iAlphaType1);
	
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR7 , m_dAlpha1);
	DDX_Text(pDX, IDC_DGN_EQ_FACTOR10, m_dAlpha2);
	DDX_Text(pDX, IDC_DGN_RCCTRL_MRFT_FACTOR, m_dMrft);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDgnConCodeNewChild2Dlg, CDconChildDialog)
	//{{AFX_MSG_MAP(CDgnConCodeNewChild2Dlg)
	ON_BN_CLICKED(IDC_DGN_RCCTRL_SPECIAL, OnDgnRcctrlSpecial)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR1, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR2, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR3, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR4, OnChangeAlphaType)
	ON_BN_CLICKED(IDC_DGN_EQ_FACTOR12, OnUpdateByCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeNewChild2Dlg message handlers

BOOL CDgnConCodeNewChild2Dlg::OnInitDialog() 
{
	// TODO: Add extra initialization here
	CDconChildDialog::OnInitDialog();
	
	InitCtrl();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnConCodeNewChild2Dlg::InitCtrl()
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

	}

	//SetComboBox();
	//SetShowExposure(strCode);

	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, 0, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);
	
	//OnChangeTorsion();

	ResizeChildDlg();

	SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
	//EnableWindowBySpecialWall();
}

void CDgnConCodeNewChild2Dlg::OnDgnRcctrlSpecial() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ResizeChildDlg();

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);
	//SetBndrElemClimitDesc(strCode);
}

void CDgnConCodeNewChild2Dlg::OnChangeAlphaType()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);
	SetEQCtrl(strCode);

	UpdateData(FALSE);
}

void CDgnConCodeNewChild2Dlg::OnUpdateByCode()
{
	UpdateData(TRUE);

	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);  m_iAlphaType1 = 0;

	m_dAlpha1 = 0.0;
	m_dAlpha2 = 0.0;
	
	m_pDgnDataCtrl->Get_DgnConSufsDef(strCode, m_iFrameType, m_iAlphaType1, m_dAlpha1, m_dAlpha2);
	UpdateData(FALSE);

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild2Dlg::UpdateChildData()
{
	CDconChildDialog::UpdateChildData();

	m_dMrft        = m_pData->dMrft     ;
	m_iAlphaType1  = m_pData->iAlphaType;
	m_dAlpha1      = m_pData->dAlpha1   ;
	m_dAlpha2      = m_pData->dAlpha2   ;

}

void CDgnConCodeNewChild2Dlg::ChangeDesignCode()
{
	CDconChildDialog::ChangeDesignCode();
	CString strCode= CDBLib::GetConvertCodeName(m_pData->DesignCode);

	if(strCode==_T("BS8110-97") || strCode==_T("Eurocode2"))
	{
		m_bSpecialEQ = FALSE;
		CDlgUtil::CtrlEnableDisable(this, IDC_DGN_RCCTRL_SPECIAL, FALSE);
	}
	else CDlgUtil::CtrlEnableDisable(this, IDC_DGN_RCCTRL_SPECIAL, TRUE);

	UpdateData(FALSE);
	ResizeChildDlg();

	SetEQCtrl(strCode);
}

void CDgnConCodeNewChild2Dlg::ResizeChildDlg()
{
	BOOL bShowShearDesign = m_bSpecialEQ && m_pData->DesignCode != _T("AIK-WSD2K");
	
	CDlgUtil::CtrlShowHide(this, m_aShearforDesign , bShowShearDesign);
	
	UINT nIDRef = IDC_DGN_RCCTRL_SPECIAL, nIDTorsion = IDC_DGN_RCCTRL_MRFT_STATIC; 
	if(bShowShearDesign) nIDRef = IDC_DGN_EQ_FACTOR0;
	
	CRect Ref, RectTorsion;
	GetDlgItem(nIDRef)->GetWindowRect(&Ref);
	GetDlgItem(nIDTorsion)->GetWindowRect(&RectTorsion);

	int nDistY = Ref.bottom - RectTorsion.top + RectTorsion.Height()/2;

	CDlgUtil::CtrlMoveDistY(this, m_aMomentRedistr, nDistY);

	CRect rMove;
	GetClientRect(rMove);
	rMove.bottom += nDistY;
	MoveWindow(rMove, TRUE);
	
	CDconChildDialog::ResizeChildDlg();
}

void CDgnConCodeNewChild2Dlg::SetEQCtrl(const CString& strCode)
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

BOOL CDgnConCodeNewChild2Dlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_pData->dMrft       = m_dMrft;
	m_pData->iAlphaType  = m_iAlphaType1;
	m_pData->dAlpha1     = m_dAlpha1;
	m_pData->dAlpha2     = m_dAlpha2;

	CDconChildDialog::Dlg2Data();
	return TRUE;
}