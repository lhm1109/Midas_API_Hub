// ElemDepMatChange.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemDepMatChange.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_base\wg_base_StrParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CODE_ITEM_CEB_FIP       _T("CEB-FIP(1990)")
#define CODE_ITEM_KOREA_STAND   _LS(IDS_TM__Korean_Standard)
#define CODE_ITEM_CHINA_STAND   _LS(IDS_TM__Chinese_Standard)
#define CODE_ITEM_JAPAN_STAND   _LS(IDS_TM__Japanese_Standard)
// My add
#define NOVOL_ITEM_NATIONAL_SIZE   _LS(IDS_TM_Change_Prop_Nation_Size)     
#define NOVOL_ITEM_VOLUME_SURFACE  _LS(IDS_TM_Change_Prop_Volume_Surface)     
#define CODE_ITEM_ACI					_LS(IDS_TM_Change_Prop_ACI)
#define CODE_ITEM_PCA					_LS(IDS_TM_Change_Prop_PCA)
#define CODE_ITEM_COM					_LS(IDS_TM_Change_Prop_Combined)

//"ACI" "PCA" "Combined(ACI & PCA)" "Notational Size of Member" "Volume Surface Ratio"
/////////////////////////////////////////////////////////////////////////////
// CElemDepMatChange dialog
#define CDialog CMenuBarChildDlg

CElemDepMatChange::CElemDepMatChange(CWnd* pParent /*=NULL*/)
	: CDialog(CElemDepMatChange::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemDepMatChange)
	m_nOption = 1;
	m_nAddOrDel = 0;
	//}}AFX_DATA_INIT
}

CElemDepMatChange::~CElemDepMatChange()
{


}

void CElemDepMatChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemDepMatChange)
	DDX_Control(pDX, IDC_TM_NOVOL_COMBO, m_novCode);
	DDX_Control(pDX, IDC_TM_LENGTH_STATIC, m_LengthUnit);
	DDX_Control(pDX, IDC_TM_H_EDIT, m_wndHEdit);
	DDX_Control(pDX, IDC_TM_A_EDIT, m_wndAEdit);
	DDX_Control(pDX, IDC_TM_CODE_COMBO, m_cboCode);
	DDX_Radio(pDX, IDC_TM_INPUT_RADIO, m_nOption);
	DDX_Radio(pDX, IDC_TM_ADD_REP_RADIO, m_nAddOrDel);
	//}}AFX_DATA_MAP
}

//////////////////////////////////////////////////////////////////////////////////////
//
// User Defined Function

void CElemDepMatChange::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_cboCode.GetDC();
	for (int i=0;i < m_cboCode.GetCount();i++)
	{
		m_cboCode.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_cboCode.ReleaseDC(pDC);

	if (m_cboCode.GetDroppedWidth() < dx)
	{
		m_cboCode.SetDroppedWidth(dx);
		ASSERT(m_cboCode.GetDroppedWidth() == dx);
	}
}



void CElemDepMatChange::InitCodeCombo()
{
	m_cboCode.ResetContent();

#if defined(_US) || defined(_RUS)
		m_cboCode.AddString(CODE_ITEM_CEB_FIP);
#ifdef _CIVIL
		m_cboCode.AddString(CODE_ITEM_JAPAN_STAND);
		m_cboCode.AddString(CODE_ITEM_CHINA_STAND);
		m_cboCode.AddString(CODE_ITEM_KOREA_STAND);
#endif // _CIVIL

#elif defined(_CH)
		m_cboCode.AddString(CODE_ITEM_CHINA_STAND);
		m_cboCode.AddString(CODE_ITEM_CEB_FIP);
		m_cboCode.AddString(CODE_ITEM_JAPAN_STAND);
		m_cboCode.AddString(CODE_ITEM_KOREA_STAND);
#elif defined(_JP)
		m_cboCode.AddString(CODE_ITEM_JAPAN_STAND);
		m_cboCode.AddString(CODE_ITEM_CEB_FIP);
		m_cboCode.AddString(CODE_ITEM_KOREA_STAND);
#else
		m_cboCode.AddString(CODE_ITEM_KOREA_STAND);
		m_cboCode.AddString(CODE_ITEM_CEB_FIP);
		m_cboCode.AddString(CODE_ITEM_JAPAN_STAND);
		m_cboCode.AddString(CODE_ITEM_CHINA_STAND);
#endif

	m_cboCode.SetCurSel(0);
	AdjustComboListBox();
	ChangeFomulaByCode();
}

// My add
void CElemDepMatChange::InitNovolCombo()
{
	m_novCode.ResetContent();
	
	//차후 언어별 수정 요함 
	m_novCode.AddString(NOVOL_ITEM_NATIONAL_SIZE);
	m_novCode.AddString(NOVOL_ITEM_VOLUME_SURFACE);

	m_novCode.SetCurSel(0);
	ChangeFomulaByCode();
}

