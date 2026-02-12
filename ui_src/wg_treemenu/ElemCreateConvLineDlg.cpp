// ElemCreateConvLineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_treemenu.h"
#include "ElemCreateConvLineDlg.h"
#include "..\wg_main\wg_mainres2.h"

#include "..\wg_base\wg_base_StrParser.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_ViewCtrl.h"
#include "..\wg_db\wg_db_FlagCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElemCreateConvLineDlg dialog
#ifdef _MVIL
int CElemCreateConvLineDlg::m_nCurETypeIndex = 1;
#elif _TOWER
int CElemCreateConvLineDlg::m_nCurETypeIndex = 0;
#else
int CElemCreateConvLineDlg::m_nCurETypeIndex = 3;
#endif

CElemCreateConvLineDlg::CElemCreateConvLineDlg(CWnd* pParent /*=NULL*/)
	: CMenuBarChildDlg(CElemCreateConvLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElemCreateConvLineDlg)
	m_strBetaAngle = _T("0");
	m_SelectElemString = _T("");
	//}}AFX_DATA_INIT  
	m_bPropInit = FALSE;
}

CElemCreateConvLineDlg::~CElemCreateConvLineDlg()
{
	
}

void CElemCreateConvLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CMenuBarChildDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElemCreateConvLineDlg)
	DDX_Control(pDX, IDC_TM_TRUSS_RADIO, m_radioTruss);
	DDX_Control(pDX, IDC_TM_BEAM_RADIO, m_radioBeam);
	DDX_Control(pDX, IDC_TM_SEC_NO_EDIT     , m_ctrlEditSect);
	DDX_Control(pDX, IDC_TM_MAT_NO_EDIT     , m_ctrlEditMatl);
	DDX_Control(pDX, IDC_TM_SEC_NAME_COMBO  , m_ctrlCobxSect);
	DDX_Control(pDX, IDC_TM_MAT_NAME_COMBO  , m_ctrlCobxMatl);
	DDX_Control(pDX, IDC_TM_BANGLE_EDIT, m_wndBAngleEdit);
	//DDX_Text(pDX, IDC_TM_BANGLE_EDIT, m_strBetaAngle);
	//DDX_Text(pDX, IDC_TM_ELEM_EDIT, m_SelectElemString);
	DDX_Control(pDX, IDC_TM_ELEM_EDIT, m_editSelectElem);
// 	DDX_Control(pDX, IDC_TM_ORI_STATIC1, m_wndOriTitle);
// 	DDX_Control(pDX, IDC_TM_SEC_STATIC1, m_wndSecTitle);
// 	DDX_Control(pDX, IDC_TM_MAT_STATIC1, m_wndMatTitle);
// 	DDX_Control(pDX, IDC_TM_ELEM_STATIC1, m_wndElemTitle);
	DDX_Control(pDX, IDC_TM_PERIPHERY_CHECK, m_chkPeriphery);
	//}}AFX_DATA_MAP
}

#define CDialog CMenuBarChildDlg
BEGIN_MESSAGE_MAP(CElemCreateConvLineDlg, CDialog)
#undef CDialog
	//{{AFX_MSG_MAP(CElemCreateConvLineDlg)
	ON_BN_CLICKED(IDC_TM_MAT_BUTTON, OnTmMatButton)
	ON_BN_CLICKED(IDC_TM_SEC_BUTTON, OnTmSecButton)
	//}}AFX_MSG_MAP  
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElemCreateConvLineDlg message handlers

void CElemCreateConvLineDlg::Execute()
{
	CWaitCursor Cursor;
	_CreateElement();
}

