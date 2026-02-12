// CMBndrSprtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "..\wg_db\wg_db_DBDoc.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditUnit.h"
#include "..\wg_db\wg_db_TextUnit.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "CMBndrSprtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLSIZE   21
/////////////////////////////////////////////////////////////////////////////
// CCMBndrSprtDlg dialog


CCMBndrSprtDlg::CCMBndrSprtDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMBndrSprtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMBndrSprtDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();

	m_arSpringType.Add(IDC_CMD_SPRT_1_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_2_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_3_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_4_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_5_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_6_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_7_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_8_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_9_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_10_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_11_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_12_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_13_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_14_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_15_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_16_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_17_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_18_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_19_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_20_EDIT);
	m_arSpringType.Add(IDC_CMD_SPRT_21_EDIT);

	m_arDSpringType.Add(IDC_CMD_SPRT_D7_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D8_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D9_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D10_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D11_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D12_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D13_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D14_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D15_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D16_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D17_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D18_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D19_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D20_EDIT);
	m_arDSpringType.Add(IDC_CMD_SPRT_D21_EDIT);

	m_bStiffness = TRUE;
	m_bMass = FALSE;
	m_bDamping = FALSE;

	m_bInit = TRUE;
}


void CCMBndrSprtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMBndrSprtDlg)
	DDX_Check  (pDX, IDC_CMD_SPRT_STIFF_CHK, m_bStiffness);
	DDX_Check  (pDX, IDC_CMD_SPRT_MASS_CHK,  m_bMass);
	DDX_Check  (pDX, IDC_CMD_SPRT_DAMP_CHK,  m_bDamping);
	DDX_Control(pDX, IDC_CMD_TAB, m_Tab);
	DDX_Control(pDX, IDC_CMD_GRID_PLACEHOLDER, m_wndGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCMBndrSprtDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMBndrSprtDlg)
	ON_BN_CLICKED(IDC_CMD_SPRT_STIFF_CHK, OnCmdSprtChangeMethod)
	ON_BN_CLICKED(IDC_CMD_SPRT_MASS_CHK,  OnCmdSprtChangeMethod)
	ON_BN_CLICKED(IDC_CMD_SPRT_DAMP_CHK,  OnCmdSprtChangeMethod)
	ON_BN_CLICKED(IDC_CMD_SPRT_ADD, OnCmdSprtAdd)
	ON_BN_CLICKED(IDC_CMD_SPRT_CLOSE, OnCmdSprtClose)
	ON_BN_CLICKED(IDC_CMD_SPRT_DELETE, OnCmdSprtDelete)
	ON_BN_CLICKED(IDC_CMD_SPRT_REPLACE, OnCmdSprtReplace)
	ON_EN_CHANGE(IDC_CMD_SPRT_7_EDIT, OnChangeCmdSprt7Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_8_EDIT, OnChangeCmdSprt8Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_9_EDIT, OnChangeCmdSprt9Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_10_EDIT, OnChangeCmdSprt10Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_11_EDIT, OnChangeCmdSprt11Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_12_EDIT, OnChangeCmdSprt12Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_13_EDIT, OnChangeCmdSprt13Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_14_EDIT, OnChangeCmdSprt14Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_15_EDIT, OnChangeCmdSprt15Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_16_EDIT, OnChangeCmdSprt16Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_17_EDIT, OnChangeCmdSprt17Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_18_EDIT, OnChangeCmdSprt18Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_19_EDIT, OnChangeCmdSprt19Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_20_EDIT, OnChangeCmdSprt20Edit)
	ON_EN_CHANGE(IDC_CMD_SPRT_21_EDIT, OnChangeCmdSprt21Edit)
	ON_NOTIFY(TCN_SELCHANGE, IDC_CMD_TAB, OnSelchangeCmdTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_CMD_TAB, OnSelchangingCmdTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMBndrSprtDlg message handlers

BOOL CCMBndrSprtDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	m_Tab.InsertItem(0, _LS(IDS_CMD_STIFFNESS));
	m_Tab.InsertItem(1, _LS(IDS_CMD_MASS));
	m_Tab.InsertItem(2, _LS(IDS_CMD_DAMPING));

	m_wndGrid.Initialize(m_pDoc, this);
	m_wndGrid.CheckCurrentRecordChanged();

	if(!m_pDoc->m_pAttrCtrl->GetCountGstp())
	{
		for(int i=0; i<m_arSpringType.GetSize(); i++)
			((CEdit *)GetDlgItem(m_arSpringType.GetAt(i)))->SetWindowText(_T("0"));

		for (int i=0; i<COLSIZE; i++)
		{
			m_dSpring[i] = 0.0;
			m_dMass[i] = 0.0;
			m_dDamping[i] = 0.0;
		}
	}
	m_Tab.SetCurSel(0);

	// Stiffness Check Box 는 무조건 비활성화 처리
	GetDlgItem(IDC_CMD_SPRT_STIFF_CHK)->EnableWindow(FALSE);

	OnCmdSprtChangeMethod();

	m_bInit = FALSE;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMBndrSprtDlg::OnCmdSprtChangeMethod() 
{
	int nTabSel = m_Tab.GetCurSel();

 	if(!m_bInit) GetData(nTabSel);

 	UpdateData(TRUE);
	
	ChangeTabState(nTabSel);

	m_Tab.SetCurSel(nTabSel);
	SetData(nTabSel);

	UpdateData(FALSE);
}

void CCMBndrSprtDlg::OnCmdSprtAdd() 
{
	T_GSTP_K key;
	T_GSTP_D data;

	int nTabSel = m_Tab.GetCurSel();

	if (!GetGstpData(data)) return;
	if (m_pDoc->m_pDataCtrl->AddGstp(data))
	{ 
		key = m_pDoc->m_pAttrCtrl->GetGstpKey(data.TypeName);
		m_wndGrid.SetCurrentGSTP(key, data);
	}

	m_Tab.SetCurSel(nTabSel);
}

void CCMBndrSprtDlg::OnCmdSprtDelete() 
{
	T_GSTP_K key;
	T_GSTP_D data;

	if (!m_wndGrid.GetCurrentGSTP(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Spring_Name_is_selected_to_be_));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetGstp(key, data));
	m_pDoc->m_pDataCtrl->DelGstp(data.TypeName);
}

