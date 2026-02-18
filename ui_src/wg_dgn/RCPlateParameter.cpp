// RCPlateParameter.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCPlateParameter.h"

#include "RCDesignPlateParamDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"
#include "..\wg_base\wg_base_StrParser.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_dbLock\LockCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"

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
// CRCPlateParameter dialog


CRCPlateParameter::CRCPlateParameter(CWnd* pParent /*=NULL*/)
	: CChildDialog(CRCPlateParameter::IDD, pParent)
{  
	//{{AFX_DATA_INIT(CRCPlateParameter)
 
	//}}AFX_DATA_INIT
	m_nOperationMode = 1;
	m_arXYZ[0].RemoveAll(); 

	ASSERT(pParent);
	m_pParent = (CRCDesignPlateParamDlg*)pParent;  

	m_Data.Initialize();  

	m_PlateGrid = new CDgnRCPlateGrid();

	m_pDoc = CDBDoc::GetDocPoint();

	m_RcplData.InitHashTable(101);

	m_strConCode = CDBLib::GetConCodeName();
}

CRCPlateParameter::~CRCPlateParameter()
{
	if(m_PlateGrid != NULL)
	{
		delete m_PlateGrid;
		m_PlateGrid = NULL;
	}

	m_arLcomKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_RcplData.RemoveAll();
}

void CRCPlateParameter::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCPlateParameter)
	DDX_Control(pDX, IDC_DGN_PLATE_LIST_LCOM_STRENGTH, m_ltbxLcomStre);
	DDX_Control(pDX, IDC_DGN_PLATE_LIST_LCOM_SERVICE, m_ltbxLcomServ);
	DDX_Control(pDX, IDC_DGN_PLATE_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_DGN_PLATE_RADIO_RBAR_RATIO_BAEM, m_radioBeam);
	DDX_Control(pDX, IDC_DGN_PLATE_RADIO_RBAR_RATIO_SLAB, m_radioSlab);
	DDX_Control(pDX, IDC_DGN_PLATE_CBO_CRACK_WIDTH, m_comboMinShearCheck);
	DDX_Control(pDX, IDC_DGN_PLATE_CHK_INCLUDE_SBAR, m_chkInclude);
	DDX_Control(pDX, IDC_DGN_PLATE_DTCUNIT, m_dDtunit);
	DDX_Control(pDX, IDC_DGN_PLATE_DBCUNIT, m_dDbunit);
	DDX_Control(pDX, IDC_DGN_PLATE_EDIT_DT, m_Dt);
	DDX_Control(pDX, IDC_DGN_PLATE_EDIT_DB, m_Db);
	DDX_Control(pDX, IDC_DGN_PLATE_EDIT_ELEMENT, m_editSelectElem);
	DDX_Control(pDX, IDC_DGN_PLATE_LOCAL_RADIO, m_radioLocal);
	DDX_Control(pDX, IDC_DGN_PLATE_UCS_RADIO, m_radioUCS);
	DDX_Control(pDX, IDC_DGN_PLATE_UCS_CBO, m_comboUCS);
	DDX_Control(pDX, IDC_DGN_PLATE_ELEMENT_RADIO, m_radioElement);
	DDX_Control(pDX, IDC_DGN_PLATE_AVG_NODAL_RADIO, m_radioAvgNodal);
	DDX_Control(pDX, IDC_DGN_PLATE_PRINT_AVG_NODAL_CHK, m_chkAvgNodal);
	DDX_Control(pDX, IDC_DGN_PLATE_X_DIR_RADIO, m_radioXDir);
	DDX_Control(pDX, IDC_DGN_PLATE_Y_DIR_RADIO, m_radioYDir);
	DDX_Control(pDX, IDC_DGN_PLATE_ETC_EDIT, m_editEtc);
	DDX_Control(pDX, IDC_DGN_PLATE_CTM_GRID, *m_PlateGrid);
	//}}AFX_DATA_MAP	
}


