// CMSectItemPSC.cpp : implementation file
//
// PSC 타입 추가시 검색 : 여기에 추가 *^^*
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSectItemPSC.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_SectDB.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "CMSectItmePSCView.h"

// 여기에 추가 *^^*
#include "CMSectItemDlg.h"
#include "CMSectItemPSCBase.h"
#include "CMSecPageStiffDlg.h"
#include "CMSectItemPSCNormal.h"
#include "CMSectItemPSCMid.h"
#include "CMSectItemPSC_I.h"
#include "CMSectItemPSC3Cell.h"
#include "CMSectItemPSC4Cell.h"
#include "CMSectItemPSCnCell.h"
#include "CMSectItemPSCTee.h"
#include "CMSectItemPSCPlat.h"
#include "CMSectItemPSCValue.h"
#include "CMSectItemPSC_CMP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

unsigned int CCMSectItemPSC::WM_AFTERINITDIALOG = RegisterWindowMessage(_T("WM_AFTERINITDIALOG"));

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC dialog
#define CDialog CCMSectItemBase

namespace SectItemPSCResource
{
	CString aImgPath[] = {
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc1c-1.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc3c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc9c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc10c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc4c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc8c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc5c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc6c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc7c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc11c.svg"),
	_T("SVG\\illustration\\ComboBox\\cmd_sec_psc12c.svg"),
	};
}

// 여기에 추가 *^^*
void CCMSectItemPSC::CreateChildDlgs()
{
	m_nCurChildDlg = -1;
	m_nInitChildDlg = 0;
	m_ComboMenuStr.RemoveAll();

	m_ChildDlgs.Add(new CCMSectItemPSCNormal(this, 0));
	m_DlgIDs.Add(__SECT_PSC_NML__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_1CELL)));

	m_ChildDlgs.Add(new CCMSectItemPSC3Cell(this));
	m_DlgIDs.Add(__SECT_PSC_NML_3_CELL__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_3CELL)));

	m_ChildDlgs.Add(new CCMSectItemPSC4Cell(this));
	m_DlgIDs.Add(__SECT_PSC_NML_4_CELL__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_NCELL)));

	m_ChildDlgs.Add(new CCMSectItemPSCnCell(this));
	m_DlgIDs.Add(__SECT_PSC_n_CELL__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_NCEL2)));

	m_ChildDlgs.Add(new CCMSectItemPSCMid(this));
	m_DlgIDs.Add(__SECT_PSC_MID__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_MID)));

	m_ChildDlgs.Add(new CCMSectItemPSC_I(this));
	m_DlgIDs.Add(__SECT_PSC_I__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_I)));

	m_ChildDlgs.Add(new CCMSectItemPSCNormal(this, 1));
	m_DlgIDs.Add(__SECT_PSC_HLF__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_HALF)));

	m_ChildDlgs.Add(new CCMSectItemPSCTee(this));
	m_DlgIDs.Add(__SECT_PSC_TEE__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_TEE)));

	m_ChildDlgs.Add(new CCMSectItemPSCPlat(this));
	m_DlgIDs.Add(__SECT_PSC_PLAT__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_PLAT)));

	m_ChildDlgs.Add(new CCMSectItemPSCValue(this));
	m_DlgIDs.Add(__SECT_PSC_VALUE__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_VALUE)));

	m_ChildDlgs.Add(new CCMSectItemPSC_CMP(this));
	m_DlgIDs.Add(__SECT_PSC_CMPWEB__);
	m_ComboMenuStr.Add(CString(_LS(IDS_CMD_SECT_PSC_CMPWEB)));
}

CCMSectItemPSC::CCMSectItemPSC(CWnd* pParent /*=NULL*/)
	: CDialog(CCMSectItemPSC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMSectItemPSC)
	//}}AFX_DATA_INIT

	// Create Child Dialog
	CreateChildDlgs();

	// Data Initialize
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_PSC;
	m_bCreateView = FALSE;

	m_pImageList = NULL;
	m_pViewer= NULL;

	m_pParent = NULL;
	ASSERT(pParent);
	m_pParent = (CCMSectItemDlg*)pParent;
}

void CCMSectItemPSC::DeleteChildDlgs()
{
	int nCount = m_ChildDlgs.GetSize();
	for(int i = 0 ; i < nCount ; i++)
	{
		delete m_ChildDlgs[i];
	}
}

