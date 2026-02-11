// CMStorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_cmd.h"

#include "CMSplcSubCalcEccnDlg.h"

#include "CMStorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_ViewBuff.h"
//#include "..\wg_db\wg_db_StoryData.h"     // for CStoryData
#include "..\wg_db\TempDrawObjectCtrl.h"   // for CTempDrawObjectCtrl
#include "..\wg_db\StorSubData.h"

#include "..\wg_main\wg_mainRes2.h"

#include "CMStorGrid.h"
#include "CMStorWindSeisGrid.h"
#include "CMStorGrid_IGen.h"
#include "CMStorAutoGenDlg.h"
#include "BldcDlg.h"
#include "CMStorAddDlg.h"

//#include "..\wg_cmd2\wg_cmd2_Dialog.h"    // for CCMModuleDlg

/////////////////////////////////////////////////////////////////////////////
// CCMStorDlg dialog


CCMStorDlg::CCMStorDlg(CWnd* pParent /*=NULL*/)
	: CCMDlgBase(CCMStorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMStorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCMStorDlg::~CCMStorDlg()
{
	CTBCommon* pGrid;
	int nCount = m_Tab.GetBeam().GetCount();
	for (int i = 0; i < nCount; i++)
	{
		pGrid = (CTBCommon*)(m_Tab.GetBeam().GetTab(i).pExtra);
		ASSERT(pGrid != 0);
		delete pGrid;
	}
}


void CCMStorDlg::DoDataExchange(CDataExchange* pDX)
{
	CCMDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMStorDlg)
	DDX_Control(pDX, IDC_ETC_STOR_BL_UNIT, m_wndBaseLevelUnit);
	DDX_Control(pDX, IDC_ETC_STOR_BL_EDIT, m_wndBaseLevelEdit);
	DDX_Control(pDX, IDC_ETC_PLACEHOLDER, m_Tab);
	DDX_Control(pDX, IDC_CHECK_FOR_LINE_GRID, m_chkLineGrid);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CCMStorDlg implementation

BEGIN_MESSAGE_MAP(CCMStorDlg, CCMDlgBase)
	//{{AFX_MSG_MAP(CCMStorDlg)
	ON_BN_CLICKED(IDC_ETC_STOR_AUTO_GEN, OnEtcStorAutoGen)
	ON_BN_CLICKED(IDC_ETC_DEFINE_MODULE_BTN, OnBnClickedDefineModule)
	ON_BN_CLICKED(IDC_ETC_ADD_STORY_BTN, OnBnClickedAddStory)
	ON_BN_CLICKED(IDC_ETC_CALC_ACC_ECCN_BTN, OnBnClickedCalcAccidentalEccn)
	ON_BN_CLICKED(IDCLOSE, OnClose)
	ON_BN_CLICKED(IDC_ETC_STOR_BL_BTN, OnEtcStorBlBtn)
	ON_BN_CLICKED(IDC_CHECK_FOR_LINE_GRID, OnBnClickedCheckForLineGrid)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMStorDlg message handlers

BOOL CCMStorDlg::OnInitDialog() 
{
	m_pDoc = CDBDoc::GetDocPoint();
	CCMDlgBase::OnInitDialog();

	// TODO: Add extra initialization here
	m_wndBaseLevelEdit.SetUnitType(CUnitCtrl::m_STOR_UNIT.dStoryLevel);
	m_wndBaseLevelUnit.SetUnitType(CUnitCtrl::m_STOR_UNIT.dStoryLevel);
	
	CString csValue;
	T_BLDC_D data;
	if (m_pDoc->m_pAttrCtrl->GetBldc(data))
	{
		if(data.bUseBaseLevel) csValue.Format(_T("%g"), data.dBaseLevel);
		else csValue = _T("0");
	}
	else csValue = _T("0");
	m_wndBaseLevelEdit.SetWindowText(csValue);

	//m_Tab.SubclassDlgItem(IDC_ETC_PLACEHOLDER, this);

	T_UNIT_SYSTEM UnitSystem;
	// story grid
	if(m_pDoc->EnableJUD())
	{
		m_pStory_IGen = new CCMStorGrid_IGen();
		m_pStory_IGen->Create(0, CRect(0, 0, 1, 1), &m_Tab, m_Tab.GetNextID());
		m_Tab.AttachWnd(m_pStory_IGen, _LS(IDS_WG_CMD__ADDD__Story));
		m_pStory_IGen->Initialize(m_pDoc);
	}
	else
	{
		m_pStory = new CCMStorGrid();
		//m_pStory->SetViewMode(D_STOR_VIEWMODE_STORY);
		m_pStory->Create(0, CRect(0, 0, 1, 1), &m_Tab, m_Tab.GetNextID());
		m_Tab.AttachWnd(m_pStory, _LS(IDS_WG_CMD__ADDD__Story));
		m_pStory->Initialize(m_pDoc);
	}  

	// wind grid
	m_pWind = new CCMStorWindSeisGrid();
	m_pWind->SetViewMode(D_STOR_VIEWMODE_WIND);
	m_pWind->Create(0, CRect(0, 0, 1, 1), &m_Tab, m_Tab.GetNextID());
	m_Tab.AttachWnd(m_pWind, _LS(IDS_WG_CMD__ADDD__Wind));
	m_pWind->Initialize(m_pDoc);

	// seismic grid
	m_pSeismic = new CCMStorWindSeisGrid();
	m_pSeismic->SetViewMode(D_STOR_VIEWMODE_SEISMIC);
	m_pSeismic->Create(0, CRect(0, 0, 1, 1), &m_Tab, m_Tab.GetNextID());
	m_Tab.AttachWnd(m_pSeismic, _LS(IDS_WG_CMD__ADDD__Seismic));
	m_pSeismic->Initialize(m_pDoc);

	auto& storSubData = CStorSubData::GetInstance();
	m_chkLineGrid.SetCheck(storSubData.IsCheckLineGridAll());

	// 2015-01-30. by Hsshim.
	// 일본 iGen인 경우 Renewal Gen에서는 다탑 지정 버튼 숨김.
	// 일본 iGen에서는 2차설계가 중요한데, 다탑을 고려한 2차설계가 아직 고려되어 있지 않고,
	// 기존의 다탑 기능은 후처리 Story Result Table 정리용인데, 일본에서는 거의 사용하고 있지 않아서 감추기로 일본법인과 합의
#if defined(_IGEN)
	GetDlgItem(IDC_ETC_DEFINE_MODULE_BTN)->ShowWindow(SW_HIDE);
#endif
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCMStorDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pDoc->EnableJUD()) this->m_pStory_IGen->OnTerminate(D_TB_SAVE_ALWAYS);
	else                    this->m_pStory->OnTerminate(D_TB_SAVE_ALWAYS);
	this->m_pWind->OnTerminate(D_TB_SAVE_ALWAYS);
	this->m_pSeismic->OnTerminate(D_TB_SAVE_ALWAYS);
	return CCMDlgBase::DestroyWindow();
}

