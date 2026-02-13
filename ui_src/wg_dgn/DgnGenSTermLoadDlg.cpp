// DgnConBarSpaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnGenSTermLoadDlg.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_QSort.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "DgnDataCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSpaceDlg dialog

#define IDLOADCASE0(n)\
	IDLOADCASE0.Add(IDC_DGN_SL_TERM_LOAD##n);
#define IDLOADCASE1(n)\
	IDLOADCASE1.Add(IDC_DGN_SHORT_TERM##n);

CDgnGenSTermLoadDlg::CDgnGenSTermLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnGenSTermLoadDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDgnConBarSpaceDlg)
	//}}AFX_DATA_INIT
	m_arLongLoadMap.RemoveAll();
	m_arLoadMap.RemoveAll();
	m_arXLoadMap.RemoveAll();
	m_arYLoadMap.RemoveAll();
}


void CDgnGenSTermLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnGenSTermLoadDlg)
	DDX_Control(pDX, IDC_DGN_SL_TERM_LOAD1, m_aLoadList);
	DDX_Control(pDX, IDC_DGN_SHORT_TERM5, m_aXLoadList);
	DDX_Control(pDX, IDC_DGN_SHORT_TERM7, m_aYLoadList);
	DDX_Control(pDX, IDC_DGN_SL_TERM_LOAD5, m_aLongLoadList);
	//}}AFX_DATA_MAP

	CArray<UINT,UINT> IDLOADCASE0; IDLOADCASE0(0)IDLOADCASE0(1)IDLOADCASE0(2)IDLOADCASE0(3)IDLOADCASE0(4)
																 IDLOADCASE0(5);
	CArray<UINT,UINT> IDLOADCASE1; IDLOADCASE1(0)IDLOADCASE1(1)IDLOADCASE1(2)IDLOADCASE1(3)IDLOADCASE1(4)
																 IDLOADCASE1(5)IDLOADCASE1(6)IDLOADCASE1(7);

	int i;
	for(i=0 ; i<iDgn_SLTermID; i++)    m_iSLTermID[i] = IDLOADCASE0[i];
	for(i=0 ; i<iDgn_ShortTermID; i++) m_iShortTermID[i] = IDLOADCASE1[i];
	
}

#define CDialog CDialogMove
BEGIN_MESSAGE_MAP(CDgnGenSTermLoadDlg, CInternationalDlg)
#undef CDialog

	//{{AFX_MSG_MAP(CDgnGenSTermLoadDlg)
	ON_BN_CLICKED(IDC_DGN_SL_TERM_LOAD2, OnDgnAddLongLoadBtn)
	ON_BN_CLICKED(IDC_DGN_SL_TERM_LOAD3, OnDgnDelLongLoadBtn)
	ON_BN_CLICKED(IDC_DGN_SHORT_TERM0, OnDgnAddXLoadBtn)
	ON_BN_CLICKED(IDC_DGN_SHORT_TERM1, OnDgnDelXLoadBtn)
	ON_BN_CLICKED(IDC_DGN_SHORT_TERM2, OnDgnAddYLoadBtn)
	ON_BN_CLICKED(IDC_DGN_SHORT_TERM3, OnDgnDelYLoadBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnConBarSpaceDlg message handlers


void CDgnGenSTermLoadDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CDgnGenSTermLoadDlg::OnOK() 
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	_DGN_STLD Load; 
	T_LCTP_D LctpData; LctpData.Initialize();

	int Index = 0;
	POSITION Pos = m_arLongLoadMap.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		m_arLongLoadMap.GetNextAssoc(Pos, Index, Load);
		LctpData.arLongKey.Add(Load.key);
		LctpData.arLongType.Add(Load.iAnalType);
	}

	Pos = m_arXLoadMap.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		m_arXLoadMap.GetNextAssoc(Pos, Index, Load);
		LctpData.arShtXKey.Add(Load.key);
		LctpData.arShtXType.Add(Load.iAnalType);
	}

	Pos = m_arYLoadMap.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		m_arYLoadMap.GetNextAssoc(Pos, Index, Load);
		LctpData.arShtYKey.Add(Load.key);
		LctpData.arShtYType.Add(Load.iAnalType);
	}

	pDoc->m_pDataCtrl->AddLctp(LctpData);

	CDialogMove::OnOK();
}

BOOL CDgnGenSTermLoadDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();

	InitCtrl();

	UpdateData(FALSE);
	return TRUE;
}

void CDgnGenSTermLoadDlg::InitDgnCode()
{    
	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	CDgnDataCtrl DataCtrl;	
	//Steel
	T_DSTL_D DstlData;
	DataCtrl.Get_DgnStlDstl(DstlData);
	//Concrete
	T_DCON_D DconData;
	DataCtrl.Get_DgnConDcon(DconData);
	//SRC
	T_DSRC_D DsrcData;
	DataCtrl.Get_DgnSrcDsrc(DsrcData);
	
	// Change by ZINU.('05.08.29). 각각 재질별 AIJ검토 -> 하나라도 AIJ이면 장단기를 적용할수 있도록 수정.
	BOOL bStlAIJ = (pDoc->IsExistStlForDgn() && DstlData.DesignCode==_T("AIJ-ASD02") ? TRUE : FALSE);
	BOOL bConAIJ = (pDoc->IsExistConForDgn() && DconData.DesignCode==_T("AIJ-WSD99") ? TRUE : FALSE);
	BOOL bSrcAIJ = (pDoc->IsExistSrcForDgn() && DsrcData.DesignCode==_T("AIJ-SRC01") ? TRUE : FALSE);
	if(!(bStlAIJ || bConAIJ || bSrcAIJ))	MoveCtrl();
	/*
	if(pDoc->IsExistStlForDgn())
	{
		if(DstlData.DesignCode!=_T("AIJ-ASD02"))
			MoveCtrl();
	}
	else if(pDoc->IsExistConForDgn())
	{
		if(DconData.DesignCode!=_T("AIJ-WSD99"))
			MoveCtrl();
	}
	else if(pDoc->IsExistSrcForDgn())
	{
		if(DsrcData.DesignCode!=_T("AIJ-SRC01"))
			MoveCtrl();
	}
	*/
}

