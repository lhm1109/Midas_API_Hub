// PostTensionModifyTendonVerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PostTensionModifyTendonVerDlg.h"
#include "PostTensionModifyTendonDlg.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_common\wg_common_TBBrowserWndEx.h"

// GdiplusDrawerStatic Shape includes
#include "GdiplusDrawerStatic\TGdiplusShapeLine.h"
#include "GdiplusDrawerStatic\TGdiplusShapePolyline.h"
#include "GdiplusDrawerStatic\TGdiplusShapeText.h"
#include "GdiplusDrawerStatic\TGdiplusShapeRect.h"

#include <vector>
#include <set>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPTScaleSliderCtrl - MStatusSlider 스타일 둥근 배경 슬라이더

IMPLEMENT_DYNAMIC(CPTScaleSliderCtrl, CBCGPSliderCtrl)

BEGIN_MESSAGE_MAP(CPTScaleSliderCtrl, CBCGPSliderCtrl)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CPTScaleSliderCtrl::OnDraw(CDC* pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);

	// 배경 지우기: 부모 다이얼로그에 배경 그리기를 요청하여 테마 배경색 적용
	{
		CPoint ptOrg(0, 0);
		MapWindowPoints(GetParent(), &ptOrg, 1);
		pDC->OffsetWindowOrg(ptOrg.x, ptOrg.y);
		GetParent()->SendMessage(WM_ERASEBKGND, (WPARAM)pDC->GetSafeHdc(), 0);
		pDC->SetWindowOrg(0, 0);
	}

	// 채널 (트랙) 그리기
	CRect rectChannel;
	GetChannelRect(&rectChannel);
	{
		int cy = rectChannel.CenterPoint().y;
		CPen penChannel(PS_SOLID, 2, RGB(180, 184, 188));
		CPen* pOldPen = pDC->SelectObject(&penChannel);
		pDC->MoveTo(rectChannel.left, cy);
		pDC->LineTo(rectChannel.right, cy);
		pDC->SelectObject(pOldPen);
	}

	// 썸 그리기 (MStatusSlider 색상: 기본 RGB(105,112,119))
	CRect rectThumb;
	GetThumbRect(&rectThumb);
	{
		rectThumb.DeflateRect(2, 1);

		COLORREF clrThumb = RGB(105, 112, 119);
		if (m_bIsThumbHighlighted)
			clrThumb = RGB(80, 90, 100);
		if (m_bIsThumbPressed)
			clrThumb = RGB(60, 70, 80);

		CBrush brush(clrThumb);
		CPen pen(PS_SOLID, 1, clrThumb);
		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		pDC->RoundRect(&rectThumb, CPoint(1, 1));
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
}

void CPTScaleSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bHighlighted)
	{
		m_bHighlighted = TRUE;
		Invalidate();

		TRACKMOUSEEVENT tme = { sizeof(tme), TME_LEAVE, m_hWnd, 0 };
		TrackMouseEvent(&tme);
	}
	CBCGPSliderCtrl::OnMouseMove(nFlags, point);
}

LRESULT CPTScaleSliderCtrl::OnMouseLeave(WPARAM wp, LPARAM lp)
{
	m_bHighlighted = FALSE;
	Invalidate();
	return Default();
}

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonVerProfileGrid - Column/Row Constants
enum EN_PROFILE_GRID_COL
{
	EN_COL_BOUNDARY_CONDITION = 1,
	EN_COL_SHAPE,
	EN_COL_CGS_YA,
	EN_COL_CGS_YC,
	EN_COL_CGS_YD,
	EN_COL_CGS_YB,
	EN_COL_X_A,
	EN_COL_X_C,
	EN_COL_X_D,
	EN_COL_X_B,
	EN_COL_COUNT = EN_COL_X_B  // Total column count
};

enum EN_PROFILE_GRID_ROW
{
	EN_ROW_DEFAULT_COUNT = 3  // Default row count for Beam/Slab/Wall
};

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonVerDlg dialog

CPostTensionModifyTendonVerDlg::CPostTensionModifyTendonVerDlg(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CChildDialog(CPostTensionModifyTendonVerDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pParent = (CPostTensionModifyTendonDlg*)pParent;
	m_pProfileGrid = nullptr;
	m_bUser = FALSE;
	m_PttnKey = 0;
	m_bUpdatingGrid = FALSE;
	m_dVertScale = 1.0;
}

CPostTensionModifyTendonVerDlg::~CPostTensionModifyTendonVerDlg()
{
	if (m_pProfileGrid)
	{
		delete m_pProfileGrid;
		m_pProfileGrid = nullptr;
	}
	m_wndPic.DeleteDrawer();
}

void CPostTensionModifyTendonVerDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_CMD_PT_MODIFY_TENDON_VER_PIC, m_wndPic);
	DDX_Control(pDX, IDD_CMD_PT_MODIFY_TENDON_VER_PROFILE_TABLE_CST, m_wndProfileTable);
	DDX_Control(pDX, IDC_CMD_PT_MODIFY_TENDON_PROFILE_CMB, m_cmbProfile);
	DDX_Control(pDX, IDC_CMD_PT_MODIFY_USER_CHK, m_chkUser);
	DDX_Check(pDX, IDC_CMD_PT_MODIFY_USER_CHK, m_bUser);
	DDX_Control(pDX, IDD_CMD_PT_MODIFY_TENDON_VER_SCALE_SLD, m_wndVertScaleSlider);
}

BEGIN_MESSAGE_MAP(CPostTensionModifyTendonVerDlg, CChildDialog)
	ON_BN_CLICKED(IDC_CMD_PT_MODIFY_USER_CHK, &CPostTensionModifyTendonVerDlg::OnBnClickedUserChk)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonVerDlg message handlers

