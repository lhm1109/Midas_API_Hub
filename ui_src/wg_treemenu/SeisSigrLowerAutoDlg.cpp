#include "stdafx.h"
#include "wg_treemenu.h"
#include "SeisSigrLowerAutoDlg.h"
#include "SeisCvlJpSetTargetLowerDlg.h"

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"
#include "..\wg_db\wg_db_InitCtrl.h"
#include "..\wg_db\wg_db_EditData.h"
#include "..\wg_db\wg_db_UndoCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\DB_QUERY_SEIS_CVL_JP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  Z_POSITION_MAX DBL_MAX
#define  Z_POSITION_MIN  -DBL_MAX

#define  DEFALUT_DGREE 45

#pragma region Dialog 
CSeisSigrLowerAutoDlg::CSeisSigrLowerAutoDlg(CSeisCvlJpSetTargetLowerDlg* pParent /*=NULL*/)
	: CChildDialog(CSeisSigrLowerAutoDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_pAutoFilter = new CAutoFilterMember();
	m_pParent = pParent;
}

CSeisSigrLowerAutoDlg::~CSeisSigrLowerAutoDlg()
{
	if (m_pAutoFilter)
	{
		delete m_pAutoFilter;
		m_pAutoFilter = NULL;
	}
}

void CSeisSigrLowerAutoDlg::DoDataExchange(CDataExchange* pDX)
{
	CChildDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeisCvlEvalGrupDlg)
	DDX_Control(pDX, IDC_SIGR_LOWER_AUTO_GROUP_CMB, m_Group);
	DDX_Control(pDX, IDC_SIGR_LOWER_AUTO_LOCAL_POSITION_CMB, m_shearAxisDir);
	DDX_Control(pDX, IDC_SIGR_LOWER_AUTO_CASE_CMB, m_bearingType);
	DDX_Control(pDX, IDC_SIGR_LOWER_BEAR_COUNT_CMB, m_bearingLine);
	DDX_Control(pDX, IDC_SIGR_LOWER_AUTO_DEGREE_EDT, m_Degree);
	DDX_Control(pDX, IDC_SIGR_LOWER_AUTO_DEGREE_UNIT, m_DegreeUnit);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSeisSigrLowerAutoDlg, CChildDialog)
	//{{AFX_MSG_MAP(CSeisSigrLowerAutoDlg)
	ON_CBN_SELCHANGE(IDC_SIGR_LOWER_AUTO_GROUP_CMB, OnChangeGroupCmb)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSeisSigrLowerAutoDlg::OnInitDialog()
{
	CChildDialog::OnInitDialog();

	InitUnit();
	InitEditBox();
	InitComboBox();
	OnChangeGroupCmb();

	return TRUE;// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSeisSigrLowerAutoDlg::InitUnit()
{
	m_DegreeUnit.SetUnitType(D_UNITSYS_BASE_DEGREE);
}

void CSeisSigrLowerAutoDlg::InitComboBox()
{
	m_Group.ResetContent();
	CArray<T_GRUP_K, T_GRUP_K>aKeyList;
	m_pDoc->m_pAttrCtrl->GetGrupKeyList(aKeyList);
	for (int i = 0; i < aKeyList.GetSize(); ++i)
	{
		T_GRUP_K key = aKeyList.GetAt(i);
		T_GRUP_D GrupD;
		if (!m_pDoc->m_pAttrCtrl->GetGrup4Stage(key, GrupD)) continue;;
		//if (GrupD.arKeyElem.GetSize() == 0 || GrupD.arKeyNode.GetSize() == 0) continue;

		int index = m_Group.AddString(GrupD.GroupName);
		m_Group.SetItemData(index, key);
	}
	m_Group.SetCurSel(0);

	int indexDir = 0;
	m_shearAxisDir.ResetContent();
	indexDir = m_shearAxisDir.AddString(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_SHEAR_AXIS_DIR_Y));
	m_shearAxisDir.SetItemData(indexDir, seis_jp::kY_Dir);
	indexDir = m_shearAxisDir.AddString(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_SHEAR_AXIS_DIR_Z));
	m_shearAxisDir.SetItemData(indexDir, seis_jp::kZ_Dir);
	m_shearAxisDir.SetCurSel(0);

	int indexBearType = 0;
	m_bearingType.ResetContent();
	indexBearType = m_bearingType.AddString(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_RIGID_LINK));
	m_bearingType.SetItemData(indexBearType, T_SIGR_LOWER_BEAR_D::kRigidLink);
	indexBearType = m_bearingType.AddString(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_FIXED));
	m_bearingType.SetItemData(indexBearType, T_SIGR_LOWER_BEAR_D::kFixed);
	indexBearType = m_bearingType.AddString(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_MOVING));
	m_bearingType.SetItemData(indexBearType, T_SIGR_LOWER_BEAR_D::kMovable);
	indexBearType = m_bearingType.AddString(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_ELASTIC));
	m_bearingType.SetItemData(indexBearType, T_SIGR_LOWER_BEAR_D::kElastic);
	indexBearType = m_bearingType.AddString(_LS(IDS_IDD_TM_SIGR_LOWER_IDC_COMBO_BEAR_TYPE_VIBRA_ISOLA));
	m_bearingType.SetItemData(indexBearType, T_SIGR_LOWER_BEAR_D::kSeismicIsolation);
	m_bearingType.SetCurSel(0);

	int indexBearLine = 0;
	m_bearingLine.ResetContent();
	indexBearLine = m_bearingLine.AddString(_T("1"));
	m_bearingLine.SetItemData(indexBearLine, T_SIGR_LOWER_BEAR_D::kLine1);
	indexBearLine = m_bearingLine.AddString(_T("2"));
	m_bearingLine.SetItemData(indexBearLine, T_SIGR_LOWER_BEAR_D::kLine2);
	m_bearingLine.SetCurSel(0);
}

void CSeisSigrLowerAutoDlg::InitEditBox()
{
	const double degree = 45;
	m_Degree.SetEditUnit(degree);
}


/////////////////////////////////////////////////////////////////////////////
// Message Handler

void CSeisSigrLowerAutoDlg::OnChangeGroupCmb()
{
	UpdateData(TRUE);

	ResetViewControls();
	T_GRUP_D GrupD;
	T_GRUP_K key = m_Group.GetItemData(m_Group.GetCurSel());
	if (!m_pDoc->m_pAttrCtrl->GetGrup(key, GrupD)) return;

	if (GrupD.arKeyElem.GetSize() > 0)
	{
		m_pDoc->m_pViewCtrl->SelectElem(NULL, GrupD.arKeyElem, FALSE);
	}
	if (GrupD.arKeyNode.GetSize() > 0)
	{
		m_pDoc->m_pViewCtrl->SelectNode(NULL, GrupD.arKeyNode, FALSE);
	}
}

void CSeisSigrLowerAutoDlg::ResetViewControls()
{
	m_pDoc->m_pViewCtrl->UnSelectAllNode(NULL);
	m_pDoc->m_pViewCtrl->UnSelectAllElem(NULL, FALSE);
}

void CSeisSigrLowerAutoDlg::OnOk()
{
	T_GRUP_K key = m_Group.GetItemData(m_Group.GetCurSel());
	const double dDgree = m_Degree.GetEditValue();
	const int nMatType = m_pParent->m_materialType.GetCurSel()+1;
	if (key == -1) return;
	
	m_pAutoFilter->SetExcute(key, dDgree, nMatType);

	AddData();
	CChildDialog::OnOK();
}