BEGIN_MESSAGE_MAP(CRCPlateParameter, CChildDialog)
	//{{AFX_MSG_MAP(CRCPlateParameter)
	ON_BN_CLICKED(IDC_DGN_PLATE_BTN_ADD, OnAddData)
	ON_BN_CLICKED(IDC_DGN_PLATE_BTN_MODIFY, OnModifyData)
	ON_BN_CLICKED(IDC_DGN_PLATE_BTN_DELETE, OnDeleteData)
	ON_BN_CLICKED(IDC_DGN_PLATE_CALC_BTN, OnDgnPlateCalcBtn)
	ON_BN_CLICKED(IDC_DGN_PLATE_LOCAL_RADIO, OnDgnRadioLocalUCS)
	ON_BN_CLICKED(IDC_DGN_PLATE_UCS_RADIO, OnDgnRadioLocalUCS)
	ON_BN_CLICKED(IDC_DGN_PLATE_ELEMENT_RADIO, OnDgnRadioElemAvg)
	ON_BN_CLICKED(IDC_DGN_PLATE_AVG_NODAL_RADIO, OnDgnRadioElemAvg)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface


/////////////////////////////////////////////////////////////////////////////
// CRCPlateParameter message handlers

BOOL CRCPlateParameter::OnInitDialog() 
{
	CChildDialog::OnInitDialog();
	
	m_PlateGrid->m_RcplData.RemoveAll();
	m_PlateGrid->Initialize(this);
		
	ASSERT(m_pDoc);

	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);	

	InitialControl();
	InitEnableColtrol();
	
	LoadDBtoDlg();
 
	//해석되어 있지 않으면 계산버튼 비활성화
	if(!m_pDoc->IsPostMode())
		GetDlgItem(IDC_DGN_PLATE_CALC_BTN)->EnableWindow(false);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRCPlateParameter::InitialControl() 
{
	// Unit.
	SetInitUnit();
	// Combo.
	InitialCboMinShearCheck();
	// Lcom.
	InitialLcomDataForDgn();

	InitUCSCmb();

	T_RCPL_D data;
	data.Initialize();

	//검토영역 선택
	m_editName.SetWindowText(_T(""));

	m_editEtc.SetWindowText(data.strRemark);

	if(data.iAxisType == 0)
	{
		m_radioLocal.SetCheck(TRUE);
		m_radioUCS.SetCheck(FALSE);
	}
	else
	{
		m_radioLocal.SetCheck(FALSE);
		m_radioUCS.SetCheck(TRUE);
	}		
	T_NUCS_D USCData;
	USCData.Initialize();
	m_pDoc->m_pAttrCtrl->GetNucs(data.ucsKey, USCData);
	int Index = m_comboUCS.FindStringExact(-1, USCData.UcsName);
	if(Index!=-1)	
		m_comboUCS.SetCurSel(Index);
	else
		m_comboUCS.SetCurSel(0);	
	if(data.iAnaType == 0)
	{
		m_radioElement.SetCheck(TRUE);
		m_radioAvgNodal.SetCheck(FALSE);
	}
	else
	{
		m_radioElement.SetCheck(FALSE);
		m_radioAvgNodal.SetCheck(TRUE);
	}
	m_chkAvgNodal.SetCheck(data.bActiveOnly);
	if(data.iDir == 0)
	{
		m_radioXDir.SetCheck(TRUE);
		m_radioYDir.SetCheck(FALSE);
	}
	else
	{
		m_radioXDir.SetCheck(FALSE);
		m_radioYDir.SetCheck(TRUE);
	}
	
	CString strTemp = _T("");
	//Design option
	if(data.iRhoStand == 0)
	{
		m_radioBeam.SetCheck(TRUE);
		m_radioSlab.SetCheck(FALSE);
	}
	else
	{
		m_radioBeam.SetCheck(FALSE);
		m_radioSlab.SetCheck(TRUE);
	}

	if(m_strConCode != _T("KCI-USD07"))//_T("KCI-USD07")일경우 사용하지 않는다
		m_comboMinShearCheck.SetCurSel(data.iServCond);

	m_chkInclude.SetCheck(data.bStirrup);
	m_Dt.SetEditUnit(data.dDt);
	m_Db.SetEditUnit(data.dDb);

	OnDgnRadioLocalUCS();
	OnDgnRadioElemAvg();	
}

