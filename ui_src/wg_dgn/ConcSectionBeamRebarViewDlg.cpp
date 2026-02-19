// ConcSectionBeamRebarViewDlg.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "ConcSectionBeamRebarViewDlg.h"
#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\matlDB.h"
#include "..\wg_db\SectUtil.h"

#include "..\wg_base\wg_base_MsgDll.h"

#include "ConcSectionRebarEventHandler.h"
#include "ConcSectionMainBeamRebar.h"
#include "ConcSectionCreateBeamRebar.h"
#include "ConcSectionShearBeamRebar.h"
#include "ConcSectionTorsionBeamRebar.h"

#include "DgnSectionTool.h"
#include "DgnTabCtrl.h"
#include "..\wg_cmd\ExportFunc.h"

#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define M_PI        3.14159265358979323846


/////////////////////////////////////////////////////////////////////////////
// CConcSectionBeamRebarViewDlg dialog


CConcSectionBeamRebarViewDlg::CConcSectionBeamRebarViewDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CConcSectionBeamRebarViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConcSectionBeamRebarViewDlg)
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

	// Section : [0]offset.x, [1]high, [2]low, [3]left, [4]right, [5]offset.y
	m_pMainRebar    = NULL;
	m_pShearRebar   = NULL;
	m_pTorsionRebar = NULL;

	m_arViewGeom.RemoveAll();
}

CConcSectionBeamRebarViewDlg::~CConcSectionBeamRebarViewDlg()
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


void CConcSectionBeamRebarViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConcSectionBeamRebarViewDlg)
	DDX_Control(pDX, IDC_DGN_RANDOM_SECT_COMBO, m_cmdSect);
	DDX_Control(pDX, IDC_DGN_VIEW_TEXT_OUT_STATIC, m_strTextOut);
	DDX_Control(pDX, IDC_DGN_FUNCTION_TAB, m_ctrlTab);
	DDX_Radio(pDX, IDC_DGN_CENTERYPE_C_RADIO, m_iCenterType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConcSectionBeamRebarViewDlg, CDialogMove)
	//{{AFX_MSG_MAP(CConcSectionBeamRebarViewDlg)
	ON_BN_CLICKED(IDC_DGN_DEFINE_SECT_BUTTON, OnDgnDefineSectButton)
	ON_BN_CLICKED(IDC_DGN_SECT_IMPORT_BUTTON, OnDgnSectImportButton)
	ON_BN_CLICKED(IDC_DGN_CANCEL_SECT_IMPORT_BUTTON, OnDgnSectImportCancelButton)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_CBN_SELCHANGE(IDC_DGN_RANDOM_SECT_COMBO, OnSelchangeDgnRandomSectCombo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_FUNCTION_TAB, OnSelchangeDgnTab)
	ON_BN_CLICKED(IDC_DGN_CENTERYPE_C_RADIO, OnChangedCenterType_Center)
	ON_BN_CLICKED(IDC_DGN_CENTERYPE_LEFTBOTTOM_RADIO, OnChangedCenterType_LeftBottom)
	ON_BN_CLICKED(IDC_DGN_CON_SECT_REBAR_APPLY_BTN, OnBtnApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConcSectionBeamRebarViewDlg message handlers

BOOL CConcSectionBeamRebarViewDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	// Rebar Area 단위 산정 
	T_UNIT_INDEX CurUnit;
	m_pDoc->m_pUnitCtrl->GetUnitIndexCurrent(CurUnit);
	m_pDoc->m_pMatlDB->Create_RebarData(CurUnit.nBase_Length,CurUnit.nBase_Force,TRUE);

	/////////////////////////////////////////////////////////////////////////////////

	m_MabrDataListI.InitHashTable(101);
	m_MabrDataListM.InitHashTable(101);
	m_MabrDataListJ.InitHashTable(101);
	KeyDataListI.InitHashTable(101);
	KeyDataListM.InitHashTable(101);
	KeyDataListJ.InitHashTable(101);
	m_MabrDataListI.RemoveAll();
	m_MabrDataListM.RemoveAll();
	m_MabrDataListJ.RemoveAll();
	m_bMainSameIMJ = FALSE;
	m_bShearSameIMJ = FALSE;
	m_bTorsionSameIMJ = FALSE;
	m_ShearRebarDataI.Initialize();
	m_ShearRebarDataM.Initialize();
	m_ShearRebarDataJ.Initialize();
	m_TorsionRebarDataI.Initialize();
	m_TorsionRebarDataM.Initialize();
	m_TorsionRebarDataJ.Initialize();
	priCenterType = 0;
	
	m_pIVP->On_InitDialog(this,IDC_DGN_SECT_REBAR_VIEW);
	
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_TAB_PLACEHOLDER2);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);
	
	//................................
	m_pMainRebar = new CConcSectionMainBeamRebar(this);
	m_pMainRebar->SetDlgViewPanel(this->m_pIVP); //Create전에 View를 연결. 호출 위치를 변경하지 말것.
	m_ctrlTab.AddTab(m_pMainRebar, _LS(IDS_DGN_CON_SECT_REBAR_MAIN_REBAR), CConcSectionMainBeamRebar::IDD, TRUE);
	
	m_pShearRebar = new CConcSectionShearBeamRebar(this);
	m_ctrlTab.AddTab(m_pShearRebar, _LS(IDS_DGN_CON_SECT_REBAR_SHEAR_REBAR), CConcSectionShearBeamRebar::IDD, TRUE);
	
	m_pTorsionRebar = new CConcSectionTorsionBeamRebar(this);
	m_ctrlTab.AddTab(m_pTorsionRebar, _LS(IDS_DGN_CON_SECT_REBAR_TORSION_REBAR), CConcSectionTorsionBeamRebar::IDD, TRUE);
	
	m_ctrlTab.ShowTab(0);    

	SetToolBarIcons();

	InitialRandomSectionData();

	//.......................................................
	m_pEventHandler = new ConcSectionRebarEventHandler();
	m_pEventHandler->SetDlgViewPanel(m_pIVP);
	m_pIVP->PushEVHandler(m_pEventHandler);

	m_pIVP->ShowPointGrid(TRUE);

	m_pIVP->SnapFree();
	m_pIVP->ShowAxis(TRUE);
	m_pEventHandler->SetWndTexOut(&m_strTextOut);
	//..........................................................
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CConcSectionBeamRebarViewDlg::ImportSection(int iPosi)
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