void CSeisSigrLowerAutoDlg::AddData()
{
	T_SIGR_LOWER_D SigrLowerD;
	ConvertDlg2Data(SigrLowerD);
	if (!CheckDlg2Data(SigrLowerD)) return;

	InsertListItem(SigrLowerD);
	// 확인용
	ViewInsertItem(SigrLowerD);
}

void CSeisSigrLowerAutoDlg::ConvertDlg2Data(T_SIGR_LOWER_D& SigrLowerD)
{
	const int nDir = m_shearAxisDir.GetItemData(m_shearAxisDir.GetCurSel());
	m_pParent->m_columnList.DeleteAllItems();
	m_pParent->m_beamList.DeleteAllItems();
	m_pParent->m_bearingList.DeleteAllItems();
	m_pParent->m_footingList.DeleteAllItems();

	// Excute Column
	if (m_pAutoFilter->m_mMembar[eMembar::kColoum].size() > 0)
	{
		SetColumn(nDir, SigrLowerD);
	}
	// Excute Beam
	if (m_pAutoFilter->m_mMembar[eMembar::kBeam].size() > 0)
	{
		SetBeam(nDir, SigrLowerD);
	}
	// Excute Bear
	if (m_pAutoFilter->m_mMembar[eMembar::kBearing].size() > 0)
	{
		SetBearing(nDir, SigrLowerD);
	}
	// Excute Foot
	if (m_pAutoFilter->m_mMembar[eMembar::kFoot].size() > 0)
	{
		SetFoot(nDir, SigrLowerD);
	}
}

void CSeisSigrLowerAutoDlg::SetColumn(const int& nDir, T_SIGR_LOWER_D& rData)
{
	std::map <CString, std::vector<T_ELEM_K>> mColumns;
	SetMapColBeamFoot(eMembar::kColoum, mColumns);

	for (auto& nameMap : mColumns)
	{
		vector<T_ELEM_K> aColumn = nameMap.second;
		if (aColumn.size() == 0) continue;
		T_SIGR_LOWER_COLM_D ColmD;
		ColmD.strName = nameMap.first;
		ColmD.nDirShear = nDir;
		ColmD.bRebarCut = FALSE;

		// sort
		std::sort(aColumn.begin(), aColumn.end(), [](const T_ELEM_K& a, const T_ELEM_K& b) {
			return a < b;
			});
		for (int j = 0; j < aColumn.size(); ++j)
		{
			T_ELEM_K ElemK = aColumn[j];
			ColmD.aInvestigatedElem.Add(ElemK);
		}
		rData.COLM_D.Add(ColmD);
	}
}

void CSeisSigrLowerAutoDlg::SetBeam(const int& nDir, T_SIGR_LOWER_D& rData)
{
	std::map <CString, std::vector<T_ELEM_K>> mBeams;
	SetMapColBeamFoot(eMembar::kBeam, mBeams);

	for (auto& nameMap : mBeams)
	{
		vector<T_ELEM_K> aBeam = nameMap.second;
		if (aBeam.size() == 0) continue;
		T_SIGR_LOWER_BEAM_D BeamD;
		BeamD.strName = nameMap.first;
		BeamD.nDirShear = nDir;

		// sort
		std::sort(aBeam.begin(), aBeam.end(), [](const T_ELEM_K& a, const T_ELEM_K& b) {
			return a < b;
			});
		for (int j = 0; j < aBeam.size(); ++j)
		{
			T_ELEM_K ElemK = aBeam[j];
			BeamD.aInvestigatedElem.Add(ElemK);
		}
		rData.BEAM_D.Add(BeamD);
	}
}

void CSeisSigrLowerAutoDlg::SetBearing(const int& nDir, T_SIGR_LOWER_D& rData)
{
	const int nBearType = m_bearingType.GetItemData(m_bearingType.GetCurSel());
	const int nBearLine = m_bearingLine.GetItemData(m_bearingLine.GetCurSel());

	CString strSubName = SetSubGroup(eMembar::kBearing);
	int nName = SetSubGroupNum(strSubName, eMembar::kBearing);;
	std::map <int, std::vector<T_NODE_K>> mBearing;
	SetMapBearing(nBearLine, mBearing);
	for (auto& lineMap : mBearing)
	{
		vector<T_NODE_K> aBear = lineMap.second;

		for (int i = 0; i < aBear.size(); i += 2)
		{
			CString strName;
			strName.Format(strSubName + _T("%d"), nName++);

			T_SIGR_LOWER_BEAR_D BearD;
			BearD.strName = strName;
			BearD.nDirShear = nDir;
			BearD.nType = nBearType;
			BearD.nLineNum = lineMap.first;

			BearD.aPosition.Add(aBear[i]);
			BearD.aPosition.Add(aBear[i + 1]);
			rData.BEAR_D.Add(BearD);
		}
	}
}

void CSeisSigrLowerAutoDlg::SetFoot(const int& nDir, T_SIGR_LOWER_D& rData)
{
	// Foot
	std::map <CString, std::vector<T_ELEM_K>> mFoots;
	SetMapColBeamFoot(eMembar::kFoot, mFoots);

	for (auto& nameMap : mFoots)
	{
		vector<T_ELEM_K> aFoot = nameMap.second;
		if (aFoot.size() == 0) continue;
		T_SIGR_LOWER_FOOT_D FootD;
		FootD.strName = nameMap.first;
		//기초는 교축방향 전단력 값을 필요로 하지 않는다.
		FootD.nDirShear = seis_jp::kDirShear_0;

		// sort
		std::sort(aFoot.begin(), aFoot.end(), [](const T_ELEM_K& a, const T_ELEM_K& b) {
			return a < b;
			});
		for (int j = 0; j < aFoot.size(); ++j)
		{
			T_ELEM_K ElemK = aFoot[j];
			FootD.aInvestigatedElem.Add(ElemK);
		}
		rData.FOOT_D.Add(FootD);
	}
}

BOOL CSeisSigrLowerAutoDlg::CheckDlg2Data(const T_SIGR_LOWER_D& SigrLowerD)
{
	// Column
	for (int i = 0; i < SigrLowerD.COLM_D.GetSize(); ++i)
	{
		const T_SIGR_LOWER_COLM_D& data = SigrLowerD.COLM_D.GetAt(i);
		if (!m_pParent->CheckData_AddCOLM(data)) return FALSE;
	}

	// Beam
	for (int i = 0; i < SigrLowerD.BEAM_D.GetSize(); ++i)
	{
		const T_SIGR_LOWER_BEAM_D& data = SigrLowerD.BEAM_D.GetAt(i);
		if (!m_pParent->CheckData_AddBEAM(data)) return FALSE;
	}

	// Bearing
	for (int i = 0; i < SigrLowerD.BEAR_D.GetSize(); ++i)
	{
		const T_SIGR_LOWER_BEAR_D& data = SigrLowerD.BEAR_D.GetAt(i);
		if (!m_pParent->CheckData_BEAR(data)) return FALSE;
	}

	// Foot
	for (int i = 0; i < SigrLowerD.FOOT_D.GetSize(); ++i)
	{
		const T_SIGR_LOWER_FOOT_D& data = SigrLowerD.FOOT_D.GetAt(i);
		if (!m_pParent->CheckData_AddFOOT(data)) return FALSE;
	}

	return TRUE;
}

