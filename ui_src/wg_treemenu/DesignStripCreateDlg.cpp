#include "stdafx.h"
#include "wg_treemenu.h"
#include "DesignStripCreateDlg.h"

#include "..\wg_base\I_GENStateDefine.h"
#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_base\wg_base_GTF.h"
#include "..\wg_base\ServiceProvider.h"

#include "..\wg_gr\Gr_ServiceDef.h"
#include "..\wg_gr\GDisplaySetting.h"

#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl2.h"

#include "..\wg_main\wg_mainRes2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDesignStripCreateDlg::CDesignStripCreateDlg(CWnd* pParent)
    : CMenuBarChildDlg(CDesignStripCreateDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_nArrDgnPoint = 0;
	m_nDgnPointNo = 1;  // 기본값 1
	m_SptlK = 0;
	m_Data.Initialize();
}

CDesignStripCreateDlg::~CDesignStripCreateDlg()
{
	ChangeSelectionFilter(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_NODE_ELEM);
	//KillTimer(TIMER_CHECK_SELECTION);

	// Design Strip Display Option 복원
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Elem.bDstr = FALSE;
	ExportDisplaySetting::SET(&dspOpt);
}

void CDesignStripCreateDlg::DoDataExchange(CDataExchange* pDX)
{
    CMenuBarChildDlg::DoDataExchange(pDX);
    DDX_Radio(pDX, IDC_TM_DGNSTRIP_CREATE_CENTER_RDO, m_nArrDgnPoint);
	DDX_Text(pDX, IDC_TM_DGNSTRIP_CREATE_NUM_DGN_SECT_EDT, m_nDgnPointNo);
}
BEGIN_MESSAGE_MAP(CDesignStripCreateDlg, CMenuBarChildDlg)
    // Add message map entries here
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CDesignStripCreateDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	//UINT nID = LOWORD(wParam);

	//if (nID == ID_VIEW_SELECT_SINGLE)
	//{
	//	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	//	if (pIGM->GetModelingMode() == IG_STATE_SEL_POINT)
	//	{
	//		ChangeSelectionFilter(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_DSTR);
	//	}
	//}

	return CMenuBarChildDlg::OnCommand(wParam, lParam);
}

BOOL CDesignStripCreateDlg::OnInitDialog()
{
    CMenuBarChildDlg::OnInitDialog();
    
	InitCtrl();

	m_Data.Initialize();
	m_SptlK = 0;

	InitSelectionFilter();

	// Design Strip을 View에서 보이도록 Display Option 설정
	GDisplaySetting dspOpt;
	ExportDisplaySetting::GET(&dspOpt);
	dspOpt.m_Elem.bDstr = TRUE;
	ExportDisplaySetting::SET(&dspOpt);

	//SetTimer(TIMER_CHECK_SELECTION, 100, NULL);

    return TRUE;
}

void CDesignStripCreateDlg::Dlg2Data()
{
	m_Data.nArrDgnPoint = m_nArrDgnPoint;

	//GetSupportingNodes();
	m_Data.SptlK = m_SptlK;

	CreateDesignStrip();
}

void CDesignStripCreateDlg::Data2Dlg()
{
	m_nArrDgnPoint    = m_Data.nArrDgnPoint;   // 0: Center-Center, 1: Edge-Edge

	m_SptlK = m_Data.SptlK;

	UpdateData(FALSE);
	return;
}

