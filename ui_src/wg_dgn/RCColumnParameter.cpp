// RCColumnParameter.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCColumnParameter.h"

#include "RCDesignParamDlg.h"
#include "DgnCodeCtrl.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_DlgUtil.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_MembCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_dbLock\LockCtrl.h"

#include "DgnConMatDlg.h"
//#include "..\wg_treemenu\wg_treemenuAll.h"
#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\TreeMenuBarBase.h"

#include "..\wg_rcs\Rcs_ServiceDef.h"
#include "..\wg_base\ServiceProvider.h"

// Coded by sshan('20061108)
#include "..\wg_db\IUsageCounter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCColumnParameter dialog


CRCColumnParameter::CRCColumnParameter(CWnd* pParent /*=NULL*/)
	: CChildDialog(CRCColumnParameter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCColumnParameter)
	m_bShearChk = TRUE;
	m_dRhomax = 0.08;
	m_dCmy = 1.0;
	m_dCmz = 1.0;
	m_bAutoBoundary = FALSE;
	m_iXDirBd = 0;
	m_iYDirBd = 0;
	m_bIEnd = TRUE;
	m_bJEnd = FALSE;
	m_bMid = FALSE;
	m_strRemark = _T("");
	m_SelectElemString = _T("");
	m_dDeltla_Nsy = 1.0;
	m_dDeltla_Nsz = 1.0;
	m_dDeltla_Sy = 1.0;
	m_dDeltla_Sz = 1.0;
	m_bEquivalMoment = FALSE;
	m_bMomentEnlarge = FALSE;
	m_bMomentEnlargeNocross = FALSE;	
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CRCDesignParamDlg*)pParent;

	m_Data.Initialize();  

	m_ColumnGrid = new CDgnRCColumnGrid();

	m_pDoc = CDBDoc::GetDocPoint();	
	m_pMembCtrl = CMembCtrl::GetMembCtrl();

	m_RccoData.InitHashTable(100);
}


CRCColumnParameter::~CRCColumnParameter()
{
	if(m_ColumnGrid != NULL)
	{
		delete m_ColumnGrid;
		m_ColumnGrid = NULL;
	}

	m_arElemK.RemoveAll();
	m_arLcomKey.RemoveAll();
	m_arGrupKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_pBadKeyList.RemoveAll();
	m_pNoExistREBTKeyList.RemoveAll();
	m_pBadMemberKeyList.RemoveAll();

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_arGroupMapKey.RemoveAll();
	m_RccoData.RemoveAll();
}


void CRCColumnParameter::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCColumnParameter)
	DDX_Control(pDX, IDC_DGN_COLUMN_AUTO_EQUIVALENT_MOMENT_CHK, m_Ctrl_Auto_EquivalentMoment_Check);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_DELTA_NSZ, m_Ctrl_Delta_Nsz_Edit);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_DELTA_NSY, m_Ctrl_Delta_Nsy_Edit);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_DELTA_SZ, m_Ctrl_Delta_Sz_Edit);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_DELTA_SY, m_Ctrl_Delta_Sy_Edit);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_EQUIVALENT_MOMENT2, m_Ctrl_CmzEdit);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_EQUIVALENT_MOMENT, m_Ctrl_CmyEdit);
	DDX_Control(pDX, IDC_DGN_COLUMN_CMZ_STATIC, m_Ctrl_Cmz);
	DDX_Control(pDX, IDC_DGN_COLUMN_CMY_STATIC, m_Ctrl_Cmy);
	DDX_Control(pDX, IDC_DGN_COLUMN_DELTA_SZ_STATIC, m_Ctrl_Delta_Sz);
	DDX_Control(pDX, IDC_DGN_COLUMN_DELTA_SY_STATIC, m_Ctrl_Delta_Sy);
	DDX_Control(pDX, IDC_DGN_COLUMN_DELTA_NSZ_STATIC, m_Ctrl_Delta_Nsz);
	DDX_Control(pDX, IDC_DGN_COLUMN_DELTA_NSY_STATIC, m_Ctrl_Delta_Nsy);
	DDX_Control(pDX, IDC_DGN_COLUMN_RAO_YDIR_BRACED, m_Ctrl_iYDirBd_Braced);
	DDX_Control(pDX, IDC_DGN_COLUMN_RAO_XDIR_BRACED, m_Ctrl_iXDirBd_Braced);
	DDX_Control(pDX, IDC_DGN_COLUMN_RAO_YDIR_UNBRACED, m_Ctrl_iYDirBd);
	DDX_Control(pDX, IDC_DGN_COLUMN_RAO_XDIR_UNBRACED, m_Ctrl_iXDirBd);
	DDX_Control(pDX, IDC_DGN_COLUMN_EnableCtrl2, m_Ctrl_EnableCtrl2);
	DDX_Control(pDX, IDC_DGN_COLUMN_EnableCtrl1, m_Ctrl_EnableCtrl1);
	DDX_Control(pDX, IDC_DGN_COLUMN_LIST_LCOM_SERVICE, m_ltbxLcomServ);
	DDX_Control(pDX, IDC_DGN_COLUMN_LIST_LCOM_STRENGTH, m_ltbxLcomStre);
	DDX_Control(pDX, IDC_DGN_COLUMN_LIST_GROUP, m_ltbxGroup);
	DDX_Check(pDX, IDC_DGN_COLUMN_CHECK_COL_SHEAR, m_bShearChk);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_MAX_RBAR_RATIO, m_dRhomax);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_EQUIVALENT_MOMENT, m_dCmy);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_EQUIVALENT_MOMENT2, m_dCmz);
	DDX_Check(pDX, IDC_DGN_COLUMN_CHECK2, m_bAutoBoundary);
	DDX_Radio(pDX, IDC_DGN_COLUMN_RAO_XDIR_UNBRACED, m_iXDirBd);
	DDX_Radio(pDX, IDC_DGN_COLUMN_RAO_YDIR_UNBRACED, m_iYDirBd);
	DDX_Check(pDX, IDC_DGN_COLUMN_CHECK_IEND, m_bIEnd);
	DDX_Check(pDX, IDC_DGN_COLUMN_CHECK_JEND, m_bJEnd);
	DDX_Check(pDX, IDC_DGN_COLUMN_CHECK_MID, m_bMid);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_POSITION, m_strRemark);
	DDX_Control(pDX, IDC_DGN_COLUMN_DCUNIT, m_dDCunit);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_DC, m_Dc);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_ELEMENT, m_SelectElemString);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_DELTA_NSY, m_dDeltla_Nsy);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_DELTA_NSZ, m_dDeltla_Nsz);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_DELTA_SY, m_dDeltla_Sy);
	DDX_Text(pDX, IDC_DGN_COLUMN_EDIT_DELTA_SZ, m_dDeltla_Sz);
	DDX_Check(pDX, IDC_DGN_COLUMN_AUTO_EQUIVALENT_MOMENT_CHK, m_bEquivalMoment);
	DDX_Check(pDX, IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_CHK, m_bMomentEnlarge);
	DDX_Check(pDX, IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_NOCROSS_CHK, m_bMomentEnlargeNocross);
	DDX_Control(pDX, IDC_DGN_COLUMN_CTC_CHECK, m_chkCTC);
	DDX_Control(pDX, IDC_DGN_COLUMN_AUTO_BD_CROSS_CHK, m_chkCroseBd);
	DDX_Control(pDX, IDC_DGN_COLUMN_AUTO_BD_NOCROSS_CHK, m_chkNoCroseBd);
	DDX_Control(pDX, IDC_DGN_COLUMN_CTC_UNIT, m_unitCTC);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_CTC, m_editCTC);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_BD_CROSS_Y, m_editBetaDCroseY);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_BD_CROSS_Z, m_editBetaDCroseZ);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_BD_NOCROSS_Y, m_editBetaDNoCroseY);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_BD_NOCROSS_Z, m_editBetaDNoCroseZ);
	DDX_Control(pDX, IDC_DGN_COLUMN_R_CHK, m_chkRfactor);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_R_Y, m_editRfactorY);
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_R_Z, m_editRfactorZ);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DGN_COLUMN_EDIT_ELEMENT, m_editSelectElem);
	DDX_Control(pDX, IDC_DGN_COLUMN_TM_COLUMN_GRID, *m_ColumnGrid);
}