// 코드 선택에 따라 계산식을 보여주는 함수
void CElemDepMatChange::ChangeFomulaByCode()
{
	
	CString strCode = _T("");
	int nIndex = m_cboCode.GetCurSel();
	if(nIndex>=0) m_cboCode.GetLBText(nIndex, strCode);

	CWnd* pWnd[8]; 
	pWnd[0] = GetDlgItem(IDC_TM_FOMULA_STATIC1) ; ASSERT(pWnd[0]);  // h = 2 * Ac / u
	pWnd[1] = GetDlgItem(IDC_TM_FOMULA_STATIC2) ; ASSERT(pWnd[1]);  // h = Ac / u
	pWnd[2] = GetDlgItem(IDC_TM_FOMULA_STATIC3) ; ASSERT(pWnd[2]);  // v / s
	pWnd[3] = GetDlgItem(IDC_TM_FOMULA2_STATIC1); ASSERT(pWnd[3]);  // h :
	pWnd[4] = GetDlgItem(IDC_TM_FOMULA2_STATIC2); ASSERT(pWnd[4]);  // v/s:
	pWnd[5] = GetDlgItem(IDC_TM_FORMULA_TEXT1)  ; ASSERT(pWnd[5]);  // u = Lo + a*Li
	pWnd[6] = GetDlgItem(IDC_TM_FORMULA_TEXT2)  ; ASSERT(pWnd[6]);  // a
	pWnd[7] = GetDlgItem(IDC_TM_A_EDIT)         ; ASSERT(pWnd[7]);  // edit a
	
	// 일본 코드일때만 다르다.
		
	int nIndex2 = m_novCode.GetCurSel();
	
	if(nIndex2 == 1)   // volume surface ratio
	{
		pWnd[0]->ShowWindow(SW_HIDE);
		pWnd[1]->ShowWindow(SW_HIDE);
		pWnd[2]->ShowWindow(SW_SHOW);
		pWnd[3]->ShowWindow(SW_HIDE);
		pWnd[4]->ShowWindow(SW_SHOW);	
		pWnd[5]->ShowWindow(SW_HIDE);	
		pWnd[6]->ShowWindow(SW_HIDE);	
		pWnd[7]->ShowWindow(SW_HIDE);	
	}
	else   // notational size of member
	{
		pWnd[2]->ShowWindow(SW_HIDE);
		pWnd[3]->ShowWindow(SW_SHOW);
		pWnd[4]->ShowWindow(SW_HIDE);
			if(strCode == CODE_ITEM_JAPAN_STAND)
			{
				pWnd[0]->ShowWindow(SW_HIDE);
				pWnd[1]->ShowWindow(SW_SHOW);
			}
			else
			{
				pWnd[0]->ShowWindow(SW_SHOW);
				pWnd[1]->ShowWindow(SW_HIDE);
			}
		pWnd[5]->ShowWindow(SW_SHOW);	
		pWnd[6]->ShowWindow(SW_SHOW);	
		pWnd[7]->ShowWindow(SW_SHOW);	
	}
}


//위쪽의 큰 콤보가 바귈때 각 콤보의 초기화및 알맞은 계산식 호출 
void CElemDepMatChange::ChangeChildByCode()
{
	int nIndex = m_novCode.GetCurSel();
	if (nIndex == 0)
	{
		InitCodeCombo();		
	}
	else
	{	
	  m_cboCode.ResetContent();

	  //차후 언어별 수정 요함 
	  m_cboCode.AddString(CODE_ITEM_ACI);
	  m_cboCode.AddString(CODE_ITEM_PCA);
	  m_cboCode.AddString(CODE_ITEM_COM);

	  m_cboCode.SetCurSel(0);
	  AdjustComboListBox();
	  ChangeFomulaByCode();		    
	}	
}


