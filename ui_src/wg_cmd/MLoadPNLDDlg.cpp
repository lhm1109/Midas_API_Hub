// MLoadPNLDDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "MLoadPNLDDlg.h"

#include "CMLoadPNLDAreaDlg.h"
#include "CMLoadPNLDLineDlg.h"
#include "CMLoadPNLDPointDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_UnitCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMLoadPNLDDlg dialog


CMLoadPNLDDlg::CMLoadPNLDDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CMLoadPNLDDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMLoadPNLDDlg)
	m_sName = _T("");
	m_nType = 0;
	m_sDscp = _T("");
	m_sCpyXdirEd = _T("");
	m_sCpyYdirEd = _T("");
	//}}AFX_DATA_INIT

	m_pDlgPtPnLd = NULL;	// point load type dialog ptr	
	m_pDlgLnPnLd = NULL;	// line	 load type dialog ptr	
	m_pDlgArPnLd = NULL;	// area load type dialog ptr	

	m_nWhichLType = 0;
}


void CMLoadPNLDDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMLoadPNLDDlg)
	DDX_Control(pDX, IDC_CMD_PLLD_DLGHOLDER, m_stDlgHolder);
	DDX_Control(pDX, IDC_CMD_PLLD_YDIR_UNIT, m_stCpyYdirUnit);
	DDX_Control(pDX, IDC_CMD_PLLD_XDIR_UNIT, m_stCpyXdirUnit);
	DDX_Text(pDX, IDC_CMD_PLLD_NAME, m_sName);
	DDX_Radio(pDX, IDC_CMD_PLLDLTYPE_POINT, m_nType);
	DDX_Text(pDX, IDC_CMD_PLLD_DESC, m_sDscp);
	DDX_Text(pDX, IDC_CMD_PLLD_XDIR_ED, m_sCpyXdirEd);
	DDX_Text(pDX, IDC_CMD_PLLD_YDIR_ED, m_sCpyYdirEd);
	DDX_Control(pDX, IDC_CMD_PLLD_GRID, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMLoadPNLDDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CMLoadPNLDDlg)
	ON_BN_CLICKED(IDC_CMD_PLLD_ADD, OnCmdPlldAdd)
	ON_BN_CLICKED(IDC_CMD_PLLD_CLOSE, OnCmdPlldClose)
	ON_BN_CLICKED(IDC_CMD_PLLD_DELETE, OnCmdPlldDelete)
	ON_BN_CLICKED(IDC_CMD_PLLD_MODIFY, OnCmdPlldModify)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CMD_PLLDLTYPE_POINT, OnCmdPlldltypePoint)
	ON_BN_CLICKED(IDC_CMD_PLLDLTYPE_LINE, OnCmdPlldltypeLine)
	ON_BN_CLICKED(IDC_CMD_PLLDLTYPE_AREA, OnCmdPlldltypeArea)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMLoadPNLDDlg message handlers

BOOL CMLoadPNLDDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CCMDlgBase::OnInitDialog();
	
	// 현재 dlg초기화 
	m_Data.Initialize();
	InitUnit();
	
	// child dlgs를 생성함 
	this->CreateChildDlgs();			// load type dlg를 생성 
	this->ShowChildDlgs(m_nType);	// 현재 mode에 맞게 보여줌 
	SetData2Dlg();					

	// Grid를 초기화 시킨다.
	m_wndGrid.Initialize(m_pDoc, this);
	m_wndGrid.SetScrollBarMode(SB_VERT|SB_HORZ, gxnEnabled | gxnEnhanced);
	m_wndGrid.CheckCurrentRecordChanged();
	
	// DB에서 데이타를 갖고 와서.. Grid control에 넣는다. 
	UpdateAllGridRecord();
	m_wndGrid.CheckCurrentRecordChanged();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMLoadPNLDDlg::OnDestroy() 
{
	this->DestroyChildDlgs();
	CCMDlgBase::OnDestroy();
}

void CMLoadPNLDDlg::OnCmdPlldAdd() 
{
// TODO: Add your control notification handler code here
	T_PNLD_K key;
	if(!SetDlg2Data())return;	
	if (m_pDoc->m_pDataCtrl->AddPnld(m_Data))
	{ //새로 추가된 PNLD로 이동(grid에서)
		key = m_pDoc->m_pAttrCtrl->GetPnldKey(m_Data.LoadTypeName);
		m_wndGrid.SetCurrentPnld(key, m_Data);
	}
}

