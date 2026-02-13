#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenLengDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_TreeMenuBarBase.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_dbLock\LockOption.h"
#include "..\wg_db\DBCodeCtrl.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int ELEM_TYPE_ELEMENT = 0;
const int ELEM_TYPE_VBEAM = 1;
const int OPTION_ADD_REPLACE = 0;
const int OPTION_DELETE = 1;

CDgnGenLengDlg::CDgnGenLengDlg(CWnd* pParent /*=NULL*/)
: CMenuBarChildDlg(CDgnGenLengDlg::IDD, pParent)
{
	m_nOption = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;
	m_BLy = 0.0;
	m_BLz = 0.0;
	m_BLu = 0.0;
	m_BLt = 0.0;
	m_bNotUseBLu = FALSE;
	m_bAutoCalcLe = FALSE;

	m_pDoc = CDBDoc::GetDocPoint();	
}

void CDgnGenLengDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLZ, m_BLzCtrl);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLY, m_BLyCtrl);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLU, m_BLuCtrl);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLT, m_BLtCtrl);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLZUNIT, m_BLzUnit);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLYUNIT, m_BLyUnit);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLUUNIT, m_BLuUnit);
	DDX_Control(pDX, IDC_DGN_LENGTH_BLTUNIT, m_BLtUnit);
	DDX_Radio(pDX, IDC_DGN_LENGTH_AR, m_nOption);
	DDX_Radio(pDX, IDC_DGN_CPG_ELEM_RDO, m_nElemType);
	DDX_Text(pDX, IDC_DGN_LENGTH_BLY, m_BLy);
	DDX_Text(pDX, IDC_DGN_LENGTH_BLZ, m_BLz);
	DDX_Text(pDX, IDC_DGN_LENGTH_BLU, m_BLu);
	DDX_Text(pDX, IDC_DGN_LENGTH_BLT, m_BLt);
	DDX_Check(pDX, IDC_DGN_LENGTH_NOTLENGTH, m_bNotUseBLu);
	DDX_Check(pDX, IDC_DGN_LENGTH_AUTOCALC_BLE, m_bAutoCalcLe);
}

void CDgnGenLengDlg::InitCtrl()
{
	m_aGenGroup.RemoveAll();
	m_aGenGroup.Add(IDC_DGN_BLU_STATIC1);
	m_aGenGroup.Add(IDC_DGN_BLU_STATIC2);
	m_aGenGroup.Add(IDC_DGN_LENGTH_BLU);
	m_aGenGroup.Add(IDC_DGN_LENGTH_BLUUNIT);
	m_aGenGroup.Add(IDC_DGN_LENGTH_NOTLENGTH);
	m_aGenGroup.Add(IDC_DGN_LENGTH_AUTOCALC_BLE);

	m_aButtonGroup.RemoveAll();
	m_aButtonGroup.Add(IDC_DGN_EXECUTE);
	m_aButtonGroup.Add(IDC_DGN_CLOSE);

	m_aElemTypeCtrl.RemoveAll();
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_TYPE_GRP);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_ELEM_RDO);
	m_aElemTypeCtrl.Add(IDC_DGN_CPG_VELEM_RDO);

	m_aPositionCtrl.RemoveAll();
	// Unbraced Length
	m_aPositionCtrl.Add(IDC_DGN_UBLN_STATIC1);
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC0);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLY);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLYUNIT);
	m_aPositionCtrl.Add(IDC_WG_DGN_STATIC1);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLZ);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLZUNIT);
	// Laterally Unbraced Length
	m_aPositionCtrl.Add(IDC_DGN_BLU_STATIC1);
	m_aPositionCtrl.Add(IDC_DGN_BLU_STATIC2);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLU);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLUUNIT);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_NOTLENGTH);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_AUTOCALC_BLE);
	// Torsional Unbraced Length
	m_aPositionCtrl.Add(IDC_DGN_BLT_STATIC);
	m_aPositionCtrl.Add(IDC_DGN_BLT_NAME);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLT);
	m_aPositionCtrl.Add(IDC_DGN_LENGTH_BLTUNIT);
}

