// ElemPlateReduFactorDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemPlateReduFactorDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_main\wg_mainRes2.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemPlateReduFactorDlg dialog


CElemPlateReduFactorDlg::CElemPlateReduFactorDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemPlateReduFactorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemPlateReduFactorDlg)
	m_nAddOrDel = 0;
	m_bUseUserDir = FALSE;
	m_nDirType = 0;
	m_nLocalAxis = 0;
	//}}AFX_DATA_INIT

	m_aLocalAxisCtrl.RemoveAll();
	m_aLocalAxisTypeCtrl.RemoveAll();
	m_aPointCtrl.RemoveAll();
	m_aVectorCtrl.RemoveAll();

	m_aLocalAxisCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_X_RDO);
	m_aLocalAxisCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_Y_RDO);

	m_aLocalAxisTypeCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_COORD_RDO);
	m_aLocalAxisTypeCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_REF_RDO);
	
	m_aPointCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_COORD_DIR_CMB);
	m_aPointCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_ORG_POINT_STC);
	m_aPointCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_POINT_EDT);
	m_aPointCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_POINT_STC);
	
	m_aVectorCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_VECTOR_EDT);
	m_aVectorCtrl.Add(IDC_TM_REDU_LOCAL_AXIS_VECTOR_STC);
}

void CElemPlateReduFactorDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemPlateReduFactorDlg)
	DDX_Control(pDX, IDC_TM_GROUP_COMBO, m_wndGroupCombo);
	DDX_Radio(pDX, IDC_TM_ADD_REP_RADIO, m_nAddOrDel);

	DDX_Control(pDX, IDC_TM_REDU_AXIAL_X_EDT,				m_wndAxialX);
	DDX_Control(pDX, IDC_TM_REDU_AXIAL_Y_EDT,				m_wndAxialY);
	DDX_Control(pDX, IDC_TM_REDU_SHEAR_EDT,					m_wndShear);
	DDX_Control(pDX, IDC_TM_REDU_OUT_BENDING_X_EDT, m_wndOutBendingX);
	DDX_Control(pDX, IDC_TM_REDU_OUT_BENDING_Y_EDT, m_wndOutBendingY);
	DDX_Control(pDX, IDC_TM_REDU_OUT_TORSION_EDT,		m_wndOutTorsion);
	DDX_Control(pDX, IDC_TM_REDU_OUT_SHEAR_X_EDT,		m_wndOutShearX);
	DDX_Control(pDX, IDC_TM_REDU_OUT_SHEAR_Y_EDT,		m_wndOutShearY);
	DDX_Check(pDX, IDC_TM_REDU_USER_DIR_CHK, m_bUseUserDir);
	DDX_Control(pDX, IDC_TM_REDU_LOCAL_AXIS_COORD_DIR_CMB, m_cmbCoordDir);
	DDX_Control(pDX, IDC_TM_REDU_LOCAL_AXIS_POINT_EDT,		m_edtPoint);
	DDX_Control(pDX, IDC_TM_REDU_LOCAL_AXIS_POINT_STC,		m_unitPoint);
	DDX_Control(pDX, IDC_TM_REDU_LOCAL_AXIS_VECTOR_EDT,		m_edtVector);
	DDX_Control(pDX, IDC_TM_REDU_LOCAL_AXIS_VECTOR_STC,		m_unitVector);
	DDX_Radio(pDX, IDC_TM_REDU_LOCAL_AXIS_X_RDO,					m_nLocalAxis);
	DDX_Radio(pDX, IDC_TM_REDU_LOCAL_AXIS_COORD_RDO,			m_nDirType);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemPlateReduFactorDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemPlateReduFactorDlg)
	ON_BN_CLICKED(IDC_TM_ADD_REP_RADIO, OnTmOption)
	ON_BN_CLICKED(IDC_TM_DELETE_RADIO, OnTmOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_GROUP_BUTTON, OnTmDefineGroupButton)
	ON_BN_CLICKED(IDC_TM_REDU_USER_DIR_CHK, OnTmUserDirButton)
	ON_BN_CLICKED(IDC_TM_REDU_LOCAL_AXIS_X_RDO, OnClickLocalAxisXRdo)
	ON_BN_CLICKED(IDC_TM_REDU_LOCAL_AXIS_Y_RDO, OnClickLocalAxisYRdo)
	ON_BN_CLICKED(IDC_TM_REDU_LOCAL_AXIS_COORD_RDO, OnClickLocalAxisType1Rdo)
	ON_BN_CLICKED(IDC_TM_REDU_LOCAL_AXIS_REF_RDO, OnClickLocalAxisType2Rdo)
	ON_CBN_SELCHANGE(IDC_TM_REDU_LOCAL_AXIS_COORD_DIR_CMB, OnSelchangeCoordDirCmb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CElemPlateReduFactorDlg::UpdateUserDirCtrls(BOOL bEnable)
{
	CArray<UINT,UINT> arIDs;
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_GRB);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_X_RDO);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_Y_RDO);
	arIDs.Add(IDC_TM_REDU_LOCAL_DIR_GRB);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_COORD_RDO);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_COORD_DIR_CMB);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_ORG_POINT_STC);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_POINT_EDT);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_POINT_STC);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_REF_RDO);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_VECTOR_EDT);
	arIDs.Add(IDC_TM_REDU_LOCAL_AXIS_VECTOR_STC);
	
	for(int i=0; i<arIDs.GetSize(); i++)
		GetDlgItem(arIDs[i])->EnableWindow(bEnable);

	if(bEnable)
	{
		CtrlEnableDisable();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CElemPlateReduFactorDlg message handlers

BOOL CElemPlateReduFactorDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();

	T_PSSF_D data;
	data.Initialize();

	CString strFactor=_T("");

	strFactor.Format(_T("%g"), data.dAxialX);			m_wndAxialX.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dAxialY);			m_wndAxialY.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dShear);			m_wndShear.SetWindowText(strFactor);

	strFactor.Format(_T("%g"), data.dOutBendingX);	m_wndOutBendingX.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutBendingY);	m_wndOutBendingY.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutTorsion);		m_wndOutTorsion.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutShearX);		m_wndOutShearX.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutShearY);		m_wndOutShearY.SetWindowText(strFactor);

	m_unitPoint.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitVector.SetUnitType(D_UNITSYS_BASE_LENGTH);
	
	CtrlEnableDisable();

	m_edtPoint.SetAttUcsPos();
	m_edtPoint.SetModeToUse(MOUSEEDIT_USE_SET_POS);
	m_edtPoint.SetEnterNotifyWindow(this);
	m_edtPoint.SetLButtonDownNotifyWindow(this);

	m_edtVector.SetAttUcsDistance();
	m_edtVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_edtVector.SetWindowText(_LS(IDS_WG_TREEMENU_0_0_0));

	CString aPlaneDir[] = {_LSX(+X), _LSX(+Y), _LSX(+Z), _LSX(+R), _LSX(+TH),_LSX(-X), _LSX(-Y), _LSX(-Z), _LSX(-R), _LSX(-TH)};

	m_cmbCoordDir.ResetContent();
	int nSize = 10;
	int nIndex = 0;
	for (int i = 0; i < nSize; i++)
	{
		nIndex = m_cmbCoordDir.AddString(aPlaneDir[i]);
		m_cmbCoordDir.SetItemData(nIndex, i);
	}
	m_cmbCoordDir.SetCurSel(data.nCoordDir);
	OnSelchangeCoordDirCmb();

	CDlgUtil::CtrlRadioSetCheck(this, m_aLocalAxisCtrl, m_nLocalAxis);
	CDlgUtil::CtrlRadioSetCheck(this, m_aLocalAxisTypeCtrl, m_nDirType);
	
	UpdateUserDirCtrls(m_bUseUserDir);

	return FALSE;  // return TRUE unless you set the focus to a control
	               // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemPlateReduFactorDlg::OnTmOption()
{
	UpdateData();
	this->m_wndGroupCombo.ShowHideAll(m_nAddOrDel==1);
}

