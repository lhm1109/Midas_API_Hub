// RCBeamParameter.cpp : implementation file

#include "stdafx.h"
#include "wg_dgn.h"
#include "RCBeamParameter.h"

#include "RCDesignParamDlg.h"

#include "..\wg_base\wg_base_CompFunc.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_dbLock\LockCtrl.h"

#include "DgnConMatDlg.h"
#include "..\wg_rcs\Rcs_ServiceDef.h"
#include "..\wg_base\ServiceProvider.h"

#include "DgnConMatDlg.h"
#include "DgnCodeCtrl.h"

// Coded by sshan('20061108)
#include "..\wg_db\IUsageCounter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRCBeamParameter dialog


CRCBeamParameter::CRCBeamParameter(CWnd* pParent /*=NULL*/)
	: CChildDialog(CRCBeamParameter::IDD, pParent)
{  
	//{{AFX_DATA_INIT(CRCBeamParameter)
	m_iRbarRatio = 0;   
	m_bIncludeSbar = TRUE; //IDC_CHK_INCLUDE_SBAR
	m_bIEnd = TRUE;
	m_bJEnd = FALSE;
	m_bMid = FALSE;
	m_strRemark = _T("");
	m_SelectElemString = _T("");
	//}}AFX_DATA_INIT

	ASSERT(pParent);
	m_pParent = (CRCDesignParamDlg*)pParent;  

	m_Data.Initialize();  

	m_BeamGrid = new CDgnRCBeamGrid();

	m_pDoc = CDBDoc::GetDocPoint();

	m_RcbeData.InitHashTable(100);

	m_strConCode = CDBLib::GetConCodeName();
}

CRCBeamParameter::~CRCBeamParameter()
{
	if(m_BeamGrid != NULL)
	{
		delete m_BeamGrid;
		m_BeamGrid = NULL;
	}

	m_arElemK.RemoveAll();
	m_arLcomKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_pBadKeyList.RemoveAll();

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_RcbeData.RemoveAll();
}

void CRCBeamParameter::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRCBeamParameter)
	DDX_Radio(pDX, IDC_DGN_BEAM_RADIO_RBAR_RATIO_BAEM, m_iRbarRatio);
	DDX_Control(pDX, IDC_DGN_BEAM_CBO_CRACK_WIDTH, m_cboCrackWidth);  
	DDX_Check(pDX, IDC_DGN_BEAM_CHK_INCLUDE_SBAR, m_bIncludeSbar);
	DDX_Control(pDX, IDC_DGN_BEAM_LIST_LCOM_STRENGTH, m_ltbxLcomStre);
	DDX_Control(pDX, IDC_DGN_BEAM_LIST_LCOM_SERVICE, m_ltbxLcomServ);
	DDX_Check(pDX, IDC_DGN_BEAM_CHECK_IEND, m_bIEnd);
	DDX_Check(pDX, IDC_DGN_BEAM_CHECK_JEND, m_bJEnd);
	DDX_Check(pDX, IDC_DGN_BEAM_CHECK_MID, m_bMid);
	DDX_Text(pDX, IDC_DGN_BEAM_EDIT_POSITION, m_strRemark);
	DDX_Control(pDX, IDC_DGN_BEAM_DTCUNIT, m_dDtunit);
	DDX_Control(pDX, IDC_DGN_BEAM_DBCUNIT, m_dDbunit);
	DDX_Control(pDX, IDC_DGN_BEAM_EDIT_DT, m_Dt);
	DDX_Control(pDX, IDC_DGN_BEAM_EDIT_DB, m_Db);
	DDX_Text(pDX, IDC_DGN_BEAM_EDIT_ELEMENT, m_SelectElemString);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DGN_BEAM_EDIT_ELEMENT, m_editSelectElem);
	DDX_Control(pDX, IDC_DGN_BEAM_CTM_BEAM_GRID, *m_BeamGrid);
}


