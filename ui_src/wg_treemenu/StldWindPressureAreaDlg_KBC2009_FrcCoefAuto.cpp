// CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "StldWindPressureAreaDlg_KBC2009_FrcCoefAuto.h"

#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_Qsort.h"
#include "..\wg_db\wg_db_WindLoadGeneratorCtrl.h"

#include "..\wg_base\wg_base_DlgUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

enum enmHorCrossSectType
{
	ENM_T_SQUARE_NORMAL = 0,
	ENM_T_SQUARE_DIA,
	ENM_T_HEX_OCTA,
	ENM_T_ROUND
};
enum enmInstallType
{
	ENM_T_AT_GR_LEVEL = 0,
	ENM_T_ABOVE_GR_LEVEL
};
enum enmSectMemType
{
	ENM_T_FLAT_SIDED_MEMS = 0,
	ENM_T_ROUNDED_MEMS
};
enum enmPlanType
{
	ENM_T_PLAN_SQUARE = 0,
	ENM_T_PLAN_TRIANGLE
};
enum enmMemType
{
	ENM_T_STEEL_RND_MEMS = 0,
	ENM_T_SQUARE_CRSSECT_MEMS,
	ENM_T_OTHER_TYPE
};

const int CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::COLCOUNT = 3;

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto dialog

CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto(CWnd* pParent)
	: CDialogMove(CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	T_NODE_K NodeK;
	T_NODE_D NodeD;
	POSITION posNode = m_pDoc->m_pAttrCtrl->GetStartNode();
	double dMaxZ = 0.0;
	double dMinZ = 0.0;
	BOOL bInit = TRUE;
	while(posNode)
	{
		m_pDoc->m_pAttrCtrl->GetNextNode(posNode, NodeK, NodeD);
		if(bInit) { dMaxZ = NodeD.z; dMinZ = NodeD.z; bInit = FALSE; }
		else
		{
			if(dMaxZ < NodeD.z) dMaxZ = NodeD.z;
			if(dMinZ > NodeD.z) dMinZ = NodeD.z;
		}
	}

	T_BLDC_D BldcD;
	if (m_pDoc->m_pAttrCtrl->GetBldc(BldcD))
	{
		if (BldcD.bUseBaseLevel) dMinZ = BldcD.dBaseLevel;
	}

	m_dTotalHeight = dMaxZ - dMinZ;

	m_Data.Initialize();
	m_DataAutoCalc.Initialize();
	m_strTargerLevel = _T("");
	m_rdoHorCrosSectType = 0;
	m_rdoInstallType = 0;
	m_rdoSectMemType = 0;
	m_rdoPlanType = 0;
	m_rdoMemType = 0;
	//
	m_edtChimneysTSSDiaHc.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysTSSDiaXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtChimneysTSSDepthXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolSignsHeightXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtSolSignsBreathXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);
	m_edtOpenSignsLFDiaXDir.SetUnitType(CUnitCtrl::m_WNAT_UNIT.dXDir);


	m_aHorCrsSectTypeShowHideCtrl.RemoveAll();
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_STATIC_DEPTHXY);
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR);
	m_aHorCrsSectTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR_UNIT);
	m_aHorCrsSectTypeShowHideCtrl.FreeExtra();

	m_aChimneysTSS.RemoveAll();
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSTSS);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSTSS_HCSTYPE);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_SQUARE_NORMAL);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_SQUARE_DIAGONAL);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_HEX_OCTA);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_RDO_ROUND);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSSTSS_DIAXY);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR_UNIT);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSSTSS_HC);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_CHIM_HC);
	m_aChimneysTSS.Add(IDC_WINDP_AUTOCALC_EDIT_CHIM_HC_UNIT);
	m_aChimneysTSS.Append(m_aHorCrsSectTypeShowHideCtrl);
	m_aChimneysTSS.FreeExtra();

	m_aSolSigns.RemoveAll();
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_STATIC_SOLSIGNS);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_STATIC_SOLID_SIGNS_INSTALL_TYPE);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_RDO_AT_GRAOUND_LEVEL);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_RDO_ABOVE_GROUND_LEVEL);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_STATIC_SOLSIGNS_HEIGHT);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_STATIC_SOLSIGNS_BREADTH);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_XDIR);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_XDIR_UNIT);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_BREADTH_XDIR);
	m_aSolSigns.Add(IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_BREADTH_XDIR_UNIT);
	m_aSolSigns.FreeExtra();

	m_aSectMemsTypeShowHideCtrl.RemoveAll();
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSIGNSLF_DIA);
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR);
	m_aSectMemsTypeShowHideCtrl.Add(IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR_UNIT);
	m_aSectMemsTypeShowHideCtrl.FreeExtra();

	m_aOpenSignsLF.RemoveAll();
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF_SECT_MEMTYPE);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_RDO_FLAT_SIDED_MEM);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_RDO_ROUNDED_MEMS);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_STATIC_OPENSIGNSLF_RATIO);
	m_aOpenSignsLF.Add(IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_XDIR);
	m_aOpenSignsLF.Append(m_aSectMemsTypeShowHideCtrl);
	m_aOpenSignsLF.FreeExtra();

	m_aTrusTowers.RemoveAll();
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_STATIC_TRUSOWERS);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_STATIC_TRUSTOWERS_PLANTYPE);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_STATIC_TRUSTOWERS_MEMTYPE);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_RDO_SQUARE_PLAN);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_RDO_TRIANGLE_PLAN);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_RDO_STEEL_ROUNDEDMEMS);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_RDO_SQUARE_CROSSECT_MEMS);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_RDO_OTHER_TYPE);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_STATIC_TRUSTOWERS_RATIO);
	m_aTrusTowers.Add(IDC_WINDP_AUTOCALC_EDIT_TRUSTOWERS_RATIO_XDIR);
	m_aTrusTowers.FreeExtra();
}