void CSeisSigrLowerAutoDlg::InsertListItem(const T_SIGR_LOWER_D& SigrLowerD)
{
	BOOL bMember[4] = { FALSE, FALSE, FALSE, FALSE };
	// Column
	int nCount = m_pParent->m_columnList.GetItemCount();
	for (int i = 0; i < SigrLowerD.COLM_D.GetSize(); ++i)
	{
		bMember[1] = TRUE;
		const T_SIGR_LOWER_COLM_D& data = SigrLowerD.COLM_D.GetAt(i);
		m_pParent->InsertItem_COLM(nCount++, const_cast<T_SIGR_LOWER_COLM_D&>(data));
	}

	// Beam
	nCount = m_pParent->m_beamList.GetItemCount();
	for (int i = 0; i < SigrLowerD.BEAM_D.GetSize(); ++i)
	{
		bMember[2] = TRUE;
		const T_SIGR_LOWER_BEAM_D& data = SigrLowerD.BEAM_D.GetAt(i);
		m_pParent->InsertItem_BEAM(nCount++, const_cast<T_SIGR_LOWER_BEAM_D&>(data));
	}

	// Bearing
	nCount = m_pParent->m_bearingList.GetItemCount();
	for (int i = 0; i < SigrLowerD.BEAR_D.GetSize(); ++i)
	{
		bMember[0] = TRUE;
		const T_SIGR_LOWER_BEAR_D& data = SigrLowerD.BEAR_D.GetAt(i);
		m_pParent->InsertItem_BEAR(nCount++, const_cast<T_SIGR_LOWER_BEAR_D&>(data));
	}

	// Foot
	nCount = m_pParent->m_footingList.GetItemCount();
	for (int i = 0; i < SigrLowerD.FOOT_D.GetSize(); ++i)
	{
		bMember[3] = TRUE;
		const T_SIGR_LOWER_FOOT_D& data = SigrLowerD.FOOT_D.GetAt(i);
		m_pParent->InsertItem_FOOT(nCount++, const_cast<T_SIGR_LOWER_FOOT_D&>(data));
	}

	for (int i = 0; i < 4; ++i)
	{
		if (bMember[i])
		{
			m_pParent->m_memberType.SetCurSel(i);
			m_pParent->SendMessage(WM_COMMAND, MAKEWPARAM( IDC_TM_SIGR_LOWER_MEMB_TYPE_CMB, CBN_SELCHANGE), (LPARAM)m_pParent->m_memberType.GetSafeHwnd());
			break;
		}
	}
}

void CSeisSigrLowerAutoDlg::ViewInsertItem(const T_SIGR_LOWER_D& SigrLowerD)
{
	ResetViewControls();

	for (int i = 0; i < SigrLowerD.COLM_D.GetSize(); ++i)
	{
		T_SIGR_LOWER_COLM_D ColmD = SigrLowerD.COLM_D.GetAt(i);
		if(ColmD.aInvestigatedElem.GetSize() > 0)  
			m_pDoc->m_pViewCtrl->SelectElem(NULL, ColmD.aInvestigatedElem, FALSE);
	}
	for (int i = 0; i < SigrLowerD.BEAM_D.GetSize(); ++i)
	{
		T_SIGR_LOWER_BEAM_D BeamD = SigrLowerD.BEAM_D.GetAt(i);
		if (BeamD.aInvestigatedElem.GetSize() > 0)
			m_pDoc->m_pViewCtrl->SelectElem(NULL, BeamD.aInvestigatedElem, FALSE);
	}
	for (int i = 0; i < SigrLowerD.BEAR_D.GetSize(); ++i)
	{
		T_SIGR_LOWER_BEAR_D BearD = SigrLowerD.BEAR_D.GetAt(i);
		if (BearD.aPosition.GetSize() > 0)
			m_pDoc->m_pViewCtrl->SelectNode(NULL, BearD.aPosition, FALSE);
	}
	for (int i = 0; i < SigrLowerD.FOOT_D.GetSize(); ++i)
	{
		T_SIGR_LOWER_FOOT_D FootD = SigrLowerD.FOOT_D.GetAt(i);
		if (FootD.aInvestigatedElem.GetSize() > 0)
			m_pDoc->m_pViewCtrl->SelectElem(NULL, FootD.aInvestigatedElem, FALSE);
	}
}

CString CSeisSigrLowerAutoDlg::SetSubGroup(const eMembar& nMembar)
{
	CString strSubGroup;
	switch (nMembar)
	{
	case eMembar::kColoum:
		strSubGroup = _T("C");
	break;
	case eMembar::kBeam:
		strSubGroup = _T("B");
	break;
	case eMembar::kBearing:
		strSubGroup = _T("S");
	break;
	case eMembar::kFoot:
		strSubGroup = _T("F");
	break;
	default:
		break;
	}
	return strSubGroup;
}

void CSeisSigrLowerAutoDlg::SetMapColBeamFoot(const eMembar& nMember, std::map <CString, std::vector<T_ELEM_K>>& mArray)
{
	CString strSubName = SetSubGroup(nMember);
	int nName = SetSubGroupNum(strSubName, nMember);
	CString strName;
	strName.Format(strSubName + _T("%d"), nName);

	const double dDgreeDist = 1; // 1도 오차
	for (int i = 0; i < m_pAutoFilter->m_mMembar[nMember].size(); ++i)
	{
		T_ELEM_K ElemK = m_pAutoFilter->m_mMembar[nMember][i];
		T_ELEM_D ElemD;
		T_NODE_D NodeD1, NodeD2;
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD1);
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2);

		double v1[3] = { NodeD2.x - NodeD1.x, NodeD2.y - NodeD1.y, NodeD2.z - NodeD1.z };
		if (mArray.size() == 0) mArray[strName].push_back(ElemK);
		else
		{
			BOOL bExist = FALSE;
			for (auto& nameMap : mArray)
			{
				T_ELEM_D ElemD2;
				T_NODE_D preNode1, preNode2;
				m_pDoc->m_pAttrCtrl->GetElem(nameMap.second[0], ElemD2);
				m_pDoc->m_pAttrCtrl->GetNode(ElemD2.elnod[0], preNode1);
				m_pDoc->m_pAttrCtrl->GetNode(ElemD2.elnod[1], preNode2);
				double v2[3] = { preNode2.x - preNode1.x, preNode2.y - preNode1.y, preNode2.z - preNode1.z }; // 기준 벡터
				double v3[3] = { NodeD1.x - preNode1.x, NodeD1.y - preNode1.y, NodeD1.z - preNode1.z }; // 위치 확인용 벡터

				double dDgree1 = CMathFunc::mathCrossAngleNormalize(v1, v2); // 각도
				double dDgree2 = CMathFunc::mathCrossAngleNormalize(v1, v3); // 각도
				// 각도가 90도 이상일 때 보/기둥 판단이 안됨
				if (dDgree1 > 90) dDgree1 = 180 - dDgree1;
				if (dDgree2 > 90) dDgree2 = 180 - dDgree2;

				// 각도 차이가 1도 이내이고, 위치가 같은 방향에 있을 때 동일 부재로 판단
				if (dDgree1 < dDgreeDist && dDgree2 < dDgreeDist)
				{
					nameMap.second.push_back(ElemK);
					bExist = TRUE;
					break;
				}
			}
			if (!bExist)
			{
				strName.Format(strSubName +_T("%d"), ++nName);
				mArray[strName].push_back(ElemK);
			}
		}
	}
}