BEGIN_MESSAGE_MAP(CDgnGenLengDlg, CMenuBarChildDlg)
	ON_BN_CLICKED(IDC_DGN_LENGTH_AR, OnDgnLengthAr)
	ON_BN_CLICKED(IDC_DGN_LENGTH_DEL, OnDgnLengthDel)
	ON_BN_CLICKED(IDC_DGN_LENGTH_NOTLENGTH, OnDgnLengthNotlength)
	ON_BN_CLICKED(IDC_DGN_LENGTH_AUTOCALC_BLE, OnDgnLengthAutocalcBle)
	ON_BN_CLICKED(IDC_DGN_CPG_ELEM_RDO, OnDgnCPGElemTypeRdo)
	ON_BN_CLICKED(IDC_DGN_CPG_VELEM_RDO, OnDgnCPGElemTypeRdo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnGenLengDlg message handlers

BOOL CDgnGenLengDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	if(m_pDoc==NULL) return TRUE;

	// Default 값 초기화
	Initial_Data();
	InitCtrl();
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDgnGenLengDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if(m_BLy < 0.)		bCheck = FALSE;
	if(m_BLz < 0.)		bCheck = FALSE;
	if(!m_bNotUseBLu)
	{
		if(m_BLu < 0.)	bCheck = FALSE;
	}
	if(m_BLt < 0.)   bCheck = FALSE;
	if(!bCheck)	AfxMessageBox(_LS(IDS_DGN_CHK_UNBRACED_LEN),MB_OK);

	return bCheck;
}

void CDgnGenLengDlg::Initial_Data()
{
	m_BLyCtrl.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_BLyCtrl.SetAttWcsDistance();
	m_BLyCtrl.SetDistValueMode(TRUE);
	
	m_BLzCtrl.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_BLzCtrl.SetAttWcsDistance();
	m_BLzCtrl.SetDistValueMode(TRUE);

	m_BLuCtrl.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_BLuCtrl.SetAttWcsDistance();
	m_BLuCtrl.SetDistValueMode(TRUE);

	m_BLtCtrl.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_BLtCtrl.SetAttWcsDistance();
	m_BLtCtrl.SetDistValueMode(TRUE);

	//BOOL GetCurMousePosDistVal(double &Distance);// 나중에 값을 받아올 때...
	CDgnDataCtrl DataCtrl;
	T_DSTL_D cData;
	cData.Initialize();
	DataCtrl.Get_DgnStlDstl(cData);
	if(cData.bAllFrameLaterallyBraced)
	{
		m_bNotUseBLu	= TRUE;
		m_bAutoCalcLe	= FALSE;
		GetDlgItem(IDC_DGN_LENGTH_BLU)->EnableWindow(FALSE);
	}
	else
	{
	  m_bNotUseBLu	= FALSE;
		m_bAutoCalcLe	= FALSE;
		GetDlgItem(IDC_DGN_LENGTH_BLU)->EnableWindow(TRUE);
	}
	m_nOption  = OPTION_ADD_REPLACE;
	m_nElemType = ELEM_TYPE_ELEMENT;

	m_BLy = 0.0;
	m_BLz = 0.0;
	m_BLu = 0.0;
	m_BLt = 0.0;

	Update_InitDataByCode();
	UpdateData(FALSE);
}

void CDgnGenLengDlg::AlignControl()
{
	T_CPGD_D CpgdD;
	CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	CWnd* pElementTypeGroup = GetDlgItem(IDC_DGN_CPG_ELEM_TYPE_GRP);
	CWnd* pUnbracedLengthGroup = GetDlgItem(IDC_DGN_UBLN_STATIC1);

	if (pElementTypeGroup == nullptr || pUnbracedLengthGroup == nullptr) 
		return;	

	CRect rElementType, rUnbracedLength;
	pElementTypeGroup->GetWindowRect(rElementType);
	pUnbracedLengthGroup->GetWindowRect(rUnbracedLength);

	const int nDistX = rElementType.left - rUnbracedLength.left;
	const int nDistY = (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode)) ?
		(rElementType.top - rUnbracedLength.top) :
		(rElementType.bottom - rUnbracedLength.top + globalUtils.ScaleByDPI(4));

	CDlgUtil::CtrlMoveDistXY(this, m_aPositionCtrl, nDistX, nDistY);
}

