// PostTensionModifyTendonHorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "PostTensionModifyTendonHorDlg.h"
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
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonHorProfileGrid - Column Constants
enum EN_HOR_PROFILE_GRID_COL
{
	EN_HOR_COL_NO = 1,
	EN_HOR_COL_X,
	EN_HOR_COL_Y,
	EN_HOR_COL_COUNT = EN_HOR_COL_Y  // Total column count
};

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonHorDlg dialog

CPostTensionModifyTendonHorDlg::CPostTensionModifyTendonHorDlg(CDBDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CChildDialog(CPostTensionModifyTendonHorDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pParent = (CPostTensionModifyTendonDlg*)pParent;
	m_pProfileGrid = nullptr;
	m_PttnKey = 0;
}

CPostTensionModifyTendonHorDlg::~CPostTensionModifyTendonHorDlg()
{
	if (m_pProfileGrid)
	{
		delete m_pProfileGrid;
		m_pProfileGrid = nullptr;
	}
	m_wndPic.DeleteDrawer();
}

void CPostTensionModifyTendonHorDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_CMD_PT_MODIFY_TENDON_HOR_PIC, m_wndPic);
	DDX_Control(pDX, IDD_CMD_PT_MODIFY_TENDON_HOR_PROFILE_TABLE_CST, m_wndProfileTable);
}

BEGIN_MESSAGE_MAP(CPostTensionModifyTendonHorDlg, CChildDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonHorDlg message handlers

BOOL CPostTensionModifyTendonHorDlg::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	// Initialize GdiplusDrawerStatic
	m_wndPic.BindCtr(GetDlgItem(IDD_CMD_PT_MODIFY_TENDON_HOR_PIC));
	m_wndPic.SetBackGroundColor(255, 255, 255);  // White background
	m_wndPic.SetScaleWithZoom(FALSE);            // 선/폰트 크기 고정
	m_wndPic.SetOriginalLineWidth(1.0);          // 선 두께 1픽셀

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

	return TRUE;
}

void CPostTensionModifyTendonHorDlg::CreateProfileGrid()
{
	CRect rect;
	GetDlgItem(IDD_CMD_PT_MODIFY_TENDON_HOR_PROFILE_TABLE_CST)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_pProfileGrid = new CPostTensionModifyTendonHorProfileGrid();
	m_pProfileGrid->SetParentDlg(this);

	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | WS_HSCROLL | WS_VSCROLL;
	m_pProfileGrid->Create(dwStyle, rect, this, IDD_CMD_PT_MODIFY_TENDON_HOR_PROFILE_TABLE_CST);
}