void CSeisSigrLowerAutoDlg::SetMapBearing(const int& nBearLine, std::map <int, std::vector<T_NODE_K>>& mBearing)
{
	// 각 노드를 x 축 기준으로 nBearLine 가 2일 때 2개의 선으로 나눔
	if (nBearLine == 1)
	{
		for (int i = 0; i < m_pAutoFilter->m_mMembar[eMembar::kBearing].size(); i += 2)
		{
			T_NODE_K NodeK1 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i];
			T_NODE_K NodeK2 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i + 1];
			T_NODE_D NodeD1, NodeD2;
			m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeD1);
			m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeD2);
			mBearing[nBearLine].push_back(NodeK1);
			mBearing[nBearLine].push_back(NodeK2);
		}
	}
	else
	{
		// x 기준
		// 중앙값
		double dMid = GetMidValue(0);
		for (int i = 0; i < m_pAutoFilter->m_mMembar[eMembar::kBearing].size(); i += 2)
		{
			T_NODE_K NodeK1 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i];
			T_NODE_K NodeK2 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i + 1];
			T_NODE_D NodeD1, NodeD2;
			m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeD1);
			m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeD2);
			
			if (NodeD1.x <= dMid || NodeD2.x <= dMid)
			{
				mBearing[1].push_back(NodeK1);
				mBearing[1].push_back(NodeK2);
			}
			else
			{
				mBearing[2].push_back(NodeK1);
				mBearing[2].push_back(NodeK2);
			}
		}

		if (mBearing[2].size() == 0)
		{
			mBearing.clear();
			// y 기준으로 한번 더
			// 중앙값
			double dMid = GetMidValue(1);
			for (int i = 0; i < m_pAutoFilter->m_mMembar[eMembar::kBearing].size(); i += 2)
			{
				T_NODE_K NodeK1 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i];
				T_NODE_K NodeK2 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i + 1];
				T_NODE_D NodeD1, NodeD2;
				m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeD1);
				m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeD2);

				if (NodeD1.y <= dMid || NodeD2.y <= dMid)
				{
					mBearing[1].push_back(NodeK1);
					mBearing[1].push_back(NodeK2);
				}
				else
				{
					mBearing[2].push_back(NodeK1);
					mBearing[2].push_back(NodeK2);
				}
			}
		}
	}
}

double CSeisSigrLowerAutoDlg::GetMidValue(const int& nPos)
{
	double nMax = Z_POSITION_MIN;
	double nMin = Z_POSITION_MAX;
	for (int i = 0; i < m_pAutoFilter->m_mMembar[eMembar::kBearing].size(); i += 2)
	{
		T_NODE_K NodeK1 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i];
		T_NODE_K NodeK2 = m_pAutoFilter->m_mMembar[eMembar::kBearing][i + 1];
		T_NODE_D NodeD1, NodeD2;
		m_pDoc->m_pAttrCtrl->GetNode(NodeK1, NodeD1);
		m_pDoc->m_pAttrCtrl->GetNode(NodeK2, NodeD2);
		if (nPos == 0)
		{
			nMax = max(nMax, max(NodeD1.x, NodeD2.x));
			nMin = min(nMin, min(NodeD1.x, NodeD2.x));
		}
		else
		{
			nMax = max(nMax, max(NodeD1.y, NodeD2.y));
			nMin = min(nMin, min(NodeD1.y, NodeD2.y));
		}
	}
	return (nMax + nMin) / 2;
}

int CSeisSigrLowerAutoDlg::SetSubGroupNum(const CString& strSubGroup, const eMembar& nMembar)
{
	CString strAllName;
	auto pList = &m_pParent->m_sigrLowerList;
	for (int nItem = 0; nItem < pList->GetItemCount(); ++nItem)
	{
		CString strListName;
		switch (nMembar)
		{
		case eMembar::kColoum:
		{
			strListName = pList->GetItemText(nItem, 2);//기둥 명칭
		}
		break;
		case eMembar::kBeam:
		{
			strListName = pList->GetItemText(nItem, 3);//보   명칭
		}
		break;
		case eMembar::kBearing:
		{
			strListName = pList->GetItemText(nItem, 1);//지승 명칭
		}
		break;
		case eMembar::kFoot:
		{
			strListName = pList->GetItemText(nItem, 4);//기초 명칭
		}
		break;
		default:
			break;
		}
		if (strListName == _T("")) continue;
		strAllName += strListName + _T(", ");
	}

	int nSubGroup = 1;
	if (strAllName == _T("")) return nSubGroup;
	while (true)
	{
		CString strNum = strSubGroup + std::to_wstring(nSubGroup).c_str();
		if (m_pParent->IsContainString(strNum, strAllName) == FALSE) break;
		nSubGroup++;
	}

	return nSubGroup;
}
#pragma endregion

#pragma region Filter Function
CAutoFilterMember::CAutoFilterMember()
{
	m_pDoc = CDBDoc::GetDocPoint();
	m_dDgree = DEFALUT_DGREE;
}

CAutoFilterMember::~CAutoFilterMember()
{

}

void CAutoFilterMember::SetExcute(const T_GRUP_K& key, const double& dDgree, const int& nMatType)
{
	m_mMembar.clear();
	m_mPreMembar.clear();
	m_dDgree = dDgree;
	SetExistMemberType(key);

	// 모든 부재 찾기
	FindAllMemberType();

	// 추가조건 - 탄성 부재 제외
	ExceptElasticElem(nMatType);
	// 추가조건 - 기둥의 하단 탄성 부재는 기초로 확정(라멘교가 아닐때)
	if(m_mMembar[eMembar::kFoot].size() == 0 )AddFootForElasticElem();

	SortElemForY();
}

void CAutoFilterMember::SetExistMemberType(const T_GRUP_K& key)
{
	// Member 타입이 있는 값 확정(col, beam)
	T_GRUP_D GrupD;
	if (!m_pDoc->m_pAttrCtrl->GetGrup4Stage(key, GrupD)) return;
	for (int i = 0; i < GrupD.arKeyElem.GetSize(); ++i)
	{
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = GrupD.arKeyElem.GetAt(i);
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;

		if (!IsLowerElem(ElemK)) continue;
		// 절점 Bearing 판단
		FindBearingforElem(ElemD);
		const int nMemType = GetExistMemberType(ElemK);
		// 요소 각도 1도 내외 -> 기둥/보 판단
		switch (nMemType)
		{
		case eMembar::kColoum:
			m_mMembar[eMembar::kColoum].push_back(ElemK);
			break;
		case eMembar::kBeam:
			m_mMembar[eMembar::kBeam].push_back(ElemK);
			break;
		default:
			m_mPreMembar[eMembar::kUnknown].push_back(ElemK);
			break;
		}
	}

	// 절점 Bearing 판단
	for(int i=0; i< GrupD.arKeyNode.GetSize(); ++i)
	{
		T_NODE_K NodeK = GrupD.arKeyNode.GetAt(i);
		FindBearingforNode(NodeK);
	}
}

void CAutoFilterMember::FindAllMemberType()
{
	// 보/기둥 구별 -> 각도 기준
	vector<T_ELEM_K> aUnknown = m_mPreMembar[eMembar::kUnknown];
	for (int i = 0; i < aUnknown.size(); ++i)
	{
		T_ELEM_K ElemK = aUnknown[i];
		if (!IsLowerElem(ElemK)) continue;
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);

		BOOL bChange = FALSE;
		eMembar nMembar = CustomCheckFilter(ElemD, m_dDgree);
		if (nMembar == eMembar::kColoum)
		{
			m_mPreMembar[eMembar::kColoum].push_back(ElemK);
			bChange = TRUE;
		}
		else if(nMembar == eMembar::kBeam)
		{
			m_mPreMembar[eMembar::kBeam].push_back(ElemK);
			bChange = TRUE;
		}

		if (bChange) 
		{
			auto iter = std::find(m_mPreMembar[eMembar::kUnknown].begin(), m_mPreMembar[eMembar::kUnknown].end(), ElemK);
			if (iter != m_mPreMembar[eMembar::kUnknown].end()) m_mPreMembar[eMembar::kUnknown].erase(iter);
		}
	}

	FindColumnBeam(eMembar::kColoum);
	FindColumnBeam(eMembar::kBeam);
	FindBearing();
	FindFoot();
}

