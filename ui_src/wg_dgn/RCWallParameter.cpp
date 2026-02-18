// RCWallParameter.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCWallParameter.h"

#include "RCDesignParamDlg.h"

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

#include "..\wg_base\TreeMenuMode.h"
#include "..\wg_base\TreeMenuBarBase.h"

#include "..\wg_rcs\Rcs_ServiceDef.h"
#include "..\wg_base\ServiceProvider.h"

// Coded by sshan('20061108)
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_dgn\DgnCodeCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCWallParameter dialog


CRCWallParameter::CRCWallParameter(CWnd* pParent /*=NULL*/)
	: CChildDialog(CRCWallParameter::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRCWallParameter)
	m_SelectElemString = _T("");
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CRCDesignParamDlg*)pParent;

	m_Data.Initialize();  

	m_WallGrid = new CDgnRCWallGrid();

	m_pDoc = CDBDoc::GetDocPoint();	
	m_pMembCtrl = CMembCtrl::GetMembCtrl();

	m_RcwaData.InitHashTable(100);

	m_strConCode = CDBLib::GetConCodeName();
}

CRCWallParameter::~CRCWallParameter()
{
	if(m_WallGrid != NULL)
	{
		delete m_WallGrid;
		m_WallGrid = NULL;
	}

	m_arElemK.RemoveAll();	
	m_arLcomKey.RemoveAll();
	m_arGrupKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_pBadKeyList.RemoveAll();
	m_pBadMemberKeyList.RemoveAll();

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_arGroupMapKey.RemoveAll();
	m_RcwaData.RemoveAll();
}


void CRCWallParameter::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCWallParameter)
	DDX_Control(pDX, IDC_DGN_WALL_SHORT_CHECK_RADIO,								m_radioShort);
	DDX_Control(pDX, IDC_DGN_WALL_LONG_SHORT_RADIO,									m_radioLongShort);
	DDX_Control(pDX, IDC_DGN_WALL_CLOSS_BINDING_CHECK,							m_chkAutoSway);
	DDX_Control(pDX, IDC_DGN_WALL_SWAY_RADIO,												m_radioSway);
	DDX_Control(pDX, IDC_DGN_WALL_NON_SWAY_RADIO,										m_radioNonSway);
	DDX_Control(pDX, IDC_DGN_WALL_AUTO_MOMENT_ENLARGE_CHK,					m_chkAutoMomentEnlarge);
	DDX_Control(pDX, IDC_DGN_WALL_AUTO_EQUIVALENT_MOMENT_CHK,				m_chkAutoMomentEquivalent);
	DDX_Control(pDX, IDC_DGN_WALL_AUTO_MOMENT_ENLARGE_NOCROSS_CHK,	m_chkAutoMomentExpansion);
	DDX_Control(pDX, IDC_DGN_WALL_DELTA_NS_EDIT,										m_editDeltaNs);
	DDX_Control(pDX, IDC_DGN_WALL_CM_EDIT,													m_editCm);
	DDX_Control(pDX, IDC_DGN_WALL_DELTA_S_EDIT,											m_editDeltaS);
	DDX_Control(pDX, IDC_DGN_WALL_CRACK_WIDTH_COMBO,								m_comboCrackWidth);
	DDX_Control(pDX, IDC_DGN_WALL_INCLUDE_SBAR_CHK,									m_chkIncludeShear);
	DDX_Control(pDX, IDC_DGN_WALL_AUTO_REBAR_POSI_CHK,							m_chkAutoRebarPosi);
	DDX_Control(pDX, IDC_DGN_WALL_REBAR_TOP_REBAR_RATIO_EDIT,				m_editTopRebarRatio);
	DDX_Control(pDX, IDC_DGN_WALL_MAX_REBAR_RATIO_CHK,							m_chkMaxRebarRatio);
	DDX_Control(pDX, IDC_DGN_WALL_PMAX_EDIT,												m_editPMax);
	DDX_Control(pDX, IDC_DGN_WALL_REBAR_DT_EDIT,										m_editDt);
	DDX_Control(pDX, IDC_DGN_WALL_REBAR_DB_EDIT,										m_editDb);
	DDX_Control(pDX, IDC_DGN_WALL_HAUNCH_HEIGH_I_EDIT,							m_editHeightI);
	DDX_Control(pDX, IDC_DGN_WALL_HAUNCH_HEIGH_J_EDIT,							m_editHeightJ);
	DDX_Control(pDX, IDC_DGN_WALL_REBAR_DT_UNIT,										m_unitDt);
	DDX_Control(pDX, IDC_DGN_WALL_REBAR_DB_UNIT,										m_unitDb);
	DDX_Control(pDX, IDC_DGN_WALL_HAUNCH_HEIGH_I_UNIT,							m_unitHeightI);
	DDX_Control(pDX, IDC_DGN_WALL_HAUNCH_HEIGH_J_UNIT,							m_unitHeightJ);
	DDX_Control(pDX, IDC_DGN_WALL_IEND_CHK,													m_chkIEnd);
	DDX_Control(pDX, IDC_DGN_WALL_MID_CHK,													m_chkMid);
	DDX_Control(pDX, IDC_DGN_WALL_JEND_CHK,													m_chkJEnd);
	DDX_Control(pDX, IDC_DGN_WALL_EDIT_POSITION,										m_editEtc);
	DDX_Control(pDX, IDC_DGN_WALL_EDIT_ELEMENT,											m_editSelectElem);
	DDX_Control(pDX, IDC_DGN_WALL_LIST_LCOM_SERVICE,								m_ltbxLcomServ);
	DDX_Control(pDX, IDC_DGN_WALL_LIST_LCOM_STRENGTH,								m_ltbxLcomStre);
	DDX_Control(pDX, IDC_DGN_WALL_LIST_GROUP,												m_ltbxGroup);
	DDX_Text(pDX, IDC_DGN_WALL_EDIT_ELEMENT, m_SelectElemString);
	DDX_Control(pDX, IDC_DGN_WALL_AUTO_BD_CROSS_CHK,								m_chkCroseBd);
	DDX_Control(pDX, IDC_DGN_WALL_AUTO_BD_NOCROSS_CHK,							m_chkNoCroseBd);
	DDX_Control(pDX, IDC_DGN_WALL_EDIT_BD_CROSS,										m_editBetaDCrose);
	DDX_Control(pDX, IDC_DGN_WALL_EDIT_BD_NOCROSS,									m_editBetaDNoCrose);
	DDX_Control(pDX, IDC_DGN_WALL_GRID, *m_WallGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRCWallParameter, CChildDialog)
	//{{AFX_MSG_MAP(CRCWallParameter)
	ON_BN_CLICKED(IDC_DGN_WALL_BTN_ADD, OnAddData)
	ON_BN_CLICKED(IDC_DGN_WALL_BTN_MODIFY, OnModifyData)
	ON_BN_CLICKED(IDC_DGN_WALL_BTN_DELETE, OnDeleteData)
	ON_BN_CLICKED(IDC_DGN_WALL_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_DGN_WALL_CALC_BTN, OnDgnWallCalcBtn)	
	ON_BN_CLICKED(IDC_DGN_WALL_UNBRACED_LENGTH_BTN, OnDgnUnbracedLengthBtn)	
	ON_BN_CLICKED(IDC_DGN_WALL_EFFECTIVE_LENGTH_BTN, OnDgnEffectiveLengthBtn)
	ON_BN_CLICKED(IDC_DGN_WALL_CLOSS_BINDING_CHECK, OnDgnChkClossBinding)	
	ON_BN_CLICKED(IDC_DGN_WALL_AUTO_MOMENT_ENLARGE_CHK, OnDgnChkAutoMomentEnlarge)	
	ON_BN_CLICKED(IDC_DGN_WALL_AUTO_EQUIVALENT_MOMENT_CHK, OnDgnChkAutoEquivalentMoment)
	ON_BN_CLICKED(IDC_DGN_WALL_AUTO_MOMENT_ENLARGE_NOCROSS_CHK, OnDgnChkAutoMomentEnlargeNocress)
	ON_BN_CLICKED(IDC_DGN_WALL_SHORT_CHECK_RADIO, OnDgnRadioLongShort)
	ON_BN_CLICKED(IDC_DGN_WALL_LONG_SHORT_RADIO, OnDgnRadioLongShort)
	ON_BN_CLICKED(IDC_DGN_WALL_SWAY_RADIO, OnDgnRadioSwayNonsway)
	ON_BN_CLICKED(IDC_DGN_WALL_NON_SWAY_RADIO, OnDgnRadioSwayNonsway)
	ON_BN_CLICKED(IDC_DGN_WALL_AUTO_REBAR_POSI_CHK, OnDgnChkAutoRebarPosi)
	ON_BN_CLICKED(IDC_DGN_WALL_MAX_REBAR_RATIO_CHK, OnDgnChkRhoMax)
	ON_BN_CLICKED(IDC_DGN_WALL_AUTO_BD_CROSS_CHK, OnClickChkCroseBd)
	ON_BN_CLICKED(IDC_DGN_WALL_AUTO_BD_NOCROSS_CHK, OnClickChkNoCroseBd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRCWallParameter message handlers

BOOL CRCWallParameter::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
		
	// TODO: Add extra initialization here  

	m_WallGrid->m_RcwaData.RemoveAll();
	m_WallGrid->Initialize(this);
	
	ASSERT(m_pDoc);

	// Unit.
	SetInitUnit();
	// Combo.
	InitialCboCrackWidth();
	// Lcom.
	InitialLcomDataForDgn();

	InitEnableColtrol();
	
	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);  

	LoadDBtoDlg();

	Data2Dlg();
	OnDgnRadioLongShort();
	//OnDgnChkClossBinding();
	OnDgnChkAutoRebarPosi();
	OnDgnChkRhoMax();
	  
	//해석되어 있지 않으면 계산버튼 비활성화
	if(!m_pDoc->IsPostMode())
		GetDlgItem(IDC_DGN_WALL_CALC_BTN)->EnableWindow(false);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CRCWallParameter::DestroyWindow() 
{	
	m_arElemK.RemoveAll();
	m_arLcomKey.RemoveAll();
	m_arGrupKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_pBadKeyList.RemoveAll();
	m_pBadMemberKeyList.RemoveAll();

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_arGroupMapKey.RemoveAll();
	m_RcwaData.RemoveAll();

	return CChildDialog::DestroyWindow();
}

