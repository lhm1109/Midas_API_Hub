// CMSdveDisp.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSdveDlg.h"
#include "CMSdvePropDlg.h"

#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\IGenSDCDBManager.h"
#include "..\wg_base\wg_base_TestEnvMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCMSdveDlg dialog

CCMSdveDlg::CCMSdveDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMSdveDlg::IDD)
{
	//{{AFX_DATA_INIT(CCMSdveDlg)
	m_bModify = FALSE;
	m_key = 0;
	m_data.Initialize();

	m_Name = _T("");
	m_Desc = _T("");
	m_nInputMethod = 0;

	m_pIGenSDCDB=CIGenSDCDBManager::GetInstance();
	m_bOnInit = TRUE;

	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVE_SA_EDT);
	m_aCtrlID_DBValue_ReadOnly.Add(IDC_ETC_SDVE_TH_EDT);
	m_aCtrlID_DBValue_Disabled.Add(IDC_ETC_SDVE_MATLTYPE_CMB);
	//}}AFX_DATA_INIT
}

CCMSdveDlg::~CCMSdveDlg()
{
}

void CCMSdveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSdveDlg)
	DDX_Control(pDX, IDC_ETC_SDVE_COMPANY_CMB, m_cmbCompany);
	DDX_Control(pDX, IDC_ETC_SDVE_PRONAME_CMB, m_cmbProductName);
	DDX_Control(pDX, IDC_ETC_SDVE_TYPENUM_CMB, m_cmbTypeNumber);
	DDX_Control(pDX, IDC_ETC_SDVE_MATLTYPE_CMB, m_cmbMatlType);
	DDX_Control(pDX, IDC_ETC_SDVE_DIR_CMB     , m_cmbDir     );
	//DDX_Control(pDX, IDC_ETC_SDVE_CTC_CMB     , m_cmbCTC     );
	DDX_Text   (pDX, IDC_ETC_SDVE_NAME_EDT    , m_Name);
	DDX_Text   (pDX, IDC_ETC_SDVE_DESC_EDT    , m_Desc);
	DDX_Radio  (pDX, IDC_ETC_SDVE_USER_RDO    , m_nInputMethod);
	DDX_Control(pDX, IDC_ETC_SDVE_SA_EDT      ,  m_edtSA);
	DDX_Control(pDX, IDC_ETC_SDVE_TH_EDT      ,  m_edtTh);
	DDX_Control(pDX, IDC_ETC_SDVE_MP_EDT      ,  m_edtMp);
	DDX_Control(pDX, IDC_ETC_SDVE_LD_EDT      ,  m_edtLD);
	DDX_Check  (pDX, IDC_ETC_SDVE_MS_CHK      ,  m_bMS);
	DDX_Control(pDX, IDC_ETC_SDVE_MS_EDT      ,  m_edtMS);
	DDX_Control(pDX, IDC_ETC_SDVE_ES_EDT      ,  m_edtES);
	DDX_Control(pDX, IDC_ETC_SDVE_ED_EDT      ,  m_edtED);
	DDX_Check  (pDX, IDC_ETC_SDVE_KF_CHK      ,  m_bKF);
	DDX_Control(pDX, IDC_ETC_SDVE_KF_EDT      ,  m_edtKF);
	DDX_Control(pDX, IDC_ETC_SDVE_FREQ_EDT    ,  m_edtFq);
	DDX_Control(pDX, IDC_ETC_SDVE_SF_EDT      ,  m_edtSF);
	DDX_Control(pDX, IDC_ETC_SDVE_DF_EDT      ,  m_edtDF);
	DDX_Control(pDX, IDC_ETC_SDVE_RT_EDT      ,  m_edtRT);
	DDX_Control(pDX, IDC_ETC_SDVE_SA_UNT      ,  m_untSA);
	DDX_Control(pDX, IDC_ETC_SDVE_TH_UNT      ,  m_untTh);
	DDX_Control(pDX, IDC_ETC_SDVE_MP_UNT      ,  m_untMp);
	DDX_Control(pDX, IDC_ETC_SDVE_LD_UNT      ,  m_untLD);
	DDX_Control(pDX, IDC_ETC_SDVE_MS_UNT      ,  m_untMS);
	DDX_Control(pDX, IDC_ETC_SDVE_ES_UNT      ,  m_untES);
	DDX_Control(pDX, IDC_ETC_SDVE_ED_UNT      ,  m_untED);
	DDX_Control(pDX, IDC_ETC_SDVE_KF_UNT      ,  m_untKF);
	DDX_Control(pDX, IDC_ETC_SDVE_BMP, m_ImgViewer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMSdveDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMSdveDlg)
	ON_BN_CLICKED(IDOK                        , OnCmdOK)
	ON_BN_CLICKED(IDC_CMD_APPLY               , OnCmdApply)
	ON_BN_CLICKED(IDC_ETC_SDVE_USER_RDO       , OnCmdInputMethodRdo)
	ON_BN_CLICKED(IDC_ETC_SDVE_IMDB_RDO       , OnCmdInputMethodRdo)
	ON_CBN_SELCHANGE(IDC_ETC_SDVE_COMPANY_CMB , OnSelChangeCompany    )
	ON_CBN_SELCHANGE(IDC_ETC_SDVE_PRONAME_CMB , OnSelChangeProductName)
	ON_CBN_SELCHANGE(IDC_ETC_SDVE_TYPENUM_CMB , OnSelChangeTypeNumber )
	ON_CBN_SELCHANGE(IDC_ETC_SDVE_MATLTYPE_CMB   , OnMatlTypeCmb)
	ON_BN_CLICKED(IDC_ETC_SDVE_CHK_BTN, OnClickChkBtn)

	ON_EN_CHANGE(IDC_ETC_SDVE_SA_EDT  , Update3MPropEdt)
	ON_EN_CHANGE(IDC_ETC_SDVE_TH_EDT  , Update3MPropEdt)
	ON_EN_CHANGE(IDC_ETC_SDVE_LD_EDT  , Update3MPropEdt)
	ON_EN_CHANGE(IDC_ETC_SDVE_FREQ_EDT, Update3MPropEdt)
	ON_EN_CHANGE(IDC_ETC_SDVE_RT_EDT  , Update3MPropEdt)
	ON_BN_CLICKED(IDC_ETC_SDVE_MS_CHK, OnCmdMountStiffChk)
	ON_BN_CLICKED(IDC_ETC_SDVE_KF_CHK, OnCmdKineticFricChk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCMSdveDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	InitCtrl();
	Data2Dlg();
	UpdateCtrls();
	if(m_data.nInputMethod==1)
	{
		UpdateCompany();
	}

	m_bOnInit=FALSE;

	return FALSE;
}