BEGIN_MESSAGE_MAP(CRCColumnParameter, CChildDialog)
	//{{AFX_MSG_MAP(CRCColumnParameter)
	ON_BN_CLICKED(IDC_DGN_COLUMN_BTN_ADD, OnAddData)
	ON_BN_CLICKED(IDC_DGN_COLUMN_BTN_MODIFY, OnModifyData)
	ON_BN_CLICKED(IDC_DGN_COLUMN_BTN_DELETE, OnDeleteData)
	ON_BN_CLICKED(IDC_DGN_COLUMN_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_DGN_COLUMN_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_DGN_COLUMN_CALC_BTN, OnDgnColumnCalcBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_AUTO_EQUIVALENT_MOMENT_CHK, OnAutoEquivalentMomentChk)
	ON_BN_CLICKED(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_NOCROSS_CHK, OnAutoMomentEnlargeNocrossChk)
	ON_BN_CLICKED(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_CHK, OnAutoMomentEnlargeChk)
	ON_BN_CLICKED(IDC_DGN_COLUMN_MODIFY_CONC_MATE_BTN, OnDgnColumnModifyConcreteMaterialBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_UNBRACED_LENGTH_BTN, OnDgnColumnUnbracedLengthBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_EFFECTIVE_LENGTH_BTN, OnDgnColumnEffectiveLengthBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_RAO_XDIR_UNBRACED, OnClickYDirectionRadioBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_RAO_XDIR_BRACED, OnClickYDirectionRadioBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_RAO_YDIR_UNBRACED, OnClickZDirectionRadioBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_RAO_YDIR_BRACED, OnClickZDirectionRadioBtn)
	ON_BN_CLICKED(IDC_DGN_COLUMN_CTC_CHECK, OnClickChkCTC)
	ON_BN_CLICKED(IDC_DGN_COLUMN_AUTO_BD_CROSS_CHK, OnClickChkCroseBd)
	ON_BN_CLICKED(IDC_DGN_COLUMN_AUTO_BD_NOCROSS_CHK, OnClickChkNoCroseBd)
	ON_BN_CLICKED(IDC_DGN_COLUMN_R_CHK, OnClickRfactor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface
/*
void CRCColumnParameter::SetCurData(T_RPSC_D* pData)
{
	ASSERT(pData);
	m_Data = *pData;
	if (m_Data.bMbarJ == 0)
	{
		m_Data.Mbar[1].Copy(m_Data.Mbar[0]);
	}
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd))
		Data2Dlg();
}

BOOL CRCColumnParameter::GetCurData(T_RPSC_D* pData)
{
	ASSERT(pData);
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd != NULL && ::IsWindow(hWnd)) Dlg2Data(pData);
	else
	{
		pData->bMbarJ = m_Data.bMbarJ;
		pData->Mbar[0].Copy(m_Data.Mbar[0]);
		pData->Mbar[1].Copy(m_Data.Mbar[1]);
	}
	return TRUE;
}
*/
/////////////////////////////////////////////////////////////////////////////
// CRCColumnParameter message handlers

BOOL CRCColumnParameter::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
		
	// TODO: Add extra initialization here

	
	m_ColumnGrid->m_RccoData.RemoveAll();
	m_ColumnGrid->Initialize(this);
	
	ASSERT(m_pDoc);

	// Unit.
	SetInitUnit();
	// Lcom.
	InitialLcomDataForDgn();
	
	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);  

	LoadDBtoDlg();
	OnAutoMomentEnlargeNocrossChk();
	OnAutoEquivalentMomentChk();
	OnAutoMomentEnlargeChk();
	YZDirectionAllSwayEnableControl();
	OnCheck2();
	OnClickChkCTC();
	OnClickChkCroseBd();
	OnClickChkNoCroseBd();
	OnClickRfactor();
//  Data2Dlg();
	  
	//해석되어 있지 않으면 계산버튼 비활성화
	if(!m_pDoc->IsPostMode())
		GetDlgItem(IDC_DGN_COLUMN_CALC_BTN)->EnableWindow(false);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CRCColumnParameter::DestroyWindow() 
{
	m_arElemK.RemoveAll();
	m_arLcomKey.RemoveAll();
	m_arGrupKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_pBadKeyList.RemoveAll();
	m_pNoExistREBTKeyList.RemoveAll();
	m_pBadMemberKeyList.RemoveAll();

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_arGroupMapKey.RemoveAll();
	m_RccoData.RemoveAll();

	return CChildDialog::DestroyWindow();
}

void CRCColumnParameter::InitialData()
{
	m_arLcomKey.RemoveAll();
}

BOOL CRCColumnParameter::Data2Dlg()
{
	CArray<long, long> caKey;
	m_SelectElemString.Empty();

	m_ColumnGrid->GetSelectedKeys(caKey);
	if(caKey.GetSize()>0)
	{
		CString TempString;
		for(int i=0 ; i<caKey.GetSize() ; i++)
		{
			if(m_SelectElemString.GetLength()==0)
				m_SelectElemString.Format(_T("%d"), caKey[i]);
			else
			{
				TempString.Format(_T(" %d"), caKey[i]);
				m_SelectElemString += TempString;
			}
		}
	}
	else
	{
		T_ELEM_K aKey;
		if(m_ColumnGrid->GetCurrentRcco(aKey))
			m_SelectElemString.Format(_T("%d"), aKey);
	}

	m_iXDirBd = m_Data.iXDirBd;
	m_iYDirBd = m_Data.iYDirBd;
	m_bShearChk = m_Data.bShearChk;
	m_bAutoBoundary = m_Data.bAutoBoundary;
	m_dRhomax = m_Data.dRhomax;
	m_dCmy = m_Data.dCmy;
	m_dCmz = m_Data.dCmz;
	m_Dc.SetEditUnit(m_Data.dDc);
	m_dDeltla_Nsy = m_Data.dDelta_nsy;
	m_dDeltla_Nsz = m_Data.dDelta_nsz;
	m_dDeltla_Sy = m_Data.dDelta_sy;
	m_dDeltla_Sz = m_Data.dDelta_sz;
	m_bEquivalMoment = m_Data.bAutoEqCoeff;
	m_bMomentEnlarge = m_Data.bAutoDeltans;
	m_bMomentEnlargeNocross = m_Data.bAutoDeltas;
	m_strRemark = m_Data.strRemark;
	
	m_bIEnd = m_Data.bPosi[0];
	m_bJEnd = m_Data.bPosi[2];
	m_bMid	= m_Data.bPosi[1];

	
	//CTC	
	m_chkCTC.SetCheck(m_Data.bCTC);
	m_editCTC.SetEditUnit(m_Data.dCTC);

	//Bd
	CString strTemp = _T("");
	m_chkCroseBd.SetCheck(m_Data.bUserBd_NS);
	m_chkNoCroseBd.SetCheck(m_Data.bUserBd_S);
	strTemp.Format(_T("%.3f"), m_Data.dBd_nsy);
	m_editBetaDCroseY.SetWindowText(strTemp);
	strTemp.Format(_T("%.3f"), m_Data.dBd_nsz);
	m_editBetaDCroseZ.SetWindowText(strTemp);
	strTemp.Format(_T("%.3f"), m_Data.dBd_sy);
	m_editBetaDNoCroseY.SetWindowText(strTemp);
	strTemp.Format(_T("%.3f"), m_Data.dBd_sz);
	m_editBetaDNoCroseZ.SetWindowText(strTemp);

	//R_factor
	m_chkRfactor.SetCheck(m_Data.bRSCoeff);
	m_editRfactorY.SetEditUnit(m_Data.dRy);
	m_editRfactorZ.SetEditUnit(m_Data.dRz);
	
	UpdateData(FALSE);

	m_editSelectElem.SelectByStr(m_SelectElemString);

	OnAutoMomentEnlargeNocrossChk();
	OnAutoEquivalentMomentChk();
	OnAutoMomentEnlargeChk();
	YZDirectionAllSwayEnableControl();
	OnCheck2();
	OnClickChkCTC();
	OnClickChkCroseBd();
	OnClickChkNoCroseBd();
	OnClickRfactor();

	return TRUE;
}