BEGIN_MESSAGE_MAP(CRCBeamParameter, CChildDialog)
	//{{AFX_MSG_MAP(CRCBeamParameter)
	ON_CBN_SELCHANGE(IDC_DGN_BEAM_CBO_CRACK_WIDTH, OnSelChangeCrackWidth)
	ON_BN_CLICKED(IDC_DGN_BEAM_BTN_ADD, OnAddData)
	ON_BN_CLICKED(IDC_DGN_BEAM_BTN_MODIFY, OnModifyData)
	ON_BN_CLICKED(IDC_DGN_BEAM_BTN_DELETE, OnDeleteData)
	ON_BN_CLICKED(IDC_DGN_BEAM_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_DGN_BEAM_CALC_BTN, OnDgnBeamCalcBtn)
	ON_BN_CLICKED(IDC_DGN_BEAM_MODIFY_CONC_MATE_BTN, OnDgnBeamModifyConcreteMaterialBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Interface


/////////////////////////////////////////////////////////////////////////////
// CRCBeamParameter message handlers

BOOL CRCBeamParameter::OnInitDialog() 
{
	CChildDialog::OnInitDialog();

	// TODO: Add extra initialization here

	//m_BeamGrid = new CDgnRCBeamGrid(m_pParent);
	//m_BeamGrid = new CDgnRCBeamGrid();

	m_BeamGrid->m_RcbeData.RemoveAll();
	m_BeamGrid->Initialize(this);
		
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
//  Data2Dlg();
 
	//해석되어 있지 않으면 계산버튼 비활성화
	if(!m_pDoc->IsPostMode())
		GetDlgItem(IDC_DGN_BEAM_CALC_BTN)->EnableWindow(false);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CRCBeamParameter::DestroyWindow() 
{
	m_arElemK.RemoveAll();
	m_arLcomKey.RemoveAll();
	m_StreLcom.RemoveAll();
	m_ServLcom.RemoveAll();
	m_pBadKeyList.RemoveAll();

	m_arStreLcomKey.RemoveAll();
	m_arServLcomKey.RemoveAll();
	m_RcbeData.RemoveAll();

	return CChildDialog::DestroyWindow();
}

void CRCBeamParameter::InitialData()
{
	m_arLcomKey.RemoveAll();
}

void CRCBeamParameter::InitEnableColtrol()
{
	m_strConCode = CDBLib::GetConCodeName();

	if(m_strConCode == _T("KCI-USD07") || m_strConCode == _T("KCI-USD12"))
	{
		GetDlgItem(IDC_DGN_BEAM_STATIC7)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BEAM_CBO_CRACK_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_DGN_BEAM_CHK_INCLUDE_SBAR)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DGN_BEAM_STATIC7)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BEAM_CBO_CRACK_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_DGN_BEAM_CHK_INCLUDE_SBAR)->EnableWindow(TRUE);
	}
}

BOOL CRCBeamParameter::Data2Dlg()
{	
	CArray<long, long> caKey;
	m_SelectElemString.Empty();

	m_BeamGrid->GetSelectedKeys(caKey);
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
		if(m_BeamGrid->GetCurrentRcbe(aKey))
			m_SelectElemString.Format(_T("%d"), aKey);
	}	

	m_iRbarRatio = m_Data.iRhoStand;
	if(m_strConCode==_T("KSCE-USD05"))
	{
		m_cboCrackWidth.SetCurSel(m_Data.iServCond);
	}
	else if(m_strConCode==_T("KSCE-RAIL-USD04"))
	{
		m_cboCrackWidth.SetCurSel(m_Data.iServCond);
	}
	else if(m_strConCode==_T("KCI-USD07") || m_strConCode==_T("KCI-USD12"))
	{
		//m_cboCrackWidth Not Used...
	}
	else ASSERT(0);
	m_bIncludeSbar = m_Data.bStirrup;
	m_Dt.SetEditUnit(m_Data.dDt);
	m_Db.SetEditUnit(m_Data.dDb);
	m_strRemark = m_Data.strRemark;

	m_bIEnd = m_Data.bPosi[0];
	m_bJEnd = m_Data.bPosi[2];
	m_bMid	= m_Data.bPosi[1];
	
	UpdateData(FALSE);

	m_editSelectElem.SelectByStr(m_SelectElemString);

	return TRUE;
}

void CRCBeamParameter::SetInitUnit()
{
	m_Dt.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_Db.SetUnitType(D_UNITSYS_BASE_LENGTH);

	m_dDtunit.SetUnitType(D_UNITSYS_BASE_LENGTH);
	m_dDbunit.SetUnitType(D_UNITSYS_BASE_LENGTH);	
}

