// PsllItemDlg.cpp : implementation file
//
// 2003. 12. 27   by TAE
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "PsllItemDlg.h"

#include "PsllItemCodeMgr.h"
#include "WindSeisAddLoadDlg.h"
#include "WindSeisAddLoadGrid.h"    // List를 Grid로 변경 // PMS:XXXX-4183-20100525

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_LateralLoad.h"
#include "..\wg_db\IUsageCounter.h"
#include "..\wg_main\wg_mainRes2.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_InitCtrl.h"//T_PREFERENCE
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPsllItemDlg dialog

CPsllItemDlg::CPsllItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CPsllItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPsllItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_Key = 0;
	m_Data.Initialize();
	m_bModify = FALSE;

	m_pCodeMgr = NULL;
	m_pCodeMgr = new CPsllItemCodeMgr(this);

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_pGridWnd = new CWindSeisAddLoadGrid();

	m_aMoveCtrlID.Add(IDC_CMD_DESCRIPTION_STC);
	m_aMoveCtrlID.Add(IDC_CMD_DESCRIPTION);
	m_aMoveCtrlID.Add(IDC_CMD_CHILD_FRAME);
	m_aMoveCtrlID.Add(IDC_CMD_SEISLOADFACTOR);
	m_aMoveCtrlID.Add(IDC_CMD_ADD_LOAD_STC);
	m_aMoveCtrlID.Add(IDC_CMD_BTN_PROFILE);
	m_aMoveCtrlID.Add(IDOK);
	m_aMoveCtrlID.Add(IDCANCEL);
	m_aMoveCtrlID.Add(IDC_CMD_APPLY);
	m_aMoveCtrlID.Add(IDC_CMD_XDIRECTION);
	m_aMoveCtrlID.Add(IDC_CMD_SCALE_GX);
	m_aMoveCtrlID.Add(IDC_CMD_YDIRECTION);
	m_aMoveCtrlID.Add(IDC_CMD_SCALE_GY);
	m_aMoveCtrlID.Add(IDC_CMD_GRID);
	m_aMoveCtrlID.Add(IDC_CMD_BTN_ADD);
	m_aMoveCtrlID.Add(IDC_CMD_LIST);
	m_aMoveCtrlID.Add(IDC_CMD_BTN_DELETE);
}

