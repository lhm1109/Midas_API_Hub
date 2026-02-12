// MassDmasDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "MassDmasDlg.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"

#include "..\wg_main\wg_mainRes2.h"

#define D_POINT_MASS        0
#define D_LINE_MASS         1
#define D_ARC_MASS          2
#define D_ARC_AREA_MASS     3
#define D_TRIANGULAR_MASS   4
#define D_RECTANGULAR_MASS  5

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMassDmasDlg dialog


CMassDmasDlg::CMassDmasDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CMassDmasDlg::IDD, pParent)
{
	m_pDoc = 0;
	m_nOldSel = 0;
	//{{AFX_DATA_INIT(CMassDmasDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_aPmCtrl.Add(IDC_TM_PM_MP_TITLE);
	m_aPmCtrl.Add(IDC_TM_PM_MP_EDIT);
	m_aPmCtrl.Add(IDC_TM_PM_MP_UNIT);
	m_aPmCtrl.Add(IDC_TM_PM_MA_TITLE);
	m_aPmCtrl.Add(IDC_TM_PM_MA_EDIT);
	m_aPmCtrl.Add(IDC_TM_PM_MA_UNIT);
	m_aPmCtrl.Add(IDC_TM_PM_XY_TITLE);
	m_aPmCtrl.Add(IDC_TM_PM_XY_EDIT);
	m_aPmCtrl.Add(IDC_TM_PM_XY_UNIT);

	m_aLmCtrl.Add(IDC_TM_LM_ML_TITLE);
	m_aLmCtrl.Add(IDC_TM_LM_ML_EDIT);
	m_aLmCtrl.Add(IDC_TM_LM_ML_UNIT);
	m_aLmCtrl.Add(IDC_TM_LM_D1_TITLE);
	m_aLmCtrl.Add(IDC_TM_LM_D1_EDIT);
	m_aLmCtrl.Add(IDC_TM_LM_D1_UNIT);
	m_aLmCtrl.Add(IDC_TM_LM_XY_TITLE);
	m_aLmCtrl.Add(IDC_TM_LM_XY_EDIT);
	m_aLmCtrl.Add(IDC_TM_LM_XY_UNIT);

	m_aAmCtrl.Add(IDC_TM_AM_ML_TITLE);
	m_aAmCtrl.Add(IDC_TM_AM_ML_EDIT);
	m_aAmCtrl.Add(IDC_TM_AM_ML_UNIT);
	m_aAmCtrl.Add(IDC_TM_AM_XY_TITLE);
	m_aAmCtrl.Add(IDC_TM_AM_XY_EDIT);
	m_aAmCtrl.Add(IDC_TM_AM_XY_UNIT);
	m_aAmCtrl.Add(IDC_TM_AM_D1_TITLE);
	m_aAmCtrl.Add(IDC_TM_AM_D1_EDIT);
	m_aAmCtrl.Add(IDC_TM_AM_D1_UNIT);
	m_aAmCtrl.Add(IDC_TM_AM_D2_TITLE);
	m_aAmCtrl.Add(IDC_TM_AM_D2_EDIT);
	m_aAmCtrl.Add(IDC_TM_AM_D2_UNIT);
	m_aAmCtrl.Add(IDC_TM_AM_D3_TITLE);
	m_aAmCtrl.Add(IDC_TM_AM_D3_EDIT);
	m_aAmCtrl.Add(IDC_TM_AM_D3_UNIT);

	m_aCamCtrl.Add(IDC_TM_CAM_MA_TITLE);
	m_aCamCtrl.Add(IDC_TM_CAM_MA_EDIT);
	m_aCamCtrl.Add(IDC_TM_CAM_MA_UNIT);
	m_aCamCtrl.Add(IDC_TM_CAM_XY_TITLE);
	m_aCamCtrl.Add(IDC_TM_CAM_XY_EDIT);
	m_aCamCtrl.Add(IDC_TM_CAM_XY_UNIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D1_TITLE);
	m_aCamCtrl.Add(IDC_TM_CAM_D1_EDIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D1_UNIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D2_TITLE);
	m_aCamCtrl.Add(IDC_TM_CAM_D2_EDIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D2_UNIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D3_TITLE);
	m_aCamCtrl.Add(IDC_TM_CAM_D3_EDIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D3_UNIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D4_TITLE);
	m_aCamCtrl.Add(IDC_TM_CAM_D4_EDIT);
	m_aCamCtrl.Add(IDC_TM_CAM_D4_UNIT);

	m_aTamCtrl.Add(IDC_TM_TAM_MA_TITLE);
	m_aTamCtrl.Add(IDC_TM_TAM_MA_EDIT);
	m_aTamCtrl.Add(IDC_TM_TAM_MA_UNIT);
	m_aTamCtrl.Add(IDC_TM_TAM_PT1_TITLE);
	m_aTamCtrl.Add(IDC_TM_TAM_PT1_EDIT);
	m_aTamCtrl.Add(IDC_TM_TAM_PT1_UNIT);
	m_aTamCtrl.Add(IDC_TM_TAM_PT2_TITLE);
	m_aTamCtrl.Add(IDC_TM_TAM_PT2_EDIT);
	m_aTamCtrl.Add(IDC_TM_TAM_PT2_UNIT);
	m_aTamCtrl.Add(IDC_TM_TAM_PT3_TITLE);
	m_aTamCtrl.Add(IDC_TM_TAM_PT3_EDIT);
	m_aTamCtrl.Add(IDC_TM_TAM_PT3_UNIT);

	m_aRamCtrl.Add(IDC_TM_RAM_MA_TITLE);
	m_aRamCtrl.Add(IDC_TM_RAM_MA_EDIT);
	m_aRamCtrl.Add(IDC_TM_RAM_MA_UNIT);
	m_aRamCtrl.Add(IDC_TM_RAM_XY_TITLE);
	m_aRamCtrl.Add(IDC_TM_RAM_XY_EDIT);
	m_aRamCtrl.Add(IDC_TM_RAM_XY_UNIT);
	m_aRamCtrl.Add(IDC_TM_RAM_D1_TITLE);
	m_aRamCtrl.Add(IDC_TM_RAM_D1_EDIT);
	m_aRamCtrl.Add(IDC_TM_RAM_D1_UNIT);
	m_aRamCtrl.Add(IDC_TM_RAM_D2_TITLE);
	m_aRamCtrl.Add(IDC_TM_RAM_D2_EDIT);
	m_aRamCtrl.Add(IDC_TM_RAM_D2_UNIT);
}

