// StldFmldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldFmldDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"

#include "..\wg_db\wg_db_FmLoad.h"  // temporary
		
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStldFmldDlg dialog

CStldFmldDlg::CStldFmldDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CStldFmldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldFmldDlg)
	m_nOption = 0;
	m_nCoverType = 0;  	
// 	m_csScale = "-1";
	m_wndScaleFactor = _T("-1");
	//}}AFX_DATA_INIT

	m_pBitmap = 0;
}

void CStldFmldDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldFmldDlg)
	DDX_Control(pDX, IDC_TM_FML_DIR_COMBO, m_wndDirection);
	DDX_Control(pDX, IDC_TM_FML_DENSITY_UNIT, m_wndDensityUnit);
	DDX_Control(pDX, IDC_TM_FML_THICK_UNIT, m_wndThickUnit);
	DDX_Control(pDX, IDC_TM_FML_DENSITY, m_wndDensity);
	DDX_Control(pDX, IDC_TM_FML_THICK, m_wndThick);
	DDX_Control(pDX, IDC_TM_LDGR_CBO_LC, m_wndLoadGroup);
	DDX_Control(pDX, IDC_TM_FML_CBO_LC, m_wndLoadCase);
	DDX_Radio(pDX, IDC_TM_FML_OPT_ADD, m_nOption);
	DDX_Radio(pDX, IDC_TM_FML_TYP_ENV, m_nCoverType);
	DDX_Text(pDX, IDC_TM_FML_SCALE, m_wndScaleFactor);
	//}}AFX_DATA_MAP
 	DDX_Check(pDX, IDC_TM_RANGE_F_CHK1, m_bRangeF[0]);
	DDX_Check(pDX, IDC_TM_RANGE_F_CHK2, m_bRangeF[1]);
	DDX_Check(pDX, IDC_TM_RANGE_F_CHK3, m_bRangeF[2]);
	DDX_Check(pDX, IDC_TM_RANGE_F_CHK4, m_bRangeF[3]);
	DDX_Check(pDX, IDC_TM_RANGE_H_CHK1, m_bRangeH[0]);
	DDX_Check(pDX, IDC_TM_RANGE_H_CHK2, m_bRangeH[1]);
	DDX_Check(pDX, IDC_TM_RANGE_H_CHK3, m_bRangeH[2]);
	DDX_Check(pDX, IDC_TM_RANGE_H_CHK4, m_bRangeH[3]);

	DDX_Control(pDX, IDC_TM_RANGE_PICTURE, m_wndRangePicture);
	DDX_Control(pDX, IDC_TM_TYPE_PICTURE, m_wndTypePicture);
}

BEGIN_MESSAGE_MAP(CStldFmldDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CStldFmldDlg)
 	ON_BN_CLICKED(IDC_TM_RANGE_F_CHK1, OnCmdRangeFullChk)
	ON_BN_CLICKED(IDC_TM_RANGE_F_CHK2, OnCmdRangeFullChk)
	ON_BN_CLICKED(IDC_TM_RANGE_F_CHK3, OnCmdRangeFullChk)
	ON_BN_CLICKED(IDC_TM_RANGE_F_CHK4, OnCmdRangeFullChk)
	ON_BN_CLICKED(IDC_TM_RANGE_H_CHK1, OnCmdRangeHalfChk)
	ON_BN_CLICKED(IDC_TM_RANGE_H_CHK2, OnCmdRangeHalfChk)
	ON_BN_CLICKED(IDC_TM_RANGE_H_CHK3, OnCmdRangeHalfChk)
	ON_BN_CLICKED(IDC_TM_RANGE_H_CHK4, OnCmdRangeHalfChk)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_FML_OPT_ADD, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_FML_BTN_LC, OnLoadCaseBtn)
	ON_BN_CLICKED(IDC_TM_FML_OPT_REPLACE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_FML_OPT_DELETE, OnChangeOption)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnLoadGroupBtn)
	ON_BN_CLICKED(IDC_TM_FML_TYP_ENV, OnCoverType)
	ON_BN_CLICKED(IDC_TM_FML_TYP_FIL, OnCoverType)
	ON_BN_CLICKED(IDC_TM_FML_TYP_SUR, OnCoverType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldFmldDlg message handlers
BOOL CStldFmldDlg::OnInitDialog() 
{
//#if defined(_US) || defined(_RUS)
#if defined(_RUS)
	// CEditData::CheckFmld() -> US, RUS not supported
	ASSERT(false);
	AfxMessageBox(_LS(IDS_WG_TREEMENU_NOT_SUPPORTED));
#endif

	CMenuBarChildDlg::OnInitDialog();
	
	m_pDoc = CDBDoc::GetDocPoint();
	
	UINT CtrlGrpID[] = { 
		IDC_TM_FML_TYP_ENV,  IDC_TM_FML_TYP_FIL,  IDC_TM_FML_TYP_SUR,  IDC_TM_RANGE_F_CHK1, IDC_TM_RANGE_F_CHK2, 
		IDC_TM_RANGE_F_CHK3, IDC_TM_RANGE_F_CHK4, IDC_TM_RANGE_H_CHK1, IDC_TM_RANGE_H_CHK2, IDC_TM_RANGE_H_CHK3, 
		IDC_TM_RANGE_H_CHK4, IDC_TM_FML_THICK,    IDC_TM_FML_DENSITY,  IDC_TM_FML_DIR_COMBO,IDC_TM_FML_SCALE};  
		
	for(int i = 0; i < sizeof(CtrlGrpID) / sizeof(UINT); i++)
	{ 
		m_aOptGrpCtrl.Add(CtrlGrpID[i]);  
	}	

	for(int i = 0; i < 4; i++)
	{
		m_bRangeF[i]= TRUE;
	  m_bRangeH[i]= FALSE;
	}
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_X));
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Y));
	m_wndDirection.AddString(_LS(IDS_WG_TREEMENU_Global_Z));
	
	// Initialize dialog member
	m_nOption = 0;
	m_nCoverType = 0;
	m_wndThick.SetEditUnit(0);
	m_wndDensity.SetEditUnit(0);
	ChangeBitmap(m_nCoverType);
	m_wndDirection.SetCurSel(2);  // Default : Global Z
