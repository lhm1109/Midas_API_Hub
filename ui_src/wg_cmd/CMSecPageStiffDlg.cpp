// CMSecPageStiffDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMSecPageStiffDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_base\wg_base_DlgUtil.h"
#include "..\wg_base\wg_base_I_GridColorMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CCMSecPageStiffDlgGrid member functions
// 
// GetSectCount - 타입에 따른 컬럼 갯수 리턴
// GetColumnTitle - 타입에 따른 컬럼 제목 리턴
// SetColumnStyle - 갯수에 맞게 컬럼 숨기는 함수
// SetStiffnessData - initDialog 에서 Stiffness 값 세팅

BOOL CCMSecPageStiffDlgGrid::ProcessKeys(CWnd *pSender, UINT nMessage, UINT nChar, UINT nRepCnt, UINT flags)
{
	CRowColArray awRows;
	ROWCOL nRow=1, nCol=1;
	CGXControl* pControl;

	if (!GetCurrentCell(nRow, nCol)) // if no current cell, default action
		return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);

	pControl = GetControl(nRow, nCol);
	if (nMessage == WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_TAB: 
			if (TabKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		case VK_RETURN: case VK_RIGHT:  // 다음 셀로 이동
			if (RightKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		case VK_LEFT: // 이전 셀로 이동
			if (LeftKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		}
	}
	else if (nMessage == WM_CHAR)
	{
		switch(nChar)
		{
		// 셀이 Inactive 상태에서 Edit 셀에 스페이스를 눌렀을 때 편집을 활성화 시킨다.
		case VK_SPACE:
			if (SpaceKeyPressed(nRow, nCol, pControl)) return TRUE;
			break;
		}
	}
	return CGXGridWnd::ProcessKeys(pSender, nMessage, nChar, nRepCnt, flags);
}


BOOL CCMSecPageStiffDlgGrid::TabKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	GetParent()->GetDlgItem(IDC_CMD_SP_SD_BTN_OK)->SetFocus();
	return TRUE;
}

BOOL CCMSecPageStiffDlgGrid::LeftKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetLeftValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMSecPageStiffDlgGrid::RightKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (SetRightValidCell(nRow, nCol)) return TRUE;
	return TRUE;
}

BOOL CCMSecPageStiffDlgGrid::SpaceKeyPressed(ROWCOL nRow, ROWCOL nCol, CGXControl* pControl)
{
	if (pControl && !pControl->IsActive() 
			&& pControl->CGXControl::IsKindOf(CONTROL_CLASS(CGXEditControl)))
	{
		if (!pControl->IsInit()) pControl->Init(nRow, nCol);
		CString value = GetExpressionRowCol(nRow, nCol);
		pControl->SetActive(TRUE);
		pControl->Refresh(); // 반드시 호출

		if (!pControl->IsKindOf(CONTROL_CLASS(CGXComboBox)))  // combo box가 아니면
			((CGXEditControl*)pControl)->SetSel(0, value.GetLength());
		return TRUE;
	}
	return FALSE;
}

BOOL CCMSecPageStiffDlgGrid::SetLeftValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol-1; // 현재 행 이전 열
	if (j < 1) { i--; j = GetColCount(); }  

	while (i >= 1)
	{
		while (j >= 1)
		{
			// 열이 선택될 수 있는지 조사

			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				SetCurrentCell(i, j);
				return TRUE;
			}
			j--;  // 이전 열을 조사


		}
		i--;  // 이전 줄로 이동
		j = GetColCount();
	}

	return FALSE;
}