CMassDmasDlg::~CMassDmasDlg()
{
}

void CMassDmasDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMassDmasDlg)
	DDX_Control(pDX, IDC_TM_LIST, m_List);
	DDX_Control(pDX, IDC_TM_RAM_D2_UNIT, m_wndRamD2Unit);
	DDX_Control(pDX, IDC_TM_RAM_D2_EDIT, m_wndRamD2Edit);
	DDX_Control(pDX, IDC_TM_RAM_D1_UNIT, m_wndRamD1Unit);
	DDX_Control(pDX, IDC_TM_RAM_D1_EDIT, m_wndRamD1Edit);
	DDX_Control(pDX, IDC_TM_RAM_XY_UNIT, m_wndRamXyUnit);
	DDX_Control(pDX, IDC_TM_RAM_XY_EDIT, m_wndRamXyEdit);
	DDX_Control(pDX, IDC_TM_RAM_MA_UNIT, m_wndRamMaUnit);
	DDX_Control(pDX, IDC_TM_RAM_MA_EDIT, m_wndRamMaEdit);
	DDX_Control(pDX, IDC_TM_TAM_PT3_UNIT, m_wndTamPt3Unit);
	DDX_Control(pDX, IDC_TM_TAM_PT3_EDIT, m_wndTamPt3Edit);
	DDX_Control(pDX, IDC_TM_TAM_PT2_UNIT, m_wndTamPt2Unit);
	DDX_Control(pDX, IDC_TM_TAM_PT2_EDIT, m_wndTamPt2Edit);
	DDX_Control(pDX, IDC_TM_TAM_PT1_UNIT, m_wndTamPt1Unit);
	DDX_Control(pDX, IDC_TM_TAM_PT1_EDIT, m_wndTamPt1Edit);
	DDX_Control(pDX, IDC_TM_TAM_MA_UNIT, m_wndTamMaUnit);
	DDX_Control(pDX, IDC_TM_TAM_MA_EDIT, m_wndTamMaEdit);
	DDX_Control(pDX, IDC_TM_CAM_D4_UNIT, m_wndCamD4Unit);
	DDX_Control(pDX, IDC_TM_CAM_D4_EDIT, m_wndCamD4Edit);
	DDX_Control(pDX, IDC_TM_CAM_D3_UNIT, m_wndCamD3Unit);
	DDX_Control(pDX, IDC_TM_CAM_D3_EDIT, m_wndCamD3Edit);
	DDX_Control(pDX, IDC_TM_CAM_D2_UNIT, m_wndCamD2Unit);
	DDX_Control(pDX, IDC_TM_CAM_D2_EDIT, m_wndCamD2Edit);
	DDX_Control(pDX, IDC_TM_CAM_D1_UNIT, m_wndCamD1Unit);
	DDX_Control(pDX, IDC_TM_CAM_D1_EDIT, m_wndCamD1Edit);
	DDX_Control(pDX, IDC_TM_CAM_XY_UNIT, m_wndCamXyUnit);
	DDX_Control(pDX, IDC_TM_CAM_XY_EDIT, m_wndCamXyEdit);
	DDX_Control(pDX, IDC_TM_CAM_MA_UNIT, m_wndCamMaUnit);
	DDX_Control(pDX, IDC_TM_CAM_MA_EDIT, m_wndCamMaEdit);
	DDX_Control(pDX, IDC_TM_AM_D3_UNIT, m_wndAmD3Unit);
	DDX_Control(pDX, IDC_TM_AM_D3_EDIT, m_wndAmD3Edit);
	DDX_Control(pDX, IDC_TM_AM_D2_UNIT, m_wndAmD2Unit);
	DDX_Control(pDX, IDC_TM_AM_D2_EDIT, m_wndAmD2Edit);
	DDX_Control(pDX, IDC_TM_AM_D1_UNIT, m_wndAmD1Unit);
	DDX_Control(pDX, IDC_TM_AM_D1_EDIT, m_wndAmD1Edit);
	DDX_Control(pDX, IDC_TM_AM_XY_UNIT, m_wndAmXyUnit);
	DDX_Control(pDX, IDC_TM_AM_XY_EDIT, m_wndAmXyEdit);
	DDX_Control(pDX, IDC_TM_AM_ML_UNIT, m_wndAmMlUnit);
	DDX_Control(pDX, IDC_TM_AM_ML_EDIT, m_wndAmMlEdit);
	DDX_Control(pDX, IDC_TM_LM_XY_UNIT, m_wndLmXyUnit);
	DDX_Control(pDX, IDC_TM_LM_XY_EDIT, m_wndLmXyEdit);
	DDX_Control(pDX, IDC_TM_LM_D1_UNIT, m_wndLmD1Unit);
	DDX_Control(pDX, IDC_TM_LM_D1_EDIT, m_wndLmD1Edit);
	DDX_Control(pDX, IDC_TM_LM_ML_UNIT, m_wndLmMlUnit);
	DDX_Control(pDX, IDC_TM_LM_ML_EDIT, m_wndLmMlEdit);
	DDX_Control(pDX, IDC_TM_PM_XY_UNIT, m_wndPmXyUnit);
	DDX_Control(pDX, IDC_TM_PM_XY_EDIT, m_wndPmXyEdit);
	DDX_Control(pDX, IDC_TM_PM_MP_UNIT, m_wndPmMpUnit);
	DDX_Control(pDX, IDC_TM_PM_MP_EDIT, m_wndPmMpEdit);
	DDX_Control(pDX, IDC_TM_PM_MA_UNIT, m_wndPmMaUnit);
	DDX_Control(pDX, IDC_TM_PM_MA_EDIT, m_wndPmMaEdit);
	DDX_Control(pDX, IDC_TM_END_STOR, m_wndEndStor);
	DDX_Control(pDX, IDC_TM_START_STOR, m_wndStartStor);
	DDX_Control(pDX, IDC_TM_MASS_TYPE, m_wndMassType);
	DDX_Control(pDX, IDC_TM_PICTURE, m_wndPicture);
	//}}AFX_DATA_MAP
}