void CRCWallParameter::InitialData()
{
	m_arLcomKey.RemoveAll();
}

void CRCWallParameter::InitEnableColtrol()
{
	m_strConCode = CDBLib::GetConCodeName();

	if(m_strConCode == _T("KCI-USD07"))
	{
		GetDlgItem(IDC_DGN_WALL_CRACK_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_CRACK_WIDTH_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_INCLUDE_SBAR_CHK)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALL_CRACK_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_CRACK_WIDTH_COMBO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_INCLUDE_SBAR_CHK)->EnableWindow(TRUE);
	}
}

BOOL CRCWallParameter::Data2Dlg()
{
	CArray<long, long> caKey;
	m_SelectElemString.Empty();

	m_WallGrid->GetSelectedKeys(caKey);
	int cakeySize = caKey.GetSize();
	if(cakeySize>0)
	{
		CString TempString;		
		for(int i=0 ; i<cakeySize ; i++)
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
		if(m_WallGrid->GetCurrentRcwa(aKey))
			m_SelectElemString.Format(_T("%d"), aKey);
	}

	CString strTemp = _T("");
	//검토조건
	if(m_Data.iSlenderType == 0)
	{
		m_radioShort.SetCheck(TRUE);
		m_radioLongShort.SetCheck(FALSE);
	}
	else
	{
		m_radioShort.SetCheck(FALSE);
		m_radioLongShort.SetCheck(TRUE);
	}

	//횡구속여부 자동결정
	m_chkAutoSway.SetCheck(m_Data.bAutoBoundary);
	if(m_Data.iXDirBd == 0)
	{
		m_radioSway.SetCheck(TRUE);
		m_radioNonSway.SetCheck(FALSE);
	}
	else
	{
		m_radioSway.SetCheck(FALSE);
		m_radioNonSway.SetCheck(TRUE);
	}

	//횡구속변수
	m_chkAutoMomentEnlarge.SetCheck(m_Data.bAutoDeltans);
	m_chkAutoMomentEquivalent.SetCheck(m_Data.bAutoEqCoeff);
	strTemp.Format(_T("%.3f"), m_Data.dDelta_ns);
	m_editDeltaNs.SetWindowText(strTemp);
	strTemp.Format(_T("%.3f"), m_Data.dCm);
	m_editCm.SetWindowText(strTemp);
	//비횡구속변수
	m_chkAutoMomentExpansion.SetCheck(m_Data.bAutoDeltas);
	strTemp.Format(_T("%.3f"), m_Data.dDelta_s);
	m_editDeltaS.SetWindowText(strTemp);
	//사용성검토조건
	if(m_strConCode != _T("KCI-USD07"))//_T("KCI-USD07")이면 불러오지 않는다
	{
		m_comboCrackWidth.SetCurSel(m_Data.iServCond);
		m_chkIncludeShear.SetCheck(m_Data.bStirrup);
	}
	//필요철근량 산출시 전면,배면 철근비율 
	m_chkAutoRebarPosi.SetCheck(m_Data.bAutoRebarPosi);
	strTemp.Format(_T("%.2f"), m_Data.dTopRebarRatio*100.0);// 0~1의 값을 %값으로 변경
	m_editTopRebarRatio.SetWindowText(strTemp);
	//최대철근비검토
	m_chkMaxRebarRatio.SetCheck(m_Data.bRhomax);
	strTemp.Format(_T("%.3f"), m_Data.dRhomax);
	m_editPMax.SetWindowText(strTemp);
	//주철근위치
	m_editDt.SetEditUnit(m_Data.dDt);
	m_editDb.SetEditUnit(m_Data.dDb);
	//헌치높이
	m_editHeightI.SetEditUnit(m_Data.dHaunch_hi);
	m_editHeightJ.SetEditUnit(m_Data.dHaunch_hj);
	//Part
	m_chkIEnd.SetCheck(m_Data.bPosi[0]);
	m_chkMid.SetCheck(m_Data.bPosi[1]);
	m_chkJEnd.SetCheck(m_Data.bPosi[2]);	
	//Bd
	m_chkCroseBd.SetCheck(m_Data.bUserBd_NS);
	m_chkNoCroseBd.SetCheck(m_Data.bUserBd_S);
	strTemp.Format(_T("%.3f"), m_Data.dBd_nsy);
	m_editBetaDCrose.SetWindowText(strTemp);
	strTemp.Format(_T("%.3f"), m_Data.dBd_sy);
	m_editBetaDNoCrose.SetWindowText(strTemp);

	m_editEtc.SetWindowText(m_Data.strRemark);

	UpdateData(FALSE);

	m_editSelectElem.SelectByStr(m_SelectElemString);

	OnDgnRadioLongShort();	
	OnDgnChkAutoRebarPosi();
	OnDgnChkRhoMax();

	return TRUE;
}