void CDgnGenLengDlg::EnableDisableControls()
{
	UpdateData(TRUE);

	CDlgUtil::CtrlEnableDisable(this, m_aPositionCtrl, (m_nOption == OPTION_ADD_REPLACE));
}

void CDgnGenLengDlg::ShowHideControls()
{
	T_CPGD_D CpgdD; CpgdD.Initialize();
	m_pDoc->m_pAttrCtrl->GetDgnCpgd(CpgdD);

	if (!m_pDoc->m_pAttrCtrl2->IsDgnVbeamMode() || !m_pDoc->m_pAttrCtrl2->IsDgnVbeamCpgdCode(CpgdD.iDgnCode))
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_HIDE);
	}
	else
	{
		CDlgUtil::CtrlShowHide(this, m_aElemTypeCtrl, SW_SHOW);
	}

	// Apply Close Button Hide
	GetDlgItem(IDC_DGN_EXECUTE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DGN_CLOSE)->ShowWindow(SW_HIDE);
}

void CDgnGenLengDlg::Execute() 
{
#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

	auto SetCommonData = [this](auto& rData) {
		UpdateData(TRUE);
		rData.BLy = m_BLy;
		rData.BLz = m_BLz;
		rData.BLu = m_BLu;
		rData.BLt = m_BLt;
		rData.bNotUseBLu = m_bNotUseBLu;
		rData.bAutoCalcLe = m_bAutoCalcLe;

		if (m_bNotUseBLu || m_bAutoCalcLe)
			rData.BLu = 0.0;		
	};

	auto ExcuteAddReplace = [this, &SetCommonData](auto& selKeys, bool isElement) {
		if (isElement) 
		{
			T_LENG_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddLeng(selKeys, rData)) Initial_SelectItem();
		}
		else 
		{
			T_LENV_D rData;
			rData.Initialize();
			SetCommonData(rData);
			if (m_pDoc->m_pDataCtrl->AddLenv(selKeys, rData)) Initial_SelectItem();
		}
	};

	auto ExcuteDelete = [this](auto& selKeys, bool isElement) {
		bool success = isElement ?
			m_pDoc->m_pDataCtrl->DelLeng(selKeys) :
			m_pDoc->m_pDataCtrl->DelLenv(selKeys);
		if (success) Initial_SelectItem();
	};

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;

	if (m_nElemType == ELEM_TYPE_ELEMENT) 
	{
		pIGM->GetSelectedElemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) {
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}

		// Frame Type 요소만 필터링
		CArray<T_ELEM_K, T_ELEM_K> rSelKey;
		for (int i = 0; i < aSelKey.GetSize(); i++) {
			T_ELEM_K key = aSelKey.GetAt(i);
			T_ELEM_D eData;
			eData.Initialize();
			m_pDoc->m_pAttrCtrl->GetElem(key, eData);
			if (m_pDoc->m_pAttrCtrl->IsFrameType(eData.eltyp)) {
				rSelKey.Add(key);
			}
		}

		if (rSelKey.GetSize() == 0) 
		{
			AfxMessageBox(_LS(IDS_DGN_CHK_ELEM_TYPE), MB_OK);
			return;
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(rSelKey, true);		
		else
			ExcuteDelete(rSelKey, true);
		
	}
	else if (m_nElemType == ELEM_TYPE_VBEAM) 
	{
		pIGM->GetSelectedVbemKeyListForDgn(aSelKey);

		if (aSelKey.GetSize() == 0) 
		{
			AfxMessageBox(_LS(IDS_DGN_COM_NO_SEL_ELEM), MB_OK);
			return;
		}

		if (m_nOption == OPTION_ADD_REPLACE)
			ExcuteAddReplace(aSelKey, false);		
		else
			ExcuteDelete(aSelKey, false);		
	}
}

void CDgnGenLengDlg::Initial_SelectItem() 
{
	// Unselected All.
	if(m_pDoc==NULL)	return;
	if(m_pDoc->m_pViewCtrl->SelectedElemCount()) m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
	// Initialize Data.
	Initial_Data();
}