//--------------------------------------------------------------------------
// Implementations
void CMassDmasDlg::FillStoryCombo(MComboBox& wndCombo, int nStart)
{
	wndCombo.ResetContent();

	CArray<T_STOR_K, T_STOR_K> KeyList;

	// get all keys
	m_pDoc->m_pAttrCtrl->GetStorKeyList(KeyList);

	// get filtered keys
	if (KeyList.GetSize() > 0)
	{
		T_STOR_K StorK_start = KeyList[nStart];
		m_pDoc->m_pAttrCtrl->GetStorKeyListByConnect(KeyList, &StorK_start, NULL);
	}

	//int nCount = KeyList.GetSize();
	//for (int i = nStart; i < nCount; i++)
	T_STOR_D Stor;
	for (int i = 0; i < KeyList.GetSize(); i++)
	{
		m_pDoc->m_pAttrCtrl->GetStor(KeyList[i], Stor);
		int nIndex = wndCombo.AddString(Stor.StoryName);
		wndCombo.SetItemData(nIndex, KeyList[i]);
	}
}

void CMassDmasDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

void CMassDmasDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_DMAS_K Key;
	T_DMAS_D Data;

	BOOL bStorChanged = FALSE;
	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_STOR_ADD):
		case(UR_STOR_DEL):
		case(UR_STOR_MFD):
		case(UR_STOR_MFS): 
			bStorChanged = TRUE;
			break;
		case(UR_DMAS_ADD):
			{
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetDmas(nKey, Key, Data);
				InsertItem(Key, Data);
			}
			break;
		case(UR_DMAS_DEL):
			{
				// 삭제될 위치(nRow)를 찾아서 지운다.
				pViewBuff->GetDmas(nKey, Key, Data);
				DeleteItem(Key, Data);
			}
			break;
		default:
			break;
		}
	} // end of while

	if (bStorChanged)
	{
		// Combo의 내용을 변경한다.
		FillStoryCombo(m_wndStartStor, 0);
		FillStoryCombo(m_wndEndStor, 0);
		if (m_wndStartStor.GetCount() > 0)
		{
			m_wndStartStor.SetCurSel(0);
			m_wndEndStor.SetCurSel(0);
		}
		UpdateStorNameInList();
	}
}

void CMassDmasDlg::UpdateUnit()
{
	MakeItemEx();
}