CCMSectItemPSC::~CCMSectItemPSC()
{
	DeleteChildDlgs();

	//_KJH
	if(m_pImageList != NULL){delete m_pImageList; m_pImageList=NULL;}
	//
}

void CCMSectItemPSC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMSectItemPSC)
	DDX_Control(pDX, IDC_CMD_MESH_SIZE_UNIT, m_unitMeshSize);
	DDX_Control(pDX, IDC_CMD_MESH_SIZE_CHK, m_chkMeshSize);
	DDX_Control(pDX, IDC_CMD_MESH_SIZE_EDIT, m_editMeshSize);
	DDX_Control(pDX, IDC_CMD_PSC_TYPE_CB, m_cTypeCB);
	DDX_Control(pDX, IDC_CMD_PLACEHOLDER, m_wndHolder);
	DDX_Control(pDX, IDC_CMD_SP_ID_NAME, m_wndName);
	DDX_Control(pDX, IDC_CMD_SP_ID_ID, m_wndID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMSectItemPSC, CDialog)
	//{{AFX_MSG_MAP(CCMSectItemPSC)
	ON_CBN_SELCHANGE(IDC_CMD_PSC_TYPE_CB, OnSelchangeCmdPscTypeCb)
	ON_BN_CLICKED(IDC_CMD_MESH_SIZE_CHK, OnCmdMeshSizeChk)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(CCMSectItemPSC::WM_AFTERINITDIALOG, OnAfterInitDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMSectItemPSC message handlers
void CCMSectItemPSC::SetIDName(/*UINT nID, */CString strName)
{
	HWND hWnd = this->GetSafeHwnd();
	if (hWnd && ::IsWindow(hWnd))
	{
		m_wndName.SetWindowText(strName);
	}
}

void CCMSectItemPSC::InitSectionTypeCombo()
{
	m_cTypeCB.ResetContent();

	int nSize = m_ComboMenuStr.GetSize();
	m_cTypeCB.SetImageList(SectItemPSCResource::aImgPath, nSize);
	m_cTypeCB.SetComboHeight(globalUtils.ScaleByDPI(24));

	InsertItems();

	m_cTypeCB.SetCurSel(m_nInitChildDlg);
}

//_KJH//
void CCMSectItemPSC::InsertItems()
{
	// 상수 사용을 억제를 위해 다음과 같은 방식으로 변경 
	int nSize = m_ComboMenuStr.GetSize();
	m_cTypeCB.SetRedraw(FALSE);
	for(int i=0; i<nSize; i++)
	{
		m_cTypeCB.AddString(m_ComboMenuStr[i]);
	}
	m_cTypeCB.SetRedraw(TRUE);
}

void CCMSectItemPSC::InitChildDialog(BOOL bModify)
{
	T_SECT_D tempData;
	for (int i=0; i<m_ChildDlgs.GetSize(); i++)
	{
		tempData.Initialize();
		if (CProduct::GetMovingType() == D_PRODUCT_MOVING_JP)
			tempData.SectBefore.bConsiderShearDeform = FALSE;

		if (m_nInitChildDlg == i) 
			m_ChildDlgs[i]->SetCurDlgData(&m_Data, bModify);
		else                     
			m_ChildDlgs[i]->SetCurDlgData(&tempData, bModify);
	}
	m_cTypeCB.SetCurSel(m_nInitChildDlg);
	ShowCurChildDlg(m_nInitChildDlg);
}

void CCMSectItemPSC::CreatePSCView()
{
	// Window가 이미 있으면 만들지 않는다.
	if(IsViewWindow()) return;
 
	m_pViewer = new CCMSectItmePSCView();
	m_pViewer->SetInitPos(D_INIT_POS_RT);
	m_pViewer->Create(IDD_CMD_SECT_ITEM_PSC_VIEW, this);
	m_pViewer->ShowWindow(SW_SHOW);
	m_bCreateView = TRUE;
}

void CCMSectItemPSC::DestroyPSCView()
{
	// Window가 있으면 없앤다.
	if(m_pViewer != NULL)
	{
		m_pViewer->DestroyWindow();
		//-------------------------------------------
		// m_pViewer가 CCMDlgBase를 상속받았으므로
		// 명시적인 delete을 하면 안된단다.. 이크~~~ 
		// delete m_pViewer;    
		m_pViewer = NULL;    
	}
}

// 여기에 추가 *^^*
BOOL CCMSectItemPSC::OnInitDialog() 
{
	SetRedraw(FALSE);
	
	CDialog::OnInitDialog();

	m_unitMeshSize.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.dMeshSize);
	m_editMeshSize.SetUnitType(CUnitCtrl::m_SECT_UNIT.SectBefore.dMeshSize);

	if (!m_bModify && !m_bInitDone)
	{
		// 시작 번호는 현재 번호에서 DB가 생성해 주는 다음 번호로 설정
		if(m_bWorkTab)  m_Key = m_FixID;
		else            m_Key = m_pDoc->m_pAttrCtrl->GetStartNumSect();
	}
	// ID
	ShowKey2Dlg();

	// Name
	m_wndName.SetWindowText(m_Data.SName);

	// Shape
	// 여기추가 *^^*
	if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_1CELL ||
		 m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_2CELL)       m_nInitChildDlg = 0;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_3CELL)  m_nInitChildDlg = 1;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_4CELL)  m_nInitChildDlg = 2;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_NCEL2)  m_nInitChildDlg = 3;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_MID)    m_nInitChildDlg = 4;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_I)      m_nInitChildDlg = 5;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_HALF)   m_nInitChildDlg = 6;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_T)      m_nInitChildDlg = 7;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_BOX)    m_nInitChildDlg = 8;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)  m_nInitChildDlg = 9;
	else if(m_Data.SectBefore.Shape == D_SECT_SHAPE_PSC_CMPWEB) m_nInitChildDlg =10;
	 
	// Section Type
	InitSectionTypeCombo();
	
	// Child Dialog
	//InitChildDialog(m_bModify);
	this->PostMessage(WM_AFTERINITDIALOG);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 뷰의 위치문제로 인해 사용자 정의 메세지(OnInitDialog 직후에 날아옴)를 이용