void CDgnGenSTermLoadDlg::InitLCaseData()
{
	m_arLongLoadMap.RemoveAll();
	m_arLoadMap.RemoveAll();
	m_arXLoadMap.RemoveAll();
	m_arYLoadMap.RemoveAll();

	_DGN_STLD Load; 

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	ASSERT(pDoc);

	T_LCTP_D LctpData; LctpData.Initialize();
	BOOL bExist = pDoc->m_pAttrCtrl->GetLctp(LctpData);

	int iSeqNum = 0;
	//D_LCOM_STATIC		1 : 정적 하중 조건
	T_STLD_D rData;
	CArray<T_STLD_K,T_STLD_K> KeyList;
	pDoc->m_pAttrCtrl->GetStldKeyList(KeyList);
	for(int i=0; i<KeyList.GetSize(); i++)
	{
		int key = KeyList.GetAt(i);
		T_LCTB_K LctbK;
		if(pDoc->m_pAttrCtrl->GetLctbKeyFromStldKey(key, LctbK))	continue;

		rData.Initialize();
		if(!pDoc->m_pAttrCtrl->GetStld(key,rData))	continue;
		Load.Initialize();
		Load.strName = rData.LoadCaseName;
		Load.key = key;
		Load.iAnalType = D_LCOM_STATIC;
		Load.iSeqNum = iSeqNum;
		
//    if(LctpData.arLongKey.GetSize() == 0 &&
//       CDBLib::IsStldDL(rData.LoadCaseType) || CDBLib::IsStldLL(rData.LoadCaseType))
//      m_arLongLoadMap.SetAt(iSeqNum, Load);
//    else 
//      m_arLoadMap.SetAt(iSeqNum, Load);

		if(!bExist)
		{
			if (CDBLib::IsStldLongTerm(rData.LoadCaseType))
				m_arLongLoadMap.SetAt(iSeqNum, Load);
			else 
				m_arLoadMap.SetAt(iSeqNum, Load);
		}
		else
				m_arLoadMap.SetAt(iSeqNum, Load);
		iSeqNum++;
	}
	//D_LCOM_SPECTRUM		2 : 응답 스펙트럼 하중 조건
	T_SPLC_K SplcKey;
	T_SPLC_D SplcData;
	CArray<T_SPLC_K, T_SPLC_K> aSplcKey;
	pDoc->m_pAttrCtrl->GetSplcKeyList(aSplcKey);
	for(int i = 0; i < aSplcKey.GetSize(); i++)
	{
		SplcKey = aSplcKey[i];
		pDoc->m_pAttrCtrl->GetSplc(SplcKey, SplcData);
		Load.Initialize();
		Load.strName = SplcData.LoadCaseName + _T("(RS)");
		Load.key = SplcKey;
		Load.iAnalType = D_LCOM_SPECTRUM;
		Load.iSeqNum = iSeqNum;
		m_arLoadMap.SetAt(iSeqNum, Load);
		iSeqNum++;
	}

	//D_LCOM_HISTORY		3 : 시간 이력 하중 조건
	T_THIS_K ThisKey;
	T_THIS_D ThisData;
	CArray<T_THIS_K, T_THIS_K> aThisKey;
	pDoc->m_pAttrCtrl->GetThisKeyList(aThisKey);
	for (int i = 0; i < aThisKey.GetSize(); i++)
	{
		ThisKey = aThisKey[i];
		pDoc->m_pAttrCtrl->GetThis(ThisKey, ThisData);
		Load.Initialize();
		Load.strName = ThisData.LoadCaseName;
		Load.key = ThisKey;
		Load.iAnalType = D_LCOM_HISTORY;
		Load.iSeqNum = iSeqNum;
		m_arLoadMap.SetAt(iSeqNum, Load);
		iSeqNum++;
	}
	
	//D_LCOM_MOVING		  4 : 이동 하중 조건
	// 코드에 따른 방식으로 수정 
	T_MVCD_D DataMvcd;
	if(!pDoc->m_pAttrCtrl->GetMvcd(DataMvcd)) DataMvcd.Initialize();
	switch (DataMvcd.nCodeType)
	{
	case D_MOVE_CODE_NONE:
		break;
	case D_MOVE_CODE_AASHTO_STAN:
	case D_MOVE_CODE_AASHTO_LRFD:
	case D_MOVE_CODE_TAIWAN:
	case D_MOVE_CODE_KOREA:
	case D_MOVE_CODE_CANADA:
	case D_MOVE_CODE_PENDOT:
	case D_MOVE_CODE_EURO_BS:
	case D_MOVE_CODE_RUSSIA:
	case D_MOVE_CODE_KOREA_LRFD_2011:
	case D_MOVE_CODE_AUSTRALIA:
	case D_MOVE_CODE_POLAND:
	case D_MOVE_CODE_SOUTH_AFRICA:
	case D_MOVE_CODE_NEWZEALAND:
	case D_MOVE_CODE_BRAZIL:
		{
			T_MVLD_K MvldKey;
			T_MVLD_D MvldData;
			CArray<T_MVLD_K, T_MVLD_K> aMvldKey;
			pDoc->m_pAttrCtrl->GetMvldKeyList(aMvldKey);
			for (int i = 0; i < aMvldKey.GetSize(); i++)
			{
				MvldKey = aMvldKey[i];
				pDoc->m_pAttrCtrl->GetMvld(MvldKey, MvldData);
				Load.Initialize();
				Load.strName = MvldData.LoadCaseName;
				Load.key = MvldKey;
				Load.iAnalType = D_LCOM_MOVING;
				Load.iSeqNum = iSeqNum;
				m_arLoadMap.SetAt(iSeqNum, Load);
				iSeqNum++;
			}
		}
		break;
	case D_MOVE_CODE_CHINA:
		{
			T_MVLDch_K MvldchKey;
			T_MVLDch_D MvldchData;
			CArray<T_MVLDch_K, T_MVLDch_K> aMvldchKey;
			pDoc->m_pAttrCtrl->GetMvldchKeyList(aMvldchKey);
			for (int i = 0; i < aMvldchKey.GetSize(); i++)
			{
				MvldchKey = aMvldchKey[i];
				pDoc->m_pAttrCtrl->GetMvldch(MvldchKey, MvldchData);
				Load.Initialize();
				Load.strName = MvldchData.LoadCaseName;
				Load.key = MvldchKey;
				Load.iAnalType = D_LCOM_MOVING;
				Load.iSeqNum = iSeqNum;
				m_arLoadMap.SetAt(iSeqNum, Load);
				iSeqNum++;
			}
		}
		break;
	case D_MOVE_CODE_JAPAN:
		{
			T_MVLDjp_K MvldjpKey;
			T_MVLDjp_D MvldjpData;
			CArray<T_MVLDjp_K, T_MVLDjp_K> aMvldjpKey;
			pDoc->m_pAttrCtrl->GetMvldjpKeyList(aMvldjpKey);
			for (int i = 0; i < aMvldjpKey.GetSize(); i++)
			{
				MvldjpKey = aMvldjpKey[i];
				pDoc->m_pAttrCtrl->GetMvldjp(MvldjpKey, MvldjpData);
				Load.Initialize();
				Load.strName = MvldjpData.LoadCaseName;
				Load.key = MvldjpKey;
				Load.iAnalType = D_LCOM_MOVING;
				Load.iSeqNum = iSeqNum;
				m_arLoadMap.SetAt(iSeqNum, Load);
				iSeqNum++;
			}
		}
		break;
	case D_MOVE_CODE_INDIA:
		{
			T_MVLDid_K MvldidKey;
			T_MVLDid_D MvldidData;
			CArray<T_MVLDid_K, T_MVLDid_K> aMvldidKey;
			pDoc->m_pAttrCtrl->GetMvldidKeyList(aMvldidKey);
			for (int i = 0; i < aMvldidKey.GetSize(); i++)
			{
				MvldidKey = aMvldidKey[i];
				pDoc->m_pAttrCtrl->GetMvldid(MvldidKey, MvldidData);
				Load.Initialize();
				Load.strName = MvldidData.LoadCaseName;
				Load.key = MvldidKey;
				Load.iAnalType = D_LCOM_MOVING;
				Load.iSeqNum = iSeqNum;
				m_arLoadMap.SetAt(iSeqNum, Load);
				iSeqNum++;
			}
		}
		break;
	case D_MOVE_CODE_BS:
		{
			T_MVLDbs_K MvldbsKey;
			T_MVLDbs_D MvldbsData;
			CArray<T_MVLDbs_K, T_MVLDbs_K> aMvldbsKey;
			pDoc->m_pAttrCtrl->GetMvldbsKeyList(aMvldbsKey);
			for (int i = 0; i < aMvldbsKey.GetSize(); i++)
			{
				MvldbsKey = aMvldbsKey[i];
				pDoc->m_pAttrCtrl->GetMvldbs(MvldbsKey, MvldbsData);
				Load.Initialize();
				Load.strName = MvldbsData.LoadCaseName;
				Load.key = MvldbsKey;
				Load.iAnalType = D_LCOM_MOVING;
				Load.iSeqNum = iSeqNum;
				m_arLoadMap.SetAt(iSeqNum, Load);
				iSeqNum++;
			}
		}
		break;
	case D_MOVE_CODE_FRANCE:
		{
			T_MVLDfr_K MvldfrKey;
			T_MVLDfr_D MvldfrData;
			CArray<T_MVLDfr_K, T_MVLDfr_K> aMvldfrKey;
			pDoc->m_pAttrCtrl->GetMvldfrKeyList(aMvldfrKey);
			for (int i = 0; i < aMvldfrKey.GetSize(); i++)
			{
				MvldfrKey = aMvldfrKey[i];
				pDoc->m_pAttrCtrl->GetMvldfr(MvldfrKey, MvldfrData);
				Load.Initialize();
				Load.strName = MvldfrData.LoadCaseName;
				Load.key = MvldfrKey;
				Load.iAnalType = D_LCOM_MOVING;
				Load.iSeqNum = iSeqNum;
				m_arLoadMap.SetAt(iSeqNum, Load);
				iSeqNum++;
			}
		}
		break;
	case D_MOVE_CODE_TRANS:
		{
			T_MVLDtr_K MvldKey;
			T_MVLDtr_D MvldData;
			CArray<T_MVLDtr_K, T_MVLDtr_K> aMvldKey;
			pDoc->m_pAttrCtrl->GetMvldtrKeyList(aMvldKey);
			for (int i = 0; i < aMvldKey.GetSize(); i++)
			{
				MvldKey = aMvldKey[i];
				pDoc->m_pAttrCtrl->GetMvldtr(MvldKey, MvldData);
				Load.Initialize();
				Load.strName = MvldData.LoadCaseName;
				Load.key = MvldKey;
				Load.iAnalType = D_LCOM_MOVING;
				Load.iSeqNum = iSeqNum;
				m_arLoadMap.SetAt(iSeqNum, Load);
				iSeqNum++;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}


	//D_LCOM_SETTLEMENT	5 : 지점침하 하중 조건
	T_SMLC_K SmlcKey;
	T_SMLC_D SmlcData;
	CArray<T_SMLC_K, T_SMLC_K> aSmlcKey;
	pDoc->m_pAttrCtrl->GetSmlcKeyList(aSmlcKey);
	for (int i = 0; i < aSmlcKey.GetSize(); i++)
	{
		SmlcKey = aSmlcKey[i];
		pDoc->m_pAttrCtrl->GetSmlc(SmlcKey, SmlcData);
		Load.Initialize();
		Load.strName = SmlcData.LoadCaseName;
		Load.key = SmlcKey;
		Load.iAnalType = D_LCOM_SETTLEMENT;
		Load.iSeqNum = iSeqNum;
		m_arLoadMap.SetAt(iSeqNum, Load);
		iSeqNum++;
	}

	//D_LCOM_STAGE 13 : 시공해석 하중 조건
	T_SGLD_K SgldKey;
	T_SGLD_D SgldData;
	CArray<T_SGLD_K, T_SGLD_K> aSgldKey;
	pDoc->m_pAttrCtrl->GetSgldKeyList(aSgldKey);
	for (int i = 0; i < aSgldKey.GetSize(); i++)
	{
		SgldKey = aSgldKey[i];
		pDoc->m_pAttrCtrl->GetSgld(SgldKey, SgldData);
		Load.Initialize();
		Load.strName = SgldData.LoadCaseName;
		Load.key = SgldKey;
		Load.iAnalType = D_LCOM_STAGE;
		Load.iSeqNum = iSeqNum;
		m_arLoadMap.SetAt(iSeqNum, Load);
		iSeqNum++;
	}

	//D_LCOM_ECCENSPEC  : 우발편심 응답 스펙트럼 하중 조건
	T_ESPC_K EspcKey;
	T_ESPC_D EspcData;
	CArray<T_ESPC_K, T_ESPC_K> aEspcKey;
	pDoc->m_pAttrCtrl->GetEspcKeyList(aEspcKey);
	for (int i = 0; i < aEspcKey.GetSize(); i++)
	{
		EspcKey = aEspcKey[i];
		pDoc->m_pAttrCtrl->GetEspc(EspcKey, EspcData);
		Load.Initialize();
		Load.strName = EspcData.LoadCaseName + _T("(ES)");
		Load.key = EspcKey;
		Load.iAnalType = D_LCOM_ECCENSPEC;
		Load.iSeqNum = iSeqNum;
		m_arLoadMap.SetAt(iSeqNum, Load);
		iSeqNum++;
	}

	//Long term
	for(int j=0; j<LctpData.arLongKey.GetSize(); j++)
	{
		int key = LctpData.arLongKey.GetAt(j);
		int iType = LctpData.arLongType.GetAt(j);
		int Index = 0;
	  POSITION Pos = m_arLoadMap.GetStartPosition();
	  while(Pos)
	  {
		  Load.Initialize();
		  m_arLoadMap.GetNextAssoc(Pos, Index, Load);
			if(Load.key == key && Load.iAnalType == iType)
			{
				m_arLongLoadMap.SetAt(Load.iSeqNum, Load);
				m_arLoadMap.RemoveKey(Load.iSeqNum);
				break;
			}
		}
	}
	//Short-term X Direction
	for(int j=0; j<LctpData.arShtXKey.GetSize(); j++)
	{
		int key = LctpData.arShtXKey.GetAt(j);
		int iType = LctpData.arShtXType.GetAt(j);
		int Index = 0;
        POSITION Pos = m_arLoadMap.GetStartPosition();
        while ( Pos )
        {
            Load.Initialize();
            m_arLoadMap.GetNextAssoc(Pos, Index, Load);
            if ( Load.key == key && Load.iAnalType == iType )
            {
                m_arXLoadMap.SetAt(Load.iSeqNum, Load);
                m_arLoadMap.RemoveKey(Load.iSeqNum);
				break;
			}
		}
	}
	//Short-term Y Direction
	for(int j=0; j<LctpData.arShtYKey.GetSize(); j++)
	{
		int key = LctpData.arShtYKey.GetAt(j);
		int iType = LctpData.arShtYType.GetAt(j);
		int Index = 0;
        POSITION Pos = m_arLoadMap.GetStartPosition();
        while ( Pos )
        {
            Load.Initialize();
            m_arLoadMap.GetNextAssoc(Pos, Index, Load);
            if ( Load.key == key && Load.iAnalType == iType )
            {
                m_arYLoadMap.SetAt(Load.iSeqNum, Load);
                m_arLoadMap.RemoveKey(Load.iSeqNum);
                break;
			}
		}
	}

}

void CDgnGenSTermLoadDlg::InitCtrl()
{
	InitDgnCode();
	InitLCaseData();

	DWORD dwStyle;
	dwStyle = ListView_GetExtendedListViewStyle(m_aLoadList.GetSafeHwnd());
	dwStyle |= LVS_EX_FULLROWSELECT; 
	dwStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aLoadList.GetSafeHwnd(),dwStyle);

	LV_COLUMN lvcolumn;
	TCHAR* list[2] = {_T("Load Case"), _T("Seq")};
	int width[2]	 = {120,0};
	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aLoadList.InsertColumn(i,&lvcolumn);
	}

	Write_ListCtrl(m_aLoadList, m_arLoadMap);

	//Long-term
	DWORD LongStyle;
	LongStyle = ListView_GetExtendedListViewStyle(m_aLongLoadList.GetSafeHwnd());
	LongStyle |= LVS_EX_FULLROWSELECT; 
	LongStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aLongLoadList.GetSafeHwnd(),LongStyle);

	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aLongLoadList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aLongLoadList, m_arLongLoadMap);

	//Short-term X-Dir
	DWORD XStyle;
	XStyle = ListView_GetExtendedListViewStyle(m_aXLoadList.GetSafeHwnd());
	XStyle |= LVS_EX_FULLROWSELECT; 
	XStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aXLoadList.GetSafeHwnd(),XStyle);

	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aXLoadList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aXLoadList, m_arXLoadMap);

	//Short-term Y-Dir
	DWORD YStyle;
	YStyle = ListView_GetExtendedListViewStyle(m_aYLoadList.GetSafeHwnd());
	YStyle |= LVS_EX_FULLROWSELECT; 
	YStyle |= LVS_NOCOLUMNHEADER;
	ListView_SetExtendedListViewStyle(m_aYLoadList.GetSafeHwnd(),YStyle);

	for(int i=0; i<2; i++)
	{
		lvcolumn.mask     = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvcolumn.fmt      = LVCFMT_CENTER;
		lvcolumn.pszText  = list[i];
		lvcolumn.iSubItem = i;
		lvcolumn.cx       = globalUtils.ScaleByDPI(width[i]);
		m_aYLoadList.InsertColumn(i,&lvcolumn);
	}
	Write_ListCtrl(m_aYLoadList, m_arYLoadMap);

}