void CMLoadPNLDDlg::OnCmdPlldClose() 
{
	//this->EndDialog(0);	
	//CCMDlgBase::OnClose();
	DestroyWindow();
}

void CMLoadPNLDDlg::OnCmdPlldDelete() 
{
	// TODO: Add your control notification handler code here
	T_PNLD_K key;
	T_PNLD_D data;
	if (!m_wndGrid.GetCurrentPnld(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_Load_is_selected_to_be_d));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetPnld(key, data));
	m_pDoc->m_pDataCtrl->DelPnld(data.LoadTypeName);
	m_wndGrid.CheckCurrentRecordChanged();
}

void CMLoadPNLDDlg::OnCmdPlldModify() 
{
	// TODO: Add your control notification handler code here
	T_PNLD_K key;
	T_PNLD_D data;
	if (!m_wndGrid.GetCurrentPnld(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_floor_Load_is_selected_to_be_m));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetPnld(key, data));
	if(!SetDlg2Data())return;	
	m_Data.nSeq = data.nSeq;	// sequence number는 기억을 하고 있어야함 
	m_pDoc->m_pDataCtrl->ModifyPnld(data.LoadTypeName, m_Data);		
	m_wndGrid.CheckCurrentRecordChanged();
}



/////////////////////////////////////////////////////////////
// DATE : 2002.1.11. by KYE-HONG
// DESC : 모든 load type의 child dlgs를 만들어 둔다.
/////////////////////////////////////////////////////////////

BOOL	CMLoadPNLDDlg::CreateChildDlgs(void)
{

	CRect rc;
	m_stDlgHolder.GetWindowRect(&rc);
	this->ScreenToClient(&rc);

	m_pDlgPtPnLd= new CCMLoadPNLDPointDlg(&m_Data);	
	ASSERT(m_pDlgPtPnLd);	
	m_pDlgPtPnLd->Create(IDD_CMD_PNLD_PAGE_POINTLD,this);				
	m_pDlgPtPnLd->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);

	m_pDlgLnPnLd= new CCMLoadPNLDLineDlg(&m_Data);	
	ASSERT(m_pDlgLnPnLd);	
	m_pDlgLnPnLd->Create(IDD_CMD_PNLD_PAGE_LINELD,this);				
	m_pDlgLnPnLd->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);

	m_pDlgArPnLd= new CCMLoadPNLDAreaDlg(&m_Data);	
	ASSERT(m_pDlgArPnLd);	
	m_pDlgArPnLd->Create(IDD_CMD_PNLD_PAGE_AREALD,this);				
	m_pDlgArPnLd->SetWindowPos(NULL,rc.left,rc.top,rc.Width(),rc.Height(),SWP_NOZORDER|SWP_NOACTIVATE);
			
	return TRUE;
}

/////////////////////////////////////////////////////////////
// DATE : 2002.1.11. by KYE-HONG
// DESC : 모든 load type의 child dlgs를 destroy
/////////////////////////////////////////////////////////////