BOOL CCMSecPageStiffDlgGrid::SetRightValidCell(ROWCOL nRow, ROWCOL nCol)
{
	ROWCOL i, j;
	i = nRow; j = nCol+1; // 현재 행 다음 열
	if (j > GetColCount()) { i++; j = 1; }

	while (i <= GetRowCount())
	{
		while (j <= GetColCount())
		{
			// 열이 선택될 수 있는지 조사


			//if (SetCurrentCell(i, j)) return TRUE;
			CGXStyle style;
			ComposeStyleRowCol(i, j, &style);
			if (!style.GetIncludeEnabled() || style.GetEnabled())
			{
				SetCurrentCell(i, j);
				return TRUE;
			}
			j++;  // 다음 열을 조사


		}
		i++;  // 다음 줄로 이동
		j = 1;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CCMStiffDlg dialog
// 이 다이얼로그는 Stiffnes를 보여주기 위한 다이얼로그이다.
CCMStiffDlg::CCMStiffDlg(CDBDoc* pDoc,  T_SECT_D* pSectData, 
												 CArray<T_SECT_STIFFNESS, T_SECT_STIFFNESS&>* pOrgStiff, CWnd* pParent)
	: CDialogMove(CCMStiffDlg::IDD, pParent)
{
	m_pDoc = pDoc;
	m_pSectData = pSectData;
	m_paOrgStiff = pOrgStiff;
	m_bSectPSC = (m_pSectData->nStype == D_SECT_TYPE_PSC || 
								m_pSectData->SectBefore.nStype == D_SECT_TYPE_PSC     );
//                 m_pSectData->SectBefore.nStype ==D_SECT_TYPE_COMPO_CI ||
//                 m_pSectData->SectBefore.nStype ==D_SECT_TYPE_COMPO_CT ||
//                 m_pSectData->SectBefore.nStype ==D_SECT_TYPE_COMPO_PC ||
//                 m_pSectData->SectBefore.nStype ==D_SECT_TYPE_COMPO_G  );

	// Composite Tapered
	m_bCmpTap = (m_pSectData->nStype==D_SECT_TYPE_TAPERED && 
								 (m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_B  ||
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_I  ||
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_TUB||
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_B   ||
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_I   ||
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_TUB ||
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_CI ||
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_CT ||
									/*m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_PC ||*/
									m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_G)
								);
	// Composite Tapered - Box or I
	m_bCmpTapUseMulti = (m_pSectData->nStype==D_SECT_TYPE_TAPERED && 
						(m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_B        ||
						m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_I         ||
						m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_TUB       ||
						m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_B    ||
						m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_I    ||
						m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_STLG_TUB  ||
						m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_CI        ||
						m_pSectData->SectBefore.nStype==D_SECT_TYPE_COMPO_CT) );

	if (m_pSectData->SectBefore.Shape == D_SECT_SHAPE_PSC_VALUE)
		m_bSectPSC = FALSE;   // PSC-Value는 x
	if (m_paOrgStiff == NULL) m_bSectPSC = FALSE; 

	m_aCtrlIJ.RemoveAll();
	m_aCtrlIJ.Add(IDC_CMD_I_BTN);
	m_aCtrlIJ.Add(IDC_CMD_J_BTN);

	//{{AFX_DATA_INIT(CCMStiffDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCMStiffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStiffDlg)
	DDX_Control(pDX, IDC_CMD_USER_CHECK, m_wndUserCheck);
	DDX_Control(pDX, IDC_CMD_SP_SD_BTN_SAVE, m_wndSaveBtn);
	DDX_Control(pDX, IDC_CMD_SP_SD_BTN_OK, m_wndCloseBtn);
	DDX_Control(pDX, IDC_CMD_SP_SD_GRID, m_wndGrid);
	// DDX_Control(pDX, IDC_CMD_I_BTN, m_wndIBtn);
	// DDX_Control(pDX, IDC_CMD_J_BTN, m_wndJBtn);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMStiffDlg implementation

void CCMStiffDlg::SetTitleInGrid()
{
	if(m_pSectData->SectBefore.bConsiderWarpingEffect)
	{
		CString aTitle[] = {
			_LS(IDS_WG_CMD__ADDD__Area), _LS(IDS_WG_CMD__ADDD__Asy), _LS(IDS_WG_CMD__ADDD__Asz), _LS(IDS_WG_CMD__ADDD__Ixx), _LS(IDS_WG_CMD__ADDD__Iyy), _LS(IDS_WG_CMD__ADDD__Izz),  
				_LS(IDS_WG_CMD__ADDD__Cyp), _LS(IDS_WG_CMD__ADDD__Cym), _LS(IDS_WG_CMD__ADDD__Czp), _LS(IDS_WG_CMD__ADDD__Czm), _LS(IDS_WG_CMD__ADDD__Qyb), _LS(IDS_WG_CMD__ADDD__Qzb), 
				_LS(IDS_WG_CMD__ADDD__PeriOut), _LS(IDS_WG_CMD__ADDD__PeriIn), _LS(IDS_WG_CMD__ADDD__Center_Y), _LS(IDS_WG_CMD__ADDD__Center_Z),
				_T("y1"), _T("z1"), _T("y2"), _T("z2"), _T("y3"), _T("z3"), _T("y4"), _T("z4"),
				_T("Iw"), _T("w1"), _T("w2"), _T("w3"), _T("w4"),
				_T("Cxy1"), _T("Cxy2"), _T("Cxy3"), _T("Cxy4"),
				_T("Cxz1"), _T("Cxz2"), _T("Cxz3"), _T("Cxz4"),
				_T("ys-yc"), _T("zs-zc"), _T("Ip")
		};
		
		int nNum = sizeof(aTitle)/sizeof(CString);
		for (int i = 0; i < nNum; i++) 
			m_wndGrid.SetValueRange(CGXRange(i+1, 0), aTitle[i]);
	}
	else
	{
		CString aTitle[] = {
			_LS(IDS_WG_CMD__ADDD__Area), _LS(IDS_WG_CMD__ADDD__Asy), _LS(IDS_WG_CMD__ADDD__Asz), _LS(IDS_WG_CMD__ADDD__Ixx), _LS(IDS_WG_CMD__ADDD__Iyy), _LS(IDS_WG_CMD__ADDD__Izz),  
				_LS(IDS_WG_CMD__ADDD__Cyp), _LS(IDS_WG_CMD__ADDD__Cym), _LS(IDS_WG_CMD__ADDD__Czp), _LS(IDS_WG_CMD__ADDD__Czm), _LS(IDS_WG_CMD__ADDD__Qyb), _LS(IDS_WG_CMD__ADDD__Qzb), 
				_LS(IDS_WG_CMD__ADDD__PeriOut), _LS(IDS_WG_CMD__ADDD__PeriIn), _LS(IDS_WG_CMD__ADDD__Center_Y), _LS(IDS_WG_CMD__ADDD__Center_Z),
				_T("y1"), _T("z1"), _T("y2"), _T("z2"), _T("y3"), _T("z3"), _T("y4"), _T("z4")
		};
						
		int nNum = sizeof(aTitle)/sizeof(CString);
		for (int i = 0; i < nNum; i++) 
			m_wndGrid.SetValueRange(CGXRange(i+1, 0), aTitle[i]);
	}

	CStringArray aColHeader;
	GetColumnTitle(aColHeader);
	for (int i = 0; i < aColHeader.GetSize(); i++)
		m_wndGrid.SetValueRange(CGXRange(0, i+1), aColHeader[i]);
}

void CCMStiffDlg::SetUnitInGrid()
{
	CString aUnit[24];

	T_SECT_STIFFNESS_UNIT *pUnit = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI.Stiffness);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Area, aUnit[0]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Asy, aUnit[1]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Asz, aUnit[2]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Rxx, aUnit[3]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Ryy, aUnit[4]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Rzz, aUnit[5]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Cyp, aUnit[6]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Cym, aUnit[7]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Czp, aUnit[8]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Czm, aUnit[9]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Qyb, aUnit[10]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->Qzb, aUnit[11]);

	T_SECT_SECTBASE_UNIT *pUnit2 = &(CUnitCtrl::m_SECT_UNIT.SectBefore.SectI);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->PeriOut, aUnit[12]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->PeriIn, aUnit[13]);

	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->Design.YBar, aUnit[14]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit2->Design.ZBar, aUnit[15]);

	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx1, aUnit[16]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy1, aUnit[17]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx2, aUnit[18]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy2, aUnit[19]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx3, aUnit[20]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy3, aUnit[21]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dx4, aUnit[22]);
	m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dy4, aUnit[23]);

	for (int i = 0; i < 24; i++)
		m_wndGrid.SetValueRange(CGXRange(i+1, GetUnitCol()), aUnit[i]);

	if(m_pSectData->SectBefore.bConsiderWarpingEffect)
	{
		CString aUnit[16];
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dIw,	aUnit[0]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dw1,	aUnit[1]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dw2,	aUnit[2]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dw3,	aUnit[3]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dw4,	aUnit[4]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxy1, aUnit[5]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxy2, aUnit[6]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxy3, aUnit[7]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxy4, aUnit[8]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxz1, aUnit[9]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxz2, aUnit[10]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxz3, aUnit[11]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dxz4, aUnit[12]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dYsy, aUnit[13]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dYsz, aUnit[14]);
		m_pDoc->m_pUnitCtrl->GetUnitSystem(pUnit->dIp, aUnit[15]);
		for (int i = 0; i < 16; i++)
			m_wndGrid.SetValueRange(CGXRange(i+25, GetUnitCol()), aUnit[i]);
	}
}

