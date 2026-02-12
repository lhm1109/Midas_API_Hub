// CmdAutoLoadUnderGroundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdAutoLoadUnderGroundDlg.h"
#include "CmdAutoLoadSeisFactor.h"
#include "LoadCaseUnderMainGrid.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace mit::frx;

enum SCALE_INDEX
{
	SCALE_FACT = 0,
	SCALE_NAME = 1,
};

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadUnderGroundDlg dialog

CCmdAutoLoadUnderGroundDlg::CCmdAutoLoadUnderGroundDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdAutoLoadUnderGroundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdAutoLoadUnderGroundDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dScaleFactor = 1.0;
	m_pDoc = CDBDoc::GetDocPoint();
	m_bUseSeisFactor = FALSE;
	m_Data.Initialize();
	m_pLoadGroupGrid = new CLoadCaseUnderMainGrid();
	m_strKey4CheckBoxList = _LS(IDS_DB_NONE);
}

void CCmdAutoLoadUnderGroundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdAutoLoadUnderGroundDlg)
	DDX_Text(pDX, IDC_CMD_SCALEUP_FACTOR, m_dScaleFactor);
	DDX_Control(pDX, IDC_CMD_SCALEUP_LCASE, m_ScaleFactorCmb);
	DDX_Control(pDX, IDC_CMD_SCALEUP_LIST, m_ScaleFactorList);
	DDX_Control(pDX, IDC_CMD_SEIS_FACTOR_CHK, m_wndSeisFactorChk);
	DDX_Control(pDX, IDC_CMD_LOAD_EARTH_SEIS_LIST, m_SeisList);
	DDX_Control(pDX, IDC_CMD_LOAD_EARTH_STATIC_LIST, m_StaticList);
	DDX_Control(pDX, IDC_CMD_LOAD_SEIS_GRID, *m_pLoadGroupGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdAutoLoadUnderGroundDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdAutoLoadUnderGroundDlg)
	ON_NOTIFY(NM_CLICK, IDC_CMD_SCALEUP_LIST, OnClickCmdScaleFactorList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CMD_SCALEUP_LIST, OnItemchangedCmdScaleFactorList)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_ADD,		OnCmdScaleFactorAdd)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_DEL,		OnCmdScaleFactorDel)
	ON_BN_CLICKED(IDC_CMD_SCALEUP_MODIFY,	OnCmdScaleFactorMod)
	ON_BN_CLICKED(IDC_CMD_SEIS_FACTOR_CHK,	OnCmdUnderLoadChk)
	ON_BN_CLICKED(IDC_CMD_SEIS_FACTOR_BTN,	OnCmdUnderLoadBtn)
	ON_BN_CLICKED(IDC_CMD_UNDER_LOAD_CANCEL_BTN, OnCmdCancelBtn)
	ON_BN_CLICKED(IDC_CMD_UNDER_LOAD_OK_BTN, OnCmdOkBtn)
	ON_CLBN_CHKCHANGE(IDC_CMD_LOAD_EARTH_SEIS_LIST, OnClickCmdSeismicList)
	ON_CLBN_CHKCHANGE(IDC_CMD_LOAD_EARTH_STATIC_LIST, OnClickCmdStaticList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdAutoLoadUnderGroundDlg message handlers

CCmdAutoLoadUnderGroundDlg::~CCmdAutoLoadUnderGroundDlg()
{
	if (m_pLoadGroupGrid)
	{
		delete m_pLoadGroupGrid;
		m_pLoadGroupGrid = NULL;
	}
}

BOOL CCmdAutoLoadUnderGroundDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();

	if (!m_pDoc->m_pAttrCtrl2->GetUglc(m_Data))
	{
		m_Data.Initialize();
		T_UGLC_BASE_LOAD SeisGroup; SeisGroup.Initialize();

		int nCountDefault = 0;

		// Spectrum.
		T_SPLC_D SplcD;
		CArray<T_SPLC_K, T_SPLC_K> aSplcK;
		m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
		int nCount = aSplcK.GetSize();
		for (int i = 0; i < nCount; i++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetSplc(aSplcK[i], SplcD)) { ASSERT(0); continue; }

			SeisGroup.LoadCaseK = aSplcK[i];
			SeisGroup.nLoadCaseType = D_LOADCASE_SPECTRUM;

			SeisGroup.nDir = UG_PLUS;
			m_Data.aUglcBaseLoad.Add(SeisGroup);

			SeisGroup.nDir = UG_MINUS;
			m_Data.aUglcBaseLoad.Add(SeisGroup);

			nCountDefault++;
		}

		if(nCountDefault == 0)
		{
			T_STLD_D StldD;
			T_STLD_K_LIST aStldK;
			m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
			int nStldCount = aStldK.GetSize();
			
			for (int i = 0; i < nStldCount; i++)
			{
				if(!m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD)) { ASSERT(0); continue; }
				if(StldD.LoadCaseType != _T("E")) continue;

				SeisGroup.LoadCaseK = aStldK[i];
				SeisGroup.nLoadCaseType = D_LOADCASE_STATIC;

				SeisGroup.nDir = UG_PLUS;
				m_Data.aUglcBaseLoad.Add(SeisGroup);

				SeisGroup.nDir = UG_MINUS;
				m_Data.aUglcBaseLoad.Add(SeisGroup);

				nCountDefault++;
				if(nCountDefault == 2) break;
			}
		}
	}

	InitCtrl();
	Data2Dlg();
	m_pLoadGroupGrid->CheckCurrentRecordChanged();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdAutoLoadUnderGroundDlg::InitCtrl()
{
	Initial_ScaleFactorComboBox();
	Initial_ScaleFactorListBox();
	Initial_SeisListCtrl();

	m_wndSeisFactorChk.SetCheck(m_bUseSeisFactor);
	OnCmdUnderLoadChk(); //Seis Factor

	CRect rect;
	GetDlgItem(IDC_CMD_LOAD_SEIS_GRID)->GetWindowRect(&rect);
	m_pLoadGroupGrid->Initialize(this, rect.Width());
}