BOOL CCMSdveDlg::OnExecute()
{
	UpdateData(TRUE);

	if (!Dlg2Data())
		return FALSE;

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	if (m_bModify)
	{
		if (!pDoc->m_pDataCtrl->ModifySdve(m_key, m_key, m_data))
			return FALSE;
	}
	else
	{
		if (!pDoc->m_pDataCtrl->AddSdve(m_data))
			return FALSE;
	}
	return TRUE;
}

void CCMSdveDlg::OnCmdOK()
{
	if(!OnExecute())
	{
		return;
	}

	CDialogMove::OnOK();
}

void CCMSdveDlg::OnCmdApply()
{
	OnExecute();
}

void CCMSdveDlg::Data2Dlg()
{
	m_Name = m_data.strName;
	m_Desc = m_data.strDesc;
	m_nInputMethod = m_data.nInputMethod;
	
	CDlgUtil::CobxSetCurSelItemData(m_cmbMatlType, m_data.nMatlType);
	m_edtSA.SetEditUnit(m_data.dShearArea );
	m_edtTh.SetEditUnit(m_data.dThickness );
	m_edtMp.SetEditUnit(m_data.dMultiplier);
	m_cmbDir.SetCurSel(m_data.nDir-1);
	m_edtFq.SetEditUnit(m_data.dFreq         );
	m_edtSF.SetEditUnit(m_data.dScaleFactor  );
	m_edtDF.SetEditUnit(m_data.dDampingFactor);
	m_edtRT.SetEditUnit(m_data.dReferenceT   );
	//m_cmbCTC.SetCurSel(!m_data.bConsiderTC);
	m_edtLD.SetEditUnit(m_data.dLimitDeform );
	m_bMS = m_data.bMountStiff;
	m_edtMS.SetEditUnit(m_data.dMountStiff);
	m_edtES.SetEditUnit(m_data.dEffStiffness);
	m_edtED.SetEditUnit(m_data.dEquiDamping );
	m_bKF = m_data.bKineticFric;
	m_edtKF.SetEditUnit(m_data.dKineticFric);
 
	UpdateData(FALSE);
}

