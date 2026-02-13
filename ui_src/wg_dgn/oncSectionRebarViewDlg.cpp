// oncSectionRebarViewDlg.cpp : implementation file
// (060123) sshan Create

#include "stdafx.h"
#include "wg_dgn.h"
#include "oncSectionRebarViewDlg.h"
#include "..\wg_sw\GSectConverter.h"
#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"
#include "..\wg_cmd\CMSecViewWnd.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\matlDB.h"
#include "..\wg_db\SectUtil.h"
//#include "ConcSectionRebarView.h"
#include "ConcSectionRebarEventHandler.h"
#include "ConcSectionMainRebar.h"
#include "ConcSectionShearRebar.h"
#include "ConcSectionCreateRebar.h"
#include "ConcSectionCreateRebar_J.h"

#include "DgnSectionTool.h"
#include "DgnTabCtrl.h"
#include "..\wg_cmd\ExportFunc.h"


//(2010.02.05) Add by Unsang!!
#include "DgnDataCtrl.h"

#include <math.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define M_PI        3.14159265358979323846


/////////////////////////////////////////////////////////////////////////////
// ConcSectionRebarViewDlg dialog


ConcSectionRebarViewDlg::ConcSectionRebarViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(ConcSectionRebarViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ConcSectionRebarViewDlg)
	m_iCenterType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = 0;
	m_pDoc = CDBDoc::GetDocPoint();
	//................................................................
	CArray<UINT,UINT> TBarLayout;
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_SINGLE     );
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_POLYGON    );
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_POLYLINE   );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_RECT     );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_POLYGON  );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_POLYLINE );
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_ALL        );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_ALL      );
	TBarLayout.Add(VP_TBarID::Instance().ZOOM_ALL          );
	TBarLayout.Add(VP_TBarID::Instance().ZOOM_WINDOW       );
	TBarLayout.Add(VP_TBarID::Instance()._ZOOM_IN          );
	TBarLayout.Add(VP_TBarID::Instance()._ZOOM_OUT         );
	TBarLayout.Add(VP_TBarID::Instance().SHOW_UCSAXIS      );

	//---------- DLG MDView -----------------
	m_pIVP = I_Factory_DlgViewPanel::Instance()->NewDlgViewPanel();

	m_pIVP->SetToolBarLayout(TBarLayout);
	//m_pIVP->SetHidePositionPanel();

	m_pEventHandler = NULL;
	//................................................................

	m_sectData.Initialize();
	m_RebtData.Initialize();
	m_pMainRebar = NULL;
	// Section : [0]offset.x, [1]high, [2]low, [3]left, [4]right, [5]offset.y
	m_pShearRebar = NULL;

	m_arViewGeom.RemoveAll();

	//m_pView = NULL;

	nSectionType = 1;
	m_bIsScaleTaperedGroup = FALSE;
	m_bIsShearRebar = FALSE;
	//
	m_strDgnCode	=	_T("");
}

ConcSectionRebarViewDlg::~ConcSectionRebarViewDlg()
{
	if(NULL != m_pEventHandler)
	{
		m_pIVP->ResetAllEVHandler();
		delete m_pEventHandler;
	}

	if(NULL != m_pIVP)
	{
		m_pIVP->On_Destroy(this);
		delete m_pIVP;
	}
}


void ConcSectionRebarViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ConcSectionRebarViewDlg)
	DDX_Control(pDX, IDC_DGN_RANDOM_SECT_COMBO, m_RandomSectionCombo);
	DDX_Control(pDX, IDC_DGN_VIEW_TEXT_OUT_STATIC, m_wndTextOut);
	DDX_Control(pDX, IDC_DGN_FUNCTION_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_SECT_COMBO, m_wndSectCombo);
	DDX_Radio(pDX, IDC_DGN_CENTERYPE_C_RADIO, m_iCenterType);
	DDX_Text(pDX, IDC_DGN_ELEM_NUMBER_EDIT, m_Ctrl_ElemNumber_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ConcSectionRebarViewDlg, CDialogMove)
	//{{AFX_MSG_MAP(ConcSectionRebarViewDlg)
	ON_BN_CLICKED(IDC_DGN_SECT_IMPORT_BUTTON, OnDgnSectImportButton)
	ON_BN_CLICKED(IDC_DGN_CANCEL_SECT_IMPORT_BUTTON, OnDgnSectImportCancelButton)
	ON_BN_CLICKED(IDC_DGN_DEFINE_SECT_BUTTON, OnDgnDefineSectButton)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_CBN_SELCHANGE(IDC_DGN_RANDOM_SECT_COMBO, OnSelchangeDgnRandomSectCombo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_FUNCTION_TAB, OnSelchangeDgnTab)
	ON_BN_CLICKED(IDC_DGN_CENTERYPE_C_RADIO, OnChangedCenterType_Center)
	ON_BN_CLICKED(IDC_DGN_CENTERYPE_LEFTBOTTOM_RADIO, OnChangedCenterType_LeftBottom)
	ON_BN_CLICKED(IDC_DGN_CON_SECT_REBAR_APPLY_BTN, OnBtnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ConcSectionRebarViewDlg message handlers

void ConcSectionRebarViewDlg::LayoutChildDlg()
{
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_TAB_PLACEHOLDER2);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	//................................
	m_pMainRebar = new CConcSectionMainRebar(this);
	m_pMainRebar->SetDlgViewPanel(this->m_pIVP); //Create전에 View를 연결. 호출 위치를 변경하지 말것.
	BOOL bChangeTitle=FALSE;
	if (m_strDgnCode==CONCODE_EC2_2_05 || m_strDgnCode==CONCODE_AASHTO_LRFD12 ||
		m_strDgnCode==CONCODE_AASHTO_LRFD07 || m_strDgnCode==CONCODE_AASHTO_LRFD16 ||
		m_strDgnCode==CONCODE_AASHTO_LRFD17 || m_strDgnCode==CONCODE_AASHTO_LRFD20 ||
		m_strDgnCode==CONCODE_AASHTO_LRFD24 ) bChangeTitle = TRUE;

	CString strTabTitle = _T("");
	strTabTitle = (bChangeTitle)? _LS(IDS_DGN_CON_SECT_REBAR_MAIN_REBAR_EC2) : _LS(IDS_DGN_CON_SECT_REBAR_MAIN_REBAR);

	m_ctrlTab.AddTab(m_pMainRebar, strTabTitle, CConcSectionMainRebar::IDD, TRUE);

	//060301 sshan 현재 전단철근 사용안함

	if(m_bIsShearRebar)
	{
		strTabTitle = (bChangeTitle)? _LS(IDS_DGN_CON_SECT_REBAR_SHEAR_REBAR_EC2) : _LS(IDS_DGN_CON_SECT_REBAR_SHEAR_REBAR);

		m_pShearRebar = new CConcSectionShearRebar(this);
		m_ctrlTab.AddTab(m_pShearRebar, strTabTitle, CConcSectionShearRebar::IDD, TRUE);
	}
	m_ctrlTab.ShowTab(0);    
}

/*
void ConcSectionRebarViewDlg::OnCancelButton() 
{
	m_pIVP->DeleteAllObject();
	m_pIVP->DelAllLabel();
	
	CDialogMove::OnCancel();
}*/