void CCmdAutoLoadUnderGroundDlg::Dlg2Data()
{
	m_Data.aUglcBaseFactor.RemoveAll();
	T_UGLC_BASE_FACTOR UglcBaseFactor;
	CString strFactor = _T("");
	CString strLcName = _T("");
	int nBaseFactor = m_ScaleFactorList.GetItemCount();
	for (int i = 0; i < nBaseFactor; ++i)
	{
		strFactor = m_ScaleFactorList.GetItemText(i, SCALE_FACT);
		strLcName = m_ScaleFactorList.GetItemText(i, SCALE_NAME);

		UglcBaseFactor.dFactor = _tstof(strFactor);
		if(!m_pDoc->m_pAttrCtrl2->GetLoadCaseKeyAndType4Uglc(strLcName, UglcBaseFactor.LoadCaseK, UglcBaseFactor.nLoadCaseType)) { ASSERT(0); continue; }
		m_Data.aUglcBaseFactor.Add(UglcBaseFactor);
	}

	m_pLoadGroupGrid->GetData(m_Data);
}

void CCmdAutoLoadUnderGroundDlg::Data2Dlg()
{
	m_ScaleFactorList.DeleteAllItems();
	CString strLoadCaseName = _T("");
	int nFactorSize = m_Data.aUglcBaseFactor.GetSize();
	for (int i = 0; i < nFactorSize; ++i)
	{
		const T_UGLC_BASE_FACTOR& BaseFactor = m_Data.aUglcBaseFactor[i];
		if(!m_pDoc->m_pAttrCtrl2->GetLoadCaseName4Uglc(BaseFactor.LoadCaseK, BaseFactor.nLoadCaseType, strLoadCaseName)) { ASSERT(0); continue; }
		
		Write_ScaleFactorListBox(i, BaseFactor.dFactor, strLoadCaseName);
	}

	m_pLoadGroupGrid->ResetComboColumn();
	m_pLoadGroupGrid->SetData(m_Data);
	m_pLoadGroupGrid->Data2Grid();
}