BOOL CRCColumnParameter::Dlg2Data()
{
	m_Data.Initialize();  
	//m_arElemK.Copy(m_pParent->m_arSelElemK);

	m_arElemK.RemoveAll();  

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(m_arElemK);

	m_pParent->UpdateData(TRUE);
	
	UpdateData(TRUE);
	if(addDelMod != 2)//Delete일 경우는 데이터 체크를 하지않는다
		if(!DataCheck()) return FALSE;
	
	int iChkElemNum = m_arElemK.GetSize();
	m_RccoData.RemoveAll();   

 	UpdateData(TRUE);

	// Check Position.
	m_bCheckPosi[0] = m_bIEnd;
	m_bCheckPosi[2] = m_bJEnd;
	m_bCheckPosi[1] = m_bMid;  
	
	// Strength LoadCombination.
	
	CString sName;
	DWORD nID;
	int nCount = m_ltbxLcomStre.GetCount();
	//int *aIndex = new int[nCount]; // Not Used.
	
	for (int k = 0; k < nCount ; k++)
	{
		if(m_ltbxLcomStre.GetCheck(k) == 1)
		{
			nID = m_ltbxLcomStre.GetItemData(k);
			T_LCOM_K LcomK = m_arLcomKey.GetAt(int(nID));
			m_arStreLcomKey.SetAt(LcomK,LcomK);
		}
	}

	nCount = m_ltbxLcomServ.GetCount();
	//aIndex = new int[nCount];

	for (int j = 0; j < nCount ; j++)
	{
		if(m_ltbxLcomServ.GetCheck(j) == 1)
		{
			nID = m_ltbxLcomServ.GetItemData(j);
			T_LCOM_K LcomK = m_arLcomKey.GetAt(int(nID));
			m_arServLcomKey.SetAt(LcomK,LcomK);
		}
	}
	
	T_LCOM_K LcomK, GetLcomK;
	POSITION Pos = m_arStreLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arStreLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		m_StreLcom.Add(GetLcomK);
	}
	Pos = m_arServLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arServLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		m_ServLcom.Add(GetLcomK);
	}
	
	// Sorting.
	qsort(m_StreLcom.GetData(), m_StreLcom.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
	qsort(m_ServLcom.GetData(), m_ServLcom.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);
		
	m_Data.iXDirBd = m_iXDirBd;
	m_Data.iYDirBd = m_iYDirBd;
	m_Data.bShearChk = m_bShearChk;
	m_Data.bAutoBoundary = m_bAutoBoundary;
	m_Data.dRhomax = m_dRhomax;
	m_Data.dCmy = m_dCmy;
	m_Data.dCmz = m_dCmz;
	m_Data.strRemark = m_strRemark;
	m_Data.dDc = m_Dc.GetEditValue();
	m_Data.dDelta_nsy = m_dDeltla_Nsy;
	m_Data.dDelta_nsz = m_dDeltla_Nsz;
	m_Data.dDelta_sy = m_dDeltla_Sy;
	m_Data.dDelta_sz = m_dDeltla_Sz;
	m_Data.bAutoEqCoeff = m_bEquivalMoment;
	m_Data.bAutoDeltans = m_bMomentEnlarge;
	m_Data.bAutoDeltas = m_bMomentEnlargeNocross;

	//CTC
	m_Data.bCTC = m_chkCTC.GetCheck();
	m_Data.dCTC = m_editCTC.GetEditValue();

	//Bd
	m_Data.bUserBd_NS = m_chkCroseBd.GetCheck();
	m_Data.bUserBd_S = m_chkNoCroseBd.GetCheck();
	CString strTemp = _T("");
	m_editBetaDCroseY.GetWindowText(strTemp);
	m_Data.dBd_nsy = _tstof(strTemp);	
	m_editBetaDCroseZ.GetWindowText(strTemp);
	m_Data.dBd_nsz = _tstof(strTemp);	
	m_editBetaDNoCroseY.GetWindowText(strTemp);
	m_Data.dBd_sy = _tstof(strTemp);	
	m_editBetaDNoCroseZ.GetWindowText(strTemp);
	m_Data.dBd_sz = _tstof(strTemp);

	//R_factor
	m_Data.bRSCoeff = m_chkRfactor.GetCheck();
	m_Data.dRy = m_editRfactorY.GetEditValue();
	m_Data.dRz = m_editRfactorZ.GetEditValue();

	for(int i=0; i<m_arElemK.GetSize(); i++)
	{
		m_Data.bPosi[0] = m_bIEnd;
		m_Data.bPosi[2] = m_bJEnd;
		m_Data.bPosi[1] = m_bMid;

		m_RccoData.SetAt(m_arElemK[i], m_Data);
	}

	return TRUE;
}

void CRCColumnParameter::SetInitUnit()
{
	m_Dc.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dDCunit.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_editCTC.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitCTC.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_editRfactorY.SetUnitType(D_UNITSYS_NONE);
	m_editRfactorZ.SetUnitType(D_UNITSYS_NONE);
}

BOOL CRCColumnParameter::GetMaterialData(T_ELEM_K ElemK, double& dfck, double& dfy, double& dEratio)
{
	// Get Elem.
	T_ELEM_D ElemD;
	ElemD.Initialize();
	if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK,ElemD))	return FALSE;
	// Get Matl.
	T_MATD_D MatdD;
	if(!m_pDoc->m_pAttrCtrl->GetMatlDesign(ElemD.elmat,MatdD))	return FALSE;	
	if(MatdD.Type!=_T("C"))	return FALSE;
	// Set Default.
	//Get_ConMatd(MatdD);

	//Material
	dfck= MatdD.Data1.Design.C_fc;
	dfy = MatdD.MainRebarData.B_fy;  
	double dEs=0.0, dEc=0.0;
	dEc = MatdD.Data1.Analysis.Elast;
	dEs = MatdD.MainRebarData.B_Elast;
	dEratio = dEc==0.0 ? 0.0 : dEs/dEc;

	return TRUE;  
}


void CRCColumnParameter::OnAddData()
{
	// 여러개 선택할 수도 있다. -> 처리해야함.
	addDelMod = 0;

	if (!Dlg2Data()) 
	{
		AfxMessageBox(_LS(IDS_DGN_INPUT_DATA_ERROR));
		return;
	}

	m_pNoExistREBTKeyList.RemoveAll();
	m_pBadKeyList.RemoveAll();
	m_pBadMemberKeyList.RemoveAll();

	T_ELEM_K ElemK;
	T_RCCO_D RccoD;
	POSITION Pos = m_RccoData.GetStartPosition();
	while(Pos)
	{
		m_RccoData.GetNextAssoc(Pos,ElemK,RccoD); 
		
		BOOL bReverseDir;
	  T_ELEM_K ElemShowKey;
	  CArray<T_ELEM_K, T_ELEM_K> aElemList;
	  aElemList.RemoveAll();
	  m_pMembCtrl->GetElemListByIncludeElem(ElemK, bReverseDir, ElemShowKey, aElemList);    
		if(aElemList.GetSize() > 0)
		{
			if(ElemK != ElemShowKey)
				m_pBadMemberKeyList.Add(ElemK);
		}
		if(!ExistSectREBTByElem(ElemK))
			m_pNoExistREBTKeyList.Add(ElemK);
	}

	int nBadMemberKeySize = m_pBadMemberKeyList.GetSize();
	for(int i=0 ; i<nBadMemberKeySize ; ++i)
	{
		m_RccoData.RemoveKey(m_pBadMemberKeyList[i]);//Member가 아닌 키 삭제
	}
	int nNoExistREBTKeySize = m_pNoExistREBTKeyList.GetSize();
	for(int i=0 ; i<nNoExistREBTKeySize ; ++i)
	{
		m_RccoData.RemoveKey(m_pNoExistREBTKeyList[i]);//철근이 없는 임의단면키 삭제
	}

	UpdateDgnData();//DB 업데이트

	if(m_pBadMemberKeyList.GetSize()>0)
	{
		CString Message = _T("");
		CString strElem = _T("");
		for(int i=0 ; i<m_pBadMemberKeyList.GetSize() ; i++)
		{
			strElem.Format(_T("%d"), m_pBadMemberKeyList[i]);
			if(Message == _T(""))
				Message = strElem;
			else
				Message = Message + _T(", ") + strElem;
		}
		Message = Message + _LS(IDS_DGN_NOT_INPUT_DATA_NOT_MEMBER);
		AfxMessageBox(Message, MB_OK);
	}

	if(m_pNoExistREBTKeyList.GetSize()>0)
	{
		CString Message = _T("");
		CString strElem = _T("");
		for(int i=0 ; i<m_pNoExistREBTKeyList.GetSize() ; i++)
		{
			strElem.Format(_T("%d"), m_pNoExistREBTKeyList[i]);
			if(Message == _T(""))
				Message = strElem;
			else
				Message = Message + _T(", ") + strElem;
		}
		Message = Message + _LS(IDS_DGN_NOT_INPUT_REBAR_ERROR);//_T("번\n요소의 해당 단면에 입력된 철근 정보가 없습니다.\nDesign >>RC Design >>Rebar Input for General Section...에서 \n철근을 입력하시기 바랍니다")
		AfxMessageBox(Message, MB_OK);
	}

	if(m_pBadKeyList.GetSize()>0)
	{
		CString Message = _T("");
		CString strElem = _T("");
		for(int i=0 ; i<m_pBadKeyList.GetSize() ; i++)
		{
			strElem.Format(_T("%d"), m_pBadKeyList[i]);
			if(Message == _T(""))
				Message = strElem;
			else
				Message = Message + _T(", ") + strElem;
		}
		Message = Message + _LS(IDS_DGN_BAD_INPUT_DATA_FOR_MEMBER_ERROR);//_T("번\n요소는 데이터가 부적절하여 추가하실 수 없습니다.")
		AfxMessageBox(Message, MB_OK);
	}

	Pos = m_RccoData.GetStartPosition();
	while(Pos)
	{
		m_RccoData.GetNextAssoc(Pos,ElemK,RccoD);        
	}
	m_ColumnGrid->SetCurrentRcco(ElemK, RccoD);//마지막으로 추가된것을 현재 셀로
}