eMembar CAutoFilterMember::CustomCheckFilter(const T_ELEM_D& ElemD, const double& dDgree)
{
	const double dSin = sin(CMathFunc::ToRadian(dDgree));
	T_NODE_D DataNodeI, DataNodeJ;
	VERIFY(m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], DataNodeI));
	VERIFY(m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], DataNodeJ));

	double dx = DataNodeJ.x - DataNodeI.x;
	double dy = DataNodeJ.y - DataNodeI.y;
	double dz = DataNodeJ.z - DataNodeI.z;
	double Length = CMathFunc::mathLength(dx, dy, dz);
	if (Length < m_pDoc->m_pInitCtrl->ZeroLength())return eMembar::kUnknown;
	double Distance = fabs(dz);
	//if(Distance/Length < m_pInitCtrl->VertTol())return TRUE;
	if (Distance / Length < dSin)return eMembar::kBeam;
	else return eMembar::kColoum;
}

BOOL CAutoFilterMember::IsExistMemberType(const eMembar& nMember, const T_KEY& rKey)
{
	auto iter = std::find(m_mMembar[nMember].begin(), m_mMembar[nMember].end(), rKey);

	if (iter == m_mMembar[nMember].end()) return FALSE;
	else return TRUE;
}

int CAutoFilterMember::GetExistMemberType(const T_ELEM_K& ElemK)
{
	T_MBTP_D DataMbtp;
	if (m_pDoc->m_pAttrCtrl->GetMbtp(ElemK, DataMbtp))
	{
		if (DataMbtp.nMbType == 1)
			return 1; // column
		else if (DataMbtp.nMbType == 2)
			return 2; // beam
		else 
			return 0;
	}
	return 0;
}

BOOL CAutoFilterMember::IsLowerElem(const T_ELEM_K& ElemK)
{
	CArray<T_SIGR_UPPER_K, T_SIGR_UPPER_K> aSigrUpperK;
	m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->GetKeyList(aSigrUpperK);
	for (int i = 0; i < aSigrUpperK.GetSize(); ++i)
	{
		T_SIGR_UPPER_K SigrUpperK = aSigrUpperK.GetAt(i);
		T_SIGR_UPPER_D SigrUpperD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrUpper()->Get(SigrUpperK, SigrUpperD)) continue;

		auto iter = std::find(SigrUpperD.aElements.GetData(), SigrUpperD.aElements.GetData() + SigrUpperD.aElements.GetSize(), ElemK);
		if (iter != SigrUpperD.aElements.GetData() + SigrUpperD.aElements.GetSize())
		{
			return FALSE;
		}
	}

	CArray<T_SIGR_USER_K, T_SIGR_USER_K> aSigrUserK;
	m_pDoc->m_pAttrCtrl2->GetQSigrUser()->GetKeyList(aSigrUserK);
	for (int i = 0; i < aSigrUserK.GetSize(); ++i)
	{
		T_SIGR_USER_K SigrUserK = aSigrUserK.GetAt(i);
		T_SIGR_USER_D SigrUserD;
		if (!m_pDoc->m_pAttrCtrl2->GetQSigrUser()->Get(SigrUserK, SigrUserD)) continue;

		auto iter = std::find(SigrUserD.aInvestigatedElem.GetData(), SigrUserD.aInvestigatedElem.GetData() + SigrUserD.aInvestigatedElem.GetSize(), ElemK);
		if (iter != SigrUserD.aInvestigatedElem.GetData() + SigrUserD.aInvestigatedElem.GetSize())
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CAutoFilterMember::ExceptElasticElem(const int& nMatType)
{
	double dElastic = 0.0;
	double dSectAs = 0.0;
	switch (nMatType)
	{
	case T_SIGR_LOWER_D::kRC:
		dElastic = 2.0e8; // kN/m2
		dSectAs = 300; // 단면적비
		break;
	case T_SIGR_LOWER_D::kSteel:
	case T_SIGR_LOWER_D::kSRC:
		dElastic = 2.0e9; // kN/m2
		dSectAs = 100; // 단면적비
		break;
	default:
		break;
	}

	CCurUnitSaver unitSaver(TRUE);
	T_UNIT_INDEX tmpIndex;
	tmpIndex.nBase_Force = D_UNITSYS_FORCE_INDEX_KN;
	tmpIndex.nBase_Length = D_UNITSYS_LENGTH_INDEX_M;
	m_pDoc->m_pUnitCtrl->SetUnitIndexCurrent(tmpIndex);
	// 제외
	ExceptElem(eMembar::kColoum, dElastic, dSectAs, nMatType);
	ExceptElem(eMembar::kBeam, dElastic, dSectAs, nMatType);

	unitSaver.Restore();
}

void CAutoFilterMember::ExceptElem(const eMembar& nMember, const double& dElastic, const double& dSectAs, const int& nMatType)
{
	std::vector<T_ELEM_K> aElemArr;
	std::copy(m_mMembar[nMember].begin(), m_mMembar[nMember].end(), std::back_inserter(aElemArr));

	for (int i = 0; i < aElemArr.size(); i++)
	{
		T_ELEM_K ElemK = aElemArr[i];
		T_ELEM_D ElemD;
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);
		T_MATL_D MatlD;
		if (!m_pDoc->m_pAttrCtrl->GetMatl(ElemD.elmat, MatlD)) continue;
		T_SECT_D SectD;
		if (!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) continue;

		const double dElemElastic = nMatType == T_SIGR_LOWER_D::kSRC ? MatlD.Data2.Analysis.Elast : MatlD.Data1.Analysis.Elast;
		const double dSectAreaI = SectD.SectBefore.SectI.Stiffness.Area;
		const double dSectAreaJ = SectD.SectBefore.SectJ.Stiffness.Area;

		if (dElemElastic >= dElastic || (dSectAreaI >= dSectAs || dSectAreaJ >= dSectAs))
		{
			auto iter = std::find(m_mMembar[nMember].begin(), m_mMembar[nMember].end(), ElemK);
			if (iter != m_mMembar[nMember].end())
			{
				m_mMembar[nMember].erase(iter);
				// 기둥의 하단 탄성 부재를 예비 기초로 지정
				if (nMember == eMembar::kColoum) m_mPreMembar[eMembar::kFoot].push_back(ElemK);
			}
		}
	}
}

void CAutoFilterMember::SortElemForY()
{
	// Node 의 y 좌표를 기준으로 정렬
	for (auto& pair : m_mMembar)
	{
		if (pair.first == eMembar::kBearing) continue;

		std::sort(pair.second.begin(), pair.second.end(), [this](const T_ELEM_K& a, const T_ELEM_K& b) {
			T_ELEM_D ElemD1, ElemD2;
			m_pDoc->m_pAttrCtrl->GetElem(a, ElemD1);
			m_pDoc->m_pAttrCtrl->GetElem(b, ElemD2);

			T_NODE_D NodeD1, NodeD2;
			m_pDoc->m_pAttrCtrl->GetNode(ElemD1.elnod[0], NodeD1);
			m_pDoc->m_pAttrCtrl->GetNode(ElemD2.elnod[0], NodeD2);

			if (NodeD1.y == NodeD2.y)
			{
				// 같을 경우 x 기준
				return NodeD1.x < NodeD2.x;
			}
			else return NodeD1.y > NodeD2.y;
			});
	}
}

