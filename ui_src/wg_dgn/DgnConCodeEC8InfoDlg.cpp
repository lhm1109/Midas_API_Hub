// DgnConCodeEC8InfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnConCodeEC8InfoDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"

#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_base\wg_base_LocaleLib.h"
#include "..\wg_base\DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeEC8InfoDlg dialog


CDgnConCodeEC8InfoDlg::CDgnConCodeEC8InfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnConCodeEC8InfoDlg::IDD, pParent)
{
	m_pDconData = NULL;

	//{{AFX_DATA_INIT(CDgnConCodeEC8InfoDlg)
	
	//}}AFX_DATA_INIT
	m_aBehaviorFactRdo.Add(IDC_DGN_CALC_PROGRAM_RDO);
	m_aBehaviorFactRdo.Add(IDC_DGN_USER_INPUT_RDO);

	m_aBehaviorT1Rdo.Add(IDC_DGN_T1_CALC_PROGRAM_RDO);
	m_aBehaviorT1Rdo.Add(IDC_DGN_T1_USER_INPUT_RDO);

	m_aT1User.Add(IDC_DGN_T1X_STATIC);
	m_aT1User.Add(IDC_DGN_T1Y_STATIC);
	m_aT1User.Add(IDC_DGN_USER_INPUT_T1X_EDT);
	m_aT1User.Add(IDC_DGN_USER_INPUT_T1Y_EDT);
}


void CDgnConCodeEC8InfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnConCodeEC8InfoDlg)
	DDX_Control(pDX, IDC_DGN_SYSTEM_COMBO, m_comboFrameType);
	DDX_Control(pDX, IDC_DGN_AUAL_EDIT, m_editAua1);
	DDX_Control(pDX, IDC_DGN_FUNCTION_COMBO, m_comboFunction);
	DDX_Control(pDX, IDC_DGN_SF_EDIT, m_editSf);
	DDX_Control(pDX, IDC_DGN_TB_EDIT, m_editTb);
	DDX_Control(pDX, IDC_DGN_TC_EDIT, m_editTc);
	DDX_Control(pDX, IDC_DGN_TD_EDIT, m_editTd);
	DDX_Control(pDX, IDC_DGN_AGR_EDIT, m_editAgR);
	DDX_Control(pDX, IDC_DGN_I_EDIT, m_editI);
	DDX_Control(pDX, IDC_DGN_VDR_EDIT, m_editVdr);
	DDX_Control(pDX, IDC_DGN_USER_INPUT_Q_EDT, m_editUserInputQ);
	DDX_Control(pDX, IDC_DGN_USER_INPUT_QO_EDT, m_editUserInputQO);
	DDX_Control(pDX, IDC_DGN_USER_INPUT_T1X_EDT, m_editT1_X);
	DDX_Control(pDX, IDC_DGN_USER_INPUT_T1Y_EDT, m_editT1_Y);

	//}}AFX_DATA_MAP
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnConCodeEC8InfoDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnConCodeEC8InfoDlg)
	ON_BN_CLICKED(IDC_DGN_CALC_PROGRAM_RDO, OnBehaviorFactorRdo)
	ON_BN_CLICKED(IDC_DGN_USER_INPUT_RDO, OnBehaviorFactorRdo)
	ON_BN_CLICKED(IDC_DGN_T1_CALC_PROGRAM_RDO, OnBehaviorT1Rdo)
	ON_BN_CLICKED(IDC_DGN_T1_USER_INPUT_RDO, OnBehaviorT1Rdo)
	ON_CBN_SELCHANGE(IDC_DGN_FUNCTION_COMBO, OnSelchangeFumctionCombo)
	ON_CBN_SELCHANGE(IDC_DGN_SYSTEM_COMBO, OnSelchangeFrameTypeCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConCodeEC8InfoDlg message handlers

BOOL CDgnConCodeEC8InfoDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_pDoc = CDBDoc::GetDocPoint();
	InitUnit();

	InitFrameCombo();
	InitFunctionCombo();

	Data2Dlg();
	OnBehaviorFactorRdo();
	OnBehaviorT1Rdo();
	return TRUE;
}

void CDgnConCodeEC8InfoDlg::InitUnit()
{
	m_editAua1.SetUnitType(D_UNITSYS_NONE);
	m_editSf.SetUnitType(D_UNITSYS_NONE);
	m_editTb.SetUnitType(D_UNITSYS_NONE);
	m_editTc.SetUnitType(D_UNITSYS_NONE);
	m_editTd.SetUnitType(D_UNITSYS_NONE);
	m_editAgR.SetUnitType(D_UNITSYS_NONE);
	m_editI.SetUnitType(D_UNITSYS_NONE);
	m_editVdr.SetUnitType(D_UNITSYS_NONE);
	m_editUserInputQ.SetUnitType(D_UNITSYS_NONE);
	m_editUserInputQO.SetUnitType(D_UNITSYS_NONE);
	m_editT1_X.SetUnitType(D_UNITSYS_NONE);
	m_editT1_Y.SetUnitType(D_UNITSYS_NONE);
}