void CDgnGenSTermLoadDlg::Write_ListCtrl(CListCtrl& LoadList, CMap<int, int, _DGN_STLD, _DGN_STLD&>& LoadMap)
{
	LoadList.DeleteAllItems();

	LV_ITEM lvitem;
	LPTSTR szText[2];

	_DGN_STLD Load; Load.Initialize();

	int iLoadCount = LoadMap.GetCount();
	int* pSortLoad = new int[iLoadCount];

	int Index = 0;
	int iCount = 0;
	POSITION Pos = LoadMap.GetStartPosition();
	while(Pos)
	{
		Load.Initialize();
		LoadMap.GetNextAssoc(Pos, Index, Load);
		pSortLoad[iCount] = Index;
		iCount++;
	}

	// Sorting by Ascending order.
	CQSort::QSortInt(pSortLoad, iLoadCount);

	for(iCount=0; iCount<iLoadCount; iCount++)
	{
		Load.Initialize();
		LoadMap.Lookup(pSortLoad[iCount], Load);

		CString strSeq = _T(""); strSeq.Format(_T("%d"), Load.iSeqNum);
	  szText[0] = (LPTSTR)(LPCTSTR)Load.strName;
	  szText[1] = (LPTSTR)(LPCTSTR)strSeq;
	  // Input ListBox.
	  for(int i=0; i<2; i++)
	  {
		  lvitem.mask		  = LVIF_TEXT;
		  lvitem.iItem  	= iCount;
		  lvitem.iSubItem = i;
		  lvitem.pszText	= szText[i];
		  if(i==0)	LoadList.InsertItem(&lvitem);
		  else			LoadList.SetItem(&lvitem);
	  }
	}

	delete[] pSortLoad;
}