void CDgnGenLengDlg::Update_InitDataByCode()
{
	AlignControl();
	ShowHideControls();
	EnableDisableControls();

	// BLe 활성화 조건 확인
	const BOOL bEnableBLe = m_pDoc->IsBS5950_90() ||
		m_pDoc->IsBS5950_2K() ||
		m_pDoc->IsIS800_2007();

	// Lb 라벨 텍스트 설정
	CString strInitLb = _T("Lb");
	CString strLb = m_pDoc->GetDesignParamNa(strInitLb);
	GetDlgItem(IDC_DGN_BLU_STATIC2)->SetWindowText(strLb);

	// Auto Calculate BLe 컨트롤 표시/숨김
	const int nShowAutoCalc = bEnableBLe ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_DGN_LENGTH_AUTOCALC_BLE)->ShowWindow(nShowAutoCalc);

	// Auto Calculate BLe 컨트롤 활성화/비활성화
	const BOOL bEnableAutoCalc = m_bNotUseBLu ? FALSE : bEnableBLe;
	GetDlgItem(IDC_DGN_LENGTH_AUTOCALC_BLE)->EnableWindow(bEnableAutoCalc);

	// Torsional Unbraced Length 표시/숨김 조건 결정
	int nShowLt = SW_HIDE;
#if defined(_CIVIL)
	nShowLt = SW_HIDE;
#else
	if (CDBCodeCtrl::IsStlCodeConsiderKtLt() ||
		CDBCodeCtrl::IsCfsCodeConsiderKtLt()) {
		nShowLt = SW_SHOW;
	}
#endif

	// Torsional Unbraced Length 관련 컨트롤들 표시/숨김
	GetDlgItem(IDC_DGN_BLT_STATIC)->ShowWindow(nShowLt);
	GetDlgItem(IDC_DGN_BLT_NAME)->ShowWindow(nShowLt);
	GetDlgItem(IDC_DGN_LENGTH_BLT)->ShowWindow(nShowLt);
	GetDlgItem(IDC_DGN_LENGTH_BLTUNIT)->ShowWindow(nShowLt);		
}

void CDgnGenLengDlg::OnDgnLengthAr()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnGenLengDlg::OnDgnLengthDel()
{
	UpdateData(TRUE);

	EnableDisableControls();
}

void CDgnGenLengDlg::OnDgnLengthNotlength()
{
	UpdateData(TRUE);

	GetDlgItem(IDC_DGN_LENGTH_BLU)->EnableWindow(TRUE);
	if (m_bNotUseBLu)	// Check(Not Consider)
	{
		m_BLu = 0.0;
		m_bAutoCalcLe = FALSE;
		GetDlgItem(IDC_DGN_LENGTH_BLU)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_LENGTH_AUTOCALC_BLE)->EnableWindow(FALSE);
	}
	else	// Not Check(Consider)
	{
		const BOOL bEnableBLe = [this]()
			{
				if (m_pDoc->IsBS5950_90()) return TRUE;
				if (m_pDoc->IsBS5950_2K()) return TRUE; // Coded by Seungjun MNET:No.2086 BS5950-2K ('20060630)
				if (m_pDoc->IsIS800_2007()) return TRUE;
				return FALSE;
			}();

		GetDlgItem(IDC_DGN_LENGTH_AUTOCALC_BLE)->EnableWindow(bEnableBLe);
	}
	UpdateData(FALSE);
}

void CDgnGenLengDlg::OnDgnLengthAutocalcBle()
{
	UpdateData(TRUE);

	if (m_bAutoCalcLe)
	{
		m_BLu = 0.0;
		GetDlgItem(IDC_DGN_LENGTH_BLU)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_LENGTH_BLU)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

void CDgnGenLengDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
	{
	case D_UPDATE_DEFAULT:
		// do something...
		break;
	case D_UPDATE_BUFFER_BEFORE:
		// do something...
		break;
	case D_UPDATE_BUFFER_AFTER:
	{
		// Add by ZINU.('01.1.3).
		Update_InitDataByCode();
		break;
	}
	case D_UPDATE_UNIT:
		// do something...
		break;
	default:
		//ASSERT(FALSE)->ASSERT(TRUE)로 임시변경 ('2000.2.10)
		ASSERT(TRUE);
	}
}

void CDgnGenLengDlg::OnDgnCPGElemTypeRdo()
{
	UpdateData(TRUE);

	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(m_nElemType == 0 ? EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM : EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_VBEM);

	EnableDisableControls();
}