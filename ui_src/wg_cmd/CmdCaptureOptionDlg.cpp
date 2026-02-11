// CmdCaptureOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CmdCaptureOptionDlg.h"
#include "..\wg_base\DlgUtil.h"
#include "..\wg_db\FileCtrl.h"

// 폴더 대화상자를 위한 include
#include <ShObjIdl.h> 
#include <ShlObj.h>   

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void T_CAPTURE_OPT::ReadFromReg()
{
	Init();
	nSizeType = AfxGetApp()->GetProfileInt(_T("Settings"), _T("Capture_nSizeType"), nSizeType);
	nWidth = AfxGetApp()->GetProfileInt(_T("Settings"), _T("Capture_nWidth"), nWidth);
	nHeight = AfxGetApp()->GetProfileInt(_T("Settings"), _T("Capture_nHeight"), nHeight);
	nExtension = AfxGetApp()->GetProfileInt(_T("Settings"), _T("Capture_nExtension"), nExtension);
	bSaveModelFolder = AfxGetApp()->GetProfileInt(_T("Settings"), _T("Capture_bSaveModelFolder"), bSaveModelFolder);
	strDirPath = AfxGetApp()->GetProfileString(_T("Settings"), _T("strDirPath"), strDirPath);
}

void T_CAPTURE_OPT::GetCaptureOpt(int& nWidthParm, int& nHeightParm, CString& strPathParm)
{
	switch (nSizeType)
	{
	case 0:
		nWidthParm = 1920;
		nHeightParm = 1080;
		break;
	case 1:
		nWidthParm = 1366;
		nHeightParm = 768;
		break;
	case 2:
		nWidthParm = 1600;
		nHeightParm = 900;
		break;
	case 3:
		nWidthParm = 1280;
		nHeightParm = 800;
		break;
	case 4:
		nWidthParm = 1024;
		nHeightParm = 768;
		break;
	case 5:
		nWidthParm = 800;
		nHeightParm = 600;
		break;
	case 6:
		nWidthParm = 640;
		nHeightParm = 480;
		break;
	case 7:
		nWidthParm = nWidth;
		nHeightParm = nHeight;
		break;
	default:
		nWidthParm = 1024;
		nWidthParm = 768;
		break;
	}

	CDBDoc *pDoc = CDBDoc::GetDocPoint();
	CString strModelPath = _T("");

	strModelPath = pDoc->GetOrgFilePath();
	CFileCtrl FileCtrl(strModelPath);

	CString strMakingSavePath;
	if (bSaveModelFolder)
	{	
		if (pDoc != NULL)
		{	
			strMakingSavePath = FileCtrl.GetFilePathAndNameWithoutExtension();
		}
	}
	else
	{
		strMakingSavePath = strDirPath;
		strMakingSavePath += _T("\\");
		strMakingSavePath += FileCtrl.GetFileNameWithoutExtension();		
	}
	
	strMakingSavePath += _T("_Image");

	CString strExtension, strPathCheck;
	int nCnt = 1;
	while (true)
	{		
		BOOL bExist = FALSE;
		for (int i = 0; i < 5; i++)
		{
			if (i == 0)
			{
				// emf 는 지원 안함.. emf 지원하게 되면 여기랑 CCmdCaptureOptionDlg::InitCtrl() 를 같이 수정해야 함
				continue;
			}
			strExtension = GetExtension(i);
			strPathCheck.Format(_T("%s(%d).%s"), strMakingSavePath, nCnt, strExtension);  // 확장자를 모두 고려한 이름 부여를 하게 됬음. GRenderView_NS::ExportGraphic() 의 아랫쪽에 dwg, dxf 는 서로가 이미 파일이 있으면 지우는 로직이 있는데.. 이로 인해서 사용자가 만든 이미지가 지워져서 하게 된 거임
			CFileCtrl FileCtrlExistText(strPathCheck);
			if (FileCtrlExistText.FileExists())
			{
				bExist = TRUE;
				break;
			}
		}
		if (bExist)
		{
			nCnt++;
		}
		else
		{
			break;
		}
	}

	strExtension = GetExtension(nExtension);
	strPathParm.Format(_T("%s(%d).%s"), strMakingSavePath, nCnt, strExtension);
}

CString T_CAPTURE_OPT::GetExtension(int nExtensionParam)
{
	CString strExtensionReturn;
	switch (nExtensionParam)
	{
	case 0:
		strExtensionReturn = _T("emf");
		break;
	case 1:
		strExtensionReturn = _T("bmp");
		break;
	case 2:
		strExtensionReturn = _T("jpg");
		break;
	case 3:
		strExtensionReturn = _T("dxf");
		break;
	case 4:
		strExtensionReturn = _T("dwg");
		break;
	default:
		break;
	}	return strExtensionReturn;
}