#pragma region Column/Beam
void CAutoFilterMember::FindColumnBeam(const eMembar& nMember)
{
	if (m_mPreMembar[nMember].size() == 0) return;

	// m-pai 데이터 있으면 기둥/보 확정
	vector<T_ELEM_K> aColumn = m_mPreMembar[nMember];
	for (int i = 0; i < aColumn.size(); ++i)
	{
		T_ELEM_K ElemK = aColumn[i];
		if (CheckMpaiElem(ElemK))
		{
			if (IsExistMemberType(nMember, ElemK) == FALSE)
			{
				m_mMembar[nMember].push_back(ElemK);
			}
		}
	}

	// m-phi 요소 or 맴버 타입과 이어져 있으면 기둥/보 확정
	if (m_mMembar[nMember].size() > 0)
	{
		for (int i = 0; i < m_mMembar[nMember].size(); ++i)
		{
			T_ELEM_K DefinedElemK = m_mMembar[nMember][i];
			T_ELEM_D DefinedElemD;
			m_pDoc->m_pAttrCtrl->GetElem(DefinedElemK, DefinedElemD);
			vector<T_ELEM_K> aChecked;
			aChecked.push_back(DefinedElemK);
			SetConnectMphiElem(nMember, DefinedElemD, m_mPreMembar[nMember], aChecked);
		}

		// m-phi 데이터 제거
		for (int i = 0; i < m_mMembar[nMember].size(); ++i)
		{
			T_ELEM_K ElemK = m_mMembar[nMember][i];
			auto iter = std::find(m_mPreMembar[nMember].begin(), m_mPreMembar[nMember].end(), ElemK);
			if (iter != m_mPreMembar[nMember].end()) m_mPreMembar[nMember].erase(iter);
		}
	}
}

void CAutoFilterMember::SetConnectMphiElem(const eMembar& nMember, const T_ELEM_D& ElemD, vector<T_KEY>& aVecElemK, vector<T_ELEM_K>& aChecked)
{
	/*
	재귀 함수
	*/
	if (aVecElemK.size() == 0) return;

	// Node I
	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	T_NODE_K NodeI = ElemD.elnod[0];
	int nSize = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(NodeI, aElemK);
	if (nSize > 0)
	{
		for (int i = 0; i < aElemK.GetSize(); ++i)
		{
			T_ELEM_K ElemK = aElemK.GetAt(i);
			auto iter = std::find(aVecElemK.begin(), aVecElemK.end(), ElemK);
			if (iter == aVecElemK.end()) continue;

			iter = std::find(aChecked.begin(), aChecked.end(), ElemK);
			if (iter != aChecked.end()) continue;
			aChecked.push_back(ElemK);

			T_ELEM_D ElemD2;
			m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD2);
			if (CustomCheckFilter(ElemD2, m_dDgree) != nMember) continue; // 같은 요소인지 확인

			//if (CheckMpaiElem(ElemK) == TRUE) continue;  // m-pai 요소 만나면 종료
			SetConnectMphiElem(nMember, ElemD2, aVecElemK, aChecked);
			if (IsExistMemberType(nMember, ElemK) == FALSE) m_mMembar[nMember].push_back(ElemK);
		}
	}

	// Node J
	aElemK.RemoveAll();
	T_NODE_K NodeJ = ElemD.elnod[1];
	nSize = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(NodeJ, aElemK);
	if (nSize > 0)
	{
		for (int i = 0; i < aElemK.GetSize(); ++i)
		{
			T_ELEM_K ElemK = aElemK.GetAt(i);
			auto iter = std::find(aVecElemK.begin(), aVecElemK.end(), ElemK);
			if (iter == aVecElemK.end()) continue;

			iter = std::find(aChecked.begin(), aChecked.end(), ElemK);
			if (iter != aChecked.end()) continue;
			aChecked.push_back(ElemK);

			T_ELEM_D ElemD2;
			m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD2);
			if (CustomCheckFilter(ElemD2, m_dDgree) != nMember) continue; // 같은 요소인지 확인

			//if (CheckMpaiElem(ElemK) == TRUE) continue;  // m-pai 요소 만나면 종료
			SetConnectMphiElem(nMember, ElemD2, aVecElemK, aChecked);
			if (IsExistMemberType(nMember, ElemK) == FALSE) m_mMembar[nMember].push_back(ElemK);
		}
	}
}

BOOL CAutoFilterMember::CheckMpaiElem(const T_ELEM_K& ElemK)
{
	T_IEHG_D IehgD;
	T_IEHG_K IehgK = m_pDoc->m_pAttrCtrl->MakeElemIehgK(ElemK);
	if (!m_pDoc->m_pAttrCtrl->GetIehg(IehgK, IehgD)) { return FALSE; }

	T_IEHP_D IehpD;
	if (!m_pDoc->m_pAttrCtrl->GetIehp(IehgD.IehpKey, IehpD)) { ASSERT(0); return FALSE; }

	return TRUE;
}

#pragma endregion 

#pragma region Bearing
void CAutoFilterMember::FindBearingforNode(const T_NODE_K& NodeK)
{
	// 절점에서 찾기
	// Bearing..
	CArray<T_NLNK_K, T_NLNK_K> aNlnkKey;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(aNlnkKey);

	for (int k = 0; k < aNlnkKey.GetSize(); ++k)
	{
		T_NLNK_D NlnkD;
		if (!m_pDoc->m_pAttrCtrl->GetNlnk(aNlnkKey.GetAt(k), NlnkD)) continue;

		if (NlnkD.Node1 == NodeK || NlnkD.Node2 == NodeK)
		{
			auto iter = std::find(m_mPreMembar[eMembar::kBearing].begin(), m_mPreMembar[eMembar::kBearing].end(), NodeK);
			if (iter == m_mPreMembar[eMembar::kBearing].end()) m_mPreMembar[eMembar::kBearing].push_back(NodeK);
		}
	}
}

void CAutoFilterMember::FindBearingforElem(const T_ELEM_D& ElemD)
{
	CArray<T_NLNK_K, T_NLNK_K> aNlnkKey;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(aNlnkKey);

	// 이 요소의 양쪽 노드에 연결된 범용링크가 있으면 Bearing 확정
	for (int k = 0; k < aNlnkKey.GetSize(); ++k)
	{
		T_NLNK_D NlnkD;
		if (!m_pDoc->m_pAttrCtrl->GetNlnk(aNlnkKey.GetAt(k), NlnkD)) continue;

		if (ElemD.elnod[0] == NlnkD.Node1 || ElemD.elnod[0] == NlnkD.Node2)
		{
			auto iter = std::find(m_mPreMembar[eMembar::kBearing].begin(), m_mPreMembar[eMembar::kBearing].end(), ElemD.elnod[0]);
			if (iter == m_mPreMembar[eMembar::kBearing].end()) m_mPreMembar[eMembar::kBearing].push_back(ElemD.elnod[0]);
		}
		if (ElemD.elnod[1] == NlnkD.Node1 || ElemD.elnod[1] == NlnkD.Node2)
		{
			auto iter = std::find(m_mPreMembar[eMembar::kBearing].begin(), m_mPreMembar[eMembar::kBearing].end(), ElemD.elnod[1]);
			if (iter == m_mPreMembar[eMembar::kBearing].end()) m_mPreMembar[eMembar::kBearing].push_back(ElemD.elnod[1]);
		}
	}
}