BOOL ConcSectionRebarViewDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	// Rebar Area 단위 산정 
	T_UNIT_INDEX CurUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurUnit.nBase_Length,CurUnit.nBase_Force,TRUE);

	//CWnd* pWnd = GetDlgItem(IDC_DGN_VIEW_PLACEHOLDER);
	//ASSERT(pWnd);
	//m_pView.Init(pWnd);

	//m_pView = new CConcSectionRebarView(pWnd);

	//CRect rectWnd;
	//pWnd->GetWindowRect(&rectWnd);
	//m_pView->SetWindowPos(pWnd, 0, 0, rectWnd.Width(), rectWnd.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);


	/////////////////////////////////////////////////////////////////////////////////
	//(2010.02.05) Add by Unsang :: SET DGN.CODE!
	T_DCON_D DconD;	DconD.Initialize();
	CDgnDataCtrl DataCtrl;
	DataCtrl.Get_DgnConDcon(DconD);
	m_strDgnCode = DconD.DesignCode;

	/*
	//(2010.02.05) Add by Unsang :: Convert String!!
	if(m_strDgnCode==_T("Eurocode2-2:05"))
	{
		GetDlgItem(IDC_DGN_CENTERTYPE_STATIC)->SetWindowText(_LS(IDS_DGN_CON_SECT_REBAR_ORIGINTYPE_STC));
		GetDlgItem(IDC_DGN_CENTERYPE_C_RADIO)->SetWindowText(_LS(IDS_DGN_CON_SECT_REBAR_ORIGINTYPE_CEN));
		GetDlgItem(IDC_DGN_CENTERYPE_LEFTBOTTOM_RADIO)->SetWindowText(_LS(IDS_DGN_CON_SECT_REBAR_ORIGINTYPE_LFBT));

		// Set Resource Location!!
		CRect DlgRect, BtnRect, BtnRect2, BtnRect_Base;
		int icx=0, icy=0;
		
		// Assumed Value!!
		int iStep = 5;
		
		GetWindowRect(DlgRect);
		GetDlgItem(IDC_DGN_DEFINE_SECT_BUTTON)->GetWindowRect(BtnRect_Base);
		GetDlgItem(IDC_DGN_CENTERTYPE_STATIC)->GetWindowRect(BtnRect2);
		//
		icx = 0;	icx = (BtnRect_Base.right - DlgRect.left) + iStep*2;
		icy = 0;	icy = BtnRect2.top - DlgRect.top - (int)((BtnRect2.Height() + BtnRect_Base.Height())/2.0);	// Y는 공통임.
		GetDlgItem(IDC_DGN_CENTERTYPE_STATIC)->SetWindowPos(NULL, icx, icy, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_DGN_CENTERTYPE_STATIC)->SetWindowPos(NULL, 0, 0, BtnRect2.Width()+iStep*4, BtnRect2.Height(), SWP_NOMOVE);

		//
		GetDlgItem(IDC_DGN_CENTERTYPE_STATIC)->GetWindowRect(BtnRect);
		GetDlgItem(IDC_DGN_CENTERYPE_C_RADIO)->GetWindowRect(BtnRect2);
		icx = 0;	icx = (BtnRect.right - DlgRect.left) + iStep*2;
		icy = 0;	icy = BtnRect2.top - DlgRect.top - (int)((BtnRect2.Height() + BtnRect_Base.Height())/2.0);	// Y는 공통임.
		GetDlgItem(IDC_DGN_CENTERYPE_C_RADIO)->SetWindowPos(NULL, icx, icy, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_DGN_CENTERYPE_C_RADIO)->SetWindowPos(NULL, 0, 0, BtnRect2.Width()+iStep*4, BtnRect2.Height(), SWP_NOMOVE);
		
		//
		GetDlgItem(IDC_DGN_CENTERYPE_C_RADIO)->GetWindowRect(BtnRect);
		GetDlgItem(IDC_DGN_CENTERYPE_LEFTBOTTOM_RADIO)->GetWindowRect(BtnRect2);
		icx = 0;	icx = (BtnRect.right - DlgRect.left) + iStep*2;
		icy = 0;	icy = BtnRect2.top - DlgRect.top - (int)((BtnRect2.Height() + BtnRect_Base.Height())/2.0);	// Y는 공통임.


		// 앞선 Tool의 이벤트 포이터를 생성 후 연결함.
		GetDlgItem(IDC_DGN_CENTERYPE_LEFTBOTTOM_RADIO)->SetWindowPos(GetDlgItem(IDC_DGN_CENTERYPE_C_RADIO), icx, icy, 0, 0, SWP_NOSIZE);
		GetDlgItem(IDC_DGN_CENTERYPE_LEFTBOTTOM_RADIO)->SetWindowPos(GetDlgItem(IDC_DGN_CENTERYPE_C_RADIO), 0, 0, BtnRect2.Width()+iStep*5, BtnRect2.Height(), SWP_NOMOVE);
	}
	*/

	/////////////////////////////////////////////////////////////////////////////////

	m_MabrDataListI.InitHashTable(101);
	m_MabrDataListJ.InitHashTable(101);
	KeyDataListI.InitHashTable(101);
	KeyDataListJ.InitHashTable(101);
	m_MabrDataListI.RemoveAll();
	m_MabrDataListJ.RemoveAll();
	bMainSameIJ = FALSE;
	bShearSameIJ = FALSE;
	m_ShearRebarDataI.Initialize();
	m_ShearRebarDataJ.Initialize();
	priCenterType = 0;
	
	m_pIVP->On_InitDialog(this,IDC_DGN_SECT_REBAR_VIEW);
	
	LayoutChildDlg();

	SetToolBarIcons();

	InitialRandomSectionData();

	SetRandomSectionComboHide();
	//.......................................................
	


	m_pEventHandler = new ConcSectionRebarEventHandler();
	m_pEventHandler->SetDlgViewPanel(m_pIVP);
	m_pIVP->PushEVHandler(m_pEventHandler);


	m_pIVP->ShowPointGrid(TRUE);

	m_pIVP->SnapFree();
	m_pIVP->ShowAxis(TRUE);
	m_pEventHandler->SetWndTexOut(&m_wndTextOut);
	//..........................................................
	//AddObjects();

	if(nSectionType == 0)//요소별이면 OnInitDialog() 시에 정보를 불러들인다
		SectionImportByElem();
	
	// Modify!!!
	else if(nSectionType == 2) // RC_UMD_COLUMN :: Code By  Unsang
	{
		SectionImportByElemUsedUMD();   // 신규함수 생성해야 함. 
		GetDlgItem(IDC_DGN_CON_SECT_REBAR_APPLY_BTN)->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL ConcSectionRebarViewDlg::ImportSection(int iPosi)
{
	ASSERT(&m_sectData);
	m_arSectionPos.RemoveAll();

	if(!ConvertSection(&m_sectData, m_arSectionPos, iPosi))		return FALSE;
	
	int nGeom = m_arViewGeom.GetSize();
	for(int i = 0 ; i < nGeom; i++)
	{
	  m_pIVP->AddObject(*m_arViewGeom[i]);
	}

	//가운데에 작은 점을 그린다
	CArray<DV_3dp, DV_3dp> PolyDataList;	
	if(!MakeCirclePoints(0.0, 0.0, 0.000001, PolyDataList)) return FALSE;	
	CString strTemp;
	DlgView_Object DVObject;
	CArray<DV_3dp,DV_3dp&> arVerts;
	arVerts.SetSize(PolyDataList.GetSize());	
	double y, z, x;
	for(int i=0 ; i<PolyDataList.GetSize() ; i++)
	{
		PolyDataList[i].Get(y, z, x);
		arVerts[i].Set(y, z, 0);		
	}
	DVObject.GetGeomData().SetPolygon(arVerts);
	m_pIVP->AddObject(DVObject);

	m_pIVP->ZoomAll();

	m_arViewGeom.RemoveAll();

	return TRUE;
}


BOOL ConcSectionRebarViewDlg::ConvertSection(T_SECT_D* pSectD, CArray<double, double&> &arPos, int iPosi)
{
	int nSize = m_arViewGeom.GetSize();
	DlgView_Object* pDVO;
	for(int i = 0; i < nSize; i++)
	{
		pDVO = m_arViewGeom[i];
		delete pDVO;
	}
	m_arViewGeom.RemoveAll();

	CDgnSectionTool SectionTool;
	BOOL bResult;
	if(m_iCenterType == 0)//도심
	{
		bResult = SectionTool.Get_SectionPolygonData_DlgView_ObjectArray(*pSectD, iPosi, m_arViewGeom, arPos);		
	}
	else
	{
		if(iPosi == 0)
			bResult = SectionTool.Get_SectionPolygonData_DlgView_ObjectArray(*pSectD, iPosi, m_arViewGeom, arPos, OffsetI_Y, OffsetI_Z);
		else
			bResult = SectionTool.Get_SectionPolygonData_DlgView_ObjectArray(*pSectD, iPosi, m_arViewGeom, arPos, OffsetJ_Y, OffsetJ_Z);
	}
		return bResult;
}



void ConcSectionRebarViewDlg::AddObjects()
{
	UINT DVOKey = 0;
	CStringArray arQueryString;
	CString strTemp;
	DlgView_Object DVObject;
	CArray<DV_3dp,DV_3dp&> arVerts;
	arVerts.SetSize(4);
	arVerts[0].Set(-10,-10,0);
	arVerts[1].Set(10,-10,0);
	arVerts[2].Set(10,10,0);
	arVerts[3].Set(-10,10,0);

	arQueryString.Add(_T("Test Model !!"));
	arQueryString.Add(_T("Test Model !!"));
	DVObject.GetGeomData().SetPolygon(arVerts);
	DVObject.SetQueryString(arQueryString);
	DVOKey = m_pIVP->AddObject(DVObject);

	arVerts[0].Set(20,-10,0);
	arVerts[1].Set(30,-10,0);
	arVerts[2].Set(30,10,0);
	arVerts[3].Set(20,10,0);
	DVObject.GetGeomData().SetPolygon(arVerts);
	DVOKey = m_pIVP->AddObject(DVObject);
 
	m_pIVP->ZoomAll();
} 


void ConcSectionRebarViewDlg::SetToolBarIcons()
{
#define SetICON(IDCtrl,IDIcon)\
	{\
		CWnd* pWnd = GetDlgItem(IDCtrl);\
		if(pWnd){\
			HICON hIcon = AfxGetApp()->LoadIcon(IDIcon);\
				if(hIcon){ ((CButton*)pWnd)->SetIcon(hIcon);}\
		}\
	}
	
//---------------------------------------------------
	SetICON(IDC_DGN_SECT_IMPORT_BUTTON,				IDI_DGN_SECTION_IMPORT)
	SetICON(IDC_DGN_CANCEL_SECT_IMPORT_BUTTON, IDI_DGN_SECTION_IMPORT_CANCEL)
	SetICON(IDC_DGN_RPSC_IMPORT_BUTTON,				IDI_DGN_REBAR_IMPORT )
	SetICON(IDC_DGN_RPSC_IMPORT_CANCEL_BUTTON,	IDI_DGN_REBAR_IMPORT_CANCEL)
	//SetICON(IDC_DGN_DEFINE_SECT_BUTTON,				IDI_DGN_DEFINE_SECTION)
}

void ConcSectionRebarViewDlg::OnDgnSectImportButton() 
{
	m_pIVP->DeleteAllObject();

	int iCurSel;
	if(m_ctrlTab.GetCurSel() == 0)
		iCurSel = m_pMainRebar->GetCurSel();
	else
		iCurSel = m_pShearRebar->GetCurSel();

	int RedamSectIndex = m_RandomSectionCombo.GetCurSel();
	if(RedamSectIndex < 0) return;
	T_SECT_K SectKey;
	m_RandomSectionList.Lookup(RedamSectIndex, SectKey);

	//060331 Modify sshan
	//if(!m_pDoc->m_pAttrCtrl->GetSect(SectKey, m_sectData))	return;
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectKey, m_sectData))	return;

	if(m_sectData.nStype == D_SECT_TYPE_TAPERED)
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = m_sectData.SectBefore.SectJ.Design.YBar;
		OffsetJ_Z = m_sectData.SectBefore.SectJ.Design.ZBar;
	}
	else
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = OffsetI_Y;
		OffsetJ_Z = OffsetI_Z;
	}

	//단면을 도심 또는 최하단기준으로 그리기위하여 정보세팅
	if(m_pDoc->m_pAttrCtrl->ExistRebt(SectKey))
	{
		T_REBT_D RebtData;
		
		if(m_pDoc->m_pAttrCtrl->GetRebt(SectKey, RebtData))
			m_iCenterType = RebtData.iCenterType;
	}
	
	if(ImportSection(iCurSel))
	{
		NowSectKey = SectKey;
		bDrawSection = TRUE;
	}

	if(m_pDoc->m_pAttrCtrl->ExistRebt(SectKey))//해당 Section에 철근정보가 있으면 같이 불러와 그려준다.
	{
		T_REBT_D RebtData;
		
		if(m_pDoc->m_pAttrCtrl->GetRebt(SectKey, RebtData))
		{
			if(RebtData.iCenterType == 1)//좌하단기준
			{
				for(int i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
				{
					RebtData.arMainRebar[0][i].dCenterY += OffsetI_Y;
					RebtData.arMainRebar[0][i].dCenterZ += OffsetI_Z;
				}
				for(int i=0 ; i<RebtData.arMainRebar[1].GetSize() ; i++)
				{
					RebtData.arMainRebar[1][i].dCenterY += OffsetJ_Y;
					RebtData.arMainRebar[1][i].dCenterZ += OffsetJ_Z;
				}
				// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
				for(int i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
				{
					RebtData.arMainRebar[2][i].dCenterY += OffsetJ_Y;
					RebtData.arMainRebar[2][i].dCenterZ += OffsetJ_Z;
				}
			}

			if(m_ctrlTab.GetCurSel() == 0)
			{
				if(iCurSel == 0) //i단
				{
					m_pMainRebar->SetInitDataI(RebtData.arMainRebar[0]);
					
					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					for(int i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
					{
						m_MabrDataListJ.SetAt(i, RebtData.arMainRebar[2][i]);
					}

					m_pMainRebar->LoadDlgData(RebtData.bIJBothMain);

					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					bShearSameIJ = RebtData.bIJBothShear;
					m_ShearRebarDataI.dSubRebarSpace = RebtData.ShearRebar[0].dSubRebarSpace;
					m_ShearRebarDataI.dSubRebarNum[0] = RebtData.ShearRebar[0].dSubRebarNum[0];
					m_ShearRebarDataI.dSubRebarNum[1] = RebtData.ShearRebar[0].dSubRebarNum[1];
					m_ShearRebarDataI.strSubRebarName = RebtData.ShearRebar[0].strSubRebarName;
					m_ShearRebarDataJ.dSubRebarSpace = RebtData.ShearRebar[2].dSubRebarSpace;
					m_ShearRebarDataJ.dSubRebarNum[0] = RebtData.ShearRebar[2].dSubRebarNum[0];
					m_ShearRebarDataJ.dSubRebarNum[1] = RebtData.ShearRebar[2].dSubRebarNum[1];
					m_ShearRebarDataJ.strSubRebarName = RebtData.ShearRebar[2].strSubRebarName;
				}
				else //j단
				{
					m_pMainRebar->SetInitDataJ(RebtData.arMainRebar[1]);

					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					for(int i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
					{
						m_MabrDataListI.SetAt(i, RebtData.arMainRebar[0][i]);
					}
					
					m_pMainRebar->LoadDlgData(RebtData.bIJBothMain);

					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					bShearSameIJ = RebtData.bIJBothShear;
					m_ShearRebarDataI.dSubRebarSpace = RebtData.ShearRebar[0].dSubRebarSpace;
					m_ShearRebarDataI.dSubRebarNum[0] = RebtData.ShearRebar[0].dSubRebarNum[0];
					m_ShearRebarDataI.dSubRebarNum[1] = RebtData.ShearRebar[0].dSubRebarNum[1];
					m_ShearRebarDataI.strSubRebarName = RebtData.ShearRebar[0].strSubRebarName;
					m_ShearRebarDataJ.dSubRebarSpace = RebtData.ShearRebar[2].dSubRebarSpace;
					m_ShearRebarDataJ.dSubRebarNum[0] = RebtData.ShearRebar[2].dSubRebarNum[0];
					m_ShearRebarDataJ.dSubRebarNum[1] = RebtData.ShearRebar[2].dSubRebarNum[1];
					m_ShearRebarDataJ.strSubRebarName = RebtData.ShearRebar[2].strSubRebarName;					
				}		
			}
			else
			{
				if(iCurSel == 0) //i단
				{
					int i;
					for(i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
					{
						m_MabrDataListI.SetAt(i, RebtData.arMainRebar[0][i]);
					}
					
					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					for(i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
					{
						m_MabrDataListJ.SetAt(i, RebtData.arMainRebar[2][i]);
					}

					bMainSameIJ = RebtData.bIJBothMain;

					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					m_pShearRebar->LoadDlgData(RebtData.bIJBothShear, RebtData.ShearRebar[0], RebtData.ShearRebar[2]);
				}
				else //j단
				{
					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					int i;
					for(i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
					{
						m_MabrDataListJ.SetAt(i, RebtData.arMainRebar[2][i]);
					}

					for(i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
					{
						m_MabrDataListI.SetAt(i, RebtData.arMainRebar[0][i]);
					}
					
					// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
					bMainSameIJ = RebtData.bIJBothMain;
					m_pShearRebar->LoadDlgData(RebtData.bIJBothShear, RebtData.ShearRebar[0], RebtData.ShearRebar[2]);
				}		
			}
						
			m_iCenterType = RebtData.iCenterType;
			priCenterType = RebtData.iCenterType;
			UpdateData(FALSE);

			m_pMainRebar->SetAsRebarArea();
		}
	}	

	m_pIVP->ZoomAll();
}

void ConcSectionRebarViewDlg::OnDgnSectImportCancelButton() 
{

	m_pIVP->DeleteAllObject();
	m_pIVP->RedrawAll();
	bDrawSection = FALSE;
}

void ConcSectionRebarViewDlg::OnDgnDefineSectButton() 
{
	CMD_DoModal(D_CMD_CCMSECTPAGE4DESIGN);
	//if(AfxGetMainWnd())
	// AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECTION,0),0L);	
	OnSelchangeDgnRandomSectCombo();
}

BOOL ConcSectionRebarViewDlg::MakeCirclePoints(double Yc, double Zc, double Radius,
																			 CArray<DV_3dp, DV_3dp> &PosArr,double nSeg /*= 20*/)
{
	int i;
	double YPlot,ZPlot;
	//double x[65],y[65];
	CArray<double, double > y;
	CArray<double, double > z;
	y.SetSize(nSeg+1);
	z.SetSize(nSeg+1);

	double dTheta,dCos,dSin,num;
	DV_3dp TPos;
	num = nSeg;
	dTheta = 2. * M_PI / num;
	dCos = cos(dTheta);
	dSin = sin(dTheta);

	y[0] = Radius;
	z[0] = 0.;
	YPlot = Yc + y[0];
	ZPlot = Zc + z[0];

	TPos.Set(YPlot,ZPlot,0.);
	PosArr.Add(TPos);

	if(Radius==0.0) return TRUE;

	int Iter = (int) num;

	for( i = 0 ; i < Iter ; i++)
	{
		y[i+1] = y[i]*dCos  - z[i]*dSin;
		z[i+1] = y[i]*dSin  + z[i]*dCos;
		YPlot = Yc + y[i+1];
		ZPlot = Zc + z[i+1];
		TPos.Set(YPlot,ZPlot,0.);
		PosArr.Add(TPos);
	}
		
	return TRUE;
}


void ConcSectionRebarViewDlg::AddRebarData(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End)
{
	if(bI_End)
		m_MabrDataListI.SetAt(RebarKey, MaReberData);
	else
		m_MabrDataListJ.SetAt(RebarKey, MaReberData);

	AddRebarDraw(RebarKey, MaReberData, bI_End);

}

void ConcSectionRebarViewDlg::DeleteRebarData(REBAR_KEY RebarKey, BOOL bI_End)
{
	if(bI_End)
	{
		m_MabrDataListI.RemoveKey(RebarKey);
		
		int DKey = 0;
		if(!KeyDataListI.Lookup(RebarKey, DKey)) return;

		m_pIVP->DeleteObject(DKey);
		KeyDataListI.RemoveKey(RebarKey);
	}
	else
	{
		m_MabrDataListJ.RemoveKey(RebarKey);
		
		int DKey = 0;
		if(!KeyDataListJ.Lookup(RebarKey, DKey)) return;

		m_pIVP->DeleteObject(DKey);
		KeyDataListJ.RemoveKey(RebarKey);
	}

	m_pIVP->RedrawAll();
}

void ConcSectionRebarViewDlg::AddRebarDraw(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End)
{
	CArray<DV_3dp, DV_3dp> PolyDataList;
	_DGN_REBAR_DATA RbarData;
	RbarData = m_pDoc->m_pMatlDB->Get_RebarData(MaReberData.strRebarName);

	if(!MakeCirclePoints(MaReberData.dCenterY, MaReberData.dCenterZ, RbarData.dDia/2, PolyDataList)) return;

	UINT DVOKey = 0;
	CStringArray arQueryString;
	CString strTemp;
	DlgView_Object DVObject;
	CArray<DV_3dp,DV_3dp&> arVerts;
	arVerts.SetSize(PolyDataList.GetSize());
	
	double y, z, x;
	for(int i=0 ; i<PolyDataList.GetSize() ; i++)
	{
		PolyDataList[i].Get(y, z, x);
		arVerts[i].Set(y, z, 0);		
	}

	strTemp.Format(_T("Number : %d"), MaReberData.iLabel);
	arQueryString.Add(strTemp);
	arQueryString.Add(MaReberData.strRebarName);
	DVObject.GetGeomData().SetPolygon(arVerts);
	DVObject.SetQueryString(arQueryString);
	DVOKey = m_pIVP->AddObject(DVObject);

	if(bI_End)
	{
		KeyDataListI.SetAt(RebarKey, DVOKey);
	}
	else
	{
		KeyDataListJ.SetAt(RebarKey, DVOKey);
	}	
	
	m_pIVP->RedrawAll();
}


void ConcSectionRebarViewDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_pIVP->DeleteAllObject();
	m_pIVP->DelAllLabel();	

	CDialogMove::OnCancel();
}

void ConcSectionRebarViewDlg::OnOk() 
{
	if(bDrawSection)
	{
		BOOL bOK = SaveData();

		if(!bOK)
		{
			AfxMessageBox(_LS(IDS_DGN_CON_SECT_REBAR_ERROR_INPUTDATA), MB_OK);//_T("입력값이 올바르지 않습니다.\n메인화면 하단 Message View를 확인하시기 바랍니다.")
			return;
		}
	}
	else
		OnCancel();

	CDialogMove::OnOK();
}

void ConcSectionRebarViewDlg::InitialRandomSectionData()
{
	m_RandomSectionCombo.ResetContent();
	
	CArray<T_SECT_K, T_SECT_K> arKeyList;
	T_SECT_D SectData;
	int ComboIndex = 0;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(arKeyList);

	for(int i=0 ; i<arKeyList.GetSize() ; i++)
	{
		//060331 Modify sshan
		//if(m_pDoc->m_pAttrCtrl->GetSect(arKeyList[i], SectData))
		if(m_pDoc->m_pAttrCtrl->GetSectDesign(arKeyList[i], SectData))
		{
			if((SectData.nStype == D_SECT_TYPE_USER && SectData.SectBefore.Shape == _T("GEN")) ||
				(SectData.nStype == D_SECT_TYPE_TAPERED && SectData.SectBefore.nStype==D_SECT_TYPE_USER && SectData.SectBefore.Shape == _T("GEN")))
			{//유저 input 타입이면서 General 타입이어야한다, 또는 테이퍼드단면이면서 USER 타입이어야하고 General 타입이어야한다
				BOOL bScaleTsgr = CSectUtil::IsScaleTaperedShape_Gen(arKeyList[i], TRUE);
				if(SectData.SectBefore.SectI.aLine.GetSize()<=0)
				{//Plane Type이어야한다
					BOOL bColumn = TRUE;
					BOOL bConcrete = TRUE;
					BOOL bTsgr = TRUE;
					CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
					T_ELEM_D ElemData;
					T_MATL_D DataMatl;

					// Change by ZINU.('06.06.30). Get ElemList by SectK.
					CArray<T_SECT_K,T_SECT_K> arSectK;
					arSectK.SetSize(1);	// Always 1.
					arSectK.Add(arKeyList[i]);
					// Change by ZINU.('06.06.30). m_pDoc->m_pAttrCtrl->GetElemKeyList(arElemKeyList) -> Next Line.
					m_pDoc->m_pAttrCtrl->GetKeyListSectUser(&arSectK, NULL, &arElemKeyList);
					for(int k=0 ; k<arElemKeyList.GetSize() ; k++)
					{

						if(m_pDoc->m_pAttrCtrl->GetElemTsgrKey(arElemKeyList[k]))
						{	
							if(!m_bIsScaleTaperedGroup || !bScaleTsgr)
							{
								bTsgr = FALSE;
								break;
							}							
						}
						
						if(m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[k], ElemData))
						{
							m_pDoc->m_pAttrCtrl->GetMatl(ElemData.elmat, DataMatl);
							int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[k]);
							if(ElemData.elpro == arKeyList[i])
							{//해당 Elem의 Section키가 같은 것들 중에
								if(nMembType != 1 && nMembType != 2)  // (kyung) 임시로 보에서도 열어둠nMembType = 2
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

					if(bColumn && bConcrete && bTsgr)
					{
						m_RandomSectionCombo.AddString(SectData.SName);
						m_RandomSectionList.SetAt(ComboIndex++, arKeyList[i]);
					}
				}
			}
		}
	}  
}

void ConcSectionRebarViewDlg::OnSelchangeDgnRandomSectCombo() 
{
	OnDgnSectImportCancelButton();

	int iCurSelIJ;
	int iCurSelMainShear;
	iCurSelMainShear = m_ctrlTab.GetCurSel();
	if(iCurSelMainShear != 0) m_ctrlTab.ShowTab(0); // Add by SHIN (09.11.14) : Shear Rebar입력창에서 단면 변경시 처리되지 않은 항목이 많아 Main Rebar입력창으로 강재적으로 돌림

	if(m_ctrlTab.GetCurSel() == 0)
		iCurSelIJ = m_pMainRebar->GetCurSel();
	else
		iCurSelIJ = m_pShearRebar->GetCurSel();

	REBAR_KEY Key;
	T_REBT_MABR Data;

	if(iCurSelMainShear == 0 && iCurSelIJ == 0)
	{
		BOOL bOldLock = m_pMainRebar->m_pMainRebarI->m_pRebarGrid->LockUpdate(TRUE);

		POSITION PosI = m_MabrDataListI.GetStartPosition();
		while(PosI)
		{
			m_MabrDataListI.GetNextAssoc(PosI,Key,Data);	
			m_pMainRebar->RemoveMainRebarDataListI(Key, Data);
		}
		m_pMainRebar->m_pMainRebarI->m_pRebarGrid->LockUpdate(bOldLock);
		if (!bOldLock) m_pMainRebar->m_pMainRebarI->m_pRebarGrid->Redraw();
	}
	if(iCurSelMainShear == 0 && iCurSelIJ == 1)
	{
		BOOL bOldLock = m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->LockUpdate(TRUE);

		POSITION PosJ = m_MabrDataListJ.GetStartPosition();
		while(PosJ)
		{
			m_MabrDataListJ.GetNextAssoc(PosJ,Key,Data);	
			m_pMainRebar->RemoveMainRebarDataListJ(Key, Data);
		}
		m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->LockUpdate(bOldLock);
		if (!bOldLock) m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->Redraw();
	}
	m_MabrDataListI.RemoveAll();
	m_MabrDataListJ.RemoveAll();

	bMainSameIJ = FALSE;
	bShearSameIJ = FALSE;
	m_ShearRebarDataI.Initialize();
	m_ShearRebarDataJ.Initialize();

	OnDgnSectImportButton();
}

void ConcSectionRebarViewDlg::SetCurDataIJ(BOOL bI_End) 
{
	//해당되는 단면을 그린다


	//그리드에 철근정보를 세팅한다
}

void ConcSectionRebarViewDlg::DrawSectionRebarTabChanged() 
{
	m_pIVP->DeleteAllObject();
	T_SECT_K SectKey;

	//단면을 그린다
	if(nSectionType == 1)
	{
		int RedamSectIndex = m_RandomSectionCombo.GetCurSel();
		if(RedamSectIndex < 0) return;
		
		m_RandomSectionList.Lookup(RedamSectIndex, SectKey);
	}
	else if(nSectionType == 0)
	{
		T_ELEM_D ElemData;
		if(!m_pDoc->m_pAttrCtrl->GetElem(Elemkey, ElemData)) return;
		SectKey = ElemData.elpro;
	}
	
	//060331 Modify sshan
	//if(!m_pDoc->m_pAttrCtrl->GetSect(SectKey, m_sectData))	return;
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectKey, m_sectData))	return;

	if(m_sectData.nStype == D_SECT_TYPE_TAPERED)
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = m_sectData.SectBefore.SectJ.Design.YBar;
		OffsetJ_Z = m_sectData.SectBefore.SectJ.Design.ZBar;
	}
	else
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = OffsetI_Y;
		OffsetJ_Z = OffsetI_Z;
	}

	int iPosi;
	if(m_ctrlTab.GetCurSel() == 0)
		iPosi = m_pMainRebar->GetCurSel();
	else
		iPosi = m_pShearRebar->GetCurSel();

	if(ImportSection(iPosi))
	{
		NowSectKey = SectKey;
		bDrawSection = TRUE;
	}	
	
	if(m_ctrlTab.GetCurSel() == 0)//주철근 입력중
	{
		//철근을 그린다
		int iCurSel = m_pMainRebar->GetCurSel();
			
		if(iCurSel == 0) //i단
		{
			m_pMainRebar->SetRebarListDataI(m_MabrDataListI);
		}
		else //j단
		{
			m_pMainRebar->SetRebarListDataJ(m_MabrDataListJ);
		}
	}

	m_pIVP->RedrawAll();
}

void ConcSectionRebarViewDlg::OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_ctrlTab.GetCurSel() == 0)
		DrawSectionRebarTabChanged();
}

void ConcSectionRebarViewDlg::SetSectionInputType(int nSectiontype, BOOL bIsShearRebar, BOOL bIsScaleTaperedGroup)
{
	nSectionType = nSectiontype;
	m_bIsScaleTaperedGroup = bIsScaleTaperedGroup;
	m_bIsShearRebar = bIsShearRebar;
}

void ConcSectionRebarViewDlg::SetSectionInputType(int nSectiontype, CString strName, T_SECT_D SectData, T_REBT_D RebtData)
{
	m_strName = strName;
	nSectionType = nSectiontype;
	m_sectData = SectData;
	m_RebtData = RebtData;
}

void ConcSectionRebarViewDlg::SetElemKey(T_ELEM_K key)
{
	Elemkey = key;
}


void ConcSectionRebarViewDlg::SetRandomSectionComboHide()
{
	if(nSectionType == 0)//요소별 철근 입력일경우는 보여주지 않는다
	{
		GetDlgItem(IDC_DGN_TYPE1_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RANDOM_SECT_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_DEFINE_SECT_BUTTON)->ShowWindow(SW_HIDE);
		m_Ctrl_ElemNumber_Edit.Format(_T("Element Number : %d"), Elemkey);
	}
	else if(nSectionType == 1)
	{
		GetDlgItem(IDC_DGN_ELEM_NUMBER_EDIT)->ShowWindow(SW_HIDE);
	}
	// RC_UMD_COLUMN :: Code By Unsang!!
	else if(nSectionType == 2)// RC_UMD_COLUMN :: Code By Unsang: 일단, 위치확보!!
	{
		GetDlgItem(IDC_DGN_TYPE1_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_RANDOM_SECT_COMBO)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_DGN_DEFINE_SECT_BUTTON)->ShowWindow(SW_HIDE);
		//m_Ctrl_ElemNumber_Edit.Format(_T("Element Number : %d"), Elemkey);
		CString strElemName = _T("");
		strElemName = _T("Element Name :") + m_strName;
		GetDlgItem(IDC_DGN_ELEM_NUMBER_EDIT)->SetWindowText(strElemName);
	}
}