BOOL CPostTensionModifyTendonVerDlg::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// Initialize GdiplusDrawerStatic
	m_wndPic.BindCtr(GetDlgItem(IDD_CMD_PT_MODIFY_TENDON_VER_PIC));
	m_wndPic.SetBackGroundColor(255, 255, 255);  // White background
	m_wndPic.SetScaleWithZoom(FALSE);  // 선/폰트 크기 고정 (화면 픽셀 기준)
	m_wndPic.SetOriginalLineWidth(1.0);  // 선 두께 1픽셀

	// Initialize Tendon Profile ComboBox
	UpdateTendonProfileList();

	// Create and Initialize Profile Grid
	CreateProfileGrid();
	InitializeProfileGrid();

	// 부모 다이얼로그에서 현재 선택된 텐던 키 가져오기
	if (m_pParent)
	{
		int nKey = m_pParent->GetSelectedTendonKey();
		if (nKey > 0)
		{
			SetPttnKey(static_cast<T_PTTN_K>(nKey));
		}
	}

	// 초기 그림 그리기 (SetPttnKey에서 이미 호출되지 않은 경우)
	if (m_PttnKey == 0)
	{
		DrawProfile();
	}

	// WS_CLIPSIBLINGS: 그림 컨트롤이 슬라이더 영역을 덮어 그리지 않도록 함
	m_wndPic.ModifyStyle(0, WS_CLIPSIBLINGS);
	m_wndVertScaleSlider.ModifyStyle(0, WS_CLIPSIBLINGS);

	// 수직 스케일 슬라이더 초기화 (로그 스케일: 0.1x ~ 10.0x)
	m_wndVertScaleSlider.m_bVisualManagerStyle = TRUE; // OnPaint에서 OnDraw 가상함수 호출 경로 활성화
	m_wndVertScaleSlider.SetRange(0, 100, TRUE);       // 0~100 정수 범위
	m_wndVertScaleSlider.SetPos(50);                    // 50 = 1.0x (중앙)

	// User Defined 초기 상태 설정
	UpdateUserDefinedState();

	return TRUE;
}

void CPostTensionModifyTendonVerDlg::UpdateTendonProfileList()
{
    m_cmbProfile.ResetContent();

    CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
    m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
    T_TDPF_D TdpfD;
    for (auto i = 0; i < aTdpfK.GetSize(); i++)
    {
        if (!m_pDoc->m_pAttrCtrl2->GetTdpf(aTdpfK[i], TdpfD))
        {
            ASSERT(0); return;
        }
        m_cmbProfile.AddString(TdpfD.strName);
    }

    m_cmbProfile.SetCurSel(0);
}

void CPostTensionModifyTendonVerDlg::CreateProfileGrid()
{
	CRect rect;
	GetDlgItem(IDD_CMD_PT_MODIFY_TENDON_VER_PROFILE_TABLE_CST)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_pProfileGrid = new CPostTensionModifyTendonVerProfileGrid();
	m_pProfileGrid->SetParentDlg(this);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_HSCROLL | WS_VSCROLL;
	m_pProfileGrid->Create(dwStyle, rect, this, IDD_CMD_PT_MODIFY_TENDON_VER_PROFILE_TABLE_CST);
}

void CPostTensionModifyTendonVerDlg::InitializeProfileGrid()
{
	if (m_pProfileGrid && m_pProfileGrid->GetSafeHwnd())
	{
		m_pProfileGrid->Initialize(m_pDoc);
		m_pProfileGrid->ShowWindow(SW_SHOW);
		m_pProfileGrid->UpdateWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonVerDlg - User Defined checkbox related functions

void CPostTensionModifyTendonVerDlg::OnBnClickedUserChk()
{
	UpdateData(TRUE);
	UpdateUserDefinedState();
}

void CPostTensionModifyTendonVerDlg::UpdateUserDefinedState()
{
	// User Defined 체크 시 ComboBox 비활성화
	if (m_cmbProfile.GetSafeHwnd())
	{
		m_cmbProfile.EnableWindow(!m_bUser);
	}

	// User Defined 체크 상태에 따라 Grid 활성화/비활성화
	if (m_pProfileGrid && m_pProfileGrid->GetSafeHwnd())
	{
		m_pProfileGrid->EnableWindow(m_bUser);
		m_pProfileGrid->SetVisualReadOnly(!m_bUser);
	}
}

void CPostTensionModifyTendonVerDlg::SetPttnKey(T_PTTN_K key)
{
	// 다이얼로그가 초기화되지 않은 경우 UI 업데이트 스킵
	if (!GetSafeHwnd()) return;

	m_PttnKey = key;

	// DB에서 원본 PTTN 데이터 가져오기
	if (!m_pDoc->m_pAttrCtrl2->GetPttn(m_PttnKey, m_OriginalPttnData))
	{
		m_OriginalPttnData.Initialize();
	}

	// User Defined 상태 설정
	m_bUser = m_OriginalPttnData.bUser;
	UpdateData(FALSE);

	// Profile ComboBox 선택 업데이트
	if (m_cmbProfile.GetSafeHwnd())
	{
		CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
		m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
		for (int i = 0; i < aTdpfK.GetSize(); i++)
		{
			if (aTdpfK[i] == m_OriginalPttnData.TdpfK)
			{
				m_cmbProfile.SetCurSel(i);
				break;
			}
		}
	}

	// Grid에 원본 데이터 표시
	T_TDPF_D TdpfD;
	if (m_pDoc->m_pAttrCtrl2->GetTdpf(m_OriginalPttnData.TdpfK, TdpfD))
	{
		// bUser가 true면 aInfo 사용, false면 TdpfD 사용
		if (m_OriginalPttnData.bUser)
		{
			T_TDPF_INFO_LIST aInfo;
			for(int i=0; i < m_OriginalPttnData.aInfo.GetSize(); i++)
			{
				T_TDPF_INFO Info = m_OriginalPttnData.aInfo[i].Info;
				aInfo.Add(Info);
			}
			TdpfD.aInfo.Copy(aInfo);
		}
		// Grid 업데이트 중 플래그 설정 (OnEndEditing에서 OnGridDataChanged 호출 방지)
		m_bUpdatingGrid = TRUE;
		m_pProfileGrid->UpdateProfileGridData(TdpfD);
		m_bUpdatingGrid = FALSE;
	}

	// Grid 데이터 로딩 후 비활성 스타일 적용 (데이터 로딩 전 적용 시 덮어씌워짐)
	UpdateUserDefinedState();

	DrawProfile();
}

BOOL CPostTensionModifyTendonVerDlg::IsGridDataChanged()
{
	if (!m_pProfileGrid || !m_pProfileGrid->GetSafeHwnd()) return FALSE;

	// 현재 Grid 데이터를 T_PTTN_D로 수집
	T_PTTN_D CurrentData;
	GetCurrentGridData(CurrentData);

	// aInfo 배열만 비교 (프로파일 정보)
	if (CurrentData.aInfo.GetSize() != m_OriginalPttnData.aInfo.GetSize()) return TRUE;

	for (int i = 0; i < CurrentData.aInfo.GetSize(); i++)
	{
		if (!(CurrentData.aInfo[i] == m_OriginalPttnData.aInfo[i])) return TRUE;
	}

	return FALSE;  // 모든 데이터가 동일
}

void CPostTensionModifyTendonVerDlg::GetCurrentGridData(T_PTTN_D& Data)
{
	Data.Initialize();

	if (!m_pProfileGrid || !m_pProfileGrid->GetSafeHwnd()) return;
	if (m_OriginalPttnData.aInfo.GetSize() < 1) return;

	// GetRowCount()는 헤더 포함, row < nRowCount로 데이터 행만 처리
	int nRowCount = m_pProfileGrid->GetRowCount();
	int nArraySize = static_cast<int>(m_OriginalPttnData.aInfo.GetSize());

	for (int row = 1; row < nRowCount; row++)
	{
		// 배열 범위 체크
		int nArrayIndex = row - 1;
		if (nArrayIndex >= nArraySize) break;

		// 원본 데이터를 복사하여 시작 (Node1, Node2, Data1, Data2 등 유지)
		T_PTTN_Base pttnBase = m_OriginalPttnData.aInfo[nArrayIndex];

		// Grid에서 Info 데이터 읽기
		T_TDPF_INFO& info = pttnBase.Info;
		info.strBoundaryCondition = m_pProfileGrid->GetValueRowCol(row, EN_COL_BOUNDARY_CONDITION);

		// Shape Type: 원본 데이터의 nType 유지
		CString strShape = m_pProfileGrid->GetValueRowCol(row, EN_COL_SHAPE);
		// info.nType은 원본에서 이미 복사됨

		info.dYa = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YA));
		info.dYc = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YC));
		info.dYd = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YD));
		info.dYb = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YB));
		info.dXa = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_A));
		info.dXc = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_C));
		info.dXd = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_D));
		info.dXb = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_B));

		// Data.aInfo에 추가
		Data.aInfo.Add(pttnBase);
	}
}