void CRCPlateParameter::InitEnableColtrol()
{
	m_strConCode = CDBLib::GetConCodeName();

	if(m_strConCode == _T("KCI-USD07"))
	{
		GetDlgItem(IDC_DGN_PLATE_STATIC7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PLATE_CBO_CRACK_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_PLATE_CHK_INCLUDE_SBAR)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_PLATE_STATIC7)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PLATE_CBO_CRACK_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_PLATE_CHK_INCLUDE_SBAR)->EnableWindow(TRUE);
	}
}

void CRCPlateParameter::InitUCSCmb()
{
	if(!::IsWindow(this->GetSafeHwnd()))return; 

	CArray<CString,CString&> arNUCSNames;
	m_comboUCS.ResetContent();
	m_pDoc->m_pAttrCtrl->GetNucsList(arNUCSNames);
	int arNUCSNamesSize = arNUCSNames.GetSize();
	for(int i=0; i<arNUCSNamesSize; i++)
	{
		m_comboUCS.AddString(arNUCSNames[i]);
	}
	m_comboUCS.SetCurSel(0);  
	
	if(m_comboUCS.GetCount() == 0)
	{
		m_radioUCS.EnableWindow(FALSE);
		m_comboUCS.EnableWindow(FALSE);
		m_radioLocal.SetCheck(TRUE);
		m_radioUCS.SetCheck(FALSE);
	}
	else 
	{
		m_radioUCS.EnableWindow(TRUE);
		m_comboUCS.EnableWindow(TRUE);
	}
}

BOOL CRCPlateParameter::DestroyWindow() 
{
	m_arLcomKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_RcplData.RemoveAll();
	
	return CChildDialog::DestroyWindow();
}

void CRCPlateParameter::InitialData()
{
	m_arLcomKey.RemoveAll();
}

BOOL CRCPlateParameter::Data2Dlg()
{	
	//검토영역 선택
	m_editName.SetWindowText(m_Data.strName);

	m_editEtc.SetWindowText(m_Data.strRemark);

	m_SelectElemString.Empty();
	
	int arElemkeySize = m_Data.arElemK.GetSize();
	if(arElemkeySize>0)
	{
		CString TempString;
		for(int i=0 ; i<arElemkeySize ; i++)
		{
			if(m_SelectElemString.GetLength()==0)
				m_SelectElemString.Format(_T("%d"), m_Data.arElemK[i]);
			else
			{
				TempString.Format(_T(" %d"), m_Data.arElemK[i]);
				m_SelectElemString += TempString;
			}
		}
	}		
	
	if(m_Data.iAxisType == 0 || m_comboUCS.GetCount() == 0)
	{
		m_radioLocal.SetCheck(TRUE);
		m_radioUCS.SetCheck(FALSE);
	}
	else
	{
		m_radioLocal.SetCheck(FALSE);
		m_radioUCS.SetCheck(TRUE);
	}		
	T_NUCS_D USCData;
	USCData.Initialize();
	m_pDoc->m_pAttrCtrl->GetNucs(m_Data.ucsKey, USCData);
	int Index = m_comboUCS.FindStringExact(-1, USCData.UcsName);
	if(Index!=-1)	
		m_comboUCS.SetCurSel(Index);
	else
		m_comboUCS.SetCurSel(0);	
	if(m_Data.iAnaType == 0)
	{
		m_radioElement.SetCheck(TRUE);
		m_radioAvgNodal.SetCheck(FALSE);
	}
	else
	{
		m_radioElement.SetCheck(FALSE);
		m_radioAvgNodal.SetCheck(TRUE);
	}
	m_chkAvgNodal.SetCheck(m_Data.bActiveOnly);
	if(m_Data.iDir == 0)
	{
		m_radioXDir.SetCheck(TRUE);
		m_radioYDir.SetCheck(FALSE);
	}
	else
	{
		m_radioXDir.SetCheck(FALSE);
		m_radioYDir.SetCheck(TRUE);
	}

	//Design option
	if(m_Data.iRhoStand == 0)
	{
		m_radioBeam.SetCheck(TRUE);
		m_radioSlab.SetCheck(FALSE);
	}
	else
	{
		m_radioBeam.SetCheck(FALSE);
		m_radioSlab.SetCheck(TRUE);
	}

	if(m_strConCode != _T("KCI-USD07"))//_T("KCI-USD07")일경우 저장하지 않는다
		m_comboMinShearCheck.SetCurSel(m_Data.iServCond);

	m_chkInclude.SetCheck(m_Data.bStirrup);
	m_Dt.SetEditUnit(m_Data.dDt);
	m_Db.SetEditUnit(m_Data.dDb);

	m_editSelectElem.SelectByStr(m_SelectElemString);

	OnDgnRadioLocalUCS();
	OnDgnRadioElemAvg();
	
	return TRUE;
}

