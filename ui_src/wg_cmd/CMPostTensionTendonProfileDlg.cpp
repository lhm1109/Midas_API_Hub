#include "stdafx.h"
#include "wg_cmd.h"

#include "CMPostTensionTendonProfileDlg.h"

#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"

#include "..\wg_common\wg_common_TBBrowserWndEx.h"  // ID_USER_ATTR_COL_ID 등을 위해
#include "..\wg_common\wg_commonres.h"  // IDS_CTRL_TENDON_SHAPE_IMAGE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CDialog CDialogMove

/////////////////////////////////////////////////////////////////////////////
// CCMTendonShapeImageControl implementation

CCMTendonShapeImageControl::CCMTendonShapeImageControl(CGXGridCore* pGrid)
	: CGXControl(pGrid)
{
	m_bInitialized = FALSE;
	InitializeBitmaps();
}

CCMTendonShapeImageControl::~CCMTendonShapeImageControl()
{
	// First
	m_bmFirstPara1.DeleteObject();
	m_bmFirstPara2.DeleteObject();
	m_bmFirstPara3.DeleteObject();
	m_bmFirstStr1.DeleteObject();
	m_bmFirstStr2.DeleteObject();
	// Middle
	m_bmMiddlePara1.DeleteObject();
	m_bmMiddlePara2.DeleteObject();
	m_bmMiddlePara3.DeleteObject();
	m_bmMiddleStr1.DeleteObject();
	m_bmMiddleStr2.DeleteObject();
	m_bmMiddleStr3.DeleteObject();
	// Last
	m_bmLastPara1.DeleteObject();
	m_bmLastPara2.DeleteObject();
	m_bmLastPara3.DeleteObject();
	m_bmLastStr1.DeleteObject();
	m_bmLastStr2.DeleteObject();
}

void CCMTendonShapeImageControl::InitializeBitmaps()
{
	if (m_bInitialized)
		return;

	// 리소스 핸들을 찾아서 비트맵 로드
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_FIRST_PARA1), RT_BITMAP);

	HBITMAP hBmp = NULL;

	// First position bitmaps
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_FIRST_PARA1));
	if (hBmp) m_bmFirstPara1.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_FIRST_PARA2));
	if (hBmp) m_bmFirstPara2.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_FIRST_PARA3));
	if (hBmp) m_bmFirstPara3.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_FIRST_STR1));
	if (hBmp) m_bmFirstStr1.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_FIRST_STR2));
	if (hBmp) m_bmFirstStr2.Attach(hBmp);

	// Middle position bitmaps
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_MIDDLE_PARA1));
	if (hBmp) m_bmMiddlePara1.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_MIDDLE_PARA2));
	if (hBmp) m_bmMiddlePara2.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_MIDDLE_PARA3));
	if (hBmp) m_bmMiddlePara3.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_MIDDLE_STR1));
	if (hBmp) m_bmMiddleStr1.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_MIDDLE_STR2));
	if (hBmp) m_bmMiddleStr2.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_MIDDLE_STR3));
	if (hBmp) m_bmMiddleStr3.Attach(hBmp);

	// Last position bitmaps
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_LAST_PARA1));
	if (hBmp) m_bmLastPara1.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_LAST_PARA2));
	if (hBmp) m_bmLastPara2.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_LAST_PARA3));
	if (hBmp) m_bmLastPara3.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_LAST_STR1));
	if (hBmp) m_bmLastStr1.Attach(hBmp);
	hBmp = ::LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CMD_TENDON_SHAPE_LAST_STR2));
	if (hBmp) m_bmLastStr2.Attach(hBmp);

	m_bInitialized = TRUE;
}

CBitmap* CCMTendonShapeImageControl::GetBitmapForShape(const CString& strBoundary, int nShapeType)
{
	if (strBoundary == _T("First"))
	{
		switch (nShapeType)
		{
		case 1: return &m_bmFirstPara1;
		case 2: return &m_bmFirstPara2;
		case 3: return &m_bmFirstPara3;
		case 4: return &m_bmFirstStr1;
		case 5: return &m_bmFirstStr2;
		default: return nullptr;
		}
	}
	else if (strBoundary == _T("Middle"))
	{
		switch (nShapeType)
		{
		case 1: return &m_bmMiddlePara1;
		case 2: return &m_bmMiddlePara2;
		case 3: return &m_bmMiddlePara3;
		case 4: return &m_bmMiddleStr1;
		case 5: return &m_bmMiddleStr2;
		case 6: return &m_bmMiddleStr3;
		default: return nullptr;
		}
	}
	else if (strBoundary == _T("Last"))
	{
		switch (nShapeType)
		{
		case 1: return &m_bmLastPara1;
		case 2: return &m_bmLastPara2;
		case 3: return &m_bmLastPara3;
		case 4: return &m_bmLastStr1;
		case 5: return &m_bmLastStr2;
		default: return nullptr;
		}
	}
	return nullptr;
}

