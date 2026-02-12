// StldPresAreaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldPresAreaDlg.h"
#include "..\wg_base\wg_base_DlgUtil.h"
//#include "..\wg_cmd\CMPsltDlg.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\WindLoadGeneratorCtrl.h"
#include "..\wg_db\MITC_CommonTool.h"

#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_cmd\wg_cmdAll.h"
#include "..\wg_cmd\wg_cmd_GroupDlgs.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_gr\GDisplaySetting.h"

#include "..\wg_common\TBUtilFunc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum DB_OPTION { OPT_ADD, OPT_REPLACE, OPT_DELETE };
enum D_ELTYPE { D_ELTYPE_FRAME=0, D_ELTYPE_PLANAR };
enum D_PROJECTION { D_PROJECTION_YES=0, D_PROJECTION_NO };
enum D_INPUTTYPE { D_INPUTTYPE_LOADCASE=0, D_INPUTTYPE_LOADTYPE };

/////////////////////////////////////////////////////////////////////////////
// CStldPresAreaDlg dialog
#define CDialog CMenuBarChildDlg
static int SaveWhoAmI  = -100;
static int SaveWhoAmI2 = -100;

CStldPresAreaDlg::CStldPresAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStldPresAreaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStldPresAreaDlg)
	m_nOption = OPT_ADD;
	m_nProjection = D_PROJECTION_NO;
	m_nInputType = D_INPUTTYPE_LOADCASE;
	m_nElementType = D_ELTYPE_FRAME;
	m_nSelection = D_ARPR_SEL_GROUP;
	m_LagrK = 0;
	//}}AFX_DATA_INIT
}

CStldPresAreaDlg::~CStldPresAreaDlg()
{
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Load.nTypeLaplLagr = 0;
	dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
	ExportDisplaySetting::SET(&dspOpt); 
}

void CStldPresAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStldPresAreaDlg)
	DDX_Radio(pDX, IDC_TM_PRES2_LCASE_RDO, m_nInputType);
	DDX_Control(pDX, IDC_TM_CBO_LC, m_CobxLC);
	DDX_Control(pDX, IDC_TM_LDGR_CBO, m_CobxLdgr);
	DDX_Radio(pDX, IDC_TM_OPT_ADD, m_nOption);
	DDX_Control(pDX, IDC_TM_CBO_LTYPE, m_CobxPslt);  
	DDX_Control(pDX, IDC_TM_DIR_COMBO, m_CobxDir);
	DDX_Control(pDX, IDC_TM_INNER_PT_EDT,  m_edtInnerPt);
	DDX_Control(pDX, IDC_TM_INNER_PT_UNT,  m_untInnerPt);
	DDX_Control(pDX, IDC_TM_VECTOR_EDIT, m_EditVector);
	DDX_Radio(pDX, IDC_TM_PROJ_YES, m_nProjection);
	DDX_Control(pDX, IDC_TM_PU_EDT, m_edtP1);
	DDX_Control(pDX, IDC_TM_PU_UNT, m_untP1);
	DDX_Radio(pDX, IDC_TM_TARGET_RDO1, m_nSelection);
	DDX_Control(pDX, IDC_TM_LAGR_NAME, m_cobxLagr); 
	DDX_Radio(pDX, IDC_TM_LAPL_ELEM_TYPE1_RDO, m_nElementType);
	DDX_Control(pDX, IDC_TM_ELEM_DEFINE, m_edElemList);  
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStldPresAreaDlg, CDialog)
	//{{AFX_MSG_MAP(CStldPresAreaDlg)
	ON_BN_CLICKED(IDC_TM_OPT_ADD, OnTmOption)
	ON_CBN_SELCHANGE(IDC_TM_CBO_LTYPE    , OnSelchangeTmLType)
	ON_BN_CLICKED(IDC_TM_BTN_LC, OnTmBtnLc)
	ON_BN_CLICKED(IDC_TM_BTN_LTYPE, OnTmBtnLType)
	ON_BN_CLICKED(IDC_TM_DEFINE_LDGR_BTN, OnTmDefineLdgrBtn)
	ON_BN_CLICKED(IDC_TM_OPT_DELETE, OnTmOption)
	ON_BN_CLICKED(IDC_TM_OPT_REPLACE, OnTmOption)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_BN_CLICKED(IDC_TM_EXECUTE, OnTmExecute)
	ON_BN_CLICKED(IDC_TM_PRES2_LCASE_RDO, OnInputTypeRdo)
	ON_BN_CLICKED(IDC_TM_PRES2_LTYPE_RDO, OnInputTypeRdo)
	ON_BN_CLICKED(IDC_TM_LAGR_BTN,      OnCmdBtnLoadAreaGroup)
	ON_CBN_SELCHANGE(IDC_TM_LAGR_NAME,  OnSelChangeLagr)
	ON_CBN_SELCHANGE(IDC_TM_DIR_COMBO,  OnSelChangeDir)
	ON_BN_CLICKED(IDC_TM_TARGET_RDO1,   OnCmdTargetChanged)  
	ON_BN_CLICKED(IDC_TM_TARGET_RDO2,   OnCmdTargetChanged)  
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStldPresAreaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitCtrls();
	SetDirCobx();
	ProjectionCtrlMan();
	OnCmdTargetChanged();

	m_edtInnerPt.SetAttUcsPos();
	m_edtInnerPt.SetModeToUse(MOUSEEDIT_USE_SET_POS);  
	m_edtInnerPt.SetNextLink(NULL);
	m_edtInnerPt.SetWindowText(_T("0, 0, 0"));
	m_untInnerPt.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_edtP1.SetEditUnit(0.0);
	m_edtP1.SetUnitType(D_UNITSYS_BASE_STRESS);
	m_untP1.SetUnitType(D_UNITSYS_BASE_STRESS);
	
	m_edElemList.Connect(SC_ID_ELEM, &m_edElemList);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CStldPresAreaDlg::InitCtrls()
{
	m_EditVector.SetAttUcsDistance();
	m_EditVector.SetModeToUse(MOUSEEDIT_USE_SET_DIST);
	m_EditVector.SetWindowText(_T("0, 0, 0"));

	m_aCtrlLCaseInputEnable.RemoveAll();
	m_aCtrlLCaseInputEnable.Add(IDC_TM_CBO_LC);
	m_aCtrlLCaseInputEnable.Add(IDC_TM_BTN_LC);
	m_aCtrlLCaseInputEnable.Add(IDC_TM_PU_STC);
	m_aCtrlLCaseInputEnable.Add(IDC_TM_PU_EDT);
	m_aCtrlLCaseInputEnable.Add(IDC_TM_PU_UNT);

	m_aCtrlLTypeInputEnable.RemoveAll();
	m_aCtrlLTypeInputEnable.Add(IDC_TM_CBO_LTYPE);
	m_aCtrlLTypeInputEnable.Add(IDC_TM_BTN_LTYPE);

	OnInputTypeRdo();

	ResetPsltCmb();

	m_wndPicture.SetImage(_T("SVG\\Illustration\\Dialog\\tm_pres7.svg"));
}