void CRCPlateParameter::SetInitUnit()
{
	m_Dt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Db.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dDtunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dDbunit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}

BOOL CRCPlateParameter::Dlg2Data()
{
	m_Data.Initialize();  
		
	m_pParent->UpdateData(TRUE);

	UpdateData(TRUE);

	if(m_iAddDelMod != 2)//Delete일 경우는 데이터 체크를 하지않는다
		if(!DataCheck()) return FALSE;
	
	m_RcplData.RemoveAll();  
 
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
	
	//검토 영역 선택
	CString strTemp = _T("");
	
	m_editName.GetWindowText(m_Data.strName);
	m_editEtc.GetWindowText(m_Data.strRemark);

	m_pDoc->m_pViewCtrl->GetAllSelectedElem(m_Data.arElemK);
	
	if(m_radioLocal.GetCheck())
		m_Data.iAxisType = 0;
	else if(m_radioUCS.GetCheck())
		m_Data.iAxisType = 1;
	else 
	{
		ASSERT(0);
		m_Data.iAxisType = 0;
	}
	m_comboUCS.GetWindowText(strTemp);
	m_Data.ucsKey = m_pDoc->m_pAttrCtrl->GetNucsKey(strTemp);	
	if(m_radioElement.GetCheck())
		m_Data.iAnaType = 0;
	else if(m_radioAvgNodal.GetCheck())
		m_Data.iAnaType = 1;
	else 
	{
		ASSERT(0);
		m_Data.iAnaType = 0;
	}
	m_Data.bActiveOnly = m_chkAvgNodal.GetCheck();
	if(m_radioXDir.GetCheck())
		m_Data.iDir = 0;
	else if(m_radioYDir.GetCheck())
		m_Data.iDir = 1;
	else 
	{
		ASSERT(0);
		m_Data.iDir = 0;
	}

	//Design Option
	if(m_radioBeam.GetCheck())
		m_Data.iRhoStand = 0;
	else 
		m_Data.iRhoStand = 1;

	if(m_strConCode != _T("KCI-USD07"))//_T("KCI-USD07")일경우 저장하지 않는다
		m_Data.iServCond = m_comboMinShearCheck.GetCurSel();
	m_Data.bStirrup = m_chkInclude.GetCheck();
	m_Data.dDt = m_Dt.GetEditValue();
	m_Data.dDb = m_Db.GetEditValue();
	
	return TRUE;
}

void CRCPlateParameter::OnAddData()
{
	// 여러개 선택할 수도 있다. -> 처리해야함.
	m_iAddDelMod = 0;

	if (!Dlg2Data()) 
	{
		AfxMessageBox(_LS(IDS_DGN_INPUT_DATA_ERROR));
		return;
	}

	m_pDoc->m_pDataCtrl->AddRcpl(m_Data);
}