void CMassDmasDlg::AlignControls()
{
	// TODO: Add your control notification handler code here
	CRect rRef;
	CRect rToMove;
	int nDistY;

	CArray<UINT, UINT> *aCtrlSet[] = { 
		&m_aLmCtrl, &m_aAmCtrl, &m_aCamCtrl, &m_aTamCtrl, &m_aRamCtrl,
	};

	GetDlgItem(m_aPmCtrl[0])->GetWindowRect(rRef);
	for (int i = 0; i < 5; i++)
	{
		GetDlgItem(aCtrlSet[i]->GetAt(0))->GetWindowRect(rToMove);
		nDistY = rRef.top - rToMove.top;
		CtrlMoveDistY(*aCtrlSet[i], nDistY);
	}

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDC_TM_LIST);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(10);
	CWnd* pParent = GetParent();
	pParent->ScreenToClient(r);
	MoveWindow(r);
}

void CMassDmasDlg::ChangePicture(int nMassType)
{
	// Bitmap을 Oneway, Twoway로 바꾼다.
	CString aSVGPath[] =
	{
		_T("SVG\\Illustration\\Dialog\\tm_dmas1.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_dmas2.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_dmas3.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_dmas4.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_dmas5.svg"),
		_T("SVG\\Illustration\\Dialog\\tm_dmas6.svg"),
	};

	m_wndPicture.SetImage(aSVGPath[nMassType]);
}

void CMassDmasDlg::ShowHideCtrl(int nMassType)
{
	CtrlShowHide(m_aPmCtrl, nMassType == D_POINT_MASS);
	CtrlShowHide(m_aLmCtrl, nMassType == D_LINE_MASS);
	CtrlShowHide(m_aAmCtrl, nMassType == D_ARC_MASS);
	CtrlShowHide(m_aCamCtrl, nMassType == D_ARC_AREA_MASS);
	CtrlShowHide(m_aTamCtrl, nMassType == D_TRIANGULAR_MASS);
	CtrlShowHide(m_aRamCtrl, nMassType == D_RECTANGULAR_MASS);
}

BOOL CMassDmasDlg::ItemToData(CArray<T_STOR_K, T_STOR_K> &aStorKey, T_DMAS_D &Data)
{
	aStorKey.RemoveAll();
	Data.Initialize();

	if (m_wndStartStor.GetCount() == 0) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Story_is_not_selected_));
		return FALSE;
	}

	T_STOR_K StorKey, EndStorKey;
	int nIndex;
	nIndex = m_wndEndStor.GetCurSel();
	EndStorKey = m_wndEndStor.GetItemData(nIndex);
	nIndex = m_wndStartStor.GetCurSel();
	do {
		StorKey = m_wndStartStor.GetItemData(nIndex);
		aStorKey.Add(StorKey);
		nIndex++;
	} while (StorKey != EndStorKey);

	CString csVal;
	int nCurSel = m_wndMassType.GetCurSel();
	Data.nMassType = m_wndMassType.GetItemData(nCurSel);
	switch(nCurSel)
	{
	case D_POINT_MASS:
		Data.Point.dPointMass = m_wndPmMpEdit.GetEditValue();
		Data.Point.dRotationMass = m_wndPmMaEdit.GetEditValue();
		m_wndPmXyEdit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.Point.dPointX, Data.Point.dPointY))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_center_coordinate_));
			return FALSE;
		}
		break;
	case D_LINE_MASS:
		Data.Line.dLineMass = m_wndLmMlEdit.GetEditValue();
		Data.Line.dLength = m_wndLmD1Edit.GetEditValue();
		m_wndLmXyEdit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.Line.dPointX, Data.Line.dPointY))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_center_coordinate_));
			return FALSE;
		}
		break;
	case D_ARC_MASS:
		Data.CirArc.dLineMass = m_wndAmMlEdit.GetEditValue();
		m_wndAmXyEdit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.CirArc.dPointX, Data.CirArc.dPointY))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_center_coordinate_));
			return FALSE;
		}
		Data.CirArc.dArcAngle = m_wndAmD1Edit.GetEditValue();
		Data.CirArc.dArcRadius = m_wndAmD2Edit.GetEditValue();
		Data.CirArc.dCenterAngle = m_wndAmD3Edit.GetEditValue();
		break;
	case D_ARC_AREA_MASS:
		Data.CirArea.dAreaMass = m_wndCamMaEdit.GetEditValue();
		m_wndCamXyEdit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.CirArea.dPointX, Data.CirArea.dPointY))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_center_coordinate_));
			return FALSE;
		}
		Data.CirArea.dArcAngle = m_wndCamD1Edit.GetEditValue();
		Data.CirArea.dArcRadius = m_wndCamD2Edit.GetEditValue();
		Data.CirArea.dArcWidth = m_wndCamD3Edit.GetEditValue();
		Data.CirArea.dCenterAngle = m_wndCamD4Edit.GetEditValue();
		break;
	case D_TRIANGULAR_MASS:
		Data.Triangle.dAreaMass = m_wndTamMaEdit.GetEditValue();
		m_wndTamPt1Edit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.Triangle.dP1X, Data.Triangle.dP1Y))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_1st_point_coordinat));
			return FALSE;
		}
		m_wndTamPt2Edit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.Triangle.dP2X, Data.Triangle.dP2Y))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_2nd_point_coordinat));
			return FALSE;
		}
		m_wndTamPt3Edit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.Triangle.dP3X, Data.Triangle.dP3Y))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_3rd_point_coordinat));
			return FALSE;
		}
		break;
	case D_RECTANGULAR_MASS:
		Data.Rect.dAreaMass = m_wndRamMaEdit.GetEditValue();
		m_wndRamXyEdit.GetWindowText(csVal);
		if (!Get2FloatNumber(csVal, Data.Rect.dPointX, Data.Rect.dPointY))
		{
			AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___Invalid_center_coordinate_));
			return FALSE;
		}
		Data.Rect.dLength1 = m_wndRamD1Edit.GetEditValue();
		Data.Rect.dLength2 = m_wndRamD2Edit.GetEditValue();
		break;
	}

	return TRUE;
}