void CDgnConCodeEC8InfoDlg::InitFrameCombo()
{
	m_comboFrameType.ResetContent();

	CString str;
	str = _LS(IDS_DGN_STR_EC8_DATA_FRAME);
	m_comboFrameType.AddString(str);
	str = _LS(IDS_DGN_STR_EC8_DATA_DUAL);
	m_comboFrameType.AddString(str);
	str = _LS(IDS_DGN_STR_EC8_DATA_COUP);
	m_comboFrameType.AddString(str);
	str = _LS(IDS_DGN_STR_EC8_DATA_UNCOUP);
	m_comboFrameType.AddString(str);
	// str = _LS(IDS_DGN_STR_EC8_DATA_TORSION);
	// m_comboFrameType.AddString(str);
	// str = _LS(IDS_DGN_STR_EC8_DATA_INVERT);
	// m_comboFrameType.AddString(str);

	m_comboFrameType.SetCurSel(0);
}

void CDgnConCodeEC8InfoDlg::InitFunctionCombo()
{
	m_comboFunction.ResetContent();
	m_aOKSpfcK.RemoveAll();

	m_comboFunction.AddString(_LSX(None));
	m_aOKSpfcK.Add(0);

	CArray<T_SPFC_K, T_SPFC_K> aSpfcK;
	m_pDoc->m_pAttrCtrl->GetSpfcKeyList(aSpfcK);
	int nOKSize = 0;
	int nSize = aSpfcK.GetSize();
	for(int i=0 ; i<nSize ; ++i)
	{
		T_SPFC_K SpfcK = aSpfcK[i];
		T_SPFC_D SpfcD; SpfcD.Initialize();
		if(!m_pDoc->m_pAttrCtrl->GetSpfc(SpfcK, SpfcD)) continue;

		if(SpfcD.nSpecCodeType == D_SPFC_CODE_EURO2004 || SpfcD.nSpecCodeType == D_SPFC_CODE_USER)
		{
			++nOKSize;
			m_comboFunction.AddString(SpfcD.SpecFuncName);
			m_aOKSpfcK.Add(SpfcK);
		}
	}

	if(nOKSize == 0)  m_comboFunction.EnableWindow(FALSE);
	m_comboFunction.SetCurSel(0);
	OnSelchangeFumctionCombo();
}

void CDgnConCodeEC8InfoDlg::SetDconData(T_DCON_D* pDconData)
{
	m_pDconData = pDconData;
}

void CDgnConCodeEC8InfoDlg::Data2Dlg()
{
	m_comboFrameType.SetCurSel(m_pDconData->iFrameType);
	m_editAua1.SetEditUnit(m_pDconData->dAua1);
	m_editSf.SetEditUnit(m_pDconData->dSoilFactor);
	m_editTb.SetEditUnit(m_pDconData->dT[0]);
	m_editTc.SetEditUnit(m_pDconData->dT[1]);
	m_editTd.SetEditUnit(m_pDconData->dT[2]);
	m_editAgR.SetEditUnit(m_pDconData->dAgR);
	m_editI.SetEditUnit(m_pDconData->dI);
	m_editVdr.SetEditUnit(m_pDconData->dDampingRat);
	m_editUserInputQ.SetEditUnit(m_pDconData->dBehaviorFactor_q);
	m_editUserInputQO.SetEditUnit(m_pDconData->dBehaviorFactor_qo);
	m_editT1_X.SetEditUnit(m_pDconData->dT1_x);
	m_editT1_Y.SetEditUnit(m_pDconData->dT1_y);

	int nSpfc = m_aOKSpfcK.GetSize();
	for(int i=0; i<nSpfc; i++)
	{
		if(m_pDconData->SplcK == m_aOKSpfcK[i])
		{
			m_comboFunction.SetCurSel(i);
			OnSelchangeFumctionCombo();
			break;
		}
	}

	int nCheck = m_pDconData->bBehaviorFact ? 0 : 1;
	CDlgUtil::CtrlRadioSetCheck(this, m_aBehaviorFactRdo, nCheck);

	int nCheckT1 = m_pDconData->bT1User ? 1 : 0;
	CDlgUtil::CtrlRadioSetCheck(this, m_aBehaviorT1Rdo, nCheckT1);
}