void CCmdAutoLoadUnderGroundDlg::Initial_ScaleFactorComboBox()
{
	m_ScaleFactorCmb.ResetContent();

	CString strLoadCaseName = _T("");

	T_STLD_D StldD;
	T_STLD_K_LIST aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);
	int nStldCount = aStldK.GetSize();
	for(int i=0; i<nStldCount; ++i)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD)) { ASSERT(0); continue; }
		if(StldD.LoadCaseType != _T("E")) continue;

		if(!m_pDoc->m_pAttrCtrl2->GetLoadCaseName4Uglc(aStldK[i], D_LOADCASE_STATIC, strLoadCaseName)) { ASSERT(0); continue; }
		m_ScaleFactorCmb.AddString(strLoadCaseName);
	}

	CArray<T_SPLC_K, T_SPLC_K> aSplcK;
	m_pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcK);
	int nSplcCount = aSplcK.GetSize();
	for(int i=0; i<nSplcCount; ++i)
	{
		if(!m_pDoc->m_pAttrCtrl2->GetLoadCaseName4Uglc(aSplcK[i], D_LOADCASE_SPECTRUM, strLoadCaseName)) { ASSERT(0); continue; }
		m_ScaleFactorCmb.AddString(strLoadCaseName);
	}

	if (m_ScaleFactorCmb.GetCount() != 0)  m_ScaleFactorCmb.SetCurSel(0);
}

void CCmdAutoLoadUnderGroundDlg::Initial_ScaleFactorListBox()
{
	DWORD dwStyle = ListView_GetExtendedListViewStyle(m_ScaleFactorList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	ListView_SetExtendedListViewStyle(m_ScaleFactorList.GetSafeHwnd(), dwStyle);

	LV_COLUMN lvcolumn;
	CString aTitle[] = { _LS(IDS_CMD_AUTOLC_factor), _LS(IDS_WG_CMD__ADDD__Load_Case) };

	CString title;
	int width[2] = { 100, 100 };
	for (int i = 0; i < sizeof(aTitle) / sizeof(CString); i++)
	{
		title = aTitle[i];

		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = title.GetBuffer(0);
		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(width[i]);
		m_ScaleFactorList.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CCmdAutoLoadUnderGroundDlg::Initial_SeisListCtrl()
{
	m_SeisList.ResetContent();
	m_StaticList.ResetContent();

	T_STLD_D StldD;
	T_STLD_K_LIST aStldK;
	m_pDoc->m_pAttrCtrl->GetStldKeyList(aStldK);

	int nIndexSeis = 0;
	int nIndexStatic = 0;
	for(int i=0; i<aStldK.GetSize(); ++i)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(aStldK[i], StldD)) { ASSERT(0); continue; }
		if(StldD.LoadCaseType == _T("EEP"))
		{
			m_SeisList.AddString(StldD.LoadCaseName);
			m_SeisList.SetItemData(nIndexSeis, aStldK[i]);
			nIndexSeis++;
		}
		else if(StldD.LoadCaseType == _T("EH"))
		{
			m_StaticList.AddString(StldD.LoadCaseName);
			m_StaticList.SetItemData(nIndexStatic, aStldK[i]);
			nIndexStatic++;
		}
	}
}

void CCmdAutoLoadUnderGroundDlg::OnClickCmdScaleFactorList(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}

void CCmdAutoLoadUnderGroundDlg::OnItemchangedCmdScaleFactorList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;
	CString strFactor = m_ScaleFactorList.GetItemText(nItem, SCALE_FACT);
	CString strLcName = m_ScaleFactorList.GetItemText(nItem, SCALE_NAME);
	if (strFactor != _T("") && strLcName != _T(""))
	{
		m_dScaleFactor = _tstof(strFactor);
		int Index = m_ScaleFactorCmb.FindString(-1, strLcName);
		if (Index != -1) m_ScaleFactorCmb.SetCurSel(Index);

		UpdateData(FALSE);
	}
	*pResult = 0;
}