CPsllItemDlg::~CPsllItemDlg()
{
	if (m_pCodeMgr != NULL)
	{
		delete m_pCodeMgr;
		m_pCodeMgr = NULL;
	}

	if (m_pGridWnd)
	{
		delete m_pGridWnd;
		m_pGridWnd = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Interface

void CPsllItemDlg::SetParamData(T_PSLL_K Key, T_PSLL_D &ParamData) 
{ 
	m_Key = m_OldKey = Key;
	m_Data = ParamData; 
	m_bModify = TRUE; 
}
	
void CPsllItemDlg::AddAdditionalLoad(T_STOR_K StartStorKey, T_STOR_K EndStorKey, double dblX, double dblY, double dblRZ)
{
	// Story Key List를 얻는다.
	CArray<T_STOR_K, T_STOR_K> aKeyList;
	m_pDoc->m_pAttrCtrl->GetStorKeyListByConnect(aKeyList, &StartStorKey, &EndStorKey);

	// Additional Seismic Load Item 을 만든다.
	T_SEIS_ADDITION item;
	item.AddX = dblX;
	item.AddY = dblY;
	item.AddR = dblRZ;

	// Story List에서 시작 키의 Index를 찾는다.
	int nCount = aKeyList.GetSize();
	int i = 0;
	for (i = 0; i < nCount; i++) 
		if (aKeyList[i] == StartStorKey) break;
	
	// 입력된 Seismic Load List를 만든다.
	T_STOR_K key;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aAddition;
	do
	{
		key = aKeyList[i++];
		item.KeyStor = key;
		aAddition.Add(item);
	}
	while (key != EndStorKey);

	// 새로 만들어진 List와 기존의 Seismic Load List를 Merge한다.
	int nCount1 = aAddition.GetSize();
	m_pGridWnd->GetData(m_Data.arAddition);
	int nCount2 = m_Data.arAddition.GetSize();
	int n1, n2;
	CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aNewAddition;
	for (n1 = n2 = 0; n1 < nCount1 && n2 < nCount2; )
	{
		if (aAddition[n1].KeyStor == m_Data.arAddition[n2].KeyStor)
		{
			aNewAddition.Add(aAddition[n1]);
			n1++; n2++;
		}
		else
		{
			T_STOR_D data1, data2;
			m_pDoc->m_pAttrCtrl->GetStor(aAddition[n1].KeyStor, data1);
			m_pDoc->m_pAttrCtrl->GetStor(m_Data.arAddition[n2].KeyStor, data2);
			if (data1.dStoryLevel < data2.dStoryLevel)
				aNewAddition.Add(aAddition[n1++]);
			else aNewAddition.Add(m_Data.arAddition[n2++]);
		}
	}
	// 남은 부분 처리
	for (; n1 < nCount1; n1++) aNewAddition.Add(aAddition[n1]);
	for (; n2 < nCount2; n2++) aNewAddition.Add(m_Data.arAddition[n2]);

	// 새로 만들어진 것을 복사한다.
	m_Data.arAddition.RemoveAll();
	m_Data.arAddition.Copy(aNewAddition);

	// List를 갱신한다.
	MakeItemEx();
}

void CPsllItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPsllItemDlg)
	DDX_Control(pDX, IDC_CMD_LOADCASE, m_wndLoadCase);
	DDX_Control(pDX, IDC_CMD_CODE_NAME, m_wndCodeName);
	DDX_Control(pDX, IDC_CMD_DESCRIPTION, m_wndDescription);
	DDX_Control(pDX, IDC_CMD_SCALE_GY, m_wndScaleGY);
	DDX_Control(pDX, IDC_CMD_SCALE_GX, m_wndScaleGX);
	DDX_Control(pDX, IDC_CMD_CHILD_FRAME, m_wndHolder);
	DDX_Control(pDX, IDC_CMD_GRID, *m_pGridWnd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPsllItemDlg, CDialogMove)
	//{{AFX_MSG_MAP(CPsllItemDlg)
	ON_BN_CLICKED(IDC_CMD_APPLY, OnCmdApply)
	ON_BN_CLICKED(IDC_CMD_BTN_ADD, OnCmdBtnAdd)
	ON_BN_CLICKED(IDC_CMD_BTN_PROFILE, OnCmdBtnCalcSheet)
	ON_BN_CLICKED(IDC_CMD_LOADCASE_BTN, OnCmdLoadcaseBtn)
	ON_BN_CLICKED(IDC_CMD_METHOD_BTN, OnCmdMethodBtn)
	ON_CBN_SELCHANGE(IDC_CMD_CODE_NAME, OnSelchangeCmdCodeName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPsllItemDlg message handlers

BOOL CPsllItemDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_wndLoadCase.SetLoadType(D_LOADCASE_STATIC, TRUE);
	InitCodeNameCobx();
// [PsllCode] 추가시 수정
	if (!m_bModify)
	{
		m_Data.Initialize();
		m_Data.nCodeType = m_pDoc->m_pAttrCtrl2->GetSemeMethod();
		m_Data.ScaleFactor_X = 1.0;
		m_Data.ScaleFactor_Y = 1.0;

		CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, m_Data.nCodeType);

		// [2011-05-04] Kim, Geun Young (Tel: 2042, gykim@midasit.com) : Default 선택
		CArray<T_STLD_K, T_STLD_K> aStldK;
		m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
		for (int i=0; i<aStldK.GetSize(); i++)
		{
			T_STLD_D StldD;
			if(!m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD)) {ASSERT(0); continue;}
			if(StldD.LoadCaseType == _T("E")) 
			{
				m_Key = aStldK[i];
				break;
			}
		}
	}
	else
	{
		GetDlgItem(IDC_CMD_APPLY)->EnableWindow(FALSE); 
	}

	CRect rect;
	GetDlgItem(IDC_CMD_GRID)->GetWindowRect(&rect);
	m_pGridWnd->Initialize(this, TRUE, rect.Width(), FALSE, FALSE);
	
	CString strName, strNewName;
	GetDlgItem(IDC_CMD_ADD_LOAD_STC)->GetWindowText(strName);
	
	CString strLengthUnit, strForceUnit;
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_LENGTH, strLengthUnit);
	CUnitCtrl::GetUnitSystem(D_UNITSYS_BASE_FORCE, strForceUnit);
	
	strNewName.Format(_T("%s (%s:%s,%s)"), strName, _LS(IDS_WG_CMD__ADDD__Unit), strForceUnit, strLengthUnit);
	GetDlgItem(IDC_CMD_ADD_LOAD_STC)->SetWindowText(strNewName);

	Data2Dlg();

	OnSelchangeCmdCodeName();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPsllItemDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (!Dlg2Data()) return; 
	
	BOOL bSuccess;
	if (m_bModify)
		bSuccess = m_pDoc->m_pDataCtrl->ModifyPsll(m_OldKey, m_Key, m_Data);
	else
		bSuccess = m_pDoc->m_pDataCtrl->AddPsll(m_Key, m_Data);

	switch (m_Data.nCodeType)
	{
	case D_SEME_METHOD_KISTEC2013:
	case D_SEME_METHOD_KISTEC2019:
		IUsageCounter::Use(_T("SPE_PSLK"));
		break;
	case D_SEME_METHOD_MOE2018:
	case D_SEME_METHOD_MOE2019:
		IUsageCounter::Use(_T("SPE_PSLM"));
		break;
	default:
		break;
	}

	if (!bSuccess) return;

	if(m_pDoc->m_pAttrCtrl->ExistSeis(m_Key))
	{
		T_STLD_D DataStld;
		if(m_pDoc->m_pAttrCtrl->GetStld(m_Key, DataStld))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD_WARNING_SEISMIC_DUPLICATE), DataStld.LoadCaseName);
		}
	}
	
	CDialogMove::OnOK();
}