BOOL CConcSectionBeamRebarViewDlg::ConvertSection(T_SECT_D* pSectD, CArray<double, double&> &arPos, int iPosi)
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

void CConcSectionBeamRebarViewDlg::SetToolBarIcons()
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
	SetICON(IDC_DGN_SECT_IMPORT_BUTTON,				  IDI_DGN_SECTION_IMPORT)
	SetICON(IDC_DGN_CANCEL_SECT_IMPORT_BUTTON,  IDI_DGN_SECTION_IMPORT_CANCEL)
	SetICON(IDC_DGN_RPSC_IMPORT_BUTTON,			  	IDI_DGN_REBAR_IMPORT)
	SetICON(IDC_DGN_RPSC_IMPORT_CANCEL_BUTTON,	IDI_DGN_REBAR_IMPORT_CANCEL)
	//SetICON(IDC_DGN_DEFINE_SECT_BUTTON,			  	IDI_DGN_DEFINE_SECTION)
}

void CConcSectionBeamRebarViewDlg::OnDgnSectImportButton() 
{
	m_pIVP->DeleteAllObject();

	int iCurSel;
	if(m_ctrlTab.GetCurSel() == 0)
		iCurSel = m_pMainRebar->GetCurSel();
	else if(m_ctrlTab.GetCurSel() == 1)
		iCurSel = m_pShearRebar->GetCurSel();
	else
		iCurSel = m_pTorsionRebar->GetCurSel();	

	int RedamSectIndex = m_cmdSect.GetCurSel();
	if(RedamSectIndex < 0) return;
	T_SECT_K SectKey;
	m_RandomSectionList.Lookup(RedamSectIndex, SectKey);

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
	if(m_pDoc->m_pAttrCtrl2->ExistRbgc(SectKey))
	{
		T_RBGC_D RbgcData;
		if(m_pDoc->m_pAttrCtrl2->GetRbgc(SectKey, RbgcData))
			m_iCenterType = RbgcData.nCenterType;
	}
	
	if(ImportSection(iCurSel))
	{
		NowSectKey = SectKey;
		bDrawSection = TRUE;
	}

	if(m_pDoc->m_pAttrCtrl2->ExistRbgc(SectKey))//해당 Section에 철근정보가 있으면 같이 불러와 그려준다.
	{
		T_RBGC_D RbgcData;
		
		if(m_pDoc->m_pAttrCtrl2->GetRbgc(SectKey, RbgcData))
		{
			if(RbgcData.nCenterType == 1)//좌하단기준
			{
				for(int i=0 ; i<RbgcData.arMainRebar[0].GetSize() ; i++)
				{
					RbgcData.arMainRebar[0][i].dCenterY += OffsetI_Y;
					RbgcData.arMainRebar[0][i].dCenterZ += OffsetI_Z;
				}
				for(int i=0 ; i<RbgcData.arMainRebar[1].GetSize() ; i++)
				{
					RbgcData.arMainRebar[1][i].dCenterY += OffsetJ_Y;
					RbgcData.arMainRebar[1][i].dCenterZ += OffsetJ_Z;
				}
				for(int i=0 ; i<RbgcData.arMainRebar[2].GetSize() ; i++)
				{
					RbgcData.arMainRebar[2][i].dCenterY += OffsetJ_Y;
					RbgcData.arMainRebar[2][i].dCenterZ += OffsetJ_Z;
				}
			}

			for(int i=0 ; i<RbgcData.arMainRebar[0].GetSize() ; i++) m_MabrDataListI.SetAt(i, RbgcData.arMainRebar[0][i]);
			for(int i=0 ; i<RbgcData.arMainRebar[1].GetSize() ; i++) m_MabrDataListM.SetAt(i, RbgcData.arMainRebar[1][i]);
			for(int i=0 ; i<RbgcData.arMainRebar[2].GetSize() ; i++) m_MabrDataListJ.SetAt(i, RbgcData.arMainRebar[2][i]);

			m_bShearSameIMJ = RbgcData.bIMJBothShear;
			m_ShearRebarDataI = RbgcData.ShearRebar[0];
			m_ShearRebarDataM = RbgcData.ShearRebar[1];
			m_ShearRebarDataJ = RbgcData.ShearRebar[2];
			
			m_bTorsionSameIMJ = RbgcData.bIMJBothTorsion;
			m_TorsionRebarDataI = RbgcData.TorsionRebar[0];
			m_TorsionRebarDataM = RbgcData.TorsionRebar[1];
			m_TorsionRebarDataJ = RbgcData.TorsionRebar[2];

			m_iCenterType = RbgcData.nCenterType;
			priCenterType = RbgcData.nCenterType;
			
			if(m_ctrlTab.GetCurSel() == 0)
			{
				if(iCurSel == 0) //i단
				{
					m_pMainRebar->SetInitDataI(RbgcData.arMainRebar[0]);
					m_pMainRebar->LoadDlgData(RbgcData.bIMJBothMain);
				}
				else if(iCurSel == 1) // M 단
				{
					m_pMainRebar->SetInitDataM(RbgcData.arMainRebar[1]);
					m_pMainRebar->LoadDlgData(RbgcData.bIMJBothMain);
				}
				else //j단
				{
					m_pMainRebar->SetInitDataJ(RbgcData.arMainRebar[2]);
					m_pMainRebar->LoadDlgData(RbgcData.bIMJBothMain);
				}		
			}

			// 이부분이 아마 Data Setting 하는 부분 같다. 
			UpdateData(FALSE);

			m_pMainRebar->SetAsRebarArea();
		}
	}	

	m_pIVP->ZoomAll();
}