// Modify!!
// RC_UMD_COLUMN :: Code By Unsang.. New_Code!!
void ConcSectionRebarViewDlg::SectionImportByElemUsedUMD() 
{
	m_pIVP->DeleteAllObject();

	int iCurSel;
	if(m_ctrlTab.GetCurSel() == 0)
		iCurSel = m_pMainRebar->GetCurSel();
	else
		iCurSel = m_pShearRebar->GetCurSel();
		
	if(m_sectData.nStype == D_SECT_TYPE_TAPERED)
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = m_sectData.SectBefore.SectJ.Design.YBar;
		OffsetJ_Z = m_sectData.SectBefore.SectJ.Design.ZBar;
	}
	else
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = OffsetI_Y;
		OffsetJ_Z = OffsetI_Z;
	}

	m_iCenterType = m_RebtData.iCenterType;
	if(ImportSection(iCurSel))
	{
//		NowSectKey = SectKey;
		bDrawSection = TRUE;
	}
	
	
	if(m_RebtData.iCenterType == 1)//좌하단기준
	{
		for(int i=0 ; i<m_RebtData.arMainRebar[0].GetSize() ; i++)
		{
			m_RebtData.arMainRebar[0][i].dCenterY += OffsetI_Y;
			m_RebtData.arMainRebar[0][i].dCenterZ += OffsetI_Z;
		}

		// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
		for(int i=0 ; i<m_RebtData.arMainRebar[2].GetSize() ; i++)
		{
			m_RebtData.arMainRebar[2][i].dCenterY += OffsetJ_Y;
			m_RebtData.arMainRebar[2][i].dCenterZ += OffsetJ_Z;
		}
	}

	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		if(iCurSel == 0) //i단
		{
			m_pMainRebar->SetInitDataI(m_RebtData.arMainRebar[0]);
			
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			for(int i=0 ; i<m_RebtData.arMainRebar[2].GetSize() ; i++)
			{
				m_MabrDataListJ.SetAt(i, m_RebtData.arMainRebar[2][i]);
			}

			m_pMainRebar->LoadDlgData(m_RebtData.bIJBothMain);

			bShearSameIJ = m_RebtData.bIJBothShear;
			m_ShearRebarDataI.dSubRebarSpace = m_RebtData.ShearRebar[0].dSubRebarSpace;
			m_ShearRebarDataI.dSubRebarNum[0] = m_RebtData.ShearRebar[0].dSubRebarNum[0];
			m_ShearRebarDataI.dSubRebarNum[1] = m_RebtData.ShearRebar[0].dSubRebarNum[1];
			m_ShearRebarDataI.strSubRebarName = m_RebtData.ShearRebar[0].strSubRebarName;

			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_ShearRebarDataJ.dSubRebarSpace = m_RebtData.ShearRebar[2].dSubRebarSpace;
			m_ShearRebarDataJ.dSubRebarNum[0] = m_RebtData.ShearRebar[2].dSubRebarNum[0];
			m_ShearRebarDataJ.dSubRebarNum[1] = m_RebtData.ShearRebar[2].dSubRebarNum[1];
			m_ShearRebarDataJ.strSubRebarName = m_RebtData.ShearRebar[2].strSubRebarName;
		}
		else //j단
		{
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_pMainRebar->SetInitDataJ(m_RebtData.arMainRebar[2]);

			for(int i=0 ; i<m_RebtData.arMainRebar[0].GetSize() ; i++)
			{
				m_MabrDataListI.SetAt(i, m_RebtData.arMainRebar[0][i]);
			}
			
			m_pMainRebar->LoadDlgData(m_RebtData.bIJBothMain);

			bShearSameIJ = m_RebtData.bIJBothShear;
			m_ShearRebarDataI.dSubRebarSpace = m_RebtData.ShearRebar[0].dSubRebarSpace;
			m_ShearRebarDataI.dSubRebarNum[0] = m_RebtData.ShearRebar[0].dSubRebarNum[0];
			m_ShearRebarDataI.dSubRebarNum[1] = m_RebtData.ShearRebar[0].dSubRebarNum[1];
			m_ShearRebarDataI.strSubRebarName = m_RebtData.ShearRebar[0].strSubRebarName;

			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_ShearRebarDataJ.dSubRebarSpace = m_RebtData.ShearRebar[2].dSubRebarSpace;
			m_ShearRebarDataJ.dSubRebarNum[0] = m_RebtData.ShearRebar[2].dSubRebarNum[0];
			m_ShearRebarDataJ.dSubRebarNum[1] = m_RebtData.ShearRebar[2].dSubRebarNum[1];
			m_ShearRebarDataJ.strSubRebarName = m_RebtData.ShearRebar[2].strSubRebarName;					
		}		
	}
	else
	{
		if(iCurSel == 0) //i단
		{
			int i;
			for(i=0 ; i<m_RebtData.arMainRebar[0].GetSize() ; i++)
			{
				m_MabrDataListI.SetAt(i, m_RebtData.arMainRebar[0][i]);
			}
			
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			for(i=0 ; i<m_RebtData.arMainRebar[2].GetSize() ; i++)
			{
				m_MabrDataListJ.SetAt(i, m_RebtData.arMainRebar[2][i]);
			}

			bMainSameIJ = m_RebtData.bIJBothMain;

			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_pShearRebar->LoadDlgData(m_RebtData.bIJBothShear, m_RebtData.ShearRebar[0], m_RebtData.ShearRebar[2]);
		}
		else //j단
		{
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			int i;
			for(i=0 ; i<m_RebtData.arMainRebar[2].GetSize() ; i++)
			{
				m_MabrDataListJ.SetAt(i, m_RebtData.arMainRebar[2][i]);
			}

			for(i=0 ; i<m_RebtData.arMainRebar[0].GetSize() ; i++)
			{
				m_MabrDataListI.SetAt(i, m_RebtData.arMainRebar[0][i]);
			}
			
			bMainSameIJ = m_RebtData.bIJBothMain;

			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_pShearRebar->LoadDlgData(m_RebtData.bIJBothShear, m_RebtData.ShearRebar[0], m_RebtData.ShearRebar[2]);
		}		
	}

	m_iCenterType = m_RebtData.iCenterType;
	priCenterType = m_RebtData.iCenterType;
	UpdateData(FALSE);

	m_pMainRebar->SetAsRebarArea();
	
	m_pIVP->ZoomAll();
}