void CStldPresAreaDlg::ProjectionCtrlMan()
{
	int nSel = this->m_CobxDir.GetCurSel();
	ASSERT(nSel>= 0);
	int nDir = (int)m_CobxDir.GetItemData(nSel);
	
	CArray<UINT, UINT> Ctrls;
	Ctrls.Add(IDC_TM_PROJ_YES);
	Ctrls.Add(IDC_TM_PROJ_NO );

	if(nDir == D_ARPR_DIR_GX || nDir == D_ARPR_DIR_GY || nDir == D_ARPR_DIR_GZ)
		CDlgUtil::CtrlEnableDisable(this,Ctrls,TRUE);
	else
		CDlgUtil::CtrlEnableDisable(this,Ctrls,FALSE);
	
	if(nDir == D_ARPR_DIR_VECTOR)
		m_EditVector.EnableWindow(TRUE);
	else
		m_EditVector.EnableWindow(FALSE);
}

void CStldPresAreaDlg::OPCtrlMan()
{
	if(m_nOption == OPT_ADD || m_nOption == OPT_REPLACE) //Add
	{
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_FRAME,TRUE,FALSE);
		if(m_nInputType==D_INPUTTYPE_LOADCASE)
		{
				CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_STATIC14,TRUE,FALSE);
		}
		ProjectionCtrlMan();
	}
	else  // Delete
	{
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_FRAME,FALSE,FALSE);
		CDlgUtil::CtrlEnableDisableByRect(this,IDC_TM_STATIC14,FALSE,FALSE);
	}
}

void CStldPresAreaDlg::OnTmOption() 
{
	UpdateData(TRUE);
	OPCtrlMan();
}

void CStldPresAreaDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	SaveWhoAmI = -100;
	SaveWhoAmI2= -100;
}

void CStldPresAreaDlg::OnTmBtnLc() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));
}