void CConcSectionBeamRebarViewDlg::OnDgnSectImportCancelButton() 
{
	m_pIVP->DeleteAllObject();
	m_pIVP->RedrawAll();
	bDrawSection = FALSE;
}

void CConcSectionBeamRebarViewDlg::OnDgnDefineSectButton() 
{
	CMD_DoModal(D_CMD_CCMSECTPAGE4DESIGN);
	//if(AfxGetMainWnd())
	// AfxGetMainWnd()->SendMessage(WM_COMMAND,MAKEWPARAM(ID_STRUCT_PROP_SECTION,0),0L);	
	OnSelchangeDgnRandomSectCombo();
}

BOOL CConcSectionBeamRebarViewDlg::MakeCirclePoints(double Yc, double Zc, double Radius,
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

void CConcSectionBeamRebarViewDlg::AddRebarData(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition)
{
	if(nPosition==0)
		m_MabrDataListI.SetAt(RebarKey, MaReberData);
	else if(nPosition==1)
		m_MabrDataListM.SetAt(RebarKey, MaReberData);
	else
		m_MabrDataListJ.SetAt(RebarKey, MaReberData);

	AddRebarDraw(RebarKey, MaReberData, nPosition);
}

void CConcSectionBeamRebarViewDlg::DeleteRebarData(REBAR_KEY RebarKey, int nPosition)
{
	if(nPosition==0)
	{
		m_MabrDataListI.RemoveKey(RebarKey);
		
		int DKey = 0;
		if(!KeyDataListI.Lookup(RebarKey, DKey)) return;

		m_pIVP->DeleteObject(DKey);
		KeyDataListI.RemoveKey(RebarKey);
	}
	else if(nPosition==1)
	{
		m_MabrDataListM.RemoveKey(RebarKey);
		
		int DKey = 0;
		if(!KeyDataListM.Lookup(RebarKey, DKey)) return;
		
		m_pIVP->DeleteObject(DKey);
		KeyDataListM.RemoveKey(RebarKey);
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

void CConcSectionBeamRebarViewDlg::AddRebarDraw(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition)
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

	strTemp.Format(_T("Number : %d"), MaReberData.nLabel);
	arQueryString.Add(strTemp);
	arQueryString.Add(MaReberData.strRebarName);
	DVObject.GetGeomData().SetPolygon(arVerts);
	DVObject.SetQueryString(arQueryString);
	DVOKey = m_pIVP->AddObject(DVObject);

	if(nPosition==0)
	{
		KeyDataListI.SetAt(RebarKey, DVOKey);
	}
	else if(nPosition==1)
	{
		KeyDataListM.SetAt(RebarKey, DVOKey);
	}
	else
	{
		KeyDataListJ.SetAt(RebarKey, DVOKey);
	}	
	
	m_pIVP->RedrawAll();
}


void CConcSectionBeamRebarViewDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_pIVP->DeleteAllObject();
	m_pIVP->DelAllLabel();	

	CDialogMove::OnCancel();
}

void CConcSectionBeamRebarViewDlg::OnOk() 
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

void CConcSectionBeamRebarViewDlg::InitialRandomSectionData()
{
	m_cmdSect.ResetContent();
	
	CArray<T_SECT_K, T_SECT_K> arKeyList;
	T_SECT_D SectData;
	int ComboIndex = 0;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(arKeyList);

	for(int i=0 ; i<arKeyList.GetSize() ; i++)
	{		
		if(m_pDoc->m_pAttrCtrl->GetSectDesign(arKeyList[i], SectData))
		{
			if(SectData.nStype == D_SECT_TYPE_USER && SectData.SectBefore.Shape == _T("GEN"))
			{
				if(SectData.SectBefore.SectI.aLine.GetSize()<=0)
				{//Plane Type이어야한다
					m_cmdSect.AddString(SectData.SName);
					m_RandomSectionList.SetAt(ComboIndex++, arKeyList[i]);
				}
			}
		}
	}  
}

void CConcSectionBeamRebarViewDlg::OnSelchangeDgnRandomSectCombo() 
{
	OnDgnSectImportCancelButton();

	int iCurSelIMJ;
	int iCurSelMainShear;
	iCurSelMainShear = m_ctrlTab.GetCurSel();
	if(iCurSelMainShear != 0) m_ctrlTab.ShowTab(0); // Add by SHIN (09.11.14) : Shear Rebar입력창에서 단면 변경시 처리되지 않은 항목이 많아 Main Rebar입력창으로 강재적으로 돌림

	if(m_ctrlTab.GetCurSel() == 0)
		iCurSelIMJ = m_pMainRebar->GetCurSel();
	else if(m_ctrlTab.GetCurSel() == 1)
		iCurSelIMJ = m_pShearRebar->GetCurSel();
	else 
		iCurSelIMJ = m_pTorsionRebar->GetCurSel();

	REBAR_KEY Key;
	T_RBGC_MABR Data;

	if(iCurSelMainShear == 0 && iCurSelIMJ == 0)
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
	if(iCurSelMainShear == 0 && iCurSelIMJ == 1)
	{
		BOOL bOldLock = m_pMainRebar->m_pMainRebarM->m_pRebarGrid->LockUpdate(TRUE);
		
		POSITION PosM = m_MabrDataListM.GetStartPosition();
		while(PosM)
		{
			m_MabrDataListM.GetNextAssoc(PosM,Key,Data);	
			m_pMainRebar->RemoveMainRebarDataListM(Key, Data);
		}
		m_pMainRebar->m_pMainRebarM->m_pRebarGrid->LockUpdate(bOldLock);
		if (!bOldLock) m_pMainRebar->m_pMainRebarM->m_pRebarGrid->Redraw();
	}
	if(iCurSelMainShear == 0 && iCurSelIMJ == 2)
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
	m_MabrDataListM.RemoveAll();
	m_MabrDataListJ.RemoveAll();

	m_bMainSameIMJ = FALSE;
	m_bShearSameIMJ = FALSE;
	m_bTorsionSameIMJ = FALSE;
	
	m_ShearRebarDataI.Initialize();
	m_ShearRebarDataM.Initialize();
	m_ShearRebarDataJ.Initialize();

	m_TorsionRebarDataI.Initialize();
	m_TorsionRebarDataM.Initialize();
	m_TorsionRebarDataJ.Initialize();
	
	OnDgnSectImportButton();
}

void CConcSectionBeamRebarViewDlg::DrawSectionRebarTabChanged() 
{
	m_pIVP->DeleteAllObject();
	T_SECT_K SectKey;

	//단면을 그린다
	int RedamSectIndex = m_cmdSect.GetCurSel();
	if(RedamSectIndex < 0) return;
	
	m_RandomSectionList.Lookup(RedamSectIndex, SectKey);
	  	
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
	else if(m_ctrlTab.GetCurSel() == 1)
		iPosi = m_pShearRebar->GetCurSel();
	else 
		iPosi = m_pTorsionRebar->GetCurSel();

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
		else if(iCurSel == 1) // M 단
		{
			m_pMainRebar->SetRebarListDataM(m_MabrDataListM);
		}
		else //j단
		{
			m_pMainRebar->SetRebarListDataJ(m_MabrDataListJ);
		}
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		int nCurSel = m_pShearRebar->GetCurSel();

		if(nCurSel==0)
		{
			m_pShearRebar->m_DataI = m_ShearRebarDataI;
			m_pShearRebar->Data2Dlg();
		}
		else if(nCurSel==1)
		{
			m_pShearRebar->m_DataM = m_ShearRebarDataM;
			m_pShearRebar->Data2Dlg();
		}
		else
		{
			m_pShearRebar->m_DataJ = m_ShearRebarDataJ;
			m_pShearRebar->Data2Dlg();
		}
	}
	else
	{
		int nCurSel = m_pTorsionRebar->GetCurSel();
		
		if(nCurSel==0)
		{
			m_pTorsionRebar->m_DataI = m_TorsionRebarDataI;
			m_pTorsionRebar->Data2Dlg();
		}
		else if(nCurSel==1)
		{
			m_pTorsionRebar->m_DataM = m_TorsionRebarDataM;
			m_pTorsionRebar->Data2Dlg();
		}
		else
		{
			m_pTorsionRebar->m_DataJ = m_TorsionRebarDataJ;
			m_pTorsionRebar->Data2Dlg();
		}
	}

	m_pIVP->RedrawAll();
}