//void CDesignStripCreateDlg::GetSupportingNodes()
//{
//	m_aNodeK.RemoveAll();
//
//	CArray<int, int> aNodeK;
//	aNodeK.RemoveAll();
//
//	if (m_bAuto)
//	{
//		CArray<int, int> aElemK;
//		aElemK.RemoveAll();
//
//		CString strElem;
//		m_edtSupElem.GetWindowText(strElem);
//		if (!CStrParser::ParsingListByTo(strElem, aElemK)) return;
//
//		T_ELEM_D ElemD;
//		for (auto i = 0; i < aElemK.GetSize(); i++)
//		{
//			if (!m_pDoc->m_pAttrCtrl->GetElem(aElemK[i], ElemD))
//			{
//				ASSERT(0); return;
//			}
//
//			T_NODE_K MaxLevelNodeK = 0;
//			double dMaxLevelCoord = DBL_MIN;
//
//			for (auto j = 0; j < 8; j++)
//			{
//				if (ElemD.elnod[j] == 0) continue;
//
//				T_NODE_D NodeD;
//				if (!m_pDoc->m_pAttrCtrl->GetNode(ElemD.elnod[j], NodeD)) continue;
//
//				if (dgn::LT(dMaxLevelCoord, NodeD.z))
//				{
//					dMaxLevelCoord = NodeD.z;
//					MaxLevelNodeK = ElemD.elnod[j];
//				}
//			}
//
//			aNodeK.Add(MaxLevelNodeK);
//		}
//	}
//	else
//	{
//		CString strText;
//		m_edtSupNode.GetWindowText(strText);
//		if (!CStrParser::ParsingListByTo(strText, aNodeK)) return;
//	}
//
//	int nSize = aNodeK.GetSize();
//
//	T_NODE_D NodeD; NodeD.Initialize();
//	CArray<T_NODE_UDRD_D, T_NODE_UDRD_D&> aNode;
//	aNode.SetSize(nSize);
//	for (auto i = 0; i < nSize; i++)
//	{
//		if (!m_pDoc->m_pAttrCtrl->GetNode(aNodeK[i], NodeD))
//		{
//			ASSERT(0); return;
//		}
//		aNode[i].key = aNodeK[i];
//		aNode[i].data.x = NodeD.x;
//		aNode[i].data.y = NodeD.y;
//		aNode[i].data.z = NodeD.z;
//	}
//
//	int nDir = m_Data.nDgnStripDir;
//	if (!m_pDoc->m_pAttrCtrl2->SortNodes4Dir(aNode, nDir))
//	{
//		ASSERT(0); return;
//	}
//
//	for (auto i = 0; i < nSize; i++)
//	{
//		m_aNodeK.Add(aNode[i].key);
//	}
//}

void CDesignStripCreateDlg::CreateDesignStrip()
{
	// Step 1: 모든 Support Line 키 목록 조회
	CArray<T_SPTL_K, T_SPTL_K> aSptlKeyList;
	m_pDoc->m_pAttrCtrl2->GetSptlKeyList(aSptlKeyList);

	if (aSptlKeyList.GetSize() == 0) return;

	// Step 2: Direction별로 Support Line 분류
	CArray<T_SPTL_K, T_SPTL_K> aXDirSptlKeys;  // nDirection == 0
	CArray<T_SPTL_K, T_SPTL_K> aYDirSptlKeys;  // nDirection == 1

	GetSupportLinesByDirection(0, aXDirSptlKeys);
	GetSupportLinesByDirection(1, aYDirSptlKeys);

	// Step 3: X방향 Support Line 처리
	for (int i = 0; i < aXDirSptlKeys.GetSize(); i++)
	{
		T_DSTR_D DstrData;
		DstrData.Initialize();

		if (CreateDesignStripForSingleSptl(aXDirSptlKeys[i], aXDirSptlKeys, DstrData))
		{
			m_pDoc->m_pDataCtrl->AddDstr(DstrData);
		}
	}

	// Step 4: Y방향 Support Line 처리
	for (int i = 0; i < aYDirSptlKeys.GetSize(); i++)
	{
		T_DSTR_D DstrData;
		DstrData.Initialize();

		if (CreateDesignStripForSingleSptl(aYDirSptlKeys[i], aYDirSptlKeys, DstrData))
		{
			m_pDoc->m_pDataCtrl->AddDstr(DstrData);
		}
	}
}