/////////////////////////////////////////////////////////////////////////////
// CCmdCaptureOptionDlg dialog
CCmdCaptureOptionDlg::CCmdCaptureOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCmdCaptureOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdCaptureOptionDlg)
	//}}AFX_DATA_INIT
	m_pDoc = CDBDoc::GetDocPoint();
}

CCmdCaptureOptionDlg::~CCmdCaptureOptionDlg()
{
}

void CCmdCaptureOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdCaptureOptionDlg)
	DDX_Control(pDX, IDC_WG_CMD_SIZE_CBX, m_cbxSize);
	DDX_Control(pDX, IDC_WG_CMD_WIDTH_EDT, m_edtWidth);
	DDX_Control(pDX, IDC_WG_CMD_HEIGHT_EDT, m_edtHeight);
	DDX_Control(pDX, IDC_WG_CMD_FILE_EXTENSION_CBX, m_cbxExtension);
	DDX_Control(pDX, IDC_WG_CMD_SAVE_MODEL_FLD_CHK, m_chkModelFolder);
	DDX_Control(pDX, IDC_WG_CMD_SAVE_FOLDER_EDT, m_edtFolder);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCmdCaptureOptionDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCmdCaptureOptionDlg)
	ON_BN_CLICKED(IDC_WG_CMD_SAVE_FOLDER_BTN, OnSaveFolderBtn )	
	ON_CBN_SELCHANGE(IDC_WG_CMD_SIZE_CBX, OnSizeCbxChange)
	ON_BN_CLICKED(IDC_WG_CMD_SAVE_MODEL_FLD_CHK, OnSaveModelFolder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdCaptureOptionDlg message handlers

BOOL CCmdCaptureOptionDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	SetData(); // 레지스트리에서 데이터를 불러온다.
	InitCtrl();
	Data2Dlg();
	EnableCtrl();

	return TRUE;
}

void CCmdCaptureOptionDlg::OnOK() 
{
	if(!Dlg2Data()) return;

	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("Capture_nSizeType"), m_Data.nSizeType);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("Capture_nWidth"), m_Data.nWidth);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("Capture_nHeight"), m_Data.nHeight);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("Capture_nExtension"), m_Data.nExtension);
	AfxGetApp()->WriteProfileInt(_T("Settings"), _T("Capture_bSaveModelFolder"), m_Data.bSaveModelFolder);
	AfxGetApp()->WriteProfileString(_T("Settings"), _T("strDirPath"), m_Data.strDirPath);

	CDialogMove::OnOK();
}

void CCmdCaptureOptionDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CCmdCaptureOptionDlg::InitCtrl()
{
	m_cbxSize.ResetContent();
	CDlgUtil::CobxAddItem(m_cbxSize, _T("1920x1080(FHD)"), 0);
	CDlgUtil::CobxAddItem(m_cbxSize, _T("1366x768(HD)"), 1);
	CDlgUtil::CobxAddItem(m_cbxSize, _T("1600x900(HD + )"), 2);
	CDlgUtil::CobxAddItem(m_cbxSize, _T("1280x800(WXGA)"), 3);
	CDlgUtil::CobxAddItem(m_cbxSize, _T("1024x768(XGA)"), 4);
	CDlgUtil::CobxAddItem(m_cbxSize, _T("800x600(SVGA)"), 5);
	CDlgUtil::CobxAddItem(m_cbxSize, _T("640x480(VGA)"), 6);
	CDlgUtil::CobxAddItem(m_cbxSize, _LS(IDS_CMD_CUSTOM), 7);
	m_cbxSize.SetCurSel(0);

	m_cbxExtension.ResetContent();
	//CDlgUtil::CobxAddItem(m_cbxExtension, _T("*.emf"), 0);  emf 를 지원하게 되면 void T_CAPTURE_OPT::GetCaptureOpt(int& nWidthParm, int& nHeightParm, CString& strPathParm) 에 있는 반복문에 i == 0 을 contine 하는 코드를 없애야 함.
	CDlgUtil::CobxAddItem(m_cbxExtension, _T("*.bmp"), 1);
	CDlgUtil::CobxAddItem(m_cbxExtension, _T("*.jpg"), 2);
	CDlgUtil::CobxAddItem(m_cbxExtension, _T("*.dxf"), 3);
	CDlgUtil::CobxAddItem(m_cbxExtension, _T("*.dwg"), 4);
	m_cbxExtension.SetCurSel(0);

	m_edtFolder.EnableFolderBrowseButton();
}