void CConcSectionBeamRebarViewDlg::OnSelchangeDgnTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_ctrlTab.GetCurSel() == 0)
		DrawSectionRebarTabChanged();
}

BOOL CConcSectionBeamRebarViewDlg::SaveData()
{ 
	BOOL bOK = FALSE;

	T_RBGC_D DBInputData;
	DBInputData.Initialize();

	int MainRebarNum = m_MabrDataListI.GetCount();
	DBInputData.arMainRebar[0].SetSize(MainRebarNum);
	MainRebarNum = m_MabrDataListM.GetCount();
	DBInputData.arMainRebar[1].SetSize(MainRebarNum);
	MainRebarNum = m_MabrDataListJ.GetCount();
	DBInputData.arMainRebar[2].SetSize(MainRebarNum);

	REBAR_KEY RebarKey;
	T_RBGC_MABR MaReberData;
	int Index = 0;
	POSITION Pos = m_MabrDataListI.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListI.GetNextAssoc(Pos,RebarKey,MaReberData);
		MaReberData.nLabel = Index + 1;
		DBInputData.arMainRebar[0].SetAt(Index++, MaReberData);
	}

	Index = 0;
	Pos = m_MabrDataListM.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListM.GetNextAssoc(Pos,RebarKey,MaReberData);
		MaReberData.nLabel = Index + 1;
		DBInputData.arMainRebar[1].SetAt(Index++, MaReberData);
	}
	
	Index = 0;
	Pos = m_MabrDataListJ.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListJ.GetNextAssoc(Pos,RebarKey,MaReberData);
		MaReberData.nLabel = Index + 1;
		DBInputData.arMainRebar[2].SetAt(Index++, MaReberData);
	}
	
	if(m_ctrlTab.GetCurSel() == 0)
	{
		m_pMainRebar->SaveDlgData();
		m_bMainSameIMJ = m_pMainRebar->m_bSameIMJ;
	}
	else if(m_ctrlTab.GetCurSel() == 1)
	{
		m_pShearRebar->Dlg2Data();
		m_bShearSameIMJ = m_pShearRebar->m_bSameIMJ;
		m_ShearRebarDataI = m_pShearRebar->m_DataI;
		m_ShearRebarDataM = m_pShearRebar->m_DataM;
		m_ShearRebarDataJ = m_pShearRebar->m_DataJ;
	}
	else
	{
		m_pTorsionRebar->Dlg2Data();
		m_bTorsionSameIMJ = m_pTorsionRebar->m_bSameIMJ;
		m_TorsionRebarDataI = m_pTorsionRebar->m_DataI;
		m_TorsionRebarDataM = m_pTorsionRebar->m_DataM;
		m_TorsionRebarDataJ = m_pTorsionRebar->m_DataJ;
	}

	DBInputData.bIMJBothMain = m_bMainSameIMJ;
	DBInputData.bIMJBothShear = m_bShearSameIMJ;
	DBInputData.bIMJBothTorsion = m_bTorsionSameIMJ;
	DBInputData.ShearRebar[0] = m_ShearRebarDataI;
	DBInputData.ShearRebar[1] = m_ShearRebarDataM;
	DBInputData.ShearRebar[2] = m_ShearRebarDataJ;
	DBInputData.TorsionRebar[0] = m_TorsionRebarDataI;
	DBInputData.TorsionRebar[1] = m_TorsionRebarDataM;
	DBInputData.TorsionRebar[2] = m_TorsionRebarDataJ;

	UpdateData(TRUE);
	
	DBInputData.nCenterType = m_iCenterType;
	
	if(DBInputData.nCenterType == 1)//좌하단기준
	{
		for(int i=0 ; i<DBInputData.arMainRebar[0].GetSize() ; i++)
		{
			DBInputData.arMainRebar[0][i].dCenterY -= OffsetI_Y;
			DBInputData.arMainRebar[0][i].dCenterZ -= OffsetI_Z;
		}

		for(int i=0 ; i<DBInputData.arMainRebar[1].GetSize() ; i++)
		{
			DBInputData.arMainRebar[1][i].dCenterY -= OffsetJ_Y;
			DBInputData.arMainRebar[1][i].dCenterZ -= OffsetJ_Z;
		}
		
		for(int i=0 ; i<DBInputData.arMainRebar[2].GetSize() ; i++)
		{
			DBInputData.arMainRebar[2][i].dCenterY -= OffsetJ_Y;
			DBInputData.arMainRebar[2][i].dCenterZ -= OffsetJ_Z;
		}
	}

	if(m_pDoc->m_pAttrCtrl2->ExistRbgc(NowSectKey))
		bOK = m_pDoc->m_pDataCtrl->ModifyRbgc(NowSectKey, NowSectKey, DBInputData);
	else
		bOK = m_pDoc->m_pDataCtrl->AddRbgc(NowSectKey, DBInputData);

	return bOK;
}