void CStldPresAreaDlg::OnTmBtnLType()
{
	T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
	CMD_DoModal(D_CMD_PSLT_ITEM_DLG, &PsltK);
	ResetPsltCmb();

	for(int i=0; i<m_CobxPslt.GetCount(); i++)
	{
		T_PSLT_K PsltK_cur = m_CobxPslt.GetItemData(i);
		if(PsltK==PsltK_cur) m_CobxPslt.SetCurSel(i);
	}
}

void CStldPresAreaDlg::OnTmDefineLdgrBtn() 
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(),CCMLoadGroupDlg::IDD);		
}


/*
m_wndDirVector.GetWindowText(strDirVector);
				if(TRUE == m_wndDirVector.IsKeyPressed())
				{
					if(GetDirectionVector(strDirVector,ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ))
					{
						 pIGM->GetWcsVector(ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ,
																ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ);
					}
					else return FALSE;
				}
				else
				{
					//double dwx,dwy,dwz;
					m_wndDirVector.GetCurMousePosDist(ET.m_UEQ_WX,ET.m_UEQ_WY,ET.m_UEQ_WZ,
																						ET.m_UEQ_UX,ET.m_UEQ_UY,ET.m_UEQ_UZ);
				}
				*/
void CStldPresAreaDlg::GetWCSVectorVal(double& WX, double& WY, double& WZ)
{
	double UX,UY,UZ;
	if(TRUE == m_EditVector.IsKeyPressed())
	{
		CString StrText;
		m_EditVector.GetWindowText(StrText);    
		CStrParser::GetDirectionVector(StrText,UX,UY,UZ);
		I_GENModelBase::GetCurMySelfST()->GetWcsVector(UX,UY,UZ,WX,WY,WZ);
	}
	else
	{
		m_EditVector.GetCurMousePosDist(WX,WY,WZ,UX,UY,UZ);
	}
}

void CStldPresAreaDlg::OnTmClose() 
{
	if(CDBDoc::GetDocPoint())
		CDBDoc::GetDocPoint()->m_pViewCtrl->SetPressureInputMode(FALSE);
	CloseDlg();
}