void CMassDmasDlg::DataToItem(T_DMAS_K Key, T_DMAS_D &Data)
{
	int nCount = m_wndMassType.GetCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
		if (m_wndMassType.GetItemData(i) == Data.nMassType) break;

	CString csVal;
	int nCurSel = i;
	m_wndMassType.SetCurSel(nCurSel);
	ChangePicture(nCurSel);
	ShowHideCtrl(nCurSel);

	T_STOR_D stor;
	m_pDoc->m_pAttrCtrl->GetStor(Key.key.entity, stor);
	m_wndStartStor.SelectString(-1, stor.StoryName);
	int nStorSel = m_wndStartStor.GetCurSel();
	FillStoryCombo(m_wndEndStor, nStorSel);
	m_wndEndStor.SetCurSel(0);

	switch(nCurSel)
	{
	case D_POINT_MASS:
		m_wndPmMpEdit.SetEditUnit(Data.Point.dPointMass);
		m_wndPmMaEdit.SetEditUnit(Data.Point.dRotationMass);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.Point.dPointX, Data.Point.dPointY);
		m_wndPmXyEdit.SetWindowText(csVal);
		break;
	case D_LINE_MASS:
		m_wndLmMlEdit.SetEditUnit(Data.Line.dLineMass);
		m_wndLmD1Edit.SetEditUnit(Data.Line.dLength);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.Line.dPointX, Data.Line.dPointY);
		m_wndLmXyEdit.SetWindowText(csVal);
		break;
	case D_ARC_MASS:
		m_wndAmMlEdit.SetEditUnit(Data.CirArc.dLineMass);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.CirArc.dPointX, Data.CirArc.dPointY);
		m_wndAmXyEdit.SetWindowText(csVal);
		m_wndAmD1Edit.SetEditUnit(Data.CirArc.dArcAngle);
		m_wndAmD2Edit.SetEditUnit(Data.CirArc.dArcRadius);
		m_wndAmD3Edit.SetEditUnit(Data.CirArc.dCenterAngle);
		break;
	case D_ARC_AREA_MASS:
		m_wndCamMaEdit.SetEditUnit(Data.CirArea.dAreaMass);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.CirArea.dPointX, Data.CirArea.dPointY);
		m_wndCamXyEdit.SetWindowText(csVal);
		m_wndCamD1Edit.SetEditUnit(Data.CirArea.dArcAngle);
		m_wndCamD2Edit.SetEditUnit(Data.CirArea.dArcRadius);
		m_wndCamD3Edit.SetEditUnit(Data.CirArea.dArcWidth);
		m_wndCamD4Edit.SetEditUnit(Data.CirArea.dCenterAngle);
		break;
	case D_TRIANGULAR_MASS:
		m_wndTamMaEdit.SetEditUnit(Data.Triangle.dAreaMass);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.Triangle.dP1X, Data.Triangle.dP1Y);
		m_wndTamPt1Edit.SetWindowText(csVal);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.Triangle.dP2X, Data.Triangle.dP2Y);
		m_wndTamPt2Edit.SetWindowText(csVal);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.Triangle.dP3X, Data.Triangle.dP3Y);
		m_wndTamPt3Edit.SetWindowText(csVal);
		break;
	case D_RECTANGULAR_MASS:
		m_wndRamMaEdit.SetEditUnit(Data.Rect.dAreaMass);
		csVal.Format(_LS(IDS_WG_TREEMENU__g____g), Data.Rect.dPointX, Data.Rect.dPointY);
		m_wndRamXyEdit.SetWindowText(csVal);
		m_wndRamD1Edit.SetEditUnit(Data.Rect.dLength1);
		m_wndRamD2Edit.SetEditUnit(Data.Rect.dLength2);
		break;
	}
}

#define COLCOUNT 6
void CMassDmasDlg::SetHeaderTitle()
{
	CString aTitle[] = {_LS(IDS_WG_TREEMENU_Story), _LS(IDS_WG_TREEMENU_Type), _LS(IDS_WG_TREEMENU_Xc), _LS(IDS_WG_TREEMENU_Yc), _LS(IDS_WG_TREEMENU_Mp), _LS(IDS_WG_TREEMENU_Mr)};
	int nColWidth[COLCOUNT] = {45, 42, 45, 45, 45, 45 };
	int nAlign[] = {
		LVCFMT_LEFT, LVCFMT_CENTER, 
		LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT,
	};

	CString title;
	int i;
	LV_COLUMN lvcolumn;

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_List.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	ListView_SetExtendedListViewStyle(m_List.GetSafeHwnd(), dwStyle);

	// Set Title
	for(i = 0; i < COLCOUNT; i++)
	{
		title = aTitle[i];
		lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvcolumn.fmt = nAlign[i];

		lvcolumn.iSubItem = i;
		lvcolumn.cx = globalUtils.ScaleByDPI(nColWidth[i]);
		lvcolumn.pszText = title.GetBuffer(0);
		m_List.InsertColumn(i,&lvcolumn);
		title.ReleaseBuffer();
	}
}