BOOL CCMSdveDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_data.Initialize();
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	m_data.nInputMethod= m_nInputMethod;
	if(m_data.nInputMethod==1)
	{
		m_cmbCompany.GetLBText(m_cmbCompany.GetCurSel(), m_data.strCompany);
		m_cmbProductName.GetLBText(m_cmbProductName.GetCurSel(), m_data.strProductName);
		m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), m_data.strTypeNumber);
	}  

	m_data.nMatlType      = CDlgUtil::CobxGetCurSelItemData(m_cmbMatlType, m_cmbMatlType.GetCurSel());
	m_data.dShearArea     = m_edtSA.GetEditValue();
	m_data.dThickness     = m_edtTh.GetEditValue();
	m_data.dMultiplier    = m_edtMp.GetEditValue();
	m_data.nDir           = CDlgUtil::CobxGetCurSelItemData(m_cmbDir, m_cmbDir.GetCurSel());
	m_data.dFreq          = m_edtFq.GetEditValue();
	m_data.dScaleFactor   = m_edtSF.GetEditValue();
	m_data.dDampingFactor = m_edtDF.GetEditValue();
	m_data.dReferenceT    = m_edtRT.GetEditValue();
	//m_data.bConsiderTC    = CDlgUtil::CobxGetCurSelItemData(m_cmbCTC, m_cmbCTC.GetCurSel());
	m_data.dLimitDeform   = m_edtLD.GetEditValue();
	m_data.bMountStiff    = m_bMS;
	m_data.dMountStiff    = m_edtMS.GetEditValue();
	m_data.bKineticFric   = m_bKF;
	m_data.dKineticFric   = m_edtKF.GetEditValue();	
	
	if(m_data.nMatlType==D_SDVE_ISD111 || m_data.nMatlType==D_SDVE_ISD111H)
	{
		double dVal[11];
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdve3MProp(m_data,dVal))
		{
			GSaveHistoryFormatNF(_LS(IDS_ETC_SDVE_CANNOT_CALC_3MPROP));
			return FALSE;
		}
	}

	return TRUE;
}

void CCMSdveDlg::OnMatlTypeCmb()
{
	UpdateCtrls();
}