void CAutoFilterMember::FindBearing()
{
	if (m_mPreMembar[eMembar::kBearing].size() == 0) return;

	CArray<T_NLNK_K, T_NLNK_K> aNlnkKey;
	m_pDoc->m_pAttrCtrl->GetNlnkKeyList(aNlnkKey);

	// Bearing 확정
	for (int k = 0; k < aNlnkKey.GetSize(); ++k)
	{
		T_NLNK_D NlnkD;
		if (!m_pDoc->m_pAttrCtrl->GetNlnk(aNlnkKey.GetAt(k), NlnkD)) continue;
		T_NODE_K NodeKey[2] = { 0,0 };

		for (int i = 0; i < m_mPreMembar[eMembar::kBearing].size(); ++i)
		{
			T_NODE_K preNodeK = m_mPreMembar[eMembar::kBearing][i];

			if (NlnkD.Node1 == preNodeK) NodeKey[0] = NlnkD.Node1;
			if (NlnkD.Node2 == preNodeK) NodeKey[1] = NlnkD.Node2;

			// 양쪽 노드가 범용링크에 연결되어 있으면 Bearing 확정
			if (NodeKey[0] != 0 && NodeKey[1] != 0)
			{
				for (int node = 0; node < 2; ++node)
				{
					if (IsExistMemberType(eMembar::kBearing, NodeKey[node]) == FALSE)
					{
						m_mMembar[eMembar::kBearing].push_back(NodeKey[node]);
						auto iter = std::find(m_mPreMembar[eMembar::kBearing].begin(), m_mPreMembar[eMembar::kBearing].end(), NodeKey[node]);
						if (iter != m_mPreMembar[eMembar::kBearing].end()) m_mPreMembar[eMembar::kBearing].erase(iter);
					}
				}
				break;
			}
		}
	}
}
#pragma endregion

#pragma region Foot
void CAutoFilterMember::FindFoot()
{
	// 라멘교 일 때 기초 찾기
	// 기둥을 통해 보와 연결된 기초(기둥)을 찾는다.
	FindColumn2Foot();
	// 예비 기초에서 최하단 절점과 최상단 절점을 통해 기초를 확정
	FindNodeZ2Foot();
}

void CAutoFilterMember::FindColumn2Foot()
{
	// 기초는 기둥에 포함되어 있음!
	vector<T_NODE_K> aLowerNode;
	vector<T_ELEM_K> aChecked;
	for (int i = 0; i < m_mMembar[eMembar::kColoum].size(); ++i)
	{
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = m_mMembar[eMembar::kColoum][i];
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;

		auto iter = std::find(aChecked.begin(), aChecked.end(), ElemK);
		if (iter != aChecked.end()) continue;

		// 한 기둥 모두 찾기
		vector<T_ELEM_K> aColumn;
		MakeOneColumn(ElemD, m_mMembar[eMembar::kColoum], aColumn);
		aChecked.insert(aChecked.end(), aColumn.begin(), aColumn.end());

		// 기둥의 가장 낮은 절점 찾기
		if (aColumn.size() > 0)
		{
			T_NODE_K NodeMin = FindLowerNode(aColumn);
			if (m_pDoc->m_pAttrCtrl2->IsVerElemConnectedNode(NodeMin, FALSE) == FALSE)
			{
				// 기둥의 가장 낮은 절점이 요소가 2개 이상일 때 기초와 연결된 보와 가깝다
				CArray<T_ELEM_K, T_ELEM_K> aTemp;
				m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(NodeMin, aTemp);
				if (aTemp.GetSize() > 1)
				{
					auto pLower = std::find(aLowerNode.begin(), aLowerNode.end(), NodeMin);
					if (pLower == aLowerNode.end()) aLowerNode.push_back(NodeMin);
				}
			}
		}
	}

	// 기초가 시작되는 절점 찾기
	aChecked.clear();
	for (int i = 0; i < aLowerNode.size(); ++i)
	{
		T_NODE_K NodeK = aLowerNode[i];
		aChecked.push_back(NodeK);
		FindFootForBeam(NodeK, aChecked);
	}
}

void CAutoFilterMember::MakeOneColumn(const T_ELEM_D& ElemD, const vector<T_ELEM_K>& allColumn, vector<T_ELEM_K>& aElemArr)
{
	/*
	재귀 함수
	*/
	// Node I
	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	T_NODE_K NodeI = ElemD.elnod[0];
	int nSize = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(NodeI, aElemK);
	if (nSize > 0)
	{
		for (int i = 0; i < aElemK.GetSize(); ++i)
		{
			T_ELEM_K ElemK = aElemK.GetAt(i);

			auto iter = std::find(allColumn.begin(), allColumn.end(), ElemK);
			if (iter == allColumn.end()) continue;

			iter = std::find(aElemArr.begin(), aElemArr.end(), ElemK);
			if (iter == aElemArr.end())
			{
				T_ELEM_D ElemD2;
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD2)) continue;
				if (CustomCheckFilter(ElemD2, m_dDgree) != eMembar::kColoum) continue; // 기둥요소인지 확인

				aElemArr.push_back(ElemK);
				MakeOneColumn(ElemD2, allColumn, aElemArr);
			}
		}
	}

	// Node J
	aElemK.RemoveAll();
	T_NODE_K NodeJ = ElemD.elnod[1];
	nSize = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(NodeJ, aElemK);
	if (nSize > 0)
	{
		for (int i = 0; i < aElemK.GetSize(); ++i)
		{
			T_ELEM_K ElemK = aElemK.GetAt(i);

			auto iter = std::find(allColumn.begin(), allColumn.end(), ElemK);
			if (iter == allColumn.end()) continue;

			iter = std::find(aElemArr.begin(), aElemArr.end(), ElemK);
			if (iter == aElemArr.end())
			{
				T_ELEM_D ElemD2;
				if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD2)) continue;
				if (CustomCheckFilter(ElemD2, m_dDgree) != eMembar::kColoum) continue; // 기둥요소인지 확인

				aElemArr.push_back(ElemK);
				MakeOneColumn(ElemD2, allColumn, aElemArr);
			}
		}
	}
}

void CAutoFilterMember::FindFootForBeam(const T_NODE_K& NodeK, vector<T_NODE_K>& aChecked)
{
	T_NODE_D CurrNodeD;
	m_pDoc->m_pAttrCtrl->GetNode(NodeK, CurrNodeD);

	CArray<T_ELEM_K, T_ELEM_K> aElemK;
	int nSize = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(NodeK, aElemK);
	for (int i = 0; i < nSize; ++i)
	{
		T_ELEM_K ElemK = aElemK.GetAt(i);
		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;

		// 이미 기초가 있으면 넘김
		if (IsExistMemberType(eMembar::kFoot, ElemK) == TRUE) continue;

		// 기둥 요소면 안됨
		if (IsExistMemberType(eMembar::kColoum, ElemK) == TRUE) continue;

		// Node I
		T_NODE_K NodeI = ElemD.elnod[0];
		auto iterI = std::find(aChecked.begin(), aChecked.end(), NodeI);
		if (iterI == aChecked.end())
		{
			aChecked.push_back(NodeI);
			// 현재 노드보다 옆/아래에 있는 노드만
			T_NODE_D NodeD;
			m_pDoc->m_pAttrCtrl->GetNode(NodeI, NodeD);
			if (NodeD.z > CurrNodeD.z) continue;

			FindFootForBeam(NodeI, aChecked);
			SetFootElem(NodeI);
		}

		// Node J
		T_NODE_K NodeJ = ElemD.elnod[1];
		auto iterJ = std::find(aChecked.begin(), aChecked.end(), NodeJ);
		if (iterJ == aChecked.end())
		{
			aChecked.push_back(NodeJ);
			// 현재 노드보다 옆/아래에 있는 노드만
			T_NODE_D NodeD;
			m_pDoc->m_pAttrCtrl->GetNode(NodeJ, NodeD);
			if (NodeD.z > CurrNodeD.z) continue;

			FindFootForBeam(NodeJ, aChecked);
			SetFootElem(NodeJ);
		}
	}
}