void CPostTensionModifyTendonVerDlg::OnGridDataChanged()
{
	// Grid 업데이트 중에는 무시 (초기화 과정에서 OnEndEditing이 호출되는 것 방지)
	if (m_bUpdatingGrid) return;

	// User Defined 체크 상태는 체크박스로만 제어 (Grid 편집으로 자동 토글하지 않음)
	// Grid 활성화/비활성화는 UpdateUserDefinedState()에서 EnableWindow()로 관리

	// 기존 OnProfileDataChanged 활용하여 그림 갱신
	OnProfileDataChanged();
}

void CPostTensionModifyTendonVerDlg::UpdateProfileGridData()
{
	if (!m_pProfileGrid || !m_pProfileGrid->GetSafeHwnd()) return;

	int nProfSel = m_cmbProfile.GetCurSel();
	if (nProfSel < 0) return;

	CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
	m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
	if (nProfSel >= aTdpfK.GetSize()) return;

	T_TDPF_D TdpfD;
	if (!m_pDoc->m_pAttrCtrl2->GetTdpf(aTdpfK[nProfSel], TdpfD))
	{
		ASSERT(0); return;
	}

	m_pProfileGrid->UpdateProfileGridData(TdpfD);
}

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonVerProfileGrid implementation

CPostTensionModifyTendonVerProfileGrid::CPostTensionModifyTendonVerProfileGrid()
{
	m_pParentDlg = nullptr;
	m_pDoc = nullptr;
	SetDefaultValues();
}

CPostTensionModifyTendonVerProfileGrid::~CPostTensionModifyTendonVerProfileGrid()
{
}

BEGIN_MESSAGE_MAP(CPostTensionModifyTendonVerProfileGrid, CTBBrowserWndEx)
END_MESSAGE_MAP()

void CPostTensionModifyTendonVerProfileGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWndEx::Initialize();

	m_pDoc = pDoc;
	SetDefaultValues();

	LockUpdate(TRUE);

	m_csTableName = _T("PostTensionModifyTendonVerProfile");

	SetColCount(EN_COL_COUNT);
	SetRecordCount(EN_ROW_DEFAULT_COUNT);

	EnableAppendRow(FALSE);

	GetParam()->EnableUndo(FALSE);
	EnableScrollTips();
	EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE);

	m_aTitle.RemoveAll();
	m_aType.RemoveAll();
	m_aUnit.RemoveAll();

	for (int i = 0; i < EN_COL_COUNT; i++)
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

	SetColumnStyles();

	// 수평 스크롤을 위한 컬럼 너비 고정 설정
	SetColWidth(EN_COL_BOUNDARY_CONDITION, EN_COL_BOUNDARY_CONDITION, 200);  // Boundary Condition
	SetColWidth(EN_COL_SHAPE, EN_COL_SHAPE, 140);                            // Shape
	SetColWidth(EN_COL_CGS_YA, EN_COL_CGS_YB, 100);                           // CGS_ya ~ CGS_yb
	SetColWidth(EN_COL_X_A, EN_COL_X_B, 60);                                  // a/L ~ b/L

	MakeDefaultItem();

	LockUpdate(FALSE);
	Redraw();
}

