// CImportTHLoadCombItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "../wg_db/wg_db_UnitCtrl.h"
#include "../wg_db/wg_db_DataCtrl.h"
#include "../wg_db/wg_db_AttrCtrl.h"
#include "../wg_db/wg_db_FileCtrl.h"
#include "../wg_db\IUsageCounter.h"

#include "../wg_base/wg_base_DlgUtil.h"
#include "../wg_base/wg_base_ArrayUtil.h"
#include "../wg_base/wg_base_CompFunc.h"

#include "ImportTHLoadCombItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define COLCNT 3
const int nDummyRadio = -1;

/////////////////////////////////////////////////////////////////////////////
// CImportTHLoadCombItemDlg dialog
CImportTHLoadCombItemDlg::CImportTHLoadCombItemDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CImportTHLoadCombItemDlg::IDD, pParent)
{
	m_bModify = FALSE;
	m_Key = 0;
	m_Data.Initialize();
	m_nType = D_THJI_TYPE_AVG;
	//m_nOptType = D_THJI_OPTION_SELECT_LCOM;
}

void CImportTHLoadCombItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CMD_NAME_EDT, m_edtName);
	DDX_Control(pDX, IDC_CMD_DESC_EDT, m_edtDesc);
	DDX_Control(pDX, IDC_CMD_TH_CASE_LIST, m_lstTHLoadCase);
	DDX_Radio  (pDX, IDC_CMD_TYPE_AVG_RDO, m_nType);
	//DDX_Radio  (pDX, IDC_CMD_OPTION_RDO3, m_nOptType);
	//DDX_Control(pDX, IDC_CMD_OPTION1_LOADCASE_CMB, m_cmbSelected);
}

BEGIN_MESSAGE_MAP(CImportTHLoadCombItemDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_TH_IMPORT_BTN, OnButtonImport)
	ON_BN_CLICKED(IDC_CMD_TH_MODIFY_BTN, OnButtonModify)
	ON_BN_CLICKED(IDC_CMD_TH_DELETE_BTN, OnButtonDelete)
	ON_BN_CLICKED(IDC_CMD_APPLY_BTN, OnApply)
	//ON_BN_CLICKED(IDC_CMD_OPTION_RDO1, OnOptRadio)
	//ON_BN_CLICKED(IDC_CMD_OPTION_RDO2, OnOptRadio)
	//ON_BN_CLICKED(IDC_CMD_OPTION_RDO3, OnOptRadio)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportTHLoadCombItemDlg message handlers
BOOL CImportTHLoadCombItemDlg::OnInitDialog()
{
	CDialogMove::OnInitDialog();
	m_pDoc = CDBDoc::GetDocPoint();

	SetListViewCtrl();
	Data2Dlg();
	//OnOptRadio();
	
	return TRUE;
}

void CImportTHLoadCombItemDlg::SetData(T_THJI_K Key, T_THJI_D &Data)
{
	m_Key = Key;
	m_Data = Data;
}

BOOL CImportTHLoadCombItemDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_edtName.GetWindowText(m_Data.strName);
	m_edtDesc.GetWindowText(m_Data.strDesc);
	m_Data.nType = m_nType;
	m_Data.nOption = D_THJI_OPTION_SELECT_LCOM;
	m_Data.nSelectIndex = 0;

	//if ( m_nOptType == D_THJI_OPTION_SELECT_LCOM )
	//{
	//	m_Data.nSelectIndex = 0;
	//}

	return TRUE;
}

BOOL CImportTHLoadCombItemDlg::Data2Dlg()
{
	m_edtName.SetWindowText(m_Data.strName);
	m_edtDesc.SetWindowText(m_Data.strDesc);
	m_nType = m_Data.nType;

	m_lstTHLoadCase.DeleteAllItems();
	//m_cmbSelected.ResetContent();

	const int nJsonFilePathSize = m_Data.aJsonFilePath.GetSize();
	for ( int i = 0; i < nJsonFilePathSize; i++ )
	{
		std::string JsonFilePath = std::string(CT2CA(m_Data.aJsonFilePath.GetAt(i)));
		CString filename(TrimJsonFileName(JsonFilePath).c_str());
		m_lstTHLoadCase.InsertItem(i, _T(""));
		SetItem(m_lstTHLoadCase, i, JsonFilePath);
		//CDlgUtil::CobxAddItem(m_cmbSelected, filename, i);
	}
	   
	//if ( m_nOptType == D_THJI_OPTION_SELECT_LCOM )
	//{
	//	m_cmbSelected.SetCurSel(m_Data.nSelectIndex);
	//}

	UpdateData(FALSE);

	return TRUE;
}