LRESULT CCMSectItemPSC::OnAfterInitDialog(WPARAM wParam, LPARAM lParam)
{
	CreatePSCView();

	InitChildDialog(m_bModify);
	// mesh size
	if (IsPSCValue()) 
	{
		m_chkMeshSize.SetCheck(0);
		m_chkMeshSize.EnableWindow(FALSE);
	}
	else
	{
		m_chkMeshSize.SetCheck(m_Data.SectBefore.bUserDefMeshSize);
		if (m_Data.SectBefore.bUserDefMeshSize)
			m_editMeshSize.SetEditUnit(m_Data.SectBefore.dMeshSize);
	}
	OnCmdMeshSizeChk();
	
	return 0L;
}


/////////////////////////////////////////////////////////////////////////////
//
// Destroy
//
BOOL CCMSectItemPSC::ChildDlg2Data()
{
	if (m_nCurChildDlg<0) return FALSE;
	return m_ChildDlgs[m_nCurChildDlg]->GetDlgData(&m_Data);
}

BOOL CCMSectItemPSC::Dlg2Data()
{
	// stiffness data backup
	BOOL bUserStiff;
	T_SECT_STIFFNESS aUserStiff[2];
	bUserStiff = m_Data.SectBefore.bUserInputStiffness;
	if (bUserStiff)
	{
		aUserStiff[0] = m_Data.SectBefore.SectI.Stiffness;
		aUserStiff[1] = m_Data.SectBefore.SectJ.Stiffness;
	}

	UpdateData(TRUE);
	m_Data.Initialize();
	m_Data.nStype = D_SECT_TYPE_PSC;

	// ID
	CString csID;
	m_wndID.GetWindowText(csID);
	m_Key = (T_SECT_K)_tstol(csID);

	// Name
	m_wndName.GetWindowText(m_Data.SName);
	m_Data.SName.TrimLeft(); m_Data.SName.TrimRight();

	// mesh size
	m_Data.SectBefore.bUserDefMeshSize = (m_chkMeshSize.GetCheck() == 1) ? TRUE: FALSE;
	if (m_Data.SectBefore.bUserDefMeshSize)
		m_Data.SectBefore.dMeshSize = m_editMeshSize.GetEditValue();

	// Child Dialog
	ChildDlg2Data();

	// stiffness data
	m_Data.SectBefore.bUserInputStiffness = bUserStiff;
	if (bUserStiff)
	{
		m_Data.SectBefore.SectI.Stiffness = aUserStiff[0];
		m_Data.SectBefore.SectJ.Stiffness = aUserStiff[1];
	}

	return TRUE;
}