void CCMSdveDlg::UpdateCtrls()
{
	UpdateData(TRUE);

	BOOL bDBDevice=m_nInputMethod==1;
	if(!bDBDevice)
	{
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly[i]))->SetReadOnly(FALSE);
		}
		for(int i=0; i<m_aCtrlID_DBValue_Disabled.GetSize(); i++)
		{
			GetDlgItem(m_aCtrlID_DBValue_Disabled[i])->EnableWindow(TRUE);
		}
	}

	GetDlgItem(IDC_ETC_SDVE_COMPANY_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVE_COMPANY_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVE_PRONAME_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVE_PRONAME_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVE_TYPENUM_STC)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVE_TYPENUM_CMB)->EnableWindow(m_nInputMethod==1);
	GetDlgItem(IDC_ETC_SDVE_KF_CHK     )->EnableWindow(m_nInputMethod==0); 
	((CEdit*)GetDlgItem(IDC_ETC_SDVE_KF_EDT))->SetReadOnly(m_nInputMethod!=0 || !m_bKF);

	int nMatlType = CDlgUtil::CobxGetCurSelItemData(m_cmbMatlType, m_cmbMatlType.GetCurSel());
	
	// PMS 5253-55 : GR400,ISD111,ISD111H일 때만 진동수 활성화
	BOOL bFreqUse = CDBDoc::GetDocPoint()->m_pAttrCtrl2->IsFreqUseSdveMatl(nMatlType);
	GetDlgItem(IDC_ETC_SDVE_FREQ_STC)->EnableWindow(bFreqUse);
	GetDlgItem(IDC_ETC_SDVE_FREQ_EDT)->EnableWindow(bFreqUse);
	GetDlgItem(IDC_ETC_SDVE_FREQ_STC2)->ShowWindow (bFreqUse);

	GetDlgItem(IDC_ETC_SDVE_MS_EDT)->EnableWindow(m_bMS);
	GetDlgItem(IDC_ETC_SDVE_MS_UNT)->EnableWindow(m_bMS);
	GetDlgItem(IDC_ETC_SDVE_KF_EDT)->EnableWindow(m_bKF);
	GetDlgItem(IDC_ETC_SDVE_KF_UNT)->EnableWindow(m_bKF);
	
	BOOL bSumitomo=FALSE;
	BOOL b3M=FALSE;
	BOOL bTRC=FALSE;
	if(nMatlType==D_SDVE_ISD111 || nMatlType==D_SDVE_ISD111H)
	{
		b3M=TRUE;
		Update3MPropEdt();
		GetDlgItem(IDC_ETC_SDVE_HP_GRP)->SetWindowText(_LS(IDS_ETC_SDVE_HP_3M));
		CString strCaption;
		double dLow=0, dHigh=40;
		strCaption.Format(_LS(IDS_ETC_SDVE_RT_RANGE_f_f),dLow,dHigh);
		GetDlgItem(IDC_ETC_SDVE_RT_STC2)->SetWindowText(strCaption);
		dLow=0.1; 
		dHigh=3.0;
		strCaption.Format(_LS(IDS_ETC_SDVE_FREQ_RANGE_f_f),dLow,dHigh);
		GetDlgItem(IDC_ETC_SDVE_FREQ_STC2)->SetWindowText(strCaption);
	}
	else if(nMatlType==D_SDVE_CST)
	{
		bSumitomo=TRUE;
		GetDlgItem(IDC_ETC_SDVE_CHK_BTN)->SetWindowText(_LS(IDS_ETC_SDVE_CHECK_CST_PROP));
		GetDlgItem(IDC_ETC_SDVE_HP_GRP)->SetWindowText(_LS(IDS_ETC_SDVE_HP_CST));
		CString strCaption;
		double dLow=-20, dHigh=60;
		strCaption.Format(_LS(IDS_ETC_SDVE_RT_RANGE_f_f),dLow,dHigh);
		GetDlgItem(IDC_ETC_SDVE_RT_STC2)->SetWindowText(strCaption);
		GetDlgItem(IDC_ETC_SDVE_FREQ_STC2)->SetWindowText(_T(""));
	}
	else if(nMatlType==D_SDVE_TRC)
	{
		bTRC=TRUE;
		CString strCaption;
		double dLow=-20, dHigh=60;
		strCaption.Format(_LS(IDS_ETC_SDVE_RT_RANGE_f_f),dLow,dHigh);
		GetDlgItem(IDC_ETC_SDVE_RT_STC2)->SetWindowText(strCaption);
		GetDlgItem(IDC_ETC_SDVE_FREQ_STC2)->SetWindowText(_T(""));
	}
	else
	{
		bSumitomo=TRUE;
		GetDlgItem(IDC_ETC_SDVE_CHK_BTN)->SetWindowText(_LS(IDS_ETC_SDVE_CHECK_SUMITOMO_PROP));
		GetDlgItem(IDC_ETC_SDVE_HP_GRP)->SetWindowText(_LS(IDS_ETC_SDVE_HP_SM));
		GetDlgItem(IDC_ETC_SDVE_RT_STC2)->SetWindowText(_T(""));
		GetDlgItem(IDC_ETC_SDVE_FREQ_STC2)->SetWindowText(_T(""));
	}

	CArray<UINT,UINT> aCtrls_Sumitomo, aCtrls_3M;
	aCtrls_Sumitomo.Add(IDC_ETC_SDVE_CHK_BTN);
	aCtrls_3M.Add(IDC_ETC_SDVE_ES_STC);
	aCtrls_3M.Add(IDC_ETC_SDVE_ES_EDT);
	aCtrls_3M.Add(IDC_ETC_SDVE_ES_UNT);
	aCtrls_3M.Add(IDC_ETC_SDVE_ED_STC);
	aCtrls_3M.Add(IDC_ETC_SDVE_ED_EDT);
	aCtrls_3M.Add(IDC_ETC_SDVE_ED_UNT);
	aCtrls_3M.Add(IDC_ETC_SDVE_KF_CHK);
	aCtrls_3M.Add(IDC_ETC_SDVE_KF_EDT);
	aCtrls_3M.Add(IDC_ETC_SDVE_KF_UNT);

	GetDlgItem(IDC_ETC_SDVE_HP_GRP)->ShowWindow(bSumitomo || b3M);
	CDlgUtil::CtrlShowHide(this, aCtrls_Sumitomo, bSumitomo);
	CDlgUtil::CtrlShowHide(this, aCtrls_3M, b3M);

	GetDlgItem(IDC_ETC_SDVE_DF_STC)->EnableWindow(bSumitomo || b3M);
	GetDlgItem(IDC_ETC_SDVE_DF_EDT)->EnableWindow(bSumitomo || b3M);
	GetDlgItem(IDC_ETC_SDVE_SF_STC)->EnableWindow(bSumitomo || b3M);
	GetDlgItem(IDC_ETC_SDVE_SF_EDT)->EnableWindow(bSumitomo || b3M);

	CRect rRef,rToMove;
	if(bSumitomo || bTRC)
		GetDlgItem(IDC_ETC_SDVE_CHK_BTN)->GetWindowRect(rRef);
	else
		GetDlgItem(IDC_ETC_SDVE_KF_EDT)->GetWindowRect(rRef);
	
	//Hinge Properties
	GetDlgItem(IDC_ETC_SDVE_HP_GRP)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(12);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_ETC_SDVE_HP_GRP)->MoveWindow(rToMove);
	
	//Input Properties
	GetDlgItem(IDC_ETC_SDVE_INPUTPARAM_GRP)->GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom + globalUtils.ScaleByDPI(20);
	ScreenToClient(rToMove);
	GetDlgItem(IDC_ETC_SDVE_INPUTPARAM_GRP)->MoveWindow(rToMove);

	//OK
	GetDlgItem(IDOK)->GetWindowRect(rToMove);
	int nDistY = rRef.bottom - rToMove.top + globalUtils.ScaleByDPI(36);
	CArray<UINT,UINT> aCtrls_OKCancelApply;
	aCtrls_OKCancelApply.Add(IDOK);
	aCtrls_OKCancelApply.Add(IDCANCEL);
	aCtrls_OKCancelApply.Add(IDC_CMD_APPLY);
	CDlgUtil::CtrlMoveDistY(this, aCtrls_OKCancelApply, nDistY);

	// resize self size  
	GetDlgItem(IDOK)->GetWindowRect(rRef);
	GetWindowRect(rToMove);
	rToMove.bottom = rRef.bottom+ globalUtils.ScaleByDPI(8);
	MoveWindow(rToMove);

	SetBmpImage();

	if(bDBDevice)
	{
		for(int i=0; i<m_aCtrlID_DBValue_ReadOnly.GetSize(); i++)
		{
			((CEdit*)GetDlgItem(m_aCtrlID_DBValue_ReadOnly[i]))->SetReadOnly(TRUE);
		}
		for(int i=0; i<m_aCtrlID_DBValue_Disabled.GetSize(); i++)
		{
			GetDlgItem(m_aCtrlID_DBValue_Disabled[i])->EnableWindow(FALSE);
		}
	}
}