void CRCColumnParameter::OnModifyData()
{
	addDelMod = 1;

	T_ELEM_K ElemK;
	T_RCCO_D RccoD;

	CArray<T_RCCO_K, T_RCCO_K> arKey;
	CArray<T_RCCO_D, T_RCCO_D&> arData;
	arKey.RemoveAll();
	arData.RemoveAll();
	
	if (!Dlg2Data()) return;

	POSITION Pos = m_RccoData.GetStartPosition();
	while(Pos)
	{
		m_RccoData.GetNextAssoc(Pos,ElemK,RccoD);
		arKey.Add(ElemK);
		arData.Add(RccoD);	 
	}  	
	m_pDoc->m_pDataCtrl->ModifyRcco(arKey, arKey, arData);
}


void CRCColumnParameter::OnDeleteData()
{
	addDelMod = 2;

	if (!Dlg2Data()) return;	

	T_ELEM_K ElemK;
	T_RCCO_D RccoD;
	CArray<T_RCCO_K, T_RCCO_K> arKey;  
	arKey.RemoveAll();
	
	POSITION Pos = m_RccoData.GetStartPosition();
	while(Pos)
	{
		m_RccoData.GetNextAssoc(Pos,ElemK,RccoD);   
		arKey.Add(ElemK);      
	} 
	m_pDoc->m_pDataCtrl->DelRcco(arKey);
}

void CRCColumnParameter::ChangeGroupandLoadCombination()
{
	m_ltbxLcomServ.ResetContent();
	m_ltbxLcomStre.ResetContent();
	m_ltbxGroup.ResetContent();

	InitialLcomDataForDgn();
}


void CRCColumnParameter::InitialLcomDataForDgn()
{ 
	// Save Total Load Combination Name by Serial Number(No LcomKey).
	
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, m_arLcomKey);
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(m_arGrupKey);

	T_RCCG_D RccgData;
	if(m_pDoc->m_pAttrCtrl->ExistRccg())//DB에 데이터가 있다면 DB에서 데이터를 가져와서 세팅
	{
		m_pDoc->m_pAttrCtrl->GetRccg(RccgData);
		int i;
		m_arStreLcomKey.RemoveAll();
		for(i=0 ; i<RccgData.StreLcom.GetSize() ; i++)
		{
			m_arStreLcomKey.SetAt(RccgData.StreLcom[i], RccgData.StreLcom[i]);
		}

		m_arServLcomKey.RemoveAll();
		for(i=0 ; i<RccgData.ServLcom.GetSize() ; i++)
		{
			m_arServLcomKey.SetAt(RccgData.ServLcom[i], RccgData.ServLcom[i]);
		}

		m_arGroupMapKey.RemoveAll();
		for(i=0 ; i<RccgData.ColGroupKey.GetSize() ; i++)
		{
			m_arGroupMapKey.SetAt(RccgData.ColGroupKey[i], RccgData.ColGroupKey[i]);
		}
	}

	int iLcomSerNum=0;
	int i=0, j=0;
	int iLcomCount=0;
	
	int nID;
	int nIndex;  
	for(i=0; i<m_arLcomKey.GetSize(); i++)
	{
		T_LCOM_D LcomData;
		T_LCOM_K ExistKey;
		LcomData.Initialize();
		m_pDoc->m_pAttrCtrl->GetLcom(D_LCOMTYPE_CONCRETE, m_arLcomKey[i], LcomData);
		if(LcomData.nActive==0)	continue;
		// Modify by GAY. MNET:2903. ('07.08.24). Add Type의 하중조합만 Combo Box에 Setting.
		if(LcomData.LoadCombType!=0) continue;
		
		CString strLcomName = LcomData.LoadCombName;
		
		nID = i; //i+1;
		//m_aItem.Add(STBSortItem(nID, TRUE));

		bool bExist = false; //해당 LoadCase가 DB에 존재하면 Check True
		if(LcomData.nActive==1)
		{
			nIndex = m_ltbxLcomStre.AddString(strLcomName);
			m_ltbxLcomStre.SetItemData(nIndex, DWORD(nID));
			if(m_pDoc->m_pAttrCtrl->ExistRccg())//DB에 저장된것이 있다면 저장된 값으로 체크
			{
				if(m_arStreLcomKey.Lookup(m_arLcomKey[i], ExistKey))
					bExist = true;
				else
					bExist = false;
			}
			else//DB저장된 데이터가 없다면
			{
				bExist = true;
			}			
			m_ltbxLcomStre.SetCheck(nIndex, bExist);
		}
		else if(LcomData.nActive==2)
		{
			nIndex = m_ltbxLcomServ.AddString(strLcomName);
			m_ltbxLcomServ.SetItemData(nIndex, DWORD(nID));
			if(m_pDoc->m_pAttrCtrl->ExistRccg())//DB에 저장된것이 있다면 저장된 값으로 체크
			{
				if(m_arServLcomKey.Lookup(m_arLcomKey[i], ExistKey))
					bExist = false;//현재 사용하중 조합 사용하지 않음(sshan)
				else
					bExist = false;
			}
			else//DB저장된 데이터가 없다면
			{
				bExist = false;//현재 사용하중 조합 사용하지 않음(sshan)
			}			
			m_ltbxLcomServ.SetCheck(nIndex, bExist);
		}   
	}

	CMap<T_ELEM_K,T_ELEM_K,T_ELEM_K,T_ELEM_K> ElemKeyListInGroup;
	CMap<T_ELEM_K,T_ELEM_K,T_ELEM_K,T_ELEM_K> OverlappingElemKeyList;
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K> GroupKeyList;
	CArray<CString, CString> OverlappingGroupList;
	
	//중복되는 요소키를 뽑아 낸다
	for(i=0; i<m_arGrupKey.GetSize(); i++)
	{
		T_GRUP_D dGrup;
		CArray<T_ELEM_K, T_ELEM_K> ElemKeyList;
		dGrup.Initialize();
		m_pDoc->m_pAttrCtrl->GetGrup(m_arGrupKey[i], dGrup);
		ElemKeyList.Copy(dGrup.arKeyElem);

		if(i==0)
		{//첫번째 그룹의 elem키 리스트는 그대로 ElemKeyListInGroup에 넣는다
			for(int c=0 ; c<ElemKeyList.GetSize() ; c++)
			{
				ElemKeyListInGroup.SetAt(ElemKeyList[c], ElemKeyList[c]);
			}
		}
		else
		{//두번째 부터는 ElemKeyListInGroup에 존재하는지 확인한 후 존재하는 것은 중복되는 것이므로
		 //OverlappingElemKeyList에 넣는다
			for(int c=0 ; c<ElemKeyList.GetSize() ; c++)
			{
				T_ELEM_K temp;
				if(ElemKeyListInGroup.Lookup(ElemKeyList[c], temp))//중복
				{
					OverlappingElemKeyList.SetAt(ElemKeyList[c], ElemKeyList[c]);
				}
				else//중복되지 않음
				{
					ElemKeyListInGroup.SetAt(ElemKeyList[c], ElemKeyList[c]);
				}
			}
		}
	}

	//중복되는 요소키가 없는 그룹만 고른다
	for(i=0; i<m_arGrupKey.GetSize(); i++)
	{
		T_GRUP_D dGrup;
		CArray<T_ELEM_K, T_ELEM_K> ElemKeyList;
		dGrup.Initialize();
		m_pDoc->m_pAttrCtrl->GetGrup(m_arGrupKey[i], dGrup);
		ElemKeyList.Copy(dGrup.arKeyElem);

		BOOL bGrup = TRUE;

		int c = 0;
		while(c<ElemKeyList.GetSize() && bGrup)
		{
			T_ELEM_K temp = -1;
			if(OverlappingElemKeyList.Lookup(ElemKeyList[c], temp))//중복된 elem키가 저장된곳에서 일치하는 것이 있으면 중복요소를 가지고있는 그룹이다
			{
				OverlappingGroupList.Add(dGrup.GroupName);
				bGrup = FALSE;			
			}
			c++;
		}
		
		if(bGrup)
			GroupKeyList.SetAt(m_arGrupKey[i], m_arGrupKey[i]);
	}

	m_arGrupKey.RemoveAll();

	T_GRUP_K gkey, gKey;
	POSITION Pos = GroupKeyList.GetStartPosition();
	while(Pos)
	{
		GroupKeyList.GetNextAssoc(Pos,gkey,gKey);
		m_arGrupKey.Add(gKey);
	}


	for(i=0; i<m_arGrupKey.GetSize(); i++)
	{
		T_GRUP_D GroupData;
		T_GRUP_K ExistKey;
		GroupData.Initialize();
		m_pDoc->m_pAttrCtrl->GetGrup(m_arGrupKey[i], GroupData);
				
		CString strGrupName = GroupData.GroupName;
		
		nID = i; //i+1;
		//m_aItem.Add(STBSortItem(nID, TRUE));

		bool bExist = false; //해당 LoadCase가 DB에 존재하면 Check True
		
		nIndex = m_ltbxGroup.AddString(strGrupName);
		m_ltbxGroup.SetItemData(nIndex, DWORD(nID));

		if(m_pDoc->m_pAttrCtrl->ExistRccg())//DB에 저장된것이 있다면 저장된 값으로 체크
		{
			if(m_arGroupMapKey.Lookup(m_arGrupKey[i], ExistKey))
				bExist = true;
			else
				bExist = false;
		}
		else//DB저장된 데이터가 없다면
		{
			bExist = true;
		}			
		m_ltbxGroup.SetCheck(nIndex, bExist);    
	}

	CArray<T_ELEM_K, T_ELEM_K> arOverlappingElemKey;
	arOverlappingElemKey.RemoveAll();

	T_ELEM_K ekey, eKey;
	Pos = OverlappingElemKeyList.GetStartPosition();
	while(Pos)
	{
		OverlappingElemKeyList.GetNextAssoc(Pos,ekey,eKey);		
		arOverlappingElemKey.Add(eKey);
	}

	qsort(arOverlappingElemKey.GetData(), arOverlappingElemKey.GetSize(), sizeof(UINT), CCompFunc::UINTAsc);


	if(OverlappingElemKeyList.GetCount()>0 && !m_pParent->m_bGroupMessage)
	{
		CString strOverlappingElemKey = _T("");
		CString strOverlappingGroupName = _T("");
		CString strTemp;		

		for(i=0 ; i<arOverlappingElemKey.GetSize() ; i++)
		{
			if(strOverlappingElemKey == _T(""))
			{
				strTemp.Format(_T("%d"), arOverlappingElemKey[i]);
				strOverlappingElemKey = strTemp;
			}
			else
			{
				strTemp.Format(_T(", %d"), arOverlappingElemKey[i]);
				strOverlappingElemKey += strTemp;
			}
		}

		for(i=0 ; i<OverlappingGroupList.GetSize() ; i++)
		{
			if(strOverlappingGroupName == _T(""))
				strOverlappingGroupName = OverlappingGroupList[i];
			else
			{
				strTemp.Format(_T(", %s"), OverlappingGroupList[i]);
				strOverlappingGroupName += strTemp;
			}
		}

		CString Message = _T("");
		Message.Format(_LS(IDS_DGN_MEMBER_GROUP_ERROR), strOverlappingElemKey, strOverlappingGroupName);//_T("%s번요소가\n여러 그룹에 중복정의 되어\n%s 그룹을\n제외하였습니다.")
		AfxMessageBox(Message, MB_OK);

		m_pParent->m_bGroupMessage = TRUE;
	}
	
}