void CDgnGenSTermLoadDlg::OnDgnAddLongLoadBtn()
{
	UpdateData(TRUE);

	int iSelectedCount = m_aLoadList.GetSelectedCount();
	int  nItem = -1;

	_DGN_STLD Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aLoadList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aLoadList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arLoadMap.Lookup(iSeqNum, Load);
			m_arLoadMap.RemoveKey(iSeqNum);
			m_arLongLoadMap.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aLoadList, m_arLoadMap);
		Write_ListCtrl(m_aLongLoadList, m_arLongLoadMap);
	}
	UpdateData(FALSE);
}

void CDgnGenSTermLoadDlg::OnDgnDelLongLoadBtn()
{
	UpdateData(TRUE);

	int iSelectedCount = m_aLongLoadList.GetSelectedCount();
	int  nItem = -1;

	_DGN_STLD Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aLongLoadList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aLongLoadList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arLongLoadMap.Lookup(iSeqNum, Load);
			m_arLongLoadMap.RemoveKey(iSeqNum);
			m_arLoadMap.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aLoadList, m_arLoadMap);
		Write_ListCtrl(m_aLongLoadList, m_arLongLoadMap);
	}
	UpdateData(FALSE);
}

void CDgnGenSTermLoadDlg::OnDgnAddXLoadBtn()
{
	UpdateData(TRUE);

	int iSelectedCount = m_aLoadList.GetSelectedCount();
	int  nItem = -1;

	_DGN_STLD Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aLoadList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aLoadList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arLoadMap.Lookup(iSeqNum, Load);
			m_arLoadMap.RemoveKey(iSeqNum);
			m_arXLoadMap.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aLoadList, m_arLoadMap);
		Write_ListCtrl(m_aXLoadList, m_arXLoadMap);
	}
	UpdateData(FALSE);
}