void CCMSdveDlg::SetBmpImage()
{
	int nMatlType = CDlgUtil::CobxGetCurSelItemData(m_cmbMatlType, m_cmbMatlType.GetCurSel());
	BOOL bSumitomo = TRUE;
	if(nMatlType==D_SDVE_ISD111 || nMatlType==D_SDVE_ISD111H)
	{
		bSumitomo = FALSE;
	}

	CString csImgPath = _T("");
	switch(nMatlType)
	{
	case D_SDVE_GR100  :
	case D_SDVE_GR300  :
	case D_SDVE_SR05   :
	case D_SDVE_GR400  :
	case D_SDVE_CST    :
		csImgPath = _T("SVG\\illustration\\Dialog\\Sdve_dlg.svg");
		break;
	case D_SDVE_ISD111 :
	case D_SDVE_ISD111H:
		csImgPath = _T("SVG\\illustration\\Dialog\\Sdve_dlg2.svg");
		break;
	case D_SDVE_TRC    :
		csImgPath = _T("SVG\\illustration\\Dialog\\Sdve_dlg3.svg");
		break;
	default:
		ASSERT(0);
		break;
	}
	m_ImgViewer.SetImage(csImgPath);
}

void CCMSdveDlg::InitCtrl()
{
	CRect rRef;
	CRect rToMove;

	GetDlgItem(IDC_ETC_SDVE_ES_STC)->GetWindowRect(rRef);
	GetDlgItem(IDC_ETC_SDVE_CHK_BTN)->GetWindowRect(rToMove);
	int nDistX = rRef.left - rToMove.left;
	int nDistY = rRef.top - rToMove.top;
	CArray<UINT,UINT> aCtrl;
	aCtrl.Add(IDC_ETC_SDVE_CHK_BTN);
	CDlgUtil::CtrlMoveDistXY(this, aCtrl, nDistX, nDistY);

	m_edtSA.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dShearArea);
	m_untSA.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dShearArea);
	m_edtTh.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dThickness);
	m_untTh.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dThickness);
	m_edtMp.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dMultiplier);
	m_untMp.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dMultiplier);
	m_edtLD.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dLimitDeform);
	m_untLD.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dLimitDeform);
	m_edtMS.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dMountStiff);
	m_untMS.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dMountStiff);
	m_edtES.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dEffStiffness);
	m_untES.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dEffStiffness);
	m_edtED.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dEquiDamping);
	m_untED.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dEquiDamping);
	m_edtKF.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dKineticFric);
	m_untKF.SetUnitType(CUnitCtrl::m_SDVE_UNIT.dKineticFric);

	CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_GR100  ), D_SDVE_GR100  );
	CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_GR300  ), D_SDVE_GR300  );
	CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_SR05   ), D_SDVE_SR05   );
	CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_GR400  ), D_SDVE_GR400  );

	BOOL b3MShow = FALSE;
	if(CTestEnvMgr::IsTestEnvST(_T("SeisCtrlDeviceLater"), _T("yes")))
		b3MShow=TRUE;