void CImportTHLoadCombItemDlg::OnOK()
{
	if ( !AddToDB() )  return;

	CDialogMove::OnOK();
}

void CImportTHLoadCombItemDlg::OnApply()
{
	AddToDB();
}

void CImportTHLoadCombItemDlg::OnButtonImport()
{
	AfxMessageBox(_LS(IDS_CMD_IMPORT_TH_LOADCOMB_WARNING));

	CFileDialog Files(TRUE, _T(".json"), NULL,
		OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_ALLOWMULTISELECT,
		_T("Json Files(*.json)|*.json|All File(*.*)|*.*||"), NULL);

	DWORD MAXFILE = 4000;
	Files.m_ofn.nMaxFile = MAXFILE;
	TCHAR* pc = new TCHAR[MAXFILE];
	Files.m_ofn.lpstrFile = pc;
	Files.m_ofn.lpstrFile[0] = NULL;

	std::vector<std::string> vstrFilePath;
	if ( Files.DoModal() == IDOK )
	{
		POSITION pos = Files.GetStartPosition();
		while ( pos )
		{
			std::string strJsonFilePath = std::string(CT2CA(Files.GetNextPathName(pos)));
			std::string RelativePath = ConvertRelativePath(strJsonFilePath);
			vstrFilePath.push_back(RelativePath);
		}
		delete[]pc;
	}
	else
	{
		delete[]pc;
		return;
	}

	for ( auto& strFilePath : vstrFilePath )
	{
		if ( !CheckSamePathName(strFilePath) )
		{
			return;
		}
	}

	for ( auto& strFilePath : vstrFilePath )
	{
		CString csFilePath = strFilePath.c_str();
		const int nSize = m_Data.aJsonFilePath.GetSize();
		m_lstTHLoadCase.InsertItem(nSize, _T(""));
		SetItem(m_lstTHLoadCase, nSize, strFilePath);
		m_Data.aJsonFilePath.Add(csFilePath);
	}

	//ResetSelectedCombo();

	return;
}

BOOL CImportTHLoadCombItemDlg::AddToDB()
{
	if ( !Dlg2Data() ) return FALSE;
	if ( !ValidateThjiFile(m_Data) ) return FALSE;

	BOOL bSuccess = FALSE;
	if ( m_bModify )  bSuccess = m_pDoc->m_pDataCtrl->ModifyThji(m_Key, m_Key, m_Data);
	else              bSuccess = m_pDoc->m_pDataCtrl->AddThji(m_Data);

	IUsageCounter::Use(_T("SPD_THJI"));

	return bSuccess;
}

void CImportTHLoadCombItemDlg::OnButtonModify()
{
	int nIndex;
	if ( !GetSelectedIndex(nIndex) )
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_Error___No_selected_Data__nChoose_one_));
		return;
	}

	CFileDialog File(TRUE, _T(".json"), NULL,
		OFN_FILEMUSTEXIST | OFN_LONGNAMES,
		_T("Json Files(*.json)|*.json|All File(*.*)|*.*||"), NULL);
		
	std::string strJsonFilePath;
	if ( File.DoModal() == IDOK )
	{
		strJsonFilePath = ConvertRelativePath(std::string(CT2CA(File.GetPathName())));
	}
	else
	{
		return;
	}

	if ( !CheckSamePathName(strJsonFilePath) )
	{
		return;
	}

	m_Data.aJsonFilePath[nIndex] = strJsonFilePath.c_str();

	int nIndexdel = m_lstTHLoadCase.GetNextItem(-1, LVIS_FOCUSED | LVIS_SELECTED); // 선택된 첫 번째 항목 인덱스 가져오기
	while ( nIndexdel != -1 )
	{
		m_lstTHLoadCase.SetItemState(nIndexdel, 0, LVIS_SELECTED); // 선택된 항목의 상태에서 LVIS_SELECTED 비트 제거
		nIndexdel = m_lstTHLoadCase.GetNextItem(-1, LVNI_SELECTED); // 다음 선택된 항목 인덱스 가져오기
	}
	SetItem(m_lstTHLoadCase, nIndex, strJsonFilePath);
	m_lstTHLoadCase.SetItemState(nIndex, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	m_lstTHLoadCase.SetFocus();

	//ResetSelectedCombo();

	return;
}