void CDesignStripCreateDlg::GetSupportLinesByDirection(int nDirection, CArray<T_SPTL_K, T_SPTL_K>& rSptlKeyList)
{
	rSptlKeyList.RemoveAll();

	CArray<T_SPTL_K, T_SPTL_K> aAllSptlKeys;
	m_pDoc->m_pAttrCtrl2->GetSptlKeyList(aAllSptlKeys);

	for (int i = 0; i < aAllSptlKeys.GetSize(); i++)
	{
		T_SPTL_D SptlD;
		if (m_pDoc->m_pAttrCtrl2->GetSptl(aAllSptlKeys[i], SptlD))
		{
			if (SptlD.nDirection == nDirection)
			{
				rSptlKeyList.Add(aAllSptlKeys[i]);
			}
		}
	}
}

BOOL CDesignStripCreateDlg::CreateDesignStripForSingleSptl(
	T_SPTL_K SptlKey,
	const CArray<T_SPTL_K, T_SPTL_K>& aSameDirSptlKeys,
	T_DSTR_D& rDstrData)
{
	// Step 1: Support Line 데이터 조회
	T_SPTL_D SptlD;
	if (!m_pDoc->m_pAttrCtrl2->GetSptl(SptlKey, SptlD))
	{
		return FALSE;
	}

	// Step 2: 노드가 2개 미만이면 Span 생성 불가
	if (SptlD.aNodeK.GetSize() < 2)
	{
		return FALSE;
	}

	// Step 3: 기본 정보 설정
	rDstrData.nArrDgnPoint = m_nArrDgnPoint;
	rDstrData.SptlK = SptlKey;
	rDstrData.nDgnSection = m_nDgnPointNo;

	int nDirection = SptlD.nDirection;

	// Step 4: 각 노드 쌍에 대해 Span 생성
	CArray<UINT, UINT> aNodeK;
	aNodeK.RemoveAll();

	if ( SptlD.StartNodeK > 0 )
		aNodeK.Add(SptlD.StartNodeK);

	for (int i = 0; i < SptlD.aNodeK.GetSize(); i++)
	{
		aNodeK.Add(SptlD.aNodeK[i]);
	}

	if ( SptlD.EndNodeK > 0 )
		aNodeK.Add(SptlD.EndNodeK);

	for (int i = 0; i < aNodeK.GetSize() - 1; i++)
	{
		T_NODE_K StartNodeK = aNodeK[i];
		T_NODE_K EndNodeK = aNodeK[i + 1];

		T_NODE_D StartNodeD, EndNodeD;
		if (!m_pDoc->m_pAttrCtrl->GetNode(StartNodeK, StartNodeD) ||
			!m_pDoc->m_pAttrCtrl->GetNode(EndNodeK, EndNodeD))
		{
			continue;
		}

		// Step 4.1: 시작 노드에서 인접 SL 거리 계산
		double dStart_TL = 0.0, dStart_BR = 0.0;
		CalculateAdjacentDistances(StartNodeD, nDirection, aSameDirSptlKeys, SptlKey, dStart_TL, dStart_BR);

		// Step 4.2: 끝 노드에서 인접 SL 거리 계산
		double dEnd_TL = 0.0, dEnd_BR = 0.0;
		CalculateAdjacentDistances(EndNodeD, nDirection, aSameDirSptlKeys, SptlKey, dEnd_TL, dEnd_BR);

		// Step 4.3: 폭 계산 (인접 거리의 절반, 가장자리는 0)
		T_DGNSTRIP_SPAN_INFO SpanInfo;
		SpanInfo.nSpanNo = i + 1;
		SpanInfo.dWidth_StartTL = dStart_TL / 2.0;
		SpanInfo.dWidth_StartBR = dStart_BR / 2.0;
		SpanInfo.dWidth_EndTL = dEnd_TL / 2.0;
		SpanInfo.dWidth_EndBR = dEnd_BR / 2.0;
		SpanInfo.DgnPointNo = m_nDgnPointNo;

		MakeDesignSection(StartNodeK, EndNodeK, SpanInfo);

		rDstrData.aSpan.Add(SpanInfo);
	}

	return (rDstrData.aSpan.GetSize() > 0);
}