void CCMStiffDlg::SetStiffnessData(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness, double PeriIn, double PeriOut, double cy, double cz)
{
	SetStiffnessDataSub(nCol, pStiffness);

	m_wndGrid.SetValueRange(CGXRange(7, nCol),  pStiffness->Cyp);
	m_wndGrid.SetValueRange(CGXRange(8, nCol),  pStiffness->Cym);
	m_wndGrid.SetValueRange(CGXRange(9, nCol),  pStiffness->Czp);
	m_wndGrid.SetValueRange(CGXRange(10, nCol), pStiffness->Czm);
	m_wndGrid.SetValueRange(CGXRange(11, nCol), pStiffness->Qyb);
	m_wndGrid.SetValueRange(CGXRange(12, nCol), pStiffness->Qzb);

	m_wndGrid.SetValueRange(CGXRange(13, nCol), PeriOut);
	m_wndGrid.SetValueRange(CGXRange(14, nCol), PeriIn);

	m_wndGrid.SetValueRange(CGXRange(15, nCol), cy);
	m_wndGrid.SetValueRange(CGXRange(16, nCol), cz);

	m_wndGrid.SetValueRange(CGXRange(17, nCol), pStiffness->dx1);
	m_wndGrid.SetValueRange(CGXRange(18, nCol), pStiffness->dy1);
	m_wndGrid.SetValueRange(CGXRange(19, nCol), pStiffness->dx2);
	m_wndGrid.SetValueRange(CGXRange(20, nCol), pStiffness->dy2);
	m_wndGrid.SetValueRange(CGXRange(21, nCol), pStiffness->dx3);
	m_wndGrid.SetValueRange(CGXRange(22, nCol), pStiffness->dy3);
	m_wndGrid.SetValueRange(CGXRange(23, nCol), pStiffness->dx4);
	m_wndGrid.SetValueRange(CGXRange(24, nCol), pStiffness->dy4);

	if(m_pSectData->SectBefore.bConsiderWarpingEffect)
	{
		m_wndGrid.SetValueRange(CGXRange(25, nCol), pStiffness->dIw);
		m_wndGrid.SetValueRange(CGXRange(26, nCol), pStiffness->dw1);
		m_wndGrid.SetValueRange(CGXRange(27, nCol), pStiffness->dw2);
		m_wndGrid.SetValueRange(CGXRange(28, nCol), pStiffness->dw3);
		m_wndGrid.SetValueRange(CGXRange(29, nCol), pStiffness->dw4);
		m_wndGrid.SetValueRange(CGXRange(30, nCol), pStiffness->dxy1);
		m_wndGrid.SetValueRange(CGXRange(31, nCol), pStiffness->dxy2);
		m_wndGrid.SetValueRange(CGXRange(32, nCol), pStiffness->dxy3);
		m_wndGrid.SetValueRange(CGXRange(33, nCol), pStiffness->dxy4);
		m_wndGrid.SetValueRange(CGXRange(34, nCol), pStiffness->dxz1);
		m_wndGrid.SetValueRange(CGXRange(35, nCol), pStiffness->dxz2);
		m_wndGrid.SetValueRange(CGXRange(36, nCol), pStiffness->dxz3);
		m_wndGrid.SetValueRange(CGXRange(37, nCol), pStiffness->dxz4);
		m_wndGrid.SetValueRange(CGXRange(38, nCol), pStiffness->dYsy - pStiffness->dYcy);
		m_wndGrid.SetValueRange(CGXRange(39, nCol), pStiffness->dYsz - pStiffness->dYcz);
		m_wndGrid.SetValueRange(CGXRange(40, nCol), pStiffness->dIp);
	}
}

void CCMStiffDlg::SetStiffnessDataNull(int nCol) // added by mylee
{
	for (int i=1 ; i<=24 ; ++i)
		m_wndGrid.SetValueRange(CGXRange(i, nCol),  _T("-"));

	if(m_pSectData->SectBefore.bConsiderWarpingEffect)
	{
		for (int i = 1; i <= 16; i++)
			m_wndGrid.SetValueRange(CGXRange(i+24, nCol), _T("-"));
	}
}

void CCMStiffDlg::SetStiffnessDataSub(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness)
{
	m_wndGrid.SetValueRange(CGXRange(1, nCol),  pStiffness->Area);
	m_wndGrid.SetValueRange(CGXRange(2, nCol),  pStiffness->Asy);
	m_wndGrid.SetValueRange(CGXRange(3, nCol),  pStiffness->Asz);
	m_wndGrid.SetValueRange(CGXRange(4, nCol),  pStiffness->Rxx);
	m_wndGrid.SetValueRange(CGXRange(5, nCol),  pStiffness->Ryy);
	m_wndGrid.SetValueRange(CGXRange(6, nCol),  pStiffness->Rzz);
}