void CCMBndrSprtDlg::OnCmdSprtReplace() 
{
	T_GSTP_K key;
	T_GSTP_D data;
	if (!m_wndGrid.GetCurrentGSTP(key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__No_Spring_Name_is_selected_to_be__MSG1));
		return;
	}
	VERIFY(m_pDoc->m_pAttrCtrl->GetGstp(key, data));

	CString csOldName = data.TypeName;

	int nTabSel = m_Tab.GetCurSel();

	unsigned int nOldTypeID = data.nTypeId;
	if (!GetGstpData(data)) return;
	data.nTypeId = nOldTypeID;
	m_pDoc->m_pDataCtrl->ModifyGstp(csOldName, data);

	m_Tab.SetCurSel(nTabSel);
}

void CCMBndrSprtDlg::OnCmdSprtClose() 
{
	CDialogMove::OnOK();
}

void CCMBndrSprtDlg::OnChangeCurrentGSTP(T_GSTP_K nKey)
{
	T_GSTP_K key;
	T_GSTP_D data;
	if (nKey == 0)
		data.Initialize();
	else
	{
		if (!m_wndGrid.GetCurrentGSTP(key))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Programming_Error___No_selected_G));
			return;
			data.Initialize();
		}
		if (!m_pDoc->m_pAttrCtrl->GetGstp(key, data))
		{
			AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Programming_Error___Can_t_read_GS));
			return;
		}
	}
	
	CString strTemp;
	((CEdit *)GetDlgItem(IDC_CMD_SPRT_NAME_EDIT))->SetWindowText(data.TypeName);	

	for (int i=0; i<COLSIZE; i++)
	{
		m_dSpring[i] = data.Spring[i];
		m_dMass[i] = data.Mass[i];
		m_dDamping[i] = data.Damping[i];
	}

	m_bStiffness = data.bStiffness;
	m_bMass = data.bMass;
	m_bDamping = data.bDamping;

	UpdateData(FALSE);

	int nTabSel = m_Tab.GetCurSel();
	ChangeTabState(nTabSel);
		
	SetData(nTabSel);
}

void CCMBndrSprtDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
		//    UnitChanged();
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

void CCMBndrSprtDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	// 현재 Row, Col을 저장 : 나중에 복구하기 위함
	ROWCOL ncRow, ncCol, nkCol;
	T_GSTP_K cKey;
	m_wndGrid.GetCurrentCell(ncRow, ncCol);
	m_wndGrid.GetColumnNoFor(m_wndGrid.GetKeyColID(), nkCol);
	cKey = _tstol(m_wndGrid.GetValueRowCol(ncRow, nkCol));
	BOOL bCurrentRecordChanged = FALSE;

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_GSTP_K Key, KeyBak;
	T_GSTP_D Data, DataBak;

	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	BOOL bMFD = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		switch(nCmd)
		{
		case(UR_GSTP_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetGstp(nKey, Key, Data);
				m_wndGrid.InsertRecordByKey(Key, Data);
				break;
		case(UR_GSTP_DEL):
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetGstp(nKey, Key, Data);
				m_wndGrid.DeleteRecordByKey(Key, Data);
				m_wndGrid.GetCurrentCell(ncRow, ncCol);
				m_wndGrid.OnChangeCurrentRecord(ncRow);
				break;
		case(UR_GSTP_MFD):
				// 여기서는 저장만 하고 UR_FBLD_MFS에서 처리한다.
				pViewBuff->GetGstp(nKey, Key, Data);
				KeyBak = Key;
				DataBak = Data;
				bMFD = TRUE;
				break;
		case(UR_GSTP_MFS):
				// MFD가 먼저오고 MFS가 온다. 항상 이 순서대로 한 짝이 되어 온다.
				ASSERT(bMFD);
				pViewBuff->GetGstp(nKey, Key, Data);
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

BOOL CCMBndrSprtDlg::GetGstpData(T_GSTP_D& data)
{
	if (!UpdateData(TRUE)) 
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error_in_input_data_));
		return FALSE;
	}
	data.Initialize();

	CString strTemp;
	
	((CEdit *)GetDlgItem(IDC_CMD_SPRT_NAME_EDIT))->GetWindowText(strTemp);	
	data.TypeName = strTemp;

	/**
	for(int i=0; i<m_arSpringType.GetSize(); i++)
	{
		((CEdit *)GetDlgItem(m_arSpringType.GetAt(i)))->GetWindowText(strTemp);
		data.Spring[i]=strTemp.IsEmpty() ? 0 : _tstof(strTemp); 
	}
	**/
	//현재 활성화된 탭의 Data
	int nTabSel = m_Tab.GetCurSel();
	GetData(nTabSel);

	for (int i=0; i<COLSIZE; i++)
	{
		data.Spring[i] = m_dSpring[i];

		if(m_bMass) data.Mass[i] = m_dMass[i];
		else        data.Mass[i] = 0.;

		if(m_bDamping) data.Damping[i] = m_dDamping[i];
		else           data.Damping[i] = 0.;
	}

	data.bStiffness = m_bStiffness;
	data.bMass = m_bMass;
	data.bDamping = m_bDamping;

	return TRUE;
}

void CCMBndrSprtDlg::OnChangeCmdSprt7Edit()  {	ChangeSpringNumber(7);  }
void CCMBndrSprtDlg::OnChangeCmdSprt8Edit()  {	ChangeSpringNumber(8);  }
void CCMBndrSprtDlg::OnChangeCmdSprt9Edit()  {	ChangeSpringNumber(9);  }
void CCMBndrSprtDlg::OnChangeCmdSprt10Edit() {	ChangeSpringNumber(10); }
void CCMBndrSprtDlg::OnChangeCmdSprt11Edit() {	ChangeSpringNumber(11); }
void CCMBndrSprtDlg::OnChangeCmdSprt12Edit() {	ChangeSpringNumber(12); }
void CCMBndrSprtDlg::OnChangeCmdSprt13Edit() {	ChangeSpringNumber(13); }
void CCMBndrSprtDlg::OnChangeCmdSprt14Edit() {	ChangeSpringNumber(14); }
void CCMBndrSprtDlg::OnChangeCmdSprt15Edit() {	ChangeSpringNumber(15); }
void CCMBndrSprtDlg::OnChangeCmdSprt16Edit() {	ChangeSpringNumber(16); }
void CCMBndrSprtDlg::OnChangeCmdSprt17Edit() {	ChangeSpringNumber(17); }
void CCMBndrSprtDlg::OnChangeCmdSprt18Edit() {	ChangeSpringNumber(18); }
void CCMBndrSprtDlg::OnChangeCmdSprt19Edit() {	ChangeSpringNumber(19); }
void CCMBndrSprtDlg::OnChangeCmdSprt20Edit() {	ChangeSpringNumber(20); }
void CCMBndrSprtDlg::OnChangeCmdSprt21Edit() {	ChangeSpringNumber(21); }

void CCMBndrSprtDlg::ChangeSpringNumber(int nEditNum)
{
	CString strTemp;
	((CEdit *)GetDlgItem(m_arSpringType.GetAt(nEditNum-1)))->GetWindowText(strTemp);
	((CEdit *)GetDlgItem(m_arDSpringType.GetAt(nEditNum-7)))->SetWindowText(strTemp);
}