BOOL CRCWallParameter::Dlg2Data()
{
	m_Data.Initialize();    

	m_arElemK.RemoveAll();  

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(m_arElemK);

	m_pParent->UpdateData(TRUE);
	
	UpdateData(TRUE);
	if(addDelMod != 2)//Delete일 경우는 데이터 체크를 하지않는다
		if(!DataCheck()) return FALSE;
	
	m_RcwaData.RemoveAll();   

 	// Strength LoadCombination.
	CString sName;
	DWORD nID;
	int nCount = m_ltbxLcomStre.GetCount();
	//int *aIndex = new int[nCount];
	
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
		
	CString strTemp = _T("");
	//검토조건
	if(m_radioShort.GetCheck())
		m_Data.iSlenderType = 0;
	else if(m_radioLongShort.GetCheck())
		m_Data.iSlenderType = 1;
	else
		ASSERT(0);
	//횡구속여부 자동결정
	m_Data.bAutoBoundary = m_chkAutoSway.GetCheck();
	if(m_radioSway.GetCheck())
		m_Data.iXDirBd = 0;
	else if(m_radioNonSway.GetCheck())
		m_Data.iXDirBd = 1;
	else
		ASSERT(0);
	//횡구속변수
	m_Data.bAutoDeltans = m_chkAutoMomentEnlarge.GetCheck();
	m_Data.bAutoEqCoeff = m_chkAutoMomentEquivalent.GetCheck();
	m_editDeltaNs.GetWindowText(strTemp);
	m_Data.dDelta_ns = _tstof(strTemp);
	m_editCm.GetWindowText(strTemp);
	m_Data.dCm = _tstof(strTemp);
	//비횡구속변수
	m_Data.bAutoDeltas = m_chkAutoMomentExpansion.GetCheck();
	m_editDeltaS.GetWindowText(strTemp);
	m_Data.dDelta_s = _tstof(strTemp);	
	//사용성검토조건
	if(m_strConCode != _T("KCI-USD07"))//_T("KCI-USD07")이면 저장하지 않는다
	{
		m_Data.iServCond = m_comboCrackWidth.GetCurSel();
		m_Data.bStirrup = m_chkIncludeShear.GetCheck();	
	}
	//필요철근량 산출시 전면,배면 철근비율 
	m_Data.bAutoRebarPosi = m_chkAutoRebarPosi.GetCheck();	
	m_editTopRebarRatio.GetWindowText(strTemp);
	m_Data.dTopRebarRatio = _tstof(strTemp)/100.0;// %값을 0~1의 값으로 변경		
	//최대철근비검토
	m_Data.bRhomax = m_chkMaxRebarRatio.GetCheck();	
	m_editPMax.GetWindowText(strTemp);
	m_Data.dRhomax = _tstof(strTemp);		
	//주철근위치
	m_Data.dDt = m_editDt.GetEditValue();
	m_Data.dDb = m_editDb.GetEditValue();
	//헌치높이
	m_Data.dHaunch_hi = m_editHeightI.GetEditValue();
	m_Data.dHaunch_hj = m_editHeightJ.GetEditValue();
	//Part
	m_Data.bPosi[0] = m_chkIEnd.GetCheck();
	m_Data.bPosi[1] = m_chkMid.GetCheck();
	m_Data.bPosi[2] = m_chkJEnd.GetCheck();

	m_editEtc.GetWindowText(m_Data.strRemark);

	//Bd
	m_Data.bUserBd_NS = m_chkCroseBd.GetCheck();
	m_Data.bUserBd_S = m_chkNoCroseBd.GetCheck();
	m_editBetaDCrose.GetWindowText(strTemp);
	m_Data.dBd_nsy = _tstof(strTemp);	
	m_editBetaDNoCrose.GetWindowText(strTemp);
	m_Data.dBd_sy = _tstof(strTemp);

	int m_arElemKSize = m_arElemK.GetSize();
	for(int i=0; i<m_arElemKSize; i++)
	{
		m_RcwaData.SetAt(m_arElemK[i], m_Data);
	}

	return TRUE;
}