BOOL CCMStiffDlg::GetStiffnessData(ROWCOL nCol, T_SECT_STIFFNESS* pStiffness, double PeriIn, double PeriOut, double cy, double cz)
{
	CString value;
	value = m_wndGrid.GetValueRowCol( 1, nCol); pStiffness->Area = _tstof(value);
	value = m_wndGrid.GetValueRowCol( 2, nCol); pStiffness->Asy  = _tstof(value);
	value = m_wndGrid.GetValueRowCol( 3, nCol); pStiffness->Asz  = _tstof(value);
	value = m_wndGrid.GetValueRowCol( 4, nCol); pStiffness->Rxx  = _tstof(value);
	value = m_wndGrid.GetValueRowCol( 5, nCol); pStiffness->Ryy  = _tstof(value);
	value = m_wndGrid.GetValueRowCol( 6, nCol); pStiffness->Rzz  = _tstof(value);

	pStiffness->WArea = pStiffness->Area;
	
	return TRUE;
}

int CCMStiffDlg::GetSectCount()
{

	switch(m_pSectData->nStype)
	{
	case D_SECT_TYPE_REGULAR: return 1;
	case D_SECT_TYPE_USER: return 1;
	case D_SECT_TYPE_SRC: return 1;
	case D_SECT_TYPE_COMBINED: return 1;
	case D_SECT_TYPE_HIBEAM: return 1;
	case D_SECT_TYPE_CONSTRUCTION: return 2;
	case D_SECT_TYPE_COMPO_B: // MNET:2582 20061129 mylee
	case D_SECT_TYPE_COMPO_I: 
	case D_SECT_TYPE_COMPO_TUB:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_TUB:
	case D_SECT_TYPE_COMPO_CI:
	case D_SECT_TYPE_COMPO_CT:
	case D_SECT_TYPE_COMPO_PC:
		if (m_pSectData->SectBefore.bMultipleElastic) return 4; 
		else return 2;	
	case D_SECT_TYPE_COMPO_G: return 2;
	case D_SECT_TYPE_COLDFORMED: return 1;
	case D_SECT_TYPE_TAPERED:
		if (m_bCmpTap)
		{
			if (m_bCmpTapUseMulti && m_pSectData->SectBefore.bMultipleElastic==TRUE) return 4; // MNET:XXXX 20070226 mylee
			else return 2;
		}
		else return 2;
	case D_SECT_TYPE_PSC:     return 1;
	case D_SECT_TYPE_STLG_B: return 1;
	case D_SECT_TYPE_STLG_I: return 1;
	case D_SECT_TYPE_STLG_MCELL: return 1;
	}
	ASSERT(0);
	return 0;
}

// Col 의 수는 항상 8+1 개 - 각 섹션 타입에 맞는 제목 리턴
void CCMStiffDlg::GetColumnTitle(CStringArray &aTitle)
{
	switch(m_pSectData->nStype)
	{
	case D_SECT_TYPE_REGULAR: 
	case D_SECT_TYPE_USER:
	case D_SECT_TYPE_SRC:
	case D_SECT_TYPE_COMBINED:
	case D_SECT_TYPE_HIBEAM:
	case D_SECT_TYPE_COLDFORMED:
	case D_SECT_TYPE_PSC:
	case D_SECT_TYPE_STLG_B:
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_MCELL:
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__ValueONLY)); 
		aTitle.Add(_T("")); aTitle.Add(_T("")); aTitle.Add(_T("")); 
		aTitle.Add(_T("")); aTitle.Add(_T("")); aTitle.Add(_T("")); aTitle.Add(_T(""));
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Unit));
		return;
	case D_SECT_TYPE_TAPERED:
		if (m_bCmpTap)
		{
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Before_I_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__After_I_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__LongTerm_I_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Shrinkage_I_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Before_J_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__After_J_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__LongTerm_J_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Shrinkage_J_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Unit));
		}
		else
		{
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Value_I_));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Value_J_));
			aTitle.Add(_T("")); aTitle.Add(_T(""));
			aTitle.Add(_T("")); aTitle.Add(_T("")); aTitle.Add(_T("")); aTitle.Add(_T(""));
			aTitle.Add(_LS(IDS_WG_CMD__ADDD__Unit));
		}
		return; 
	case D_SECT_TYPE_CONSTRUCTION:
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_TUB:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_TUB:
	case D_SECT_TYPE_COMPO_CI:
	case D_SECT_TYPE_COMPO_CT:
	case D_SECT_TYPE_COMPO_PC:
	case D_SECT_TYPE_COMPO_G:
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Value_Before_));
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Value_After_));
		aTitle.Add(_LS(IDS_CMD_SECF_LONGTERM)); 
		aTitle.Add(_LS(IDS_CMD_SECF_SHRINKAGE));
		aTitle.Add(_T("")); aTitle.Add(_T("")); aTitle.Add(_T("")); aTitle.Add(_T(""));
		aTitle.Add(_LS(IDS_WG_CMD__ADDD__Unit));
		return;
	}
}

ROWCOL CCMStiffDlg::GetUnitCol()
{
	return 9;
}

