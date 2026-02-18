// DgnStlSPCSectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "wg_dgn.h"
#include "DgnStlSPCSectDlg.h"

#include "DgnSteelSPCSectGrid.h"
#include "StlSPCSectEventHandler.h"

#include "..\wg_db\wg_db_DBLib.h"
#include "..\wg_db\wg_db_SectUtil.h"
#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\wg_db_DataCtrl.h"
#include "..\wg_db\wg_db_AttrCtrl.h"
#include "..\wg_dbLock\LockOption.h"

#include "..\wg_cmd\CMGeometry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// Cell Number Label
CPointLabel::CPointLabel()
{
	
#ifdef _JP
	int nSmallFontSize = 9*0.86;
	m_LogFont.lfHeight			= nSmallFontSize;
	m_LogFont.lfWidth				= (int) ((double)nSmallFontSize/2.);
#elif _CH
	int nSmallFontSize = 9*0.86;
	m_LogFont.lfHeight			= nSmallFontSize;
	m_LogFont.lfWidth				= (int) ((double)nSmallFontSize/2.3);
#else
	int nSmallFontSize = 9;
	m_LogFont.lfHeight			= nSmallFontSize;
	m_LogFont.lfWidth				= (int) ((double)nSmallFontSize/2.0);
#endif
	m_LogFont.lfHeight*=10;
	m_LogFont.lfWidth=0;

	m_LogFont.lfEscapement		= 0;
	m_LogFont.lfOrientation		= 0;
	m_LogFont.lfWeight			= FW_NORMAL;
	m_LogFont.lfItalic			= FALSE;
	m_LogFont.lfUnderline			= FALSE;
	m_LogFont.lfStrikeOut			= FALSE;
	m_LogFont.lfCharSet			= DEFAULT_CHARSET;
	m_LogFont.lfOutPrecision		= OUT_CHARACTER_PRECIS;
	m_LogFont.lfClipPrecision		= CLIP_CHARACTER_PRECIS;
	m_LogFont.lfQuality			= DEFAULT_QUALITY;
	m_LogFont.lfPitchAndFamily	= DEFAULT_PITCH | FF_DONTCARE;
	//strcpy(m_LogFont.lfFaceName, _LS(IDS_WG_GPS_FONT_COURIER_NEW));
	wcscpy(m_LogFont.lfFaceName, _LSX(Courier New));
	m_Font.CreatePointFontIndirect(&m_LogFont);
		
}

CPointLabel::~CPointLabel()
{
	if(m_Font.m_hObject) 
	{
		m_Font.DeleteObject();
		m_Font.m_hObject = NULL;
	}
}

void CPointLabel::On_Paint(I_RenderEngine* pIRE)
{
	CDgnStlSPCSectDlg* pDlg = m_pDlg;
	if(!pDlg){ ASSERT(0); return; }
	if(!::IsWindow(pDlg->GetSafeHwnd())){ ASSERT(0); return; }
	
	DrawPointLabel(pIRE);
}


void CPointLabel::DrawPointLabel(I_RenderEngine* pIRE)
{
	CArray<DV_3dp, DV_3dp&> arPoint;
	arPoint.RemoveAll();
	m_pDlg->GetPointData(arPoint);
	if(arPoint.GetSize() != 5)
		return;
	
	ASSERT(pIRE->GetCurDC());
	ASSERT(pIRE->GetOwnerWnd());
	CDC* pDC = pIRE->GetCurDC();

	CFont		*old_font = (CFont *) pDC->SelectObject(&m_Font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER|TA_TOP);

	CString strText;
	int xPos,yPos;
	//순서준수!!
	strText.Format(_T("Cent"));
	pIRE->WorldToScreen(arPoint[0].x(), arPoint[0].y(), arPoint[0].z(), xPos, yPos);
	pDC->TextOut(xPos,yPos,strText);
	strText.Format(_T("1"));
	pIRE->WorldToScreen(arPoint[1].x(), arPoint[1].y(), arPoint[1].z(), xPos, yPos);
	pDC->TextOut(xPos,yPos,strText);
	strText.Format(_T("2"));
	pIRE->WorldToScreen(arPoint[2].x(), arPoint[2].y(), arPoint[2].z(), xPos, yPos);
	pDC->TextOut(xPos,yPos,strText);
	strText.Format(_T("3"));
	pIRE->WorldToScreen(arPoint[3].x(), arPoint[3].y(), arPoint[3].z(), xPos, yPos);
	pDC->TextOut(xPos,yPos,strText);
	strText.Format(_T("4"));
	pIRE->WorldToScreen(arPoint[4].x(), arPoint[4].y(), arPoint[4].z(), xPos, yPos);
	pDC->TextOut(xPos,yPos,strText);  

	pDC->SelectObject(old_font);
}