void CRCWallParameter::SetInitUnit()
{
	m_editDt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editDb.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHeightI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_editHeightJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitDt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitDb.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHeightI.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_unitHeightJ.SetUnitType(D_UNITSYS_BASE_LENGTH);
}

void CRCWallParameter::InitialCboCrackWidth()
{
	if(m_strConCode==_T("KCI-USD07"))
		return;

	m_comboCrackWidth.ResetContent();
	CString strAlwCrack;
	if(m_strConCode==_T("KSCE-USD05"))
	{
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_DRY),												m_comboCrackWidth.AddString(strAlwCrack);//_T("건조환경(0.006tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_MOIST),											m_comboCrackWidth.AddString(strAlwCrack);//_T("습윤환경(0.005tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_RUST),											m_comboCrackWidth.AddString(strAlwCrack);//_T("부식성환경(0.004tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST),							m_comboCrackWidth.AddString(strAlwCrack);//_T("고부식성환경(0.0035tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_WATER_STRUCT),							m_comboCrackWidth.AddString(strAlwCrack);//_T("수밀성구조물(0.2mm)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST_WATER_STRUCT), m_comboCrackWidth.AddString(strAlwCrack);//_T("고부식성환경/수밀성구조물(0.13mm)")// Add/Modify by sshan. MNET:XXXX.('20070514)
	}
	else if(m_strConCode==_T("KSCE-RAIL-USD04"))
	{
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_DRYING),										m_comboCrackWidth.AddString(strAlwCrack);//_T("건조한환경(0.006tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_GENERAL),										m_comboCrackWidth.AddString(strAlwCrack);//_T("일반환경(0.005tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_RUST),											m_comboCrackWidth.AddString(strAlwCrack);//_T("부식성환경(0.004tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_VERY_INTENSE_RUST),					m_comboCrackWidth.AddString(strAlwCrack);//_T("극심한부식성환경(0.0035tc)
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_WATER_STRUCT),							m_comboCrackWidth.AddString(strAlwCrack);//")수밀성구조물(0.2mm)_T("
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST_WATER_STRUCT), m_comboCrackWidth.AddString(strAlwCrack);//")고부식성환경/수밀성구조물(0.13mm)_T("
	}
	m_comboCrackWidth.SetCurSel(0);  

	// Recalculate Width.
	CClientDC dc(this);
	int nSize = m_comboCrackWidth.GetCount();
	CSize sizeMax, sizeCur;
	CString strItem;
	m_comboCrackWidth.GetLBText(0, strItem);
	sizeMax = dc.GetTextExtent(strItem);
	for(int k=1; k<nSize; k++)
	{
		m_comboCrackWidth.GetLBText(k, strItem);
		sizeCur = dc.GetTextExtent(strItem);
		if(sizeCur.cx > sizeMax.cx)	sizeMax.cx = sizeCur.cx;
	}
	if(sizeMax.cx > m_comboCrackWidth.GetDroppedWidth())	m_comboCrackWidth.SetDroppedWidth(sizeMax.cx);  
}

void CRCWallParameter::OnAddData()
{
	// 여러개 선택할 수도 있다. -> 처리해야함.
	addDelMod = 0;

	if (!Dlg2Data()) 
	{
		AfxMessageBox(_LS(IDS_DGN_INPUT_DATA_ERROR));
		return;
	}

	m_pBadKeyList.RemoveAll();
	m_pBadMemberKeyList.RemoveAll();
	
	T_ELEM_K ElemK;
	T_RCWA_D RcwaD;
	POSITION Pos = m_RcwaData.GetStartPosition();
	while(Pos)
	{
		m_RcwaData.GetNextAssoc(Pos,ElemK,RcwaD); 
		
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
	}

	int nBadMemberKeySize = m_pBadMemberKeyList.GetSize();
	for(int i=0 ; i<nBadMemberKeySize ; ++i)
	{
		m_RcwaData.RemoveKey(m_pBadMemberKeyList[i]);//Member가 아닌 키 삭제
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
		Message = Message + _LS(IDS_DGN_BAD_INPUT_DATA_FOR_MEMBER_ERROR);//")번\n요소는 데이터가 부적절하여 추가하실 수 없습니다._T("
		AfxMessageBox(Message, MB_OK);
	}

	Pos = m_RcwaData.GetStartPosition();
	while(Pos)
	{
		m_RcwaData.GetNextAssoc(Pos,ElemK,RcwaD);        
	}
	m_WallGrid->SetCurrentRcwa(ElemK, RcwaD);//마지막으로 추가된것을 현재 셀로
}

void CRCWallParameter::OnModifyData()
{
	addDelMod = 1;

	T_ELEM_K ElemK;
	T_RCWA_D RcwaD;

	CArray<T_RCWA_K, T_RCWA_K> arKey;
	CArray<T_RCWA_D, T_RCWA_D&> arData;
	arKey.RemoveAll();
	arData.RemoveAll();
	
	if (!Dlg2Data()) return;

	POSITION Pos = m_RcwaData.GetStartPosition();
	while(Pos)
	{
		m_RcwaData.GetNextAssoc(Pos,ElemK,RcwaD);    
		arKey.Add(ElemK);
		arData.Add(RcwaD);
	}  	
	m_pDoc->m_pDataCtrl->ModifyRcwa(arKey, arKey, arData);
}


void CRCWallParameter::OnDeleteData()
{
	addDelMod = 2;

	if (!Dlg2Data()) return;	

	T_ELEM_K ElemK;
	T_RCWA_D RcwaD;
	CArray<T_RCWA_K, T_RCWA_K> arKey;  
	arKey.RemoveAll();

	POSITION Pos = m_RcwaData.GetStartPosition();
	while(Pos)
	{
		m_RcwaData.GetNextAssoc(Pos,ElemK,RcwaD);
		arKey.Add(ElemK);
	} 
	m_pDoc->m_pDataCtrl->DelRcwa(arKey);
}