void CCmdAutoLoadUnderGroundDlg::Write_ScaleFactorListBox(int Index, double dFactor, CString strLoadCase)
{
	LV_ITEM lvitem;

	CString strFactor = _T("");
	strFactor.Format(_T("%10.3f"), dFactor);
	//  ListCtrl Box에 값을 입력합니다.
	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = Index;
	lvitem.iSubItem = SCALE_FACT;
	lvitem.pszText = (LPTSTR)(LPCTSTR)strFactor;
	m_ScaleFactorList.InsertItem(&lvitem);

	lvitem.mask = LVIF_TEXT;
	lvitem.iItem = Index;
	lvitem.iSubItem = SCALE_NAME;
	lvitem.pszText = (LPTSTR)(LPCTSTR)strLoadCase;
	m_ScaleFactorList.SetItem(&lvitem);
}

void CCmdAutoLoadUnderGroundDlg::OverWrite_ScaleFactorListBox(int Index, double dFactor, CString strLoadCase)
{
	CString strFactor = _T("");
	strFactor.Format(_T("%10.3f"), dFactor);
	//  ListCtrl Box에 값을 입력합니다.
	m_ScaleFactorList.SetItemText(Index, SCALE_FACT, strFactor);
	m_ScaleFactorList.SetItemText(Index, SCALE_NAME, strLoadCase);
}

BOOL CCmdAutoLoadUnderGroundDlg::ErrorCheck()
{
	BOOL bCheck = TRUE;

	if (m_dScaleFactor < 0.0 || m_dScaleFactor > 10.0)  bCheck = FALSE;
	if (!bCheck)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Scale_Up_Factor____), MB_OK); return bCheck;
	}

	if (m_dScaleFactor < 1.0 && m_dScaleFactor >= 0.0)  bCheck = FALSE;
	if (!bCheck)	AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Warning___Check_Scale_Up_Factor__), MB_OK);

	return TRUE;
}

BOOL CCmdAutoLoadUnderGroundDlg::CheckScaleFactorName(CString str, int& Index)
{
	BOOL bCheck = TRUE;
	int nCount = m_ScaleFactorList.GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		CString strLcName = m_ScaleFactorList.GetItemText(i, SCALE_NAME);
		if (str == strLcName)
		{
			Index = i;
			bCheck = FALSE;
		}
	}
	return bCheck;
}

void CCmdAutoLoadUnderGroundDlg::OnCmdScaleFactorAdd()
{
	UpdateData(TRUE);
	if (!ErrorCheck()) return;
	
	int ListNo = 0;
	int Index = m_ScaleFactorCmb.GetCurSel();
	if (Index == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Spectrum_Loa), MB_OK);
		return;
	}
	
	CString strName = _T("");
	m_ScaleFactorCmb.GetLBText(Index, strName);
	if (CheckScaleFactorName(strName, ListNo))  //  TRUE(같은 이름이 존재하지 않을 때)=Add
	{
		//  List Control Box에 작성합니다.
		int No = m_ScaleFactorList.GetItemCount();
		Write_ScaleFactorListBox(No, m_dScaleFactor, strName);
	}
	else                        //  FALSE(같은 이름이 존재할 때)=Modify or Cancel
	{
		if (AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Existed_Spectrum_Load_Case_____nM), MB_YESNO) != IDYES) return;
		
		//  List Control Box에 재작성합니다.
		OverWrite_ScaleFactorListBox(ListNo, m_dScaleFactor, strName);
		m_ScaleFactorList.SetItemState(ListNo, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}
}

void CCmdAutoLoadUnderGroundDlg::OnCmdScaleFactorDel()
{
	int nItem = m_ScaleFactorList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}

	m_ScaleFactorList.DeleteItem(nItem);

	int nCount = m_ScaleFactorList.GetItemCount();
	if (nCount <= nItem) nItem -= 1;
	if (nItem >= 0)
		m_ScaleFactorList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdAutoLoadUnderGroundDlg::OnCmdScaleFactorMod()
{
	int nItem = m_ScaleFactorList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (nItem == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Not_Selected_Data____));
		return;
	}
	
	UpdateData(TRUE);
	if (!ErrorCheck()) return;
	
	//  Spectrum Load Case Data의 순번을 얻어옵니다.
	int Index = m_ScaleFactorCmb.GetCurSel();
	if (Index == -1)
	{
		AfxMessageBox(_LS(IDS_WG_CMD__ADDD__Error___Check_Load_Case____), MB_OK);
		return;
	}
	
	CString strName = _T("");
	m_ScaleFactorCmb.GetLBText(Index, strName);
	//  List Control Box에 재작성합니다.
	OverWrite_ScaleFactorListBox(nItem, m_dScaleFactor, strName);

	m_ScaleFactorList.SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
}

void CCmdAutoLoadUnderGroundDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	HWND hWnd = GetSafeHwnd();
	if (!hWnd || !IsWindow(hWnd)) return;

	switch (lHint)
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
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CCmdAutoLoadUnderGroundDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	m_pLoadGroupGrid->GetData(m_Data);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	UINT Key = 0;
	T_STLD_D StldD;
	T_SPLC_D SplcD;
		
	BOOL bUpdateStld = FALSE;
	BOOL bUpdateSplc = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;

		switch (nCmd)
		{
		case(UR_STLD_ADD):
			bUpdateStld = TRUE;
			break;
		case UR_STLD_MFD:
			{
				pViewBuff->GetStld(nKey, Key, StldD);

				for (int i = m_Data.aUglcBaseFactor.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseFactor[i].nLoadCaseType != D_LOADCASE_STATIC) continue;
					if(m_Data.aUglcBaseFactor[i].LoadCaseK != Key) continue;
					if(StldD.LoadCaseType == _T("E")) continue;

					m_Data.aUglcBaseFactor.RemoveAt(i);
				}

				for (int i = m_Data.aUglcBaseLoad.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseLoad[i].nLoadCaseType == D_LOADCASE_STATIC && m_Data.aUglcBaseLoad[i].LoadCaseK == Key)
					{
						if(StldD.LoadCaseType != _T("E"))
						{
							m_Data.aUglcBaseLoad.RemoveAt(i);
							continue;
						}
					}

					int nSize = m_Data.aUglcBaseLoad[i].aSeisStldK.GetSize();
					for (int j = nSize-1; j >= 0; j--)
					{
						if( m_Data.aUglcBaseLoad[i].aSeisStldK[j] != Key) continue;
						if(StldD.LoadCaseType == _T("EEP")) continue;
						m_Data.aUglcBaseLoad[i].aSeisStldK.RemoveAt(j);
					}

					nSize = m_Data.aUglcBaseLoad[i].aStacStldK.GetSize();
					for (int j = nSize-1; j >= 0; j--)
					{
						if( m_Data.aUglcBaseLoad[i].aStacStldK[j] != Key) continue;
						if(StldD.LoadCaseType == _T("EH")) continue;
						m_Data.aUglcBaseLoad[i].aStacStldK.RemoveAt(j);
					}
				}

				bUpdateStld = TRUE;
			}
			break;
		case(UR_STLD_DEL):
			{
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetStld(nKey, Key, StldD);

				for (int i = m_Data.aUglcBaseFactor.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseFactor[i].nLoadCaseType != D_LOADCASE_STATIC) continue;
					if(m_Data.aUglcBaseFactor[i].LoadCaseK != Key) continue;

					m_Data.aUglcBaseFactor.RemoveAt(i);
				}

				for (int i = m_Data.aUglcBaseLoad.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseLoad[i].nLoadCaseType == D_LOADCASE_STATIC && m_Data.aUglcBaseLoad[i].LoadCaseK == Key)
					{
						m_Data.aUglcBaseLoad.RemoveAt(i);
						continue;
					}

					int nSize = m_Data.aUglcBaseLoad[i].aSeisStldK.GetSize();
					for (int j = nSize-1; j >= 0; j--)
					{
						if( m_Data.aUglcBaseLoad[i].aSeisStldK[j] != Key) continue;
						m_Data.aUglcBaseLoad[i].aSeisStldK.RemoveAt(j);
					}

					nSize = m_Data.aUglcBaseLoad[i].aStacStldK.GetSize();
					for (int j = nSize-1; j >= 0; j--)
					{
						if( m_Data.aUglcBaseLoad[i].aStacStldK[j] != Key) continue;
						m_Data.aUglcBaseLoad[i].aStacStldK.RemoveAt(j);
					}
				}

				bUpdateStld = TRUE;
			}
			break;
		case(UR_SPLC_ADD):
			bUpdateSplc = TRUE;
			break;
		case UR_SPLC_MFD:
			{
				pViewBuff->GetSplc(nKey, Key, SplcD);

				for (int i = m_Data.aUglcBaseFactor.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseFactor[i].nLoadCaseType != D_LOADCASE_SPECTRUM) continue;
					if(m_Data.aUglcBaseFactor[i].LoadCaseK != Key) continue;

					m_Data.aUglcBaseFactor.RemoveAt(i);
				}

				for (int i = m_Data.aUglcBaseLoad.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseLoad[i].nLoadCaseType != D_LOADCASE_SPECTRUM) continue;
					if(m_Data.aUglcBaseLoad[i].LoadCaseK != Key) continue;

					m_Data.aUglcBaseLoad.RemoveAt(i);
				}

				bUpdateSplc = TRUE;
			}
			break;
		case(UR_SPLC_DEL):
			{
				// 현재 loadcase list에서 지워진 것 삭제한다.
				pViewBuff->GetSplc(nKey, Key, SplcD);

				for (int i = m_Data.aUglcBaseFactor.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseFactor[i].nLoadCaseType != D_LOADCASE_SPECTRUM) continue;
					if(m_Data.aUglcBaseFactor[i].LoadCaseK != Key) continue;

					m_Data.aUglcBaseFactor.RemoveAt(i);
				}

				for (int i = m_Data.aUglcBaseLoad.GetSize() - 1; i >= 0; i--)
				{
					if(m_Data.aUglcBaseLoad[i].nLoadCaseType != D_LOADCASE_SPECTRUM) continue;
					if(m_Data.aUglcBaseLoad[i].LoadCaseK != Key) continue;

					m_Data.aUglcBaseLoad.RemoveAt(i);
				}

				bUpdateSplc = TRUE;
			}
			break;
		default:
			break;
		}
	} // end of while

	if(bUpdateStld || bUpdateSplc)
	{
		Initial_ScaleFactorComboBox();
		Initial_SeisListCtrl();
		Data2Dlg();

		if(bUpdateStld)
		{
			int nIndex = _ttoi(m_strKey4CheckBoxList)-1;
			if(nIndex >=0 && nIndex < m_Data.aUglcBaseLoad.GetSize())
			{
				SetData_CheckBoxList(m_strKey4CheckBoxList, m_Data.aUglcBaseLoad[nIndex].aSeisStldK, m_Data.aUglcBaseLoad[nIndex].aStacStldK);
			}
			else
			{
				SetAllCheck_CheckBoxList(0);
			}
		}
	}
}