BOOL CRCBeamParameter::Dlg2Data()
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
	m_RcbeData.RemoveAll();  
	
	// Check Position.
	m_bCheckPosi[0] = m_bIEnd;
	m_bCheckPosi[2] = m_bJEnd;
	m_bCheckPosi[1] = m_bMid;

	
	int i;
	int nSel = 0;
	if(m_strConCode != _T("KCI-USD07") && m_strConCode != _T("KCI-USD12"))//_T("KCI-USD07")일경우 m_cboCrackWidth저장하지 않음
	{
		nSel = m_cboCrackWidth.GetCurSel();
		if(nSel < 0) return FALSE;
	}
	//m_Data.iServCond = nSel+1;

	// Strength LoadCombination.
	
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
		
	m_Data.iServCond = nSel;
	m_Data.strRemark = m_strRemark;
	m_Data.iRhoStand = m_iRbarRatio;
	m_Data.bStirrup = m_bIncludeSbar;	
	m_Data.dDt = m_Dt.GetEditValue();
	m_Data.dDb = m_Db.GetEditValue();

	for(i=0; i<m_arElemK.GetSize(); i++)
	{
		m_Data.bPosi[0] = m_bIEnd;
		m_Data.bPosi[2] = m_bJEnd;
		m_Data.bPosi[1] = m_bMid;
				
		m_RcbeData.SetAt(m_arElemK[i], m_Data);
	}
	
	return TRUE;
}

BOOL CRCBeamParameter::GetMaterialData(T_ELEM_K ElemK, double& dfck, double& dfy, double& dEratio)
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

void CRCBeamParameter::OnSelChangeCrackWidth()
{
}



void CRCBeamParameter::OnAddData()
{
	// 여러개 선택할 수도 있다. -> 처리해야함.
	addDelMod = 0;

	if (!Dlg2Data()) 
	{
		AfxMessageBox(_LS(IDS_DGN_INPUT_DATA_ERROR));
		return;
	}

	m_pBadKeyList.RemoveAll();
	UpdateDgnData();
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

	T_ELEM_K ElemK;
	T_RCBE_D RcbeD;
	POSITION Pos = m_RcbeData.GetStartPosition();
	while(Pos)
	{
		m_RcbeData.GetNextAssoc(Pos,ElemK,RcbeD);        
	}
	m_BeamGrid->SetCurrentRcbe(ElemK, RcbeD);
}

void CRCBeamParameter::OnModifyData()
{
	addDelMod = 1;

	T_ELEM_K ElemK;
	T_RCBE_D RcbeD;

	CArray<T_RCBE_K, T_RCBE_K> arKey;
	CArray<T_RCBE_D, T_RCBE_D&> arData;
	arKey.RemoveAll();
	arData.RemoveAll();
	
	if (!Dlg2Data()) return;

	POSITION Pos = m_RcbeData.GetStartPosition();
	while(Pos)
	{
		m_RcbeData.GetNextAssoc(Pos,ElemK,RcbeD);
		arKey.Add(ElemK);
		arData.Add(RcbeD);	      
	}  
	m_pDoc->m_pDataCtrl->ModifyRcbe(arKey, arKey, arData);
}

void CRCBeamParameter::OnDeleteData()
{
	addDelMod = 2;

	if (!Dlg2Data()) return;

	T_ELEM_K ElemK;
	T_RCBE_D RcbeD;
	CArray<T_RCBE_K, T_RCBE_K> arKey;  
	arKey.RemoveAll();
	
	POSITION Pos = m_RcbeData.GetStartPosition();
	while(Pos)
	{
		m_RcbeData.GetNextAssoc(Pos,ElemK,RcbeD);   
		arKey.Add(ElemK);      
	} 
	m_pDoc->m_pDataCtrl->DelRcbe(arKey);
}