void CCMTendonShapeImageControl::Draw(CDC* pDC, CRect rect, ROWCOL nRow, ROWCOL nCol,
                                       const CGXStyle& style, const CGXStyle* pStandardStyle)
{
	// 인쇄 시에는 비트맵 표시 안함
	if (Grid()->IsPrinting())
		return;

	// 헤더 행은 건너뛰기
	if (nRow == 0)
		return;

	// 배경 그리기
	DrawBackground(pDC, rect, style);

	// Column 1(Boundary Condition)에서 값을 읽어서 위치 결정
	CString strBoundary = Grid()->GetValueRowCol(nRow, 1);

	// Column 2(Shape)에서 값을 읽어서 Shape 타입 결정
	CString strShape = Grid()->GetValueRowCol(nRow, 2);
	int nShapeType = 0;

	// Shape 문자열을 타입 번호로 변환
	if (strShape.Find(_T("Parabolic Type1")) >= 0) nShapeType = 1;
	else if (strShape.Find(_T("Parabolic Type2")) >= 0) nShapeType = 2;
	else if (strShape.Find(_T("Parabolic Type3")) >= 0) nShapeType = 3;
	else if (strShape.Find(_T("Straight Type1")) >= 0) nShapeType = 4;
	else if (strShape.Find(_T("Straight Type2")) >= 0) nShapeType = 5;
	else if (strShape.Find(_T("Straight Type3")) >= 0) nShapeType = 6;

	CBitmap* pBitmap = GetBitmapForShape(strBoundary, nShapeType);

	if (pBitmap && pBitmap->m_hObject != NULL)
	{
		// 비트맵 크기 가져오기
		BITMAP bm;
		pBitmap->GetObject(sizeof(BITMAP), (LPTSTR)&bm);

		CPoint ptSize;
		ptSize.x = bm.bmWidth;
		ptSize.y = bm.bmHeight;

		// 셀 크기가 충분하면 비트맵 중앙에 그리기
		if (rect.Width() >= ptSize.x && rect.Height() >= ptSize.y)
		{
			ASSERT(pBitmap->m_hObject != NULL);

			int x = rect.left + max(1, (rect.Width() - ptSize.x) / 2);
			int y = rect.top + max(1, (rect.Height() - ptSize.y) / 2);

			// 투명 비트맵 그리기 (회색 배경을 투명으로)
			GXDrawTransparentBitmap(pDC,
				pBitmap,
				x,
				y,
				RGB(192, 192, 192));
		}
	}

	CGXControl::Draw(pDC, rect, nRow, nCol, style, pStandardStyle);
}

/////////////////////////////////////////////////////////////////////////////
// CCMPostTensionTendonProfileDlg dialog

CCMPostTensionTendonProfileDlg::CCMPostTensionTendonProfileDlg(CWnd* pParent)
	: CDialogMove(CCMPostTensionTendonProfileDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_strName = _T("");
	m_pTableCustom = nullptr;
	m_Data.Initialize();
}

CCMPostTensionTendonProfileDlg::~CCMPostTensionTendonProfileDlg()
{
	m_Bitmap.DeleteObject();
	
	if (m_pTableCustom)
	{
		delete m_pTableCustom;
		m_pTableCustom = nullptr;
	}
}

void CCMPostTensionTendonProfileDlg::OnGridCellChanged(int nRow, int nCol)
{


	return;
}

void CCMPostTensionTendonProfileDlg::OnShapeSelectionChanged(int nRow, const CString& strShape)
{
	if (m_pTableCustom && m_pTableCustom->GetSafeHwnd())
	{
		// 그리드의 특정 셀 값을 업데이트
		//m_pTableCustom->SetItemText(nRow, ID_USER_ATTR_COL_ID, strShape);
	}
}

int CCMPostTensionTendonProfileDlg::GetElementType()
{
	int nCurSel = m_comboElemType.GetCurSel();

	return nCurSel;
}

int CCMPostTensionTendonProfileDlg::GetElementType(CString& strElemType)
{
	if (strElemType == m_pDoc->m_pAttrCtrl->GetEltypName(BEAM_EL))
	{
		m_comboElemType.SetCurSel(0);
		return EN_ELEM_BEAM;
	}
	else if (strElemType == m_pDoc->m_pAttrCtrl->GetEltypName(BDSSLAB_EL))
	{
		m_comboElemType.SetCurSel(1);
		return EN_ELEM_SLAB;
	}
	else if (strElemType == m_pDoc->m_pAttrCtrl->GetEltypName(WALL_EL))
	{
		m_comboElemType.SetCurSel(2);
		return EN_ELEM_WALL;
	}

	ASSERT(0); return 0;
}

void CCMPostTensionTendonProfileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CMD_PT_TDN_PROFILE_ELEM_CMB, m_comboElemType);
	DDX_Control(pDX, IDC_CMD_PT_TDN_PROFILE_LIST, m_listProfile);
	
	if (GetDlgItem(IDC_CMD_PT_TDN_PROFILE_NAME_EDT))
	{
		DDX_Control(pDX, IDC_CMD_PT_TDN_PROFILE_NAME_EDT, m_editName);
		DDX_Text(pDX, IDC_CMD_PT_TDN_PROFILE_NAME_EDT, m_strName);
	}
	
	if (GetDlgItem(IDC_CMD_PT_TDN_PROFILE_TYPE_PIC))
	{
		DDX_Control(pDX, IDC_CMD_PT_TDN_PROFILE_TYPE_PIC, m_wndBmp);
	}
}

BEGIN_MESSAGE_MAP(CCMPostTensionTendonProfileDlg, CDialogMove)
	ON_BN_CLICKED(IDC_CMD_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_CMD_ADD_BTN, OnAddBtn)
	ON_BN_CLICKED(IDC_CMD_MOD_BTN, OnModifyBtn)
	ON_BN_CLICKED(IDC_CMD_DEL_BTN, OnDeleteBtn)
	ON_CBN_SELCHANGE(IDC_CMD_PT_TDN_PROFILE_ELEM_CMB, OnSelChangeElemType)
	ON_NOTIFY(NM_CLICK, IDC_CMD_PT_TDN_PROFILE_LIST, OnItemClickProfileList)
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER + 100, OnDelayedResize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMPostTensionTendonProfileDlg message handlers

BOOL CCMPostTensionTendonProfileDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitializeControls();
	SetProfileTypeImage();

	CreateProfileGrid();
	InitializeProfileGridData();

	Data2Dlg();
	
	return TRUE;
}

void CCMPostTensionTendonProfileDlg::SetProfileTypeImage()
{
	m_Bitmap.DeleteObject();

	int nBitmapID = IDB_CMD_TENDON_PROFILE_TYP_SLAB;
	int nCurSel = m_comboElemType.GetCurSel();
	
	switch (nCurSel)
	{
	case EN_ELEM_BEAM: nBitmapID = IDB_CMD_TENDON_PROFILE_TYP_BEAM; break;
	case EN_ELEM_SLAB: nBitmapID = IDB_CMD_TENDON_PROFILE_TYP_SLAB; break;
	case EN_ELEM_WALL: nBitmapID = IDB_CMD_TENDON_PROFILE_TYP_WALL; break;
	}

	if (m_Bitmap.LoadBitmap(nBitmapID))
	{
		m_wndBmp.SetBitmap(HBITMAP(m_Bitmap.Detach()));
	}
}

void CCMPostTensionTendonProfileDlg::InitializeControls()
{
	if (m_comboElemType.GetSafeHwnd())
	{
		m_comboElemType.AddString(m_pDoc->m_pAttrCtrl->GetEltypName(BEAM_EL));
		m_comboElemType.AddString(m_pDoc->m_pAttrCtrl->GetEltypName(BDSSLAB_EL));
		m_comboElemType.AddString(m_pDoc->m_pAttrCtrl->GetEltypName(WALL_EL));
		m_comboElemType.SetCurSel(0);
	}

	if (m_listProfile.GetSafeHwnd())
	{
		CStringArray HTitles;
		CArray<float, float> HRatio;
		
		HTitles.Add(_T("ID"));		HRatio.Add(0.2f);
		HTitles.Add(_T("Name"));	HRatio.Add(0.4f);
		HTitles.Add(_T("Type"));	HRatio.Add(0.4f);
		
		CDlgUtil::_SetListCtrlHeader(&m_listProfile, HTitles, &HRatio, NULL);
		m_listProfile.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	}
}

void CCMPostTensionTendonProfileDlg::InitializeProfileGridData()
{
	if (m_pTableCustom && m_pTableCustom->GetSafeHwnd())
	{
		// CTBBrowserWndEx 패턴에 맞게 초기화
		m_pTableCustom->Initialize(m_pDoc);

		// Grid가 보이도록 명시적으로 설정
		m_pTableCustom->ShowWindow(SW_SHOW);
		m_pTableCustom->UpdateWindow();
	}
}

void CCMPostTensionTendonProfileDlg::CreateProfileGrid()
{
	// Grid 생성
	CRect rect;
	GetDlgItem(IDC_CMD_PT_TDN_PROFILE_TABLE_CST)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_pTableCustom = new CCMPostTensionTendonProfileGrid();
	m_pTableCustom->SetParentDlg(this);

	// Create 전에 윈도우 스타일 설정
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP;
	m_pTableCustom->Create(dwStyle, rect, this, IDC_CMD_PT_TDN_PROFILE_TABLE_CST);
}

void CCMPostTensionTendonProfileDlg::UpdateProfileGridData()
{
	if (!m_pTableCustom || !m_pTableCustom->GetSafeHwnd()) return;

	// Grid 데이터 업데이트
	m_pTableCustom->UpdateProfileGridData();
}

void CCMPostTensionTendonProfileDlg::UpdateProfileList()
{
	InitTendonProfileList();

	CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
	m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);

	T_TDPF_D TdpfD;
	for (auto i = 0; i < aTdpfK.GetSize(); i++)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetTdpf(aTdpfK[i], TdpfD))
		{
			continue;
		}

		AddTendonProfileList(aTdpfK[i], TdpfD.strName, TdpfD.nElemType);
	}
}

void CCMPostTensionTendonProfileDlg::InitTendonProfileList()
{
	if (m_listProfile.GetSafeHwnd())
	{
		m_listProfile.DeleteAllItems();
	}
}

void CCMPostTensionTendonProfileDlg::AddTendonProfileList(const UINT& nID, const CString& strName, const int& nType)
{
	if (!m_listProfile.GetSafeHwnd()) return;

	CString strType;
	switch (nType)
	{
	case EN_ELEM_BEAM: // Beam
		strType = m_pDoc->m_pAttrCtrl->GetEltypName(BEAM_EL);
		break;
	case EN_ELEM_SLAB: // Slab
		strType = m_pDoc->m_pAttrCtrl->GetEltypName(BDSSLAB_EL);
		break;
	case EN_ELEM_WALL: // Wall
		strType = m_pDoc->m_pAttrCtrl->GetEltypName(WALL_EL);
		break;
	default:
		ASSERT(0); return;
	}

	int nIndex = m_listProfile.GetItemCount();

	CString strID;
	strID.Format(_T("%d"), nID);
	nIndex = m_listProfile.InsertItem(nIndex, strID);
	m_listProfile.SetItemText(nIndex, 1, strName);
	m_listProfile.SetItemText(nIndex, 2, strType);
}