void CStldPresAreaDlg::OnTmExecute() 
{
	if(!Dlg2Data())
		return;

// 	CArray<T_WDPR_D, T_WDPR_D&> DataSet; 
// 	if(GetWdprAry(DataSet))
// 	{
// 		if(!CheckWdpr(DataSet, TRUE))
// 			return;
// 
// 		if(CDBDoc::GetDocPoint()->m_pDataCtrl->AddWdpr(DataSet))
// 		{
// 			CWindLoadGeneratorCtrl::m_WdprLast[WDPRTYPE_AREA-1] = m_Data;
// 			CWindLoadGeneratorCtrl::m_bSaved[WDPRTYPE_AREA-1] = TRUE;
// 			CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
// 		}
// 	}

// 	if(m_nOption == OPT_ADD || m_nOption == OPT_REPLACE) // Add or Replace
// 	{
// 		// TODO: Add your control notification handler code here
// 		T_PRES_D Data;
// 		Data.Initialize();
// 		
// 		Data.LoadCommandTypeNew = 1; // Command Type 설정 1=Pressure  2=Hydrostatic Pressure
// 
// 		// LoadType 설정  1=Plate/PlaneStress/Wall Load 2=Axisymmetric/PlaneStrain 3=Solid
// 		int nESel = m_CobxEType.GetCurSel();
// 		ASSERT(nESel >= 0);
// 
// 		DWORD_PTR pData = m_CobxEType.GetItemData(nESel);
// 		ASSERT(pData);
// 
// 		Data.LoadType = ((_EType*)pData)->nLoadType;
// 		// SubLoadType 설정 0=Pressure(Plate/PlaneStress,Axisymmetric/PlaneStrain,Solid), 
// 		//1=Concentrated Load(x), 2=단위길이당 Edge Load(Plate/PlaneStress/Wall)
// 		if(((_EType*)pData)->nFaceOrEdge == _FACE)
// 			Data.nSubLoadType = 0;
// 		else
// 		{
// 			if(((_EType*)pData)->nWhoAmI == PLAINSTRAIN_EDGE ||
// 				((_EType*)pData)->nWhoAmI == AXISY_EDGE         )
// 				Data.nSubLoadType = 0;
// 			else
// 				Data.nSubLoadType = 2;
// 		}    
// 
// 		//EdgeFace Index 설정  Select By Node 이면 하중 재하시 Element 별로 적용할것.
// 		Data.nEdgeFace = this->m_CobxFaceEdge.GetItemData(m_CobxFaceEdge.GetCurSel());
// 		//Load Direction 설정 
// 		Data.LoadDirectionNew = this->m_CobxDir.GetItemData(m_CobxDir.GetCurSel());
// 		//Project Option 설정
// 		if(Data.LoadDirectionNew == 4 || Data.LoadDirectionNew == 5 || Data.LoadDirectionNew == 6)
// 		{
// 			if(m_nProjection ==  0)
// 				Data.Projection = 1;
// 			else
// 				Data.Projection = 0;
// 		}
// 		else
// 		{
// 			Data.Projection = 0;
// 		}
// 		this->m_CobxLdgr.GetSelectedLdgr(Data.GroupKey);
// 		
// 		GetWCSVectorVal(Data.Vector[0],Data.Vector[1],Data.Vector[2]);
// 
// 		CArray<UINT, UINT> aSelKey;
// 		CArray<int , int > arFaceOrEdge;
// 		GetElemByType(aSelKey,arFaceOrEdge);
// 		BOOL bSuccess=FALSE;
// 
// 		// Input Type : Load Case
// 		if(m_nInputType==0)
// 		{
// 			if(Data.nSubLoadType == 2)       // Edge Load 
// 			{
// 				if(m_nLoadType == 0) // Uniform 
// 				{
// 					Data.EdgeLoad[0] = m_wndPres1.GetEditValue();
// 				}
// 				else  //Linear
// 				{
// 					Data.EdgeLoad[1] = m_wndPres1.GetEditValue();
// 					Data.EdgeLoad[2] = m_wndPres2.GetEditValue();
// 				}
// 			}
// 			else if(Data.nSubLoadType == 0)  // Face Load 
// 			{
// 				if(m_nLoadType == 0) // Uniform 
// 				{
// 					Data.Force[0] = m_wndPres1.GetEditValue();
// 				}
// 				else  //Linear
// 				{
// 					Data.Force[1] = m_wndPres1.GetEditValue();
// 					Data.Force[2] = m_wndPres2.GetEditValue();
// 					Data.Force[3] = m_wndPres3.GetEditValue();
// 					Data.Force[4] = m_wndPres4.GetEditValue();
// 				}
// 			}
// 
// 			unsigned int nLType;
// 			m_CobxLC.GetSelectedLoad(nLType,Data.LoadCaseKey); // Load Case설정 
// 
// 			if(m_nSelection == 0) // By Node 
// 			{
// 				BOOL bTemp;
// 				if (m_nOption == OPT_ADD || m_nOption == OPT_REPLACE) // Add or Replace
// 				{
// 					int nElem  = aSelKey.GetSize();
// 					int nFE    = arFaceOrEdge.GetSize();
// 					ASSERT( nElem == nFE );
// 
// 					for( int i = 0 ; i < nElem ; i++)
// 					{
// 						Data.nEdgeFace = arFaceOrEdge[i];
// 						bTemp = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey[i],Data,(m_nOption==OPT_ADD)?FALSE:TRUE);
// 						bSuccess &= bTemp;
// 					}
// 				}
// 			}
// 			else   // By Element 
// 			{
// 				bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey,Data,(m_nOption==OPT_ADD)?FALSE:TRUE);
// 			}
// 			if(bSuccess) CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
// 		}
// 		// Input Type : Load Type
// 		else
// 		{
// 			T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
// 			T_PSLT_D PsltD;
// 			if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) return;
// 
// 			if(!CDBDoc::GetDocPoint()->m_pUndoCtrl->StartEditDB(CString(_LS(IDS_DB_DATACTRL_Add_Pressure_Load)), CMDTYPE_REMOVE_ANALYSIS)) { ASSERT(0); return; }
// 			for(int k=0; k<PSLT_NUM; k++)
// 			{
// 				if(PsltD.dLoadP1[k]==0. && PsltD.dLoadP2[k]==0. && PsltD.dLoadP3[k]==0. && PsltD.dLoadP4[k]==0.) continue;
// 
// 				memset(Data.EdgeLoad,0,sizeof(Data.EdgeLoad));
// 				memset(Data.Force,0,sizeof(Data.Force));
// 
// 				if(Data.nSubLoadType == 2)       // Edge Load 
// 				{
// 					if(PsltD.nLoadType[k] == 0) // Uniform 
// 					{
// 						Data.EdgeLoad[0] = PsltD.dLoadP1[k];
// 					}
// 					else  //Linear
// 					{
// 						Data.EdgeLoad[1] = PsltD.dLoadP1[k];
// 						Data.EdgeLoad[2] = PsltD.dLoadP2[k];
// 					}
// 				}
// 				else if(Data.nSubLoadType == 0)  // Face Load 
// 				{
// 					if(PsltD.nLoadType[k] == 0) // Uniform 
// 					{
// 						Data.Force[0] = PsltD.dLoadP1[k];
// 					}
// 					else  //Linear
// 					{
// 						Data.Force[1] = PsltD.dLoadP1[k];
// 						Data.Force[2] = PsltD.dLoadP2[k];
// 						Data.Force[3] = PsltD.dLoadP3[k];
// 						Data.Force[4] = PsltD.dLoadP4[k];
// 					}
// 				}
// 
// 				Data.LoadCaseKey = PsltD.LoadCaseKey[k];  // Load Case설정 
// 
// 				BOOL bTransaction=FALSE;
// 				bSuccess = TRUE;
// 				if(m_nSelection == 0) // By Node 
// 				{
// 					BOOL bTemp;
// 					if (m_nOption == OPT_ADD || m_nOption == OPT_REPLACE)
// 					{
// 						int nElem  = aSelKey.GetSize();
// 						int nFE    = arFaceOrEdge.GetSize();
// 						ASSERT( nElem == nFE );
// 
// 						for( int i = 0 ; i < nElem ; i++)
// 						{
// 							Data.nEdgeFace = arFaceOrEdge[i];
// 							bTemp = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey[i],Data,(m_nOption==OPT_ADD)?FALSE:TRUE,bTransaction);
// 							bSuccess &= bTemp;
// 						}
// 					}
// 				}
// 				else   // By Element 
// 				{
// 					bSuccess = CDBDoc::GetDocPoint()->m_pDataCtrl->AddPres(aSelKey,Data,(m_nOption==OPT_ADD)?FALSE:TRUE,bTransaction);
// 				}
// 			}
// 			
// 			if(bSuccess && CDBDoc::GetDocPoint()->m_pDataCtrl->CheckModifiedModel()) 
// 			{
// 				CDBDoc::GetDocPoint()->m_pUndoCtrl->CloseEditDB(FALSE, TRUE, LT_PRES_CMD);
// 				CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
// 			}
// 			else
// 			{
// 				CDBDoc::GetDocPoint()->m_pUndoCtrl->CancelEditDB();
// 			}
// 		}
// 	}
// 	else // Delete 
// 	{
// 		CArray<UINT, UINT> aSelKey;
// 		CArray<int , int > arFaceOrEdge;
// 		GetElemByType(aSelKey,arFaceOrEdge);
// 		unsigned int LCType, LCKey;
// 		T_LDGR_K LdgrK;
// 		this->m_CobxLC.GetSelectedLoad(LCType,LCKey);
// 		this->m_CobxLdgr.GetSelectedLdgr(LdgrK);
// 		if(CDBDoc::GetDocPoint()->m_pDataCtrl->DelPres(aSelKey, LCKey,LdgrK))
// 			 CDBDoc::GetDocPoint()->m_pViewCtrl->UnselectAll(NULL);
// 	}
}