void CCmdAutoLoadUnderGroundDlg::OnCmdUnderLoadChk()
{
	BOOL bSeisFactor = m_wndSeisFactorChk.GetCheck();

	GetDlgItem(IDC_CMD_SEIS_FACTOR_BTN)->EnableWindow(bSeisFactor);
	m_bUseSeisFactor = bSeisFactor;
}

void CCmdAutoLoadUnderGroundDlg::OnCmdUnderLoadBtn()
{
	CCmdAutoLoadSeisFactor dlg;
	dlg.DoModal();
}

void CCmdAutoLoadUnderGroundDlg::OnCmdCancelBtn()
{
	OnCancel();
}

void CCmdAutoLoadUnderGroundDlg::OnCmdOkBtn()
{
	UpdateData(TRUE);
	Dlg2Data();
	//////////////////////////////////////////////////////////////
	if (m_pDoc->m_pDataCtrl->AddUglc(m_Data))
	{
		CDialogMove::OnOK();
	}
}

void CCmdAutoLoadUnderGroundDlg::OnClickCmdSeismicList()
{
	if (m_strKey4CheckBoxList == _LS(IDS_DB_NONE) || m_strKey4CheckBoxList == _ULS(Append Row) || m_strKey4CheckBoxList == _ULS(New Row))
	{
		return;
	}
	m_pLoadGroupGrid->GetData(m_Data);

	int nIndex = _ttoi(m_strKey4CheckBoxList)-1;
	if(nIndex < 0) { ASSERT(0); return; }
	if(nIndex >= m_Data.aUglcBaseLoad.GetSize()) { ASSERT(0); return; }

	
	GetData_CheckBoxList(&m_SeisList, m_Data.aUglcBaseLoad[nIndex].aSeisStldK);
	m_pLoadGroupGrid->SetData(m_Data);
}