void CCMStorDlg::OnBnClickedDefineModule()
{
	//CCMModuleDlg dlg;
	//dlg.DoModal();
	//AfxGetMainWnd()->PostMessage(WM_COMMAND, MAKEWPARAM(ID_QUERY_RESULT_STOR_MODULE,0), 0);
	//m_pDoc->OnCmdMsg()>PostMessage(WM_COMMAND, MAKEWPARAM(ID_QUERY_RESULT_STOR_MODULE,0), 0);
	UINT WM_QUERY_RESULT_STOR_MODULE = RegisterWindowMessage(_T("WM_QUERY_RESULT_STOR_MODULE"));
	AfxGetMainWnd()->SendMessage(WM_QUERY_RESULT_STOR_MODULE, 0, 0);
}

void CCMStorDlg::OnBnClickedCalcAccidentalEccn()
{
	CCMSplcSubCalcEccnDlg dlg;
	dlg.SetDlgType(EN_STATIC);
	if (dlg.DoModal() == IDOK)
	{
		CArray<T_STOR_D, T_STOR_D&> aStory;

		double dPercent4PlanDimEccn = 0.0;
		double dMultiple4TopStoryEccn = 0.0;
		bool bConsiderEccnbyAnal = false;
		double dRatio4AnalEccn = 0.0;

		UINT enDgnCode = dlg.GetDgnCodeSellected();
		dlg.GetInputData(dPercent4PlanDimEccn, dMultiple4TopStoryEccn, bConsiderEccnbyAnal, dRatio4AnalEccn);

		CArray<T_SEIS_WIDTH, T_SEIS_WIDTH&> aSeisWidth;
		CCMStorAutoGenDlg dlg;
		dlg.GetSeisWidth(aSeisWidth);
		m_pDoc->m_pAttrCtrl->GetStorData(aStory);

		switch (enDgnCode)
		{
			case EN_ECCN_DGN_CODE::EN_FIXED_ECCENTRICITY:
				{
					dMultiple4TopStoryEccn = 1.0;
					bConsiderEccnbyAnal = false;
					dRatio4AnalEccn = 0.0;

					m_pDoc->m_pAttrCtrl->CalcStorEccn4UserMethod(aStory, aSeisWidth,
						dPercent4PlanDimEccn, dMultiple4TopStoryEccn, bConsiderEccnbyAnal, dRatio4AnalEccn);
				}
				break;
			case EN_ECCN_DGN_CODE::EN_DGN_CODE_NTCS_2023:
				{
					m_pDoc->m_pAttrCtrl->CalcStorEccn4UserMethod(aStory, aSeisWidth,
						dPercent4PlanDimEccn, dMultiple4TopStoryEccn, bConsiderEccnbyAnal, dRatio4AnalEccn);
				}
				break;
			default:
				{
					ASSERT(0); return;
				}
		}

		if (aStory.GetSize() > 0)
		{
			if (!m_pDoc->m_pDataCtrl->AddStor(aStory, TRUE))  return;
		}
	}

	return;
}