#if defined(_JP)
	b3MShow=TRUE;
#endif
	// 161202 : 3M社 장치는 일문에서만 활성
	if(b3MShow)
	{
		CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_ISD111 ), D_SDVE_ISD111 );
		CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_ISD111H), D_SDVE_ISD111H);
	}
	CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_CST    ), D_SDVE_CST);
	CDlgUtil::CobxAddItem(m_cmbMatlType, _LS(IDS_ETC_SDVE_MTYPE_TRC    ), D_SDVE_TRC);

	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_WG_CMD_DX), D_SDVE_DX);
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_WG_CMD_DY), D_SDVE_DY);
	CDlgUtil::CobxAddItem(m_cmbDir, _LS(IDS_WG_CMD_DZ), D_SDVE_DZ);

//   CDlgUtil::CobxAddItem(m_cmbCTC, _LS(IDS_CMD_NO) , FALSE);
//   CDlgUtil::CobxAddItem(m_cmbCTC, _LS(IDS_CMD_YES), TRUE);

	m_edtES.SetReadOnly(TRUE);
	m_edtED.SetReadOnly(TRUE);
}

void CCMSdveDlg::OnClickChkBtn()
{
	UpdateData(TRUE);

	CCMSdvePropDlg dlg;
	dlg.SetInitPos(D_INIT_POS_RT);
	int nMatlType = CDlgUtil::CobxGetCurSelItemData(m_cmbMatlType, m_cmbMatlType.GetCurSel());
	dlg.SetMatlType	(nMatlType);
	if(m_nInputMethod==0)
	{
		double dValue[20];
		CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdveSumitomoProp(nMatlType,dValue);
		dlg.SetValue(dValue);
	}
	else
	{
		dlg.SetValue(m_data.dUserSumitomoProp);
	}

	dlg.DoModal();
}