void CCMStiffDlg::GetStiffAndPeri(int nIndex, T_SECT_STIFFNESS* &ppStiff, double &PeriIn, double &PeriOut, double& cy, double& cz)
{
	// Composite Box와 I타입시 3, 4 열 추가
	BOOL bNewCase = ((nIndex==2 || nIndex==3) && (m_pSectData->nStype==D_SECT_TYPE_COMPO_B      || m_pSectData->nStype==D_SECT_TYPE_COMPO_I      || m_pSectData->nStype==D_SECT_TYPE_COMPO_TUB ||
		                                          m_pSectData->nStype==D_SECT_TYPE_COMPO_STLG_B || m_pSectData->nStype==D_SECT_TYPE_COMPO_STLG_I || m_pSectData->nStype==D_SECT_TYPE_COMPO_STLG_TUB || 
												  m_pSectData->nStype==D_SECT_TYPE_COMPO_CI     || m_pSectData->nStype==D_SECT_TYPE_COMPO_CT     || m_pSectData->nStype==D_SECT_TYPE_COMPO_PC));

	// Tapered Composite : 5~8 열 추가
	BOOL bNewCase2 = (nIndex <= 7 && m_bCmpTap);

	ASSERT(nIndex == 0 || nIndex == 1 || bNewCase || bNewCase2);

	switch(m_pSectData->nStype)
	{
	case D_SECT_TYPE_REGULAR:
	case D_SECT_TYPE_USER:
	case D_SECT_TYPE_COMBINED:
	case D_SECT_TYPE_HIBEAM:
	case D_SECT_TYPE_COLDFORMED:
	case D_SECT_TYPE_PSC:
	case D_SECT_TYPE_STLG_B:
	case D_SECT_TYPE_STLG_I:
	case D_SECT_TYPE_STLG_MCELL:
		ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, FALSE);
		PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, FALSE);
		PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, FALSE);
		cy = *CSectUtil::GetStandForYBar(*m_pSectData, FALSE);
		cz = *CSectUtil::GetStandForZBar(*m_pSectData, FALSE);
		break;

	case D_SECT_TYPE_SRC:
		{
			BOOL bAfter = FALSE;
			if(m_pSectData->SectBefore.Shape == D_SECT_SHAPE_SRC_STUB || m_pSectData->SectBefore.Shape == D_SECT_SHAPE_SRC_DUMB)
				bAfter = TRUE;

			ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, bAfter);
			PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, bAfter);
			PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, bAfter);
			cy = *CSectUtil::GetStandForYBar(*m_pSectData, bAfter);
			cz = *CSectUtil::GetStandForZBar(*m_pSectData, bAfter);
		}
		break;
	case D_SECT_TYPE_TAPERED:
		if (!m_bCmpTap) // 기존 Tapered
		{
			if (nIndex == 0)
			{
				ppStiff = &(m_pSectData->SectBefore.SectI.Stiffness);
				PeriIn = m_pSectData->SectBefore.SectI.PeriIn;
				PeriOut = m_pSectData->SectBefore.SectI.PeriOut;
				cy = m_pSectData->SectBefore.SectI.Design.YBar;
				cz = m_pSectData->SectBefore.SectI.Design.ZBar;
			}
			else if (nIndex == 1)
			{
				ppStiff = &(m_pSectData->SectBefore.SectJ.Stiffness);
				PeriIn = m_pSectData->SectBefore.SectJ.PeriIn;
				PeriOut = m_pSectData->SectBefore.SectJ.PeriOut;
				cy = m_pSectData->SectBefore.SectJ.Design.YBar;
				cz = m_pSectData->SectBefore.SectJ.Design.ZBar;
			}
		}
		else // (NEW) Composite Tapered 
		{
			if (nIndex == 0) // Before I
			{
				ppStiff = &(m_pSectData->SectBefore.SectI.Stiffness);
				PeriIn = m_pSectData->SectBefore.SectI.PeriIn;
				PeriOut = m_pSectData->SectBefore.SectI.PeriOut;
				cy = m_pSectData->SectBefore.SectI.Design.YBar;
				cz = m_pSectData->SectBefore.SectI.Design.ZBar;
			}
			if (nIndex == 1) // After I
			{
				ppStiff = &(m_pSectData->SectAfter.SectI.Stiffness);
				PeriIn = m_pSectData->SectAfter.SectI.PeriIn;
				PeriOut = m_pSectData->SectAfter.SectI.PeriOut;
				cy = m_pSectData->SectAfter.SectI.Design.YBar;
				cz = m_pSectData->SectAfter.SectI.Design.ZBar;
			}
			
			if (nIndex == 2) // long term I
			{
				ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, TRUE, 1, 'I');
				PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, TRUE, 1, 'I');
				PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, TRUE, 1, 'I');
				cy = *CSectUtil::GetStandForYBar(*m_pSectData, TRUE, 1, 'I');
				cz = *CSectUtil::GetStandForZBar(*m_pSectData, TRUE, 1, 'I');
			}
			else if (nIndex == 3) // shrinkage I
			{
				ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, TRUE, 2, 'I');
				PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, TRUE, 2, 'I');
				PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, TRUE, 2, 'I');
				cy = *CSectUtil::GetStandForYBar(*m_pSectData, TRUE, 2, 'I');
				cz = *CSectUtil::GetStandForZBar(*m_pSectData, TRUE, 2, 'I');
			}
			else if (nIndex == 4) // Before J
			{
				ppStiff = &(m_pSectData->SectBefore.SectJ.Stiffness);
				PeriIn = m_pSectData->SectBefore.SectJ.PeriIn;
				PeriOut = m_pSectData->SectBefore.SectJ.PeriOut;
				cy = m_pSectData->SectBefore.SectJ.Design.YBar;
				cz = m_pSectData->SectBefore.SectJ.Design.ZBar;
			}
			else if (nIndex == 5) // After J
			{
				ppStiff = &(m_pSectData->SectAfter.SectJ.Stiffness);
				PeriIn = m_pSectData->SectAfter.SectJ.PeriIn;
				PeriOut = m_pSectData->SectAfter.SectJ.PeriOut;
				cy = m_pSectData->SectAfter.SectJ.Design.YBar;
				cz = m_pSectData->SectAfter.SectJ.Design.ZBar;
			}
			if (nIndex == 6) // long term J
			{
				ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, TRUE, 1, 'J');
				PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, TRUE, 1, 'J');
				PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, TRUE, 1, 'J');
				cy = *CSectUtil::GetStandForYBar(*m_pSectData, TRUE, 1, 'J');
				cz = *CSectUtil::GetStandForZBar(*m_pSectData, TRUE, 1, 'J');
			}
			else if (nIndex == 7) // shrinkage J
			{
				ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, TRUE, 2, 'J');
				PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, TRUE, 2, 'J');
				PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, TRUE, 2, 'J');
				cy = *CSectUtil::GetStandForYBar(*m_pSectData, TRUE, 2, 'J');
				cz = *CSectUtil::GetStandForZBar(*m_pSectData, TRUE, 2, 'J');
			}
		}
		break;
	case D_SECT_TYPE_CONSTRUCTION:
	case D_SECT_TYPE_COMPO_B:
	case D_SECT_TYPE_COMPO_I:
	case D_SECT_TYPE_COMPO_TUB:
	case D_SECT_TYPE_COMPO_STLG_B:
	case D_SECT_TYPE_COMPO_STLG_I:
	case D_SECT_TYPE_COMPO_STLG_TUB:
	case D_SECT_TYPE_COMPO_CI:
	case D_SECT_TYPE_COMPO_CT:
	case D_SECT_TYPE_COMPO_PC:
	case D_SECT_TYPE_COMPO_G:
		if (nIndex == 0)
		{
			ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, FALSE);
			PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, FALSE);
			PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, FALSE);
			cy = *CSectUtil::GetStandForYBar(*m_pSectData, FALSE);
			cz = *CSectUtil::GetStandForZBar(*m_pSectData, FALSE);
		}
		else if (nIndex == 1)
		{
			ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, TRUE);
			PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, TRUE);
			PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, TRUE);
			cy = *CSectUtil::GetStandForYBar(*m_pSectData, TRUE);
			cz = *CSectUtil::GetStandForZBar(*m_pSectData, TRUE);
		}
		if (nIndex == 2) // long term
		{
			ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, TRUE, 1);
			PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, TRUE, 1);
			PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, TRUE, 1);
			cy = *CSectUtil::GetStandForYBar(*m_pSectData, TRUE, 1);
			cz = *CSectUtil::GetStandForZBar(*m_pSectData, TRUE, 1);
		}
		else if (nIndex == 3) // shrinkage
		{
			ppStiff = CSectUtil::GetProperStiffPosition(*m_pSectData, TRUE, 2);
			PeriIn = *CSectUtil::GetStandForPeriIn(*m_pSectData, TRUE, 2);
			PeriOut = *CSectUtil::GetStandForPeriOut(*m_pSectData, TRUE, 2);
			cy = *CSectUtil::GetStandForYBar(*m_pSectData, TRUE, 2);
			cz = *CSectUtil::GetStandForZBar(*m_pSectData, TRUE, 2);
		}
		break;
	}
}