void CRCColumnParameter::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		break;
	case D_UPDATE_UNIT:
		break;
	case D_UPDATE_SEL_ADD: case D_UPDATE_SEL_DEL:
		break;
	case D_UPDATE_EXE_START: case D_UPDATE_EXE_END:
		break;	
	default:
		//ASSERT(FALSE);
		break;
	}
}

//임의단면 철근을 입력해야하는 조건에 해당되는 단면을 가지고
//있으나 아직 단면을 입력하지 않은경우 False를 리턴한다
BOOL CRCColumnParameter::ExistSectREBTByElem(T_ELEM_K eKey)
{
	BOOL bResult = TRUE;

	T_ELEM_D ElemData;
	T_SECT_K SectKey;
	T_SECT_D SectData;
	m_pDoc->m_pAttrCtrl->GetElem(eKey, ElemData);
	SectKey = ElemData.elpro;
	m_pDoc->m_pAttrCtrl->GetSect(SectKey, SectData);

	if((SectData.nStype == D_SECT_TYPE_USER && SectData.SectBefore.Shape == _T("GEN")) ||
				(SectData.nStype == D_SECT_TYPE_TAPERED && SectData.SectBefore.nStype==D_SECT_TYPE_USER && SectData.SectBefore.Shape == _T("GEN")))
	{//유저 input 타입이면서 General 타입이어야한다, 또는 테이퍼드단면이면서 USER 타입이어야하고 General 타입이어야한다
		if(SectData.SectBefore.SectI.aLine.GetSize()<=0)
		{//Plane Type이어야한다
			BOOL bColumn = TRUE;
			BOOL bConcrete = TRUE;
			CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
			T_ELEM_D ElemData;
			T_MATL_D DataMatl;

			m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList);
			for(int k=0 ; k<arElemKeyList.GetSize() ; k++)
			{
				if(m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[k], ElemData))
				{
					m_pDoc->m_pAttrCtrl->GetMatl(ElemData.elmat, DataMatl);
					int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[k]);
					if(ElemData.elpro == SectKey)
					{//해당 Elem의 Section키가 같은 것들 중에
						if(nMembType != 1)
						{//Column이 아닌것이 하나라도 있음 안된다
							bColumn = FALSE;
						}
						if(DataMatl.Type != _T("C"))
						{//Concrete가 아닌것이 하나라도 있으면 안된다
							bConcrete = FALSE;
						}
					}
				}
			}

			if(bColumn && bConcrete)
			{
				bResult = m_pDoc->m_pAttrCtrl->ExistRebt(SectKey);
			}
		}
	}

	return bResult;
}

void CRCColumnParameter::UpdateDgnData()
{
	T_ELEM_K    Key;
	T_RCCO_D    Data;

	CArray<T_RCCO_K, T_RCCO_K> arKey;
	CArray<T_RCCO_D, T_RCCO_D&> arData;
	arKey.RemoveAll();
	arData.RemoveAll();

	POSITION Pos = m_RccoData.GetStartPosition();

	while(Pos)
	{
		m_RccoData.GetNextAssoc(Pos,Key,Data);
		arKey.Add(Key);
		arData.Add(Data);	
	}
	m_pDoc->m_pDataCtrl->AddRcco(arKey,arData);
}


void CRCColumnParameter::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_ELEM_K cKey;
	m_ColumnGrid->GetCurrentCell(ncRow, ncCol);
	m_ColumnGrid->GetColumnNoFor(m_ColumnGrid->GetKeyColID(), nkCol);
	cKey = _ttol(m_ColumnGrid->GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_ELEM_K    Key;//, KeyBak;
	T_RCCO_D    Data, DataBak;

	BOOL bOldLock = m_ColumnGrid->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RCCO_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetRcco(nKey, Key, Data);
				m_ColumnGrid->InsertRecordByKey(Key, Data);
				break;
		case(UR_RCCO_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRcco(nKey, Key, Data);
				m_ColumnGrid->DeleteRecordByKey(Key, Data);
				break;
				/*
		case(UR_RCCO_MFD):
				// 여기서는 저장만 하고 UR_RCCO_MFS에서 처리한다.
				pViewBuff->GetRcco(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_RCCO_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetRcco(nKey, Key, Data);
				m_ColumnGrid->DeleteRecordByKey(Key, Data);
				m_ColumnGrid->InsertRecordByKey(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;*/
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_ColumnGrid->m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_ColumnGrid->m_nEditMode = CTBBrowserWnd::noMode;
		if (m_ColumnGrid->GetRowCount() < ncRow) ncRow = m_ColumnGrid->GetRowCount();
		BOOL bSet = m_ColumnGrid->SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = m_ColumnGrid->SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = m_ColumnGrid->SetLeftValidCell(ncRow, ncCol);
		m_ColumnGrid->m_nEditMode = nOldMode;
	}

	m_ColumnGrid->GetCurrentCell(ncRow, ncCol);
	m_ColumnGrid->GetColumnNoFor(m_ColumnGrid->GetKeyColID(), nkCol);
	T_ELEM_K newKey = _ttol(m_ColumnGrid->GetValueRowCol(ncRow, nkCol));
	if (newKey != cKey) m_ColumnGrid->OnChangeCurrentRecord(ncRow);

	m_ColumnGrid->AdjustAppendRowNo();
	m_ColumnGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_ColumnGrid->Redraw();  
}