//  m_wndScaleFactor.SetWindowText("-1"); // Default : -1.0

	CDBDoc* pDoc  = (CDBDoc*) CDBDoc::GetDocPoint();
	CUnitCtrl* pUnitCtrl = pDoc->m_pUnitCtrl;	
	
	//////////////////////////////////////////////
	// <Remember> Data를 받아올때는 Code Unit기준
	T_UNIT_INDEX CurIndex, CodeIndex;
	pUnitCtrl->GetUnitIndexCurrent(CurIndex);
	CodeIndex = CurIndex;
	CodeIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M; // M 기준
	pUnitCtrl->SetUnitIndexCurrent(CodeIndex);   
	// <Remember> Data를 받고나서는 User Unit기준
	pUnitCtrl->SetUnitIndexCurrent(CurIndex);

	// 단위변환
	m_wndThick.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDensity.SetUnitType(D_UNITSYS_BASE_DENSITY);
	m_wndThickUnit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_wndDensityUnit.SetUnitType(D_UNITSYS_BASE_DENSITY);


	if (GetDlgItem(IDC_TM_EXECUTE))
		GetDlgItem(IDC_TM_EXECUTE)->ShowWindow(SW_HIDE);
	if (GetDlgItem(IDC_TM_CLOSE))
		GetDlgItem(IDC_TM_CLOSE)->ShowWindow(SW_HIDE);

	UpdateData(FALSE);		

	m_wndRangePicture.SetImage(_T("SVG\\illustration\\Dialog\\tm_fmld_range.svg"));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStldFmldDlg::OnTmExecute() 
{
// RUS에서는 마감하중 제외
#if defined(_RUS)
	// CEditData::CheckFmld() -> US, RUS not supported
	ASSERT(false);
	AfxMessageBox(_LS(IDS_WG_TREEMENU_NOT_SUPPORTED));
	return;
#endif

	UpdateData(TRUE);
	CArray<T_ELEM_K, T_ELEM_K> aSelKey;
	T_ELEM_D ElemData;

	I_GENModelBase * pIGM = CTreeMenuBarBase::GetI_GENModelST();  
	pIGM->GetSelectedElemKeyList(aSelKey);
	for(int i = aSelKey.GetSize()-1; i >= 0; i--)
	{
		m_pDoc->m_pAttrCtrl->GetElem(aSelKey.GetAt(i), ElemData);
		if (!m_pDoc->m_pAttrCtrl->IsBeam(ElemData.eltyp)) aSelKey.RemoveAt(i);    
//    if (!m_pDoc->m_pAttrCtrl->IsFrameType(ElemData.eltyp)) aSelKey.RemoveAt(i);    
	}
	if(aSelKey.GetSize() == 0) 
	{
		GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_beam_element_));