void CMassDmasDlg::MakeItemEx()
{
	CWaitCursor Cursor;

	m_List.DeleteAllItems();

	int nItemCount = m_pDoc->m_pAttrCtrl->GetCountDmas();
	if(nItemCount == 0) return;

	POSITION pos;
	CString str;
	int nCount;

	if(nItemCount)
	{
		T_DMAS_K Key;
		T_DMAS_D Data;

	  int* KeyBuf=new int[nItemCount];
	  nCount = 0;
		pos = m_pDoc->m_pAttrCtrl->GetStartDmas();
		while(pos !=NULL)
		{
		  m_pDoc->m_pAttrCtrl->GetNextDmas(pos, Key, Data);
		  KeyBuf[nCount++] = Key.keymap;
		}

		for(nCount = 0 ; nCount < nItemCount ; nCount++)
	  {
		  Key.keymap=KeyBuf[nCount];
			m_pDoc->m_pAttrCtrl->GetDmas(Key,Data);
			InsertItem(Key, Data);
		}
		delete []KeyBuf;
	}
}

int CMassDmasDlg::FindInsertionPos(T_DMAS_K Key)
{
	T_STOR_D Stor;
	double dFindStorLevel;
	T_DMAS_K dmasKey;

	m_pDoc->m_pAttrCtrl->GetStor((T_STOR_K)Key.key.entity, Stor);
	dFindStorLevel = Stor.dStoryLevel;
	
	int nCount = m_List.GetItemCount();
	int i = 0;
	for (i = 0; i < nCount; i++)
	{
		dmasKey.keymap = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetStor((T_STOR_K)dmasKey.key.entity, Stor);
		if ( Stor.dStoryLevel > dFindStorLevel) break;
		if (Stor.dStoryLevel == dFindStorLevel && 
				dmasKey.key.serial > Key.key.serial) break;
	}
	return i;
}

BOOL CMassDmasDlg::InsertItem(T_DMAS_K Key, T_DMAS_D &Data)
{
	CString str;

	int nItem = FindInsertionPos(Key);
	m_List.InsertItem(nItem, _T(""));
	m_List.SetItemData(nItem, Key.keymap);

	CStringArray aStrVal;
	DataToStr(aStrVal, Key, Data);
	for(int i = 0; i < COLCOUNT; i++)
	{
		str = aStrVal[i];
		m_List.SetItemText(nItem, i, str);
	}

	return TRUE;
}

BOOL CMassDmasDlg::DeleteItem(T_DMAS_K Key, T_DMAS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key.keymap;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1) m_List.DeleteItem(nItem);
	return TRUE;
}

BOOL CMassDmasDlg::ModifyItem(T_DMAS_K KeyOld, T_DMAS_K Key, T_DMAS_D &Data)
{
	LVFINDINFO FindInfo;
	FindInfo.flags = LVFI_PARAM;
	FindInfo.lParam = (LPARAM)Key.keymap;
	int nItem = m_List.FindItem(&FindInfo);
	if (nItem != -1)
	{
		CString str;
		CStringArray aStrVal;
		DataToStr(aStrVal, Key, Data);
		for(int i = 0; i < COLCOUNT; i++)
		{
			str = aStrVal[i];
			m_List.SetItemText(nItem, i, str);
		}
		m_List.SetItemData(nItem, Key.keymap);
	}
	return TRUE;
}

void CMassDmasDlg::DataToStr(CStringArray &aStrVal, T_DMAS_K Key, T_DMAS_D &Data)
{
	CString str;
	CString csType[] = {_T("P"), _T("L"), _LS(IDS_WG_TREEMENU_TR), _T("R"), _LS(IDS_WG_TREEMENU_AR), _LS(IDS_WG_TREEMENU_CI)};
	ASSERT(Data.nMassType >= 1 && Data.nMassType <= 6);

	T_STOR_D stor;
	m_pDoc->m_pAttrCtrl->GetStor(Key.key.entity, stor);
																							 aStrVal.Add(stor.StoryName);
	str = csType[Data.nMassType-1];              aStrVal.Add(str);
	str.Format(_LS(IDS_WG_TREEMENU__g), Data.Result.dPointX);       aStrVal.Add(str);
	str.Format(_LS(IDS_WG_TREEMENU__g), Data.Result.dPointY);       aStrVal.Add(str);
	str.Format(_LS(IDS_WG_TREEMENU__g), Data.Result.dPointMass);    aStrVal.Add(str);
	str.Format(_LS(IDS_WG_TREEMENU__g), Data.Result.dRotationMass); aStrVal.Add(str);
}