//BOOL CCMSectItemPSC::DestroyWindow() 
//{
//	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
//		m_ChildDlgs[m_nCurChildDlg]->End();
//
//	m_nCurChildDlg = -1;
//	m_nInitChildDlg = 0;
//	
//	if(m_bCreateView)
//	{
//		DestroyPSCView();
//		m_bCreateView = FALSE;
//	}
//	return CDialog::DestroyWindow();
//}

BOOL CCMSectItemPSC::DestroyWindow()
{
	if (m_bCreateView)
	{
		DestroyPSCView();
		m_bCreateView = FALSE;
	}
	return CDialog::ShowWindow(SW_HIDE);
}


/////////////////////////////////////////////////////////////////////////////
//
// Overrides
BOOL CCMSectItemPSC::GetPeriAndStiff(BOOL bReCalc/*=FALSE*/)
{
	CSectUtil::Do7thDOFLoopForMeshSizeFind(m_Data);

	// PSC Value도 Qy 등을 계산하기 위해 CSectUtil::CalcSectData 호출한다.
	// CalcSectData안에서 관련 없는 값은 변경하지 않는다. by jbseon 05.03.10 with zinulee
	//if (!IsPSCValue())    // PSC Value 이면 단면 강성은 입력된 값을 사용한다.
	{
		if (!CSectUtil::CalcSectData(m_Data, TRUE, FALSE, FALSE, bReCalc)) return FALSE;
	}

	if (m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
	{
		m_Data.SectBefore.dShearCheckPos[0][1] = m_Data.SectBefore.SectI.Design.ZBar;
		m_ChildDlgs[m_nCurChildDlg]->SetAutotShearCheckPos(m_Data.SectBefore.dShearCheckPos[0]);
		m_ChildDlgs[m_nCurChildDlg]->SetAutoMinWebThick(m_Data.SectBefore.dWebThkShear[0], 
																										 m_Data.SectBefore.dWebThk[0]);

		m_ChildDlgs[m_nCurChildDlg]->SetWarpingCheckPosition(m_Data.SectBefore.dWarpingCheckPosI, m_Data.SectBefore.dWarpingCheckPosJ);

		m_ChildDlgs[m_nCurChildDlg]->OnAfterCalcPeriAndStiff(&m_Data.SectBefore);
	}

	return TRUE;
}

BOOL CCMSectItemPSC::SaveData()
{
	if (!Dlg2Data()) return FALSE;
	if (m_Data.SName == _T(""))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_section_name_));
		return FALSE;
	}

	T_SECT_STIFFNESS StiffBackup[2];
	if (m_Data.SectBefore.bUserInputStiffness)
	{
		StiffBackup[0] = m_Data.SectBefore.SectI.Stiffness;
		StiffBackup[1] = m_Data.SectBefore.SectJ.Stiffness;
	}

	if (!GetPeriAndStiff()) return FALSE;

	if(!Check_ShearPos()) return FALSE;

	if (m_Data.SectBefore.bUserInputStiffness)
	{
		T_SECT_STIFFNESS* pStiff;
		pStiff = &m_Data.SectBefore.SectI.Stiffness;
		pStiff->Area  = StiffBackup[0].Area; 
		pStiff->WArea = StiffBackup[0].WArea; 
		pStiff->Asy   = StiffBackup[0].Asy; 
		pStiff->Asz   = StiffBackup[0].Asz; 
		pStiff->Rxx   = StiffBackup[0].Rxx; 
		pStiff->Ryy   = StiffBackup[0].Ryy; 
		pStiff->Rzz   = StiffBackup[0].Rzz; 
		
		pStiff = &m_Data.SectBefore.SectJ.Stiffness;
		pStiff->Area  = StiffBackup[1].Area; 
		pStiff->WArea = StiffBackup[1].WArea; 
		pStiff->Asy   = StiffBackup[1].Asy; 
		pStiff->Asz   = StiffBackup[1].Asz; 
		pStiff->Rxx   = StiffBackup[1].Rxx; 
		pStiff->Ryy   = StiffBackup[1].Ryy; 
		pStiff->Rzz   = StiffBackup[1].Rzz; 
	}

	if (m_bModify)
	{
		if (m_bDesign) return m_pDoc->m_pDataCtrl->AddSecd(m_Key, m_Data, m_bVirtual);
		else return m_pDoc->m_pDataCtrl->ModifySect(m_OrgKey, m_Key, m_Data);
	}
	else return m_pDoc->m_pDataCtrl->AddSect(m_Key, m_Data);
}