void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_WINDP_AUTOCALC_CMB_STRUCTURE_TYPE, m_cmbStructureType);
	DDX_Text(pDX,    IDC_WINDP_AUTOCALC_EDIT_TARGET_STORY, m_strTargerLevel);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_LIST_STORY, m_lstStory);
	DDX_Radio(pDX,   IDC_WINDP_AUTOCALC_RDO_SQUARE_NORMAL, m_rdoHorCrosSectType);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CHIM_HC, m_edtChimneysTSSDiaHc);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_CHIM_HC_UNIT, m_unitChimneysTSSDiaHc);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR, m_edtChimneysTSSDiaXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DIAXY_XDIR_UNIT, m_unitChimneysTSSDia);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR, m_edtChimneysTSSDepthXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_DEPTHXY_XDIR_UNIT, m_unitChimneysTSSDepth);
	DDX_Text(pDX,    IDC_WINDP_AUTOCALC_STATIC_SOLSIGNS_HEIGHT, m_txtSolSignsHeight);
	DDX_Text(pDX,    IDC_WINDP_AUTOCALC_STATIC_SOLSIGNS_BREADTH, m_txtSolSignsBreath);
	DDX_Radio(pDX,   IDC_WINDP_AUTOCALC_RDO_AT_GRAOUND_LEVEL, m_rdoInstallType);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_XDIR, m_edtSolSignsHeightXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_HEIGHT_XDIR_UNIT, m_unitSolSignsHeight);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_BREADTH_XDIR, m_edtSolSignsBreathXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_SOLSIGNS_BREADTH_XDIR_UNIT, m_unitSolSignsBreath);
	DDX_Radio(pDX,   IDC_WINDP_AUTOCALC_RDO_FLAT_SIDED_MEM, m_rdoSectMemType);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_RATIO_XDIR, m_edtOpenSignsLFRatioXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR, m_edtOpenSignsLFDiaXDir);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_OPENSIGNSLF_DIA_XDIR_UNIT, m_unitOpenSignsLFDia);
	DDX_Radio(pDX,   IDC_WINDP_AUTOCALC_RDO_SQUARE_PLAN, m_rdoPlanType);
	DDX_Radio(pDX,   IDC_WINDP_AUTOCALC_RDO_STEEL_ROUNDEDMEMS, m_rdoMemType);
	DDX_Control(pDX, IDC_WINDP_AUTOCALC_EDIT_TRUSTOWERS_RATIO_XDIR, m_edtTrusTowersRatioXDir);
}