void CRCColumnParameter::OnChangeCurrentRcco(T_ELEM_K key)
{
	m_pDoc->m_pAttrCtrl->GetRcco(key, m_Data);
	Data2Dlg();
}


void CRCColumnParameter::OnCheck2() 
{
	UpdateData(TRUE);

	if(m_bAutoBoundary)//횡구속여부 자동결정
	{
		m_Ctrl_EnableCtrl1.EnableWindow(false);
		m_Ctrl_EnableCtrl2.EnableWindow(false);
		m_Ctrl_iXDirBd.EnableWindow(false);
		m_Ctrl_iYDirBd.EnableWindow(false);
		m_Ctrl_iXDirBd_Braced.EnableWindow(false);
		m_Ctrl_iYDirBd_Braced.EnableWindow(false);

		//모멘트 확대 계수 자동결정
		GetDlgItem(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_CHK)->EnableWindow(true);
		OnAutoMomentEnlargeChk();

		//모멘트 확대 계수 자동결정
		GetDlgItem(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_NOCROSS_CHK)->EnableWindow(true);
		OnAutoMomentEnlargeNocrossChk();

		if(!m_bMomentEnlarge)//모멘트 확대계수 자동결정
		{
			m_Ctrl_Delta_Nsy.EnableWindow(true);
			m_Ctrl_Delta_Nsz.EnableWindow(true);
			m_Ctrl_Delta_Nsy_Edit.EnableWindow(true);
			m_Ctrl_Delta_Nsz_Edit.EnableWindow(true);
		}

		if(!m_bMomentEnlargeNocross)//모멘트 확대계수 자동결정(비횡구속)
		{
			m_Ctrl_Delta_Sy.EnableWindow(true);
			m_Ctrl_Delta_Sz.EnableWindow(true);
			m_Ctrl_Delta_Sy_Edit.EnableWindow(true);
			m_Ctrl_Delta_Sz_Edit.EnableWindow(true);
		}

		if(m_bMomentEnlarge && !m_bEquivalMoment)//등가모멘트 보정계수 자동결정
		{			
			m_Ctrl_Cmy.EnableWindow(true);
			m_Ctrl_Cmz.EnableWindow(true);
			m_Ctrl_CmyEdit.EnableWindow(true);
			m_Ctrl_CmzEdit.EnableWindow(true);
		}
	}
	else
	{
		m_Ctrl_EnableCtrl1.EnableWindow(true);
		m_Ctrl_EnableCtrl2.EnableWindow(true);
		m_Ctrl_iXDirBd.EnableWindow(true);
		m_Ctrl_iYDirBd.EnableWindow(true);
		m_Ctrl_iXDirBd_Braced.EnableWindow(true);
		m_Ctrl_iYDirBd_Braced.EnableWindow(true);

		OnAutoMomentEnlargeChk();
		OnAutoMomentEnlargeNocrossChk();
		YZDirectionAllSwayEnableControl();
	}	
}

// 계수하중조합과 사용하중 조합의 내용을 DB에 업데이트한다.
void CRCColumnParameter::OnBtnApply()   
{
	Dlg2Data_LoadCase();

	if(m_arStreLcomKey.IsEmpty() && m_arServLcomKey.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_DGN_LOADCASE_SELETE), MB_OK);
		m_bOnBtnApplyResult = FALSE;
		return;
	}
	
	T_RCCG_D RccgD;
	RccgD.Initialize();
	T_LCOM_K LcomK, GetLcomK;
	POSITION Pos = m_arStreLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arStreLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RccgD.StreLcom.Add(GetLcomK);
	}
	Pos = m_arServLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arServLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RccgD.ServLcom.Add(GetLcomK);
	}

	T_GRUP_K GrupK, GetGrupK;
	Pos = m_arGroupMapKey.GetStartPosition();
	while(Pos)
	{
		m_arGroupMapKey.GetNextAssoc(Pos,GrupK,GetGrupK);
		RccgD.ColGroupKey.Add(GetGrupK);
	}
	
	if(!m_pDoc->m_pDataCtrl->AddRccg(RccgD))
	{
		//
	}

	m_bOnBtnApplyResult = TRUE;
	return;
}



//LoadCase 체크유무 정보를 저장한다
void CRCColumnParameter::Dlg2Data_LoadCase() 
{
	CString sName;
	DWORD nID;

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_arGroupMapKey.RemoveAll();

	int nCount = m_ltbxLcomStre.GetCount();
		
	for (int k = 0; k < nCount ; k++)
	{
		if(m_ltbxLcomStre.GetCheck(k) == 1)
		{
			nID = m_ltbxLcomStre.GetItemData(k);
			T_LCOM_K LcomK = m_arLcomKey.GetAt(int(nID));
			m_arStreLcomKey.SetAt(LcomK,LcomK);
		}
	}

	nCount = m_ltbxLcomServ.GetCount();
	
	for (int j = 0; j < nCount ; j++)
	{
		if(m_ltbxLcomServ.GetCheck(j) == 1)
		{
			nID = m_ltbxLcomServ.GetItemData(j);
			T_LCOM_K LcomK = m_arLcomKey.GetAt(int(nID));
			m_arServLcomKey.SetAt(LcomK,LcomK);
		}
	}	

	nCount = m_ltbxGroup.GetCount();
	
	for (int c = 0; c < nCount ; c++)
	{
		if(m_ltbxGroup.GetCheck(c) == 1)
		{
			nID = m_ltbxGroup.GetItemData(c);
			T_GRUP_K GrupK = m_arGrupKey.GetAt(int(nID));
			m_arGroupMapKey.SetAt(GrupK,GrupK);
		}
	}
}

//부모 컨트롤에서 Apply버튼을 누를수 있도록
void CRCColumnParameter::Click_Btn_Apply() 
{
	OnBtnApply();
}

void CRCColumnParameter::OnDgnColumnCalcBtn() 
{
	// Add, Jaeoh. [11/4/2008]
	if(!m_pDoc->AllowCommand(D_OPTN_ID_MODS_SERVICE)) 
	{
		AfxMessageBox(_LS(IDS_DGN_MODS_Expriation_Message), MB_OK);
		return;    
	}

	OnBtnApply();

	if(!m_bOnBtnApplyResult)
		return;	

	if(m_ColumnGrid->GetRowCount() <= 1)
	{
		AfxMessageBox(_LS(IDS_DGN_NOT_MEMBER_INPUT_DATA), MB_OK);
		return;
	}

	IUsageCounter::Use(_T("RCDP"));

	// Add by sshan. MNET:2597.2523.('20070201) 로그분석에 Design 코드 추가
	IUsageCounter::Use(ReturnDgnConCode());

	//sshan RCS가 하위 프로젝트 이므로 직접 DLG클레스를 불러 쓸수없다.(Build가되지 않는다)
	//Service로 등록된 것을 불러서 Dlg를 띄운다.
	CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
	pHeadOffice->ReqService(_ULS(rcs), EN_RCSSA_CIVIL_RC_COLUMN_CALC_DLG, NULL);	  
}

void CRCColumnParameter::LoadDBtoDlg() 
{
	CArray<T_RCCO_K, T_RCCO_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRccoKeyList(KeyList);

	m_RccoData.RemoveAll();
	T_RCCO_D Data;
	
	int KeyListSize = KeyList.GetSize();
	for(int i=0 ; i<KeyListSize ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetRcco(KeyList[i], Data))
			m_RccoData.SetAt(KeyList[i], Data);
	}
	
	addDelMod = 0;
	
	if(KeyList.GetSize()>0)
		OnChangeCurrentRcco(KeyList[0]);
}