void CDesignStripCreateDlg::CalculateAdjacentDistances(
	const T_NODE_D& NodeD,
	int nDirection,
	const CArray<T_SPTL_K, T_SPTL_K>& aSameDirSptlKeys,
	T_SPTL_K CurrentSptlK,
	double& dDistanceTL,
	double& dDistanceBR)
{
	dDistanceTL = 0.0;
	dDistanceBR = 0.0;

	// 현재 노드의 수직 좌표 (X방향 SL이면 Y좌표, Y방향 SL이면 X좌표)
	double dCurrentCoord = (nDirection == 0) ? NodeD.y : NodeD.x;

	// 인접 SL 탐색을 위한 최소 거리 추적
	double dMinDistTL = DBL_MAX;  // Top/Left 방향 (양수 방향)
	double dMinDistBR = DBL_MAX;  // Bottom/Right 방향 (음수 방향)

	for (int i = 0; i < aSameDirSptlKeys.GetSize(); i++)
	{
		T_SPTL_K OtherSptlK = aSameDirSptlKeys[i];
		if (OtherSptlK == CurrentSptlK) continue;

		T_SPTL_D OtherSptlD;
		if (!m_pDoc->m_pAttrCtrl2->GetSptl(OtherSptlK, OtherSptlD)) continue;

		// 다른 SL의 노드들 중 현재 노드와 가장 가까운 수직 좌표 계산
		for (int j = 0; j < OtherSptlD.aNodeK.GetSize(); j++)
		{
			T_NODE_D OtherNodeD;
			if (!m_pDoc->m_pAttrCtrl->GetNode(OtherSptlD.aNodeK[j], OtherNodeD)) continue;

			double dOtherCoord = (nDirection == 0) ? OtherNodeD.y : OtherNodeD.x;
			double dDiff = dOtherCoord - dCurrentCoord;

			if (dDiff > 0)  // Top/Left 방향 (양수)
			{
				if (dDiff < dMinDistTL)
				{
					dMinDistTL = dDiff;
				}
			}
			else if (dDiff < 0)  // Bottom/Right 방향 (음수)
			{
				double dAbsDiff = fabs(dDiff);
				if (dAbsDiff < dMinDistBR)
				{
					dMinDistBR = dAbsDiff;
				}
			}
		}
	}

	// 가장 가까운 인접 SL까지의 거리 반환 (없으면 0)
	dDistanceTL = (dMinDistTL < DBL_MAX) ? dMinDistTL : 0.0;
	dDistanceBR = (dMinDistBR < DBL_MAX) ? dMinDistBR : 0.0;
}

void CDesignStripCreateDlg::InitSelectionFilter()
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_SPTL);
}

void CDesignStripCreateDlg::ChangeSelectionFilter(EN_SELECT_ELEM_FILTER enFilter)
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	pIGM->ChangeSelectionFilterCombo(enFilter);
}

void CDesignStripCreateDlg::InitCtrl()
{
}

void CDesignStripCreateDlg::CheckDesignStripSelectionChanged()
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if (!pIGM)
	{
		ASSERT(0); return;
	}

	if (pIGM->GetSelectFilter() == EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_SPTL)
	{
		LoadSelectedDesignStripInfo();
	}
}

void CDesignStripCreateDlg::LoadSelectedDesignStripInfo()
{
	I_GENModelBase* pIGM = CTreeMenuBarBase::GetI_GENModelST();
	if (!pIGM)
	{
		ASSERT(0); return;
	}

	CArray<T_DSTR_K, T_DSTR_K> aDstrK;
	m_pDoc->m_pAttrCtrl2->GetDstrKeyList(aDstrK);

	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	ASSERT(pServiceHeadOffice);

	T_DSTR_K selectedDstrK = 0;
	for (auto i = 0; i < aDstrK.GetSize(); i++)
	{
		if (pServiceHeadOffice->ReqService(_ULS(gr), D_GR_GET_SELECTED_DSTR, (void*)(&aDstrK[i])))
		{
			selectedDstrK = aDstrK[i]; break;
		}
	}

	if (selectedDstrK == 0) return;

	T_DSTR_D DstrD;
	if (!m_pDoc->m_pAttrCtrl2->GetDstr(selectedDstrK, DstrD))
	{
		ASSERT(0); return;
	}

	m_Data.Initialize();
	m_Data = DstrD;

	Data2Dlg();

	UpdateData(FALSE);
}