void CCmdCaptureOptionDlg::Data2Dlg()
{	
	CDlgUtil::CobxSetCurSelItemData(m_cbxSize, m_Data.nSizeType);

	CString strSetEdit;
	strSetEdit.Format(_T("%d"), m_Data.nWidth);
	m_edtWidth.SetWindowText(strSetEdit);

	strSetEdit.Format(_T("%d"), m_Data.nHeight);
	m_edtHeight.SetWindowText(strSetEdit);

	CDlgUtil::CobxSetCurSelItemData(m_cbxExtension, m_Data.nExtension);	

	m_chkModelFolder.SetCheck(m_Data.bSaveModelFolder);

	m_edtFolder.SetWindowText(m_Data.strDirPath);
}

BOOL CCmdCaptureOptionDlg::Dlg2Data()
{
	m_Data.nSizeType = CDlgUtil::CobxGetCurSelItemData(m_cbxSize, m_cbxSize.GetCurSel());

	CString strGetEdit;
	m_edtWidth.GetWindowText(strGetEdit);
	m_Data.nWidth = _wtoi(strGetEdit);

	m_edtHeight.GetWindowText(strGetEdit);
	m_Data.nHeight = _wtoi(strGetEdit);

	m_Data.nExtension = CDlgUtil::CobxGetCurSelItemData(m_cbxExtension, m_cbxExtension.GetCurSel());

	m_Data.bSaveModelFolder = m_chkModelFolder.GetCheck();

	m_edtFolder.GetWindowText(m_Data.strDirPath);

	return TRUE;
}

void CCmdCaptureOptionDlg::EnableCtrl()
{
	int nSizeType = 0;
	nSizeType = CDlgUtil::CobxGetCurSelItemData(m_cbxSize, m_cbxSize.GetCurSel());

	BOOL bSaveModelFolder;
	bSaveModelFolder = m_chkModelFolder.GetCheck();

	GetDlgItem(IDC_WG_CMD_WIDTH_STC)->EnableWindow(nSizeType == 7);
	GetDlgItem(IDC_WG_CMD_WIDTH_EDT)->EnableWindow(nSizeType == 7);

	GetDlgItem(IDC_WG_CMD_HEIGHT_STC)->EnableWindow(nSizeType == 7);
	GetDlgItem(IDC_WG_CMD_HEIGHT_EDT)->EnableWindow(nSizeType == 7);

	GetDlgItem(IDC_WG_CMD_SAVE_FOLDER_STC)->EnableWindow(!bSaveModelFolder);
	GetDlgItem(IDC_WG_CMD_SAVE_FOLDER_EDT)->EnableWindow(!bSaveModelFolder);
	GetDlgItem(IDC_WG_CMD_SAVE_FOLDER_BTN)->EnableWindow(!bSaveModelFolder);
}

void CCmdCaptureOptionDlg::SetData()
{
	// 레지스트리에서 가지고 온다
	m_Data.ReadFromReg();
}

void CCmdCaptureOptionDlg::OnSizeCbxChange()
{
	EnableCtrl();
}

void CCmdCaptureOptionDlg::OnSaveModelFolder()
{
	EnableCtrl();
}

void CCmdCaptureOptionDlg::OnSaveFolderBtn()
{
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		IFileDialog *pfd = NULL;
		hr = CoCreateInstance(CLSID_FileOpenDialog,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pfd));
		if (SUCCEEDED(hr))
		{
			DWORD dwOptions;
			hr = pfd->GetOptions(&dwOptions);
			if (SUCCEEDED(hr))
			{
				hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
			}

			if (!m_Data.strDirPath.IsEmpty())
			{
				IShellItem *psiDefaultFolder;
				hr = SHCreateItemFromParsingName(m_Data.strDirPath, NULL, IID_PPV_ARGS(&psiDefaultFolder));
				if (SUCCEEDED(hr))
				{
					pfd->SetFolder(psiDefaultFolder);
					psiDefaultFolder->Release();
				}
			}

			hr = pfd->Show(NULL);
			if (SUCCEEDED(hr))
			{
				IShellItem *psiResult = NULL;
				hr = pfd->GetResult(&psiResult);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath = NULL;
					hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					if (SUCCEEDED(hr))
					{
						m_Data.strDirPath = pszFilePath;
						CoTaskMemFree(pszFilePath);
					}
					psiResult->Release();
				}
			}
			else if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
			{
				// 사용자가 취소 버튼을 눌렀을 때의 처리를 여기에 작성
				// AfxMessageBox(_T("The operation was cancelled by the user."));
			}
			pfd->Release();
		}
		CoUninitialize();
	}

	m_edtFolder.SetWindowText(m_Data.strDirPath);
}