void CPostTensionModifyTendonVerProfileGrid::SetDefaultValues()
{
	m_aColumnTitles.RemoveAll();
	m_aColumnTypes.RemoveAll();
	m_aColumnUnits.RemoveAll();

	AddColumnInfo(_T("Boundary Condition"), t_string, 0);
	AddColumnInfo(_T("Shape"), t_string, 0);
	AddColumnInfo(_T("CGS_ya"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("CGS_yc"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("CGS_yd"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("CGS_yb"), t_real, D_UNITSYS_BASE_LENGTH);
	AddColumnInfo(_T("a/L"), t_real, 0);
	AddColumnInfo(_T("c/L"), t_real, 0);
	AddColumnInfo(_T("d/L"), t_real, 0);
	AddColumnInfo(_T("b/L"), t_real, 0);
}

void CPostTensionModifyTendonVerProfileGrid::AddColumnInfo(const CString& strTitle, int nType, int nUnit)
{
	m_aColumnTitles.Add(strTitle);
	m_aColumnTypes.Add(nType);
	m_aColumnUnits.Add(nUnit);
}

void CPostTensionModifyTendonVerProfileGrid::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);

	for (int i = 0; i < EN_COL_COUNT; i++)
	{
		ROWCOL nCol = i + 1;
		CString title = m_aColumnTitles[i];

		SetStyleRange(CGXRange(0, nCol), CGXStyle()
			.SetValue(title)
			.SetHorizontalAlignment(DT_CENTER)
			.SetVerticalAlignment(DT_VCENTER)
			.SetFont(CGXFont_GC().SetBold(TRUE)));
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CPostTensionModifyTendonVerProfileGrid::SetColumnStyles()
{
	CString strShapeList;
	GetShapeTypeList(strShapeList);

	SetStyleRange(CGXRange().SetCols(EN_COL_SHAPE), CGXStyle()
		.SetControl(GX_IDS_CTRL_COMBOBOX)
		.SetChoiceList(strShapeList)
		.SetHorizontalAlignment(DT_LEFT));

	SetStyleRange(CGXRange().SetCols(EN_COL_CGS_YA, EN_COL_X_B), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(3));
}

void CPostTensionModifyTendonVerProfileGrid::GetShapeTypeList(CString& strList)
{
	strList = _T("Parabolic Type1\nParabolic Type2\nParabolic Type3\nStraight Type1\nStraight Type2\nStraight Type3\n");
}

CString CPostTensionModifyTendonVerProfileGrid::GetProfileShape(const CString& strBoundaryCondition, int nType)
{
	if (strBoundaryCondition.IsEmpty()) return CString();
	return m_pDoc->m_pAttrCtrl->GetBeamProfileShape(strBoundaryCondition, nType);
}

void CPostTensionModifyTendonVerProfileGrid::MakeDefaultItem()
{
	SetDefaultBeamItem();
}

void CPostTensionModifyTendonVerProfileGrid::SetDefaultBeamItem()
{
	auto _L_GetBoundaryCondition = [](int nRow) -> LPCTSTR
	{
		switch (nRow)
		{
		case 1: return _T("First");
		case 2: return _T("Middle");
		case 3: return _T("Last");
		default: return _T("");
		}
	};

	BOOL bOldLock = LockUpdate(TRUE);
	ClearGrid();
	SetRecordCount(EN_ROW_DEFAULT_COUNT);

	for (int row = 1; row <= EN_ROW_DEFAULT_COUNT; row++)
	{
		CString strBC = _L_GetBoundaryCondition(row);
		CString strShape = m_pDoc->m_pAttrCtrl->GetBeamProfileShape(strBC, 1);
		SetExpressionRowCol(row, EN_COL_BOUNDARY_CONDITION, strBC);
		SetExpressionRowCol(row, EN_COL_SHAPE, strShape);
		SetExpressionRowCol(row, EN_COL_CGS_YA, _T("0.06"));
		SetExpressionRowCol(row, EN_COL_CGS_YC, _T("0.06"));
		SetExpressionRowCol(row, EN_COL_CGS_YD, _T("0.06"));
		SetExpressionRowCol(row, EN_COL_CGS_YB, _T("0.06"));
		SetExpressionRowCol(row, EN_COL_X_A, _T("0.25"));
		SetExpressionRowCol(row, EN_COL_X_C, _T("0.5"));
		SetExpressionRowCol(row, EN_COL_X_D, _T("0.75"));
		SetExpressionRowCol(row, EN_COL_X_B, _T("0.25"));
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CPostTensionModifyTendonVerProfileGrid::ClearGrid()
{
	SetRowCount(1);
	Redraw();
}

void CPostTensionModifyTendonVerProfileGrid::SetVisualReadOnly(BOOL bReadOnly)
{
	m_bLockEdit = bReadOnly;
	Redraw();
}


BOOL CPostTensionModifyTendonVerProfileGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bResult = CTBBrowserWndEx::OnEndEditing(nRow, nCol);

	// 부모 다이얼로그에 변경 알림
	if (m_pParentDlg)
	{
		m_pParentDlg->OnGridDataChanged();
	}

	return bResult;
}

void CPostTensionModifyTendonVerProfileGrid::UpdateProfileGridData(T_TDPF_D& TdpfD)
{
	MakeDefaultItem();
	InsertProfileData(TdpfD);
}

void CPostTensionModifyTendonVerProfileGrid::InsertProfileData(T_TDPF_D& Data)
{
	BOOL bOldLock = LockUpdate(TRUE);

	auto& Profile = Data.aInfo;

	for (auto i = 0; i < Profile.GetSize(); i++)
	{
		int row = i + 1;

		SetExpressionRowCol(row, EN_COL_BOUNDARY_CONDITION, Profile[i].strBoundaryCondition);

		CString strType = GetProfileShape(Profile[i].strBoundaryCondition, Profile[i].nType);
		SetExpressionRowCol(row, EN_COL_SHAPE, strType);

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

		SetExpressionRowCol(row, EN_COL_CGS_YA, strYa);
		SetExpressionRowCol(row, EN_COL_CGS_YC, strYc);
		SetExpressionRowCol(row, EN_COL_CGS_YD, strYd);
		SetExpressionRowCol(row, EN_COL_CGS_YB, strYb);
		SetExpressionRowCol(row, EN_COL_X_A, strXa);
		SetExpressionRowCol(row, EN_COL_X_C, strXc);
		SetExpressionRowCol(row, EN_COL_X_D, strXd);
		SetExpressionRowCol(row, EN_COL_X_B, strXb);
	}

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonVerDlg drawing functions

void CPostTensionModifyTendonVerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar && pScrollBar->GetSafeHwnd() == m_wndVertScaleSlider.GetSafeHwnd())
	{
		int nSliderPos = m_wndVertScaleSlider.GetPos();
		// 로그 스케일 변환: pos 0→0.1x, 50→1.0x, 100→10.0x
		m_dVertScale = std::pow(10.0, (nSliderPos - 50) / 50.0);
		DrawProfile();
	}
	CChildDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPostTensionModifyTendonVerDlg::OnProfileDataChanged()
{
	DrawProfile();
}

void CPostTensionModifyTendonVerDlg::DrawProfile()
{
	if (!m_wndPic.GetSafeHwnd()) return;

	m_wndPic.ClearAllEntity();

	//=========================================================================
	// Step 1: 유효성 검사 - 텐던 키와 문서 포인터 확인
	//=========================================================================
	if (m_PttnKey == 0 || !m_pDoc || !m_pDoc->m_pAttrCtrl)
	{
		DrawDefaultGuide();
		m_wndPic.ReDraw(TRUE);
		if (m_wndVertScaleSlider.GetSafeHwnd())
			m_wndVertScaleSlider.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		return;
	}

	//=========================================================================
	// Step 2: 텐던 프로필 데이터 계산
	//=========================================================================
	CArray<T_NODE_D, T_NODE_D&> aTendonProfile;
	CArray<BOOL, BOOL> aProfileInputPoint;

	BOOL bSuccess = m_pDoc->m_pAttrCtrl->calcPttnSlabTendonProfile(
		m_PttnKey,
		aTendonProfile,
		aProfileInputPoint,
		36,      // nDivide
		FALSE,   // bOnlyTendonLocalCoor
		FALSE,   // bOnlyControlPoint
		FALSE    // bOnlyElementLocalCoor
	);

	int nPoints = static_cast<int>(aTendonProfile.GetSize());
	if (!bSuccess || nPoints < 2)
	{
		DrawNoDataMessage();
		m_wndPic.ReDraw(TRUE);
		if (m_wndVertScaleSlider.GetSafeHwnd())
			m_wndVertScaleSlider.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
		return;
	}

	//=========================================================================
	// Step 3: 3D -> 2D 좌표 변환
	//=========================================================================
	std::vector<double> vecX2D;
	std::vector<double> vecY2D;
	vecX2D.reserve(nPoints);
	vecY2D.reserve(nPoints);

	double dCumulativeDistance = 0.0;

	for (int i = 0; i < nPoints; ++i)
	{
		const T_NODE_D& pt = aTendonProfile[i];

		if (i == 0)
		{
			dCumulativeDistance = 0.0;
		}
		else
		{
			const T_NODE_D& ptPrev = aTendonProfile[i - 1];
			double dx = pt.x - ptPrev.x;
			double dy = pt.y - ptPrev.y;
			double dDelta = std::sqrt(dx * dx + dy * dy);
			dCumulativeDistance += dDelta;
		}

		vecX2D.push_back(dCumulativeDistance);
		vecY2D.push_back(pt.z);
	}

	//=========================================================================
	// Step 3 → 3.5 사이: 원본 Y 범위 저장 (스케일 적용 전)
	//=========================================================================
	double dOrigRangeY = 0.0;
	{
		double dOrigMinY = vecY2D[0];
		double dOrigMaxY = vecY2D[0];
		for (size_t i = 1; i < vecY2D.size(); ++i)
		{
			if (dgn::LT(vecY2D[i], dOrigMinY)) dOrigMinY = vecY2D[i];
			if (dgn::MT(vecY2D[i], dOrigMaxY)) dOrigMaxY = vecY2D[i];
		}
		dOrigRangeY = dOrigMaxY - dOrigMinY;
	}

	//=========================================================================
	// Step 3.5: 수직 스케일 적용
	//=========================================================================
	double dMeanY = 0.0;
	for (size_t i = 0; i < vecY2D.size(); ++i)
		dMeanY += vecY2D[i];
	dMeanY /= static_cast<double>(vecY2D.size());

	if (!dgn::EQ(m_dVertScale, 1.0))
	{
		for (size_t i = 0; i < vecY2D.size(); ++i)
			vecY2D[i] = dMeanY + (vecY2D[i] - dMeanY) * m_dVertScale;
	}

	//=========================================================================
	// Step 3.6: Span별 Slab 두께 및 2D X 위치 수집
	//=========================================================================
	struct SpanSlabInfo {
		double dXStart;
		double dXEnd;
		double dThickness;
		double dCentroidZ;
		T_NODE_K NodeStart;
		T_NODE_K NodeEnd;
		BOOL bValid;
	};
	std::vector<SpanSlabInfo> vecSpanSlab;

	{
		int nSpansSlab = static_cast<int>(m_OriginalPttnData.aInfo.GetSize());
		for (int spanIdx = 0; spanIdx < nSpansSlab; ++spanIdx)
		{
			const T_PTTN_Base& spanInfo = m_OriginalPttnData.aInfo[spanIdx];
			SpanSlabInfo slab;
			slab.NodeStart  = spanInfo.Node1;
			slab.NodeEnd    = spanInfo.Node2;
			slab.dXStart    = -1.0;
			slab.dXEnd      = -1.0;
			slab.dThickness = 0.0;
			slab.dCentroidZ = 0.0;
			slab.bValid     = FALSE;

			// Data1/Data2 좌표로 누적거리(X) 매핑
			for (int i = 0; i < nPoints; ++i)
			{
				const T_NODE_D& pt = aTendonProfile[i];
				if (dgn::LT0(slab.dXStart))
				{
					if (dgn::EQ0(pt.x - spanInfo.Data1.x) && dgn::EQ0(pt.y - spanInfo.Data1.y))
						slab.dXStart = vecX2D[i];
				}
				if (dgn::LT0(slab.dXEnd))
				{
					if (dgn::EQ0(pt.x - spanInfo.Data2.x) && dgn::EQ0(pt.y - spanInfo.Data2.y))
						slab.dXEnd = vecX2D[i];
				}
				if (dgn::ME0(slab.dXStart) && dgn::ME0(slab.dXEnd)) break;
			}

			// 시작 노드의 연결 Slab 요소에서 두께 취득
			CArray<T_ELEM_K, T_ELEM_K> aElemList;
			m_pDoc->m_pAttrCtrl->GetConnectedElem(spanInfo.Node1, aElemList);
			for (int e = 0; e < aElemList.GetSize(); ++e)
			{
				T_ELEM_D ElemD;
				if (!m_pDoc->m_pAttrCtrl->GetElem(aElemList[e], ElemD)) continue;
				if (!m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp)) continue;
				T_THIK_D ThikD;
				if (m_pDoc->m_pAttrCtrl->GetThik(ElemD.elpro, ThikD))
				{
					slab.dThickness = ThikD.Value.ThickIn;
					T_NODE_D NodeData;
					if (m_pDoc->m_pAttrCtrl->GetNode(spanInfo.Node1, NodeData))
						slab.dCentroidZ = NodeData.z;
					slab.bValid = TRUE;
					break;
				}
			}
			vecSpanSlab.push_back(slab);
		}
	}

	//=========================================================================
	// Step 3.7: Span 노드별 수직부재 정보 수집
	//=========================================================================
	struct ColumnInfo {
		double dXPos;      // 2D X 위치 (누적거리)
		double dTopZ;      // 수직부재 상단 Z좌표
		double dBotZ;      // 수직부재 하단 Z좌표
		double dWidth;     // 단면 폭
	};
	std::vector<ColumnInfo> vecColumnInfo;

	{
		std::set<T_NODE_K> setProcessedCol;
		for (size_t s = 0; s < vecSpanSlab.size(); ++s)
		{
			if (!vecSpanSlab[s].bValid) continue;

			T_NODE_K nodeKeys[2]  = { vecSpanSlab[s].NodeStart, vecSpanSlab[s].NodeEnd };
			double   nodeXPos[2]  = { vecSpanSlab[s].dXStart,   vecSpanSlab[s].dXEnd };

			for (int n = 0; n < 2; ++n)
			{
				if (nodeKeys[n] == 0 || dgn::LT0(nodeXPos[n])) continue;
				if (setProcessedCol.count(nodeKeys[n])) continue;
				setProcessedCol.insert(nodeKeys[n]);

				CArray<T_ELEM_K, T_ELEM_K> aElemList;
				m_pDoc->m_pAttrCtrl->GetConnectedElem(nodeKeys[n], aElemList);
				for (int e = 0; e < aElemList.GetSize(); ++e)
				{
					T_ELEM_D ElemD;
					if (!m_pDoc->m_pAttrCtrl->GetElem(aElemList[e], ElemD)) continue;
					if (m_pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp)) continue;
					T_NODE_D NodeI, NodeJ;
					if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeI)) continue;
					if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeJ)) continue;
					if (m_pDoc->m_pAttrCtrl->IsVerticalFrame(NodeI, NodeJ))
					{
						// Slab 연결 노드의 Z좌표
						T_NODE_D SlabNode;
						m_pDoc->m_pAttrCtrl->GetNode(nodeKeys[n], SlabNode);

						// Column의 다른 쪽 끝 Z좌표 결정
						double dOtherZ = dgn::EQ(NodeI.z, SlabNode.z) ? NodeJ.z : NodeI.z;

						// 아래로 뻗은 경우만 처리 (dOtherZ < SlabNode.z)
						if (dgn::LT(dOtherZ, SlabNode.z))
						{
							ColumnInfo col;
							col.dXPos = nodeXPos[n];
							col.dTopZ = SlabNode.z;    // Slab 연결 노드 = Top
							col.dBotZ = dOtherZ;       // 아래쪽 노드 = Bottom
							col.dWidth = 0.0;

							double hi, bi, hj, bj;
							hi = bi = hj = bj = 0.0;
							if (m_pDoc->m_pAttrCtrl->GetSectDimen(ElemD.elpro, hi, bi, hj, bj))
								col.dWidth = bi;

							vecColumnInfo.push_back(col);
							break;  // 아래로 뻗은 부재를 찾았으면 종료
						}
						// 위로 뻗은 경우는 continue하여 다른 수직부재 탐색
					}
				}
			}
		}
	}

	//=========================================================================
	// Step 4: 좌표 범위 계산
	//=========================================================================
	double dMinX = vecX2D.front();
	double dMaxX = vecX2D.back();
	double dMinY = vecY2D[0];
	double dMaxY = vecY2D[0];

	for (size_t i = 0; i < vecY2D.size(); ++i)
	{
		if (dgn::LT(vecY2D[i], dMinY)) dMinY = vecY2D[i];
		if (dgn::MT(vecY2D[i], dMaxY)) dMaxY = vecY2D[i];
	}

	// Slab 경계도 Y범위에 포함
	for (size_t s = 0; s < vecSpanSlab.size(); ++s)
	{
		if (!vecSpanSlab[s].bValid) continue;
		double dHalfThick = vecSpanSlab[s].dThickness / 2.0;
		double dTopRaw = vecSpanSlab[s].dCentroidZ + dHalfThick;
		double dBotRaw = vecSpanSlab[s].dCentroidZ - dHalfThick;
		double dScaledTop = dMeanY + (dTopRaw - dMeanY) * m_dVertScale;
		double dScaledBot = dMeanY + (dBotRaw - dMeanY) * m_dVertScale;
		if (dgn::LT(dScaledBot, dMinY)) dMinY = dScaledBot;
		if (dgn::MT(dScaledTop, dMaxY)) dMaxY = dScaledTop;
	}

	// 수직부재 Z범위도 Y범위에 포함 (스케일 미적용 - 원본 Z값 사용)
	for (size_t c = 0; c < vecColumnInfo.size(); ++c)
	{
		double dColTop = vecColumnInfo[c].dTopZ;
		double dColBot = vecColumnInfo[c].dBotZ;
		if (dgn::LT(dColBot, dMinY)) dMinY = dColBot;
		if (dgn::MT(dColTop, dMaxY)) dMaxY = dColTop;
	}

	double dRangeX = dMaxX - dMinX;
	double dRangeY = dMaxY - dMinY;

	double dMarginX = (dgn::MT0(dRangeX)) ? dRangeX * 0.05 : 1.0;
	double dMarginY = (dgn::MT0(dRangeY)) ? dRangeY * 0.05 : 1.0;

	//=========================================================================
	// Step 5: 기준선 그리기 (Z=0 도심선)
	//=========================================================================
	if (dgn::LE0(dMinY) && dgn::ME0(dMaxY))
	{
		gdiplus_draw::TGdiplusShapeLine axisLine;
		axisLine.poStt.dx = dMinX - dMarginX;
		axisLine.poStt.dy = 0.0;
		axisLine.poEnd.dx = dMaxX + dMarginX;
		axisLine.poEnd.dy = 0.0;
		axisLine.SetColor(180, 180, 180);
		m_wndPic.AddEntity(&axisLine);

		gdiplus_draw::TGdiplusShapeText cgText;
		cgText.poInsert.dx = dMinX - dMarginX * 0.5;
		cgText.poInsert.dy = 0.0;
		cgText.strContent = _T("C.G.");
		cgText.SetColor(128, 128, 128);
		m_wndPic.AddEntity(&cgText);
	}

	//=========================================================================
	// Step 5.5: 슬래브 상하면 경계선 그리기
	//=========================================================================
	for (size_t s = 0; s < vecSpanSlab.size(); ++s)
	{
		if (!vecSpanSlab[s].bValid) continue;
		if (dgn::LT0(vecSpanSlab[s].dXStart) || dgn::LT0(vecSpanSlab[s].dXEnd)) continue;

		double dHalfThick = vecSpanSlab[s].dThickness / 2.0;
		double dTopRaw = vecSpanSlab[s].dCentroidZ + dHalfThick;
		double dBotRaw = vecSpanSlab[s].dCentroidZ - dHalfThick;
		double dTopY = dMeanY + (dTopRaw - dMeanY) * m_dVertScale;
		double dBotY = dMeanY + (dBotRaw - dMeanY) * m_dVertScale;
		double dXS = vecSpanSlab[s].dXStart;
		double dXE = vecSpanSlab[s].dXEnd;

		// 상면 라인 (회색 대시)
		gdiplus_draw::TGdiplusShapeLine topLine;
		topLine.poStt.dx = dXS;  topLine.poStt.dy = dTopY;
		topLine.poEnd.dx = dXE;  topLine.poEnd.dy = dTopY;
		topLine.SetColor(180, 180, 180);
		topLine.SetDashStyle(gdiplus_draw::DashStyleDash);
		m_wndPic.AddEntity(&topLine);

		// 하면 라인 (회색 대시)
		gdiplus_draw::TGdiplusShapeLine botLine;
		botLine.poStt.dx = dXS;  botLine.poStt.dy = dBotY;
		botLine.poEnd.dx = dXE;  botLine.poEnd.dy = dBotY;
		botLine.SetColor(180, 180, 180);
		botLine.SetDashStyle(gdiplus_draw::DashStyleDash);
		m_wndPic.AddEntity(&botLine);
	}

	//=========================================================================
	// Step 5.6: 지점부 수직부재(Column/Wall) 사각형 그리기
	//=========================================================================
	for (size_t c = 0; c < vecColumnInfo.size(); ++c)
	{
		// Y 좌표 (수직 스케일 미적용 - 원본 Z값 사용)
		double dTopY = vecColumnInfo[c].dTopZ;
		double dBotY = vecColumnInfo[c].dBotZ;

		// X 폭: 실제 단면 폭을 그대로 사용 (Slab Thickness와 동일 스케일)
		double dHalfW = 0.0;
		if (dgn::MT0(vecColumnInfo[c].dWidth))
			dHalfW = vecColumnInfo[c].dWidth / 2.0;

		// 최소/최대 폭 제한 (dRangeY 기준 - Slab Thickness 스케일과 일치)
		double dMinW = dRangeY * 0.01;
		double dMaxW = dRangeY * 0.15;
		if (dgn::LT(dHalfW, dMinW)) dHalfW = dMinW;
		if (dgn::MT(dHalfW, dMaxW)) dHalfW = dMaxW;

		gdiplus_draw::TGdiplusShapeRegionRect colRect(
			vecColumnInfo[c].dXPos - dHalfW,  // left
			dTopY,                              // top
			vecColumnInfo[c].dXPos + dHalfW,   // right
			dBotY                               // bottom
		);
		colRect.SetColor(180, 180, 180);
		colRect.SetForeColor(210, 210, 210);
		colRect.SetBackColor(210, 210, 210);
		colRect.SetHatchStyle(0);          // solid fill
		colRect.SetDrawBoundary(true);
		m_wndPic.AddEntity(&colRect);
	}

	//=========================================================================
	// Step 6: 텐던 프로필 폴리라인 그리기
	//=========================================================================
	gdiplus_draw::TGdiplusShapePolyline polyline;
	polyline.SetColor(0, 0, 192);

	for (size_t i = 0; i < vecX2D.size(); ++i)
	{
		gdiplus_draw::TGdiplusShapePoint pt;
		pt.dx = vecX2D[i];
		pt.dy = vecY2D[i];
		polyline.vertexs.push_back(pt);
	}
	m_wndPic.AddEntity(&polyline);

	//=========================================================================
	// Step 7: 컨트롤 포인트 표시 (채워진 사각형)
	// Span 경계 포인트: 빨간색 (5배 크기), 일반 포인트: 파란색 (3배 크기)
	//=========================================================================
	double dMinRange = dgn::LT(dRangeX, dOrigRangeY) ? dRangeX : dOrigRangeY;
	if (dgn::LT0(dMinRange)) dMinRange = dgn::MT0(dRangeX) ? dRangeX : dOrigRangeY;
	if (dgn::LT0(dMinRange)) dMinRange = 1.0;
	double dCtrlPtSize = dMinRange * 0.045;  // 일반 포인트 크기 (0.015 * 3)
	if (dgn::LT(dCtrlPtSize, 0.015)) dCtrlPtSize = 0.015;  // 최소 크기
	double dSpanPtSize = dMinRange * 0.075;  // Span 경계 포인트 크기 (0.015 * 5)
	if (dgn::MT(dSpanPtSize, 0.025)) dSpanPtSize = 0.025;  // 최소 크기

	ASSERT(aProfileInputPoint.GetSize() == nPoints);

	//-------------------------------------------------------------------------
	// Span 경계 노드 수집 및 3D 좌표 획득
	//-------------------------------------------------------------------------
	std::vector<T_NODE_D> vecBoundaryCoords;
	int nSpans = static_cast<int>(m_OriginalPttnData.aInfo.GetSize());
	for (int spanIdx = 0; spanIdx < nSpans; ++spanIdx)
	{
		const T_PTTN_Base& spanInfo = m_OriginalPttnData.aInfo[spanIdx];
		vecBoundaryCoords.push_back(spanInfo.Data1);
		vecBoundaryCoords.push_back(spanInfo.Data2);
	}

	//-------------------------------------------------------------------------
	// 컨트롤 포인트 그리기
	//-------------------------------------------------------------------------
	for (int i = 0; i < nPoints; ++i)
	{
		if (!aProfileInputPoint[i]) continue;

		double x = vecX2D[i];
		double y = vecY2D[i];

		// Span 경계 포인트 여부 판단
		BOOL bIsSpanBoundary = FALSE;

		// 첫 번째/마지막 포인트는 항상 경계
		if (i == 0 || i == nPoints - 1)
		{
			bIsSpanBoundary = TRUE;
		}
		else
		{
			// 경계 노드 좌표와 X, Y만 비교 (Z는 CGS 오프셋으로 다를 수 있음)
			const T_NODE_D& profilePt = aTendonProfile[i];
			for (size_t j = 0; j < vecBoundaryCoords.size(); ++j)
			{
				// X, Y 좌표만 비교 (tolerance 사용)
				double dx = profilePt.x - vecBoundaryCoords[j].x;
				double dy = profilePt.y - vecBoundaryCoords[j].y;
				if (dgn::EQ0(dx) && dgn::EQ0(dy))  // tolerance: D_ZERO_LIMIT (1e-7)
				{
					bIsSpanBoundary = TRUE;
					break;
				}
			}
		}

		double dSize = bIsSpanBoundary ? dSpanPtSize : dCtrlPtSize;

		gdiplus_draw::TGdiplusShapeRegionRect rect(
			x - dSize,
			y + dSize,
			x + dSize,
			y - dSize
		);

		if (bIsSpanBoundary)
		{
			// Span 경계 포인트: 빨간색
			rect.SetColor(192, 0, 0);
			rect.SetForeColor(192, 0, 0);
			rect.SetBackColor(192, 0, 0);
		}
		else
		{
			// 일반 포인트: 파란색 (선과 동일)
			rect.SetColor(0, 0, 192);
			rect.SetForeColor(0, 0, 192);
			rect.SetBackColor(0, 0, 192);
		}

		m_wndPic.AddEntity(&rect);
	}

	//=========================================================================
	// Step 8: 시작점/끝점 레이블
	//=========================================================================
	double dTextOffsetX = dMarginX * 0.5;  // 수평 텍스트 오프셋

	// I 레이블 - 시작점 좌측에 표시
	gdiplus_draw::TGdiplusShapeText textStart;
	textStart.poInsert.dx = vecX2D.front() - dTextOffsetX;
	textStart.poInsert.dy = vecY2D.front();
	textStart.strContent = _T("I");
	textStart.SetColor(192, 0, 0);
	m_wndPic.AddEntity(&textStart);

	// J 레이블 - 끝점 우측에 표시
	gdiplus_draw::TGdiplusShapeText textEnd;
	textEnd.poInsert.dx = vecX2D.back() + dTextOffsetX;
	textEnd.poInsert.dy = vecY2D.back();
	textEnd.strContent = _T("J");
	textEnd.SetColor(192, 0, 0);
	m_wndPic.AddEntity(&textEnd);

	//=========================================================================
	// Step 9: 화면 갱신
	//=========================================================================
	m_wndPic.ReDraw(TRUE);
	if (m_wndVertScaleSlider.GetSafeHwnd())
		m_wndVertScaleSlider.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}