/////////////////////////////////////////////////////////////////////////////
// CDgnStlSPCSectDlg dialog


CDgnStlSPCSectDlg::CDgnStlSPCSectDlg(CWnd* pParent /*=NULL*/)
	: CDialogMove(CDgnStlSPCSectDlg::IDD, pParent)
{
	m_pDoc = CDBDoc::GetDocPoint();

	m_sectData.Initialize();
	m_pDrawData = NULL;

	m_PointMode = POINT_MODE_NONE;

	//{{AFX_DATA_INIT(CDgnStlSPCSectDlg)	
	//}}AFX_DATA_INIT

	//................................................................
	CArray<UINT,UINT> TBarLayout;
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_SINGLE     );
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_POLYGON    );
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_POLYLINE   );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_RECT     );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_POLYGON  );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_POLYLINE );
	//TBarLayout.Add(VP_TBarID::Instance().SELECT_ALL        );
	//TBarLayout.Add(VP_TBarID::Instance().UNSELECT_ALL      );
	TBarLayout.Add(VP_TBarID::Instance().ZOOM_ALL          );
	TBarLayout.Add(VP_TBarID::Instance().ZOOM_WINDOW       );
	TBarLayout.Add(VP_TBarID::Instance()._ZOOM_IN          );
	TBarLayout.Add(VP_TBarID::Instance()._ZOOM_OUT         );
	//TBarLayout.Add(VP_TBarID::Instance().SHOW_UCSAXIS      );
	//---------- DLG MDView -----------------
	m_pIVP = I_Factory_DlgViewPanel::Instance()->NewDlgViewPanel();
	m_pIVP->SetWndReceiver(this);

	m_pIVP->SetToolBarLayout(TBarLayout);
	//m_pIVP->SetHidePositionPanel();

	m_pEventHandler = NULL;
	//................................................................

	m_aSectList.RemoveAll();

	m_pIDlg		= new CDgnStlSPCSectITabDlg(this);
	//m_pIDlg->SetDlgViewPanel(this->m_pIVP); //Create전에 View를 연결. 호출 위치를 변경하지 말것.
	
	m_pJDlg		= new CDgnStlSPCSectJTabDlg(this);

	m_pIDlg->m_sectData = &m_sectData.SectBefore.SectI;
	m_pJDlg->m_sectData = &m_sectData.SectBefore.SectJ;
}

CDgnStlSPCSectDlg::~CDgnStlSPCSectDlg()
{
	if(NULL != m_pEventHandler)
	{
		m_pIVP->ResetAllEVHandler();
		delete m_pEventHandler;
	}

	if(NULL != m_pIVP)
	{
		m_pIVP->On_Destroy(this);
		delete m_pIVP;
	}
}




void CDgnStlSPCSectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogMove::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDgnStlSPCSectDlg)
	DDX_Control(pDX, IDC_DGN_TAB, m_ctrlTab);
	DDX_Control(pDX, IDC_DGN_GEN_SECT_COMBO, m_ComboSectName);
	DDX_Control(pDX, IDC_DGN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_DGN_VIEW_TEXT_OUT, m_strTextOut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDgnStlSPCSectDlg, CDialogMove)
	//{{AFX_MSG_MAP(CDgnStlSPCSectDlg)
	ON_CBN_SELCHANGE(IDC_DGN_GEN_SECT_COMBO, OnSelchangeSectCombo)
	ON_NOTIFY(TCN_SELCHANGE, IDC_DGN_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_DGN_TAB, OnSelchangingTab)
	ON_BN_CLICKED(IDC_DGN_APPLY, OnClickApplyBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDgnStlSPCSectDlg message handlers


BOOL CDgnStlSPCSectDlg::OnInitDialog() 
{
	CDialogMove::OnInitDialog();
	
	//.......................................................
	m_pIVP->On_InitDialog(this,IDC_DGN_SECT_VIEW); 
	m_PointLabel.SetDlg(this);
	m_pIVP->AddLabel(&m_PointLabel);


	InitSectNameCombo();
	InitTabCtrl(); 	
	InitLineData();  

	m_pEventHandler = new CStlSPCSectEventHandler(this);
	m_pEventHandler->SetDlgViewPanel(m_pIVP);
	m_pIVP->PushEVHandler(m_pEventHandler);

	m_pIVP->ShowPointGrid(TRUE);
	m_pIVP->SnapFree();
	m_pIVP->ShowAxis(FALSE);
	m_pEventHandler->SetWndTexOut(&m_strTextOut);  
	//..........................................................

	/////////////////////////////////////////////////////////////////

	m_pIVP->DeleteAllObject();
	SetLineDraw();
	SetPointDraw();
	m_pIVP->ZoomAll();

	m_PointMode = POINT_MODE_NONE;
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDgnStlSPCSectDlg::InitSectNameCombo()
{  
	m_ComboSectName.ResetContent();
	
	CArray<T_SECT_K, T_SECT_K> arKeyList;
	T_SECT_D SectData;
	int ComboIndex = 0;
	m_pDoc->m_pAttrCtrl->GetSectKeyList(arKeyList);

	int arKeyListSize = arKeyList.GetSize();
	for(int i=0 ; i<arKeyListSize ; ++i)
	{
		if(m_pDoc->m_pAttrCtrl->GetSectDesign(arKeyList[i], SectData))
		{
			if((SectData.nStype == D_SECT_TYPE_USER && SectData.SectBefore.Shape == _T("GEN")) ||
				(SectData.nStype == D_SECT_TYPE_TAPERED && SectData.SectBefore.nStype==D_SECT_TYPE_USER && SectData.SectBefore.Shape == _T("GEN")))
			{//유저 input 타입이면서 General 타입이어야한다, 또는 테이퍼드단면이면서 USER 타입이어야하고 General 타입이어야한다
				if(SectData.SectBefore.SectI.aLine.GetSize()>0)
				{//Line Type이어야한다
					BOOL bExistShape = FALSE;
					if(SectData.nStype==D_SECT_TYPE_USER && SectData.SectBefore.SectI.aShape.GetSize()>0)
						bExistShape = TRUE;
					if(SectData.nStype==D_SECT_TYPE_TAPERED && SectData.SectBefore.SectI.aShape.GetSize()>0 && SectData.SectBefore.SectJ.aShape.GetSize()>0)
						bExistShape = TRUE;
				
					if(bExistShape)
					{
						BOOL bSteel = TRUE;
						//BOOL bTsgr = TRUE;
						CArray<T_ELEM_K, T_ELEM_K> arElemKeyList;
						T_ELEM_D ElemData;
						T_MATL_D DataMatl;

						
						CArray<T_SECT_K,T_SECT_K> arSectK;
						arSectK.SetSize(1);	// Always 1.
						arSectK.Add(arKeyList[i]);
						
						m_pDoc->m_pAttrCtrl->GetKeyListSectUser(&arSectK, NULL, &arElemKeyList);
						int arElemKeyListSize = arElemKeyList.GetSize();
						for(int k=0 ; k<arElemKeyListSize ; k++)
						{              

							/*
							if(m_pDoc->m_pAttrCtrl->GetElemTsgrKey(arElemKeyList[k]))
							{
								bTsgr = FALSE;
								break;
							}
							*/
							
							if(m_pDoc->m_pAttrCtrl->GetElem(arElemKeyList[k], ElemData))
							{
								m_pDoc->m_pAttrCtrl->GetMatl(ElemData.elmat, DataMatl);
								int nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(arElemKeyList[k]);
								if(ElemData.elpro == arKeyList[i])
								{//해당 Elem의 Section키가 같은 것들 중에
									if(DataMatl.Type != _T("S"))
									{//Steel이 아닌것이 하나라도 있으면 안된다
										bSteel = FALSE;
									}
								}
							}
						}//End for arElemKeyListSize

						if(bSteel /*&& bTsgr*/)
						{
							m_ComboSectName.AddString(SectData.SName);
							m_aSectList.Add(arKeyList[i]);
						}
					}//
				}//End if aLine.GetSize
			}//End if GEN Type
		}//End if
	}//End for arKeyListSize

	if(m_ComboSectName.GetCount() > 0)
	{
		m_ComboSectName.SetCurSel(0);
		//int nSectKey = m_aSectList[0];		
		//m_pDoc->m_pAttrCtrl->GetSectDesign(nSectKey, m_sectData);		
		OnSelchangeSectCombo();
	}
}

BOOL CDgnStlSPCSectDlg::InitTabCtrl()
{
	TCHAR* aTitle[]  = { _T("I"), _T("J")};
	UINT  nDlgResID;

	/*
	CWnd* pPlaceHolder = GetDlgItem(IDC_DGN_PLACE_HOLDER);
	ASSERT(pPlaceHolder);
	m_ctrlTab.SetPlaceHolder(pPlaceHolder);*/

	//I  
	nDlgResID = CDgnStlSPCSectITabDlg::IDD;
	m_ctrlTab.AddTab( m_pIDlg, aTitle[I_TAB], nDlgResID, TRUE);

	//J
	nDlgResID = CDgnStlSPCSectJTabDlg::IDD;
	m_ctrlTab.AddTab( m_pJDlg, aTitle[J_TAB], nDlgResID, TRUE);  

	m_ctrlTab.SetCurSel(0);
	m_ctrlTab.ShowTab(0);

	return TRUE;
}

BOOL CDgnStlSPCSectDlg::Dlg2Data()
{
	if( m_ctrlTab.GetCurSel() == I_TAB  )	
	{
		m_pIDlg->DlgToData();
		m_pIDlg->GridToData();
	}
	else if( m_ctrlTab.GetCurSel() == J_TAB )  
	{
		m_pJDlg->DlgToData();
		m_pJDlg->GridToData();
	}
	else 
		return FALSE;

	return TRUE;
}

BOOL CDgnStlSPCSectDlg::Data2Dlg()
{
	int nTabIndex = m_ctrlTab.GetCurSel();

	m_pDrawData = &m_sectData.SectBefore.SectI;
	InitPointListData();
	InitLineData();

	if(nTabIndex == I_TAB)
	{
		m_pDrawData = &m_sectData.SectBefore.SectI;
		InitPointListData();
		InitLineData();
		m_pIVP->DeleteAllObject();
	  SetLineDraw();
		SetPointDraw();
		m_pIVP->ZoomAll();

		////////////////////////////
		m_pIDlg->DataToDlg();
		m_pIDlg->DataToGrid();
		//m_pIDlg->SetPointComboItem();
	}
	else if(nTabIndex == J_TAB)
	{
		m_pDrawData = &m_sectData.SectBefore.SectJ;
		InitPointListData();
		InitLineData();
		m_pIVP->DeleteAllObject();
	  SetLineDraw();
		SetPointDraw();
		m_pIVP->ZoomAll();

		/////////////////////////////
		m_pJDlg->DataToDlg();		
		m_pJDlg->DataToGrid();
		//m_pJDlg->SetPointComboItem();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// OnOK 관련 
//


void CDgnStlSPCSectDlg::OnClickApplyBtn() 
{
#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

	if(m_ComboSectName.GetCount() <= 0)
		return;
	if(!Dlg2Data())
		return;

	int nIndex = m_ComboSectName.GetCurSel();
	int nSectKey = m_aSectList[nIndex];
	m_pDoc->m_pDataCtrl->AddSecd(nSectKey, m_sectData);
}

void CDgnStlSPCSectDlg::OnCancel() 
{	
	CDialogMove::OnCancel();
}

void CDgnStlSPCSectDlg::OnOK() 
{
#if defined(_CIVIL_JP)
	if (!m_pDoc->AllowCommand(D_OPTN_ID_CVLNX_JP_STEEL_MEMBER_DESIGN))
	{
		CString strErrMsg = _T("");
		strErrMsg.Format(_LS(IDS_CMD_CODE_CHECK_ERROR), _LS(IDS_IDD_MAIN_PROTECTION_EX_OPT_CIVILNX_JP_DLG_IDC_LBC_LOCKOPT_CHK7));
		AfxMessageBox(strErrMsg);
		return;
	}
#endif

	if(m_ComboSectName.GetCount() <= 0)
		return;
	if(!Dlg2Data())
		return;

	int nIndex = m_ComboSectName.GetCurSel();
	int nSectKey = m_aSectList[nIndex];
	
	if(m_pDoc->m_pDataCtrl->AddSecd(nSectKey, m_sectData))
		CDialogMove::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Tab 관련 
//
void CDgnStlSPCSectDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(!m_pJDlg->bExistJ)
	{
		if(m_ctrlTab.GetCurSel() == 1)
			AfxMessageBox(_LS(IDS_DGN_TAPERED_INPUT_ONLY), MB_OK);

		m_ctrlTab.SetCurSel(0);
		m_ctrlTab.ShowTab(0);
	}
	else
	{
		int nTabIndex = m_ctrlTab.GetCurSel();    

	  if(nTabIndex == I_TAB)
	  {
			m_pDrawData = &m_sectData.SectBefore.SectI;
			InitPointListData();
			InitLineData();
			m_pIVP->DeleteAllObject();
	    SetLineDraw();
			SetPointDraw();
			m_pIVP->ZoomAll();    
			m_pIDlg->SetPointComboItem();
	  }
	  else if(nTabIndex == J_TAB)
	  {
			m_pDrawData = &m_sectData.SectBefore.SectJ;
			InitPointListData();
			InitLineData();
			m_pIVP->DeleteAllObject();
	    SetLineDraw();
			SetPointDraw();
			m_pIVP->ZoomAll();
			m_pJDlg->SetPointComboItem();
	  }
	}

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// SectCombo 관련 
//
void CDgnStlSPCSectDlg::OnSelchangeSectCombo() 
{
	int nIndex = m_ComboSectName.GetCurSel();
	int nSectKey = m_aSectList[nIndex];	
	m_pDoc->m_pAttrCtrl->GetSectDesign(nSectKey, m_sectData);

	if(m_sectData.nStype == D_SECT_TYPE_TAPERED)
	{
		m_pJDlg->bExistJ = TRUE;
	}
	else
	{		
		m_ctrlTab.SetCurSel(0);
		m_ctrlTab.ShowTab(0);
		m_pJDlg->bExistJ = FALSE;
	}

	Data2Dlg();
}

void CDgnStlSPCSectDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_ComboSectName.GetCount() > 0)
	{
	  if(!Dlg2Data()) *pResult = 1;
		else            *pResult = 0;
	}
}

void CDgnStlSPCSectDlg::InitLineData()
{
	m_aLineData.RemoveAll();
	if(m_pDrawData == NULL)
		return;

	int nShapeSize = m_pDrawData->aShape.GetSize();//Group의 갯수
	int nLdgnSize = 0;//각각Group의 SubGroup갯수
	T_GSEC_LSHAP shapData;
	for(int i=0 ; i<nShapeSize ; ++i)
	{
		shapData.Initialize();
		shapData = m_pDrawData->aShape[i];
		nLdgnSize = shapData.aLdgn.GetSize();
		for(int j=0 ; j<nLdgnSize ; ++j)
		{
			m_aLineData.Add(shapData.aLdgn[j]);
		}
	}
}

void CDgnStlSPCSectDlg::SelectLineRedraw(ROWCOL nRow)
{ 
	m_pIVP->DeleteAllObject();
	SetLineDraw();
	SetPointDraw();
	SetSelectLineDraw(nRow);
	//SetPointData();
	m_pIVP->RedrawAll();
	//m_pIVP->ZoomAll();	  
}

void CDgnStlSPCSectDlg::Redraw(BOOL bZoomAll)
{ 
	m_pIVP->DeleteAllObject();
	SetLineDraw();
	SetPointDraw();	
	m_pIVP->RedrawAll();
	if(bZoomAll)
		m_pIVP->ZoomAll();	  
}

void CDgnStlSPCSectDlg::SetViewLButtonDown(double dX, double dY)
{ 
	//(2009.11.10) Add by Unsang, Data가 없을 경우 return.. (MQC2990)
	if(m_pDrawData==NULL) return; // DrawData가 NULL인 경우
	
	double dCheckArea = 0.0;
	dCheckArea = m_pDrawData->Stiffness.Area;
	if(dCheckArea <= 0.0) return;

	double PositionX, PositionY = 0.0;

	CSectUtil::GetNearPoint(m_pDrawData, dX, dY, PositionX, PositionY);

	int nTabIndex = m_ctrlTab.GetCurSel();
	if(nTabIndex == I_TAB)
	{
		m_pIDlg->SetPointEdit(m_PointMode, PositionX, PositionY);
	}
	else if(nTabIndex == J_TAB)
	{
		m_pJDlg->SetPointEdit(m_PointMode, PositionX, PositionY);
	}  
}

void CDgnStlSPCSectDlg::InitPointListData()
{ 
	CArray<T_2COOR,T_2COOR&> OutLines;
	
	int nOutPoly = m_pDrawData->aOuterPolygon.GetSize();
	if (nOutPoly != 1) return;
 
	T_2COOR Coord;
	SCMPoint2D Point1,Point2;
	CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &m_pDrawData->aOuterPolygon[0].aVertex;
	for (int i = 0; i < paVertex->GetSize(); i++)
	{

		Coord.x = (*paVertex)[i].dX;
		Coord.y = (*paVertex)[i].dY;
		OutLines.Add(Coord);
	}
	
	int nShapeSize = m_pDrawData->aShape.GetSize();//Group의 갯수
	int nLdgnSize = 0;//각각Group의 SubGroup갯수
	
	T_GSEC_LDGN ldgnData;
	ldgnData.Initialize();
	DV_3dp point;

	m_aPointData.RemoveAll();

	T_GSEC_LSHAP shapData;
	for(int i=0 ; i<nShapeSize ; ++i)
	{
		shapData.Initialize();
		shapData = m_pDrawData->aShape[i];
		nLdgnSize = shapData.aLdgn.GetSize();
		for(int j=0 ; j<nLdgnSize ; ++j)
		{
			ldgnData = shapData.aLdgn[j];

			int nSizeLineData = ldgnData.aLine.GetSize();
			for(int k=0 ; k<nSizeLineData ; ++k)
			{
                point.Set(OutLines[ldgnData.aLine[k].v1].x, OutLines[ldgnData.aLine[k].v1].y, 0);
				m_aPointData.Add(point);
				point.Set(OutLines[ldgnData.aLine[k].v2].x, OutLines[ldgnData.aLine[k].v2].y, 0);    
				m_aPointData.Add(point);	
			}
		}
	}
}

void CDgnStlSPCSectDlg::SetLineDraw()
{  
	CArray<T_2COOR,T_2COOR&> OutLines;

	if(m_pDrawData == NULL)
		return;
	
	int nOutPoly = m_pDrawData->aOuterPolygon.GetSize();
	if (nOutPoly != 1) return;
 
	T_2COOR Coord;
	SCMPoint2D Point1,Point2;
	CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &m_pDrawData->aOuterPolygon[0].aVertex;
	for (int i = 0; i < paVertex->GetSize(); i++)
	{

		Coord.x = (*paVertex)[i].dX;
		Coord.y = (*paVertex)[i].dY;
		OutLines.Add(Coord);
	}
	
	int aLineSize = m_pDrawData->aLine.GetSize();	

	DlgView_Object dbject;  
	DV_3dp point1, point2;
	for(int j=0;j<aLineSize;j++)
	{    
		point1.Set(OutLines[m_pDrawData->aLine[j].v1].x, OutLines[m_pDrawData->aLine[j].v1].y, 0);
		point2.Set(OutLines[m_pDrawData->aLine[j].v2].x, OutLines[m_pDrawData->aLine[j].v2].y, 0);
		dbject.m_GData.SetLine(point1, point2);
		dbject.m_nLineWidth = m_pDrawData->aLine[j].dThik;
		m_pIVP->AddObject(dbject);		
	}
}

void CDgnStlSPCSectDlg::SetPointDraw()
{ 
	if(m_pDrawData == NULL)
		return;

	if(m_pDrawData->aLine.GetSize() <= 0)
		return;

	DlgView_Object dbject1, dbject2, dbject3, dbject4, dbjectCent;  
	DV_3dp point1, point2, point3, point4, pointCent;
	
	point1.Set(m_pDrawData->Stiffness.dx1, m_pDrawData->Stiffness.dy1, 0);  
	dbject1.m_GData.SetPoint(point1);  
	dbject1.m_PointColor     = RGB(255,0,0 );
	m_pIVP->AddObject(dbject1);

	point2.Set(m_pDrawData->Stiffness.dx2, m_pDrawData->Stiffness.dy2, 0);  
	dbject2.m_GData.SetPoint(point2);  
	dbject2.m_PointColor     = RGB(255,0,0 );
	m_pIVP->AddObject(dbject2);

	point3.Set(m_pDrawData->Stiffness.dx3, m_pDrawData->Stiffness.dy3, 0);  
	dbject3.m_GData.SetPoint(point3); 
	dbject3.m_PointColor     = RGB(255,0,0 );
	m_pIVP->AddObject(dbject3);

	point4.Set(m_pDrawData->Stiffness.dx4, m_pDrawData->Stiffness.dy4, 0);  
	dbject4.m_GData.SetPoint(point4);  
	dbject4.m_PointColor     = RGB(255,0,0 );
	m_pIVP->AddObject(dbject4);    

	//Center는 0,0으로 셋팅
	pointCent.Set(0, 0, 0);  
	dbjectCent.m_GData.SetPoint(pointCent); 
	dbjectCent.m_PointColor     = RGB(0,0,255 );
	m_pIVP->AddObject(dbjectCent);
	

	/*
	pointCent.Set(m_pDrawData->Design.YBar, m_pDrawData->Design.ZBar, 0);  
	dbjectCent.m_GData.SetPoint(pointCent); 
	dbjectCent.m_PointColor     = RGB(0,0,255 );
	m_pIVP->AddObject(dbjectCent);    */  
}

void CDgnStlSPCSectDlg::GetPointData(CArray<DV_3dp, DV_3dp&>& arPoint)
{ 
	//CArray<DV_3dp, DV_3dp&> arPoint;
	arPoint.RemoveAll();
	DV_3dp point;  

	if(m_pDrawData == NULL)
		return;

	if(m_pDrawData->aLine.GetSize() <= 0)
		return;
	
	DV_3dp pointCent, point1, point2, point3, point4;  

	//pointCent.Set(m_pDrawData->Design.YBar, m_pDrawData->Design.ZBar, 0);  
	pointCent.Set(0, 0, 0);//Center는 0,0 으로 셋팅
	point1.Set(m_pDrawData->Stiffness.dx1, m_pDrawData->Stiffness.dy1, 0);    
	point2.Set(m_pDrawData->Stiffness.dx2, m_pDrawData->Stiffness.dy2, 0);  
	point3.Set(m_pDrawData->Stiffness.dx3, m_pDrawData->Stiffness.dy3, 0);  
	point4.Set(m_pDrawData->Stiffness.dx4, m_pDrawData->Stiffness.dy4, 0);  
	//순서준수!!
	arPoint.Add(pointCent);
	arPoint.Add(point1);
	arPoint.Add(point2);
	arPoint.Add(point3);
	arPoint.Add(point4);

	return;
}

void CDgnStlSPCSectDlg::SetSelectLineDraw(ROWCOL nRow)
{  
	CArray<T_2COOR,T_2COOR&> OutLines;
	
	int nOutPoly = m_pDrawData->aOuterPolygon.GetSize();
	if (nOutPoly != 1) return;
 
	T_2COOR Coord;
	SCMPoint2D Point1,Point2;
	CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &m_pDrawData->aOuterPolygon[0].aVertex;
	for (int i = 0; i < paVertex->GetSize(); i++)
	{

		Coord.x = (*paVertex)[i].dX;
		Coord.y = (*paVertex)[i].dY;
		OutLines.Add(Coord);
	}
	
	int aLineSize = 0;
	T_GSEC_LDGN ldgnData;
	ldgnData.Initialize();

	ldgnData = m_aLineData[nRow-1];//Row는 1부터 시작하므로 1을빼줌
	aLineSize = ldgnData.aLine.GetSize();
	DlgView_Object dbject;  
	dbject.m_FillColor = RGB(0,100,255);
	dbject.m_EdgeColor = RGB(0,100,255);
	DV_3dp point1, point2;
	for(int j=0 ; j<aLineSize ; j++)
	{
	  point1.Set(OutLines[ldgnData.aLine[j].v1].x, OutLines[ldgnData.aLine[j].v1].y, 0);
		point2.Set(OutLines[ldgnData.aLine[j].v2].x, OutLines[ldgnData.aLine[j].v2].y, 0);    
		dbject.m_GData.SetLine(point1, point2);
		dbject.m_nLineWidth = ldgnData.aLine[j].dThik * 2;
		m_pIVP->AddObject(dbject);
	}//for	
}

/*void CDgnStlSPCSectDlg::GetNearPoint(double dx, double dy, double& dPointX, double& dPointY)
{
	DV_3dp point;
	double p1[3];
	p1[0] = dx;
	p1[1] = dy;
	p1[2] = 0;
	double p2[3];
	double dMinLength = 9999999999.9;
	double dTemp = 0.0;
	int nSize = m_aPointData.GetSize();
	for(int i=0 ; i<nSize ; ++i)
	{
		point = m_aPointData[i];
		p2[0] = point.x();
		p2[1] = point.y();
		p2[2] = 0;
		dTemp = CMathFunc::mathDistanceToPoint(p1, p2);
		if(dMinLength > dTemp)
		{
			dMinLength = dTemp;
			dPointX = p2[0];
			dPointY = p2[1];
		}    
	}
}

void CDgnStlSPCSectDlg::GetSubGroupList(double dx, double dy, CArray<int, int&>& arGroup, CArray<int, int&>& arSubGroup)
{
	arGroup.RemoveAll();
	arSubGroup.RemoveAll();

	CArray<T_2COOR,T_2COOR&> OutLines;
	int i = 0;
	
	if(m_pDrawData == NULL) return;
	int nOutPoly = m_pDrawData->aOuterPolygon.GetSize();
	if (nOutPoly != 1) return;
 
	T_2COOR Coord;
	SCMPoint2D Point1,Point2;
	CArray<T_GSEC_VERTEX, T_GSEC_VERTEX&>* paVertex = &m_pDrawData->aOuterPolygon[0].aVertex;
	for (i = 0; i < paVertex->GetSize(); i++)
	{

		Coord.x = (*paVertex)[i].dX;
		Coord.y = (*paVertex)[i].dY;
		OutLines.Add(Coord);
	}

	double dPointX, dPointY = 0.0;
	CSectUtil::GetNearPoint(m_pDrawData, dx, dy, dPointX, dPointY);

	T_GSEC_LDGN ldgnData;
	ldgnData.Initialize();

	int nShapeSize = m_pDrawData->aShape.GetSize();//Group의 갯수
	int nLdgnSize = 0;//각각Group의 SubGroup갯수
	T_GSEC_LSHAP shapData;
	for(i=0 ; i<nShapeSize ; ++i)
	{
		shapData.Initialize();
		shapData = m_pDrawData->aShape[i];
		nLdgnSize = shapData.aLdgn.GetSize();
		for(int j=0 ; j<nLdgnSize ; ++j)
		{
			ldgnData = shapData.aLdgn[j];
			int nSizeLineData = ldgnData.aLine.GetSize();
			for(int k=0 ; k<nSizeLineData ; ++k)
			{
				if(dPointX==OutLines[ldgnData.aLine[k].v1].x && dPointY==OutLines[ldgnData.aLine[k].v1].y)
				{
					arGroup.Add(i);
					arSubGroup.Add(j);
					break;
				}
				if(dPointX==OutLines[ldgnData.aLine[k].v2].x && dPointY==OutLines[ldgnData.aLine[k].v2].y)
				{
					arGroup.Add(i);
					arSubGroup.Add(j);
					break;
				}		
			}
		}
	}
}*/

void CDgnStlSPCSectDlg::SetSectIData()
{
	m_pDrawData = &m_sectData.SectBefore.SectI;  
}

void CDgnStlSPCSectDlg::SetSectJData()
{
	m_pDrawData = &m_sectData.SectBefore.SectJ;  
}

void CDgnStlSPCSectDlg::SetPointMode(int Mode)
{
	m_PointMode = Mode;
}