void CCmdAutoLoadUnderGroundDlg::OnClickCmdStaticList()
{
	if (m_strKey4CheckBoxList == _LS(IDS_DB_NONE) || m_strKey4CheckBoxList == _ULS(Append Row) || m_strKey4CheckBoxList == _ULS(New Row))
	{
		return;
	}
	m_pLoadGroupGrid->GetData(m_Data);

	int nIndex = _ttoi(m_strKey4CheckBoxList)-1;
	if(nIndex < 0) { ASSERT(0); return; }
	if(nIndex >= m_Data.aUglcBaseLoad.GetSize()) { ASSERT(0); return; }

	
	GetData_CheckBoxList(&m_StaticList, m_Data.aUglcBaseLoad[nIndex].aStacStldK);
	m_pLoadGroupGrid->SetData(m_Data);
}

void CCmdAutoLoadUnderGroundDlg::SetAllCheck_CheckBoxList(int nCheck)
{
	int nSeis = m_SeisList.GetCount();
	for(int i = 0; i<nSeis; i++)
	{
		m_SeisList.SetCheck(i, nCheck);
	}

	int nStatic = m_StaticList.GetCount();
	for(int i = 0; i<nStatic; i++)
	{
		m_StaticList.SetCheck(i, nCheck);
	}
}

void CCmdAutoLoadUnderGroundDlg::SetData_CheckBoxList(const CString& sKey, const T_STLD_K_LIST& aSeisStldK, const T_STLD_K_LIST& aStacStldK)
{
	m_strKey4CheckBoxList = sKey;

	SetAllCheck_CheckBoxList(0);
	if (m_strKey4CheckBoxList == _ULS(Append Row) || m_strKey4CheckBoxList == _ULS(New Row))
	{
		return;
	}	

	int i = 0;
	int nIndex = 0;
	T_STLD_D StldD;

	for(i=0; i<aSeisStldK.GetSize(); ++i)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(aSeisStldK[i], StldD)) { ASSERT(0); continue; }
		nIndex = m_SeisList.FindString(0, StldD.LoadCaseName);
		if(nIndex<0) continue;
		m_SeisList.SetCheck(nIndex,1);
	}

	for(i=0; i<aStacStldK.GetSize(); ++i)
	{
		if(!m_pDoc->m_pAttrCtrl->GetStld(aStacStldK[i], StldD)) { ASSERT(0); continue; }
		nIndex = m_StaticList.FindString(0, StldD.LoadCaseName);
		if(nIndex<0) continue;
		m_StaticList.SetCheck(nIndex,1);
	}
}

void CCmdAutoLoadUnderGroundDlg::GetData_CheckBoxList(T_STLD_K_LIST& aSeisStldK, T_STLD_K_LIST& aStacStldK, BOOL bAll)
{
	GetData_CheckBoxList(&m_SeisList, aSeisStldK, bAll);
	GetData_CheckBoxList(&m_StaticList, aStacStldK, bAll);
}

void CCmdAutoLoadUnderGroundDlg::GetData_CheckBoxList(MCheckListBox* CheckList, T_STLD_K_LIST& aStldK, BOOL bAll)
{
	aStldK.RemoveAll();
	T_STLD_K StldK = 0;

	int nSeis = CheckList->GetCount();
	for(int i = 0; i<nSeis; i++)
	{
		if(!bAll && CheckList->GetCheck(i)==0) continue;
		StldK = (T_STLD_K)CheckList->GetItemData(i);
		aStldK.Add(StldK);
	}
}