void CStldPresAreaDlg::OnInputTypeRdo()
{
	UpdateData(TRUE);

	// Load Type방식일 때 Delete 기능은 사용 안 함
	if(m_nInputType==D_INPUTTYPE_LOADTYPE && m_nOption==OPT_DELETE) 
	{
		m_nOption=OPT_ADD;
		UpdateData(FALSE);
		OPCtrlMan();
	}
	GetDlgItem(IDC_TM_OPT_DELETE)->EnableWindow(m_nInputType==D_INPUTTYPE_LOADCASE);

	if(m_nInputType==D_INPUTTYPE_LOADCASE)
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLCaseInputEnable, TRUE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLTypeInputEnable, FALSE);
	}
	else
	{
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLCaseInputEnable, FALSE);
		CDlgUtil::CtrlEnableDisable(this, m_aCtrlLTypeInputEnable, TRUE);
	}
}

void CStldPresAreaDlg::OnSelchangeTmLType()
{
// 	T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
// 	T_PSLT_D PsltD;
// 	if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) { ASSERT(0); return; }
// 	m_CobxEType.SetCurSel(PsltD.nElemType);
// 	OnSelchangeTmElemType();
}

void CStldPresAreaDlg::ResetPsltCmb()
{
	m_CobxPslt.ResetContent();
	CArray<T_PSLT_K,T_PSLT_K> aPsltK;
	T_PSLT_D PsltD;
	CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPsltKeyList(aPsltK);
	for(int i=0; i<aPsltK.GetSize(); i++)
	{
		T_PSLT_K PsltK = aPsltK[i];
		if(!CDBDoc::GetDocPoint()->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) continue;

// 		// 입력된 Load Type이 Uniform 타입으로만 구성된 Load Type만 제공해야 함
// 		for(int j=0; j<PSLT_NUM; j++)
// 		{
// 			if(PsltD.dLoadP1[k]==0. && PsltD.dLoadP2[k]==0. && PsltD.dLoadP3[k]==0. && PsltD.dLoadP4[k]==0.) continue;
// 			if(PsltD.nLoadType)
// 
// 		}

		m_CobxPslt.AddString(PsltD.strName);
		m_CobxPslt.SetItemData(i,PsltK);
	}
	m_CobxPslt.SetCurSel(0);
}