void CElemPlateReduFactorDlg::Execute()
{
	if (!UpdateData(TRUE)) return;
	
	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	ASSERT(pDoc);

	// Plate List 가져오기 - Plate 요소에만 적용 
	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	I_GENModelBase::GetCurMySelfST()->GetSelectedElemKeyList(aElemList);

	T_PSSF_D data; data.Initialize();

	m_wndGroupCombo.GetSelectedBngr(data.GroupKey);

	CString strFactor=_T("");

	m_wndAxialX.GetWindowText(strFactor);		data.dAxialX = _tstof(strFactor);
	m_wndAxialY.GetWindowText(strFactor);		data.dAxialY = _tstof(strFactor);
	m_wndShear.GetWindowText(strFactor);		data.dShear = _tstof(strFactor);

	m_wndOutBendingX.GetWindowText(strFactor);	data.dOutBendingX = _tstof(strFactor);
	m_wndOutBendingY.GetWindowText(strFactor);	data.dOutBendingY = _tstof(strFactor);
	m_wndOutTorsion.GetWindowText(strFactor);		data.dOutTorsion = _tstof(strFactor);
	m_wndOutShearX.GetWindowText(strFactor);		data.dOutShearX = _tstof(strFactor);
	m_wndOutShearY.GetWindowText(strFactor);		data.dOutShearY = _tstof(strFactor);

	data.bUseUserDir = m_bUseUserDir;
	data.nLocalAxis = m_nLocalAxis;
	data.nDirType = m_nDirType;
	data.nCoordDir = m_cmbCoordDir.GetCurSel();

	CString strText=_T("");
	MyParser Parser;

	m_edtPoint.GetWindowText(strText);
	if(!Parser.ParsingPositionArg(strText)) return;
	Parser.GetPositionData(data.dCoordDirX,data.dCoordDirY,data.dCoordDirZ);
	
	m_edtVector.GetWindowText(strText);
	if(!Parser.ParsingPositionArg(strText)) return;
	Parser.GetPositionData(data.dRefVectorX,data.dRefVectorY,data.dRefVectorZ);

	T_ELEM_D ElemD;
	CArray<T_ELEM_K, T_ELEM_K> aPlateList; aPlateList.RemoveAll();
	CArray<T_PSSF_D, T_PSSF_D> aPlateData; aPlateData.RemoveAll();
	for(int i = 0; i< aElemList.GetSize(); i++)
	{
		if (!pDoc->m_pAttrCtrl->GetElem(aElemList[i], ElemD)) continue;    
		if (!pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp)) continue;

		aPlateList.Add(aElemList[i]);
		aPlateData.Add(data);
	}
	if (aPlateList.GetSize() == 0)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Plate));
		return;
	}

	/*
	if(m_bUseUserDir)
	{
	  switch(m_nDirType)
	  {
	  case 0:
		  Execute_Direction(aPlateList, aPlateData);
		  break;
	  case 1:
		  Execute_Vector(aPlateList, aPlateData);
		  break;
	  default:
		  break;
	  }
	}
	*/

	// DB
	BOOL bOK = FALSE;
	if (m_nAddOrDel == 0)   // Add/Replace
	{
		if(aPlateList.GetSize() > 0)    bOK = pDoc->m_pDataCtrl->AddPssf(aPlateList, aPlateData);
	}
	else if (m_nAddOrDel == 1)    // Delete 
	{
		if(aPlateList.GetSize() > 0)    bOK = pDoc->m_pDataCtrl->DelPssf(aPlateList, data.GroupKey);
	}
	else ASSERT(0);

	if (bOK) pDoc->m_pViewCtrl->UnselectAll(NULL);  
}