void CCMBndrSprtDlg::ChangeTabState(int &nTabSel)
{
	m_Tab.DeleteAllItems();
	m_Tab.InsertItem(0, _LS(IDS_CMD_STIFFNESS));
	
	if(m_bMass && m_bDamping)
	{
		m_Tab.InsertItem(1, _LS(IDS_CMD_MASS));
		m_Tab.InsertItem(2, _LS(IDS_CMD_DAMPING));
	}
	else if(m_bMass && !m_bDamping)
	{
		m_Tab.InsertItem(1, _LS(IDS_CMD_MASS));
		
		if(nTabSel == 2) nTabSel = 0;
	}
	else if(!m_bMass && m_bDamping)
	{
		m_Tab.InsertItem(1, _LS(IDS_CMD_DAMPING));
		
		if(nTabSel == 2) nTabSel = 0;
	}
	else
	{
		nTabSel = 0;
	}
}

void CCMBndrSprtDlg::OnSelchangingCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int nSel = m_Tab.GetCurSel();
	GetData(nSel);
	
	*pResult = 0;
}

void CCMBndrSprtDlg::OnSelchangeCmdTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	UINT ctrl[2][6] = {{IDC_CMD_SDX1, IDC_CMD_SDY1, IDC_CMD_SDZ1, IDC_CMD_SRX1, IDC_CMD_SRY1, IDC_CMD_SRZ1},
										 {IDC_CMD_SDX2, IDC_CMD_SDY2, IDC_CMD_SDZ2, IDC_CMD_SRX2, IDC_CMD_SRY2, IDC_CMD_SRZ2}};
	CString strStif[] = {_LSX(SDx), _LSX(SDy), _LSX(SDz), _LSX(SRx), _LSX(SRy), _LSX(SRz)};
	CString strMass[] = {_LSX(mx),  _LSX(my),  _LSX(mz),  _LSX(rmx), _LSX(rmy), _LSX(rmz)};
	CString strDamp[] = {_LSX(cx),  _LSX(cy),  _LSX(cz),  _LSX(rcx), _LSX(rcy), _LSX(rcz)};

	int nSel = m_Tab.GetCurSel();
	CString* pStr;
	if (nSel == 0)
	{
		pStr = strStif;
	}
	else if (nSel == 1)
	{
		if(m_bMass) pStr = strMass;
		else        pStr = strDamp;
	}
	else
	{
		pStr = strDamp;
	}

	for (int i=0; i<6; i++)
	{
		GetDlgItem(ctrl[0][i])->SetWindowText(pStr[i]);
		GetDlgItem(ctrl[1][i])->SetWindowText(pStr[i]);
	}
	SetData(nSel);
	*pResult = 0;
}

void CCMBndrSprtDlg::GetData(int nTabIndex)
{
	double* pData;
	if (nTabIndex == 0)
		pData = m_dSpring;
		
	if(m_bMass && m_bDamping)
	{
		if (nTabIndex == 1)
			pData = m_dMass;
		else if (nTabIndex == 2)
			pData = m_dDamping;
	}
	else if(m_bMass && !m_bDamping)
	{
		if (nTabIndex == 1)
			pData = m_dMass;
	}
	else if(!m_bMass && m_bDamping)
	{
		if (nTabIndex == 1)
			pData = m_dDamping;
	}
	else
	{
		// Nothing
	}

	CString strTemp;
	for(int i=0; i<m_arSpringType.GetSize(); i++)
	{
		((CEdit *)GetDlgItem(m_arSpringType.GetAt(i)))->GetWindowText(strTemp);
		pData[i] = strTemp.IsEmpty() ? 0 : _tstof(strTemp); 
	}
}

void CCMBndrSprtDlg::SetData(int nTabIndex)
{
	double* pData;
	if (nTabIndex == 0)
		pData = m_dSpring;
	
	if(m_bMass && m_bDamping)
	{
		if (nTabIndex == 1)
			pData = m_dMass;
		else if (nTabIndex == 2)
			pData = m_dDamping;
	}
	else if(m_bMass && !m_bDamping)
	{
		if (nTabIndex == 1)
			pData = m_dMass;
	}
	else if(!m_bMass && m_bDamping)
	{
		if (nTabIndex == 1)
			pData = m_dDamping;
	}
	else
	{
		// Nothing
	}

	CString strTemp;
	for (int i=0; i<m_arSpringType.GetSize(); i++)
	{
		strTemp.Format(_T("%g"), pData[i]);	
		((CEdit *)GetDlgItem(m_arSpringType.GetAt(i)))->SetWindowText(strTemp);
	}
}