void CRCWallParameter::ChangeGroupandLoadCombination()
{
	m_ltbxLcomServ.ResetContent();
	m_ltbxLcomStre.ResetContent();
	m_ltbxGroup.ResetContent();

	InitialLcomDataForDgn();
}

void CRCWallParameter::InitialLcomDataForDgn()
{ 
	// Save Total Load Combination Name by Serial Number(No LcomKey).	
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, m_arLcomKey);
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(m_arGrupKey);

	T_RCWG_D RcwgData;
	
	if(m_pDoc->m_pAttrCtrl->ExistRcwg())//DB에 데이터가 있다면 DB에서 데이터를 가져와서 세팅
	{
		m_pDoc->m_pAttrCtrl->GetRcwg(RcwgData);
		int i;
		m_arStreLcomKey.RemoveAll();
		for(i=0 ; i<RcwgData.StreLcom.GetSize() ; i++)
		{
			m_arStreLcomKey.SetAt(RcwgData.StreLcom[i], RcwgData.StreLcom[i]);
		}

		m_arServLcomKey.RemoveAll();
		for(i=0 ; i<RcwgData.ServLcom.GetSize() ; i++)
		{
			m_arServLcomKey.SetAt(RcwgData.ServLcom[i], RcwgData.ServLcom[i]);
		}

		m_arGroupMapKey.RemoveAll();
		for(i=0 ; i<RcwgData.ColGroupKey.GetSize() ; i++)
		{
			m_arGroupMapKey.SetAt(RcwgData.ColGroupKey[i], RcwgData.ColGroupKey[i]);
		}
	}

	int iLcomSerNum=0;
	int i=0, j=0;
	int iLcomCount=0;
	
	int nID;
	int nIndex;  
	int m_arLcomKeySize = m_arLcomKey.GetSize();
	for(i=0; i<m_arLcomKeySize; i++)
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
			
			if(m_pDoc->m_pAttrCtrl->ExistRcwg())//DB에 저장된것이 있다면 저장된 값으로 체크
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
			
			if(m_pDoc->m_pAttrCtrl->ExistRcwg())//DB에 저장된것이 있다면 저장된 값으로 체크
			{
				if(m_arServLcomKey.Lookup(m_arLcomKey[i], ExistKey))
					bExist = true;
				else
					bExist = false;
			}
			else//DB저장된 데이터가 없다면
			{
				bExist = true;
			}
			m_ltbxLcomServ.SetCheck(nIndex, bExist);
		}    		
	}

	CMap<T_ELEM_K,T_ELEM_K,T_ELEM_K,T_ELEM_K> ElemKeyListInGroup;
	CMap<T_ELEM_K,T_ELEM_K,T_ELEM_K,T_ELEM_K> OverlappingElemKeyList;
	CMap<T_GRUP_K, T_GRUP_K, T_GRUP_K, T_GRUP_K> GroupKeyList;
	CArray<CString, CString> OverlappingGroupList;
	
	//중복되는 요소키를 뽑아 낸다
	int m_arGrupKeySize = m_arGrupKey.GetSize();

	for(i=0; i<m_arGrupKeySize; i++)
	{
		T_GRUP_D dGrup;
		CArray<T_ELEM_K, T_ELEM_K> ElemKeyList;
		dGrup.Initialize();
		m_pDoc->m_pAttrCtrl->GetGrup(m_arGrupKey[i], dGrup);
		ElemKeyList.Copy(dGrup.arKeyElem);

		if(i==0)
		{//첫번째 그룹의 elem키 리스트는 그대로 ElemKeyListInGroup에 넣는다
			int ElemKeyListSize = ElemKeyList.GetSize();
			for(int c=0 ; c<ElemKeyListSize ; c++)
			{
				ElemKeyListInGroup.SetAt(ElemKeyList[c], ElemKeyList[c]);
			}
		}
		else
		{//두번째 부터는 ElemKeyListInGroup에 존재하는지 확인한 후 존재하는 것은 중복되는 것이므로
		 //OverlappingElemKeyList에 넣는다
			int ElemKeyListSize = ElemKeyList.GetSize();
			for(int c=0 ; c<ElemKeyListSize ; c++)
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
	m_arGrupKeySize = m_arGrupKey.GetSize();
	for(i=0; i<m_arGrupKeySize; i++)
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

	m_arGrupKeySize = m_arGrupKey.GetSize();
	for(i=0; i<m_arGrupKeySize; i++)
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
		
		if(m_pDoc->m_pAttrCtrl->ExistRcwg())//DB에 저장된것이 있다면 저장된 값으로 체크
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

		int arOverlappingElemKeySize = arOverlappingElemKey.GetSize();
		for(i=0 ; i<arOverlappingElemKeySize ; i++)
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

		int OverlappingGroupListSize = OverlappingGroupList.GetSize();
		for(i=0 ; i<OverlappingGroupListSize ; i++)
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
		Message.Format(_LS(IDS_DGN_MEMBER_GROUP_ERROR), strOverlappingElemKey, strOverlappingGroupName);//")%s번요소가\n여러 그룹에 중복정의 되어\n%s 그룹을\n제외하였습니다._T("
		AfxMessageBox(Message, MB_OK);

		m_pParent->m_bGroupMessage = TRUE;
	}
	
}


void CRCWallParameter::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CRCWallParameter::UpdateDgnData()

{
	T_ELEM_K    Key;
	T_RCWA_D    Data;

	CArray<T_RCWA_K, T_RCWA_K> arKey;
	CArray<T_RCWA_D, T_RCWA_D&> arData;
	arKey.RemoveAll();
	arData.RemoveAll();

	POSITION Pos = m_RcwaData.GetStartPosition();

	while(Pos)
	{
		m_RcwaData.GetNextAssoc(Pos,Key,Data);
		arKey.Add(Key);
		arData.Add(Data);	
	}
	m_pDoc->m_pDataCtrl->AddRcwa(arKey,arData);
}