void CDgnGenSTermLoadDlg::OnDgnDelXLoadBtn()
{
	UpdateData(TRUE);

	int iSelectedCount = m_aXLoadList.GetSelectedCount();
	int  nItem = -1;

	_DGN_STLD Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aXLoadList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aXLoadList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arXLoadMap.Lookup(iSeqNum, Load);
			m_arXLoadMap.RemoveKey(iSeqNum);
			m_arLoadMap.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aLoadList, m_arLoadMap);
		Write_ListCtrl(m_aXLoadList, m_arXLoadMap);
	}
	UpdateData(FALSE);
}

void CDgnGenSTermLoadDlg::OnDgnAddYLoadBtn()
{
	UpdateData(TRUE);

	int iSelectedCount = m_aLoadList.GetSelectedCount();
	int  nItem = -1;

	_DGN_STLD Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aLoadList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aLoadList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arLoadMap.Lookup(iSeqNum, Load);
			m_arLoadMap.RemoveKey(iSeqNum);
			m_arYLoadMap.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aLoadList, m_arLoadMap);
		Write_ListCtrl(m_aYLoadList, m_arYLoadMap);
	}
	UpdateData(FALSE);
}

void CDgnGenSTermLoadDlg::OnDgnDelYLoadBtn()
{
	UpdateData(TRUE);

	int iSelectedCount = m_aYLoadList.GetSelectedCount();
	int  nItem = -1;

	_DGN_STLD Load; 
	// Update all of the selected items.
	if(iSelectedCount > 0)
	{
		for(int i=0; i<iSelectedCount; i++)
		{
			nItem = m_aYLoadList.GetNextItem(nItem, LVNI_SELECTED);
			ASSERT(nItem != -1);
			int iSeqNum = _ttoi(m_aYLoadList.GetItemText(nItem, 1));
			Load.Initialize();
			m_arYLoadMap.Lookup(iSeqNum, Load);
			m_arYLoadMap.RemoveKey(iSeqNum);
			m_arLoadMap.SetAt(iSeqNum, Load);
		}
		Write_ListCtrl(m_aLoadList, m_arLoadMap);
		Write_ListCtrl(m_aYLoadList, m_arYLoadMap);
	}
	UpdateData(FALSE);
}