// Control을 정렬시켜주는 함수 
void CElemDepMatChange::AlignControls()
{
	CRect FromRect, FromRect2, ToRect ,FromRect3, ToRect3;
	GetDlgItem(IDC_TM_FOMULA_STATIC1)->GetWindowRect(&ToRect);
	GetDlgItem(IDC_TM_FOMULA_STATIC2)->GetWindowRect(&FromRect);
	GetDlgItem(IDC_TM_FOMULA_STATIC3)->GetWindowRect(&FromRect2);
	GetDlgItem(IDC_TM_FOMULA2_STATIC1)->GetWindowRect(&ToRect3);
	GetDlgItem(IDC_TM_FOMULA2_STATIC2)->GetWindowRect(&FromRect3);
		

	int nDistY  = ToRect.top - FromRect.top;
	int nDistY2 = ToRect.top - FromRect2.top;
	int nDistY3 = ToRect3.top - FromRect3.top;
	

	CArray<UINT,UINT> arFomulaCtrl;
	arFomulaCtrl.Add(IDC_TM_FOMULA_STATIC2);
	CDlgUtil::CtrlMoveDistY(this, arFomulaCtrl, nDistY);

	CArray<UINT,UINT> arFomulaCtrl2;
	arFomulaCtrl2.Add(IDC_TM_FOMULA_STATIC3);
	CDlgUtil::CtrlMoveDistY(this, arFomulaCtrl2, nDistY2);

	CArray<UINT,UINT> arFomulaCtrl3;
	arFomulaCtrl3.Add(IDC_TM_FOMULA2_STATIC2);
	CDlgUtil::CtrlMoveDistY(this, arFomulaCtrl3, nDistY3);
}