void CRCWallParameter::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_ELEM_K cKey;
	m_WallGrid->GetCurrentCell(ncRow, ncCol);
	m_WallGrid->GetColumnNoFor(m_WallGrid->GetKeyColID(), nkCol);
	cKey = _ttol(m_WallGrid->GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_ELEM_K    Key;//, KeyBak;
	T_RCWA_D    Data, DataBak;

	BOOL bOldLock = m_WallGrid->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RCWA_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetRcwa(nKey, Key, Data);
				m_WallGrid->InsertRecordByKey(Key, Data);
				break;
		case(UR_RCWA_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRcwa(nKey, Key, Data);
				m_WallGrid->DeleteRecordByKey(Key, Data);
				break;
				/*
		case(UR_RCWA_MFD):
				// 여기서는 저장만 하고 UR_RCCO_MFS에서 처리한다.
				pViewBuff->GetRcwa(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_RCWA_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetRcwa(nKey, Key, Data);
				m_WallGrid->DeleteRecordByKey(Key, Data);
				m_WallGrid->InsertRecordByKey(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;*/
		case(UR_DCON_ADD):// Design Code 변경
		case(UR_DCON_DEL):	
			{
				InitialCboCrackWidth();
				InitEnableColtrol();
				break;
			}		
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_WallGrid->m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_WallGrid->m_nEditMode = CTBBrowserWnd::noMode;
		if (m_WallGrid->GetRowCount() < ncRow) ncRow = m_WallGrid->GetRowCount();
		BOOL bSet = m_WallGrid->SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = m_WallGrid->SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = m_WallGrid->SetLeftValidCell(ncRow, ncCol);
		m_WallGrid->m_nEditMode = nOldMode;
	}

	m_WallGrid->GetCurrentCell(ncRow, ncCol);
	m_WallGrid->GetColumnNoFor(m_WallGrid->GetKeyColID(), nkCol);
	T_ELEM_K newKey = _ttol(m_WallGrid->GetValueRowCol(ncRow, nkCol));
	if (newKey != cKey) m_WallGrid->OnChangeCurrentRecord(ncRow);

	m_WallGrid->AdjustAppendRowNo();
	m_WallGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_WallGrid->Redraw();  
}


void CRCWallParameter::OnChangeCurrentRcwa(T_ELEM_K key)
{
	m_pDoc->m_pAttrCtrl->GetRcwa(key, m_Data);
	Data2Dlg();
}



// 계수하중조합과 사용하중 조합의 내용을 DB에 업데이트한다.
void CRCWallParameter::OnBtnApply()   
{
	Dlg2Data_LoadCase();

	if(m_arStreLcomKey.IsEmpty() /*&& m_arServLcomKey.IsEmpty()*/)//계수하중조합이 없으면 무조건 계산하지 않음
	{
		AfxMessageBox(_LS(IDS_DGN_LOADCASE_SELETE), MB_OK);
		m_bOnBtnApplyResult = FALSE;
		return;
	}
	
	T_RCWG_D RcwgD;
	RcwgD.Initialize();
	T_LCOM_K LcomK, GetLcomK;
	POSITION Pos = m_arStreLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arStreLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RcwgD.StreLcom.Add(GetLcomK);
	}
	Pos = m_arServLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arServLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RcwgD.ServLcom.Add(GetLcomK);
	}

	T_GRUP_K GrupK, GetGrupK;
	Pos = m_arGroupMapKey.GetStartPosition();
	while(Pos)
	{
		m_arGroupMapKey.GetNextAssoc(Pos,GrupK,GetGrupK);
		RcwgD.ColGroupKey.Add(GetGrupK);
	}
	
	
	if(!m_pDoc->m_pDataCtrl->AddRcwg(RcwgD))
	{
		//
	}
	m_bOnBtnApplyResult = TRUE;
	return;
}

//LoadCase 체크유무 정보를 저장한다
void CRCWallParameter::Dlg2Data_LoadCase() 
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
void CRCWallParameter::Click_Btn_Apply() 
{
	OnBtnApply();
}

void CRCWallParameter::OnDgnWallCalcBtn() 
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
	
	if(m_WallGrid->GetRowCount() <= 1)
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
	pHeadOffice->ReqService(_ULS(rcs), EN_RCSSA_CIVIL_RC_WALL_CALC_DLG, NULL);	  
}

void CRCWallParameter::LoadDBtoDlg() 
{
	CArray<T_RCWA_K, T_RCWA_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRcwaKeyList(KeyList);

	m_RcwaData.RemoveAll();
	T_RCWA_D Data;
	
	int KeyListSize = KeyList.GetSize();
	for(int i=0 ; i<KeyListSize ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetRcwa(KeyList[i], Data))
			m_RcwaData.SetAt(KeyList[i], Data);
	}
	
	addDelMod = 0;

	if(KeyList.GetSize()>0)
		OnChangeCurrentRcwa(KeyList[0]);
}