void CStldPresAreaDlg::SetDirCobx()
{
	m_CobxDir.ResetContent();
	CDlgUtil::CobxAddItem(m_CobxDir, _LS(IDS_WG_TREEMENU_Normal), D_ARPR_DIR_NORMAL);
	CDlgUtil::CobxAddItem(m_CobxDir, _LS(IDS_TM_0524Global_X), D_ARPR_DIR_GX);
	CDlgUtil::CobxAddItem(m_CobxDir, _LS(IDS_TM_0524Global_Y), D_ARPR_DIR_GY);
	CDlgUtil::CobxAddItem(m_CobxDir, _LS(IDS_TM_0524Global_Z), D_ARPR_DIR_GZ);
	CDlgUtil::CobxAddItem(m_CobxDir, _LS(IDS_TM_0524Vector), D_ARPR_DIR_VECTOR);
	m_CobxDir.SetCurSel(0);

	OnSelChangeDir();
}

void CStldPresAreaDlg::OnCmdTargetChanged()
{  
	UpdateData(TRUE);
	if(m_nSelection==D_ARPR_SEL_GROUP) //Group
	{
		GetDlgItem(IDC_TM_LAGR_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAGR_BTN)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->EnableWindow(FALSE);
	}
	else if(m_nSelection==D_ARPR_SEL_ELEMENT) //Elements
	{
		GetDlgItem(IDC_TM_LAGR_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAGR_BTN)->EnableWindow(FALSE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE_GRB)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE1_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_LAPL_ELEM_TYPE2_RDO)->EnableWindow(TRUE);
		GetDlgItem(IDC_TM_ELEM_DEFINE)->EnableWindow(TRUE);
	}
	else ASSERT(0);

	OnSelChangeLagr();
}

void CStldPresAreaDlg::OnSelChangeLagr()
{
	UpdateData();
	GDisplaySetting dspOpt;
	if(m_nSelection==D_ARPR_SEL_GROUP)
	{
		T_LAGR_K LagrK;
		m_cobxLagr.GetSelectedLagr(LagrK);
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 2;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		dspOpt.m_Load.m_LoadingAreaKeys.Add((unsigned int)LagrK);
		ExportDisplaySetting::SET(&dspOpt);
	}
	else if(m_nSelection==D_ARPR_SEL_ELEMENT)
	{
		ExportDisplaySetting::GET(&dspOpt);
		dspOpt.m_Load.nTypeLaplLagr = 0;
		dspOpt.m_Load.m_LoadingAreaKeys.RemoveAll();
		ExportDisplaySetting::SET(&dspOpt);
	}
}

void CStldPresAreaDlg::OnSelChangeDir()
{
	int nDir = m_CobxDir.GetItemData(m_CobxDir.GetCurSel());

	// Inner Pt.
	GetDlgItem(IDC_TM_INNER_PT_STC)->EnableWindow(nDir==D_ARPR_DIR_NORMAL);
	GetDlgItem(IDC_TM_INNER_PT_EDT)->EnableWindow(nDir==D_ARPR_DIR_NORMAL);
	GetDlgItem(IDC_TM_INNER_PT_UNT)->EnableWindow(nDir==D_ARPR_DIR_NORMAL);

	// Vector
	GetDlgItem(IDC_TM_VECTOR_TITLE)->EnableWindow	(nDir==D_ARPR_DIR_VECTOR);
	GetDlgItem(IDC_TM_VECTOR_EDIT)->EnableWindow	(nDir==D_ARPR_DIR_VECTOR);
	if(nDir==D_ARPR_DIR_VECTOR)
		m_EditVector.SetFocus();

	// Projection
	BOOL bGlobal = nDir==D_ARPR_DIR_GX || nDir==D_ARPR_DIR_GY || nDir==D_ARPR_DIR_GZ;
	GetDlgItem(IDC_TM_PROJ_TITLE)->EnableWindow(bGlobal);
	GetDlgItem(IDC_TM_PROJ_YES)->EnableWindow(bGlobal);
	GetDlgItem(IDC_TM_PROJ_NO)->EnableWindow(bGlobal);
}