void CPsllItemDlg::OnCmdApply() 
{
	// TODO: Add your control notification handler code here
	if (!Dlg2Data()) return; 
	
	if (m_bModify)
		m_pDoc->m_pDataCtrl->ModifyPsll(m_OldKey, m_Key, m_Data);
	else
		m_pDoc->m_pDataCtrl->AddPsll(m_Key, m_Data);	

	switch (m_Data.nCodeType)
	{
	case D_SEME_METHOD_KISTEC2013:
	case D_SEME_METHOD_KISTEC2019:
		IUsageCounter::Use(_T("SPE_PSLK"));
		break;
	case D_SEME_METHOD_MOE2018:
	case D_SEME_METHOD_MOE2019:
		IUsageCounter::Use(_T("SPE_PSLM"));
		break;
	default:
		break;
	}

	if(m_pDoc->m_pAttrCtrl->ExistSeis(m_Key))
	{
		T_STLD_D DataStld;
		if(m_pDoc->m_pAttrCtrl->GetStld(m_Key, DataStld))
		{
			GSaveHistoryFormatNF(_LS(IDS_WG_CMD_WARNING_SEISMIC_DUPLICATE), DataStld.LoadCaseName);
		}
	}
}

void CPsllItemDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}

void CPsllItemDlg::OnCmdBtnAdd() 
{
	// TODO: Add your control notification handler code here
	CWindSeisAddLoadDlg dlg;
	dlg.SetMode(nullptr, nullptr, (void*)this);
	dlg.DoModal();	
}