BOOL CElemCreateConvLineDlg::OnInitDialog() 
{
	CMenuBarChildDlg::OnInitDialog();
	
	InitPropertyCtrls();
	InitElemType();

	m_wndBAngleEdit.AddString(_T("0"));
	m_wndBAngleEdit.AddString(_LS(IDS_WG_TREEMENU_90));
	m_wndBAngleEdit.AddString(_LS(IDS_WG_TREEMENU_180));

	m_editSelectElem.Connect(SC_ID_ELEM, &m_editSelectElem);	

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElemCreateConvLineDlg::_CreateElement()
{
	_CLElemCreate EC;

	if(GetParameter(EC)) 
	{
		T_ELEM_D EData;  
		EData.Initialize();
		
		EData.eltyp = EC.m_nElemType;
		EData.angle = EC.m_fBAngle;
		EData.elmat = EC.m_nMatID;
		if(EC.m_nSecID != 0)
			EData.elpro = EC.m_nSecID;
		if(EC.m_nThkID != 0)
			EData.elpro = EC.m_nThkID;

		EData.dNonlLength = EC.m_dNonlLength;
		EData.dTensionForce = EC.m_dTensionForce;
		EData.dLimit = EC.m_dLimit; // MNET:2774 20070507 mylee
		EData.bUseLimit = EC.m_bUseLimit;
		EData.dCableLengthRatio = EC.m_dLnLRatio;
		EData.nCableOpt = EC.m_nTensCableKind;
		EData.nSubType = EC.m_nSubType;
		EData.nWallId = EC.m_nWallID;

		CArray<ST_ED_NODE_K, ST_ED_NODE_K&> arStEdNodeK;    
		GetStEdNodeKList(arStEdNodeK);

		CArray<T_ELEM_K, T_ELEM_K> rKey;
		CArray<T_ELEM_D, T_ELEM_D&> rData;

		int nElemSize = arStEdNodeK.GetSize();
		for(int i=0 ; i<nElemSize ; ++i)
		{
			ST_ED_NODE_K StEdNodeK = arStEdNodeK[i];

			T_ELEM_K ElemK = 0;
			EData.elnod[0] = StEdNodeK.StNodeK;
			EData.elnod[1] = StEdNodeK.EdNodeK;
			rKey.Add(ElemK);
			rData.Add(EData);
		}
		
		CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();  
		
		if(pDoc->m_pDataCtrl->AddElem(rKey,rData,EC.m_bIntersectNode,EC.m_bIntersectElem,EC.m_nBAngleOrRefPoint,
																	EC.m_RPWx,EC.m_RPWy,EC.m_RPWz, EC.m_bCreateInnerPoint))
		{
			pDoc->m_pViewCtrl->UnSelectAllElem(NULL,FALSE);
		}    
	}
	else
	{
		GSaveHistoryNF(_LS(IDS_WG_TREEMENU_Error___Element_Creation_Arguement_));
	}
}

BOOL CElemCreateConvLineDlg::GetParameter(_CLElemCreate & EC)
{
	UpdateData(TRUE);
		
	EC.m_nElemType = GetCurElemTypeID();

	BOOL RetVal = TRUE;

	///////////////////////////////////////////////////////////////////////////
	// DB에서 Element Type과 부합되는 Data를 요구하므로 
	// Element Type과 연관된 Data 이외의 Data는 무효한 Data를 설정해야 한다. 
	// 현재 해당 질의 함수에서 이러한 처리를 수행하고 있다. 
	if(!GetNodeListAndNodeNum(EC.m_nElemType,EC.m_NodeKeyList,EC.m_nMaxNodeNum))
			 RetVal = FALSE;
	
	if(!GetKNodeDatas(EC.m_nElemType,EC.m_NodeKeyList,EC.m_nBAngleOrRefPoint,
								EC.m_fBAngle,EC.m_RPWx,EC.m_RPWy,EC.m_RPWz))
			 RetVal = FALSE;
	if(!GetPropIDS(EC.m_nElemType,EC.m_nMatID,EC.m_nSecID,EC.m_nThkID)) 
			 RetVal = FALSE; 
	
	EC.m_bIntersectNode = TRUE;
	EC.m_bIntersectElem = TRUE;
	
	return RetVal;
}

// 평면을 둘러싸고 있는 Node들을 Beam에 맞게 시작 끝 Node로 구분하여 가져옴
int CElemCreateConvLineDlg::GetStEdNodeKList(CArray<ST_ED_NODE_K, ST_ED_NODE_K&>& arStEdNodeK)
{
	arStEdNodeK.RemoveAll();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<T_ELEM_K,T_ELEM_K> m_arElemK;
	m_arElemK.RemoveAll();
	pDoc->m_pViewCtrl->GetAllSelectedElem(m_arElemK);

	T_ELEM_K ElemK;
	T_ELEM_D ElemD; 
	int nElemSize = m_arElemK.GetSize();
	for(int i=0 ; i<nElemSize ; ++i)
	{
		ElemK = m_arElemK[i];
		ElemD.Initialize();
		if(!pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			ASSERT(0);
			continue;
		}

		if(pDoc->m_pAttrCtrl->IsPlaneType(ElemD.eltyp))
		{
			//if(pDoc->m_pAttrCtrl->IsWall(ElemD.eltyp)) continue;
			int nNodeSize = pDoc->m_pAttrCtrl->GetNumNode(ElemD.eltyp, ElemD.elnod);
			for(int n=0 ; n<nNodeSize ; ++n)
			{
				double Tolerance = pDoc->m_pFlagCtrl->GetGeneralCoordinateToleranceCurrentUnit();
				T_NODE_D DataI, DataJ;
				CArray<T_NODE_K, T_NODE_K> arKeyList;
				T_NODE_K StNodeK = 0;
				T_NODE_K EdNodeK = 0;
				if(n == nNodeSize-1)
				{
					StNodeK = ElemD.elnod[n];
					EdNodeK = ElemD.elnod[0];          
				}
				else
				{
					StNodeK = ElemD.elnod[n];
					EdNodeK = ElemD.elnod[n+1];          
				}

				pDoc->m_pAttrCtrl->GetNode(StNodeK, DataI);
				pDoc->m_pAttrCtrl->GetNode(EdNodeK, DataJ);
				pDoc->m_pDataCtrl->GetNodeBy2Point(DataI.x, DataI.y, DataI.z, DataJ.x, DataJ.y, DataJ.z,Tolerance, arKeyList);
				int nCount=arKeyList.GetSize();
				for(int j=-1; j<nCount; j++)
				{
					ST_ED_NODE_K StEdNodeK; StEdNodeK.Initialize();

					StEdNodeK.StNodeK = (j == -1) ? StNodeK : arKeyList[j];
					StEdNodeK.EdNodeK = (j == nCount-1) ? EdNodeK : arKeyList[j+1];

					//추가하기전에 이미 해당 Node사이에 다른 Frame 요소가 있는지 확인한다.
					if(SameConnectElement(ElemK, StEdNodeK))
						continue;

					//중복되는 ST_ED_NODE_K가 있는지 검사
					if(OverLapStEdNodeK(arStEdNodeK, StEdNodeK)) 
						continue;

					arStEdNodeK.Add(StEdNodeK);          
				}        
			}
		}
	}

	return arStEdNodeK.GetSize();
}

BOOL CElemCreateConvLineDlg::OverLapStEdNodeK(CArray<ST_ED_NODE_K, ST_ED_NODE_K&>& arStEdNodeK, ST_ED_NODE_K NewStEdNodeK)
{
	BOOL bOverLap = FALSE;
	BOOL bSamePosi = FALSE;

	CArray<ST_ED_NODE_K, ST_ED_NODE_K&> arNewStEdNodeK;
	arNewStEdNodeK.RemoveAll(); 

	int nSize = arStEdNodeK.GetSize();
	for(int i=0 ; i<nSize ; ++i)
	{
		bSamePosi = FALSE;

		ST_ED_NODE_K StEdNodeK; StEdNodeK.Initialize();
		StEdNodeK = arStEdNodeK[i];
		if(StEdNodeK.StNodeK == NewStEdNodeK.StNodeK && StEdNodeK.EdNodeK == NewStEdNodeK.EdNodeK)
		{
			bOverLap = TRUE;
			bSamePosi = TRUE;
		}
		if(StEdNodeK.StNodeK == NewStEdNodeK.EdNodeK && StEdNodeK.EdNodeK == NewStEdNodeK.StNodeK)
		{
			bOverLap = TRUE;
			bSamePosi = TRUE;
		}

		if(!bSamePosi) //겹치지 않으면, 추후 판과판이 만나는 부분이 생기지 않는 옵션 적용시 사용
			arNewStEdNodeK.Add(StEdNodeK);
	} 

	if(m_chkPeriphery.GetCheck())
	{
		arStEdNodeK.RemoveAll();
		arStEdNodeK.Copy(arNewStEdNodeK);
	}

	return bOverLap;
}

BOOL CElemCreateConvLineDlg::SameConnectElement(T_ELEM_K ElemK, ST_ED_NODE_K StEdNodeK)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();

	CArray<T_ELEM_K, T_ELEM_K> arConnElemKey;
	pDoc->m_pDataCtrl->GetElemKeyListConnectedToElemKeyIncludeIntersectNode(ElemK, arConnElemKey);

	int nConnElemSize = arConnElemKey.GetSize();
	for(int i=0 ; i<nConnElemSize ; ++i)
	{
		T_ELEM_K ElemK = arConnElemKey[i];
		T_ELEM_D ElemD; ElemD.Initialize();
		if(pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD))
		{
			if(pDoc->m_pAttrCtrl->IsFrameType(ElemD.eltyp))
			{
				if(ElemD.elnod[0]==StEdNodeK.StNodeK && ElemD.elnod[1]==StEdNodeK.EdNodeK)
					return TRUE;
				if(ElemD.elnod[1]==StEdNodeK.StNodeK && ElemD.elnod[0]==StEdNodeK.EdNodeK)
					return TRUE;
			}
		}
	}

	return FALSE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemCreateConvLineDlg::GetNodeListAndNodeNum(int nElemType,
																					 CArray<unsigned int,unsigned int>& _NodeList,
																					 int& nMaxNodeNum)
{
	/*CString strNodes;
	m_wndNodeListEdit.GetWindowText(strNodes);

	GetNodeList(strNodes,_NodeList);*/

	//Truss, Beam만 있으므로 2개로고정
	nMaxNodeNum = 2; 
	
	return TRUE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemCreateConvLineDlg::GetPropIDS(int nElemType,int& nMatID,int& nSecID,int& nThkID)
{
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	CString strS,strM;
	m_ctrlEditSect.GetWindowText(strS);
	m_ctrlEditMatl.GetWindowText(strM);
	
	if (pDoc->m_pAttrCtrl->IsNeedMatl(nElemType))
		GetINumber(strM,nMatID);
	else nMatID = 0;

	if (pDoc->m_pAttrCtrl->IsNeedSect(nElemType))
		GetINumber(strS,nSecID);
	else nSecID = 0;

	nThkID = 0;
	

	return TRUE;
}

// Element Type에 따른 Data설정이 이루어 지고 있음...
BOOL CElemCreateConvLineDlg::GetKNodeDatas(int nElemType, CArray<unsigned int,unsigned int>& NodeKeyList,int &nBAngleOrRefP, 
																	 double &BAngle, double &RPWx, double &RPWy, double &RPWz)
{
	nBAngleOrRefP = 0;

	CString BAStr;
	m_wndBAngleEdit.GetWindowText(BAStr);  
	
	GetFloatNumber(BAStr,BAngle);
	RPWx = RPWy = RPWz = 0.0;    
	
	return TRUE;
}

void CElemCreateConvLineDlg::InitPropertyCtrls()
{
	m_ctrlCobxMatl.SetEditBoxPoint(&m_ctrlEditMatl);
	m_ctrlCobxSect.SetEditBoxPoint(&m_ctrlEditSect);
	
	m_ctrlEditMatl.SetComboBoxPoint(&m_ctrlCobxMatl);
	m_ctrlEditSect.SetComboBoxPoint(&m_ctrlCobxSect);
	
	if (!m_bPropInit)
	{
		m_ctrlEditMatl.SetEditBoxText(1);
		m_ctrlEditSect.SetEditBoxText(1);
		m_bPropInit = TRUE;
	}
}

void CElemCreateConvLineDlg::InitElemType()
{
	m_ElemTypeIDs.RemoveAll();
	
	CDBDoc * pDoc = (CDBDoc*)CTreeMenuBarBase::GetDocST();
	// 추가할 Element Type을 Element Type Array에 설정한다. 
	// Array에 Add한 순서대로 List에 추가된다. Item Data는 Element 
	// Type으로 설정된다. 
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetTrussID());			//truss	       
#ifndef _TOWER
	m_ElemTypeIDs.Add(pDoc->m_pAttrCtrl->GetBeamID());			//beam         	
#endif

	if(m_nCurETypeIndex == 0)
	{
		m_radioTruss.SetCheck(TRUE);
		m_radioBeam.SetCheck(FALSE);
	}
	else
	{
		m_radioTruss.SetCheck(FALSE);
		m_radioBeam.SetCheck(TRUE);
	}
}

int  CElemCreateConvLineDlg::GetCurElemTypeID()
{
	if(m_radioTruss.GetCheck())
		m_nCurETypeIndex = 0;
	else
		m_nCurETypeIndex = 1;

	return m_ElemTypeIDs[m_nCurETypeIndex];
}

void CElemCreateConvLineDlg::OnTmMatButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_MATERIAL,0));
}

void CElemCreateConvLineDlg::OnTmSecButton() 
{
	AfxGetMainWnd()->SendMessage(WM_COMMAND,	MAKEWPARAM(ID_STRUCT_PROP_SECTION,0));
	
}

///////////////////////////////////////////////////////////////////////////////
//
// DB Update

void CElemCreateConvLineDlg::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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
	default:
		//ASSERT(FALSE);
		break;
	}
}

void CElemCreateConvLineDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = CDBDoc::GetDocPoint()->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);
		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_ELEM_ADD): 
		case(UR_ELEM_DEL): 
			{
				
			}
			break;
		default:
			break;
		}
	} // end of while
}