void CRCPlateParameter::OnModifyData()
{
	m_iAddDelMod = 1;
	
	if (!Dlg2Data()) return;

	m_pDoc->m_pDataCtrl->ModifyRcpl(m_Data.strName, m_Data);   
}

void CRCPlateParameter::OnDeleteData()
{
	m_iAddDelMod = 2;

	if (!Dlg2Data()) return;

	m_pDoc->m_pDataCtrl->DelRcpl(m_Data.strName);
}

void CRCPlateParameter::InitialCboMinShearCheck()
{
	if(m_strConCode==_T("KCI-USD07"))
		return;

	m_comboMinShearCheck.ResetContent();
	CString strAlwCrack;
	if(m_strConCode==_T("KSCE-USD05"))
	{
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_DRY)     , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_MOIST)     , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_RUST)   , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST), m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_WATER_STRUCT)   , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST_WATER_STRUCT)   , m_comboMinShearCheck.AddString(strAlwCrack);
	}
	else if(m_strConCode==_T("KSCE-RAIL-USD04"))
	{
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_DRYING)     , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_GENERAL)     , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_RUST)   , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_VERY_INTENSE_RUST), m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_WATER_STRUCT)   , m_comboMinShearCheck.AddString(strAlwCrack);
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST_WATER_STRUCT)   , m_comboMinShearCheck.AddString(strAlwCrack);
	}

	m_comboMinShearCheck.SetCurSel(0);  
}

void CRCPlateParameter::ChangeLoadCombination()
{
	m_ltbxLcomStre.ResetContent();
	m_ltbxLcomServ.ResetContent();	

	InitialLcomDataForDgn();
}

void CRCPlateParameter::InitialLcomDataForDgn()
{ 
	// Save Total Load Combination Name by Serial Number(No LcomKey).
	
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, m_arLcomKey);

	T_RCPG_D RcpgData;
	
	if(m_pDoc->m_pAttrCtrl->ExistRcpg())//DB에 데이터가 있다면 DB에서 데이터를 가져와서 세팅
	{
		m_pDoc->m_pAttrCtrl->GetRcpg(RcpgData);
		int i;
		m_arStreLcomKey.RemoveAll();
		for(i=0 ; i<RcpgData.StreLcom.GetSize() ; i++)
		{
			m_arStreLcomKey.SetAt(RcpgData.StreLcom[i], RcpgData.StreLcom[i]);
		}

		m_arServLcomKey.RemoveAll();
		for(i=0 ; i<RcpgData.ServLcom.GetSize() ; i++)
		{
			m_arServLcomKey.SetAt(RcpgData.ServLcom[i], RcpgData.ServLcom[i]);
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
			
			if(m_pDoc->m_pAttrCtrl->ExistRcpg())//DB에 저장된것이 있다면 저장된 값으로 체크
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
			
			if(m_pDoc->m_pAttrCtrl->ExistRcpg())//DB에 저장된것이 있다면 저장된 값으로 체크
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
}

void CRCPlateParameter::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CRCPlateParameter::SetUSCCombo()
{
	if(!::IsWindow(this->GetSafeHwnd()))return; 

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();
	
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd  = buffer_ur.nCmd;
		int nKey  = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_NUCS_ADD):
		case(UR_NUCS_DEL):
			 InitUCSCmb();
		   break;
		default:
			break;
		}
	} // end of while
}