void CPsllItemDlg::OnCmdBtnCalcSheet() 
{
	if (!Dlg2Data()) return;

	// FileName;
	TCHAR *lpBuffer;
	lpBuffer = new TCHAR[255];

	CString strFileName = m_pDoc->GetPathName();
	if(strFileName == _T(""))
	{
		GetCurrentDirectory(255, lpBuffer);
		strFileName = lpBuffer;
		strFileName += _T("\\Untitled.spf");
	}
	else 
	{
		TCHAR path_buffer[_MAX_PATH];
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];

		strFileName.GetBuffer(255);
		_tcscpy(path_buffer,strFileName);
		strFileName.ReleaseBuffer();


		_tsplitpath( path_buffer, drive, dir, fname, ext );
		_tcscat(fname, _T("_PSLL"));
		_tmakepath( path_buffer, drive, dir,fname,_T("spf") );
		strFileName = path_buffer; 
	}

	CLateralLoad LateralLoad;
	LateralLoad.MakePseudoSeismicForce(m_Data,TRUE,strFileName,TRUE);

	///////////////////////////////////////////////////////////////////////////////
	// Launching the Text Editor;
	///////////////////////////////////////////////////////////////////////////////  
	CString strTitle = _LS(IDS_WG_CMD__ADD2__SEIS_LOAD_CALC_);
	CDBLib::RunTextEditor(strFileName, strTitle);	
}

void CPsllItemDlg::OnCmdLoadcaseBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STATIC_LOADCASE,0));	
}

void CPsllItemDlg::OnCmdMethodBtn()
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_MAIN_RIBBON_SEISEVAL_METHOD,0));
}

void CPsllItemDlg::OnSelchangeCmdCodeName() 
{
	// TODO: Add your control notification handler code here
	int nSel = m_wndCodeName.GetCurSel();
	int nCode = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, nSel);
	m_pCodeMgr->ChangeCodeDlg(nCode);
}

void CPsllItemDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CPsllItemDlg::UpdateBuffer()
{
	CWaitCursor wait;

	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_STOR_K Key, KeyBak;
	T_STOR_D Data, DataBak;
	BOOL bStorChanged = FALSE;
	BOOL bMethodChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_SEME_ADD):
			bMethodChanged = TRUE;
			break;
		case(UR_STOR_ADD):
			bStorChanged = TRUE;
			break;
		case(UR_STOR_DEL):
			{
				m_pDoc->m_pViewBuff->GetStor(nKey, Key, Data);
				int nCount = m_Data.arAddition.GetSize();
				int i = 0;
				for (i = 0; i < nCount; i++)
					if (m_Data.arAddition[i].KeyStor == Key) break;
				if (i != nCount) m_Data.arAddition.RemoveAt(i);
			}
			bStorChanged = TRUE;
			break;
		case(UR_STOR_MFD):
			{
				m_pDoc->m_pViewBuff->GetStor(nKey, KeyBak, DataBak);
			}
			bStorChanged = TRUE;
			break;
		case(UR_STOR_MFS): 
			{
				m_pDoc->m_pViewBuff->GetStor(nKey, Key, Data);
				int nCount = m_Data.arAddition.GetSize();
				int i = 0;
				for (i = 0; i < nCount; i++)
					if (m_Data.arAddition[i].KeyStor == Key) break;
				if (i != nCount) m_Data.arAddition[i].KeyStor = KeyBak;
			}
			bStorChanged = TRUE;
			break;
		default:
			break;
		}
	} // end of while

	if(bMethodChanged)
	{
		int nCode = m_pDoc->m_pAttrCtrl2->GetSemeMethod();
		m_Data.Initialize_Code(nCode);
		m_bModify = FALSE; // 기준 바뀌면 PSLL 삭제
		CDlgUtil::CobxSetCurSelItemData(m_wndCodeName, nCode);
		m_pCodeMgr->Data2CodeDlg(m_Data, nCode, m_bModify);
	}

	if (bStorChanged)
	{
		// while문에서는 Data 변경만 Update했다. 이 경우 Story 순서가 바뀌는 경우가
		// 발생할 수 도 있으므로 Sorting을 한다.
		CArray<T_SEIS_ADDITION, T_SEIS_ADDITION&> aAddition;
		CArray<T_STOR_K, T_STOR_K> aKeyList;
		m_pDoc->m_pAttrCtrl->GetStorKeyList(aKeyList);

		int nCount = aKeyList.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			int j = 0;
			int nCount2 = m_Data.arAddition.GetSize();
			for (j = 0; j < nCount2; j++)
				if (aKeyList[i] == m_Data.arAddition[j].KeyStor) break;
			if (j != nCount2) 
			{
				aAddition.Add(m_Data.arAddition[j]);
				m_Data.arAddition.RemoveAt(j);
			}
		}
		m_Data.arAddition.Copy(aAddition);
		// List를 갱신한다.
		MakeItemEx();
	}
}