void CImportTHLoadCombItemDlg::OnButtonDelete()
{
	if ( !m_lstTHLoadCase.GetItemCount() )
	{
		GSaveHistoryFormatNF(_LS(IDS_PUSHOVER_Error___No_selected_Data__nChoose_one_));
		return;
	}

	POSITION pos;
	while ( pos = m_lstTHLoadCase.GetFirstSelectedItemPosition() )
	{
		int nSelItem = m_lstTHLoadCase.GetNextSelectedItem(pos);
		LVITEM lvItem = { 0 };
		lvItem.iItem = nSelItem;
		lvItem.mask = ( LVIF_TEXT | LVIF_PARAM );
		m_lstTHLoadCase.GetItem(&lvItem);
		m_Data.aJsonFilePath.RemoveAt(nSelItem);
		m_lstTHLoadCase.DeleteItem(nSelItem);
	}

	//ResetSelectedCombo();

	return;
}

//void CImportTHLoadCombItemDlg::OnOptRadio()
//{
//	UpdateData(TRUE);
//
//	BOOL bEnable = m_nOptType == D_THJI_OPTION_SELECT_LCOM ? TRUE : FALSE;
//	GetDlgItem(IDC_CMD_OPTION1_LOADCASE_CMB)->EnableWindow(bEnable);
//
//	BOOL bEnableApplyOK = m_nOptType == nDummyRadio ? FALSE : TRUE;
//	GetDlgItem(IDC_CMD_APPLY_BTN)->EnableWindow(bEnableApplyOK);
//	GetDlgItem(IDOK)->EnableWindow(bEnableApplyOK);
//
//	return;
//}

void CImportTHLoadCombItemDlg::SetListViewCtrl()
{
	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_lstTHLoadCase.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_INFOTIP;
	ListView_SetExtendedListViewStyle(m_lstTHLoadCase.GetSafeHwnd(), dwStyle);

	CString aTitle[] = { _LS(IDS_TB_NAME), _LS(IDS_TB_STATUS), _LS(IDS_TB_HNGE_LOCATION) };
	CRect rtList;
	m_lstTHLoadCase.GetWindowRect(rtList);

	//::GetSystemMetrics(SM_CXVSCROLL) = 스크롤 바의 Width 길이
	const double dWidth = static_cast<double>( rtList.Width() - ::GetSystemMetrics(SM_CXVSCROLL) - 4 );
	std::array<double, COLCNT> aSizeRatio = { 0.5, 0.2, 0.3 };
	std::vector<int> vSize;
	for ( auto SizeRatio : aSizeRatio )
	{
		const int nSize = static_cast<int>( dWidth * SizeRatio );
		vSize.push_back(nSize);
	}

	CString title;
	LVCOLUMN lvcolumn;
	for ( int i=0; i<COLCNT; i++ )
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.iSubItem = i;
		lvcolumn.cx = vSize.at(i);
		title = aTitle[i];
		lvcolumn.pszText = title.GetBuffer(0);
		m_lstTHLoadCase.InsertColumn(i, &lvcolumn);
		title.ReleaseBuffer();
	}
}

void CImportTHLoadCombItemDlg::SetItem(CListCtrl& lc, int nIndex, std::string& strFilePath)
{
	LVITEM lvitem;
	CString str;
	for ( int i = 0; i < COLCNT; i++ )
	{
		lvitem.iItem = nIndex;
		lvitem.iSubItem = i;
		str = DataToStr(i, strFilePath);
		lvitem.pszText = str.GetBuffer(0);
		lvitem.mask = LVIF_TEXT;

		lc.SetItem(&lvitem);
		str.ReleaseBuffer();
	}
}