BOOL CRCWallParameter::DataCheck() 
{
	CString ErrorMessanges;
	CString strTemp = _T("");
	double dTemp = 0.0;
	if(!m_chkIEnd.GetCheck() && !m_chkMid.GetCheck() && !m_chkJEnd.GetCheck())
		ErrorMessanges = _LS(IDS_DGN_MUST_ONE_OVER_PART_NUMBER);//")Part Number를 한가지 이상 선택하셔야 합니다._T("

	if(m_editDeltaNs.IsWindowEnabled())
	{
		m_editDeltaNs.GetWindowText(strTemp);
		dTemp = _tstof(strTemp);	
		if(dTemp <= 0) 
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_DELTA_NS_ERROR_1);//")delta_ns값은 0보다 큰값이어야 합니다._T("
			else
				ErrorMessanges += _LS(IDS_DGN_DELTA_NS_ERROR_2);//")\ndelta_ns값은 0보다 큰값이어야 합니다._T("
		}
	}

	if(m_editCm.IsWindowEnabled())
	{
		m_editCm.GetWindowText(strTemp);
		dTemp = _tstof(strTemp);	
		if(dTemp <= 0) 
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_CM_ERROR_1);//")Cm값은 0보다 큰값이어야 합니다._T("
			else
				ErrorMessanges += _LS(IDS_DGN_CM_ERROR_2);//")\nCm값은 0보다 큰값이어야 합니다._T("
		}
	}

	if(m_editDeltaS.IsWindowEnabled())
	{
		m_editDeltaS.GetWindowText(strTemp);
		dTemp = _tstof(strTemp);	
		if(dTemp <= 0) 
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_DELTA_S_ERROR_1);//")delta_s값은 0보다 큰값이어야 합니다._T("
			else
				ErrorMessanges += _LS(IDS_DGN_DELTA_S_ERROR_2);//")\ndelta_s값은 0보다 큰값이어야 합니다._T("
		}
	}

	if(!m_chkCroseBd.GetCheck())
	{
		m_editBetaDCrose.GetWindowText(strTemp);
		if(_tstof(strTemp) < 0)//m_editBetaDCrose는 0보다 작으면 안됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_BETAD_CROSS_ERROR_1);//")beta_d_Y값은 0과 같거나 큰값이어야 합니다._T("
			else
				ErrorMessanges += _LS(IDS_DGN_BETAD_CROSS_ERROR_2);//")\beta_d_Y값은 0과 같거나 큰값이어야 합니다._T("
		}	
	}

	if(!m_chkNoCroseBd.GetCheck())
	{
		m_editBetaDNoCrose.GetWindowText(strTemp);
		if(_tstof(strTemp) < 0)//m_editBetaDNoCrose 는 0보다 작으면 안됨
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_BETAD_NOCROSS_ERROR_1);//")beta_d_Y값은 0과 같거나 큰값이어야 합니다._T("
			else
				ErrorMessanges += _LS(IDS_DGN_BETAD_NOCROSS_ERROR_2);//")\beta_d_Y값은 0과 같거나 큰값이어야 합니다._T("
		}		
	}

	if(m_editPMax.IsWindowEnabled())
	{
		m_editPMax.GetWindowText(strTemp);
		dTemp = _tstof(strTemp);	
		if(dTemp <= 0 || dTemp >= 1.0) 
		{
			if(ErrorMessanges.GetLength()==0)
				ErrorMessanges = _LS(IDS_DGN_RHO_MAX_ERROR_1);//")rho_max값은 0보다 크거나 1보다 작은값이어야 합니다._T("
			else
				ErrorMessanges += _LS(IDS_DGN_RHO_MAX_ERROR_2);//")\nrho_max값은 0보다 크거나 1보다 작은값이어야 합니다._T("
		}
	}
	
	if(m_editDt.GetEditValue()<=0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DT_ERROR_1);//")Dt값은 0보다 큰값이어야 합니다._T("
		else
			ErrorMessanges += _LS(IDS_DGN_DT_ERROR_2);//")\nDt값은 0보다 큰값이어야 합니다._T("
	}


	if(m_editDb.GetEditValue()<=0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DB_ERROR_1);//")Db값은 0보다 큰값이어야 합니다._T("
		else
			ErrorMessanges += _LS(IDS_DGN_DB_ERROR_2);//")\nDb값은 0보다 큰값이어야 합니다._T("
	}

	if(m_editHeightI.GetEditValue()<0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_H_I_ERROR_1);//")h_i값은 0보다 큰값이어야 합니다._T("
		else
			ErrorMessanges += _LS(IDS_DGN_H_I_ERROR_2);//")\nh_i값은 0보다 큰값이어야 합니다._T("
	}

	if(m_editHeightJ.GetEditValue()<0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_H_J_ERROR_1);//")h_j값은 0보다 큰값이어야 합니다._T("
		else
			ErrorMessanges += _LS(IDS_DGN_H_J_ERROR_2);//")\nh_j값은 0보다 큰값이어야 합니다._T("
	}
	
	if(!ErrorMessanges.GetLength()==0)
	{
		AfxMessageBox(ErrorMessanges, MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CRCWallParameter::OnDgnUnbracedLengthBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_LENG__);	
}

void CRCWallParameter::OnDgnEffectiveLengthBtn() 
{
	CTreeMenuBarBase::SetModeST(__TMMODE_DGN_GEN_KFAC__);	
}

void CRCWallParameter::EnableCalcBtn(bool Enable) 
{
	GetDlgItem(IDC_DGN_WALL_CALC_BTN)->EnableWindow(Enable);
}

//모멘트 확대계수 자동결정
void CRCWallParameter::OnDgnChkClossBinding() 
{
	if(m_chkAutoSway.GetCheck())
	{
		m_radioSway.EnableWindow(FALSE);
		m_radioNonSway.EnableWindow(FALSE);	

		m_chkAutoMomentEnlarge.EnableWindow(TRUE);
		OnDgnChkAutoMomentEnlarge();

		m_chkAutoMomentExpansion.EnableWindow(TRUE);
		OnDgnChkAutoMomentEnlargeNocress();
	}
	else
	{
		m_radioSway.EnableWindow(TRUE);
		m_radioNonSway.EnableWindow(TRUE);
		OnDgnRadioSwayNonsway();
	}
}