void CMassDmasDlg::UpdateStorNameInList()
{
	int nCount = m_List.GetItemCount();
	T_DMAS_K key;
	T_STOR_D stor;
	for (int i = 0; i < nCount; i++)
	{
		key.keymap = m_List.GetItemData(i);
		m_pDoc->m_pAttrCtrl->GetStor(key.key.entity, stor);
		m_List.SetItemText(i, 0, stor.StoryName);
	}
}

BEGIN_MESSAGE_MAP(CMassDmasDlg, CMenuBarChildDlg)
	//{{AFX_MSG_MAP(CMassDmasDlg)
	ON_CBN_SELCHANGE(IDC_TM_MASS_TYPE, OnSelchangeTmMassType)
	ON_CBN_SELCHANGE(IDC_TM_START_STOR, OnSelchangeTmStartStor)
	ON_BN_CLICKED(IDC_TM_STOR_BTN, OnTmStorBtn)
	ON_BN_CLICKED(IDC_TM_ADD, OnTmAdd)
	ON_BN_CLICKED(IDC_TM_MODIFY, OnTmModify)
	ON_BN_CLICKED(IDC_TM_DELETE, OnTmDelete)
	ON_BN_CLICKED(IDC_TM_CLOSE, OnTmClose)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_TM_LIST, OnItemchangedTmList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMassDmasDlg message handlers

BOOL CMassDmasDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CMenuBarChildDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	AlignControls();

	int nIndex;
	int nMassType = D_POINT_MASS;
	nIndex = m_wndMassType.AddString(_LS(IDS_WG_TREEMENU_Point_Mass));
	m_wndMassType.SetItemData(nIndex, 1);
	nIndex = m_wndMassType.AddString(_LS(IDS_WG_TREEMENU_Line_Mass));
	m_wndMassType.SetItemData(nIndex, 2);
	nIndex = m_wndMassType.AddString(_LS(IDS_WG_TREEMENU_Circular_Arc_Mass));
	m_wndMassType.SetItemData(nIndex, 5);
	nIndex = m_wndMassType.AddString(_LS(IDS_WG_TREEMENU_Circular_Area_Mass));
	m_wndMassType.SetItemData(nIndex, 6);
	nIndex = m_wndMassType.AddString(_LS(IDS_WG_TREEMENU_Triangular_Area_Mass));
	m_wndMassType.SetItemData(nIndex, 3);
	nIndex = m_wndMassType.AddString(_LS(IDS_WG_TREEMENU_Rectangular_Area_Mass));
	m_wndMassType.SetItemData(nIndex, 4);
	m_wndMassType.SetCurSel(nMassType);

	m_wndPmMpEdit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.POINT_dPointMass);
	m_wndPmMpUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.POINT_dPointMass);
	m_wndPmMaEdit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.POINT_dRotationMass);
	m_wndPmMaUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.POINT_dRotationMass);
	m_wndPmXyUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.POINT_dPointX);
	m_wndPmXyEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndPmXyEdit.SetAttWcsPos();
	m_wndPmXyEdit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);

	m_wndLmMlEdit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.LINE_dLineMass);
	m_wndLmMlUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.LINE_dLineMass);
	m_wndLmD1Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.LINE_dLength);
	m_wndLmD1Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.LINE_dLength);
	m_wndLmXyUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.LINE_dPointX);
	m_wndLmXyEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndLmXyEdit.SetAttWcsPos();
	m_wndLmXyEdit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);

	m_wndAmMlEdit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dLineMass);
	m_wndAmMlUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dLineMass);
	m_wndAmXyUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dPointX);
	m_wndAmD1Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dArcAngle);
	m_wndAmD1Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dArcAngle);
	m_wndAmD2Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dArcRadius);
	m_wndAmD2Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dArcRadius);
	m_wndAmD3Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dCenterAngle);
	m_wndAmD3Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRARC_dCenterAngle);
	m_wndAmXyEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndAmXyEdit.SetAttWcsPos();
	m_wndAmXyEdit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);

	m_wndCamMaEdit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dAreaMass);
	m_wndCamMaUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dAreaMass);
	m_wndCamXyUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dPointX);
	m_wndCamD1Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dArcAngle);
	m_wndCamD1Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dArcAngle);
	m_wndCamD2Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dArcRadius);
	m_wndCamD2Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dArcRadius);
	m_wndCamD3Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dArcWidth);
	m_wndCamD3Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dArcWidth);
	m_wndCamD4Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dCenterAngle);
	m_wndCamD4Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.CIRAREA_dCenterAngle);
	m_wndCamXyEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndCamXyEdit.SetAttWcsPos();
	m_wndCamXyEdit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);

	m_wndTamMaEdit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.TRIANGLE_dAreaMass);
	m_wndTamMaUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.TRIANGLE_dAreaMass);
	m_wndTamPt1Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.TRIANGLE_dP1X);
	m_wndTamPt2Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.TRIANGLE_dP2X);
	m_wndTamPt3Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.TRIANGLE_dP3X);
	m_wndTamPt1Edit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndTamPt1Edit.SetAttWcsPos();
	m_wndTamPt1Edit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	m_wndTamPt2Edit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndTamPt2Edit.SetAttWcsPos();
	m_wndTamPt2Edit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);
	m_wndTamPt3Edit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndTamPt3Edit.SetAttWcsPos();
	m_wndTamPt3Edit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);

	m_wndTamPt1Edit.SetNextLink(&m_wndTamPt2Edit);
	m_wndTamPt2Edit.SetNextLink(&m_wndTamPt3Edit);
	m_wndTamPt3Edit.SetNextLink(NULL);

	m_wndRamMaEdit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.RECT_dAreaMass);
	m_wndRamMaUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.RECT_dAreaMass);
	m_wndRamXyUnit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.RECT_dPointX);
	m_wndRamD1Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.RECT_dLength1);
	m_wndRamD1Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.RECT_dLength1);
	m_wndRamD2Edit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.RECT_dLength2);
	m_wndRamD2Unit.SetUnitType(CUnitCtrl::m_DMAS_UNIT.RECT_dLength2);
	m_wndRamXyEdit.SetWindowText(_LS(IDS_WG_TREEMENU_0___0));
	m_wndRamXyEdit.SetAttWcsPos();
	m_wndRamXyEdit.SetModeToUse(MOUSEEDIT_USE_SET_XYPOS_ONLY);

	FillStoryCombo(m_wndStartStor, 0);
	FillStoryCombo(m_wndEndStor, 0);
	if (m_wndStartStor.GetCount() > 0) 
	{
		m_wndStartStor.SetCurSel(0);
		m_wndEndStor.SetCurSel(0);
	}

	ChangePicture(nMassType);

	ShowHideCtrl(nMassType);

	SetHeaderTitle();
	MakeItemEx();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMassDmasDlg::OnSelchangeTmMassType() 
{
	// TODO: Add your control notification handler code here
	int nMassType = m_wndMassType.GetCurSel();
	ShowHideCtrl(nMassType);
	ChangePicture(nMassType);
}