BOOL CRCColumnParameter::DataCheck() 
{
	CString ErrorMessanges;
	if(!m_bIEnd && !m_bJEnd && !m_bMid)
		ErrorMessanges = _LS(IDS_DGN_MUST_ONE_OVER_PART_NUMBER);//_T("Part Number를 한가지 이상 선택하셔야 합니다.")

	if(m_chkCTC.GetCheck())
	{
		if(m_editCTC.GetEditValue() <= 0)//m_editCTC는 0보다 커야된다
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_CTC_ERROR_1);//_T("CTC값은 0보다 큰값이어야 합니다.")
			else
				ErrorMessanges += _LS(IDS_DGN_CTC_ERROR_2);//_T("\nCTC값은 0보다 큰값이어야 합니다.")
		}
	}

	if(m_dRhomax <= 0 || m_dRhomax >= 1.0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_RHO_MAX_ERROR_1);//_T("rho_max값은 0보다 크거나 1보다 작은값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_RHO_MAX_ERROR_2);//_T("\nrho_max값은 0보다 크거나 1보다 작은값이어야 합니다.")
	}	

	if(m_Dc.GetEditValue()<=0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DC_ERROR_1);//_T("Dc값은 0보다 큰값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_DC_ERROR_2);//_T("\nDc값은 0보다 큰값이어야 합니다.")
	}

	if(m_dCmy <= 0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_CMY_ERROR_1);//_T("Cmy값은 0보다 큰값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_CMY_ERROR_2);//_T("\nCmy값은 0보다 큰값이어야 합니다.")
	}

	if(m_dCmz <= 0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_CMZ_ERROR_1);//_T("Cmz값은 0보다 큰값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_CMZ_ERROR_2);//_T("\nCmz값은 0보다 큰값이어야 합니다.")
	}

	if(m_dDeltla_Nsy < 1) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DELTA_NSY_ERROR_1);//_T("delta_nsy값은 1보다 큰값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_DELTA_NSY_ERROR_2);//_T("\ndelta_nsy값은 1보다 큰값이어야 합니다.")
	}

	if(m_dDeltla_Nsz < 1) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DELTA_NSZ_ERROR_1);//_T("delta_nsz값은 1보다 큰값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_DELTA_NSZ_ERROR_2);//_T("\ndelta_nsz값은 1보다 큰값이어야 합니다.")
	}

	if(m_dDeltla_Sy < 1) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DELTA_SY_ERROR_1);//_T("delta_sy값은 1보다 큰값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_DELTA_SY_ERROR_2);//_T("\ndelta_sy값은 1보다 큰값이어야 합니다.")
	}

	if(m_dDeltla_Sz < 1) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DELTA_SZ_ERROR_1);//_T("delta_sz값은 1보다 큰값이어야 합니다.")
		else
			ErrorMessanges += _LS(IDS_DGN_DELTA_SZ_ERROR_2);//_T("\ndelta_sz값은 1보다 큰값이어야 합니다.")
	}	

	CString strTemp = _T("");
	if(!m_chkCroseBd.GetCheck())
	{
		m_editBetaDCroseY.GetWindowText(strTemp);
		if(_tstof(strTemp) < 0)//m_editBetaDCroseY는 0보다 작으면 안됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_BETAD_CROSS_Y_ERROR_1);//_T("beta_d_Y값은 0과 같거나 큰값이어야 합니다.")
			else
				ErrorMessanges += _LS(IDS_DGN_BETAD_CROSS_Y_ERROR_2);//_T("\beta_d_Y값은 0과 같거나 큰값이어야 합니다.")
		}
		m_editBetaDCroseZ.GetWindowText(strTemp);
		if(_tstof(strTemp) < 0)//m_editBetaDCroseZ는 0보다 작으면 안됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_BETAD_CROSS_Z_ERROR_1);//_T("beta_d_Z값은 0과 같거나 큰값이어야 합니다.")
			else
				ErrorMessanges += _LS(IDS_DGN_BETAD_CROSS_Z_ERROR_2);//_T("\beta_d_Z값은 0과 같거나 큰값이어야 합니다.")
		}
	}

	if(!m_chkNoCroseBd.GetCheck())
	{
		m_editBetaDNoCroseY.GetWindowText(strTemp);
		if(_tstof(strTemp) < 0)//m_editBetaDNoCroseY 는 0보다 작으면 안됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_BETAD_CROSS_Y_ERROR_1);//_T("beta_d_Y값은 0과 같거나 큰값이어야 합니다.")
			else
				ErrorMessanges += _LS(IDS_DGN_BETAD_CROSS_Y_ERROR_2);//_T("\beta_d_Y값은 0과 같거나 큰값이어야 합니다.")
		}
		m_editBetaDNoCroseZ.GetWindowText(strTemp);
		if(_tstof(strTemp) < 0)//m_editBetaDNoCroseZ 는 0보다 작으면 안됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_BETAD_CROSS_Z_ERROR_1);//_T("beta_d_Z값은 0과 같거나 큰값이어야 합니다.")
			else
				ErrorMessanges += _LS(IDS_DGN_BETAD_CROSS_Z_ERROR_2);//_T("\beta_d_Z값은 0과 같거나 큰값이어야 합니다.")
		}
	}

	if(m_chkRfactor.GetCheck())
	{
		if(m_editRfactorY.GetEditValue() <= 0)//R_y 는 0과 같거나 커야됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_R_Y_ERROR_1);//_T("R_y값은 0 보다 큰값이어야 합니다.
			else
				ErrorMessanges += _LS(IDS_DGN_R_Y_ERROR_2);//")\nR_y값은 0 보다 큰값이어야 합니다.
		}
		if(m_editRfactorZ.GetEditValue() <= 0)//R_z 는 0과 같거나 커야됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_R_Z_ERROR_1);//_T("R_z값은 0 보다 큰값이어야 합니다.
			else
				ErrorMessanges += _LS(IDS_DGN_R_Z_ERROR_2);//")\nR_z값은 0 보다 큰값이어야 합니다.
		}
	}
	
	
	if(!ErrorMessanges.GetLength()==0)
	{
		AfxMessageBox(ErrorMessanges, MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CRCColumnParameter::OnAutoEquivalentMomentChk() 
{
	UpdateData(TRUE);

	// Change by SHIN.(07.08.23) Sway상태에서도 횡구속 변수를 입력받도록 변경함		
	if(m_bEquivalMoment)//등가모멘트 보정계수 자동결정
	{
		m_Ctrl_Cmy.EnableWindow(false);
		m_Ctrl_Cmz.EnableWindow(false);
		m_Ctrl_CmyEdit.EnableWindow(false);
		m_Ctrl_CmzEdit.EnableWindow(false);		
	}
	else
	{
		m_Ctrl_Cmy.EnableWindow(true);
		m_Ctrl_Cmz.EnableWindow(true);
		m_Ctrl_CmyEdit.EnableWindow(true);
		m_Ctrl_CmzEdit.EnableWindow(true);						
	}		
}

void CRCColumnParameter::OnAutoMomentEnlargeNocrossChk() 
{
	UpdateData(TRUE);

	if(m_bMomentEnlargeNocross)//모멘트 확대계수 자동결정(비횡구속)
	{
		m_Ctrl_Delta_Sy.EnableWindow(false);
		m_Ctrl_Delta_Sz.EnableWindow(false);
		m_Ctrl_Delta_Sy_Edit.EnableWindow(false);
		m_Ctrl_Delta_Sz_Edit.EnableWindow(false);		
	}
	else
	{
		m_Ctrl_Delta_Sy.EnableWindow(true);
		m_Ctrl_Delta_Sz.EnableWindow(true);
		m_Ctrl_Delta_Sy_Edit.EnableWindow(true);
		m_Ctrl_Delta_Sz_Edit.EnableWindow(true);
		
		if(!m_bAutoBoundary && m_iXDirBd == 1)
		{
			m_Ctrl_Delta_Sy.EnableWindow(false);
			m_Ctrl_Delta_Sy_Edit.EnableWindow(false);
		}
		if(!m_bAutoBoundary && m_iYDirBd == 1)
		{
			m_Ctrl_Delta_Sz.EnableWindow(false);
			m_Ctrl_Delta_Sz_Edit.EnableWindow(false);
		}
	}	
}

void CRCColumnParameter::OnAutoMomentEnlargeChk() 
{
	UpdateData(TRUE);

	// Change by SHIN.(07.08.23) Sway상태에서도 횡구속 변수를 입력받도록 변경함
	if(m_bMomentEnlarge)//모멘트 확대계수 자동 결정
	{
		m_Ctrl_Delta_Nsy.EnableWindow(false);
		m_Ctrl_Delta_Nsz.EnableWindow(false);
		m_Ctrl_Delta_Nsy_Edit.EnableWindow(false);
		m_Ctrl_Delta_Nsz_Edit.EnableWindow(false);

		m_Ctrl_Auto_EquivalentMoment_Check.EnableWindow(true);
		OnAutoEquivalentMomentChk();
	}
	else
	{
		m_Ctrl_Delta_Nsy.EnableWindow(true);
		m_Ctrl_Delta_Nsz.EnableWindow(true);
		m_Ctrl_Delta_Nsy_Edit.EnableWindow(true);
		m_Ctrl_Delta_Nsz_Edit.EnableWindow(true);
		
		m_Ctrl_Auto_EquivalentMoment_Check.EnableWindow(false);
		m_Ctrl_Cmy.EnableWindow(false);
		m_Ctrl_Cmz.EnableWindow(false);
		m_Ctrl_CmyEdit.EnableWindow(false);
		m_Ctrl_CmzEdit.EnableWindow(false);	
	}		
}

void CRCColumnParameter::OnClickYDirectionRadioBtn() 
{
	UpdateData(TRUE);

	if(m_iXDirBd == 0)//y-Direction _T("Sway") Select
	{
		// Change by SHIN.(07.08.23) Sway상태에서도 횡구속 변수를 입력받도록 변경함
		if(!m_bMomentEnlarge)//모멘트 확대계수 자동 결정
		{
			m_Ctrl_Delta_Nsy.EnableWindow(true);
			m_Ctrl_Delta_Nsy_Edit.EnableWindow(true);
		}
		if(m_bMomentEnlarge && !m_bEquivalMoment)
		{
			m_Ctrl_Cmy.EnableWindow(true);
			m_Ctrl_CmyEdit.EnableWindow(true);		
		}

		if(!m_bMomentEnlargeNocross)//모멘트 확대계수 자동결정(비횡구속)
		{
			m_Ctrl_Delta_Sy.EnableWindow(true);
			m_Ctrl_Delta_Sy_Edit.EnableWindow(true);
		}
	}
	else//y-Direction _T("NonSway") Select
	{
		if(!m_bMomentEnlarge)//모멘트 확대계수 자동 결정
		{
			m_Ctrl_Delta_Nsy.EnableWindow(true);
			m_Ctrl_Delta_Nsy_Edit.EnableWindow(true);
		}
		if(m_bMomentEnlarge && !m_bEquivalMoment)
		{
			m_Ctrl_Cmy.EnableWindow(true);
			m_Ctrl_CmyEdit.EnableWindow(true);		
		}

		m_Ctrl_Delta_Sy.EnableWindow(false);
		m_Ctrl_Delta_Sy_Edit.EnableWindow(false);
	}		

	YZDirectionAllSwayEnableControl();
}

void CRCColumnParameter::OnClickZDirectionRadioBtn() 
{
	UpdateData(TRUE);

	if(m_iYDirBd == 0)//z-Direction _T("Sway") Select
	{
		// Change by SHIN.(07.08.23) Sway상태에서도 횡구속 변수를 입력받도록 변경함
		if(!m_bMomentEnlarge)//모멘트 확대계수 자동 결정
		{
			m_Ctrl_Delta_Nsz.EnableWindow(true);
			m_Ctrl_Delta_Nsz_Edit.EnableWindow(true);
		}
		if(m_bMomentEnlarge && !m_bEquivalMoment)
		{
			m_Ctrl_Cmz.EnableWindow(true);
			m_Ctrl_CmzEdit.EnableWindow(true);		
		}

		if(!m_bMomentEnlargeNocross)//모멘트 확대계수 자동결정(비횡구속)
		{
			m_Ctrl_Delta_Sz.EnableWindow(true);
			m_Ctrl_Delta_Sz_Edit.EnableWindow(true);
		}
	}
	else//z-Direction _T("NonSway") Select
	{
		if(!m_bMomentEnlarge)//모멘트 확대계수 자동 결정
		{
			m_Ctrl_Delta_Nsz.EnableWindow(true);
			m_Ctrl_Delta_Nsz_Edit.EnableWindow(true);
		}
		if(m_bMomentEnlarge && !m_bEquivalMoment)
		{
			m_Ctrl_Cmz.EnableWindow(true);
			m_Ctrl_CmzEdit.EnableWindow(true);		
		}

		m_Ctrl_Delta_Sz.EnableWindow(false);
		m_Ctrl_Delta_Sz_Edit.EnableWindow(false);
	}	
	
	YZDirectionAllSwayEnableControl();
}

void CRCColumnParameter::YZDirectionAllSwayEnableControl() 
{
	UpdateData(TRUE);

	if(m_iXDirBd==0 && m_iYDirBd==0)//y-Direction _T("Sway") Select AND z-Direction _T("Sway") Select
	{
		// Change by SHIN.(07.08.23) Sway상태에서도 횡구속 변수를 입력받도록 변경함

		//모멘트 확대 계수 자동결정						
		GetDlgItem(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_CHK)->EnableWindow(true);
		//등가모멘트 보정계수 자동결정
		if(m_bMomentEnlarge)
			GetDlgItem(IDC_DGN_COLUMN_AUTO_EQUIVALENT_MOMENT_CHK)->EnableWindow(true);
		else
			GetDlgItem(IDC_DGN_COLUMN_AUTO_EQUIVALENT_MOMENT_CHK)->EnableWindow(false);

		//모멘트 확대계수 자동결정(비횡구속)
		GetDlgItem(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_NOCROSS_CHK)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_CHK)->EnableWindow(true);

		if(m_bMomentEnlarge)
			GetDlgItem(IDC_DGN_COLUMN_AUTO_EQUIVALENT_MOMENT_CHK)->EnableWindow(true);
		else
			GetDlgItem(IDC_DGN_COLUMN_AUTO_EQUIVALENT_MOMENT_CHK)->EnableWindow(false);

		if(m_iXDirBd==1 && m_iYDirBd==1)
			GetDlgItem(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_NOCROSS_CHK)->EnableWindow(false);
		else
			GetDlgItem(IDC_DGN_COLUMN_AUTO_MOMENT_ENLARGE_NOCROSS_CHK)->EnableWindow(true);
	}
}



void CRCColumnParameter::OnDgnColumnModifyConcreteMaterialBtn() 
{
	CDgnConMatDlg dlg;
	dlg.DoModal();
}

void CRCColumnParameter::OnDgnColumnUnbracedLengthBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_LENG__);	
}