void CCMStiffDlg::SetStiffnessData()
{
	T_SECT_STIFFNESS* pStiff;
	double PeriIn, PeriOut, cy, cz;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	for (int i = 0; i < GetSectCount(); i++)
	{ 
		if (i<=1)
		{
			GetStiffAndPeri(i, pStiff, PeriIn, PeriOut, cy, cz);
			SetStiffnessData(i+1, pStiff, PeriIn, PeriOut, cy, cz); // colnum + 1 : due to index col
		}
		else if (i==2 || i==3) // col 3,4: long term, shrinkage
		{
			if (m_pSectData->SectBefore.bMultipleElastic==1)
			{
				GetStiffAndPeri(i, pStiff, PeriIn, PeriOut, cy, cz);
				SetStiffnessData(i+1, pStiff, PeriIn, PeriOut, cy, cz); // colnum + 1 : due to index col
			}
			else
			{
				SetStiffnessDataNull(i+1);
			}
		}
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

void CCMStiffDlg::SetStiffnessDataCmpTap()
{
	T_SECT_STIFFNESS* pStiff;
	double PeriIn, PeriOut, cy, cz;
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);

	for (int i = 0; i < 8; i++)
	{ 
		if (i==0 || i==1 || i==4 || i==5)
		{
			GetStiffAndPeri(i, pStiff, PeriIn, PeriOut, cy, cz);
			SetStiffnessData(i+1, pStiff, PeriIn, PeriOut, cy, cz); // colnum + 1 : due to index col
		}
		else if (i==2 || i==3 || i==6 || i==7) // long term, shrinkage
		{
			if (m_pSectData->SectBefore.bMultipleElastic)
			{
				GetStiffAndPeri(i, pStiff, PeriIn, PeriOut, cy, cz);
				SetStiffnessData(i+1, pStiff, PeriIn, PeriOut, cy, cz); 
			}
		}
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();
}

BOOL CCMStiffDlg::GetStiffnessData()
{
	ASSERT(m_bSectPSC);

	T_SECT_STIFFNESS* pStiff;
	double PeriIn, PeriOut, cy, cz;
	for (int i = 0; i < GetSectCount(); i++)
	{
		GetStiffAndPeri(i, pStiff, PeriIn, PeriOut, cy, cz);
		if (!GetStiffnessData(i+1, pStiff, PeriIn, PeriOut, cy, cz)) 
			return FALSE;
	}

	return TRUE;
}

// 컬럼수는 유닛 컬럼 포함 항상 5개이나
// 이곳에서 GetSectCount 와 GetUnitCol 사이의 것들을 폭 0 으로 숨긴다
void CCMStiffDlg::SetColumnStyle()
{
	if (m_bCmpTap) return;

	ROWCOL nStartCol = GetSectCount(); // last data col
	ROWCOL nEndCol = GetUnitCol()-1; // last col before unit
	if (nStartCol == nEndCol) return;

	m_wndGrid.SetColWidth(nStartCol+1, nEndCol, globalUtils.ScaleByDPI(0));
	for (int i = 0; i < 24; i++)
	{
		m_wndGrid.SetCoveredCellsRowCol(i, nStartCol, i, nEndCol);
	}
	if(m_pSectData->SectBefore.bConsiderWarpingEffect)
	{
		for (int i = 0; i < 16; i++)
		{
			m_wndGrid.SetCoveredCellsRowCol(i+24, nStartCol, i+24, nEndCol);
		}
	}
	m_wndGrid.SetStyleRange(CGXRange().SetCols(nStartCol+1, nEndCol), CGXStyle()
		.SetEnabled(FALSE));
}

// CmpTap 일때는 위 함수 쓰지 않고 아래함수만 쓴다
void CCMStiffDlg::SetColumnStyleCmpTap(int n) // 0=I, 1=J
{
	ASSERT(n==0 || n==1);
	if (!m_bCmpTap) return;

	if (n==0)
	{
		m_wndGrid.HideCols(1, 2, FALSE);
		m_wndGrid.HideCols(3, 4, !(m_bCmpTapUseMulti && m_pSectData->SectBefore.bMultipleElastic));
		m_wndGrid.HideCols(5, 8, TRUE);
	}
	else
	{
		m_wndGrid.HideCols(1, 4, TRUE);
		m_wndGrid.HideCols(5, 6, FALSE);
		m_wndGrid.HideCols(7, 8, !(m_bCmpTapUseMulti && m_pSectData->SectBefore.bMultipleElastic));
	}
}

void CCMStiffDlg::SetDialogSize()
{
	int nSectNum = GetSectCount();
	//if (nSectNum == 2)
	//{
		int nWidth = 0;
		int nHeight = 0;
		CRect rect, rect2;
		GetWindowRect(rect);
		m_wndGrid.GetWindowRect(rect2);
		for (int i = 0; i <= GetUnitCol(); i++)
		{
			nWidth += m_wndGrid.GetColWidth(i);
		}
		nWidth += globalUtils.ScaleByDPI(5);
		for (int i = 0; i <= m_wndGrid.GetRowCount(); i++)
		{
			if (!m_wndGrid.IsRowHidden(i)) nHeight += m_wndGrid.GetRowHeight(i);
		}
		nHeight += globalUtils.ScaleByDPI(5);
		
		int nMarginW = (rect.Width() - rect2.Width());
		int nMarginH = (rect.Height() - rect2.Height());
		rect.right = rect.left + nWidth + nMarginW;
		rect.bottom = rect.top + nHeight + nMarginH;
		MoveWindow(rect);

		rect2.right = rect2.left + nWidth;
		rect2.bottom = rect2.top + nHeight;
		ScreenToClient(rect2);
		m_wndGrid.MoveWindow(rect2);

		// button
		CRect rect3;
		int nButtonW, nButtonH;
		
		m_wndSaveBtn.GetWindowRect(rect3);
		nButtonW = rect3.Width();
		nButtonH = rect3.Height();
		rect3.right = rect2.right - nButtonW - globalUtils.ScaleByDPI(5);
		rect3.left  = rect3.right - nButtonW;
		rect3.top = rect2.bottom + globalUtils.ScaleByDPI(16);
		rect3.bottom = rect3.top + nButtonH;
		m_wndSaveBtn.MoveWindow(rect3); 

		m_wndCloseBtn.GetWindowRect(rect3);
		nButtonW = rect3.Width();
		nButtonH = rect3.Height();
		rect3.right = rect2.right;
		rect3.left = rect3.right - nButtonW;
		rect3.top = rect2.bottom + globalUtils.ScaleByDPI(16);
		rect3.bottom = rect3.top + nButtonH;
		m_wndCloseBtn.MoveWindow(rect3);
	//}
}

void CCMStiffDlg::SetGridReadOnly(ROWCOL nFromCol, ROWCOL nToCol, BOOL bReadOnly/*=TRUE*/)
{
	m_wndGrid.SetStyleRange(CGXRange().SetCols(nFromCol, nToCol), 
													CGXStyle().SetReadOnly(bReadOnly));
}

BEGIN_MESSAGE_MAP(CCMStiffDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMStiffDlg)
	ON_BN_CLICKED(IDC_CMD_SP_SD_BTN_OK, OnClose)
	ON_BN_CLICKED(IDC_CMD_SP_SD_BTN_SAVE, OnCmdSpSdBtnSave)
	ON_BN_CLICKED(IDC_CMD_USER_CHECK, OnCmdUserCheck)
	ON_BN_CLICKED(IDC_CMD_I_BTN, OnCmdIBtn)
	ON_BN_CLICKED(IDC_CMD_J_BTN, OnCmdJBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStiffDlg message handlers

void CCMStiffDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	CDialogMove::OnCancel();
}

BOOL CCMStiffDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	// TODO: Add extra initialization here
	ROWCOL nUnitCol = GetUnitCol();

	m_wndGrid.Initialize();
	I_GridColorMgr::ApplyColorStyle(&m_wndGrid);
	m_wndGrid.LockUpdate(TRUE); // 설정중 화면 update를 막는다.
	m_wndGrid.GetParam()->EnableUndo(FALSE);  // undo를 금지시킨다.
	if(m_pSectData->SectBefore.bConsiderWarpingEffect)
		m_wndGrid.SetRowCount(40);  // Row 카운터 37개
	else
		m_wndGrid.SetRowCount(24);  // Row 카운터 24개
	m_wndGrid.SetColCount(nUnitCol);   // Column 카운터 9개

	SetTitleInGrid(); // 타이틀 이름을 설정한다.
	SetUnitInGrid();  // 단위계 이름을 설정한다.

	// Grid의 style을 변경한다.
	m_wndGrid.ChangeStandardStyle(CGXStyle()
			.SetFont(CGXFont_GC().SetFaceName(_LS(IDS_CMD_FONT_Ms_Sans_Serif)).SetSize(9))
			.SetVerticalAlignment(DT_VCENTER)
			.SetAllowEnter(FALSE));

	// column 크기를 맞춘다.
	m_wndGrid.SetColWidth(0, 0, globalUtils.ScaleByDPI(65));
	m_wndGrid.SetColWidth(1, nUnitCol-1, globalUtils.ScaleByDPI(116));
	m_wndGrid.SetColWidth(nUnitCol, nUnitCol, globalUtils.ScaleByDPI(60));
	m_wndGrid.SetRowHeight(0, 0, globalUtils.ScaleByDPI(19));
	m_wndGrid.SetRowHeight(1, m_wndGrid.GetRowCount(), globalUtils.ScaleByDPI(16));

	m_wndGrid.ColHeaderStyle()
			.SetReadOnly(TRUE).SetEnabled(FALSE);
	m_wndGrid.RowHeaderStyle()
			.SetReadOnly(TRUE).SetEnabled(FALSE);
	m_wndGrid.SetStyleRange(CGXRange().SetCols(1, nUnitCol-1), CGXStyle()
		.SetControl(GX_IDS_CTRL_EDIT)    
		.SetHorizontalAlignment(DT_RIGHT)
		.SetValueType(GX_VT_NUMERIC)
		.SetFormat(GX_FMT_FLOAT)
		.SetPlaces(6)
		.SetEnabled(FALSE)
		);
	m_wndGrid.SetStyleRange(CGXRange().SetCols(nUnitCol), 
			CGXStyle().SetReadOnly(TRUE).SetEnabled(FALSE));

	// cell의 width와 height 크기 변경을 금지한다.
	m_wndGrid.GetParam()->EnableTrackRowHeight(FALSE);
	m_wndGrid.GetParam()->EnableTrackColWidth(FALSE);

	// column과 row의 위치 이동을 금지시킨다.
	m_wndGrid.GetParam()->EnableMoveCols(FALSE);
	m_wndGrid.GetParam()->EnableMoveRows(FALSE);

	// double click시 셀 편집이 되도록 한다.
	m_wndGrid.GetParam()->SetActivateCellFlags(GX_CAFOCUS_DBLCLICKONCELL);

	// 현재 셀을 설정한다.
	m_wndGrid.SetCurrentCell(1, 1);
	
	CDlgUtil::CtrlShowHide(this, m_aCtrlIJ, FALSE);
	// Tapered Composite 일때 I/J 선택버튼 보이고 숨기기 & 이동하기
	if (m_bCmpTap)
	{
		CRect rTarget, rSrc;
		GetDlgItem(IDC_CMD_I_BTN)->GetWindowRect(rSrc);
		GetDlgItem(IDC_CMD_SP_SD_GRID)->GetWindowRect(rTarget);
		int distX = rTarget.left - rSrc.left;
		CDlgUtil::CtrlMoveDistX(this, m_aCtrlIJ, distX+8);

		CDlgUtil::CtrlShowHide(this, m_aCtrlIJ, TRUE);
	}

	// Stiffness 값을 설정한다.
	if (!m_bCmpTap) SetStiffnessData();
	else            SetStiffnessDataCmpTap();

	// column 개수를 섹션 타입에 맞게 숨긴다
	if (!m_bCmpTap) SetColumnStyle();  
	else            SetColumnStyleCmpTap(0); // 0:I, 1:J

	m_wndGrid.HideRows(2, 3, !m_pSectData->SectBefore.bConsiderShearDeform);

	// Dialog의 size를 설정한다.
	SetDialogSize();  

	// input type for PSC
	m_wndUserCheck.ShowWindow(m_bSectPSC);
	m_wndSaveBtn.ShowWindow(m_bSectPSC);
	if (m_bSectPSC)
	{
		m_wndUserCheck.SetCheck(m_pSectData->SectBefore.bUserInputStiffness ? 1 : 0);
		ChangeInputType(m_pSectData->SectBefore.bUserInputStiffness);
	}
	//else ASSERT(!m_pSectData->SectBefore.bUserInputStiffness);

	m_wndGrid.LockUpdate(FALSE);
	m_wndGrid.Redraw();
	m_wndGrid.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMStiffDlg::OnCmdSpSdBtnSave() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_bSectPSC);

	int nCheck = m_wndUserCheck.GetCheck();
	if (nCheck == 1)
	{
		m_pSectData->SectBefore.bUserInputStiffness = TRUE;
		if (!GetStiffnessData()) return;
	}
	else 
		m_pSectData->SectBefore.bUserInputStiffness = FALSE;

	CDialogMove::OnOK();
}