void CMassDmasDlg::OnSelchangeTmStartStor() 
{
	// TODO: Add your control notification handler code here
	CString strOldEnd;
	int nOldSel = m_wndEndStor.GetCurSel();
	m_wndEndStor.GetLBText(nOldSel, strOldEnd);
	int nCurSel = m_wndStartStor.GetCurSel();
	FillStoryCombo(m_wndEndStor, nCurSel);
	if(nOldSel+m_nOldSel > nCurSel) m_wndEndStor.SelectString(-1, strOldEnd);
	else                            m_wndEndStor.SetCurSel(0);

	m_nOldSel = nCurSel;
}

void CMassDmasDlg::OnTmStorBtn() 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_STORY,0));
}

void CMassDmasDlg::OnTmAdd() 
{
	// TODO: Add your control notification handler code here
	CArray<T_STOR_K, T_STOR_K> aStorKey;
	T_DMAS_D Data;
	if (!ItemToData(aStorKey, Data)) return;

	// Unselect current selection
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem != -1) 
		m_List.SetItemState(iItem, 0,  LVIS_SELECTED | LVIS_FOCUSED);	

	if (m_pDoc->m_pDataCtrl->AddDmas(aStorKey, Data, FALSE)) return;

	// if error, select previous selection
	if (iItem != -1)
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CMassDmasDlg::OnTmModify() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_diaphragm_mass_));
		return;
	}
	T_DMAS_K key;
	T_DMAS_D Data;
	CArray<T_STOR_K, T_STOR_K> aStorKey;

	key.keymap = m_List.GetItemData(iItem);
	if (!ItemToData(aStorKey, Data)) return;

	if (!m_pDoc->m_pDataCtrl->ModifyDmas(key, key, Data)) return;
	m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CMassDmasDlg::OnTmDelete() 
{
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) 
	{
		AfxMessageBox(_LS(IDS_WG_TREEMENU_Error___No_selected_diaphragm_mass_));
		return;
	}
	T_DMAS_K key;
	key.keymap = m_List.GetItemData(iItem);
	if (!m_pDoc->m_pDataCtrl->DelDmas(key)) return;

	int nCount = m_List.GetItemCount();
	if (nCount <= iItem) iItem -= 1;
	if (iItem >= 0) 
		m_List.SetItemState(iItem, LVIS_FOCUSED | LVIS_SELECTED,  LVIS_SELECTED | LVIS_FOCUSED);	
}

void CMassDmasDlg::OnTmClose() 
{
	// TODO: Add your control notification handler code here
	CloseDlg();
}

void CMassDmasDlg::OnItemchangedTmList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	int iItem = m_List.GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if (iItem == -1) return;

	T_DMAS_K Key;
	T_DMAS_D Data;
	Key.keymap = m_List.GetItemData(iItem);
	m_pDoc->m_pAttrCtrl->GetDmas(Key, Data);
	DataToItem(Key, Data);

	*pResult = 0;
}

BOOL CMassDmasDlg::ExternalInit(UINT key)
{
	if(key == 0) return FALSE;

	T_DMAS_K dmaskey;
	T_DMAS_D data;

	dmaskey.keymap = key;
	m_pDoc->m_pAttrCtrl->GetDmas(dmaskey, data);
	DataToItem(dmaskey, data);

	return TRUE;
}