void CRCPlateParameter::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_RCPL_K cKey;
	m_PlateGrid->GetCurrentCell(ncRow, ncCol);
	m_PlateGrid->GetColumnNoFor(m_PlateGrid->GetKeyColID(), nkCol);
	cKey = _ttol(m_PlateGrid->GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_RCPL_K    Key;//, KeyBak;
	T_RCPL_D    Data, DataBak;

	BOOL bOldLock = m_PlateGrid->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RCPL_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetRcpl(nKey, Key, Data);
				m_PlateGrid->InsertRecordByKey(Data.strName);
				break;
		case(UR_RCPL_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRcpl(nKey, Key, Data);
				m_PlateGrid->DeleteRecordByKey(Key, Data);
				//m_PlateGrid->DeleteRecordByKey(Data.strName);
				break;
				/*
		case(UR_RCPL_MFD):
				// 여기서는 저장만 하고 UR_RCPL_MFS에서 처리한다.
				pViewBuff->GetRcpl(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_RCPL_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetRcpl(nKey, Key, Data);
				m_PlateGrid->DeleteRecordByKey(Key, Data);
				m_PlateGrid->InsertRecordByKey(KeyBak, DataBak);
				// 플래그 수정
				bMFD = FALSE;
				break;*/
		case(UR_DCON_ADD):// Design Code 변경
		case(UR_DCON_DEL):	
			{
				InitialCboMinShearCheck();
				InitEnableColtrol();
				break;
			}				
		case(UR_NUCS_ADD):
		case(UR_NUCS_DEL):
			SetUSCCombo();
			break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	unsigned int nOldMode = m_PlateGrid->m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_PlateGrid->m_nEditMode = CTBBrowserWnd::noMode;
		if (m_PlateGrid->GetRowCount() < ncRow) ncRow = m_PlateGrid->GetRowCount();
		BOOL bSet = m_PlateGrid->SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = m_PlateGrid->SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = m_PlateGrid->SetLeftValidCell(ncRow, ncCol);
		m_PlateGrid->m_nEditMode = nOldMode;
	}

	m_PlateGrid->GetCurrentCell(ncRow, ncCol);
	m_PlateGrid->GetColumnNoFor(m_PlateGrid->GetKeyColID(), nkCol);
	T_ELEM_K newKey = _ttol(m_PlateGrid->GetValueRowCol(ncRow, nkCol));
	if (newKey != cKey) m_PlateGrid->OnChangeCurrentRecord(ncRow);

	m_PlateGrid->AdjustAppendRowNo();
	m_PlateGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_PlateGrid->Redraw();   
}

void CRCPlateParameter::OnChangeCurrentRcpl(T_RCPL_K key)
{
	m_pDoc->m_pAttrCtrl->GetRcpl(key, m_Data);
	Data2Dlg();
}

// 계수하중조합과 사용하중 조합의 내용을 DB에 업데이트한다.
BOOL CRCPlateParameter::LoadCaseApply() 
{
	Dlg2Data_LoadCase();

	if(m_arStreLcomKey.IsEmpty() && m_arServLcomKey.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_DGN_LOADCASE_SELETE), MB_OK);
		return FALSE;
	}

	T_RCPG_D RcpgD;
	RcpgD.Initialize();
	T_LCOM_K LcomK, GetLcomK;
	POSITION Pos = m_arStreLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arStreLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RcpgD.StreLcom.Add(GetLcomK);
	}
	Pos = m_arServLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arServLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RcpgD.ServLcom.Add(GetLcomK);
	}
	
	if(!m_pDoc->m_pDataCtrl->AddRcpg(RcpgD))
	{
		//
	}

	return TRUE;
}

//LoadCase 체크유무 정보를 저장한다
void CRCPlateParameter::Dlg2Data_LoadCase() 
{
	CString sName;
	DWORD nID;

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();

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
}

//부모 컨트롤에서 Apply버튼을 누를수 있도록
void CRCPlateParameter::Click_Btn_Apply() 
{
	LoadCaseApply();
}

void CRCPlateParameter::OnDgnPlateCalcBtn() 
{
	// Add, Jaeoh. [11/4/2008]
	if(!m_pDoc->AllowCommand(D_OPTN_ID_MODS_SERVICE)) 
	{
		AfxMessageBox(_LS(IDS_DGN_MODS_Expriation_Message), MB_OK);
		return;    
	}

	if(!LoadCaseApply())
		return;	

	if(m_PlateGrid->GetRowCount() <= 1)
	{
		AfxMessageBox(_LS(IDS_DGN_NOT_MEMBER_INPUT_DATA), MB_OK);
		return;
	}

	IUsageCounter::Use(_T("RCPL"));

	// Add by sshan. MNET:2597.2523.('20070201) 로그분석에 Design 코드 추가
	IUsageCounter::Use(ReturnDgnConCode());

	//sshan RCS가 하위 프로젝트 이므로 직접 DLG클레스를 불러 쓸수없다.(Build가되지 않는다)
	//Service로 등록된 것을 불러서 Dlg를 띄운다
	CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
	pHeadOffice->ReqService(_ULS(rcs), EN_RCSSA_CIVIL_RC_PLATE_CALC_DLG, NULL);

}