BOOL	CMLoadPNLDDlg::DestroyChildDlgs(void)
{
	// destroy pt load dlg
	if(m_pDlgPtPnLd)
	{
		m_pDlgPtPnLd->ShowWindow(SW_HIDE);
		m_pDlgPtPnLd->DestroyWindow();
		delete	m_pDlgPtPnLd;	m_pDlgPtPnLd=NULL;
	}

	// destroy line load dlg
	if(m_pDlgLnPnLd)
	{
		m_pDlgLnPnLd->ShowWindow(SW_HIDE);
		m_pDlgLnPnLd->DestroyWindow();
		delete	m_pDlgLnPnLd;	m_pDlgLnPnLd=NULL;
	}

	// destroy area load dlg
	if(m_pDlgArPnLd)
	{
		m_pDlgArPnLd->ShowWindow(SW_HIDE);
		m_pDlgArPnLd->DestroyWindow();
		delete	m_pDlgArPnLd;	m_pDlgArPnLd=NULL;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////
// DATE : 2002.1.11. by KYE-HONG
// DESC : 선택 모드(point , line, area)에 따라 child dlg를 보임 
/////////////////////////////////////////////////////////////

BOOL	CMLoadPNLDDlg::ShowChildDlgs(int which)
{
	// 모두 hide
	m_pDlgPtPnLd->ShowWindow(SW_HIDE);	
	m_pDlgLnPnLd->ShowWindow(SW_HIDE);	
	m_pDlgArPnLd->ShowWindow(SW_HIDE);	

	// 현재 선택된것만 show
	switch(which)
	{
		case 0:	m_pDlgPtPnLd->ShowWindow(SW_SHOW);	break;
		case 1:	m_pDlgLnPnLd->ShowWindow(SW_SHOW);	break;
		case 2:	m_pDlgArPnLd->ShowWindow(SW_SHOW);	break;
		default: m_pDlgPtPnLd->ShowWindow(SW_SHOW);	break;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////
// DATE : 2002.1.11. by KYE-HONG
// DESC : plane load의 load type radio버튼을 선택할때.. 
//				child dlgs를 바꿔준다. 
/////////////////////////////////////////////////////////////

void CMLoadPNLDDlg::OnCmdPlldltypePoint() 
{
	int nOldType = m_nType;
	UpdateData(TRUE);
	if(nOldType==m_nType) return; 
	m_Data.Initialize();
	m_Data.nLoadType    = m_nType;
	m_Data.LoadTypeName = this->m_sName;
	m_Data.Description	= this->m_sDscp;
	SetData2Dlg();
	ShowChildDlgs(m_nType);
	m_pDlgPtPnLd->UpdateData(FALSE);
}

void CMLoadPNLDDlg::OnCmdPlldltypeLine() 
{
	int nOldType = m_nType;
	UpdateData(TRUE);
	if(nOldType==m_nType) return; 
	m_Data.Initialize();
	m_Data.nLoadType = m_nType;
	m_Data.LoadTypeName = this->m_sName;
	m_Data.Description	= this->m_sDscp;
	SetData2Dlg();
	ShowChildDlgs(m_nType);
	m_pDlgLnPnLd->UpdateData(FALSE);
}

void CMLoadPNLDDlg::OnCmdPlldltypeArea() 
{
	int nOldType = m_nType;
	UpdateData(TRUE);
	if(nOldType==m_nType) return; 
	m_Data.Initialize();
	m_Data.nLoadType = m_nType;
	m_Data.LoadTypeName = this->m_sName;
	m_Data.Description	= this->m_sDscp;
	SetData2Dlg();
	ShowChildDlgs(m_nType);
	m_pDlgArPnLd->UpdateData(FALSE);
}


/////////////////////////////////////////////////////////////
// DATE : 2002.1.17. by KYE-HONG
// DESC : data exachage
//				
/////////////////////////////////////////////////////////////
void	CMLoadPNLDDlg::SetData2Dlg(void)
{
	this->m_sName = m_Data.LoadTypeName;
	this->m_nType = m_Data.nLoadType;
	this->m_sDscp	= m_Data.Description;

	this->m_sCpyXdirEd = CStrParser::ConvtDistArr2DistStr(m_Data.aCopyX); 	
	this->m_sCpyYdirEd = CStrParser::ConvtDistArr2DistStr(m_Data.aCopyY); 

	UpdateData(FALSE);

	switch(this->m_nType)
	{
	case 0:		
		m_pDlgPtPnLd->SetData2Dlg(); 
		m_pDlgPtPnLd->UpdateData(FALSE);
		break;
	case 1:		
		m_pDlgLnPnLd->SetData2Dlg(); 
		//m_pDlgLnPnLd->UpdateData(FALSE);
		break;
	case 2:		
		m_pDlgArPnLd->SetData2Dlg(); 
		m_pDlgArPnLd->UpdateData(FALSE);
		break;
	default: return;
	}
}

BOOL	CMLoadPNLDDlg::SetDlg2Data(void)
{
	UpdateData(TRUE);
	
	m_Data.Initialize();		
	m_Data.LoadTypeName = this->m_sName;
	m_Data.nLoadType		= this->m_nType;
	m_Data.Description	= this->m_sDscp;

	BOOL rtn;
	if(!this->m_sCpyXdirEd.IsEmpty())
	{
		rtn = CStrParser::GetUEDistance(this->m_sCpyXdirEd,m_Data.aCopyX);
		if(!rtn) m_Data.aCopyX.RemoveAll();
	}
	if(!this->m_sCpyYdirEd.IsEmpty())
	{
		rtn = CStrParser::GetUEDistance(this->m_sCpyYdirEd,m_Data.aCopyY);
		if(!rtn) m_Data.aCopyY.RemoveAll();
	}

	// child data는 현재 선택된 child만 update한다. 
	switch(m_nType)
	{
	case 0:		if(!m_pDlgPtPnLd->SetDlg2Data())return FALSE;	break;
	case 1:		if(!m_pDlgLnPnLd->SetDlg2Data())return FALSE;	break;
	case 2:		if(!m_pDlgArPnLd->SetDlg2Data())return FALSE;	break;
	default: return FALSE;
	}

	return TRUE;
}

void	CMLoadPNLDDlg::InitUnit(void)
{
	this->m_stCpyXdirUnit.SetUnitType(CUnitCtrl::m_PNLD_UNIT.CopyXY);
	this->m_stCpyYdirUnit.SetUnitType(CUnitCtrl::m_PNLD_UNIT.CopyXY);
}


void	CMLoadPNLDDlg::UpdateAllGridRecord(void)
{
	T_PNLD_D	rData;
	CArray<T_PNLD_K, T_PNLD_K> rKeyList;
	
	m_wndGrid.DeleteAllRecords();
	m_pDoc->m_pAttrCtrl->GetPnldKeyList(rKeyList);
	for(int i=0; i<rKeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetPnld(rKeyList[i],rData);
		m_wndGrid.InsertRecordByKey(rKeyList[i],rData);
		// if(i==0) OnChangeCurrentPnld(rData.LoadTypeName);	// 추가되는게 있으면.. setting해줌 
	}
}

void	CMLoadPNLDDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd;
	if ((hWnd = GetSafeHwnd()) == 0 || !IsWindow(hWnd)) return;

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
		UpdateUnit();
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

void	CMLoadPNLDDlg::UpdateBuffer(void)
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_PNLD_K cKey;
	m_wndGrid.GetCurrentCell(ncRow, ncCol);
	m_wndGrid.GetColumnNoFor(m_wndGrid.GetKeyColID(), nkCol);
	cKey = _tstol(m_wndGrid.GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_PNLD_K Key, KeyBak;
	T_PNLD_D Data, DataBak;

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_PNLD_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetPnld(nKey, Key, Data);
				m_wndGrid.InsertRecordByKey(Key, Data);
				break;
		case(UR_PNLD_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetPnld(nKey, Key, Data);
				m_wndGrid.DeleteRecordByKey(Key, Data);
				break;
		case(UR_PNLD_MFD):
				// 여기서는 저장만 하고 UR_PNLD_MFS에서 처리한다.
				pViewBuff->GetPnld(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_PNLD_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetPnld(nKey, Key, Data);
				m_wndGrid.DeleteRecordByKey(Key, Data);
				m_wndGrid.InsertRecordByKey(KeyBak, DataBak);
				if (Key == cKey) bCurrentRecordChanged = TRUE;
				// 플래그 수정
				bMFD = FALSE;
				break;
		default:
			break;
		}
	} // end of while

	// 원래 row, col을 복구하려고 시도
	if (m_wndGrid.GetRowCount() < ncRow) ncRow = m_wndGrid.GetRowCount();
	m_wndGrid.SetCurrentCell(ncRow, ncCol);

	if (!m_wndGrid.CheckCurrentRecordChanged())
	{
		// 현재 레코드 위치 변경없이 내용이 변경된 경우
		if (bCurrentRecordChanged) m_wndGrid.OnChangeCurrentRecord(ncRow);
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CMLoadPNLDDlg::UpdateUnit()
{
	UpdateData(TRUE);
	
	switch(m_nType)
	{
	case 0: // point load type에서만 이렇게 함 		
		// 단위변환을 위해 데이타를 새로 얻어옴 
		// UpdateDataBuffer(m_Data.LoadTypeName);
		// SetData2Dlg();
		// m_pDlgPtPnLd->UpdateUnit(); 
		m_pDlgPtPnLd->UpdateUnit();
		break;
	case 1:
		m_pDlgPtPnLd->UpdateUnit();
		break;
	case 2:
		m_pDlgPtPnLd->UpdateUnit();
		break;
	default: return;
	}
}

void CMLoadPNLDDlg::PostNcDestroy() 
{
	CCMDlgBase::PostNcDestroy();
//	delete this;
}

void CMLoadPNLDDlg::OnChangeCurrentPnld(CString lKey)
{
	UpdateDataBuffer(lKey);
	SetData2Dlg();	
	ShowChildDlgs(m_nType);
}


BOOL CMLoadPNLDDlg::UpdateDataBuffer(CString lKey)
{
	T_PNLD_K key = _tstoi(lKey);
	T_PNLD_D Data; 
	if(!m_pDoc->m_pAttrCtrl->GetPnld(key,Data)) return FALSE;
	m_Data.Initialize();
	m_Data = Data;
	return TRUE;
}