void CRCColumnParameter::OnDgnColumnEffectiveLengthBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_KFAC__);	
}

void CRCColumnParameter::EnableCalcBtn(bool Enable) 
{
	GetDlgItem(IDC_DGN_COLUMN_CALC_BTN)->EnableWindow(Enable);
}

void CRCColumnParameter::OnClickChkCTC() 
{
	if(m_chkCTC.GetCheck())
	{
		GetDlgItem(IDC_DGN_CTC_STATIC)->EnableWindow(TRUE);
		m_unitCTC.EnableWindow(TRUE);
		m_editCTC.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CTC_STATIC)->EnableWindow(FALSE);
		m_unitCTC.EnableWindow(FALSE);
		m_editCTC.EnableWindow(FALSE);
	}
}

void CRCColumnParameter::OnClickChkCroseBd() 
{
	// Add by GAY. MNET:2679. ('07.07.07).
	if(m_chkCroseBd.GetCheck())
	{
		GetDlgItem(IDC_DGN_CROSS_BETAD_Y_STATIC)->EnableWindow(TRUE);
		m_editBetaDCroseY.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_CROSS_BETAD_Z_STATIC)->EnableWindow(TRUE);
		m_editBetaDCroseZ.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_CROSS_BETAD_Y_STATIC)->EnableWindow(FALSE);
		m_editBetaDCroseY.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_CROSS_BETAD_Z_STATIC)->EnableWindow(FALSE);
		m_editBetaDCroseZ.EnableWindow(FALSE);
	}
}

void CRCColumnParameter::OnClickChkNoCroseBd() 
{
	// Add by GAY. MNET:2679. ('07.07.07).
	if(m_chkNoCroseBd.GetCheck())
	{
		GetDlgItem(IDC_DGN_NOCROSS_BETAD_Y_STATIC)->EnableWindow(TRUE);
		m_editBetaDNoCroseY.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_NOCROSS_BETAD_Z_STATIC)->EnableWindow(TRUE);
		m_editBetaDNoCroseZ.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_NOCROSS_BETAD_Y_STATIC)->EnableWindow(FALSE);
		m_editBetaDNoCroseY.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_NOCROSS_BETAD_Z_STATIC)->EnableWindow(FALSE);
		m_editBetaDNoCroseZ.EnableWindow(FALSE);
	}
}

void CRCColumnParameter::OnClickRfactor() 
{
	if(m_chkRfactor.GetCheck())
	{
		GetDlgItem(IDC_DGN_COLUMN_R_Y_STATIC)->EnableWindow(TRUE);
		m_editRfactorY.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_COLUMN_R_Z_STATIC)->EnableWindow(TRUE);
		m_editRfactorZ.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DGN_COLUMN_R_Y_STATIC)->EnableWindow(FALSE);
		m_editRfactorY.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_COLUMN_R_Z_STATIC)->EnableWindow(FALSE);
		m_editRfactorZ.EnableWindow(FALSE);
	}
}


// PARKHJ-20111007 : 번호 할당시 DgnStruct.h 참고!
CString CRCColumnParameter::ReturnDgnConCode()
{
	CString strCodeNumber = _T("C_CON");

	T_DCON_D DconD;
	m_pDoc->m_pAttrCtrl->GetDcon(DconD);
	
	return CDgnCodeCtrl::GetConCodeNumber4Log(strCodeNumber, DconD.DesignCode);
}