void CStldPresAreaDlg::OnCmdBtnLoadAreaGroup()
{
	CreateOrActivateDlg(CDBDoc::GetDocPoint(), CCMLdarGrupDefDlg::IDD);		
}

BOOL CStldPresAreaDlg::Dlg2Data()
{
	UpdateData();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	m_Data.Initialize();
	m_CobxLdgr.GetSelectedLdgr(m_Data.LdgrK);
	m_Data.nDirection = m_CobxDir.GetItemData(m_CobxDir.GetCurSel());
	if(m_Data.nDirection == D_ARPR_DIR_NORMAL)
	{
		CString strPos;
		m_edtInnerPt.GetWindowText(strPos);
		MyParser Parser;
		if(!Parser.ParsingPositionArg(strPos)) return FALSE;
		if(!Parser.GetPositionData(m_Data.dInnerPnt[0],m_Data.dInnerPnt[1],m_Data.dInnerPnt[2])) return FALSE;
	}
	else if(m_Data.nDirection == D_ARPR_DIR_VECTOR)
	{ 
		GetWCSVectorVal(m_Data.dVector[0],m_Data.dVector[1],m_Data.dVector[2]);
	}
	
	m_Data.bProjection = m_nProjection==D_PROJECTION_YES;
	m_Data.dP1 = m_edtP1.GetEditValue();
	
	m_Data.nSelection = m_nSelection;
	if(m_Data.nSelection==D_ARPR_SEL_GROUP)
	{
		m_cobxLagr.GetSelectedLagr(m_LagrK);
		m_Data.aElemK.RemoveAll();
	}
	else
	{
		m_LagrK = 0;
		
		pDoc->m_pViewCtrl->GetAllSelectedElem(m_Data.aElemK);
	}

	struct T_LC_AND_P1
	{
		T_STLD_K StldK;
		double P1;
		T_LC_AND_P1() : StldK(0), P1(0.0) {}
		T_LC_AND_P1(T_STLD_K lc, double value) { StldK=lc; P1=value; }
	};

	std::vector<T_LC_AND_P1> vLoadAndP1;
	if(m_nInputType==D_INPUTTYPE_LOADCASE)
	{
		T_STLD_K StldK=0;
		unsigned int nLoadCaseType;
		m_CobxLC.GetSelectedLoad(nLoadCaseType, StldK);
		if(nLoadCaseType != D_LOADCASE_STATIC || StldK <= 0)
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_SELECTED_STLD));
			return FALSE;
		}
		vLoadAndP1.push_back(T_LC_AND_P1(StldK, m_edtP1.GetEditValue()));
	}
	else
	{
		// Load Type으로 입력했을 때는 각 Loadcase별로 나눠서 입력
		T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
		T_PSLT_D PsltD;
		if(!pDoc->m_pAttrCtrl2->GetPslt(PsltK,PsltD)) return FALSE;

		for(int i=0; i<PSLT_NUM; i++)
		{
			if(PsltD.LoadCaseKey[i] == 0) break;
			if(PsltD.nLoadType[i] != 0) continue; // Uniform 타입만 사용
			vLoadAndP1.push_back(T_LC_AND_P1(PsltD.LoadCaseKey[i], PsltD.dLoadP1[i]));
		}
	}

	if (vLoadAndP1.empty())
	{
		if (m_nInputType != D_INPUTTYPE_LOADCASE)
		{
			T_PSLT_K PsltK = m_CobxPslt.GetItemData(m_CobxPslt.GetCurSel());
			T_PSLT_D PsltD;
			if (!pDoc->m_pAttrCtrl2->GetPslt(PsltK, PsltD)) return FALSE;
			for (int i = 0; i < PSLT_NUM; i++)
			{
				if (PsltD.LoadCaseKey[i] == 0) break;
				if (PsltD.nLoadType[i] != 0)
				{
					GSaveHistoryFormatNF(_LS(IDS_TM_ERROR_STLDPRES_AREADLG_NOLINE));
					break;
				}
			}
		}
		return FALSE;
	}

	CArray<T_ARPR_D,T_ARPR_D&> aAllArprD; aAllArprD.RemoveAll();
	for(auto itrLP : vLoadAndP1)
	{
		m_Data.StldK = itrLP.StldK;
		m_Data.dP1 = itrLP.P1;

		// 요소 X LAPL 단위로 데이터를 분할하여 DB 입력
		CArray<T_ARPR_D,T_ARPR_D&> aArprD;
		if(!GetPartionedData(m_Data, aArprD)) continue;
		aAllArprD.Append(aArprD);
	}

	if(m_nOption == OPT_ADD || m_nOption == OPT_REPLACE) 
	{
		BOOL bConvert = FALSE;
#ifdef _MGEN_CH
		bConvert = CDlgUtilEx::CtrlIsCheck(this, IDC_CHECK_CONVERT_TO_LOAD);
#endif
		MITC_CommonTool::AddArpr_Cnld(aAllArprD, (m_nOption == OPT_ADD ? FALSE : TRUE), bConvert);
	}
	else
	{
#ifdef _MGEN_CH
		//Del By m_Data.aElem
		if (pDoc->m_pDataCtrl->DelArpr(m_Data))
			pDoc->m_pViewCtrl->UnselectAll(NULL);
#else
		if (pDoc->m_pDataCtrl->DelArpr(aAllArprD))
			pDoc->m_pViewCtrl->UnselectAll(NULL);
#endif
	}

	return TRUE;
}