//    GSaveHistoryFormatNF(_LS(IDS_WG_TREEMENU_Error___No_selected_frame_element_));
		return;
	}

	unsigned int nLoadCaseType, nLoadCaseKey;
	T_FMLD_D data;   
	data.Initialize();
	T_LDGR_K LdgrK;
	m_wndLoadGroup.GetSelectedLdgr(LdgrK);
	data.GroupKey = LdgrK;
	m_wndLoadCase.GetSelectedLoad(nLoadCaseType, nLoadCaseKey);

	if (m_nOption == 0 || m_nOption == 1)    // Add or Replace
	{
		data.LoadCaseKey = nLoadCaseKey;        
		data.nCoveringType = m_nCoverType;
		for(int i=0; i < 4; i++)
		{
			if(m_bRangeF[i])      data.nCoveringRange[i] = 2;  // full
			else if(m_bRangeH[i]) data.nCoveringRange[i] = 1;  // half
			else                  data.nCoveringRange[i] = 0;  // none
		}
		// Input Error Check
		if(m_wndThick.GetEditValue() < 0)  {GSaveHistoryFormatNF(_LS(IDS_TM_FML_THICKNESS));  return;}
		if(m_wndDensity.GetEditValue() <0) {GSaveHistoryFormatNF(_LS(IDS_TM_FML_DENSITY));    return;}

		data.dThickness   = m_wndThick.GetEditValue();
		data.dDensity     = m_wndDensity.GetEditValue();
		data.nDirection   = m_wndDirection.GetCurSel()+1;
		if(!GetFloatNumber(m_wndScaleFactor, data.dScaleFactor)) 
		{GSaveHistoryFormatNF(_LS(IDS_TM_FML_SCALEFACTOR)); return;}
	}

	
	T_BMLD_D DataBmld;
	CFmLoad FmLoad(m_pDoc);
	for(int i = aSelKey.GetSize()-1; i >= 0; i--)
	if(!FmLoad.CalcFmld_BeamData(aSelKey.GetAt(i), data, DataBmld)) return;
	
	BOOL bSuccess=FALSE;
	if (m_nOption == 0 || m_nOption == 1) // Add or Replace
		bSuccess = m_pDoc->m_pDataCtrl->AddFmld(aSelKey, data, (m_nOption==0) ? FALSE:TRUE);	//c
	else if (m_nOption == 2)  // Delete
		bSuccess = m_pDoc->m_pDataCtrl->DelFmld(aSelKey, nLoadCaseKey, LdgrK);   //c

	if (bSuccess) m_pDoc->m_pViewCtrl->UnselectAll(NULL);	

/*///////////////////////////
// TEST /////////////////////
	T_BMLD_D DataBmld;
	DataBmld.Initialize();
	CFmLoad FmLoad(m_pDoc);
	FmLoad.CalcFmld_BeamData(aSelKey.GetAt(0), data, DataBmld);
	int nLoadType = DataBmld.LoadType;
	int nLoadDirection = DataBmld.LoadDirection;
	double force1 = DataBmld.Force_Type[2][0];
	double force2 = DataBmld.Force_Type[2][1];
*///////////////////////////////  
}

CStldFmldDlg::~CStldFmldDlg()
{
	if (m_pBitmap) delete m_pBitmap;
}

void CStldFmldDlg::OnTmClose() 
{
	CloseDlg();
}

void CStldFmldDlg::OnLoadCaseBtn() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldFmldDlg::OnLoadGroupBtn() 
{
	CreateOrActivateDlg(m_pDoc,CCMLoadGroupDlg::IDD);
}

void CStldFmldDlg::OnChangeOption() 
{
	UpdateData(TRUE);
	if (m_nOption == 2) // delete 
	{
//    EnableValueEdit(FALSE);
		CtrlEnableDisable(m_aOptGrpCtrl, FALSE);
		m_wndLoadGroup.ShowHideAll(TRUE);
	}
	else 
	{
//    EnableValueEdit(TRUE);		
		CtrlEnableDisable(m_aOptGrpCtrl, TRUE);
		m_wndLoadGroup.ShowHideAll(FALSE);
	}	
}

void CStldFmldDlg::OnCoverType() 
{
	UpdateData(TRUE);
	ChangeBitmap(m_nCoverType);
}

void CStldFmldDlg::ChangeBitmap(int nBitmap)
{
#if 0	
	// Change Bitmap according to local direction(y, z)
	UINT aBitmapID[] = {IDB_TM_FMLD_ENV, IDB_TM_FMLD_FILL, IDB_TM_FMLD_SUR};
	if (m_pBitmap != 0) delete m_pBitmap;
	CBCGPStatic* pImage = (CBCGPStatic*)GetDlgItem(IDC_TM_TYPE_PICTURE);
	m_pBitmap = new CBitmap;
	m_pBitmap->LoadBitmap(aBitmapID[nBitmap]);
	pImage->SetBitmap(HBITMAP(*m_pBitmap));
#else
	// Type Picture  
	//IDB_TM_FMLD_ENV         BITMAP                  "res\\tm_fmld_env.bmp"
	//IDB_TM_FMLD_SUR         BITMAP                  "res\\tm_fmld_sur.bmp"
	//IDB_TM_FMLD_FILL        BITMAP                  "res\\tm_fmld_fill.bmp"
	//IDB_TM_FMLD_RANGE       BITMAP                  "res\\tm_fmld_range.bmp"

	CString aBitmapPath[] = {
		_T("SVG\\illustration\\Dialog\\tm_fmld_env.svg"),
		_T("SVG\\illustration\\Dialog\\tm_fmld_fill.svg"),
		_T("SVG\\illustration\\Dialog\\tm_fmld_sur.svg")
	};
	m_wndTypePicture.SetImage(aBitmapPath[nBitmap]);
#endif 
}

void CStldFmldDlg::OnCmdRangeFullChk() 
{
	UpdateData(TRUE);
	for(int i = 0; i < 4; i++)
	{
		if(m_bRangeF[i] == TRUE)  { m_bRangeH[i] = FALSE;  UpdateData(FALSE); }    
	}
}

void CStldFmldDlg::OnCmdRangeHalfChk() 
{
	UpdateData(TRUE);
	for(int i = 0; i < 4; i++)
	{
		if(m_bRangeH[i] == TRUE)  { m_bRangeF[i] = FALSE;  UpdateData(FALSE); }
	}
}