CRect CDgnGenSTermLoadDlg::MoveCtrl(int iKind, int* pID, CRect Rect0)
{
	CRect RectOrg;
	int iArSize=0;
	if(iKind==0)			iArSize = iDgn_SLTermID;
	else if(iKind==1)	iArSize = iDgn_ShortTermID;
	else	ASSERT(0);

	CRect* pRectCng = new CRect[iArSize];

	CRect Rect1;
	for(int i=0; i<iArSize; i++)
	{
		GetDlgItem(pID[i])->ShowWindow(SW_SHOW);
		GetDlgItem(pID[i])->GetWindowRect(&(pRectCng[i]));
		if(i==0)	Rect1 = pRectCng[i];
		pRectCng[i].top		 -= Rect1.top - Rect0.top;
		pRectCng[i].bottom -= Rect1.top - Rect0.top;
		pRectCng[i].left   -= Rect1.left - Rect0.left;
		pRectCng[i].right  -= Rect1.left - Rect0.left;
	}
	RectOrg = pRectCng[0];
	for(int i=0; i<iArSize; i++)
	{
		ScreenToClient(pRectCng[i]);
		GetDlgItem(pID[i])->SetWindowPos(NULL, pRectCng[i].left, pRectCng[i].top, pRectCng[i].Width(), pRectCng[i].Height(), SWP_NOSIZE | SWP_NOZORDER);
	}
	delete[] pRectCng;
	return RectOrg;

}