void CCMStorDlg::OnBnClickedAddStory()
{
	CCMStorAddDlg dlg;
	dlg.DoModal();
	
	auto& storSubData = CStorSubData::GetInstance();
	if (storSubData.IsNeedUpdateStor())
	{
		bool isChecked = storSubData.IsCheckLineGridAll();
		m_pStory->SetLineGridAll(isChecked);
		storSubData.SetNeedUpdateStor(false);
	}
}
void CCMStorDlg::OnEtcStorAutoGen() 
{  
	CCMStorAutoGenDlg dlg;
	dlg.DoModal();

	auto& storSubData = CStorSubData::GetInstance();
	if (storSubData.IsNeedUpdateStor())
	{
		bool isChecked = storSubData.IsCheckLineGridAll();
		m_pStory->SetLineGridAll(isChecked);
		storSubData.SetNeedUpdateStor(false);
	}
}

void CCMStorDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}

void CCMStorDlg::OnUpdate(CWnd* pSender, LPARAM lHint, CObject* pHint)
{
	switch(lHint)
	{
	case D_UPDATE_DBALL:
	case D_UPDATE_DEFAULT:
		UpdateBuffer();
		if(m_pDoc->EnableJUD()) m_pStory_IGen->UpdateBuffer(m_pDoc);
		else                    m_pStory->UpdateBuffer(m_pDoc);
		m_pWind->UpdateBuffer(m_pDoc);
		m_pSeismic->UpdateBuffer(m_pDoc);
		break;
	case D_UPDATE_BUFFER_BEFORE:
		break;
	case D_UPDATE_BUFFER_AFTER:
		UpdateBuffer();
		if(m_pDoc->EnableJUD()) m_pStory_IGen->UpdateBuffer(m_pDoc);
		else                    m_pStory->UpdateBuffer(m_pDoc);
		m_pWind->UpdateBuffer(m_pDoc);
		m_pSeismic->UpdateBuffer(m_pDoc);
		break;
	case D_UPDATE_UNIT:
		if(m_pDoc->EnableJUD()) m_pStory_IGen->UnitChanged(m_pDoc);
		else                    m_pStory->UnitChanged(m_pDoc);
		m_pWind->UnitChanged(m_pDoc);
		m_pSeismic->UnitChanged(m_pDoc);
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

void CCMStorDlg::UpdateBuffer()
{
	CViewBuff* pViewBuff = m_pDoc->m_pViewBuff;
	int nCount = pViewBuff->GetCount();
	if (nCount == 0) return; 
	ASSERT(nCount == 1);

	T_UDRD_BUFFER buffer_ur;
	POSITION pos = pViewBuff->GetStartBuffer();

	T_BLDC_D    Data;
	CString     csValue;

	while(pos != NULL)
	{
		buffer_ur = pViewBuff->GetNextBuffer(pos);

		int nCmd = buffer_ur.nCmd;
		int nKey = buffer_ur.nKey;
		
		switch(nCmd)
		{
		case(UR_BLDC_ADD):
				// 삽입될 위치(nRow)를 찾아 넣는다.
				pViewBuff->GetBldc(nKey, Data);
				if(Data.bUseBaseLevel) csValue.Format(_T("%g"), Data.dBaseLevel);
				else csValue = _T("0");
				m_wndBaseLevelEdit.SetWindowText(csValue);
				break;
		case(UR_BLDC_DEL):
				m_wndBaseLevelEdit.SetWindowText(_T("0"));
				break;
		default:
			break;
		}
	} // end of while
}

void CCMStorDlg::OnEtcStorBlBtn() 
{
	// TODO: Add your control notification handler code here
	CBldcDlg dlg;
	dlg.DoModal();

//	AfxGetMainWnd()->SendMessage(WM_COMMAND, MAKEWPARAM(ID_STRUCT_BDC, 0));	
}

void CCMStorDlg::OnBnClickedCheckForLineGrid()
{
	if (m_chkLineGrid.GetCheck() == BST_CHECKED)
	{
		m_pStory->SetLineGridAll(true);
		CStorSubData::GetInstance().SetCheckLineGridAll(true);
	}
	else
	{
		m_pStory->SetLineGridAll(false);
		CStorSubData::GetInstance().SetCheckLineGridAll(false);
	}
}