void CCMSdveDlg::Update3MPropEdt()
{
	int nMatlType = CDlgUtil::CobxGetCurSelItemData(m_cmbMatlType, m_cmbMatlType.GetCurSel());
	if(nMatlType!=D_SDVE_ISD111 && nMatlType!=D_SDVE_ISD111H)
		return;

	T_SDVE_D SdveD_temp;
	SdveD_temp.Initialize();
	SdveD_temp.nMatlType      = nMatlType;
	SdveD_temp.dShearArea     = m_edtSA.GetEditValue();
	SdveD_temp.dThickness     = m_edtTh.GetEditValue();
	SdveD_temp.nDir           = CDlgUtil::CobxGetCurSelItemData(m_cmbDir, m_cmbDir.GetCurSel());
	SdveD_temp.dFreq          = m_edtFq.GetEditValue();
	SdveD_temp.dScaleFactor   = m_edtSF.GetEditValue();
	SdveD_temp.dDampingFactor = m_edtDF.GetEditValue();
	SdveD_temp.dReferenceT    = m_edtRT.GetEditValue();
	SdveD_temp.dLimitDeform   = m_edtLD.GetEditValue();
	SdveD_temp.bMountStiff    = m_bMS;
	SdveD_temp.dMountStiff    = m_edtMS.GetEditValue();
	SdveD_temp.bKineticFric   = m_bKF;
	SdveD_temp.dKineticFric   = m_edtKF.GetEditValue();	
	
	double dVal[11];
	if(CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetSdve3MProp(SdveD_temp,dVal))
	{
		m_edtES.SetEditUnit(dVal[7]);
		m_edtED.SetEditUnit(dVal[8]);
	}
	else
	{
		m_edtES.SetEditUnit(0);
		m_edtED.SetEditUnit(0);
	}
}

void CCMSdveDlg::OnCmdInputMethodRdo()
{
	UpdateData(TRUE);
	if(m_nInputMethod==1) UpdateCompany();
	UpdateCtrls();
}