BOOL CStldPresAreaDlg::GetPartionedData(T_ARPR_D& ArprD_org, CArray<T_ARPR_D,T_ARPR_D&>& aArprD)
{
	int i, nSize;
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	CArray<T_LAPL_K, T_LAPL_K> KeyLapl;

	if(ArprD_org.nSelection==D_ARPR_SEL_GROUP)
	{
		CArray<T_LAGR_K, T_LAGR_K> KeyLagr;
		KeyLagr.SetSize(1);
		KeyLagr[0] = m_LagrK;
		pDoc->m_pAttrCtrl2->GetKeyListLaplByLagr(KeyLagr, KeyLapl);
		nSize = KeyLapl.GetSize();
		aArprD.SetSize(nSize);
		for(i = 0 ; i < nSize; i++)
		{
			ArprD_org.LaplK = KeyLapl[i];
			aArprD[i] = ArprD_org;
		}
		if(nSize <= 0)
		{
			AfxMessageBox(_LS(IDS_TM_WINDP_NO_LDPL));
			return FALSE;
		}
	}
	else
	{
		if(m_nElementType==D_ELTYPE_PLANAR)
		{
			T_ELEM_D ElemD; ElemD.Initialize();
			for(int i=ArprD_org.aElemK.GetSize()-1; i>=0; i--)
			{
				if(!pDoc->m_pAttrCtrl->GetElem(ArprD_org.aElemK[i],ElemD)) { ASSERT(0); return FALSE; }
				if(!pDoc->m_pAttrCtrl->IsPlate(ElemD.eltyp)&&!pDoc->m_pAttrCtrl->IsPlstrs(ElemD.eltyp)) ArprD_org.aElemK.RemoveAt(i);
			}

			if(ArprD_org.aElemK.IsEmpty()) return FALSE;

			T_ARPR_D ArprD;
			aArprD.SetSize(ArprD_org.aElemK.GetSize());
			for(int i=0; i<ArprD_org.aElemK.GetSize(); i++)
			{
				ArprD = ArprD_org;
				ArprD.aElemK.RemoveAll();
				ArprD.aElemK.Add(ArprD_org.aElemK[i]);
				aArprD.SetAt(i, ArprD);
			}
		}
		else
		{
			CWindLoadGeneratorCtrl myCtrl;
			if(!myCtrl.RunFindClosedArea(ArprD_org.aElemK, TRUE))
				return FALSE;

			CArray<UINT, UINT> arSurfKey;
			nSize = myCtrl.GetSurfKeyList(arSurfKey);
			CArray<T_NODE_K, T_NODE_K> aNodeK_dummy;

			aArprD.SetSize(nSize);
			for(i = 0 ; i < nSize; i++)
			{
				aArprD[i] = ArprD_org;
				if(!myCtrl.GetSurfInfo(arSurfKey[i], aNodeK_dummy, aArprD[i].aElemK))
				{
					ASSERT(0);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}