BEGIN_MESSAGE_MAP(CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto, CDialogMove)
	ON_CBN_SELCHANGE(IDC_WINDP_AUTOCALC_CMB_STRUCTURE_TYPE,   OnSelchangeTypeCombo)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_WINDP_AUTOCALC_LIST_STORY, OnSelchangeStoryListCtrl)
	ON_NOTIFY(LVN_KEYDOWN, IDC_WINDP_AUTOCALC_LIST_STORY,     OnStoryListKeyDown)
	// button command
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_GENERATEALL,       OnCmdGenerate)  
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_BTN_APPLY_REPLACE, OnTgtStorAddReplace)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_BTN_SEL_STORY_DLG, OnSelectAll)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_BTN_DELETE,        OnSelStorDelete)
	// ChimneysTSS
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_SQUARE_NORMAL,   OnRdoClikedChimneysTSS)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_SQUARE_DIAGONAL, OnRdoClikedChimneysTSS)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_HEX_OCTA,        OnRdoClikedChimneysTSS)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_ROUND,           OnRdoClikedChimneysTSS)
	// SolSigns
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_AT_GRAOUND_LEVEL,   OnRdoClikedSolSigns)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_ABOVE_GROUND_LEVEL, OnRdoClikedSolSigns)
	// OpenSignsLF
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_FLAT_SIDED_MEM, OnRdoClikedOpenSignsLF)
	ON_BN_CLICKED(IDC_WINDP_AUTOCALC_RDO_ROUNDED_MEMS,   OnRdoClikedOpenSignsLF)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto message handlers