void CConcSectionBeamRebarViewDlg::OnChangedCenterType_Center() 
{
	UpdateData(TRUE);
	if(priCenterType == 0) return;

	ChangedCenterType(TRUE);
	priCenterType = 0;
}

void CConcSectionBeamRebarViewDlg::OnChangedCenterType_LeftBottom() 
{
	UpdateData(TRUE);
	if(priCenterType == 1) return;

	ChangedCenterType(FALSE);
	priCenterType = 1;
}

void CConcSectionBeamRebarViewDlg::ChangedCenterType(BOOL bCenType) 
{
	m_pIVP->DeleteAllObject();

	int iCurSel;
	if(m_ctrlTab.GetCurSel()==0)
		iCurSel = m_pMainRebar->GetCurSel();
	else if(m_ctrlTab.GetCurSel()==1)
		iCurSel = m_pShearRebar->GetCurSel();
	else
		iCurSel = m_pTorsionRebar->GetCurSel();
	//단면을 다시 그린다
	if(ImportSection(iCurSel))
	{		
		bDrawSection = TRUE;
	}

	//그리드에 값을 지워준다
	int iCurSelIMJ;
	int iCurSelMainShear;
	iCurSelMainShear = m_ctrlTab.GetCurSel();
	if(m_ctrlTab.GetCurSel() == 0)
		iCurSelIMJ = m_pMainRebar->GetCurSel();
	else if(m_ctrlTab.GetCurSel() == 1)
		iCurSelIMJ = m_pShearRebar->GetCurSel();
	else
		iCurSelIMJ = m_pTorsionRebar->GetCurSel();

	REBAR_KEY Key;
	T_RBGC_MABR Data;

	if(iCurSelMainShear == 0 && iCurSelIMJ == 0)
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
	if(iCurSelMainShear == 0 && iCurSelIMJ == 1)
	{		
		BOOL bOldLock = m_pMainRebar->m_pMainRebarM->m_pRebarGrid->LockUpdate(TRUE);
		
		POSITION PosM = m_MabrDataListM.GetStartPosition();
		while(PosM)
		{
			m_MabrDataListM.GetNextAssoc(PosM,Key,Data);	
			m_pMainRebar->RemoveMainRebarDataListM(Key, Data);
		}
		m_pMainRebar->m_pMainRebarM->m_pRebarGrid->LockUpdate(bOldLock);
		if (!bOldLock) m_pMainRebar->m_pMainRebarM->m_pRebarGrid->Redraw();
	}
	if(iCurSelMainShear == 0 && iCurSelIMJ == 2)
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
		T_RBGC_D OffsetData;
		OffsetData.Initialize();
		int MainRebarNum = m_MabrDataListI.GetCount();
		OffsetData.arMainRebar[0].SetSize(MainRebarNum);
		MainRebarNum = m_MabrDataListM.GetCount();
		OffsetData.arMainRebar[1].SetSize(MainRebarNum);		
		MainRebarNum = m_MabrDataListJ.GetCount();
		OffsetData.arMainRebar[2].SetSize(MainRebarNum);

		REBAR_KEY RebarKey;
		T_RBGC_MABR MaReberData;
		int Index = 0;
		
		POSITION Pos = m_MabrDataListI.GetStartPosition();
		while(Pos)
		{
			m_MabrDataListI.GetNextAssoc(Pos,RebarKey,MaReberData);
			MaReberData.nLabel = Index + 1;
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

		Index = 0;
		Pos = m_MabrDataListM.GetStartPosition();
		while(Pos)
		{
			m_MabrDataListM.GetNextAssoc(Pos,RebarKey,MaReberData);
			MaReberData.nLabel = Index + 1;
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
			OffsetData.arMainRebar[1].SetAt(Index++, MaReberData);
		}				
		
		Index = 0;
		Pos = m_MabrDataListJ.GetStartPosition();
		while(Pos)
		{
			m_MabrDataListJ.GetNextAssoc(Pos,RebarKey,MaReberData);
			MaReberData.nLabel = Index + 1;
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
		m_MabrDataListM.RemoveAll();
		m_MabrDataListJ.RemoveAll();

		for(int i=0 ; i<OffsetData.arMainRebar[0].GetSize() ; i++)
		{
			m_MabrDataListI.SetAt(i, OffsetData.arMainRebar[0][i]);
		}

		for(int i=0 ; i<OffsetData.arMainRebar[1].GetSize() ; i++)
		{
			m_MabrDataListM.SetAt(i, OffsetData.arMainRebar[1][i]);
		}
		
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
			else if(iCurSel == 1) //M단
			{
				BOOL bOldLock = m_pMainRebar->m_pMainRebarM->m_pRebarGrid->LockUpdate(TRUE);
				
				m_pMainRebar->SetRebarListDataM(m_MabrDataListM);
				
				m_pMainRebar->m_pMainRebarM->m_pRebarGrid->LockUpdate(bOldLock);
				if (!bOldLock) m_pMainRebar->m_pMainRebarM->m_pRebarGrid->Redraw();
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

void CConcSectionBeamRebarViewDlg::OnBtnApply() 
{
	if(bDrawSection)
	{
		BOOL bOK = SaveData();

		if(!bOK)
			AfxMessageBox(_LS(IDS_DGN_CON_SECT_REBAR_ERROR_INPUTDATA), MB_OK);//_T("입력값이 올바르지 않습니다.\n메인화면 하단 Message View를 확인하시기 바랍니다.")
	}
}

int CConcSectionBeamRebarViewDlg::GetMainRebarI_LastNum() 
{
	int result = 0;
	
	REBAR_KEY RebarKey;
	T_RBGC_MABR MaReberData;
	POSITION Pos = m_MabrDataListI.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListI.GetNextAssoc(Pos,RebarKey,MaReberData);
		if(result < MaReberData.nLabel)
			result = MaReberData.nLabel;				
	}

	return result;
}

int CConcSectionBeamRebarViewDlg::GetMainRebarM_LastNum() 
{
	int result = 0;
	
	REBAR_KEY RebarKey;
	T_RBGC_MABR MaReberData;
	POSITION Pos = m_MabrDataListM.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListM.GetNextAssoc(Pos,RebarKey,MaReberData);
		if(result < MaReberData.nLabel)
			result = MaReberData.nLabel;				
	}
	
	return result;
}

int CConcSectionBeamRebarViewDlg::GetMainRebarJ_LastNum() 
{
	int result = 0;
	
	REBAR_KEY RebarKey;
	T_RBGC_MABR MaReberData;
	POSITION Pos = m_MabrDataListJ.GetStartPosition();
	while(Pos)
	{
		m_MabrDataListJ.GetNextAssoc(Pos,RebarKey,MaReberData);
		if(result < MaReberData.nLabel)
			result = MaReberData.nLabel;				
	}

	return result;
}

void CConcSectionBeamRebarViewDlg::ReDrawRebar(REBAR_KEY RebarKey, T_RBGC_MABR MaReberData, int nPosition) 
{
	DeleteRebarData(RebarKey, nPosition);
	AddRebarData(RebarKey, MaReberData, nPosition);
}