/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CPsllItemDlg::InitCodeNameCobx()
{
	m_wndCodeName.ResetContent();

	CStringArray aCodeName;
	int nNum = m_pCodeMgr->GetCodeNameList(aCodeName);

	for(int i = 0; i < nNum; i++) 
	{
		CDlgUtil::CobxAddItem(m_wndCodeName, aCodeName[i], CDBLib::GetSeisEvalMethodByCodeName(aCodeName[i]));
	}

	m_wndCodeName.EnableWindow(FALSE);

	AdjustComboListBox();
}

void CPsllItemDlg::AdjustComboListBox()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_wndCodeName.GetDC();
	for (int i=0;i < m_wndCodeName.GetCount();i++)
	{
		m_wndCodeName.GetLBText( i, str );
		sz = pDC->GetTextExtent(str);

		if (sz.cx > dx) dx = sz.cx;
	}
	m_wndCodeName.ReleaseDC(pDC);

	if (m_wndCodeName.GetDroppedWidth() < dx)
	{
		m_wndCodeName.SetDroppedWidth(dx);
		ASSERT(m_wndCodeName.GetDroppedWidth() == dx);
	}
}

void CPsllItemDlg::Data2Dlg()
{
	m_wndLoadCase.ChangeSelect(D_LOADCASE_STATIC, m_Key);

	int nNum = m_wndCodeName.GetCount();
	int i = 0;
	for (i=0; i<nNum ; i++)
		if (m_wndCodeName.GetItemData(i) == m_Data.nCodeType) break;
	ASSERT(i != nNum);
	m_wndCodeName.SetCurSel(i);
	
	CString csVal;
	m_wndDescription.SetWindowText(m_Data.Description);
	csVal.Format(_T("%g"), m_Data.ScaleFactor_X);
	m_wndScaleGX.SetWindowText(csVal);
	csVal.Format(_T("%g"), m_Data.ScaleFactor_Y);
	m_wndScaleGY.SetWindowText(csVal);
	
	int nCode = CDlgUtil::CobxGetCurSelItemData(m_wndCodeName, i);
	m_pCodeMgr->Data2CodeDlg(m_Data, nCode, m_bModify);
	
	MakeItemEx();
}

BOOL CPsllItemDlg::Dlg2Data()
{
	UINT nLoadCaseType;
	if (!m_wndLoadCase.GetSelectedLoad(nLoadCaseType, m_Key))
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Load_case_is_not_selected));
		return FALSE;
	}

	m_Data.Initialize();

	if (!m_pCodeMgr->CodeDlg2Data(m_Data)) return FALSE;
	
	int nIndex = m_wndCodeName.GetCurSel();
	if (nIndex == CB_ERR) return FALSE;
	m_Data.nCodeType = m_wndCodeName.GetItemData(nIndex);
	
	CString csVal;
	m_wndDescription.GetWindowText(m_Data.Description);
	m_wndScaleGX.GetWindowText(csVal);
	m_Data.ScaleFactor_X = _tstof(csVal);
	m_wndScaleGY.GetWindowText(csVal);
	m_Data.ScaleFactor_Y = _tstof(csVal);

	m_pGridWnd->GetData(m_Data.arAddition);

	return TRUE;
}

void CPsllItemDlg::MakeItemEx()
{
	m_pGridWnd->SetData(m_Data.arAddition);
	m_pGridWnd->MakeItemEx();
}