int CCMPostTensionTendonProfileDlg::GetSelectedProfileKey()
{
	int nSelectedIndex = m_listProfile.GetNextItem(-1, LVNI_SELECTED);
	if (nSelectedIndex == -1)
		return -1;

	CString strID = m_listProfile.GetItemText(nSelectedIndex, 0);

	return _ttoi(strID);
}

void CCMPostTensionTendonProfileDlg::Data2Dlg()
{
	UpdateProfileList();
	UpdateProfileGridData();

	UpdateData(FALSE);
}

void CCMPostTensionTendonProfileDlg::Dlg2Data()
{
	UpdateData(TRUE);

	m_Data.strName = m_strName;
	m_Data.nElemType = GetElementType();

	if ( m_pTableCustom && m_pTableCustom->GetSafeHwnd() )
	{
		m_pTableCustom->RetrieveGridData(m_Data);
	}
}

void CCMPostTensionTendonProfileDlg::OnClose()
{
	CDialog::OnCancel();
}

void CCMPostTensionTendonProfileDlg::OnSelChangeElemType()
{
	Data2Dlg();
	SetProfileTypeImage();
	m_pTableCustom->UpdateColumnHeader();
	m_pTableCustom->UpdateGridDefaultItem();
}

void CCMPostTensionTendonProfileDlg::OnItemClickProfileList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem >= 0 && m_editName.GetSafeHwnd())
	{
		CString strName = m_listProfile.GetItemText(pNMLV->iItem, 1);
		m_editName.SetWindowText(strName);

		// DB에서 실제 nElemType을 로드하여 UI 설정
		CString strID = m_listProfile.GetItemText(pNMLV->iItem, 0);
		T_TDPF_K TdpfK = _ttoi(strID);
		T_TDPF_D TdpfD;
		if (m_pDoc->m_pAttrCtrl2->GetTdpf(TdpfK, TdpfD))
		{
			m_comboElemType.SetCurSel(TdpfD.nElemType);
		}

		UpdateProfileGridData();
	}

	*pResult = 0;
}

void CCMPostTensionTendonProfileDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogMove::OnSize(nType, cx, cy);
}

LRESULT CCMPostTensionTendonProfileDlg::OnDelayedResize(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CCMPostTensionTendonProfileDlg::OnAddBtn()
{
	Dlg2Data();

	if (!m_pDoc->m_pDataCtrl->AddTdpf(m_Data))
	{
		ASSERT(0); return;
	}

	Data2Dlg();
}

void CCMPostTensionTendonProfileDlg::OnModifyBtn()
{
	T_TDPF_K TdpfK;
	T_TDPF_D TdpfD;
	TdpfD.Initialize();

	auto nSelectedID = GetSelectedProfileKey();
	if (nSelectedID < 0)
	{
		ASSERT(0); return;
	}

	TdpfK = nSelectedID;

	// 기존 데이터를 로드하여 nElemType 보존
	T_TDPF_D TdpfDOld;
	if (m_pDoc->m_pAttrCtrl2->GetTdpf(TdpfK, TdpfDOld))
	{
		// Dlg2Data()를 호출하기 전에 원래 nElemType 저장
		int nOriginalElemType = TdpfDOld.nElemType;

		Dlg2Data();

		// nElemType을 원래 값으로 복원
		m_Data.nElemType = nOriginalElemType;
	}
	else
	{
		Dlg2Data();
	}

	TdpfD = m_Data;
	if (!m_pDoc->m_pDataCtrl->ModifyTdpf(TdpfK, TdpfK, TdpfD))
	{
		ASSERT(0); return;
	}

	Data2Dlg();
}

void CCMPostTensionTendonProfileDlg::OnDeleteBtn()
{
	auto nSelectedID = GetSelectedProfileKey();
	if (nSelectedID < 0)
	{
		ASSERT(0); return;
	}

	T_TDPF_K TdpfK = nSelectedID;

	if (!m_pDoc->m_pDataCtrl->DelTdpf(TdpfK))
	{
		ASSERT(0); return;
	}

	Data2Dlg();
}

// Grid 클래스 구현
CCMPostTensionTendonProfileGrid::CCMPostTensionTendonProfileGrid()
{
	m_pParentDlg = nullptr;
	m_pDoc = nullptr;
	SetDefaultValues();
}

CCMPostTensionTendonProfileGrid::~CCMPostTensionTendonProfileGrid()
{
}

BEGIN_MESSAGE_MAP(CCMPostTensionTendonProfileGrid, CTBBrowserWndEx)
	//{{AFX_MSG_MAP(CCMPostTensionTendonProfileGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCMPostTensionTendonProfileGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWndEx::Initialize();

	m_pDoc = pDoc;

	SetDefaultValues();

	LockUpdate(TRUE);

	m_csTableName = _T("PostTensionTendonProfile");

	SetColCount(11);
	int nRowCount;
	auto nCurElem = m_pParentDlg->GetElementType();
	switch (nCurElem)
	{
	case EN_ELEM_BEAM:
		nRowCount = 3; break;
	case EN_ELEM_SLAB:
		nRowCount = 3; break;
	case EN_ELEM_WALL:
		nRowCount = 3; break;
	}
	SetRecordCount(nRowCount);

	EnableAppendRow(FALSE);

	GetParam()->EnableUndo(FALSE);
	EnableScrollTips();
	EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE);

	m_aTitle.RemoveAll();
	m_aType.RemoveAll();
	m_aUnit.RemoveAll();
	
	for (int i = 0; i < 11; i++)
	{
		m_aTitle.Add(m_aColumnTitles[i]);
		m_aType.Add(m_aColumnTypes[i]);
		m_aUnit.Add(m_aColumnUnits[i]);
	}
	
	SetHeaderTitle();
	
	ChangeStandardStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetSize(9))
		.SetVerticalAlignment(DT_VCENTER)
		.SetHorizontalAlignment(DT_CENTER)
		.SetAllowEnter(FALSE));
	
	ChangeColHeaderStyle(CGXStyle().SetFont(CGXFont_GC().SetBold(FALSE)));
	
	ChangeRowHeaderStyle(CGXStyle()
		.SetFont(CGXFont_GC().SetBold(FALSE))
		.SetHorizontalAlignment(DT_RIGHT)
		.SetControl(GX_IDS_CTRL_HEADER));
	GetParam()->SetNumberedRowHeaders(TRUE);

	// Shape 이미지 Custom Control 등록
	RegisterControl(IDS_CTRL_TENDON_SHAPE_IMAGE,
	                new CCMTendonShapeImageControl(this));

	SetColumnStyles();

	// 행 높이를 25px로 고정
	SetRowHeight(1, GetRowCount(), globalUtils.ScaleByDPI(25), NULL, GX_UPDATENOW);

	MakeDefaultItem();
	
	LockUpdate(FALSE);
	Redraw();
}