void CAutoFilterMember::SetFootElem(const T_NODE_K& NodeK)
{
	T_NODE_D CurrNodeD;
	m_pDoc->m_pAttrCtrl->GetNode(NodeK, CurrNodeD);

	CArray<T_ELEM_K, T_ELEM_K> aTmp;
	int nConnectElem = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(NodeK, aTmp);
	for (int i = 0; i < nConnectElem; ++i)
	{
		T_ELEM_K ElemK = aTmp.GetAt(i);
		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;

		if (CustomCheckFilter(ElemD, m_dDgree) != eMembar::kColoum) continue; // 기둥요소인지 확인

		// 예비 기둥에서 찾아야함
		auto iterPre = std::find(m_mPreMembar[eMembar::kColoum].begin(), m_mPreMembar[eMembar::kColoum].end(), ElemK);
		if (iterPre == m_mPreMembar[eMembar::kColoum].end()) continue;

		// 현재 노드보다 아래에 있는 노드만
		T_NODE_D NodeD, NodeD2;
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD);
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD2);
		if (NodeD.z >= CurrNodeD.z && NodeD2.z >= CurrNodeD.z) continue;

		if(IsExistMemberType(eMembar::kFoot, ElemK) == FALSE) m_mMembar[eMembar::kFoot].push_back(ElemK);
		auto pRemove = std::find(m_mPreMembar[eMembar::kColoum].begin(), m_mPreMembar[eMembar::kColoum].end(), ElemK);
		if (pRemove != m_mPreMembar[eMembar::kColoum].end()) m_mPreMembar[eMembar::kColoum].erase(pRemove);
	}
}

void CAutoFilterMember::FindNodeZ2Foot()
{
	for (int i = 0; i <  m_mPreMembar[eMembar::kFoot].size(); ++i)
	{
		T_ELEM_K ElemK = m_mPreMembar[eMembar::kFoot][i];
		T_ELEM_D ElemD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;

		// 한 기둥 모두 찾기
		vector<T_ELEM_K> aColumn;
		MakeOneColumn(ElemD, m_mPreMembar[eMembar::kFoot], aColumn);
		if (aColumn.size() == 0) continue;

		// 가장 낮은 절점
		T_NODE_K LowerNode = FindLowerNode(aColumn);
		// 가장 높은 절점
		T_NODE_K HigherNode = FindHigherNode(aColumn);
		if (LowerNode == 0 || HigherNode == 0) continue;

		CArray<T_ELEM_K, T_ELEM_K> aTemp;
		int nMin = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(LowerNode, aTemp);
		int nMax = m_pDoc->m_pAttrCtrl2->GetConnectedElemByNode(HigherNode, aTemp);
		if (nMin == 1 && nMax > 2)
		{
			// 가장 낮은 절점이 수평절점이 아니고, 가장 높은 절점이 수평절점이면 기초 확정
			for (int j = 0; j < aColumn.size(); ++j)
			{
				T_ELEM_K ColElemK = aColumn[j];
				if (IsExistMemberType(eMembar::kFoot, ColElemK) == FALSE)
				{
					m_mMembar[eMembar::kFoot].push_back(ColElemK);
					auto pRemove = std::find(m_mPreMembar[eMembar::kFoot].begin(), m_mPreMembar[eMembar::kFoot].end(), ColElemK);
					if (pRemove != m_mPreMembar[eMembar::kFoot].end()) m_mPreMembar[eMembar::kFoot].erase(pRemove);
				}
			}
		}
	}
}

void CAutoFilterMember::AddFootForElasticElem()
{
	if (m_mPreMembar[eMembar::kFoot].size() == 0) return;

	vector<T_ELEM_K> aElasticFoot;
	for (int i = 0; i < m_mPreMembar[eMembar::kFoot].size(); ++i)
	{
		T_ELEM_K ElemK = m_mPreMembar[eMembar::kFoot][i];
		T_ELEM_D ElemD;
		T_NODE_D NodeD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
		if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD)) continue;

		// 한 기둥 모두 찾기
		vector<T_ELEM_K> aColumn;
		MakeOneColumn(ElemD, m_mMembar[eMembar::kColoum], aColumn);
		if (aColumn.size() == 0) continue;
		T_NODE_K LowerNode = FindLowerNode(aColumn);
		if (LowerNode == 0) continue;
		T_NODE_D LowerNodeD;
		if (!m_pDoc->m_pAttrCtrl->GetNode(LowerNode, LowerNodeD)) continue;
		if (LowerNodeD.z < NodeD.z) continue;

		// 가장 낮은 절점이 현재 절점보다 아래에 있으면 기초 확정
		aElasticFoot.push_back(ElemK);
	}

	for (int i = 0; i < aElasticFoot.size(); i++)
	{
		T_ELEM_K ElemK = aElasticFoot[i];
		T_ELEM_D ElemD;
		T_NODE_D NodeD;
		if (!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) continue;
		if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD)) continue;

		// 한 기둥 모두 찾기
		vector<T_ELEM_K> aColumn;
		MakeOneColumn(ElemD, m_mPreMembar[eMembar::kFoot], aColumn);
		if (aColumn.size() == 0) continue;

		for (int j = 0; j < aColumn.size(); ++j)
		{
			T_ELEM_K ColElemK = aColumn[j];
			if (IsExistMemberType(eMembar::kFoot, ColElemK) == FALSE)
			{
				m_mMembar[eMembar::kFoot].push_back(ColElemK);
				auto pRemove = std::find(m_mPreMembar[eMembar::kFoot].begin(), m_mPreMembar[eMembar::kFoot].end(), ColElemK);
				if (pRemove != m_mPreMembar[eMembar::kFoot].end()) m_mPreMembar[eMembar::kFoot].erase(pRemove);
			}

		}
	}
}

T_NODE_K CAutoFilterMember::FindLowerNode(const vector<T_ELEM_K>& aColumn)
{
	T_NODE_K NodeKMin = 0;
	double zMin = Z_POSITION_MAX;
	for (int i = 0; i < aColumn.size(); ++i)
	{
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = aColumn[i];
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);

		T_NODE_D NodeD;
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD);
		if (NodeD.z < zMin)
		{
			zMin = NodeD.z;
			NodeKMin = ElemD.elnod[0];
		}

		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD);
		if (NodeD.z < zMin)
		{
			zMin = NodeD.z;
			NodeKMin = ElemD.elnod[1];
		}
	}
	return NodeKMin;
}

T_NODE_K CAutoFilterMember::FindHigherNode(const vector<T_ELEM_K>& aColumn)
{
	T_NODE_K NodeKMax = 0;
	double zMax = Z_POSITION_MIN;
	for (int i = 0; i < aColumn.size(); ++i)
	{
		T_ELEM_D ElemD;
		T_ELEM_K ElemK = aColumn[i];
		m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD);

		T_NODE_D NodeD;
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[0], NodeD);
		if (NodeD.z > zMax)
		{
			zMax = NodeD.z;
			NodeKMax = ElemD.elnod[0];
		}
		m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[1], NodeD);
		if (NodeD.z > zMax)
		{
			zMax = NodeD.z;
			NodeKMax = ElemD.elnod[1];
		}
	}
	return NodeKMax;
}

#pragma endregion

#pragma endregion