void CRCPlateParameter::LoadDBtoDlg() 
{
	CArray<T_RCPL_K, T_RCPL_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRcplKeyList(KeyList);

	m_RcplData.RemoveAll();
	T_RCPL_D Data;
	Data.Initialize();
	
	int KeyListSize = KeyList.GetSize();
	for(int i=0 ; i<KeyListSize ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetRcpl(KeyList[i], Data))
			m_RcplData.SetAt(KeyList[i], Data);		
	}
	
	m_iAddDelMod = 0;

	if(KeyList.GetSize()>0)
		OnChangeCurrentRcpl(KeyList[0]);
}

BOOL CRCPlateParameter::DataCheck() 
{
	CString ErrorMessanges;
	
	if(m_Dt.GetEditValue()<=0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DT_ERROR_1);
		else
			ErrorMessanges += _LS(IDS_DGN_DT_ERROR_2);
	}
	if(m_Db.GetEditValue()<=0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DB_ERROR_1);
		else
			ErrorMessanges += _LS(IDS_DGN_DB_ERROR_2);
	}
	
	if(!ErrorMessanges.GetLength()==0)
	{
		AfxMessageBox(ErrorMessanges, MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CRCPlateParameter::EnableCalcBtn(bool Enable) 
{
	GetDlgItem(IDC_DGN_PLATE_CALC_BTN)->EnableWindow(Enable);
}

void CRCPlateParameter::OnDgnRadioLocalUCS() 
{
	if(m_radioLocal.GetCheck())
	{
		m_comboUCS.EnableWindow(FALSE);		
	}
	
	if(m_radioUCS.GetCheck())
	{
		m_comboUCS.EnableWindow(TRUE);		
	}
}

void CRCPlateParameter::OnDgnRadioElemAvg() 
{
	if(m_radioElement.GetCheck())
	{
		m_chkAvgNodal.EnableWindow(FALSE);	
	}
	
	if(m_radioAvgNodal.GetCheck())
	{
		m_chkAvgNodal.EnableWindow(TRUE);		
	}
}

// PARKHJ-20111007 : 번호 할당시 DgnStruct.h 참고!
CString CRCPlateParameter::ReturnDgnConCode()
{
	CString strCodeNumber = _T("C_CON");

	T_DCON_D conData;
	m_pDoc->m_pAttrCtrl->GetDcon(conData);
	CString strCode = conData.DesignCode;

	if(strCode == _T("ACI318-89"))
		strCodeNumber = strCodeNumber+_T("101");
	else if(strCode == _T("AIK-USD94"))
		strCodeNumber = strCodeNumber+_T("102");
	else if(strCode == _T("ACI318-95"))
		strCodeNumber = strCodeNumber+_T("103");
	else if(strCode == _T("KSCE-USD96"))
		strCodeNumber = strCodeNumber+_T("104");
	else if(strCode == _T("KCI-USD99"))
		strCodeNumber = strCodeNumber+_T("105");
	else if(strCode == _T("ACI318-99"))
		strCodeNumber = strCodeNumber+_T("106");
	else if(strCode == _T("AIK-WSD2K"))
		strCodeNumber = strCodeNumber+_T("107");
	else if(strCode == _T("AIJ-WSD99"))
		strCodeNumber = strCodeNumber+_T("108");
	else if(strCode == _T("BS8110-97"))
		strCodeNumber = strCodeNumber+_T("109");
	else if(strCode == _T("Eurocode2"))
		strCodeNumber = strCodeNumber+_T("110");
	else if(strCode == _T("GB50010-02"))
		strCodeNumber = strCodeNumber+_T("111");
	else if(strCode == _T("ACI318-02"))
		strCodeNumber = strCodeNumber+_T("112");
	else if(strCode == _T("CSA-A233-94"))
		strCodeNumber = strCodeNumber+_T("113");
	else if(strCode == _T("IS456-2000"))
		strCodeNumber = strCodeNumber+_T("114");
	else if(strCode == _T("TWN-USD92"))
		strCodeNumber = strCodeNumber+_T("115");
	else if(strCode == _T("TWN-USD100"))
		strCodeNumber = strCodeNumber+_T("125");
    else if (strCode == _T("TWN-USD112"))
        strCodeNumber = strCodeNumber + _T("138");
	else if(strCode == _T("KCI-USD03"))
		strCodeNumber = strCodeNumber+_T("116");
	else if(strCode == _T("KSCE-RC-USD05"))
		strCodeNumber = strCodeNumber+_T("117");
	else if(strCode == _T("KSCE-RC-RAIL-USD04"))
		strCodeNumber = strCodeNumber+_T("118");
	else if(strCode == _T("Eurocode2:04"))
		strCodeNumber = strCodeNumber+_T("119");
	else if(strCode == _T("GB/T50010-10"))
		strCodeNumber = strCodeNumber+_T("126"); // Add by GAY. PMS:4267. ('12.03.12)
	else if(strCode == _T("ACI318-08"))
		strCodeNumber = strCodeNumber+_T("127"); // Add by GAY. PMS:4450. ('12.03.12)
	else if(strCode == _T("ACI318-11"))
		strCodeNumber = strCodeNumber+_T("128"); // Add by GAY. PMS:4450. ('12.03.12)
	else if(strCode == _T("NSR-10"))
		strCodeNumber = strCodeNumber+_T("129"); // by GAY. PMS:5009. ('15.04.03).
	else if(strCode == _T("AASHTO-LC02"))
		strCodeNumber = strCodeNumber+_T("151");
	else if(strCode == _T("AASHTO-LRFD12"))
		strCodeNumber = strCodeNumber+_T("161");
	else if(strCode == _T("AASHTO-LRFD16"))
		strCodeNumber = strCodeNumber+_T("166");
    else if(strCode == _T("AASHTO-LRFD17"))
        strCodeNumber = strCodeNumber+_T("167");
    else if ( strCode == _T("AASHTO-LRFD20") )
        strCodeNumber = strCodeNumber+_T("183");
	else if (strCode == CONCODE_AASHTO_LRFD24)
		strCodeNumber = strCodeNumber + _T("192");
	else if(strCode == _T("JTJ023-85"))
		strCodeNumber = strCodeNumber+_T("152");
	else if(strCode == _T("IRC-21-00"))
		strCodeNumber = strCodeNumber+_T("153");
	else if(strCode == _T("CSA-S6-00"))
		strCodeNumber = strCodeNumber+_T("154");
	else if(strCode == _T("CSA-S6-14"))
		strCodeNumber = strCodeNumber+_T("180");
	else if(strCode == _T("AASHTO-LC96"))
		strCodeNumber = strCodeNumber+_T("155");
	else if(strCode == _T("TWN-BRG-LC90"))
		strCodeNumber = strCodeNumber+_T("156");
	else if(strCode == _T("JARA02"))
		strCodeNumber = strCodeNumber+_T("301");
	else if(strCode == _T("KSCE-USD03"))
		strCodeNumber = strCodeNumber+_T("302");
	else if(strCode == _T("KSCE-USD05"))
		strCodeNumber = strCodeNumber+_T("303");
	else if(strCode == _T("KSCE-RAIL-USD04"))
		strCodeNumber = strCodeNumber+_T("304");
	else if(strCode == _T("JTG-D62-04"))
		strCodeNumber = strCodeNumber+_T("305");
	else
		strCodeNumber = strCodeNumber+_T("Err");

	return strCodeNumber;
}