BOOL CDgnConCodeEC8InfoDlg::Dlg2Data()
{
	int nCheck = 0, nCheckT1 = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aBehaviorFactRdo, nCheck);
	CDlgUtil::CtrlRadioGetCheck(this, m_aBehaviorT1Rdo, nCheckT1);

	if(nCheck && m_editUserInputQO.GetEditValue() <= 0)
	{
		ASSERT(0); //QO값은 0보다 커야함.
		return FALSE;
	}
	m_pDconData->iFrameType = m_comboFrameType.GetCurSel();
	m_pDconData->dAua1 = m_editAua1.GetEditValue();
	m_pDconData->dSoilFactor = m_editSf.GetEditValue();
	m_pDconData->dT[0] = m_editTb.GetEditValue();
	m_pDconData->dT[1] = m_editTc.GetEditValue();
	m_pDconData->dT[2] = m_editTd.GetEditValue();
	m_pDconData->dAgR = m_editAgR.GetEditValue();
	m_pDconData->dI = m_editI.GetEditValue();
	m_pDconData->dDampingRat = m_editVdr.GetEditValue();
	m_pDconData->bBehaviorFact   = nCheck == 0 ? TRUE : FALSE;
	m_pDconData->dBehaviorFactor_q = m_editUserInputQ.GetEditValue();
	m_pDconData->dBehaviorFactor_qo = m_editUserInputQO.GetEditValue();

	m_pDconData->bT1User   = nCheckT1 == 0 ? FALSE : TRUE;
	m_pDconData->dT1_x = m_editT1_X.GetEditValue();
	m_pDconData->dT1_y = m_editT1_Y.GetEditValue();

	int nCurSel = m_comboFunction.GetCurSel();
	T_SPFC_K SpfcK = m_aOKSpfcK[nCurSel];
	m_pDconData->SplcK = SpfcK;

	return TRUE;
}

void CDgnConCodeEC8InfoDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnConCodeEC8InfoDlg::OnOK() 
{
	if(!Dlg2Data()) return ;
	CDialogMove::OnOK();
}
	
void CDgnConCodeEC8InfoDlg::OnSelchangeFumctionCombo() 
{
	int nCurSel = m_comboFunction.GetCurSel();
	T_SPFC_K SpfcK = m_aOKSpfcK[nCurSel];
	T_SPFC_D SpfcD; SpfcD.Initialize();

	BOOL bEnable = TRUE;
	BOOL bEnableTc = TRUE;
	if(m_pDoc->m_pAttrCtrl->GetSpfc(SpfcK, SpfcD))
	{
		if(SpfcD.nSpecCodeType != D_SPFC_CODE_USER)
		{
			m_editSf.SetEditUnit(SpfcD.CodeParam.EURO2004.dSoilFactor);
			m_editTb.SetEditUnit(SpfcD.CodeParam.EURO2004.dTb);
			m_editTc.SetEditUnit(SpfcD.CodeParam.EURO2004.dTc);
			m_editTd.SetEditUnit(SpfcD.CodeParam.EURO2004.dTd);
			m_editAgR.SetEditUnit(SpfcD.CodeParam.EURO2004.dAgr);
			m_editI.SetEditUnit(SpfcD.CodeParam.EURO2004.dI);
	    m_editVdr.SetEditUnit(SpfcD.CodeParam.EURO2004.dXi);
		}

		bEnable = FALSE;
		bEnableTc = (SpfcD.nSpecCodeType == D_SPFC_CODE_USER) ? TRUE : FALSE;
	}
	
	m_editSf.EnableWindow(bEnable);
	m_editTb.EnableWindow(bEnable);
	m_editTc.EnableWindow(bEnableTc);
	m_editTd.EnableWindow(bEnable);
	m_editAgR.EnableWindow(bEnable);
	m_editI.EnableWindow(bEnable);
	m_editVdr.EnableWindow(bEnable);
	

}
// PMS:xxxx-DANAKIM-20120620 EC8 5.2.2.2 (5) 기준반영
void CDgnConCodeEC8InfoDlg::OnSelchangeFrameTypeCombo()
{	
	int nCurSel = m_comboFrameType.GetCurSel();
	double daua = 1.0;
	
	if      (nCurSel == 0)  daua = 1.1;
	else if (nCurSel == 1)  daua = 1.2;
 	else if (nCurSel == 2)  daua = 1.2;
 	else if (nCurSel == 3)  daua = 1.0;
	
	m_editAua1.SetEditUnit(daua);
}

void CDgnConCodeEC8InfoDlg::OnBehaviorFactorRdo()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aBehaviorFactRdo, nCheck);
	BOOL bEnable = FALSE;
	if(nCheck == 0)   bEnable = TRUE;
 
	// Seungjun-20091228 보에도 사용되므로 항상 입력받을 수 있도록 수정.
	//GetDlgItem(IDC_DGN_AUAL_STC)->EnableWindow(bEnable);
	//GetDlgItem(IDC_DGN_AUAL_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_DGN_USER_INPUT_Q_EDT)->EnableWindow(!bEnable);
	GetDlgItem(IDC_DGN_USER_INPUT_QO_EDT)->EnableWindow(!bEnable);

	//Calculate by Program일때 disable
	GetDlgItem(IDC_DGN_AUAL_EDIT)->EnableWindow(bEnable);

/*	GetDlgItem(IDC_DGN_AUAL_EDIT)->EnableWindow(bEnable);*/


}

void CDgnConCodeEC8InfoDlg::OnBehaviorT1Rdo()
{
	int nCheck = 0;
	CDlgUtil::CtrlRadioGetCheck(this, m_aBehaviorT1Rdo, nCheck);
	CDlgUtil::CtrlEnableDisable(this, m_aT1User, nCheck == 1);
}