BEGIN_MESSAGE_MAP(CElemDepMatChange, CDialog)
	//{{AFX_MSG_MAP(CElemDepMatChange)
	ON_BN_CLICKED(IDC_TM_INPUT_RADIO, OnOptionRadio)
	ON_BN_CLICKED(IDC_TM_ADD_REP_RADIO, OnTmAddDelRadio)
	ON_CBN_SELCHANGE(IDC_TM_CODE_COMBO, OnSelchangeTmCodeCombo)
	ON_BN_CLICKED(IDC_TM_AUTOCALC_RADIO, OnOptionRadio)
	ON_BN_CLICKED(IDC_TM_DELETE_RADIO, OnTmAddDelRadio)
	ON_CBN_SELCHANGE(IDC_TM_NOVOL_COMBO, OnSelchangeTmNovolCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemDepMatChange message handlers

BOOL CElemDepMatChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_LengthUnit.SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_wndHEdit  .SetUnitType(CUnitCtrl::m_LENGTH_UNIT);
	m_wndAEdit  .SetUnitType(D_UNITSYS_NONE);
#if defined(_JP)
	m_wndAEdit.SetEditUnit(1.0);
#else
	m_wndAEdit.SetEditUnit(0.5);
#endif
	
	AlignControls();
	InitCodeCombo();
	InitNovolCombo();

	//  OptionCtrlMan();
	AddDelCtrlMan();
/*
#if defined(_MGEN)
	GetDlgItem(IDC_TM_INPUT_RADIO)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_AUTOCALC_RADIO)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_FOMULA2_STATIC1)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_H_EDIT)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_LENGTH_STATIC)->ShowWindow(FALSE);
	GetDlgItem(IDC_TM_FOMULA2_STATIC2)->ShowWindow(FALSE);  
#endif
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemDepMatChange::OnOptionRadio() 
{
	UpdateData(TRUE);
	//AddDelCtrlMan();
	OptionCtrlMan();
}

void CElemDepMatChange::OptionCtrlMan()
{
	if(m_nOption == 0)
	{
		m_wndHEdit.EnableWindow(TRUE);
		m_cboCode.EnableWindow(FALSE);
		m_wndAEdit.EnableWindow(FALSE);
	}
	else
	{
		m_wndHEdit.EnableWindow(FALSE);
		m_cboCode.EnableWindow(TRUE);
		m_wndAEdit.EnableWindow(TRUE);
	}
}

void CElemDepMatChange::OnTmAddDelRadio() 
{
	UpdateData();
	AddDelCtrlMan();
	//OptionCtrlMan();
}

void CElemDepMatChange::AddDelCtrlMan()
{
	UpdateData(TRUE);

	CArray<UINT, UINT> Ctrls;
	Ctrls.Add(IDC_TM_NOVOL_COMBO);
	Ctrls.Add(IDC_TM_INPUT_RADIO);
//#if defined(_CIVIL)
	Ctrls.Add(IDC_TM_AUTOCALC_RADIO);
//#endif
	Ctrls.Add(IDC_TM_H_EDIT);
	Ctrls.Add(IDC_TM_CODE_COMBO);
	Ctrls.Add(IDC_TM_A_EDIT);

	if(m_nAddOrDel == 0)
	{
		CtrlEnableDisable(Ctrls, TRUE);
		OptionCtrlMan();
//#if defined(_CIVIL)
		GetDlgItem(IDC_TM_AUTOCALC_RADIO)->EnableWindow(m_novCode.GetCurSel() == 0);      
//#endif    
	}
	else
	{ 
		CtrlEnableDisable(Ctrls, FALSE);    
		GetDlgItem(IDC_TM_AUTOCALC_RADIO)->EnableWindow(FALSE);
	}
}


void CElemDepMatChange::Execute()
{
	UpdateData();
	
	double H = this->m_wndHEdit.GetEditValue();
	double A = this->m_wndAEdit.GetEditValue();
		
	CArray<UINT,UINT> EList;
	
	I_GENModelBase::GetCurMySelfST()->GetSelectedElemKeyList(EList);

	CArray<T_ELEM_K, T_ELEM_K> aElemList;
	T_ELEM_D ElemD;
	int nE = EList.GetSize();
	// FrameType Element만 Filtering 할것..!!
	for(int i = 0; i< nE; i++)
	{
		if(CDBDoc::GetDocPoint()->m_pAttrCtrl->GetElem(EList[i],ElemD))
			if(CDBDoc::GetDocPoint()->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
				aElemList.Add(EList[i]);
	}
	
	if(aElemList.GetSize()==0)
	{
		GSaveHistoryFormatNF(_LS(IDS_TM_ERR__No_Selected_Frame));
		return;
	}
	
	T_EDMP_D EdmpD;
	CArray<double , double >   aNotSizes;
	CArray<T_EDMP_D, T_EDMP_D&> EdmpDatas;
	
	EdmpD.nType = GetTypeComboData();

	if (m_nAddOrDel == 0) // Add/Replace
	{
		int nElem = aElemList.GetSize();
		
		if(m_nOption == 1) // Auto Calculation
		{
			// Volume-Surface Ratio인 경우 Auto Calc. 불가
			if (EdmpD.nType == 1) { ASSERT(0);  return; }

			int nCode;
			CString strSel = _T("");
			int nIndex = m_cboCode.GetCurSel();
			m_cboCode.GetLBText(nIndex, strSel);
			if(strSel == CODE_ITEM_CEB_FIP) nCode = 0;
			else if(strSel == CODE_ITEM_KOREA_STAND) nCode = 1;
			else if(strSel == CODE_ITEM_CHINA_STAND) nCode = 2;
			else if(strSel == CODE_ITEM_JAPAN_STAND) nCode = 3;
			else ASSERT(0);

			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl->calcNotationalSize(nCode, A, aElemList,aNotSizes)) return;
			
			int nNot = aNotSizes.GetSize();
			for( int j = 0; j < nNot; j++)
			{
				EdmpD.h = aNotSizes[j];
				EdmpDatas.Add(EdmpD);
			}
		}
		else  // Input
		{
			EdmpD.h = H;
			for( int j = 0; j < nElem; j++)
			{
				EdmpDatas.Add(EdmpD);
			}
		}

		CString strItem[] = {_T("h > 0"), _T("v/s > 0")};
		CString strMsg;
		for(int i = EdmpDatas.GetSize() - 1; i >= 0; i--)
		{
			if(EdmpDatas[i].h <= 0.) 
			{
				strMsg.Format(_LS(IDS_TM_EDMP_ERROR), aElemList[i], strItem[EdmpD.nType]);        
				GSaveHistoryFormatNF(strMsg);
				EdmpDatas.RemoveAt(i);
				aElemList.RemoveAt(i);
			}
		}

		ASSERT(aElemList.GetSize() == EdmpDatas.GetSize());
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddEdmp(aElemList,EdmpDatas))
			CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
		
	}
	else  //Delete;!!!
	{
		if(CDBDoc::GetDocPoint()->m_pDataCtrl->DelEdmp(aElemList))
			CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
	}
}

int CElemDepMatChange::GetTypeComboData()
{
	int ix = m_novCode.GetCurSel();
	if (ix == CB_ERR) return -1;
	CString csItem;
	m_novCode.GetLBText(ix, csItem);
	if (csItem == NOVOL_ITEM_NATIONAL_SIZE) return 0;
	if (csItem == NOVOL_ITEM_VOLUME_SURFACE) return 1;
	return -1;
}

//----------------------------------------------------------------------------------
// OnSelchangeTmCodeCombo 코드선택이 바뀔때
//----------------------------------------------------------------------------------
void CElemDepMatChange::OnSelchangeTmCodeCombo() 
{
	ChangeFomulaByCode();
}

void CElemDepMatChange::OnSelchangeTmNovolCombo() 
{  
	ChangeChildByCode();

	int nIndex = m_novCode.GetCurSel();
//#if defined(_CIVIL)
	GetDlgItem(IDC_TM_AUTOCALC_RADIO)->EnableWindow(nIndex == 0);  
	if(nIndex == 1)  // Volume Surface Ratio
	{
		UpdateData(TRUE);
		if(m_nOption == 1)  // Auto Calculate
		{
			m_nOption = 0;    // Input
			m_wndHEdit.EnableWindow(TRUE);
		}
		UpdateData(FALSE);
	}
	else
	{
		OptionCtrlMan();
	}
	
//#endif
}