void CRCBeamParameter::InitialCboCrackWidth()
{
	if(m_strConCode==_T("KCI-USD07") || m_strConCode==_T("KCI-USD12"))
		return;

	m_cboCrackWidth.ResetContent();
	CString strAlwCrack;
	if(m_strConCode==_T("KSCE-USD05"))
	{
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_DRY),												m_cboCrackWidth.AddString(strAlwCrack);//_T("건조환경(0.006tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_MOIST),											m_cboCrackWidth.AddString(strAlwCrack);//_T("습윤환경(0.005tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_RUST),											m_cboCrackWidth.AddString(strAlwCrack);//_T("부식성환경(0.004tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST),							m_cboCrackWidth.AddString(strAlwCrack);//_T("고부식성환경(0.0035tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_WATER_STRUCT),							m_cboCrackWidth.AddString(strAlwCrack);//_T("수밀성구조물(0.2mm)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST_WATER_STRUCT), m_cboCrackWidth.AddString(strAlwCrack);//_T("고부식성환경/수밀성구조물(0.13mm)")// Add/Modify by sshan. MNET:XXXX.('20070514)
	}
	else if(m_strConCode==_T("KSCE-RAIL-USD04"))
	{
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_DRYING),										m_cboCrackWidth.AddString(strAlwCrack);//_T("건조한환경(0.006tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_GENERAL),										m_cboCrackWidth.AddString(strAlwCrack);//_T("일반환경(0.005tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_RUST),											m_cboCrackWidth.AddString(strAlwCrack);//_T("부식성환경(0.004tc)")
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_VERY_INTENSE_RUST),					m_cboCrackWidth.AddString(strAlwCrack);//_T("극심한부식성환경(0.0035tc)
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_WATER_STRUCT),							m_cboCrackWidth.AddString(strAlwCrack);//")수밀성구조물(0.2mm)_T("
		strAlwCrack = _LS(IDS_DGN_CRACK_WIDTH_INTENSE_RUST_WATER_STRUCT), m_cboCrackWidth.AddString(strAlwCrack);//")고부식성환경/수밀성구조물(0.13mm)_T("
	}

	m_cboCrackWidth.SetCurSel(0);  
}

void CRCBeamParameter::ChangeLoadCombination()
{
	m_ltbxLcomStre.ResetContent();
	m_ltbxLcomServ.ResetContent();

	InitialLcomDataForDgn();
}