// For WorkTree
BOOL CElemPlateReduFactorDlg::ExternalInit(UINT key)
{
	if (key == 0) return FALSE;

	T_PSSF_K Key;
	T_PSSF_D data;
	Key.keymap = key;
	CDBDoc::GetDocPoint()->m_pAttrCtrl->GetPssf(Key, data);

	m_wndGroupCombo.ChangeSelect(data.GroupKey);

	CString strFactor=_T("");

	strFactor.Format(_T("%g"), data.dAxialX);			m_wndAxialX.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dAxialY);			m_wndAxialY.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dShear);			m_wndShear.SetWindowText(strFactor);

	strFactor.Format(_T("%g"), data.dOutBendingX);	m_wndOutBendingX.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutBendingY);	m_wndOutBendingY.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutTorsion);		m_wndOutTorsion.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutShearX);		m_wndOutShearX.SetWindowText(strFactor);
	strFactor.Format(_T("%g"), data.dOutShearY);		m_wndOutShearY.SetWindowText(strFactor);

	m_nAddOrDel = 0;

	m_cmbCoordDir.SetCurSel(data.nCoordDir);

	CString strValue=_T("");
	strValue.Format(_T("%g,%g,%g"), data.dCoordDirX,data.dCoordDirY,data.dCoordDirZ);
	m_edtPoint.SetWindowText(strValue);
	strValue.Format(_T("%g,%g,%g"), data.dRefVectorX,data.dRefVectorY,data.dRefVectorZ);
	m_edtVector.SetWindowText(strValue);

	m_nLocalAxis = data.nLocalAxis;
	m_nDirType = data.nDirType;

	m_bUseUserDir = data.bUseUserDir;

	UpdateUserDirCtrls(m_bUseUserDir);
	UpdateData(FALSE);

	return TRUE;
}