void CDgnGenSTermLoadDlg::MoveCtrl()
{
	UpdateData(TRUE);
	int i=0;
	for(i=0; i<iDgn_ShortTermID; i++)	  GetDlgItem(m_iShortTermID[i])->ShowWindow(SW_HIDE);  
	
	CRect RectDlg, RectOk, RectCn, RectOrg;
	GetWindowRect(&RectDlg);
	GetDlgItem(IDOK    )->GetWindowRect(&RectOk);
	GetDlgItem(IDCANCEL)->GetWindowRect(&RectCn);

	CRect RectRef;
	GetDlgItem(m_iSLTermID[5])->GetWindowRect(&RectRef);
	
	CRect rOk;
	rOk.left		= RectOk.left;
	rOk.right		= RectOk.right;
	rOk.top			= RectRef.bottom + globalUtils.ScaleByDPI(16);
	rOk.bottom	= rOk.top + RectOk.Height();
	CRect rCn;
	rCn.left		= RectCn.left;
	rCn.right		= RectCn.right;
	rCn.top			= rOk.top;
	rCn.bottom	= rOk.bottom;

	CRect RectLoad;
	GetDlgItem(m_iSLTermID[1])->GetWindowRect(&RectLoad);
	CRect rLoad;
	rLoad.left		= RectLoad.left;
	rLoad.right		= RectLoad.right;
	rLoad.top			= RectLoad.top;
	rLoad.bottom  = RectRef.bottom;
				
	CRect rDlg;
	rDlg.left		= RectDlg.left;
	rDlg.top		= RectDlg.top;
	rDlg.right	=	RectDlg.right;
	rDlg.bottom	= rOk.bottom + globalUtils.ScaleByDPI(8);

	ScreenToClient(rDlg);
	ScreenToClient(rOk);
	ScreenToClient(rCn);
	ScreenToClient(rLoad);
	SetWindowPos(NULL, rDlg.left, rDlg.top, rDlg.Width(), rDlg.Height(), SWP_NOMOVE | SWP_NOZORDER);
	GetDlgItem(IDOK    )->SetWindowPos(NULL, rOk.left, rOk.top, rOk.Width(), rOk.Height(), SWP_NOSIZE);
	GetDlgItem(IDCANCEL)->SetWindowPos(NULL, rCn.left, rCn.top, rCn.Width(), rCn.Height(), SWP_NOSIZE);
	GetDlgItem(m_iSLTermID[1])->SetWindowPos(NULL, rLoad.left, rLoad.top, rLoad.Width(), rLoad.Height(), SWP_NOMOVE);

	UpdateData(FALSE);  

}