void CPostTensionModifyTendonVerDlg::GetPttnData(T_PTTN_D& Data)
{
	// TdpfK: Profile ComboBox에서 선택된 인덱스에 해당하는 키
	if (m_cmbProfile.GetSafeHwnd())
	{
		int nProfSel = m_cmbProfile.GetCurSel();
		if (nProfSel >= 0)
		{
			CArray<T_TDPF_K, T_TDPF_K> aTdpfK;
			m_pDoc->m_pAttrCtrl2->GetTdpfKeyList(aTdpfK);
			if (nProfSel < aTdpfK.GetSize())
			{
				Data.TdpfK = aTdpfK[nProfSel];
			}
		}
	}

	// bUser: User Defined 체크박스 상태
	Data.bUser = m_bUser;

	// aInfo: Grid 데이터에서 추출 (원본 데이터의 Node/Data 정보 보존)
	if (m_pProfileGrid && m_pProfileGrid->GetSafeHwnd())
	{
		int nRowCount = m_pProfileGrid->GetRowCount();
		int nOriginalSize = static_cast<int>(m_OriginalPttnData.aInfo.GetSize());
		Data.aInfo.RemoveAll();

		for (int row = 1; row < nRowCount; row++)
		{
			int nArrayIndex = row - 1;
			T_PTTN_Base pttnBase;

			// 원본 데이터에서 Node1, Node2, Data1, Data2, nType 복사 (있는 경우)
			if (nArrayIndex < nOriginalSize)
			{
				pttnBase.Node1 = m_OriginalPttnData.aInfo[nArrayIndex].Node1;
				pttnBase.Node2 = m_OriginalPttnData.aInfo[nArrayIndex].Node2;
				pttnBase.Data1 = m_OriginalPttnData.aInfo[nArrayIndex].Data1;
				pttnBase.Data2 = m_OriginalPttnData.aInfo[nArrayIndex].Data2;
				pttnBase.Info.nType = m_OriginalPttnData.aInfo[nArrayIndex].Info.nType;
			}

			// Grid에서 Info 데이터 추출
			T_TDPF_INFO& info = pttnBase.Info;

			info.strBoundaryCondition = m_pProfileGrid->GetValueRowCol(row, EN_COL_BOUNDARY_CONDITION);

			// Shape Type: 원본 데이터에서 이미 복사됨 (위에서 nType 설정)
			CString strShape = m_pProfileGrid->GetValueRowCol(row, EN_COL_SHAPE);

			info.dYa = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YA));
			info.dYc = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YC));
			info.dYd = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YD));
			info.dYb = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_CGS_YB));
			info.dXa = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_A));
			info.dXc = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_C));
			info.dXd = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_D));
			info.dXb = _ttof(m_pProfileGrid->GetValueRowCol(row, EN_COL_X_B));

			Data.aInfo.Add(pttnBase);
		}
	}
}