void ConcSectionRebarViewDlg::SectionImportByElem() 
{
	m_pIVP->DeleteAllObject();

	int iCurSel;
	if(m_ctrlTab.GetCurSel() == 0)
		iCurSel = m_pMainRebar->GetCurSel();
	else
		iCurSel = m_pShearRebar->GetCurSel();

	T_SECT_K SectKey;
	T_ELEM_D ElemData;
	if(!m_pDoc->m_pAttrCtrl->GetElem(Elemkey, ElemData)) return;
	SectKey = ElemData.elpro;

	//060331 Modify sshan
	//if(!m_pDoc->m_pAttrCtrl->GetSect(SectKey, m_sectData))	return;
	if(!m_pDoc->m_pAttrCtrl->GetSectDesign(SectKey, m_sectData))	return;

	if(m_sectData.nStype == D_SECT_TYPE_TAPERED)
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = m_sectData.SectBefore.SectJ.Design.YBar;
		OffsetJ_Z = m_sectData.SectBefore.SectJ.Design.ZBar;
	}
	else
	{
		OffsetI_Y = m_sectData.SectBefore.SectI.Design.YBar;
		OffsetI_Z = m_sectData.SectBefore.SectI.Design.ZBar;
		OffsetJ_Y = OffsetI_Y;
		OffsetJ_Z = OffsetI_Z;
	}

	//S_RBTE_T에 요소별 데이터가 있으면 그곳 정보를 불러오고
	//없으면 요소의 Section키로 T_REBT_D에서 철근 정보를 불러온다.
	//모두 없으면 불러오지 않는다.
	T_REBT_D RebtData;
	T_RBTE_D RbteData;
	CDgnSectionTool::GetRbte(Elemkey, RbteData);
	CDgnSectionTool::Cng_From_RBTE_To_REBT(RbteData, RebtData);

	m_iCenterType = RebtData.iCenterType;
	if(ImportSection(iCurSel))
	{
		NowSectKey = SectKey;
		bDrawSection = TRUE;
	}
	
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	if(RebtData.iCenterType == 1)//좌하단기준
	{
		for(int i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
		{
			RebtData.arMainRebar[0][i].dCenterY += OffsetI_Y;
			RebtData.arMainRebar[0][i].dCenterZ += OffsetI_Z;
		}
		for(int i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
		{
			RebtData.arMainRebar[2][i].dCenterY += OffsetJ_Y;
			RebtData.arMainRebar[2][i].dCenterZ += OffsetJ_Z;
		}
	}

	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		if(iCurSel == 0) //i단
		{
			m_pMainRebar->SetInitDataI(RebtData.arMainRebar[0]);
			
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			for(int i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
			{
				m_MabrDataListJ.SetAt(i, RebtData.arMainRebar[2][i]);
			}

			m_pMainRebar->LoadDlgData(RebtData.bIJBothMain);

			bShearSameIJ = RebtData.bIJBothShear;
			m_ShearRebarDataI.dSubRebarSpace = RebtData.ShearRebar[0].dSubRebarSpace;
			m_ShearRebarDataI.dSubRebarNum[0] = RebtData.ShearRebar[0].dSubRebarNum[0];
			m_ShearRebarDataI.dSubRebarNum[1] = RebtData.ShearRebar[0].dSubRebarNum[1];
			m_ShearRebarDataI.strSubRebarName = RebtData.ShearRebar[0].strSubRebarName;
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_ShearRebarDataJ.dSubRebarSpace = RebtData.ShearRebar[2].dSubRebarSpace;
			m_ShearRebarDataJ.dSubRebarNum[0] = RebtData.ShearRebar[2].dSubRebarNum[0];
			m_ShearRebarDataJ.dSubRebarNum[1] = RebtData.ShearRebar[2].dSubRebarNum[1];
			m_ShearRebarDataJ.strSubRebarName = RebtData.ShearRebar[2].strSubRebarName;
		}
		else //j단
		{
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_pMainRebar->SetInitDataJ(RebtData.arMainRebar[2]);

			for(int i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
			{
				m_MabrDataListI.SetAt(i, RebtData.arMainRebar[0][i]);
			}
			
			m_pMainRebar->LoadDlgData(RebtData.bIJBothMain);

			bShearSameIJ = RebtData.bIJBothShear;
			m_ShearRebarDataI.dSubRebarSpace = RebtData.ShearRebar[0].dSubRebarSpace;
			m_ShearRebarDataI.dSubRebarNum[0] = RebtData.ShearRebar[0].dSubRebarNum[0];
			m_ShearRebarDataI.dSubRebarNum[1] = RebtData.ShearRebar[0].dSubRebarNum[1];
			m_ShearRebarDataI.strSubRebarName = RebtData.ShearRebar[0].strSubRebarName;
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_ShearRebarDataJ.dSubRebarSpace = RebtData.ShearRebar[2].dSubRebarSpace;
			m_ShearRebarDataJ.dSubRebarNum[0] = RebtData.ShearRebar[2].dSubRebarNum[0];
			m_ShearRebarDataJ.dSubRebarNum[1] = RebtData.ShearRebar[2].dSubRebarNum[1];
			m_ShearRebarDataJ.strSubRebarName = RebtData.ShearRebar[2].strSubRebarName;					
		}		
	}
	else
	{
		if(iCurSel == 0) //i단
		{
			int i;
			for(i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
			{
				m_MabrDataListI.SetAt(i, RebtData.arMainRebar[0][i]);
			}

			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			for(i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
			{
				m_MabrDataListJ.SetAt(i, RebtData.arMainRebar[2][i]);
			}

			bMainSameIJ = RebtData.bIJBothMain;

			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			m_pShearRebar->LoadDlgData(RebtData.bIJBothShear, RebtData.ShearRebar[0], RebtData.ShearRebar[2]);
		}
		else //j단
		{
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			int i;
			for(i=0 ; i<RebtData.arMainRebar[2].GetSize() ; i++)
			{
				m_MabrDataListJ.SetAt(i, RebtData.arMainRebar[2][i]);
			}

			for(i=0 ; i<RebtData.arMainRebar[0].GetSize() ; i++)
			{
				m_MabrDataListI.SetAt(i, RebtData.arMainRebar[0][i]);
			}
			
			// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
			bMainSameIJ = RebtData.bIJBothMain;
			m_pShearRebar->LoadDlgData(RebtData.bIJBothShear, RebtData.ShearRebar[0], RebtData.ShearRebar[2]);
		}		
	}

	m_iCenterType = RebtData.iCenterType;
	priCenterType = RebtData.iCenterType;
	UpdateData(FALSE);

	m_pMainRebar->SetAsRebarArea();
	
	m_pIVP->ZoomAll();
}


BOOL ConcSectionRebarViewDlg::Cng_From_REBT_To_RBTE(T_REBT_D rebtData, T_RBTE_D& rbteData)
{
	rbteData.Initialize();
	rbteData.iCenterType  = rebtData.iCenterType;
	rbteData.bIJBothMain  = rebtData.bIJBothMain;
	rbteData.bIJBothShear = rebtData.bIJBothShear;

	int MainRebarNum = rebtData.arMainRebar[0].GetSize();
	rbteData.arMainRebar[0].SetSize(MainRebarNum);
	MainRebarNum = rebtData.arMainRebar[1].GetSize();
	rbteData.arMainRebar[1].SetSize(MainRebarNum);
	MainRebarNum = rebtData.arMainRebar[2].GetSize();
	rbteData.arMainRebar[2].SetSize(MainRebarNum);
	for(int i=0 ; i<rebtData.arMainRebar[0].GetSize() ; i++)
	{
		rbteData.arMainRebar[0][i].iLabel = rebtData.arMainRebar[0][i].iLabel;
		rbteData.arMainRebar[0][i].dCenterY = rebtData.arMainRebar[0][i].dCenterY;
		rbteData.arMainRebar[0][i].dCenterZ = rebtData.arMainRebar[0][i].dCenterZ;
		rbteData.arMainRebar[0][i].strRebarName = rebtData.arMainRebar[0][i].strRebarName;
	}
	for(int i=0 ; i<rebtData.arMainRebar[1].GetSize() ; i++)
	{
		rbteData.arMainRebar[1][i].iLabel = rebtData.arMainRebar[1][i].iLabel;
		rbteData.arMainRebar[1][i].dCenterY = rebtData.arMainRebar[1][i].dCenterY;
		rbteData.arMainRebar[1][i].dCenterZ = rebtData.arMainRebar[1][i].dCenterZ;
		rbteData.arMainRebar[1][i].strRebarName = rebtData.arMainRebar[1][i].strRebarName;
	}
	for(int i=0 ; i<rebtData.arMainRebar[2].GetSize() ; i++)
	{
		rbteData.arMainRebar[2][i].iLabel = rebtData.arMainRebar[2][i].iLabel;
		rbteData.arMainRebar[2][i].dCenterY = rebtData.arMainRebar[2][i].dCenterY;
		rbteData.arMainRebar[2][i].dCenterZ = rebtData.arMainRebar[2][i].dCenterZ;
		rbteData.arMainRebar[2][i].strRebarName = rebtData.arMainRebar[2][i].strRebarName;
	}

	rbteData.ShearRebar[0].dSubRebarSpace = rebtData.ShearRebar[0].dSubRebarSpace;
	rbteData.ShearRebar[0].dSubRebarNum[0] = rebtData.ShearRebar[0].dSubRebarNum[0];
	rbteData.ShearRebar[0].dSubRebarNum[1] = rebtData.ShearRebar[0].dSubRebarNum[1];
	rbteData.ShearRebar[0].strSubRebarName = rebtData.ShearRebar[0].strSubRebarName;

	rbteData.ShearRebar[1].dSubRebarSpace = rebtData.ShearRebar[1].dSubRebarSpace;
	rbteData.ShearRebar[1].dSubRebarNum[0] = rebtData.ShearRebar[1].dSubRebarNum[0];
	rbteData.ShearRebar[1].dSubRebarNum[1] = rebtData.ShearRebar[1].dSubRebarNum[1];
	rbteData.ShearRebar[1].strSubRebarName = rebtData.ShearRebar[1].strSubRebarName;

	rbteData.ShearRebar[2].dSubRebarSpace = rebtData.ShearRebar[2].dSubRebarSpace;
	rbteData.ShearRebar[2].dSubRebarNum[0] = rebtData.ShearRebar[2].dSubRebarNum[0];
	rbteData.ShearRebar[2].dSubRebarNum[1] = rebtData.ShearRebar[2].dSubRebarNum[1];
	rbteData.ShearRebar[2].strSubRebarName = rebtData.ShearRebar[2].strSubRebarName;

	return TRUE;
}

BOOL ConcSectionRebarViewDlg::Cng_From_RBTE_To_REBT(T_RBTE_D rbteData, T_REBT_D& rebtData)
{
	rebtData.Initialize();
	rebtData.iCenterType  = rbteData.iCenterType;
	rebtData.bIJBothMain  = rbteData.bIJBothMain;
	rebtData.bIJBothShear = rbteData.bIJBothShear;
	
	int MainRebarNum = rbteData.arMainRebar[0].GetSize();
	rebtData.arMainRebar[0].SetSize(MainRebarNum);
	MainRebarNum = rbteData.arMainRebar[1].GetSize();
	rebtData.arMainRebar[1].SetSize(MainRebarNum);
	MainRebarNum = rbteData.arMainRebar[2].GetSize();
	rebtData.arMainRebar[2].SetSize(MainRebarNum);
	for(int i=0 ; i<rbteData.arMainRebar[0].GetSize() ; i++)
	{
		rebtData.arMainRebar[0][i].iLabel = rbteData.arMainRebar[0][i].iLabel;
		rebtData.arMainRebar[0][i].dCenterY = rbteData.arMainRebar[0][i].dCenterY;
		rebtData.arMainRebar[0][i].dCenterZ = rbteData.arMainRebar[0][i].dCenterZ;
		rebtData.arMainRebar[0][i].strRebarName = rbteData.arMainRebar[0][i].strRebarName;
	}
	for(int i=0 ; i<rbteData.arMainRebar[1].GetSize() ; i++)
	{
		rebtData.arMainRebar[1][i].iLabel = rbteData.arMainRebar[1][i].iLabel;
		rebtData.arMainRebar[1][i].dCenterY = rbteData.arMainRebar[1][i].dCenterY;
		rebtData.arMainRebar[1][i].dCenterZ = rbteData.arMainRebar[1][i].dCenterZ;
		rebtData.arMainRebar[1][i].strRebarName = rbteData.arMainRebar[1][i].strRebarName;
	}
	for(int i=0 ; i<rbteData.arMainRebar[2].GetSize() ; i++)
	{
		rebtData.arMainRebar[2][i].iLabel = rbteData.arMainRebar[2][i].iLabel;
		rebtData.arMainRebar[2][i].dCenterY = rbteData.arMainRebar[2][i].dCenterY;
		rebtData.arMainRebar[2][i].dCenterZ = rbteData.arMainRebar[2][i].dCenterZ;
		rebtData.arMainRebar[2][i].strRebarName = rbteData.arMainRebar[2][i].strRebarName;
	}

	rebtData.ShearRebar[0].dSubRebarSpace = rbteData.ShearRebar[0].dSubRebarSpace;
	rebtData.ShearRebar[0].dSubRebarNum[0] = rbteData.ShearRebar[0].dSubRebarNum[0];
	rebtData.ShearRebar[0].dSubRebarNum[1] = rbteData.ShearRebar[0].dSubRebarNum[1];
	rebtData.ShearRebar[0].strSubRebarName = rbteData.ShearRebar[0].strSubRebarName;

	rebtData.ShearRebar[1].dSubRebarSpace = rbteData.ShearRebar[1].dSubRebarSpace;
	rebtData.ShearRebar[1].dSubRebarNum[0] = rbteData.ShearRebar[1].dSubRebarNum[0];
	rebtData.ShearRebar[1].dSubRebarNum[1] = rbteData.ShearRebar[1].dSubRebarNum[1];
	rebtData.ShearRebar[1].strSubRebarName = rbteData.ShearRebar[1].strSubRebarName;

	rebtData.ShearRebar[2].dSubRebarSpace = rbteData.ShearRebar[2].dSubRebarSpace;
	rebtData.ShearRebar[2].dSubRebarNum[0] = rbteData.ShearRebar[2].dSubRebarNum[0];
	rebtData.ShearRebar[2].dSubRebarNum[1] = rbteData.ShearRebar[2].dSubRebarNum[1];
	rebtData.ShearRebar[2].strSubRebarName = rbteData.ShearRebar[2].strSubRebarName;

	return TRUE;
}


void ConcSectionRebarViewDlg::Data2Dlg()
{
	
}

BOOL ConcSectionRebarViewDlg::Dlg2Data()
{  
	return TRUE;
}

BOOL ConcSectionRebarViewDlg::SaveData()
{ 
	BOOL bOK = FALSE;

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	T_REBT_D DBInputData;
	DBInputData.Initialize();

	int MainRebarNum = m_MabrDataListI.GetCount();
	DBInputData.arMainRebar[0].SetSize(MainRebarNum);
	MainRebarNum = m_MabrDataListJ.GetCount();
	DBInputData.arMainRebar[2].SetSize(MainRebarNum);

	REBAR_KEY RebarKey;
	T_REBT_MABR MaReberData;
	int Index = 0;
	POSITION Pos = m_MabrDataListI.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListI.GetNextAssoc(Pos,RebarKey,MaReberData);
		MaReberData.iLabel = Index + 1;
		DBInputData.arMainRebar[0].SetAt(Index++, MaReberData);
	}


	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	Index = 0;
	Pos = m_MabrDataListJ.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListJ.GetNextAssoc(Pos,RebarKey,MaReberData);
		MaReberData.iLabel = Index + 1;
		DBInputData.arMainRebar[2].SetAt(Index++, MaReberData);
	}
	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pMainRebar->SaveDlgData();
		bMainSameIJ = m_pMainRebar->bSameIJ;
	}
	else
	{
		m_pShearRebar->SaveDlgData();
		bShearSameIJ = m_pShearRebar->bSameIJ;
		m_ShearRebarDataI = m_pShearRebar->m_DataI;
		m_ShearRebarDataJ = m_pShearRebar->m_DataJ;
	}

	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	DBInputData.bIJBothMain = bMainSameIJ;
	DBInputData.bIJBothShear = bShearSameIJ;
	DBInputData.ShearRebar[0] = m_ShearRebarDataI;
	DBInputData.ShearRebar[2] = m_ShearRebarDataJ;

	UpdateData(TRUE);
	
	DBInputData.iCenterType = m_iCenterType;
	
	if(DBInputData.iCenterType == 1)//좌하단기준
	{
		for(int i=0 ; i<DBInputData.arMainRebar[0].GetSize() ; i++)
		{
			DBInputData.arMainRebar[0][i].dCenterY -= OffsetI_Y;
			DBInputData.arMainRebar[0][i].dCenterZ -= OffsetI_Z;
		}

		// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
		for(int i=0 ; i<DBInputData.arMainRebar[2].GetSize() ; i++)
		{
			DBInputData.arMainRebar[2][i].dCenterY -= OffsetJ_Y;
			DBInputData.arMainRebar[2][i].dCenterZ -= OffsetJ_Z;
		}
	}

	// add by SeungJun (`06.03.27)
	// use shearRebar data of RBTE
	/*
	DBInputData.ShearRebar[0].dSubRebarNum[0] = 0.0;
	DBInputData.ShearRebar[0].dSubRebarNum[1] = 0.0;
	DBInputData.ShearRebar[1].dSubRebarNum[0] = 0.0;
	DBInputData.ShearRebar[1].dSubRebarNum[1] = 0.0;
	*/

	//부재별 입력일경우는 Rbte에
	//Section별 일경우는 Rebt에 저장
	if(nSectionType == 1)
	{
		if(m_pDoc->m_pAttrCtrl->ExistRebt(NowSectKey))
			bOK = m_pDoc->m_pDataCtrl->ModifyRebt(NowSectKey, NowSectKey, DBInputData);
		else
			bOK = m_pDoc->m_pDataCtrl->AddRebt(NowSectKey, DBInputData);
	}
	else if(nSectionType == 0)
	{
		T_RBTE_D RbteData;
		Cng_From_REBT_To_RBTE(DBInputData, RbteData);

		if(m_pDoc->m_pAttrCtrl->ExistRbte(Elemkey))
			bOK = m_pDoc->m_pDataCtrl->ModifyRbte(Elemkey, Elemkey, RbteData);
		else
			bOK = m_pDoc->m_pDataCtrl->AddRbte(Elemkey, RbteData);
	}
	else// RC_UMD_COLUMN :: Code By Unsang!! 
	{
		m_RebtData = DBInputData;
		bOK = TRUE;
	}
	
	return bOK;
}

/*
BOOL ConcSectionRebarViewDlg::Get_ConvertRebtData(T_REBT_D DBInputData, T_REBT_D RebtData)
{
	RebtData = DBInputData;

	return TRUE;
}
*/

void ConcSectionRebarViewDlg::OnChangedCenterType_Center() 
{
	UpdateData(TRUE);
	if(priCenterType == 0) return;

	ChangedCenterType(TRUE);
	priCenterType = 0;
}

void ConcSectionRebarViewDlg::OnChangedCenterType_LeftBottom() 
{
	UpdateData(TRUE);
	if(priCenterType == 1) return;

	ChangedCenterType(FALSE);
	priCenterType = 1;
}

void ConcSectionRebarViewDlg::ChangedCenterType(BOOL bCenType) 
{
	m_pIVP->DeleteAllObject();

	int iCurSel;
	if(m_ctrlTab.GetCurSel()==0)
		iCurSel = m_pMainRebar->GetCurSel();
	else
		iCurSel = m_pShearRebar->GetCurSel();
	//단면을 다시 그린다
	if(ImportSection(iCurSel))
	{		
		bDrawSection = TRUE;
	}

	//그리드에 값을 지워준다
	int iCurSelIJ;
	int iCurSelMainShear;
	iCurSelMainShear = m_ctrlTab.GetCurSel();
	if(m_ctrlTab.GetCurSel() == 0)
		iCurSelIJ = m_pMainRebar->GetCurSel();
	else
		iCurSelIJ = m_pShearRebar->GetCurSel();

	REBAR_KEY Key;
	T_REBT_MABR Data;

	if(iCurSelMainShear == 0 && iCurSelIJ == 0)
	{
		BOOL bOldLock = m_pMainRebar->m_pMainRebarI->m_pRebarGrid->LockUpdate(TRUE);

		POSITION PosI = m_MabrDataListI.GetStartPosition();
		while(PosI)
		{
			m_MabrDataListI.GetNextAssoc(PosI,Key,Data);	
			m_pMainRebar->RemoveMainRebarDataListI(Key, Data);			
		}
		m_pMainRebar->m_pMainRebarI->m_pRebarGrid->LockUpdate(bOldLock);
		if (!bOldLock) m_pMainRebar->m_pMainRebarI->m_pRebarGrid->Redraw();
	}
	if(iCurSelMainShear == 0 && iCurSelIJ == 1)
	{		
		BOOL bOldLock = m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->LockUpdate(TRUE);

		POSITION PosJ = m_MabrDataListJ.GetStartPosition();
		while(PosJ)
		{
			m_MabrDataListJ.GetNextAssoc(PosJ,Key,Data);	
			m_pMainRebar->RemoveMainRebarDataListJ(Key, Data);
		}
		m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->LockUpdate(bOldLock);
		if (!bOldLock) m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->Redraw();
	}

	//기존 철근 값을 offset하여 철근을 다시그린다
	if(bDrawSection)
	{
		T_REBT_D OffsetData;
		OffsetData.Initialize();
		int MainRebarNum = m_MabrDataListI.GetCount();
		OffsetData.arMainRebar[0].SetSize(MainRebarNum);

		// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
		MainRebarNum = m_MabrDataListJ.GetCount();
		OffsetData.arMainRebar[2].SetSize(MainRebarNum);

		REBAR_KEY RebarKey;
		T_REBT_MABR MaReberData;
		int Index = 0;
		
		POSITION Pos = m_MabrDataListI.GetStartPosition();
		while(Pos)
		{
			m_MabrDataListI.GetNextAssoc(Pos,RebarKey,MaReberData);
			MaReberData.iLabel = Index + 1;
			if(bCenType)
			{
				MaReberData.dCenterY = MaReberData.dCenterY - OffsetI_Y;
				MaReberData.dCenterZ = MaReberData.dCenterZ - OffsetI_Z;
			}
			else
			{
				MaReberData.dCenterY = MaReberData.dCenterY + OffsetI_Y;
				MaReberData.dCenterZ = MaReberData.dCenterZ + OffsetI_Z;
			}
			OffsetData.arMainRebar[0].SetAt(Index++, MaReberData);
		}
				
		// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
		Index = 0;
		Pos = m_MabrDataListJ.GetStartPosition();
		while(Pos)
		{
			m_MabrDataListJ.GetNextAssoc(Pos,RebarKey,MaReberData);
			MaReberData.iLabel = Index + 1;
			if(bCenType)
			{
				MaReberData.dCenterY = MaReberData.dCenterY - OffsetJ_Y;
				MaReberData.dCenterZ = MaReberData.dCenterZ - OffsetJ_Z;
			}
			else
			{
				MaReberData.dCenterY = MaReberData.dCenterY + OffsetJ_Y;
				MaReberData.dCenterZ = MaReberData.dCenterZ + OffsetJ_Z;
			}
			OffsetData.arMainRebar[2].SetAt(Index++, MaReberData);
		}
		
		m_MabrDataListI.RemoveAll();
		m_MabrDataListJ.RemoveAll();

		for(int i=0 ; i<OffsetData.arMainRebar[0].GetSize() ; i++)
		{
			m_MabrDataListI.SetAt(i, OffsetData.arMainRebar[0][i]);
		}

		// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
		for(int i=0 ; i<OffsetData.arMainRebar[2].GetSize() ; i++)
		{
			m_MabrDataListJ.SetAt(i, OffsetData.arMainRebar[2][i]);
		}
		
		if(m_ctrlTab.GetCurSel()==0)
		{
			if(iCurSel == 0) //i단
			{
				BOOL bOldLock = m_pMainRebar->m_pMainRebarI->m_pRebarGrid->LockUpdate(TRUE);

				m_pMainRebar->SetRebarListDataI(m_MabrDataListI);

				m_pMainRebar->m_pMainRebarI->m_pRebarGrid->LockUpdate(bOldLock);
				if (!bOldLock) m_pMainRebar->m_pMainRebarI->m_pRebarGrid->Redraw();
			}
			else //j단
			{
				BOOL bOldLock = m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->LockUpdate(TRUE);

				m_pMainRebar->SetRebarListDataJ(m_MabrDataListJ);

				m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->LockUpdate(bOldLock);
				if (!bOldLock) m_pMainRebar->m_pMainRebarJ->m_pRebarGrid->Redraw();
			}
		}
	}

	m_pIVP->ZoomAll();
}

void ConcSectionRebarViewDlg::OnBtnApply() 
{
	if(bDrawSection)
	{
		BOOL bOK = SaveData();

		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_CON_SECT_REBAR_ERROR_INPUTDATA), MB_OK);//_T("입력값이 올바르지 않습니다.\n메인화면 하단 Message View를 확인하시기 바랍니다.")
	}
}

int ConcSectionRebarViewDlg::GetMainRebarI_LastNum() 
{
	int result = 0;
	
	REBAR_KEY RebarKey;
	T_REBT_MABR MaReberData;
	POSITION Pos = m_MabrDataListI.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListI.GetNextAssoc(Pos,RebarKey,MaReberData);
		if(result < MaReberData.iLabel)
			result = MaReberData.iLabel;				
	}

	return result;
}

int ConcSectionRebarViewDlg::GetMainRebarJ_LastNum() 
{
	int result = 0;
	
	REBAR_KEY RebarKey;
	T_REBT_MABR MaReberData;
	POSITION Pos = m_MabrDataListJ.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListJ.GetNextAssoc(Pos,RebarKey,MaReberData);
		if(result < MaReberData.iLabel)
			result = MaReberData.iLabel;				
	}

	return result;
}

void ConcSectionRebarViewDlg::ReDrawRebar(REBAR_KEY RebarKey, T_REBT_MABR MaReberData, BOOL bI_End) 
{
	DeleteRebarData(RebarKey, bI_End);
	AddRebarData(RebarKey, MaReberData, bI_End);
}