void CCMStiffDlg::OnCmdUserCheck() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_bSectPSC);
	
	int nCheck = m_wndUserCheck.GetCheck();
	ChangeInputType(nCheck == 1);
}

void CCMStiffDlg::ChangeInputType(BOOL bUserInput)
{
	ASSERT(m_bSectPSC);

	ROWCOL nFromCol, nToCol;
	nFromCol = 1;
	nToCol = GetSectCount();
	
	BOOL bOldLock = m_wndGrid.LockUpdate(TRUE);
	if (bUserInput)
		m_wndGrid.SetStyleRange(CGXRange(1, nFromCol, 6, nToCol), CGXStyle()
			.SetEnabled(bUserInput)
			.SetInterior(COLORREF(RGB(192, 220, 192))));
	else
	{
		m_wndGrid.SetStyleRange(CGXRange(1, nFromCol, 6, nToCol), CGXStyle()
			.SetEnabled(bUserInput)
			.SetInterior(COLORREF(RGB(255, 255, 255))));
		for (int i = 0; i < GetSectCount(); i++)
			SetStiffnessDataSub(i+1, &m_paOrgStiff->GetAt(i));
	}

	m_wndGrid.LockUpdate(bOldLock);
	if (!bOldLock) m_wndGrid.Redraw();  
}

void CCMStiffDlg::OnCmdIBtn() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_bCmpTap);
	
	SetColumnStyleCmpTap(0);
}

void CCMStiffDlg::OnCmdJBtn() 
{
	// TODO: Add your control notification handler code here
	ASSERT(m_bCmpTap);
	
	SetColumnStyleCmpTap(1);
}