BOOL CCMSectItemPSC::Check_ShearPos()
{
	if(m_Data.SectBefore.dShearCheckPos[0][2] <0.0 ||
		 m_Data.SectBefore.dShearCheckPos[0][0] <0.0) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Invalid_Shear_check_positions )); // _T("Shear check positions must be over zero.")
		return FALSE;
	}
	
	return TRUE;
}

BOOL CCMSectItemPSC::IsPSCValue()
{
	if(m_nCurChildDlg >= 0 && m_DlgIDs.GetSize()-1 >= m_nCurChildDlg)
	{
		if (m_DlgIDs[m_nCurChildDlg] == __SECT_PSC_VALUE__) return TRUE;
	}
	return FALSE;
}

BOOL CCMSectItemPSC::OnOKPublic()
{
	return SaveData();
}

BOOL CCMSectItemPSC::OnApplyPublic()
{
	if (!SaveData()) return FALSE;
	if (!m_bModify)  // modify 면 그냥 두고 add면 재 초기화 
	{
		// ID만 수정한다.
		m_Key = m_pDoc->m_pAttrCtrl->GetNextNumSect(m_Key);
		ShowKey2Dlg();
	}
	else
	{
		m_OrgKey = m_Key;  // Change Origin Key
	}
	return TRUE;
}

void CCMSectItemPSC::ShowCalcResult()
{
	CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&> aUserStiff, aOrgStiff;
	aUserStiff.SetSize(2);
	aOrgStiff.SetSize(2);
	BOOL bUserInput = m_Data.SectBefore.bUserInputStiffness;
	if (bUserInput) 
	{
		aUserStiff[0] = m_Data.SectBefore.SectI.Stiffness;
		aUserStiff[1] = m_Data.SectBefore.SectJ.Stiffness;
	}
	
	if (!Dlg2Data() || !GetPeriAndStiff())
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}

	m_Data.SectBefore.bUserInputStiffness = bUserInput;
	aOrgStiff[0] = m_Data.SectBefore.SectI.Stiffness;
	aOrgStiff[1] = m_Data.SectBefore.SectJ.Stiffness;
	if (bUserInput) 
	{
		m_Data.SectBefore.SectI.Stiffness = aUserStiff[0];
		m_Data.SectBefore.SectJ.Stiffness = aUserStiff[1];
	}
	
	CCMStiffDlg dlg(m_pDoc, &m_Data, &aOrgStiff, this);
	dlg.DoModal();
}

void CCMSectItemPSC::DisplayOffsetPoint()
{
	if (m_nCurChildDlg < 0) return;
	m_ChildDlgs[m_nCurChildDlg]->DisplayOffsetPoint();
}

void CCMSectItemPSC::SetNewID(unsigned nNewID)
{
	m_Key = nNewID;
	ShowKey2Dlg();
}

void CCMSectItemPSC::ShowKey2Dlg()
{
	if(m_wndID.GetSafeHwnd() == nullptr) return;
	CString strID;

	strID.Format(_T("%d"), m_Key);
	m_wndID.SetWindowText(strID);
}

/////////////////////////////////////////////////////////////////////////////
//
// Change Child Dialog
//
void CCMSectItemPSC::ShowCurChildDlg(int nDlgIndex)
{
	if(m_nCurChildDlg == nDlgIndex) return;
		
	if(nDlgIndex > m_ChildDlgs.GetSize() - 1) 
	{  
		m_nCurChildDlg = nDlgIndex;
		return;
	}

	if(m_nCurChildDlg >= 0 && m_ChildDlgs.GetSize()-1 >= m_nCurChildDlg)
		m_ChildDlgs[m_nCurChildDlg]->End();

	SetRedraw(FALSE);

	CRect WRect;
	m_wndHolder.GetWindowRect(&WRect);
	ScreenToClient(WRect);

	m_ChildDlgs[nDlgIndex]->CreateInit(this,WRect.left,WRect.top);
	if (m_bModify) 
	{
		m_ChildDlgs[nDlgIndex]->SetAutotShearCheckPos(m_Data.SectBefore.dShearCheckPos[0]);
		m_ChildDlgs[nDlgIndex]->SetAutoMinWebThick(m_Data.SectBefore.dWebThkShear[0], m_Data.SectBefore.dWebThk[0]);
	}
	m_nCurChildDlg = nDlgIndex;
	BOOL bValue = IsPSCValue();
	m_pParent->ExternalSetCalcBtn(!bValue);

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
}