void CRCBeamParameter::InitialLcomDataForDgn()
{ 
	// Save Total Load Combination Name by Serial Number(No LcomKey).
		
	m_pDoc->m_pAttrCtrl->GetLcomKeyList(D_LCOMTYPE_CONCRETE, m_arLcomKey);

	T_RCBG_D RcbgData;
	if(m_pDoc->m_pAttrCtrl->ExistRcbg())//DB에 데이터가 있다면 DB에서 데이터를 가져와서 세팅
	{
		m_pDoc->m_pAttrCtrl->GetRcbg(RcbgData);
		int i;
		m_arStreLcomKey.RemoveAll();
		for(i=0 ; i<RcbgData.StreLcom.GetSize() ; i++)
		{
			m_arStreLcomKey.SetAt(RcbgData.StreLcom[i], RcbgData.StreLcom[i]);
		}

		m_arServLcomKey.RemoveAll();
		for(i=0 ; i<RcbgData.ServLcom.GetSize() ; i++)
		{
			m_arServLcomKey.SetAt(RcbgData.ServLcom[i], RcbgData.ServLcom[i]);
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
		if(LcomData.LoadCombType != 0) continue;
		
		CString strLcomName = LcomData.LoadCombName;
		
		nID = i; //i+1;
		//m_aItem.Add(STBSortItem(nID, TRUE));

		bool bExist = false; //해당 LoadCase가 DB에 존재하면 Check True
		if(LcomData.nActive==1)
		{
			nIndex = m_ltbxLcomStre.AddString(strLcomName);
			m_ltbxLcomStre.SetItemData(nIndex, DWORD(nID));
			if(m_pDoc->m_pAttrCtrl->ExistRcbg())//DB에 저장된것이 있다면 저장된 값으로 체크
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
			if(m_pDoc->m_pAttrCtrl->ExistRcbg())//DB에 저장된것이 있다면 저장된 값으로 체크
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

void CRCBeamParameter::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
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

void CRCBeamParameter::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_ELEM_K cKey;
	m_BeamGrid->GetCurrentCell(ncRow, ncCol);
	m_BeamGrid->GetColumnNoFor(m_BeamGrid->GetKeyColID(), nkCol);
	cKey = _ttol(m_BeamGrid->GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_ELEM_K    Key;//, KeyBak;
	T_RCBE_D    Data, DataBak;

	BOOL bOldLock = m_BeamGrid->LockUpdate(TRUE);

	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_RCBE_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetRcbe(nKey, Key, Data);
				m_BeamGrid->InsertRecordByKey(Key, Data);
				break;
		case(UR_RCBE_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetRcbe(nKey, Key, Data);
				m_BeamGrid->DeleteRecordByKey(Key, Data);
				break;
				/*
		case(UR_RCBE_MFD):
				// 여기서는 저장만 하고 UR_RCBE_MFS에서 처리한다.
				pViewBuff->GetRcbe(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_RCBE_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetRcbe(nKey, Key, Data);
				m_BeamGrid->DeleteRecordByKey(Key, Data);
				m_BeamGrid->InsertRecordByKey(KeyBak, DataBak);
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
	unsigned int nOldMode = m_BeamGrid->m_nEditMode;
	if (nOldMode != CTBBrowserWnd::addnew)
	{ // addnew 상태에서 복구하면 추가되는 Row가 삭제(CancelEdit에서)된다.
		m_BeamGrid->m_nEditMode = CTBBrowserWnd::noMode;
		if (m_BeamGrid->GetRowCount() < ncRow) ncRow = m_BeamGrid->GetRowCount();
		BOOL bSet = m_BeamGrid->SetCurrentCell(ncRow, ncCol);
		if (!bSet) bSet = m_BeamGrid->SetRightValidCell(ncRow, ncCol);
		if (!bSet) bSet = m_BeamGrid->SetLeftValidCell(ncRow, ncCol);
		m_BeamGrid->m_nEditMode = nOldMode;
	}

	m_BeamGrid->GetCurrentCell(ncRow, ncCol);
	m_BeamGrid->GetColumnNoFor(m_BeamGrid->GetKeyColID(), nkCol);
	T_ELEM_K newKey = _ttol(m_BeamGrid->GetValueRowCol(ncRow, nkCol));
	if (newKey != cKey) m_BeamGrid->OnChangeCurrentRecord(ncRow);

	m_BeamGrid->AdjustAppendRowNo();
	m_BeamGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_BeamGrid->Redraw();
	
}

void CRCBeamParameter::UpdateDgnData()
{
	T_ELEM_K    Key;
	T_RCBE_D    Data;

	CArray<T_RCBE_K, T_RCBE_K> arKey;
	CArray<T_RCBE_D, T_RCBE_D&> arData;
	arKey.RemoveAll();
	arData.RemoveAll();

	POSITION Pos = m_RcbeData.GetStartPosition();

	while(Pos)
	{
		m_RcbeData.GetNextAssoc(Pos,Key,Data);
		arKey.Add(Key);
		arData.Add(Data);	
	}
	m_pDoc->m_pDataCtrl->AddRcbe(arKey,arData);
	
/*
	while(Pos)
	{
		if(addDelMod == 0)
		{
			m_RcbeData.GetNextAssoc(Pos,Key,Data);
			if(m_pDoc->m_pDataCtrl->AddRcbe(Key,Data))
			{
				
			}
			else
			{
				m_pBadKeyList.Add(Key);
			}
		}
		else if(addDelMod == 1)
		{
			m_RcbeData.GetNextAssoc(Pos,Key,Data);
			T_RCBE_D    tData;
			
		}
		else if(addDelMod == 2)
		{
			m_RcbeData.GetNextAssoc(Pos,Key,Data);	
			
		}
	}*/


	
}

void CRCBeamParameter::OnChangeCurrentRcbe(T_ELEM_K key)
{
	m_pDoc->m_pAttrCtrl->GetRcbe(key, m_Data);
	Data2Dlg();
}

// 계수하중조합과 사용하중 조합의 내용을 DB에 업데이트한다.
void CRCBeamParameter::OnBtnApply() 
{
	Dlg2Data_LoadCase();

	if(m_arStreLcomKey.IsEmpty() && m_arServLcomKey.IsEmpty())
	{
		AfxMessageBox(_LS(IDS_DGN_LOADCASE_SELETE), MB_OK);
		m_bOnBtnApplyResult = FALSE;
		return;
	}

	T_RCBG_D RcbgD;
	RcbgD.Initialize();
	T_LCOM_K LcomK, GetLcomK;
	POSITION Pos = m_arStreLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arStreLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RcbgD.StreLcom.Add(GetLcomK);
	}
	Pos = m_arServLcomKey.GetStartPosition();
	while(Pos)
	{
		m_arServLcomKey.GetNextAssoc(Pos,LcomK,GetLcomK);
		RcbgD.ServLcom.Add(GetLcomK);
	}
	
	if(!m_pDoc->m_pDataCtrl->AddRcbg(RcbgD))
	{
		//
	}

	m_bOnBtnApplyResult = TRUE;
	return;
}

//LoadCase 체크유무 정보를 저장한다
void CRCBeamParameter::Dlg2Data_LoadCase() 
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
void CRCBeamParameter::Click_Btn_Apply() 
{
	OnBtnApply();
}

void CRCBeamParameter::OnDgnBeamCalcBtn() 
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


	if(m_BeamGrid->GetRowCount() <= 1)
	{
		AfxMessageBox(_LS(IDS_DGN_NOT_MEMBER_INPUT_DATA), MB_OK);
		return;
	}

	IUsageCounter::Use(_T("RCDP"));

	// Add by sshan. MNET:2597.2523.('20070201) 로그분석에 Design 코드 추가
	IUsageCounter::Use(ReturnDgnConCode());

	//sshan RCS가 하위 프로젝트 이므로 직접 DLG클레스를 불러 쓸수없다.(Build가되지 않는다)
	//Service로 등록된 것을 불러서 Dlg를 띄운다
	CServiceHeadOffice* pHeadOffice = CServiceHeadOffice::Instance();
	pHeadOffice->ReqService(_ULS(rcs), EN_RCSSA_CIVIL_RC_BEAM_CALC_DLG, NULL);

}

void CRCBeamParameter::OnDgnBeamModifyConcreteMaterialBtn() 
{
	CDgnConMatDlg dlg;
	dlg.DoModal();
}

void CRCBeamParameter::LoadDBtoDlg() 
{
	CArray<T_RCBE_K, T_RCBE_K> KeyList;
	m_pDoc->m_pAttrCtrl->GetRcbeKeyList(KeyList);

	m_RcbeData.RemoveAll();
	T_RCBE_D Data;
	
	int KeyListSize = KeyList.GetSize();
	for(int i=0 ; i<KeyListSize ; i++)
	{
		if(m_pDoc->m_pAttrCtrl->GetRcbe(KeyList[i], Data))
			m_RcbeData.SetAt(KeyList[i], Data);
	}
	
	addDelMod = 0;
	
	if(KeyList.GetSize()>0)
		OnChangeCurrentRcbe(KeyList[0]);
}

BOOL CRCBeamParameter::DataCheck() 
{
	CString ErrorMessanges;
	
	if(!m_bIEnd && !m_bJEnd && !m_bMid)
		ErrorMessanges = _LS(IDS_DGN_MUST_ONE_OVER_PART_NUMBER);//")Part Number를 한가지 이상 선택하셔야 합니다._T("
		
	if(m_Dt.GetEditValue()<=0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DT_ERROR_1);//")Dt값은 0보다 큰값이어야 합니다._T("
		else
			ErrorMessanges += _LS(IDS_DGN_DT_ERROR_2);//")\nDt값은 0보다 큰값이어야 합니다._T("
	}
	if(m_Db.GetEditValue()<=0) 
	{
		if(ErrorMessanges.GetLength()==0)
			ErrorMessanges = _LS(IDS_DGN_DB_ERROR_1);//")Db값은 0보다 큰값이어야 합니다._T("
		else
			ErrorMessanges += _LS(IDS_DGN_DB_ERROR_2);//")\nDb값은 0보다 큰값이어야 합니다._T("
	}
	
	if(!ErrorMessanges.GetLength()==0)
	{
		AfxMessageBox(ErrorMessanges, MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CRCBeamParameter::EnableCalcBtn(bool Enable) 
{
	GetDlgItem(IDC_DGN_BEAM_CALC_BTN)->EnableWindow(Enable);
}


// PARKHJ-20111007 : 번호 할당시 DgnStruct.h 참고!
CString CRCBeamParameter::ReturnDgnConCode()
{
	CString strCodeNumber = _T("C_CON");

	T_DCON_D DconD;
	m_pDoc->m_pAttrCtrl->GetDcon(DconD);
	
	return CDgnCodeCtrl::GetConCodeNumber4Log(strCodeNumber, DconD.DesignCode);
}