BOOL CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCtrls();  
	Data2Dlg(m_DataAutoCalc);

	SetDlgByCombo();

	SetLstHeaderTitle();
	MakeLstItem();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnOK() 
{
	// TODO: Add extra validation here
	CDialogMove::OnOK();
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnCancel() 
{
	// TODO: Add extra cleanup here
	CDialogMove::OnCancel();
}


void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnSelchangeTypeCombo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_DataAutoCalc.Initialize();
	m_DataAutoCalc.nStructType = m_cmbStructureType.GetCurSel();

	Data2Dlg(m_DataAutoCalc);

	SetDlgByCombo();

	UpdateData(FALSE);
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnSelchangeStoryListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nIndex = 0;
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// 	BOOL IsOriginSel = (pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED));
	// 	if (!(pNMLV->uChanged == LVIF_STATE && (IsOriginSel || (pNMLV->uNewState == LVIS_SELECTED)))) return; 

	int nSelectItem = pNMLV->iItem;
	if (nSelectItem == -1) return;
	nIndex = (int)pNMLV->lParam;

	Data2Dlg(m_Data.arKBC2009ForceCoef[nIndex]);
	SetDlgByCombo();

	m_strTargerLevel.Empty();
	CString strTemp;
	POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nSelItem = m_lstStory.GetNextSelectedItem(pos);
		LVITEM lvItem = {0};
		lvItem.iItem = nSelItem;
		lvItem.mask = (LVIF_TEXT | LVIF_PARAM);
		m_lstStory.GetItem(&lvItem);
		nIndex = (int)lvItem.lParam;
		if(m_strTargerLevel.IsEmpty())
			strTemp.Format(_T("%.3f"), m_Data.arKBC2009ForceCoef[nIndex].dLevel);
		else
			strTemp.Format(_T(", %.3f"), m_Data.arKBC2009ForceCoef[nIndex].dLevel);
		m_strTargerLevel += strTemp;
	}

	UpdateData(FALSE);
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnStoryListKeyDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	/* ESC가 눌러졌을때 처리 */
	if (pLKeyDown->wVKey == VK_ESCAPE)
	{
		int nCnt = m_lstStory.GetSelectedCount();
		if (nCnt > 0)
		{
			m_DataAutoCalc.Initialize();
			Data2Dlg(m_DataAutoCalc);
			SetDlgByCombo();

			POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
			while (pos != NULL)
			{
				int nSelItem = m_lstStory.GetNextSelectedItem(pos);
				m_lstStory.SetItemState(nSelItem, FALSE, LVIS_SELECTED | LVIS_FOCUSED);
			}
		}
		m_strTargerLevel.Empty();    
		UpdateData(FALSE);
	}
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnSelectAll()
{
	int i;
	int nCnt = m_lstStory.GetItemCount();
	for( i = 0 ; i < nCnt; ++i)
	{
		m_lstStory.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	}
	m_lstStory.SetFocus();
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnTgtStorAddReplace()
{
	if(!Dlg2Data(m_DataAutoCalc))
		return;

	UpdateData(TRUE);

	CArray<double, double> caDblLevel;
	TCHAR* pCharTgt = m_strTargerLevel.GetBuffer(m_strTargerLevel.GetLength());
	TCHAR* pCh;
	pCh = _tcstok(pCharTgt, _T(" ,"));
	while(pCh != NULL)
	{
		caDblLevel.Add(_tstof(pCh));
		pCh = _tcstok(NULL, _T(" ,"));
	}

	double dQz;
	double dCf;
	double dLevel;
	int i, j;
	int nSize = caDblLevel.GetSize();
	int nSizeData;
	int nIdxInsert;
	BOOL bReplace = FALSE;
	for(i = 0 ; i < nSize; i++)
	{
		dLevel = caDblLevel[i];
		if(!m_pDoc->m_pAttrCtrl2->GetWindQz(m_Data, dLevel, dQz))
			continue;    
		m_DataAutoCalc.dLevel = dLevel;
		m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalc(m_DataAutoCalc, m_dTotalHeight, dQz, dCf);
		m_DataAutoCalc.dCoefFactor = dCf;
		// 일단 추가될 데이터를 만든다.

		bReplace = FALSE;
		// Replace를 검사하고 같은 Level이 존재하면 Replace..
		nSizeData = m_Data.arKBC2009ForceCoef.GetSize();
		for(j = 0 ; j < nSizeData; j++)
		{
			if(fabs(dLevel - m_Data.arKBC2009ForceCoef[j].dLevel) <= 1E-5)
			{
				bReplace = TRUE;
				m_Data.arKBC2009ForceCoef[j] = m_DataAutoCalc;
				break;
			}
		}
		if(bReplace) continue;

		//같은 값이 없을때는 추가한다. 추가하는 위치를 찾고 그위치에 insert.
		if(nSizeData == 0) nIdxInsert = 0;
		else
		{
			if(dLevel < m_Data.arKBC2009ForceCoef[0].dLevel)                  nIdxInsert = 0;
			else if(dLevel > m_Data.arKBC2009ForceCoef[nSizeData - 1].dLevel) nIdxInsert = nSizeData;
			else
			{
				for(j = 1 ; j < nSizeData; j++)
				{
					if(dLevel <= m_Data.arKBC2009ForceCoef[j].dLevel)
					{
						nIdxInsert = j;
						break;
					}
				}
			}
		}

		m_Data.arKBC2009ForceCoef.InsertAt(nIdxInsert, m_DataAutoCalc);
	}

	MakeLstItem();
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnSelStorDelete()
{
	CArray<int, int> caIdx;
	POSITION pos = m_lstStory.GetFirstSelectedItemPosition();
	while (pos)
	{
		int nSelItem = m_lstStory.GetNextSelectedItem(pos);
		LVITEM lvItem = {0};
		lvItem.iItem = nSelItem;	
		lvItem.mask = (LVIF_TEXT | LVIF_PARAM);
		m_lstStory.GetItem(&lvItem);
		int nSelIdx = (int)lvItem.lParam;
		caIdx.Add(nSelIdx);
	}

	int nSize = caIdx.GetSize();
	int nIdxPre = -1;
	CQSort::QSortInt(caIdx.GetData(), nSize); //오름차순으로 정렬된다.
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		if(nIdxPre == caIdx[i])
			continue;

		if(caIdx[i] >= 0 && caIdx[i] < m_Data.arKBC2009ForceCoef.GetSize())
		{
			m_Data.arKBC2009ForceCoef.RemoveAt(caIdx[i]);
			nIdxPre = caIdx[i];
		}
	}

	MakeLstItem();

	m_strTargerLevel.Empty();    
	UpdateData(FALSE);
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnRdoClikedChimneysTSS() 
{
	UpdateData(TRUE);
	ShowCtrlByHorCrsSectType();
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnRdoClikedSolSigns()
{
	UpdateData(TRUE);
	ShowCtrlByInstallType();
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnRdoClikedOpenSignsLF() 
{
	UpdateData(TRUE);
	ShowCtrlBySectMemsType();
}

/////////////////////////////////////////////////////////////////////////////
// CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto member method
void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::InitCtrls()
{	
	AlignControl();	

	// Structure Combo
	m_cmbStructureType.ResetContent();
	int nIndex = 0;

	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_CHIMNEYS_TANKS_SIMILARSTRUCTURES)); m_cmbStructureType.SetItemData(nIndex, ENM_T_CHIMNEYSTSS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_SOLSIGNS);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_OPEN_SIGNS_LATTICE_FRAMEWORKS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_OPENSIGNSLF);
	nIndex = m_cmbStructureType.AddString(_LS(IDS_CMD_WIND_WNAT_TRUSSED_TOWERS)); m_cmbStructureType.SetItemData(nIndex, ENM_T_TRUSTOWERS);
	m_cmbStructureType.SetCurSel(0);
}
void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::Data2Dlg(const T_WDPR_KBC_AUTOCALC& WdprKBC2009)
{
	m_cmbStructureType.SetCurSel(WdprKBC2009.nStructType);

	int nSubType1 = WdprKBC2009.nSubType1;
	int nSubType2 = WdprKBC2009.nSubType2;
	double dDir[2] = {WdprKBC2009.dDirVal1, WdprKBC2009.dDirVal2};


	switch(WdprKBC2009.nStructType)
	{
	case ENM_T_CHIMNEYSTSS:	m_rdoHorCrosSectType = nSubType1; break;
	case ENM_T_SOLSIGNS:		m_rdoInstallType = nSubType1; break;
	case ENM_T_OPENSIGNSLF:	m_rdoSectMemType = nSubType1; break;
	case ENM_T_TRUSTOWERS:	m_rdoPlanType = nSubType1; m_rdoMemType  = nSubType2; break;
	}

	m_edtChimneysTSSDiaXDir.SetEditUnit(dDir[0]);
	m_edtChimneysTSSDiaHc.SetEditUnit(WdprKBC2009.dHc);
	//m_edtChimneysTSSDiaYDir.SetEditUnit(dYDir[0]);
	m_edtChimneysTSSDepthXDir.SetEditUnit(dDir[1]);
	//m_edtChimneysTSSDepthYDir.SetEditUnit(dYDir[1]);

	m_edtSolSignsHeightXDir.SetEditUnit(dDir[0]); 
	//m_edtSolSignsHeightYDir.SetEditUnit(dYDir[0]); 
	m_edtSolSignsBreathXDir.SetEditUnit(dDir[1]); 
	//m_edtSolSignsBreathYDir.SetEditUnit(dYDir[1]); 

	m_edtOpenSignsLFRatioXDir.SetEditUnit(dDir[0]);
	//m_edtOpenSignsLFRatioYDir.SetEditUnit(dYDir[0]);
	m_edtOpenSignsLFDiaXDir.SetEditUnit(dDir[1]);
	//m_edtOpenSignsLFDiaYDir.SetEditUnit(dYDir[1]);

	m_edtTrusTowersRatioXDir.SetEditUnit(dDir[0]); 
	//m_edtTrusTowersRatioYDir.SetEditUnit(dYDir[0]); 

	UpdateData(FALSE);
}

BOOL CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::Dlg2Data(T_WDPR_KBC_AUTOCALC& WdprKBC2009)
{	
	UpdateData(TRUE);

	WdprKBC2009.nStructType = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	int nSubType1 = 0;
	int nSubType2 = 0;
	double dHc = 0.0;
	double dDir[2] = {0., };


	switch(WdprKBC2009.nStructType)
	{
	case ENM_T_CHIMNEYSTSS:		
		{
			nSubType1 = m_rdoHorCrosSectType; 
			dDir[0]	= m_edtChimneysTSSDiaXDir.GetEditValue();
			dHc = m_edtChimneysTSSDiaHc.GetEditValue();
			//dYDir[0]	= m_edtChimneysTSSDiaYDir.GetEditValue();
			dDir[1]	= m_edtChimneysTSSDepthXDir.GetEditValue();
			//dYDir[1]	= m_edtChimneysTSSDepthYDir.GetEditValue();
			break;
		}
	case ENM_T_SOLSIGNS:			
		{
			nSubType1 = m_rdoInstallType; 
			dDir[0]	= m_edtSolSignsHeightXDir.GetEditValue();
			//dYDir[0]	= m_edtSolSignsHeightYDir.GetEditValue();
			dDir[1]	= m_edtSolSignsBreathXDir.GetEditValue();
			//dYDir[1]	= m_edtSolSignsBreathYDir.GetEditValue();
			break;
		}
	case ENM_T_OPENSIGNSLF:		
		{
			nSubType1 = m_rdoSectMemType; 
			dDir[0]	= m_edtOpenSignsLFRatioXDir.GetEditValue();
			//dYDir[0]	= m_edtOpenSignsLFRatioYDir.GetEditValue();
			dDir[1]	= m_edtOpenSignsLFDiaXDir.GetEditValue();
			//dYDir[1]	= m_edtOpenSignsLFDiaYDir.GetEditValue();
			break;
		}
	case ENM_T_TRUSTOWERS:		
		{
			nSubType1 = m_rdoPlanType; 
			nSubType2 = m_rdoMemType; 
			dDir[0]	= m_edtTrusTowersRatioXDir.GetEditValue();
			//dYDir[0]	= m_edtTrusTowersRatioYDir.GetEditValue();
			break;
		}
	}

	WdprKBC2009.nSubType1 = nSubType1;
	WdprKBC2009.nSubType2 = nSubType2;
	WdprKBC2009.dDirVal1 = dDir[0]; 
	WdprKBC2009.dDirVal2 = dDir[1];
	WdprKBC2009.dHc       = dHc;

	const double dTol = 1E-5;
	BOOL bWarning = TRUE;
	if(bWarning)
	{
		if(WdprKBC2009.nStructType == 0)
		{
			if(WdprKBC2009.dDirVal1 <= dTol || dHc <= dTol)
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Dim),MB_OK);
				return FALSE;
			}
		}
		else if (WdprKBC2009.nStructType == 1)
		{
			if(WdprKBC2009.dDirVal2 <= dTol) 
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Signs),MB_OK);
				return FALSE;
			}
		}
		else if(WdprKBC2009.nStructType == 2)
		{
			if(WdprKBC2009.dDirVal1 > 0.7) 
			{
				AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir_Ratio_Solid),MB_OK);
				return FALSE;
			}
		}

		if(WdprKBC2009.dDirVal1 <= -dTol || WdprKBC2009.dDirVal2 <= -dTol)
		{
			AfxMessageBox(_LS(IDS_CMD_WIND_WNAT_Error_XYDir),MB_OK);
			return FALSE;
		}
	}

	return TRUE;
}
void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::AlignControl()
{
	CRect rRef;
	CRect rToMove;
	int nDistY;

	//기본이 되는 프레임 위치.
	GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_CHIMNEYSTSS)->GetWindowRect(rRef);

	//아래의 프레임은 기본이 되는 프레임 위치로 옮긴다.
	GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_SOLSIGNS)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aSolSigns, nDistY);

	GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_OPENSINGSLF)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aOpenSignsLF, nDistY);

	GetDlgItem(IDC_WINDP_AUTOCALC_STATIC_TRUSOWERS)->GetWindowRect(rToMove);
	nDistY = rRef.top - rToMove.top;
	CDlgUtil::CtrlMoveDistY(this, m_aTrusTowers, nDistY);

	// resize self size
	CWnd* pWndLast = GetDlgItem(IDCANCEL);
	CRect rectLast;
	pWndLast->GetWindowRect(rectLast);
	CRect r;
	GetWindowRect(r);
	r.bottom = rectLast.bottom+globalUtils.ScaleByDPI(8);

	SetWindowPos(NULL, r.left, r.top, r.Width(), r.Height(), SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::SetDlgByCombo()
{
	int nSel = m_cmbStructureType.GetItemData(m_cmbStructureType.GetCurSel());

	switch(nSel)
	{
	case ENM_T_CHIMNEYSTSS:
		{
			CDlgUtil::CtrlShowHide(this, m_aChimneysTSS, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aSolSigns, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aOpenSignsLF, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aTrusTowers,  FALSE);
			ShowCtrlByHorCrsSectType();
			break;
		}
	case ENM_T_SOLSIGNS:
		{
			CDlgUtil::CtrlShowHide(this, m_aChimneysTSS, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aSolSigns, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aOpenSignsLF, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aTrusTowers,  FALSE);
			ShowCtrlByInstallType();
			break;
		}
	case ENM_T_OPENSIGNSLF:
		{
			CDlgUtil::CtrlShowHide(this, m_aChimneysTSS, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aSolSigns, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aOpenSignsLF, TRUE);
			CDlgUtil::CtrlShowHide(this, m_aTrusTowers,  FALSE);
			ShowCtrlBySectMemsType();
			break;
		}
	case ENM_T_TRUSTOWERS:
		{
			CDlgUtil::CtrlShowHide(this, m_aChimneysTSS, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aSolSigns, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aOpenSignsLF, FALSE);
			CDlgUtil::CtrlShowHide(this, m_aTrusTowers,  TRUE);
			break;
		}
	}
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::SetLstHeaderTitle()
{
	CStringArray strAry;
	strAry.Add(_LS(IDS_TM_WINDP_KBC2009_LIST_INDEX));
	{
		CString strTemp;
		CString strUnit;
		m_pDoc->m_pUnitCtrl->GetUnitSystemLength(strUnit);
		strTemp.Format(_T("%s(%s)"), _LS(IDS_TM_WINDP_KBC2009_LIST_LEVEL), strUnit);
		strAry.Add(strTemp);
	}
	strAry.Add(_LS(IDS_TM_WINDP_KBC2009_LIST_COEFF));

	CRect listRect;
	m_lstStory.GetWindowRect(listRect);
	int nWidth = listRect.Width();
	int nColWidth[COLCOUNT];
	nColWidth[0] = nWidth * 0.18;
	nColWidth[1] = nWidth * 0.32;
	nColWidth[2] = nWidth - GetSystemMetrics(SM_CYVSCROLL) - nColWidth[0] - nColWidth[1] - 5;

	m_lstStory.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	for(int i = 0; i < COLCOUNT; ++i)
	{
		LV_COLUMN lvCol;
		lvCol.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
		lvCol.fmt = i == 0 ? LVCFMT_LEFT : LVCFMT_RIGHT;
		lvCol.cx = nColWidth[i];
		lvCol.pszText = strAry[i].GetBuffer(strAry[i].GetLength());
		m_lstStory.InsertColumn(i, &lvCol);
	}
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::MakeLstItem()
{
	m_lstStory.DeleteAllItems();

	CArray<T_STOR_K, T_STOR_K> aStorK;
	CArray<double, double> caLevel;

	int i, nSize;
	nSize = m_Data.arKBC2009ForceCoef.GetSize();

	CString strTemp;
	CStringArray strAry;
	int nIdx = 1;
	for(i = nSize - 1 ; i >= 0; i--)
	{
		InsertItem(m_Data.arKBC2009ForceCoef[i], i);
	}
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::InsertItem(const T_WDPR_KBC_AUTOCALC& WdprKBC2009, const int& nIndex)
{
	int nItemCnt = m_lstStory.GetItemCount();

	CStringArray strAry;
	CString strTemp;
	strTemp.Format(_T("%d"), nItemCnt + 1);
	strAry.Add(strTemp);
	strTemp.Format(_T("%.2f"), WdprKBC2009.dLevel);
	strAry.Add(strTemp);
	strTemp.Format(_T("%.4f"), WdprKBC2009.dCoefFactor);
	strAry.Add(strTemp);

	if(strAry.GetSize() != COLCOUNT)
	{
		ASSERT(0);
		return;
	}

	LVITEM lvitem = {0};
	lvitem.iItem = nItemCnt;
	for(int i = 0; i < COLCOUNT; ++i)
	{
		lvitem.iSubItem = i;
		lvitem.pszText = strAry[i].GetBuffer(strAry[i].GetLength());
		lvitem.mask = LVIF_TEXT;

		if(i > 0) 
			m_lstStory.SetItem(&lvitem);
		else
		{
			lvitem.mask |= LVIF_PARAM;
			lvitem.lParam = (LPARAM)nIndex;
			m_lstStory.InsertItem(&lvitem);		
		}
	}
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::ShowCtrlByHorCrsSectType()
{
	BOOL IsShowCtrl = (m_rdoHorCrosSectType == ENM_T_ROUND);
	CDlgUtil::CtrlShowHide(this, m_aHorCrsSectTypeShowHideCtrl, IsShowCtrl);
}
void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::ShowCtrlByInstallType()
{
	if(m_rdoInstallType == ENM_T_AT_GR_LEVEL) 
	{
		m_txtSolSignsHeight = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_HEIGHT);
		m_txtSolSignsBreath = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_BREADTH);
		UpdateData(FALSE);
	}
	else if (m_rdoInstallType == ENM_T_ABOVE_GR_LEVEL)
	{
		m_txtSolSignsHeight = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_LARGER_DIM);
		m_txtSolSignsBreath = _LS(IDS_CMD_WIND_WNAT_SOLID_SIGNS_SMALLER_DIM);
		UpdateData(FALSE);
	}
}
void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::ShowCtrlBySectMemsType()
{
	BOOL IsShowCtrl = (m_rdoSectMemType == ENM_T_ROUNDED_MEMS);
	CDlgUtil::CtrlShowHide(this, m_aSectMemsTypeShowHideCtrl, IsShowCtrl);
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::MakeTgtEditStory()
{
	// 	m_strTargerStory.Empty();
	// 	int nCnt = m_aTgtStorK.GetSize();
	// 	for (int i = 0; i < nCnt; ++i)
	// 	{
	// 		T_STOR_D StorD;
	// 		if(m_pDoc->m_pAttrCtrl->GetStor(m_aTgtStorK[i], StorD))
	// 		{
	// 			m_strTargerStory += "<" + StorD.StoryName + ">";
	// 		}
	// 	}
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::SetData( T_WDPR_D* pData )
{
	m_Data = *pData;
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::GetData( T_WDPR_D* pData )
{
	pData->arKBC2009ForceCoef.Copy(m_Data.arKBC2009ForceCoef);
}

void CStldWindPressureAreaDlg_KBC2009_FrcCoefAuto::OnCmdGenerate()
{
	CArray<double, double> caHeight;
	CArray<T_NODE_D, T_NODE_D&> aNode;

	if(!Dlg2Data(m_DataAutoCalc))
		return;

	m_lstStory.DeleteAllItems();
	if(CWindLoadGeneratorCtrl::GetModelHeightAry(caHeight, aNode))
	{
		double dQz;
		double dCf;
		int i;
		int nSize = caHeight.GetSize();
		m_Data.arKBC2009ForceCoef.RemoveAll();
		for(i = 0 ; i < nSize; i++)
		{
			if(!m_pDoc->m_pAttrCtrl2->GetWindQz(m_Data, caHeight[i], dQz))
			{
				AfxMessageBox(_LS(IDS_TM_WINDP_NODAL_INVALID_FAIL_AUTOCOEF));
				break;
			}

			m_DataAutoCalc.dLevel = caHeight[i];
			m_pDoc->m_pAttrCtrl2->WindCoeffAutoCalc(m_DataAutoCalc, m_DataAutoCalc.dHc, dQz, dCf);
			if(fabs(dCf) >= 1E-5)
			{
				m_DataAutoCalc.dCoefFactor = dCf;
				m_Data.arKBC2009ForceCoef.Add(m_DataAutoCalc);
			}
		}
		MakeLstItem();
	}
}