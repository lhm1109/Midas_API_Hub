// CMTendonExportDXFDlg.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_cmd.h"
#include "CMTendonExportDXFDlg.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_db\wg_db_FileCtrl.h"
#include "..\wg_db\wg_db_PostCtrl.h"
#include "..\wg_db\wg_db_UnitCtrl.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db.h"
#include "..\wg_db\MathFunc.h"


#include "CMThfcRpsrProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define   DIR_XZ_PLANE  0
#define   DIR_XY_PLANE  1

/////////////////////////////////////////////////////////////////////////////
// CCMTendonExportDXFDlg dialog

CCMTendonExportDXFDlg::CCMTendonExportDXFDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CCMTendonExportDXFDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCMTendonExportDXFDlg)
	//}}AFX_DATA_INIT

	m_aSelectedProfiles.RemoveAll();

	m_pDoc = CDBDoc::GetDocPoint();
	ASSERT(m_pDoc);

	m_pTendonInfo = NULL;
}

CCMTendonExportDXFDlg::~CCMTendonExportDXFDlg() 
{
}

void CCMTendonExportDXFDlg::SetSelectedProfiles(CArray<UINT, UINT> &aSelectedK)
{
	m_aSelectedProfiles.Copy(aSelectedK);
}

void CCMTendonExportDXFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCMTendonExportDXFDlg)
	DDX_Control(pDX, IDC_CMD_SELECTED_LIST        , m_listSelProfiles);
	DDX_Control(pDX, IDC_CMD_ALL_LIST             , m_listAllProfiles);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCMTendonExportDXFDlg, CDialogMove)
	//{{AFX_MSG_MAP(CCMTendonExportDXFDlg)
	ON_BN_CLICKED(IDC_CMD_SEL_BUTTON    , OnCmdSelButton)
	ON_BN_CLICKED(IDC_CMD_UNSEL_BUTTON  , OnCmdUnselButton)
	ON_BN_CLICKED(IDC_CMD_ALLSEL_BUTTON , OnCmdAllSelButton)
	ON_BN_CLICKED(IDC_CMD_NONESEL_BUTTON, OnCmdNoneSelButton)
	ON_LBN_DBLCLK(IDC_CMD_ALL_LIST      , OnDblclkCmdAllList)
	ON_LBN_DBLCLK(IDC_CMD_SELECTED_LIST , OnDblclkCmdSelectedList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCMTendonExportDXFDlg message handlers

BOOL CCMTendonExportDXFDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	InitAllList();
	InitSelList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCMTendonExportDXFDlg::OnOK() 
{
	if(m_aSelectedProfiles.GetSize() <= 0)
	{
		AfxMessageBox(_LS(IDS_CMD_TENDON_CHOOSE_TENDON_PROFILE));
		return;
	}

	// 선택한 Tendon Profile이 Tapered Section Group에 속해 있는지 알아보고
	T_TDNA_K  TdnaK=0;
	T_TSGR_K  TsgrK=0;
	T_TDNA_D  TdnaD; TdnaD.Initialize();
	T_ELEM_D  ElemD; ElemD.Initialize();
	T_SECT_D  SectD; SectD.Initialize();
	CArray<T_TSGR_K, T_TSGR_K>  aTsgrK; aTsgrK.RemoveAll();
	CMapEx<T_TSGR_K, T_TSGR_K, int, int>  mTsgrK; mTsgrK.InitHashTable(1001);
	
	for(int i=0; i<m_aSelectedProfiles.GetSize(); i++)
	{
		if(!m_pDoc->m_pAttrCtrl->GetTdna(m_aSelectedProfiles[i], TdnaD)) continue;
		for(int k=0; k<TdnaD.aElemList.GetSize(); k++)
		{
			if(!m_pDoc->m_pAttrCtrl->GetElem(TdnaD.aElemList[k], ElemD)) continue;
			if(!m_pDoc->m_pAttrCtrl->GetSect(ElemD.elpro, SectD)) continue;
			if(SectD.nStype != D_SECT_TYPE_TAPERED) continue;

			TsgrK = m_pDoc->m_pAttrCtrl->GetElemTsgrKey(TdnaD.aElemList[k]);
			if(TsgrK > 0)
			{
				int nTemp=0;
				if(mTsgrK.Lookup(TsgrK, nTemp) && nTemp==1)
					continue;
				mTsgrK.SetAt(TsgrK, 1);
				aTsgrK.Add(TsgrK);
			}
		}
	}
	// 속해 있다면 Convert 할건지 안할 건지 메세지창을 띄우고
	if(aTsgrK.GetSize() > 0)
	{
		int nRet=0;
		nRet = AfxMessageBox(_LS(IDS_CMD_TENDON_NEED_TO_CONVERT_TAPERED_SECTION_GROUP));
		return;
//     if(nRet == IDNO)
//       return;
//     if(!m_pDoc->m_pDataCtrl->ConvertTsgr(aTsgrK, 1)) { ASSERT(0); }
	}

	CString strSuccessMsg=_T("");
	if(ExportToDXFFile(strSuccessMsg))
	{
	  CDialogMove::OnOK();
		AfxMessageBox(strSuccessMsg);
	}
}

void CCMTendonExportDXFDlg::OnCancel() 
{
	CDialogMove::OnCancel();
}

void CCMTendonExportDXFDlg::OnCmdSelButton() 
{
	DoSelectItem();
}

void CCMTendonExportDXFDlg::OnCmdUnselButton() 
{
	DoUnselectItem();
}

void CCMTendonExportDXFDlg::OnCmdAllSelButton() 
{
	DoAllSelectItem();
}

void CCMTendonExportDXFDlg::OnCmdNoneSelButton() 
{
	DoNoneSelectItem();
}

void CCMTendonExportDXFDlg::OnDblclkCmdAllList() 
{
	DoSelectItem();
}

void CCMTendonExportDXFDlg::OnDblclkCmdSelectedList() 
{
	DoUnselectItem();
}

void CCMTendonExportDXFDlg::PostNcDestroy() 
{
	if(!( m_pDoc->m_pPostCtrl->IsPostEnable() && m_pDoc->m_pPostCtrl->IsAnalysisResultEnable() ))
	{
		if(m_pTendonInfo != NULL)
		{
			delete m_pTendonInfo;
			m_pTendonInfo = NULL;
		}
	}

	CDialogMove::PostNcDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// User Defined Function

void CCMTendonExportDXFDlg::InitAllList()
{
	m_listAllProfiles.ResetContent();

	int ix=0;
	T_TDNA_D data;
	data.Initialize();

	CArray<T_TDNA_K, T_TDNA_K> aKey;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(aKey);
	int nNum = aKey.GetSize();

	for(int i=0; i<nNum; i++)
	{
		if(!IsSelectedProfile(aKey[i]))
		{
			m_pDoc->m_pAttrCtrl->GetTdna(aKey[i], data);
			ix = m_listAllProfiles.AddString(data.TendonName);
			m_listAllProfiles.SetItemData(ix, aKey[i]);
		}
	}
	if(m_listAllProfiles.GetCount() > 0)
		m_listAllProfiles.SetCurSel(0);
}

BOOL CCMTendonExportDXFDlg::IsSelectedProfile(UINT TdnaK)
{
	BOOL bResult=FALSE;
	CMapEx<UINT,UINT,int,int> mSelectedProfiles;
	for(int i=0; i<m_aSelectedProfiles.GetSize(); i++)
	{
		mSelectedProfiles.SetAt(m_aSelectedProfiles[i], 1);
	}

	int nData=0;
	if(mSelectedProfiles.Lookup(TdnaK, nData) && nData==1)
		bResult=TRUE;

	return bResult;
}

void CCMTendonExportDXFDlg::InitSelList()
{
	m_listSelProfiles.ResetContent();

	int ix;
	T_TDNA_K key;
	T_TDNA_D data;

	int nNum = m_aSelectedProfiles.GetSize();
	for (int i=0; i<nNum; i++)
	{
		key = m_aSelectedProfiles.GetAt(i);
		m_pDoc->m_pAttrCtrl->GetTdna(key, data);

		ix = m_listSelProfiles.AddString(data.TendonName);
		m_listSelProfiles.SetItemData(ix, key);
	}
	if (nNum > 0) m_listSelProfiles.SetCurSel(0);
}

void CCMTendonExportDXFDlg::DoSelectItem()
{
	int nCount = m_listAllProfiles.GetSelCount();
	if (nCount <= 0) return;

	T_TDNA_K key;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listAllProfiles.GetSelItems(nCount, aSelectedItem.GetData()); 

	for (int i=0; i<nCount; i++)
	{
		key = m_listAllProfiles.GetItemData(aSelectedItem[i]);
		m_aSelectedProfiles.Add(key);
	}

	InitAllList();
	InitSelList();
}

void CCMTendonExportDXFDlg::DoUnselectItem()
{
	int nCount = m_listSelProfiles.GetSelCount();
	if (nCount <= 0) return;

	T_TDNA_K key;
	CArray<int, int> aSelectedItem;
	aSelectedItem.SetSize(nCount);
	m_listSelProfiles.GetSelItems(nCount, aSelectedItem.GetData()); 

	for (int i=m_aSelectedProfiles.GetSize()-1; i>=0; i--)
	{
		int k = 0;
		for (k=0; k<nCount; k++)
		{
			key = m_listSelProfiles.GetItemData(aSelectedItem[k]);
			if (key == m_aSelectedProfiles[i]) break;
		}
		if (k != nCount) m_aSelectedProfiles.RemoveAt(i);
	}

	InitAllList();
	InitSelList();
}

void CCMTendonExportDXFDlg::DoAllSelectItem()
{
	m_aSelectedProfiles.RemoveAll();

	CArray<T_TDNA_K, T_TDNA_K> aKey;
	m_pDoc->m_pAttrCtrl->GetTdnaKeyList(aKey);
	if(aKey.GetSize()<=0) return;

	m_aSelectedProfiles.Copy(aKey);
	
	InitAllList();
	InitSelList();
}

void CCMTendonExportDXFDlg::DoNoneSelectItem()
{
	m_aSelectedProfiles.RemoveAll();
	
	InitAllList();
	InitSelList();
}

BOOL CCMTendonExportDXFDlg::ExportToDXFFile(CString& strSuccessMsg)
{
	CString csFileName = CDBLib::MakeFileNameWithNewExt(_T(".dxf"));
	LPCTSTR szFileName;
	if (csFileName.IsEmpty()) szFileName = NULL;
	else szFileName = csFileName;
	
	CFileDialog fd(FALSE, _T(".dxf"), szFileName,
									OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_LONGNAMES, 
									_T("AutoCAD DXF File(*.dxf)|*.dxf|All File(*.*)|*.*||"), NULL);
	if(fd.DoModal() == IDOK)
	{
		if(WriteTendonToDXF(fd.GetPathName()))
		{
			strSuccessMsg.Format(_LS(IDS_CMD_TENDON_MAKE_FILE), fd.GetFileName());
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CCMTendonExportDXFDlg::WriteTendonToDXF(CString strFileName)
{
	TRY
	{
		CFileException FileException;
		if (!m_cfile.Open(strFileName, CFile::modeCreate | CFile::modeWrite, &FileException))
			AfxThrowFileException(FileException.m_cause, FileException.m_lOsError, FileException.m_strFileName);
		
		dxfSection(_T("HEADER"));
		dxfVersion();
		dxfEndSection();
		
		dxfSection(_T("TABLES"));
		dxfTable();
		WriteLayerInfo();
		dxfEndTable();
		dxfEndSection();
		
		dxfSection(_T("ENTITIES"));
		WriteEntities();
		dxfEndSection();
		
		dxfEOF();
		
		m_cfile.Close();
	}
	CATCH(CFileException, e)
	{
		e->ReportError();
		m_cfile.Abort();
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

void CCMTendonExportDXFDlg::WriteLayerInfo()
{
	dxfTableName(_T("LAYER"));
	dxfTableEntry(1);
	dxfLayer(_T("0"), 0, 7, _ULS(CONTINUOUS));

	T_TDNA_K key;
	T_TDNA_D data;
	int nColorIndex = 1;
	for(int i=0; i<m_aSelectedProfiles.GetSize(); i++)
	{
		if(nColorIndex==10) nColorIndex=1;
		key = m_aSelectedProfiles[i];
		if(!m_pDoc->m_pAttrCtrl->GetTdna(key, data)) { ASSERT(0); continue; }
		CString strTendonName = data.TendonName;
		ReplaceSpecialString(strTendonName);
		dxfLayer(strTendonName, 0, nColorIndex++, _ULS(CONTINUOUS));
	}
}

void CCMTendonExportDXFDlg::WriteEntities()
{
	WriteTendonProfile();
}

void CCMTendonExportDXFDlg::WriteTendonProfile()
{
	CWaitCursor Cursor;

//   time_t time1, time2, time3;
//   struct tm *timeR;
// 
//   time(&time1);

	CString strProgress = _T("");
	CDBProgressDlg* ProgressDlg = new CDBProgressDlg();
	ProgressDlg->Create(IDD_DB_PROGRESS_DLG, NULL);  
	ProgressDlg->ShowWindow(SW_SHOW);
	ProgressDlg->SetPos(0);
	ProgressDlg->UpdateWindow(); 
	ProgressDlg->SetRange(0, 100);
	int nPos = 0;

	if(m_pTendonInfo==NULL)
	{
		if(m_pDoc->m_pPostCtrl->IsPostEnable() && m_pDoc->m_pPostCtrl->IsAnalysisResultEnable())
		{
			m_pTendonInfo = m_pDoc->m_pPostCtrl->GetTendonInfo();
			nPos = 50;
			ProgressDlg->SetPos(nPos);
		}
		else
		{
			m_pTendonInfo = new CTendonInfo;
			m_pTendonInfo->Initialize();
			T_POSTCTRL_DATACOUNT DataCount; DataCount.Initialize();
			m_pTendonInfo->SetTendonView(TRUE);
			if(!m_pTendonInfo->MakeTendonProfileInfo(&DataCount, ProgressDlg))
			{ 
				ASSERT(0); 
				ProgressReturn(ProgressDlg, FALSE);
				return; 
			}
		}
	}

	CArray<CArray<T_NODE_D, T_NODE_D&>*, CArray<T_NODE_D, T_NODE_D&>*> aXZNodes; aXZNodes.RemoveAll();
	CArray<CArray<T_NODE_D, T_NODE_D&>*, CArray<T_NODE_D, T_NODE_D&>*> aXYNodes; aXYNodes.RemoveAll();
	aXZNodes.SetSize(m_aSelectedProfiles.GetSize());
	aXYNodes.SetSize(m_aSelectedProfiles.GetSize());
	CArray<T_NODE_D, T_NODE_D&>* paNodesTemp;

	double dXZMinX = DBL_MAX, dXZMaxX = -DBL_MAX, dXZMinY = DBL_MAX, dXZMaxY = -DBL_MAX;
	double dXYMinX = DBL_MAX, dXYMaxX = -DBL_MAX, dXYMinY = DBL_MAX, dXYMaxY = -DBL_MAX;

	T_TENDON_INFO TendonInfo;
	T_NODE_D NodeD;//, ConvPos1, ConvPos2;
	CMapEx<T_NODE_K, T_NODE_K, T_NODE_D, T_NODE_D&> mNodesCoord;
	double a5Pos[] = {0.00, 0.25, 0.50, 0.75, 1.00};

	// xz plane의 min값과 xy plane의 max값을 구한다.
	// xz plane의 tendon 형상과 xy plane의 tendon 형상이 겹치지 않기 위해서 xy plane의 tendon 형상을 일정거리만큼 띄워 놓는다.
	// Node 좌표를 구한다.
	//const double dZero = 1.E-07;
	//double dPreX, dPreY, dPreZ, dCurX, dCurY, dCurZ;
	for(int nDir=0; nDir<2; nDir++)
	{
		for(int i=0; i<m_aSelectedProfiles.GetSize(); i++)
		{
			TendonInfo.Initialize();
			if(!m_pTendonInfo->GetTendonInfo(m_aSelectedProfiles[i], TendonInfo)) { ASSERT(0); continue; }

			paNodesTemp = new CArray<T_NODE_D, T_NODE_D&>;
			//paNodesTemp->SetSize(TendonInfo.aTendonInfoElem.GetSize()*5);

			// Set start position
			//ConvPos1.Initialize();
			//ConvPos2.Initialize();
			for(int k=0; k<TendonInfo.aTendonInfoElem.GetSize(); k++)
			{/*
				T_ELEM_D ElemD;
				if(!m_pDoc->m_pAttrCtrl->GetElem(TendonInfo.aTendonInfoElem[k].KeyElem, ElemD)) { ASSERT(0); continue; }
				
				double dLocalVector[3][3]={0.,};
				if(!m_pDoc->calcElemLocalVector(ElemD, dLocalVector)) { ASSERT(0); continue; }
				
				double dElemLength = m_pDoc->calcLAVElem(ElemD);
				if(dElemLength<dZero) { ASSERT(0); continue; }
				
				T_NODE_D Node1, Node2;
				if(!m_pDoc->m_pAttrCtrl->GetNode((T_NODE_K)ElemD.elnod[0], Node1)) { ASSERT(0); continue; }
				if(!m_pDoc->m_pAttrCtrl->GetNode((T_NODE_K)ElemD.elnod[1], Node2)) { ASSERT(0); continue; }            
				Node1.x = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, Node1.x);
				Node1.y = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, Node1.y);
				Node1.z = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, Node1.z);
				Node2.x = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, Node2.x);
				Node2.y = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, Node2.y);
				Node2.z = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, Node2.z);*/
				
				for(int p=0; p<5; p++)
				{
					mNodesCoord.RemoveAll(); mNodesCoord.InitHashTable(HASHSIZETDNA);
				
					// 텐던 프로파일 내 요소들간의 텐던 정보 계산시 곡률로 인해 앞쪽 텐던과 뒤쪽 텐던의 불연속점이 발생
					// 마지막 포인트는 다음 절점의 첫번째로 한다.
					if((k<TendonInfo.aTendonInfoElem.GetSize()-1) && p==4)
					{
						int p2=0;
						if(TendonInfo.aTendonInfoElem[k].bReverseIJ) p2 = 4;
						if(!m_pTendonInfo->GetElemTendonGCSCoord(TendonInfo.aTendonInfoElem[k+1].KeyElem, a5Pos[p2], &mNodesCoord, FALSE,
							TRUE, m_aSelectedProfiles[i], FALSE/*nDir==DIR_XZ_PLANE*/, TRUE)) 
						{ continue; }
					}
					else
					{
						int p2=p;
						if(TendonInfo.aTendonInfoElem[k].bReverseIJ) p2 = 4-p;
						if(!m_pTendonInfo->GetElemTendonGCSCoord(TendonInfo.aTendonInfoElem[k].KeyElem, a5Pos[p2], &mNodesCoord, FALSE,
							TRUE, m_aSelectedProfiles[i], FALSE/*nDir==DIR_XZ_PLANE*/, TRUE)) 
						{ continue; }
					}
					
					NodeD.Initialize();
					if(!mNodesCoord.Lookup(m_aSelectedProfiles[i], NodeD)) { ASSERT(0); continue; }           
					NodeD.x = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, NodeD.x);
					NodeD.y = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, NodeD.y);
					NodeD.z = m_pDoc->m_pUnitCtrl->ConvertCurUnitData2TgtUnit(D_UNITSYS_FORCE_INDEX_N, D_UNITSYS_LENGTH_INDEX_M, D_UNITSYS_BASE_LENGTH, NodeD.z);

					if(nDir==DIR_XZ_PLANE)
					{/*
						double dPos_x = a5Pos[p2] * dElemLength;
						
						// Set converted(X Axis) position
						ConvPos1 = ConvPos2;
						ConvPos2.x += dPos_x;

						// Get transfer coordinate, (Node2-Node1)*a5Pos[p2] to ConvPos1


						CMathFunc::mathLength(dCurX,dCurY,dCurZ, dPreX,dPreY,dPreZ);

						ConvNode1 = Node2;*/

						dXZMinX = min(dXZMinX, NodeD.x);
						dXZMaxX = max(dXZMaxX, NodeD.x);
						dXZMinY = min(dXZMinY, NodeD.z);
						dXZMaxY = max(dXZMaxY, NodeD.z);
					}
					else if(nDir==DIR_XY_PLANE)
					{
						dXYMinX = min(dXYMinX, NodeD.x);
						dXYMaxX = max(dXYMaxX, NodeD.x);
						dXYMinY = min(dXYMinY, NodeD.y);
						dXYMaxY = max(dXYMaxY, NodeD.y);
					}

					paNodesTemp->Add(NodeD);
				}
			} // end of TendonInfo.aTendonInfoElem

			paNodesTemp->FreeExtra();
					 if(nDir==DIR_XZ_PLANE) aXZNodes[i] = paNodesTemp;
			else if(nDir==DIR_XY_PLANE) aXYNodes[i] = paNodesTemp;
			else ASSERT(0);
		} // end of m_aSelectedProfiles

		nPos = 50+12*(nDir+1);
		ProgressDlg->SetPos(nPos);
	} // end of nDir

//   time(&time2);
//   time3 = time2-time1;
//   timeR = localtime(&time3);
// 
//   CString str="";
//   str.Format("%d분 %d초", timeR->tm_min, timeR->tm_sec);
//   GSaveHistoryFormatNF(str);
// 
//   time(&time1);

	double dXZDiffX = dXZMaxX-dXZMinX;
	double dXYDiffX = dXYMaxX-dXYMinX;
	double dMinMaxDiffX = max(dXZDiffX, dXYDiffX);

	double dXZDiffY = dXZMaxY-dXZMinY;
	double dXYDiffY = dXYMaxY-dXYMinY;
	double dMinMaxDiffY = max(dXZDiffY, dXYDiffY);

	double dPlaneMarginY = 4.0*dMinMaxDiffY;
	double dXYConvertY = dXZMinY-dXYMaxY-dPlaneMarginY;

	double aLableCoord[3] = {DBL_MAX, DBL_MAX, DBL_MAX};
	T_TDNA_K key;
	T_TDNA_D data;
	double dMinX =  DBL_MAX, dMinY =  DBL_MAX;
	double dMaxX = -DBL_MAX, dMaxY = -DBL_MAX;
	double dMarginX = 2.0, dMarginY = 0.5;
	CString strTendonName=_T("");

	for(int nDir=0; nDir<2; nDir++)
	{
		aLableCoord[0] = aLableCoord[1] = aLableCoord[2] = DBL_MAX;

		for(int i=0; i<m_aSelectedProfiles.GetSize(); i++)
		{
			int nNodeSize=0;
					 if(nDir==DIR_XZ_PLANE) nNodeSize = aXZNodes[i]->GetSize();
			else if(nDir==DIR_XY_PLANE) nNodeSize = aXYNodes[i]->GetSize();
			else ASSERT(0);

			if(nNodeSize <= 0) continue;

			dMinX =  DBL_MAX; dMinY =  DBL_MAX;
			dMaxX = -DBL_MAX; dMaxY = -DBL_MAX;
			
			key = m_aSelectedProfiles[i];
			m_pDoc->m_pAttrCtrl->GetTdna(key, data);

			dxfEntities(_ULS(POLYLINE));
			strTendonName = data.TendonName;
			ReplaceSpecialString(strTendonName);
			dxfLayerName(strTendonName);
			Write(_T("%3d\n%3d\n"), 66, 1);
			dxfCoord(1, 1, 0.0);
			dxfCoord(1, 2, 0.0);
			dxfCoord(1, 3, 0.0);
			Write(_T("%3d\n%3d\n"), 71, nNodeSize);
			
			//double dLength;
			//double dPreX, dPreY, dPreZ, dCurX, dCurY, dCurZ;
			//double dPrevInxX;
			for(int k=0; k<nNodeSize; k++)
			{        
				double dx1=0.0, dy1=0.0, dz1=0.0;
				if(nDir==DIR_XZ_PLANE)
				{
					dx1 = aXZNodes[i]->GetAt(k).x; 
					dy1 = aXZNodes[i]->GetAt(k).z;
					dz1 = 0.0;
					/*
					dCurX = aXZNodes[i]->GetAt(k).x;
					dCurY = aXZNodes[i]->GetAt(k).y;
					dCurZ = aXZNodes[i]->GetAt(k).z;

					if( k == 0 )
					{
						dPreX = dx1;
						dPreY = dy1;
						dPreZ = dz1;
					}
					else
					{
						double dZero = 1.E-07;
						T_ELEM_D ElemD;
						if(!m_pDoc->m_pAttrCtrl->GetElem(ElemK, ElemD)) { ASSERT(0); return FALSE; }
						
						double dLocalVector[3][3]={0.,};
						if(!m_pDoc->calcElemLocalVector(ElemD, dLocalVector)) { ASSERT(0); return FALSE; }
						
						double dElemLength = m_pDoc->calcLAVElem(ElemD);
						if(dElemLength<dZero) { ASSERT(0); return FALSE; }
						double dPos_x = dPosR * dElemLength;
						
						T_NODE_D BaseNodeD;
	          if(!m_pDoc->m_pAttrCtrl->GetNode((T_NODE_K)ElemD.elnod[0], BaseNodeD)) { ASSERT(0); return FALSE; }

						dLength = CMathFunc::mathLength(dCurX,dCurY,dCurZ, dPreX,dPreY,dPreZ);
						if( dLength < 1e-7 )
							continue;
						dPreX = dCurX;
						dPreY = dCurY;
						dPreZ = dCurZ;
					}*/
				}
				else if(nDir==DIR_XY_PLANE)
				{
					dx1 = aXYNodes[i]->GetAt(k).x; 
					dy1 = aXYNodes[i]->GetAt(k).y + dXYConvertY;
					dz1 = 0.0;
				}
				
				if(dx1 < aLableCoord[0])
				{
					aLableCoord[0] = dx1;
					aLableCoord[1] = dy1;
					aLableCoord[2] = dz1;
				}
				dMinX = min(dx1, dMinX);
				dMinY = min(dy1, dMinY);
				dMaxX = max(dx1, dMaxX);
				dMaxY = max(dy1, dMaxY);

				dxfEntities(_ULS(VERTEX));
				strTendonName = data.TendonName;
				ReplaceSpecialString(strTendonName);
				dxfLayerName(strTendonName);
				dxfCoord(1, 1, dx1);
				dxfCoord(1, 2, dy1);
				dxfCoord(1, 3, dz1);
			}
			dxfEntities(_ULS(SEQEND));

			dMarginX = dMinMaxDiffY/20.0;
			if(dMarginX<0.2) dMarginX = 0.2;
			dxfEntities(_ULS(TEXT));
			strTendonName = data.TendonName;
			ReplaceSpecialString(strTendonName);
			dxfLayerName(strTendonName);
			dxfCoord(1, 1, dMinX+(dMaxX-dMinX)/2.0);
			dxfCoord(1, 2, dMaxY+dMarginY);
			dxfCoord(1, 3, 0.0);
			dxfCoord(1, 4, dMarginX);
			dxfText(data.TendonName);
		} // end of m_aSelectedProfiles

		nPos = 75+12*(nDir+1);
		ProgressDlg->SetPos(nPos);

		dMarginX = dMinMaxDiffY/5.0;
		if(dMarginX<0.2) dMarginX = 0.2;
		dxfEntities(_ULS(TEXT));
		dxfLayerName(_ULS(0));
		dxfCoord(1, 1, aLableCoord[0]-dMarginX*4);
		dxfCoord(1, 2, aLableCoord[1]);
		dxfCoord(1, 3, aLableCoord[2]);
		dxfCoord(1, 4, dMarginX);
		if(nDir==DIR_XZ_PLANE)
			dxfText(_ULS(X-Z));
		else if(nDir==DIR_XY_PLANE)
			dxfText(_ULS(X-Y));
	} // end of nDir

	nPos = 100;
	ProgressDlg->SetPos(nPos);
	ProgressDlg->ShowWindow(SW_HIDE);
	ProgressReturn(ProgressDlg, TRUE);

	for(int i=0; i<aXZNodes.GetSize(); i++)
	{
		delete aXZNodes[i];
		aXZNodes[i] = NULL;
	}
	for(int i=0; i<aXYNodes.GetSize(); i++)
	{
		delete aXYNodes[i];
		aXYNodes[i] = NULL;
	}
	paNodesTemp = NULL;
	aXZNodes.RemoveAll();
	aXYNodes.RemoveAll();

//   time(&time2);
//   time3 = time2-time1;
//   timeR = localtime(&time3);
// 
//   str.Format("%d분 %d초", timeR->tm_min, timeR->tm_sec);
//   GSaveHistoryFormatNF(str);

}

BOOL CCMTendonExportDXFDlg::ProgressReturn(CDBProgressDlg *pPrgDlg, BOOL bReturn)
{
	if(pPrgDlg==NULL)
	{
		return bReturn;
	}
	pPrgDlg->SendMessage(WM_DESTROY,0,0);
	if (pPrgDlg) { delete pPrgDlg; pPrgDlg = NULL; }
	
	return bReturn;
}

BOOL CCMTendonExportDXFDlg::Write(LPCTSTR pFormat, ...)
{
	CWaitCursor Cursor;

	CString sData;
	int nLen;
	
	va_list args;
	va_start(args, pFormat);
	sData.FormatV(pFormat, args);
	nLen = sData.GetLength();
	
	m_cfile.WriteString(sData);
	//m_cfile.Write((const void*)sData, nLen);
	
	return TRUE;
}

void CCMTendonExportDXFDlg::dxfSection(CString csSectionName)
{
	Write(_T("%3d\n%s\n%3d\n%s\n"), 0, _T("SECTION"), 2, csSectionName);
}

void CCMTendonExportDXFDlg::dxfEndSection()
{
	Write(_T("%3d\n%s\n"), 0, _T("ENDSEC"));
}

void CCMTendonExportDXFDlg::dxfVersion()
{
	Write(_T("%3d\n%s\n%3d\n%s\n"), 9, _T("$ACADVER"), 1, _T("AC1009"));
}

void CCMTendonExportDXFDlg::dxfTable()
{
	Write(_T("%3d\n%s\n"), 0, _T("TABLE"));
}

void CCMTendonExportDXFDlg::dxfEndTable()
{
	Write(_T("%3d\n%s\n"), 0, _T("ENDTAB"));
}

void CCMTendonExportDXFDlg::dxfEOF()
{
	Write(_T("%3d\n%s\n"), 0, _T("EOF"));
}

void CCMTendonExportDXFDlg::dxfTableName(CString csTableName)
{
	Write(_T("%3d\n%s\n"), 2, csTableName);
}

void CCMTendonExportDXFDlg::dxfTableEntry(int nEntry)
{
	Write(_T("%3d\n%6d\n"), 70, nEntry);
}

void CCMTendonExportDXFDlg::dxfLayer(CString csLayerName, int nLayerFlag, int nLayerColor, CString csLineType)
{
	Write(_T("%3d\n%s\n")
		_T("%3d\n%s\n")
		_T("%3d\n%6d\n")
		_T("%3d\n%6d\n")
		_T("%3d\n%s\n"),
		0, _T("LAYER"),
		2, csLayerName,
		70, nLayerFlag,
		62, nLayerColor,
		6, csLineType);
}

void CCMTendonExportDXFDlg::dxfEntities(CString csEntityName)
{
	Write(_T("%3d\n%s\n"), 0, csEntityName);
}

void CCMTendonExportDXFDlg::dxfLayerName(CString csLayerName)
{
	Write(_T("%3d\n%s\n"), 8, csLayerName);
}

void CCMTendonExportDXFDlg::dxfCoord(int np, int nXYZ, double dCoord)
{
	int in = nXYZ*10+np-1;
	Write(_T("%3d\n%13.6f\n"), in, dCoord);
}

void CCMTendonExportDXFDlg::dxfText(CString csText)
{
	Write(_T("%3d\n%s\n"), 1, csText);
}

void CCMTendonExportDXFDlg::ReplaceSpecialString(CString& str)
{
	str.Replace(_T(" "), _T("_"));
	str.Replace(_T("  "), _T("__"));
	str.Replace(_T("   "), _T("___"));

	str.Replace(_T("\\"), _T("_"));
	str.Replace(_T("/"),  _T("_"));
	str.Replace(_T(":"),  _T("_"));
	str.Replace(_T("*"),  _T("_"));
	str.Replace(_T("?"),  _T("_"));
	str.Replace(_T("\""), _T("_"));
	str.Replace(_T("<"),  _T("_"));
	str.Replace(_T(">"),  _T("_"));
	str.Replace(_T("|"),  _T("_"));

	str.Replace(_T("~"),  _T("_"));
	str.Replace(_T("`"),  _T("_"));
	str.Replace(_T("!"),  _T("_"));
	str.Replace(_T("@"),  _T("_"));
	str.Replace(_T("#"),  _T("_"));
	str.Replace(_T("$"),  _T("_"));
	str.Replace(_T("%"),  _T("_"));
	str.Replace(_T("^"),  _T("_"));
	str.Replace(_T("&"),  _T("_"));
	str.Replace(_T("("),  _T("_"));
	str.Replace(_T(")"),  _T("_"));
	str.Replace(_T("+"),  _T("_"));
	str.Replace(_T("{"),  _T("_"));
	str.Replace(_T("}"),  _T("_"));
	str.Replace(_T(";"),  _T("_"));
	str.Replace(_T("\""), _T("_"));
	str.Replace(_T("\'"), _T("_"));
	str.Replace(_T(","),  _T("_"));
	str.Replace(_T("."),  _T("_"));
}