void CElemPlateReduFactorDlg::OnTmDefineGroupButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_DEF_BNDR_GROUP,0));	
}

void CElemPlateReduFactorDlg::OnTmUserDirButton() 
{
	m_bUseUserDir = !m_bUseUserDir;
	UpdateUserDirCtrls(m_bUseUserDir);
}

void CElemPlateReduFactorDlg::Execute_Direction(CArray<T_ELEM_K, T_ELEM_K>& aKeyList, CArray<T_PSSF_D, T_PSSF_D>& aDataList)
{
	if(aKeyList.GetSize()<1) return;
	if(aKeyList.GetSize()!=aDataList.GetSize()) { ASSERT(0); return; }

	double RefPoint[3] = {0.0, 0.0, 0.0};
	RefPoint[0] = aDataList[0].dCoordDirX;
	RefPoint[1] = aDataList[0].dCoordDirY;
	RefPoint[2] = aDataList[0].dCoordDirZ;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	T_ELEM_K ElemK=NULL;
	int nDirType=m_cmbCoordDir.GetCurSel();
	for(int i=0; i<aKeyList.GetSize(); i++)
	{
		ElemK = aKeyList[i];
		aDataList[i].dBetaAngle = (-1.0)*pDoc->m_pAttrCtrl->GetLocalAxisAngle4Dir(ElemK, m_nLocalAxis, nDirType, RefPoint);
	}
}

void CElemPlateReduFactorDlg::Execute_Vector(CArray<T_ELEM_K, T_ELEM_K>& aKeyList, CArray<T_PSSF_D, T_PSSF_D>& aDataList)
{
	if(aKeyList.GetSize()<1) return;
	if(aKeyList.GetSize()!=aDataList.GetSize()) { ASSERT(0); return; }

	double RefVector[3] = {0.0, 0.0, 0.0};
	RefVector[0] = aDataList[0].dRefVectorX;
	RefVector[1] = aDataList[0].dRefVectorY;
	RefVector[2] = aDataList[0].dRefVectorZ;

	CDBDoc* pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	T_ELEM_K ElemK=NULL;
	for(int i=0; i<aKeyList.GetSize(); i++)
	{
		ElemK = aKeyList[i];
		aDataList[i].dBetaAngle = (-1.0)*pDoc->m_pAttrCtrl->GetLocalAxisAngle4Vec(ElemK, m_nLocalAxis, RefVector);
	}
}

void CElemPlateReduFactorDlg::CtrlEnableDisable(CArray<UINT, UINT> &aCtrl, BOOL bEnable)
{
	CWnd* pWnd;
	for (int i = 0; i < aCtrl.GetSize(); i++)
	{
		pWnd = GetDlgItem(aCtrl.GetAt(i));
		ASSERT(pWnd);
		pWnd->EnableWindow(bEnable);
	}
}

void CElemPlateReduFactorDlg::CtrlEnableDisable()
{
	OnSelchangeCoordDirCmb();
	CtrlEnableDisable(m_aVectorCtrl, m_nDirType == 1);
}

void CElemPlateReduFactorDlg::OnClickLocalAxisType1Rdo()
{
	m_nDirType=0;
	CtrlEnableDisable();
}

void CElemPlateReduFactorDlg::OnClickLocalAxisType2Rdo()
{
	m_nDirType=1;
	CtrlEnableDisable();
}

void CElemPlateReduFactorDlg::OnClickLocalAxisXRdo()
{
	m_nLocalAxis=0;
}

void CElemPlateReduFactorDlg::OnClickLocalAxisYRdo()
{
	m_nLocalAxis=1;
}

void CElemPlateReduFactorDlg::OnSelchangeCoordDirCmb()
{
	int nSel = m_cmbCoordDir.GetCurSel();
	CtrlEnableDisable(m_aPointCtrl, m_nDirType == 0 && (nSel==3||nSel==4||nSel==8||nSel==9));
	GetDlgItem(IDC_TM_REDU_LOCAL_AXIS_COORD_DIR_CMB)->EnableWindow(m_nDirType==0);
}