void CPostTensionModifyTendonHorDlg::InitializeProfileGrid()
{
	if (m_pProfileGrid && m_pProfileGrid->GetSafeHwnd())
	{
		m_pProfileGrid->Initialize(m_pDoc);
		m_pProfileGrid->ShowWindow(SW_SHOW);
		m_pProfileGrid->UpdateWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonHorProfileGrid implementation

CPostTensionModifyTendonHorProfileGrid::CPostTensionModifyTendonHorProfileGrid()
{
	m_pParentDlg = nullptr;
	m_pDoc = nullptr;

	// 컬럼 정보 등록 (m_aTitle 배열 설정)
	AddColInfo(_T("No."), t_integer, 0);
	AddColInfo(_T("x"), t_real, 0);
	AddColInfo(_T("y"), t_real, 0);

	m_aPrimaryKey.Add(0);  // 첫 번째 컬럼(No.)을 Key로 지정
}

CPostTensionModifyTendonHorProfileGrid::~CPostTensionModifyTendonHorProfileGrid()
{
}

BEGIN_MESSAGE_MAP(CPostTensionModifyTendonHorProfileGrid, CTBBrowserWndEx)
END_MESSAGE_MAP()

void CPostTensionModifyTendonHorProfileGrid::Initialize(CDBDoc* pDoc)
{
	CTBBrowserWndEx::Initialize();

	m_pDoc = pDoc;

	LockUpdate(TRUE);

	m_csTableName = _T("PostTensionModifyTendonHorProfile");

	SetColCount(EN_HOR_COL_COUNT);  // No., x, y
	SetRecordCount(10);             // 기본 10행 (데이터에 따라 가변)

	EnableAppendRow(FALSE);

	GetParam()->EnableUndo(FALSE);
	EnableScrollTips();
	EnableHints();
	GetParam()->EnableTrackRowHeight(FALSE);
	GetParam()->SetNumberedRowHeaders(TRUE);

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

	SetColumnStyles();

	// 컬럼 너비 설정
	SetColWidth(EN_HOR_COL_NO, EN_HOR_COL_NO, 50);   // No.
	SetColWidth(EN_HOR_COL_X, EN_HOR_COL_X, 80);     // x
	SetColWidth(EN_HOR_COL_Y, EN_HOR_COL_Y, 80);     // y

	LockUpdate(FALSE);
	Redraw();
}

void CPostTensionModifyTendonHorProfileGrid::SetHeaderTitle()
{
	BOOL bOldLock = LockUpdate(TRUE);

	SetStyleRange(CGXRange(0, EN_HOR_COL_NO), CGXStyle()
		.SetValue(_T("No."))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetFont(CGXFont_GC().SetBold(TRUE)));

	SetStyleRange(CGXRange(0, EN_HOR_COL_X), CGXStyle()
		.SetValue(_T("x"))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetFont(CGXFont_GC().SetBold(TRUE)));

	SetStyleRange(CGXRange(0, EN_HOR_COL_Y), CGXStyle()
		.SetValue(_T("y"))
		.SetHorizontalAlignment(DT_CENTER)
		.SetVerticalAlignment(DT_VCENTER)
		.SetFont(CGXFont_GC().SetBold(TRUE)));

	LockUpdate(bOldLock);
	if (!bOldLock) Redraw();
}

void CPostTensionModifyTendonHorProfileGrid::SetColumnStyles()
{
	// No. 컬럼
	SetStyleRange(CGXRange().SetCols(EN_HOR_COL_NO), CGXStyle()
		.SetHorizontalAlignment(DT_CENTER));

	// x, y 컬럼 (숫자)
	SetStyleRange(CGXRange().SetCols(EN_HOR_COL_X, EN_HOR_COL_Y), CGXStyle()
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FIXED)
		.SetPlaces(3));
}

void CPostTensionModifyTendonHorProfileGrid::ClearGrid()
{
	SetRowCount(1);
	Redraw();
}

BOOL CPostTensionModifyTendonHorProfileGrid::OnEndEditing(ROWCOL nRow, ROWCOL nCol)
{
	BOOL bResult = CTBBrowserWndEx::OnEndEditing(nRow, nCol);

	// 부모 다이얼로그에 변경 알림
	if (m_pParentDlg)
	{
		m_pParentDlg->OnProfileDataChanged();
	}

	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonHorDlg - Public methods

void CPostTensionModifyTendonHorDlg::SetPttnKey(T_PTTN_K key)
{
	// 다이얼로그가 초기화되지 않은 경우 UI 업데이트 스킵
	if (!GetSafeHwnd()) return;

	m_PttnKey = key;

	// DB에서 PTTN 데이터 로드
	if (m_PttnKey != 0 && m_pDoc && m_pDoc->m_pAttrCtrl2)
	{
		if (!m_pDoc->m_pAttrCtrl2->GetPttn(m_PttnKey, m_OriginalPttnData))
		{
			m_OriginalPttnData.Initialize();
		}
	}
	else
	{
		m_OriginalPttnData.Initialize();
	}

	// Grid 데이터 업데이트
	UpdateGridData();

	// 프로필 그리기
	DrawProfile();
}

void CPostTensionModifyTendonHorDlg::UpdateGridData()
{
	if (!m_pProfileGrid || !m_pProfileGrid->GetSafeHwnd()) return;

	BOOL bOldLock = m_pProfileGrid->LockUpdate(TRUE);

	// Grid 초기화
	m_pProfileGrid->ClearGrid();

	// aX, aY 사이즈 검증
	int nXSize = (int)m_OriginalPttnData.aX.GetSize();
	int nYSize = (int)m_OriginalPttnData.aY.GetSize();

	if (nXSize != nYSize || nXSize == 0)
	{
		m_pProfileGrid->LockUpdate(bOldLock);
		if (!bOldLock) m_pProfileGrid->Redraw();
		return;
	}

	// Grid 행 수 설정 (SetRecordCount 사용) - 데이터에 따라 가변
	m_pProfileGrid->SetRecordCount(nXSize);

	// 데이터 채우기
	for (int i = 0; i < nXSize; ++i)
	{
		int row = i + 1;

		CString strNo, strX, strY;
		strNo.Format(_T("%d"), row);
		strX.Format(_T("%.6f"), m_OriginalPttnData.aX[i]);
		strY.Format(_T("%.6f"), m_OriginalPttnData.aY[i]);

		m_pProfileGrid->SetExpressionRowCol(row, EN_HOR_COL_NO, strNo);
		m_pProfileGrid->SetExpressionRowCol(row, EN_HOR_COL_X, strX);
		m_pProfileGrid->SetExpressionRowCol(row, EN_HOR_COL_Y, strY);
	}

	m_pProfileGrid->LockUpdate(bOldLock);
	if (!bOldLock) m_pProfileGrid->Redraw();
}

void CPostTensionModifyTendonHorDlg::OnProfileDataChanged()
{
	DrawProfile();
}

/////////////////////////////////////////////////////////////////////////////
// CPostTensionModifyTendonHorDlg - Drawing functions

void CPostTensionModifyTendonHorDlg::DrawProfile()
{
	if (!m_wndPic.GetSafeHwnd()) return;

	m_wndPic.ClearAllEntity();

	//=========================================================================
	// Step 1: 유효성 검사 - 텐던 키와 문서 포인터 확인
	//=========================================================================
	if (m_PttnKey == 0 || !m_pDoc)
	{
		DrawDefaultGuide();
		m_wndPic.ReDraw(TRUE);
		return;
	}

	//=========================================================================
	// Step 2: aX, aY 사이즈 검증
	//=========================================================================
	int nXSize = (int)m_OriginalPttnData.aX.GetSize();
	int nYSize = (int)m_OriginalPttnData.aY.GetSize();

	if (nXSize != nYSize || nXSize < 2)
	{
		DrawNoDataMessage();
		m_wndPic.ReDraw(TRUE);
		return;
	}

	//=========================================================================
	// Step 3: 좌표 범위 계산
	//=========================================================================
	double dMinX = m_OriginalPttnData.aX[0];
	double dMaxX = m_OriginalPttnData.aX[0];
	double dMinY = m_OriginalPttnData.aY[0];
	double dMaxY = m_OriginalPttnData.aY[0];

	for (int i = 1; i < nXSize; ++i)
	{
		double x = m_OriginalPttnData.aX[i];
		double y = m_OriginalPttnData.aY[i];

		if (dgn::LT(x, dMinX)) dMinX = x;
		if (dgn::MT(x, dMaxX)) dMaxX = x;
		if (dgn::LT(y, dMinY)) dMinY = y;
		if (dgn::MT(y, dMaxY)) dMaxY = y;
	}

	double dRangeX = dMaxX - dMinX;
	double dRangeY = dMaxY - dMinY;

	double dMarginX = dgn::MT0(dRangeX) ? dRangeX * 0.1 : 1.0;
	double dMarginY = dgn::MT0(dRangeY) ? dRangeY * 0.1 : 1.0;

	//=========================================================================
	// Step 4: 텐던 프로필 폴리라인 그리기 (파란색)
	//=========================================================================
	gdiplus_draw::TGdiplusShapePolyline polyline;
	polyline.SetColor(0, 0, 192);

	for (int i = 0; i < nXSize; ++i)
	{
		gdiplus_draw::TGdiplusShapePoint pt;
		pt.dx = m_OriginalPttnData.aX[i];
		pt.dy = m_OriginalPttnData.aY[i];
		polyline.vertexs.push_back(pt);
	}
	m_wndPic.AddEntity(&polyline);

	//=========================================================================
	// Step 5: 컨트롤 포인트 표시 (빨간색 채워진 사각형)
	// Span 경계 포인트는 2배 크기로 표시
	//=========================================================================
	double dMinRange = dgn::LT(dRangeX, dRangeY) ? dRangeX : dRangeY;
	if (dgn::LE0(dMinRange)) dMinRange = dgn::MT0(dRangeX) ? dRangeX : dRangeY;
	if (dgn::LE0(dMinRange)) dMinRange = 1.0;
	double dCtrlPtSize = dMinRange * 0.0025;  // 일반 포인트 크기
	if (dgn::LT(dCtrlPtSize, 0.0025)) dCtrlPtSize = 0.0025;  // 최소 크기
	double dSpanPtSize = dCtrlPtSize * 1.5;  // Span 경계 포인트 크기 (1.5배)

	// aIdx를 사용하여 Span 경계 포인트 식별
	int nIdxSize = (int)m_OriginalPttnData.aIdx.GetSize();

	for (int i = 0; i < nXSize; ++i)
	{
		double x = m_OriginalPttnData.aX[i];
		double y = m_OriginalPttnData.aY[i];

		// Span 경계 포인트 여부 판단
		BOOL bIsSpanBoundary = FALSE;
		if (i == 0 || i == nXSize - 1)
		{
			// 첫 번째 또는 마지막 포인트는 항상 Span 경계
			bIsSpanBoundary = TRUE;
		}
		else if (nIdxSize == nXSize && i > 0)
		{
			// aIdx가 유효하고, Span 인덱스가 변경되는 지점
			if (m_OriginalPttnData.aIdx[i] != m_OriginalPttnData.aIdx[i - 1])
			{
				bIsSpanBoundary = TRUE;
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

		// 각 점 위에 순번 표시 (1, 2, 3, ...)
		gdiplus_draw::TGdiplusShapeText textNo;
		textNo.poInsert.dx = x;
		textNo.poInsert.dy = y + dSize * 2.5;  // 점 상단에 위치
		textNo.strContent.Format(_T("%d"), i + 1);  // 1-based 번호
		textNo.SetColor(0, 0, 0);  // 검정색
		textNo.dFontScale = 0.75;  // 폰트 크기 0.75배
		m_wndPic.AddEntity(&textNo);
	}

	//=========================================================================
	// Step 6: 시작점/끝점 레이블 (I, J)
	//=========================================================================
	double dTextOffsetX = dMarginX * 0.5;

	// I 레이블 - 시작점 좌측에 표시
	gdiplus_draw::TGdiplusShapeText textStart;
	textStart.poInsert.dx = m_OriginalPttnData.aX[0] - dTextOffsetX;
	textStart.poInsert.dy = m_OriginalPttnData.aY[0];
	textStart.strContent = _T("I");
	textStart.SetColor(192, 0, 0);
	m_wndPic.AddEntity(&textStart);

	// J 레이블 - 끝점 우측에 표시
	gdiplus_draw::TGdiplusShapeText textEnd;
	textEnd.poInsert.dx = m_OriginalPttnData.aX[nXSize - 1] + dTextOffsetX;
	textEnd.poInsert.dy = m_OriginalPttnData.aY[nXSize - 1];
	textEnd.strContent = _T("J");
	textEnd.SetColor(192, 0, 0);
	m_wndPic.AddEntity(&textEnd);

	//=========================================================================
	// Step 7: 화면 갱신
	//=========================================================================
	m_wndPic.ReDraw(TRUE);
}

void CPostTensionModifyTendonHorDlg::DrawDefaultGuide()
{
	// 기본 축 그리기
	gdiplus_draw::TGdiplusShapeLine axisX;
	axisX.poStt.dx = 0.0;   axisX.poStt.dy = 0.0;
	axisX.poEnd.dx = 100.0; axisX.poEnd.dy = 0.0;
	axisX.SetColor(180, 180, 180);
	m_wndPic.AddEntity(&axisX);

	gdiplus_draw::TGdiplusShapeLine axisY;
	axisY.poStt.dx = 0.0;  axisY.poStt.dy = 0.0;
	axisY.poEnd.dx = 0.0;  axisY.poEnd.dy = 100.0;
	axisY.SetColor(180, 180, 180);
	m_wndPic.AddEntity(&axisY);

	// 안내 메시지
	gdiplus_draw::TGdiplusShapeText text;
	text.poInsert.dx = 50.0;
	text.poInsert.dy = 50.0;
	text.strContent = _T("Select a tendon");
	text.SetColor(128, 128, 128);
	m_wndPic.AddEntity(&text);
}

void CPostTensionModifyTendonHorDlg::DrawNoDataMessage()
{
	// 기본 축 그리기
	gdiplus_draw::TGdiplusShapeLine axisX;
	axisX.poStt.dx = 0.0;   axisX.poStt.dy = 0.0;
	axisX.poEnd.dx = 100.0; axisX.poEnd.dy = 0.0;
	axisX.SetColor(180, 180, 180);
	m_wndPic.AddEntity(&axisX);

	gdiplus_draw::TGdiplusShapeLine axisY;
	axisY.poStt.dx = 0.0;  axisY.poStt.dy = 0.0;
	axisY.poEnd.dx = 0.0;  axisY.poEnd.dy = 100.0;
	axisY.SetColor(180, 180, 180);
	m_wndPic.AddEntity(&axisY);

	// 에러 메시지
	gdiplus_draw::TGdiplusShapeText text;
	text.poInsert.dx = 50.0;
	text.poInsert.dy = 50.0;
	text.strContent = _T("No profile data");
	text.SetColor(128, 128, 128);
	m_wndPic.AddEntity(&text);
}