void CCMSectItemPSC::OnSelchangeCmdPscTypeCb() 
{
	int nIndex = m_cTypeCB.GetCurSel();
	if(nIndex<0 || nIndex>= m_ChildDlgs.GetSize()) return;

	// 해당 다이얼로그 보여주기
	ShowCurChildDlg(nIndex);
	
	// turn off user input stiff. option
	m_Data.SectBefore.bUserInputStiffness = FALSE;

	// for PSC-Value section
	BOOL bValue = IsPSCValue();
	if (bValue) m_chkMeshSize.SetCheck(0);
	m_chkMeshSize.EnableWindow(!bValue);
	m_editMeshSize.EnableWindow(!bValue);
	m_pParent->ExternalSetCalcBtn(!bValue);
	OnCmdMeshSizeChk();
	if (m_pViewer) 
		m_pViewer->ShowSectView(!bValue);
}

/////////////////////////////////////////////////////////////////////////////
//
// Update Viewer
//

BOOL CCMSectItemPSC::IsViewWindow()
{ 
	if(m_pViewer == NULL) return FALSE;
	HWND hWnd = m_pViewer->GetSafeHwnd();
	if(!hWnd) return FALSE;
	if(!(::IsWindow(hWnd))) return FALSE;

	return TRUE;
}

BOOL CCMSectItemPSC::ChangeBitmap(UINT nIndex)
{
	if(!IsViewWindow()) return FALSE;
	
	return m_pViewer->ChangeBitmap(nIndex); 
 
	return TRUE;
}

void CCMSectItemPSC::SetDataSource(T_SECT_D* pDataSrc)
{
	if(m_pViewer == NULL) return ;

	m_pViewer->SetDataSource(pDataSrc);
}

void CCMSectItemPSC::RedrawSection()
{
	if(!IsViewWindow()) return;

	m_pViewer->RedrawSection();
}

void CCMSectItemPSC::DisplayCentoid()
{
	if (m_pViewer == NULL) return ;
	m_pViewer->RedrawSection(TRUE);
}

void CCMSectItemPSC::DisplayShearCheck()
{
	if (m_pViewer == NULL) return ;
	m_wndSecView.SetShearCheckFlag(TRUE);
}

void CCMSectItemPSC::ShowSectView(BOOL bShow)
{
	if (!IsViewWindow()) return;
	m_pViewer->ShowSectView(bShow);
}

void CCMSectItemPSC::OnCmdMeshSizeChk() 
{
	// TODO: Add your control notification handler code here
	int nCheck = m_chkMeshSize.GetCheck();
	BOOL bEnable = (nCheck == 1);
	m_editMeshSize.EnableWindow(bEnable);
	if (!bEnable) m_editMeshSize.SetWindowText(_T(""));	
}

void CCMSectItemPSC::OnCalculateButton()
{
	int nIndex = m_cTypeCB.GetCurSel();
	BOOL bError=FALSE;
	if(!IsPSCValue()){ASSERT(0);return;}
	if(Dlg2Data())
	{
		if(m_Data.SectBefore.SectI.aOuterPolygon.GetSize()==0)
			bError = TRUE;
		else if(!GetPeriAndStiff(TRUE))
			bError = TRUE; 
	}
	else
		bError =TRUE;
	if(bError)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Can_t_calculate_stiffness));
		return;
	}
	double dQy,dQz;
	if (CSectUtil::GetPscValueDataQ(&m_Data, dQy, dQz))
	{
		m_Data.SectBefore.SectI.Stiffness.Qyb = dQy;
		m_Data.SectBefore.SectI.Stiffness.Qzb = dQz;
	}
	m_ChildDlgs[nIndex]->SetCurDlgData(&m_Data, TRUE);
	((CCMSectItemPSCValue*)m_ChildDlgs[nIndex])->ExternalShowData();
	
}

BOOL CCMSectItemPSC::ShowWindow(int nCmdShow)
{
	if (m_bCreateView)
	{
		DestroyPSCView();
		m_bCreateView = FALSE;
	}
	return CDialog::ShowWindow(nCmdShow);
}