void CCMSdveDlg::OnSelChangeCompany    () 
{
	UpdateData(TRUE);
	UpdateProductName();
}
void CCMSdveDlg::OnSelChangeProductName() 
{
	UpdateData(TRUE);
	UpdateTypeNumber();
}
void CCMSdveDlg::OnSelChangeTypeNumber () 
{
	UpdateData(TRUE);
	int nIdx = m_cmbTypeNumber.GetCurSel();
	if(nIdx<0) { ASSERT(0); return; }
	CString strTypeNum;
	m_cmbTypeNumber.GetLBText(m_cmbTypeNumber.GetCurSel(), strTypeNum);
	m_pIGenSDCDB->GetSdveFromDB(strTypeNum,m_data);
	m_data.strName = m_Name;
	m_data.strDesc = m_Desc;
	Data2Dlg();
	UpdateCtrls();
}
void CCMSdveDlg::UpdateCompany()          
{
	CStringArray aDBCompany;
	m_pIGenSDCDB->GetCompanyList(IGENSDC_DMP_VISCOEL,aDBCompany);
	m_cmbCompany.ResetContent();
	for(int i=0; i<aDBCompany.GetSize(); i++)
	{
		CDlgUtil::CobxAddItem(m_cmbCompany, aDBCompany[i], i);
	}

	int nCmbIdx = m_cmbCompany.FindString(0, m_data.strCompany);
	if(nCmbIdx==-1) m_cmbCompany.SetCurSel(0);
	else            m_cmbCompany.SetCurSel(nCmbIdx);
	UpdateProductName();
}
void CCMSdveDlg::UpdateProductName()      
{
	int nIdxCompany= m_cmbCompany.GetCurSel();
	if(nIdxCompany<0) { ASSERT(0); return; } // 회사가 없다니

	CString strCompany;
	m_cmbCompany.GetLBText(nIdxCompany, strCompany);

	CStringArray aDBPrdName;
	m_pIGenSDCDB->GetPrdNameList(IGENSDC_DMP_VISCOEL,strCompany,aDBPrdName);
	m_cmbProductName.ResetContent();
	for(int i=0; i<aDBPrdName.GetSize(); i++)
	{
		CDlgUtil::CobxAddItem(m_cmbProductName, aDBPrdName[i], i);
	}

	int nCmbIdx = m_cmbProductName.FindString(0, m_data.strProductName);
	if(nCmbIdx==-1) m_cmbProductName.SetCurSel(0);
	else            m_cmbProductName.SetCurSel(nCmbIdx);
	UpdateTypeNumber();
}
void CCMSdveDlg::UpdateTypeNumber()      
{
	int nIdxCompany= m_cmbCompany.GetCurSel();
	int nIdxPrdName= m_cmbProductName.GetCurSel();
	if(nIdxCompany<0 || nIdxPrdName<0) { ASSERT(0); return; } 

	CString strCompany, strPrdName;
	m_cmbCompany.GetLBText(nIdxCompany, strCompany);
	m_cmbProductName.GetLBText(nIdxPrdName, strPrdName);

	CStringArray aDBTypeNum;
	m_pIGenSDCDB->GetTypeNumListAndMakeData(IGENSDC_DMP_VISCOEL,strCompany,strPrdName,aDBTypeNum);
	m_cmbTypeNumber.ResetContent();
	for(int i=0; i<aDBTypeNum.GetSize(); i++)
	{
		CDlgUtil::CobxAddItem(m_cmbTypeNumber, aDBTypeNum[i], i);
	}
	int nCmbIdx = m_cmbTypeNumber.FindString(0, m_data.strTypeNumber);
	if(nCmbIdx==-1) m_cmbTypeNumber.SetCurSel(0);
	else            m_cmbTypeNumber.SetCurSel(nCmbIdx);
	if(!m_bOnInit) OnSelChangeTypeNumber();
}

void CCMSdveDlg::OnCmdMountStiffChk()
{
	UpdateCtrls();
}

void CCMSdveDlg::OnCmdKineticFricChk()
{
	UpdateCtrls();
}