//Short Column  or Long-Short Check
void CRCWallParameter::OnDgnRadioLongShort()
{
	if(m_radioShort.GetCheck())
	{
		GetDlgItem(IDC_DGN_WALL_CROSS_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_SWAY_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_NONSWAY_GROUP)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_BD_GROUP)->EnableWindow(FALSE);
		m_chkAutoSway.EnableWindow(FALSE);
		m_radioSway.EnableWindow(FALSE);
		m_radioNonSway.EnableWindow(FALSE);	

		m_chkAutoMomentEnlarge.EnableWindow(FALSE);
		m_chkAutoMomentEquivalent.EnableWindow(FALSE);				
		m_editDeltaNs.EnableWindow(FALSE);
		m_editCm.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_DELTA_NS_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_CM_STATIC)->EnableWindow(FALSE);

		m_chkAutoMomentExpansion.EnableWindow(FALSE);
		m_editDeltaS.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_DELTA_S_STATIC)->EnableWindow(FALSE);
		
		m_chkCroseBd.EnableWindow(FALSE);
		m_chkNoCroseBd.EnableWindow(FALSE);
		m_editBetaDCrose.EnableWindow(FALSE);
		m_editBetaDNoCrose.EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_WALL_CROSS_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_SWAY_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_NONSWAY_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_BD_GROUP)->EnableWindow(TRUE);
		m_chkAutoSway.EnableWindow(TRUE);
		m_chkCroseBd.EnableWindow(TRUE);
		m_chkNoCroseBd.EnableWindow(TRUE);

		// (2008.06.30) Modify by Unsang :: Sway상태에서 횡구속변수입력하는 Editbox 활성화되지 않음
		m_chkAutoMomentEnlarge.EnableWindow(TRUE);
		m_chkAutoMomentEquivalent.EnableWindow(TRUE);				
		m_editDeltaNs.EnableWindow(TRUE);
		m_editCm.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_DELTA_NS_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_CM_STATIC)->EnableWindow(TRUE);

		m_chkAutoMomentExpansion.EnableWindow(TRUE);
		m_editDeltaS.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_DELTA_S_STATIC)->EnableWindow(TRUE);


		OnDgnChkClossBinding();
		OnClickChkCroseBd();
		OnClickChkNoCroseBd(); 
	}
}
//Sway or Nonsway
void CRCWallParameter::OnDgnRadioSwayNonsway() 
{
	// Change by SHIN.(07.08.23) Sway상태에서도 횡구속 변수를 입력받도록 변경함
	if(m_radioSway.GetCheck())
	{
		//GetDlgItem(IDC_DGN_WALL_SWAY_GROUP)->EnableWindow(FALSE);
		//m_chkAutoMomentEnlarge.EnableWindow(FALSE);
		//m_chkAutoMomentEquivalent.EnableWindow(FALSE);
		//m_editDeltaNs.EnableWindow(FALSE);
		//m_editCm.EnableWindow(FALSE);
		//GetDlgItem(IDC_DGN_WALL_DELTA_NS_STATIC)->EnableWindow(FALSE);
		//GetDlgItem(IDC_DGN_WALL_CM_STATIC)->EnableWindow(FALSE);

		// (2008.06.30) Modify by Unsang :: Sway상태에서 횡구속변수입력하는 Editbox 활성화되지 않음
		OnDgnChkAutoMomentEnlarge();    


		GetDlgItem(IDC_DGN_WALL_NONSWAY_GROUP)->EnableWindow(TRUE);
		m_chkAutoMomentExpansion.EnableWindow(TRUE);
		OnDgnChkAutoMomentEnlargeNocress();
	}
	
	if(m_radioNonSway.GetCheck())
	{
		GetDlgItem(IDC_DGN_WALL_NONSWAY_GROUP)->EnableWindow(FALSE);
		m_chkAutoMomentExpansion.EnableWindow(FALSE);
		m_editDeltaS.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_DELTA_S_STATIC)->EnableWindow(FALSE);

		//GetDlgItem(IDC_DGN_WALL_SWAY_GROUP)->EnableWindow(TRUE);
		//m_chkAutoMomentEnlarge.EnableWindow(TRUE);
		OnDgnChkAutoMomentEnlarge();
	}
}

//모멘트 확대계수 자동결정
void CRCWallParameter::OnDgnChkAutoMomentEnlarge() 
{
	if(m_chkAutoMomentEnlarge.GetCheck())
	{
		m_editDeltaNs.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_DELTA_NS_STATIC)->EnableWindow(FALSE);

		m_chkAutoMomentEquivalent.EnableWindow(TRUE);
		OnDgnChkAutoEquivalentMoment();
	}
	else
	{
		m_editDeltaNs.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_DELTA_NS_STATIC)->EnableWindow(TRUE);

		m_chkAutoMomentEquivalent.EnableWindow(FALSE);
		m_editCm.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_CM_STATIC)->EnableWindow(FALSE);
	}
}

//등가모멘트 보정계수 자동결정
void CRCWallParameter::OnDgnChkAutoEquivalentMoment() 
{
	if(m_chkAutoMomentEquivalent.GetCheck())
	{
		m_editCm.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_CM_STATIC)->EnableWindow(FALSE);
	}
	else
	{
		m_editCm.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_CM_STATIC)->EnableWindow(TRUE);
	}
}

//모멘트 확대 계수 자동결정(비횡구속)
void CRCWallParameter::OnDgnChkAutoMomentEnlargeNocress() 
{
	if(m_chkAutoMomentExpansion.GetCheck())
	{
		m_editDeltaS.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_DELTA_S_STATIC)->EnableWindow(FALSE);
	}
	else
	{
		m_editDeltaS.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_DELTA_S_STATIC)->EnableWindow(TRUE);
	}
}
//최대 철근비검토
void CRCWallParameter::OnDgnChkAutoRebarPosi() 
{
	if(m_chkAutoRebarPosi.GetCheck())
	{
		m_editTopRebarRatio.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_TOP_REBAR_RATIO_STATIC)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_TOP_REBAR_RATIO_UNIT_STATIC)->EnableWindow(FALSE);
	}
	else
	{
		m_editTopRebarRatio.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_TOP_REBAR_RATIO_STATIC)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_TOP_REBAR_RATIO_UNIT_STATIC)->EnableWindow(TRUE);
	}
}
//최대 철근비검토
void CRCWallParameter::OnDgnChkRhoMax() 
{
	if(m_chkMaxRebarRatio.GetCheck())
	{
		m_editPMax.EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_WALL_PMAX_STATIC)->EnableWindow(TRUE);
	}
	else
	{
		m_editPMax.EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_WALL_PMAX_STATIC)->EnableWindow(FALSE);
	}
}

void CRCWallParameter::OnClickChkCroseBd() 
{
	if(m_chkCroseBd.GetCheck())
		m_editBetaDCrose.EnableWindow(TRUE);
	else
		m_editBetaDCrose.EnableWindow(FALSE);
}

void CRCWallParameter::OnClickChkNoCroseBd() 
{
	if(m_chkNoCroseBd.GetCheck())
		m_editBetaDNoCrose.EnableWindow(TRUE);
	else
		m_editBetaDNoCrose.EnableWindow(FALSE);
}


// PARKHJ-20111007 : 번호 할당시 DgnStruct.h 참고!
CString CRCWallParameter::ReturnDgnConCode()
{
	CString strCodeNumber = _T("C_CON");

	T_DCON_D DconD;
	m_pDoc->m_pAttrCtrl->GetDcon(DconD);

	return CDgnCodeCtrl::GetConCodeNumber4Log(strCodeNumber, DconD.DesignCode);	
}