void CPostTensionModifyTendonVerDlg::DrawDefaultGuide()
{
	// 기본 축 그리기
	gdiplus_draw::TGdiplusShapeLine axisX;
	axisX.poStt.dx = 0.0;   axisX.poStt.dy = 0.0;
	axisX.poEnd.dx = 100.0; axisX.poEnd.dy = 0.0;
	axisX.SetColor(180, 180, 180);
	m_wndPic.AddEntity(&axisX);

	gdiplus_draw::TGdiplusShapeLine axisY;
	axisY.poStt.dx = 0.0;  axisY.poStt.dy = -25.0;
	axisY.poEnd.dx = 0.0;  axisY.poEnd.dy = 25.0;
	axisY.SetColor(180, 180, 180);
	m_wndPic.AddEntity(&axisY);

	// 안내 메시지
	gdiplus_draw::TGdiplusShapeText text;
	text.poInsert.dx = 50.0;
	text.poInsert.dy = 0.0;
	text.strContent = _T("Select a tendon");
	text.SetColor(128, 128, 128);
	m_wndPic.AddEntity(&text);
}

void CPostTensionModifyTendonVerDlg::DrawNoDataMessage()
{
	// 기본 축 그리기
	gdiplus_draw::TGdiplusShapeLine axisX;
	axisX.poStt.dx = 0.0;   axisX.poStt.dy = 0.0;
	axisX.poEnd.dx = 100.0; axisX.poEnd.dy = 0.0;
	axisX.SetColor(180, 180, 180);
	m_wndPic.AddEntity(&axisX);

	// 안내 메시지
	gdiplus_draw::TGdiplusShapeText text;
	text.poInsert.dx = 50.0;
	text.poInsert.dy = 0.0;
	text.strContent = _T("No profile data");
	text.SetColor(128, 128, 128);
	m_wndPic.AddEntity(&text);
}