std::string CImportTHLoadCombItemDlg::TrimJsonFileName(const std::string& FilePath)
{
	const int nJson = 5; // .json 자릿수
	const int nfind = FilePath.rfind("\\") + 1;
	if ( nfind == std::string::npos )
	{
		// 경로에 해당 문자열 "\\" 이 없는 경우
		ASSERT(0);
		return std::string("ASSERT");
	}

	return FilePath.substr(nfind, FilePath.length() - nfind - nJson);
}

bool CImportTHLoadCombItemDlg::GetSelectedIndex(int &nIndex)
{
	int iItem = m_lstTHLoadCase.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if ( iItem == -1 ) return false;

	nIndex = iItem;
	return true;
}

CString CImportTHLoadCombItemDlg::DataToStr(const int nCol, std::string& strFilePath)
{
	switch ( nCol )
	{
	case Name:
		{
			CString filename(TrimJsonFileName(strFilePath).c_str());
			return filename;
		}
	case Status:
		{
			std::string JsonFilePath(strFilePath);
			CFileCtrl::JsonErrType ErrType = CFileCtrl::ValidateJsonFile(JsonFilePath);
			switch ( ErrType )
			{
			case CFileCtrl::JsonErrType::None:
				return _T("Loaded");
			case CFileCtrl::JsonErrType::InvalidPath:
				return _T("Failed(Path)");
			case CFileCtrl::JsonErrType::FileOpen:
				return _T("Failed(Open)");
			case CFileCtrl::JsonErrType::InvalidJson:
				return _T("Failed(.json)");
			default: ASSERT(0); return _T("Loaded");
			}
		}
	case Location:
		{
			std::string strLocation(strFilePath);
			std::replace(strLocation.begin(), strLocation.end(), L'\\', L'/');
			return strLocation.c_str();
		}
	default: ASSERT(0); return CString();
	}
}

bool CImportTHLoadCombItemDlg::CheckSamePathName(std::string& strFilePath)
{
	const int nSize = m_Data.aJsonFilePath.GetSize();
	for ( int i = 0; i < nSize; i++ )
	{
		CString csFilePath = strFilePath.c_str();
		if ( m_Data.aJsonFilePath[i].Compare(csFilePath) == 0 )
		{
			CString msg;
			msg.Format(_LS(IDS_PUSHOVER_Error___The_data_already_exist_));
			AfxMessageBox(msg);
			return false;
		}
	}

	return true;
}

std::string CImportTHLoadCombItemDlg::ConvertRelativePath(const std::string& JsonFilePath)
{
	CString csFullPath=_T("");
	csFullPath = CDBDoc::GetDocPoint()->GetPathName();
	std::string FilePathName = std::string(CT2CA(csFullPath));

	// Convert Relative Path
	auto RootName_Model = CFileCtrl::root_name(FilePathName);
	auto RootName_Json  = CFileCtrl::root_name(JsonFilePath);
	if ( RootName_Model != RootName_Json )
	{
		// 드라이브 경로가 다른 경우는 절대경로로 저장
		return JsonFilePath;
	}

	std::string relative_path = CFileCtrl::Relative_Path(JsonFilePath, FilePathName); // 상대경로로 변환
	return relative_path;
}

bool CImportTHLoadCombItemDlg::ValidateThjiFile(const T_THJI_D& rData)
{
	for ( int i = 0; i < rData.aJsonFilePath.GetCount(); i++ )
	{
		std::string JsonFilePath = std::string(CT2CA(rData.aJsonFilePath[i]));
		if ( CFileCtrl::ValidateJsonFile(JsonFilePath) != CFileCtrl::JsonErrType::None )
		{
			return false;
		}
	}

	return true;
}

//void CImportTHLoadCombItemDlg::ResetSelectedCombo()
//{
//	m_cmbSelected.ResetContent();
//	const int nJsonFilePathSize = m_Data.aJsonFilePath.GetSize();
//	for ( int i = 0; i < nJsonFilePathSize; i++ )
//	{
//		std::string JsonFilePath = std::string(CT2CA(m_Data.aJsonFilePath.GetAt(i)));
//		CString filename(TrimJsonFileName(JsonFilePath).c_str());
//		CDlgUtil::CobxAddItem(m_cmbSelected, filename, i);
//	}
//	m_cmbSelected.SetCurSel(0);
//
//	return;
//}