void CCMPostTensionTendonProfileGrid::SetDefaultValues()
{
	m_aColumnTitles.RemoveAll();
	m_aColumnTypes.RemoveAll();
	m_aColumnUnits.RemoveAll();
	m_aDefValues.RemoveAll();
	
	// 컬럼 정보 설정 (Post Tension Tendon Profile에 맞게)
	AddColumnInfo(_T("Boundary Condition"), t_string, 0);
	AddColumnInfo(_T("Shape"), t_string, 0);
	AddColumnInfo(_T("Preview"), t_string, 0);  // Shape 이미지 미리보기 컬럼
	AddColumnInfo(_T("CGS_ya"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("CGS_yc"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("CGS_yd"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("CGS_yb"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("a/L"), t_real, 0);
	AddColumnInfo(_T("c/L"), t_real, 0);
	AddColumnInfo(_T("d/L"), t_real, 0);
	AddColumnInfo(_T("b/L"), t_real, 0);
}

void CCMPostTensionTendonProfileGrid::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nColWidth[11] = { 150, 120, 70, 80,80,80,80,80,80,80,80 };  // Col3: 70 for Preview image

	// 헤더 설정
	for (int i = 0; i < 11; i++)
	{
		ROWCOL nCol = i + 1;
		CString title = m_aColumnTitles[i];

		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(title)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			.SetFont(CGXFont_GC().SetBold(TRUE)));

		SetColWidthByDPI(i + 1, i + 1, nColWidth[i]);
	}

	// Column 3 is now visible for Preview image (removed HideColsByIntend)

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMPostTensionTendonProfileGrid::MakeDefaultItem()
{
	// UI의 현재 Element Type 사용
	auto nCurElem = m_pParentDlg->GetElementType();
	MakeDefaultItem(nCurElem);
}

void CCMPostTensionTendonProfileGrid::MakeDefaultItem(int nElemType)
{
	BOOL bOldLock = LockUpdate(TRUE);

	ClearGrid();

	switch (nElemType)
	{
	case EN_ELEM_BEAM:
		SetDefaultBeamItem();
		break;
	case EN_ELEM_SLAB:
		SetDefaultSlabItem();
		break;
	case EN_ELEM_WALL:
		SetDefaultWallItem();
		break;
	default:
		ASSERT(0); return;
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMPostTensionTendonProfileGrid::SetDefaultBeamItem()
{
	auto _L_GetBoundaryCondition = [](int& nRow)
	{
		switch (nRow)
		{
		case 1:
			return _T("First");
		case 2:
			return _T("Middle");
		case 3:
			return _T("Last");;
		case 4:
		case 5:
		case 6:
		default:
			ASSERT(0); return _T("");
		}
	};

	SetRowCountByElemType(EN_ELEM_BEAM);

	for (int row = 1; row <= 3; row++)
	{
		CString strBoundary = _L_GetBoundaryCondition(row);
		CString strShape = m_pDoc->m_pAttrCtrl->GetBeamProfileShape(strBoundary, 1);
		int nShapeType = ConvertShapeStringToType(strShape);
		CString strShapeTypeNum;
		strShapeTypeNum.Format(_T("%d"), nShapeType);

		SetExpressionRowCol(row, 1, strBoundary);
		SetExpressionRowCol(row, 2, strShape);
		SetExpressionRowCol(row, 3, strShapeTypeNum);  // Shape 타입 번호 (이미지용)
		SetExpressionRowCol(row, 4, _T("0.06"));
		SetExpressionRowCol(row, 5, _T("0.06"));
		SetExpressionRowCol(row, 6, _T("0.06"));
		SetExpressionRowCol(row, 7, _T("0.06"));
		SetExpressionRowCol(row, 8, _T("0.25"));
		SetExpressionRowCol(row, 9, _T("0.5"));
		SetExpressionRowCol(row, 10, _T("0.75"));
		SetExpressionRowCol(row, 11, _T("0.25"));

		// 행별로 Boundary Condition에 맞는 Shape 목록 설정
		CString strShapeList = GetShapeTypeListByBoundary(strBoundary);
		SetStyleRange(CGXRange(row, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_COMBOBOX)
			.SetChoiceList(strShapeList)
			.SetHorizontalAlignment(DT_LEFT));
	}
}

void CCMPostTensionTendonProfileGrid::SetDefaultSlabItem()
{
	auto _L_GetBoundaryCondition = [](int& nRow)
	{
		switch (nRow)
		{
		case 1:
			return _T("First");
		case 2:
			return _T("Middle");
		case 3:
			return _T("Last");;
		case 4:
		default:
			ASSERT(0); return _T("");
		}
	};

	SetRowCountByElemType(EN_ELEM_SLAB);

	for (int row = 1; row <= 3; row++)
	{
		CString strBoundary = _L_GetBoundaryCondition(row);
		CString strShape = m_pDoc->m_pAttrCtrl->GetSlabProfileShape(strBoundary, 1);
		int nShapeType = ConvertShapeStringToType(strShape);
		CString strShapeTypeNum;
		strShapeTypeNum.Format(_T("%d"), nShapeType);

		SetExpressionRowCol(row, 1, strBoundary);
		SetExpressionRowCol(row, 2, strShape);
		SetExpressionRowCol(row, 3, strShapeTypeNum);  // Shape 타입 번호 (이미지용)
		SetExpressionRowCol(row, 4, _T("0.06"));
		SetExpressionRowCol(row, 5, _T("0.06"));
		SetExpressionRowCol(row, 6, _T("0.06"));
		SetExpressionRowCol(row, 7, _T("0.06"));
		SetExpressionRowCol(row, 8, _T("0.25"));
		SetExpressionRowCol(row, 9, _T("0.5"));
		SetExpressionRowCol(row, 10, _T("0.75"));
		SetExpressionRowCol(row, 11, _T("0.25"));

		// 행별로 Boundary Condition에 맞는 Shape 목록 설정
		CString strShapeList = GetShapeTypeListByBoundary(strBoundary);
		SetStyleRange(CGXRange(row, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_COMBOBOX)
			.SetChoiceList(strShapeList)
			.SetHorizontalAlignment(DT_LEFT));
	}
}

void CCMPostTensionTendonProfileGrid::SetDefaultWallItem()
{
	// Todo HJM : Wall이 들어오면 Wall에 맞게 처리해줄 필요 있음
	auto _L_GetBoundaryCondition = [](int& nRow)
	{
		switch (nRow)
		{
		case 1:
			return _T("First");
		case 2:
			return _T("Middle");
		case 3:
			return _T("Last");;
		case 4:
		case 5:
		case 6:
		default:
			ASSERT(0); return _T("");
		}
	};

	SetRowCountByElemType(EN_ELEM_WALL);

	for (int row = 1; row <= 3; row++)
	{
		CString strBoundary = _L_GetBoundaryCondition(row);
		CString strShape = m_pDoc->m_pAttrCtrl->GetWallProfileShape(strBoundary, 1);
		int nShapeType = ConvertShapeStringToType(strShape);
		CString strShapeTypeNum;
		strShapeTypeNum.Format(_T("%d"), nShapeType);

		SetExpressionRowCol(row, 1, strBoundary);
		SetExpressionRowCol(row, 2, strShape);
		SetExpressionRowCol(row, 3, strShapeTypeNum);  // Shape 타입 번호 (이미지용)
		SetExpressionRowCol(row, 4, _T("0.06"));
		SetExpressionRowCol(row, 5, _T("0.06"));
		SetExpressionRowCol(row, 6, _T("0.06"));
		SetExpressionRowCol(row, 7, _T("0.06"));
		SetExpressionRowCol(row, 8, _T("0.25"));
		SetExpressionRowCol(row, 9, _T("0.5"));
		SetExpressionRowCol(row, 10, _T("0.75"));
		SetExpressionRowCol(row, 11, _T("0.25"));

		// 행별로 Boundary Condition에 맞는 Shape 목록 설정
		CString strShapeList = GetShapeTypeListByBoundary(strBoundary);
		SetStyleRange(CGXRange(row, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_COMBOBOX)
			.SetChoiceList(strShapeList)
			.SetHorizontalAlignment(DT_LEFT));
	}
}

void CCMPostTensionTendonProfileGrid::GetShapeTypeList(CString& strList)
{
	// Middle용 전체 목록 (기본값)
	strList = _T("Parabolic Type1\nParabolic Type2\nParabolic Type3\nStraight Type1\nStraight Type2\nStraight Type3\n");
}

CString CCMPostTensionTendonProfileGrid::GetShapeTypeListByBoundary(const CString& strBoundary)
{
	if (strBoundary == _T("First") || strBoundary == _T("Last"))
	{
		// First/Last: Straight Type3 없음
		return _T("Parabolic Type1\nParabolic Type2\nParabolic Type3\nStraight Type1\nStraight Type2\n");
	}
	else // Middle
	{
		return _T("Parabolic Type1\nParabolic Type2\nParabolic Type3\nStraight Type1\nStraight Type2\nStraight Type3\n");
	}
}

CString CCMPostTensionTendonProfileGrid::GetProfileShape(const int& nCurElem, const CString& strBoundaryCondition, int nType)
{
	if (strBoundaryCondition.IsEmpty()) return CString();

	switch (nCurElem)
	{
	case EN_ELEM_BEAM:
		return m_pDoc->m_pAttrCtrl->GetBeamProfileShape(strBoundaryCondition, nType);
	case EN_ELEM_SLAB:
		return m_pDoc->m_pAttrCtrl->GetSlabProfileShape(strBoundaryCondition, nType);
	case EN_ELEM_WALL:
		return m_pDoc->m_pAttrCtrl->GetWallProfileShape(strBoundaryCondition, nType);
	}

	ASSERT(0);
	return CString();
}

void CCMPostTensionTendonProfileGrid::UpdateColumnHeader()
{
	SetDefaultValues();
	SetHeaderTitle();
}

void CCMPostTensionTendonProfileGrid::UpdateGridDefaultItem()
{
	MakeDefaultItem();
}

void CCMPostTensionTendonProfileGrid::SetColumnStyles()
{
	CString strShapeList;
	GetShapeTypeList(strShapeList);

	// Shape 컬럼 (콤보박스)
	SetStyleRange(CGXRange().SetCols(2), CGXStyle()
		.SetControl(GX_IDS_CTRL_COMBOBOX)
		.SetChoiceList(strShapeList)
		.SetHorizontalAlignment(DT_LEFT));

	// Preview 이미지 컬럼 (Col 3) - Custom Control로 이미지 표시
	SetStyleRange(CGXRange().SetCols(3), CGXStyle()
		.SetControl(IDS_CTRL_TENDON_SHAPE_IMAGE)
		.SetReadOnly(TRUE)
		.SetEnabled(FALSE));

	// 실수 컬럼들
	SetStyleRange(CGXRange().SetCols(4, 11), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(3));
}

void CCMPostTensionTendonProfileGrid::OnModifyCell(ROWCOL nRow, ROWCOL nCol)
{
	CTBBrowserWndEx::OnModifyCell(nRow, nCol);

	// Shape(Col 2) 변경 시 Preview(Col 3) 이미지 자동 업데이트
	if (nCol == 2)
	{
		CString strShape = GetValueRowCol(nRow, 2);
		int nShapeType = ConvertShapeStringToType(strShape);
		CString strShapeType;
		strShapeType.Format(_T("%d"), nShapeType);
		SetExpressionRowCol(nRow, 3, strShapeType);
		RedrawRowCol(nRow, 3);
	}

	if (m_pParentDlg)
	{
		m_pParentDlg->OnGridCellChanged(nRow, nCol);
	}
}

BOOL CCMPostTensionTendonProfileGrid::OnStartEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBBrowserWndEx::OnStartEditing(nRow, nCol);
}

BOOL CCMPostTensionTendonProfileGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	return CTBBrowserWndEx::OnEndEditing(nRow, nCol);
}

void CCMPostTensionTendonProfileGrid::UpdateProfileGridData()
{
	T_TDPF_D TdpfD;

	int nID = m_pParentDlg->GetSelectedProfileKey();
	if (nID < 1) return;

	if (!m_pDoc->m_pAttrCtrl2->GetTdpf(nID, TdpfD))
	{
		ASSERT(0); return;
	}

	// DB에서 로드한 nElemType 사용
	MakeDefaultItem(TdpfD.nElemType);
	InsertProfileData(TdpfD);
}

void CCMPostTensionTendonProfileGrid::InsertProfileData(T_TDPF_D& Data)
{
	BOOL bOldLock = LockUpdate(TRUE);

	int nCurElem = m_pParentDlg->GetElementType();

	auto& Profile = Data.aInfo;

	// 저장된 실제 데이터만큼 출력 (더미 데이터 없음)
	for (auto i = 0; i < Profile.GetSize(); i++)
	{
		int row = i + 1;

		CString strBoundary = Profile[i].strBoundaryCondition;
		SetExpressionRowCol(row, 1, strBoundary);

		CString strType = GetProfileShape(nCurElem, strBoundary, Profile[i].nType);
		SetExpressionRowCol(row, 2, strType);

		// Shape 타입 번호 설정 (Column 3 - 이미지용)
		CString strShapeTypeNum;
		strShapeTypeNum.Format(_T("%d"), Profile[i].nType);
		SetExpressionRowCol(row, 3, strShapeTypeNum);

		CString strYa, strYc, strYd, strYb;
		CString strXa, strXc, strXd, strXb;
		strYa.Format(_T("%.3f"), Profile[i].dYa);
		strYb.Format(_T("%.3f"), Profile[i].dYb);
		strYc.Format(_T("%.3f"), Profile[i].dYc);
		strYd.Format(_T("%.3f"), Profile[i].dYd);
		strXa.Format(_T("%.3f"), Profile[i].dXa);
		strXb.Format(_T("%.3f"), Profile[i].dXb);
		strXc.Format(_T("%.3f"), Profile[i].dXc);
		strXd.Format(_T("%.3f"), Profile[i].dXd);

		SetExpressionRowCol(row,  4, strYa);
		SetExpressionRowCol(row,  5, strYc);
		SetExpressionRowCol(row,  6, strYd);
		SetExpressionRowCol(row,  7, strYb);
		SetExpressionRowCol(row,  8, strXa);
		SetExpressionRowCol(row,  9, strXc);
		SetExpressionRowCol(row, 10, strXd);
		SetExpressionRowCol(row, 11, strXb);

		// 행별로 Boundary Condition에 맞는 Shape 목록 설정
		CString strShapeList = GetShapeTypeListByBoundary(strBoundary);
		SetStyleRange(CGXRange(row, 2), CGXStyle()
			.SetControl(GX_IDS_CTRL_COMBOBOX)
			.SetChoiceList(strShapeList)
			.SetHorizontalAlignment(DT_LEFT));
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CCMPostTensionTendonProfileGrid::ClearGrid()
{
	SetRowCount(1); // 헤더만 남김
	Redraw();
}

void CCMPostTensionTendonProfileGrid::SetRowCountByElemType(PT_ELEM_TYPE nElemType)
{
	int nRowCount = 3;
	switch (nElemType)
	{
	case EN_ELEM_BEAM:
		nRowCount = 3; break;
	case EN_ELEM_SLAB:
		nRowCount = 3; break;
	case EN_ELEM_WALL:
		nRowCount = 3; break;
	default:
		ASSERT(0); return;
	}
	SetRecordCount(nRowCount);

	// 행 높이를 25px로 고정
	SetRowHeight(1, nRowCount, globalUtils.ScaleByDPI(25), NULL, GX_UPDATENOW);
}

void CCMPostTensionTendonProfileGrid::RetrieveGridData(T_TDPF_D& TdpfD)
{
	if (!GetSafeHwnd()) return;

	TdpfD.aInfo.RemoveAll();

	int nRowCount = GetRowCount();

	for (int row = 1; row <= nRowCount-1; row++)
	{
		T_TDPF_INFO info;
		info.Initialize();

// 		if (info.strBoundaryCondition.IsEmpty())
// 			continue;

		info.strBoundaryCondition = GetValueRowCol(row, 1);

		CString strShape = GetValueRowCol(row, 2);
		info.nType = ConvertShapeStringToType(strShape);

		// Y 좌표 값들 (컬럼 4-7: Ya, Yc, Yd, Yb)
		info.dYa = _tstof(GetValueRowCol(row, 4));
		info.dYc = _tstof(GetValueRowCol(row, 5));
		info.dYd = _tstof(GetValueRowCol(row, 6));
		info.dYb = _tstof(GetValueRowCol(row, 7));

		// X 좌표 값들 (컬럼 8-11: a/L, c/L, d/L, b/L)
		info.dXa = _tstof(GetValueRowCol(row, 8));
		info.dXc = _tstof(GetValueRowCol(row, 9));
		info.dXd = _tstof(GetValueRowCol(row, 10));
		info.dXb = _tstof(GetValueRowCol(row, 11));

		TdpfD.aInfo.Add(info);
	}
}

int CCMPostTensionTendonProfileGrid::ConvertShapeStringToType(const CString& strShape)
{
	// Todo HJM : Elem Type 별로 처리하기
	auto nCurElem = m_pParentDlg->GetElementType();
	switch (nCurElem)
	{
	case EN_ELEM_BEAM:
		{

		}
		break;
	case EN_ELEM_SLAB:
		{

		}
		break;
	case EN_ELEM_WALL:
		{

		}
		break;
	default:
		ASSERT(0); return 0;
	}

	if (strShape == _T("Parabolic Type1")) return 1;
	if (strShape == _T("Parabolic Type2")) return 2;
	if (strShape == _T("Parabolic Type3")) return 3;
	if (strShape == _T("Straight Type1")) return 4;
	if (strShape == _T("Straight Type2")) return 5;
	if (strShape == _T("Straight Type3")) return 6;

	return 0; // 기본값
}

CString CCMPostTensionTendonProfileGrid::GetShapeTypeLabel(int nShapeType)
{
	switch (nShapeType)
	{
	case 1: return _T("Para.1");
	case 2: return _T("Para.2");
	case 3: return _T("Para.3");
	case 4: return _T("Str.1");
	case 5: return _T("Str.2");
	case 6: return _T("Str.3");
	default: return _T("");
	}
}

void CCMPostTensionTendonProfileGrid::AddColumnInfo(const CString& strTitle, int nType, int nUnit)
{
	m_aColumnTitles.Add(strTitle);
	m_aColumnTypes.Add(nType);
	m_aColumnUnits.Add(nUnit);
}

CString CCMPostTensionTendonProfileGrid::GetDefValue(int nCol)
{
	if (nCol >= 0 && nCol < m_aDefValues.GetSize())
		return m_aDefValues[nCol];
	return _T("");
}

BOOL CCMPostTensionTendonProfileGrid::OnValidateField(ROWCOL nRow, ROWCOL nCol)
{
	CString value = GetValueRowCol(nRow, nCol);
	value.TrimLeft();
	value.TrimRight();
	
	// 컬럼 타입에 따른 유효성 검사
	int nColIndex = nCol - 1;
	if (nColIndex >= 0 && nColIndex < m_aColumnTypes.GetSize())
	{
		int nType = m_aColumnTypes[nColIndex];
		
		if (nType == t_real)
		{
			// 간단한 숫자 유효성 검사
			if (!value.IsEmpty())
			{
				double dValue = _ttof(value);
				// 기본적인 범위 검사 등을 할 수 있음
			}
		}
	}
	
	return TRUE;
}

BOOL CCMPostTensionTendonProfileGrid::OnInsertEmptyRecord(ROWCOL nRow)
{
	BOOL bActive;
	ROWCOL nCol, ncRow, ncCol;
	CString value;
	
	// 현재 셀이 Active되어 있으면 해당 셀은 채우지 않는다.
	VERIFY(GetCurrentCell(ncRow, ncCol));
	CGXControl* pControl = (CGXControl*)GetControl(ncRow, ncCol);
	if (pControl && pControl->IsActive()) 
		bActive = TRUE;
	else 
		bActive = FALSE;
	
	BOOL bOldLock = LockUpdate(TRUE);
	int nColCount = GetColCount();
	for (int i = 0; i < nColCount; i++)
	{
		nCol = i + 1;
		value = GetDefValue(i);
		if (!value.IsEmpty() && (nCol != ncCol || !bActive))
			SetExpressionRowCol(nRow, nCol, value);
	}
	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
	
	return TRUE;
}