void CDesignStripCreateDlg::MakeDesignSection(UINT StartNodeK, UINT EndNodeK, T_DGNSTRIP_SPAN_INFO& SpanData)
{
	int nDgnPointNo = (int)SpanData.DgnPointNo;
	if (nDgnPointNo <= 0) return;

	SpanData.aDspsK.RemoveAll();

	for (int i = 0; i < nDgnPointNo; i++)
	{
		T_DSPS_D DspsD;
		DspsD.Initialize();

		DspsD.Node1 = StartNodeK;
		DspsD.Node2 = EndNodeK;

		// dDist: Span 내 위치 비율 (0.0 ~ 1.0)
		// nDgnPointNo가 1이면 0.5 (중앙), 그 외에는 등간격 배치
		double dRatio;
		if (nDgnPointNo == 1)
			dRatio = 0.5;
		else
			dRatio = (double)i / (double)(nDgnPointNo - 1);

		DspsD.dDist = dRatio;

		// dLeft, dRight: Start와 End 값을 dRatio로 interpolation
		// dLeft = StartTL + (EndTL - StartTL) * dRatio
		// dRight = StartBR + (EndBR - StartBR) * dRatio
		DspsD.dLeft = SpanData.dWidth_StartTL + (SpanData.dWidth_EndTL - SpanData.dWidth_StartTL) * dRatio;
		DspsD.dRight = SpanData.dWidth_StartBR + (SpanData.dWidth_EndBR - SpanData.dWidth_StartBR) * dRatio;

		// DB에 Design Section 추가하고 생성된 Key를 aDspsK에 저장
		T_DSPS_K DspsK = m_pDoc->m_pAttrCtrl2->GetLastNumDsps() + 1;
		if (m_pDoc->m_pDataCtrl->AddDsps(DspsK, DspsD))
		{
			SpanData.aDspsK.Add(DspsK);
		}
	}
}

void CDesignStripCreateDlg::OnDblclkTmSpanWidthList(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CDesignStripCreateDlg::OnSetfocusNodeList()
{

}

void CDesignStripCreateDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_CHECK_SELECTION)
	{
		CheckDesignStripSelectionChanged();
	}

	CMenuBarChildDlg::OnTimer(nIDEvent);
}

void CDesignStripCreateDlg::Execute()
{
	UpdateData(TRUE);

	bool bDesignStripSelectMode = false;
	CServiceHeadOffice* pServiceHeadOffice = CServiceHeadOffice::Instance();
	ASSERT(pServiceHeadOffice);
	int nReq = pServiceHeadOffice->ReqService(_ULS(gr), D_GR_CREATE_DESIGN_STRIP, (void*)(&bDesignStripSelectMode));

	m_Data.Initialize();

	Dlg2Data();
	// Todo HJM : Data2Dlg가 필요한지 확인 필요

	ChangeSelectionFilter(EN_SELECT_ELEM_FILTER::EN_SEL_ELEM_FILTER_DSTR);
}

void CDesignStripCreateDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	case D_UPDATE_SEL_ADD:
	case D_UPDATE_SEL_DEL:
		break;
	default: ASSERT(TRUE);
	}
}

void CDesignStripCreateDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return;
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	BOOL bMFD = FALSE;
	while (pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;

		switch (nCmd)
		{
		case(UR_EVGP_ADD):
		case(UR_EVGP_DEL):
		case(UR_EVGP_MFD):
		case(UR_EVGP_MFS):
			break;
		case(UR_SECT_ADD):
		case(UR_SECT_DEL):
		case(UR_SECT_MFD):
		case(UR_SECT_MFS):
		case(UR_ELEM_ADD):
		case(UR_ELEM_DEL):
		case(UR_ELEM_MFD):
		case(UR_ELEM_MFS):
			break;
		default:
			break;
		}
	}
}