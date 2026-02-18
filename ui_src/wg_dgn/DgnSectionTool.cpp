// DgnSectionTool.cpp: implementation of the CDgnSectionTool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DgnSectionTool.h"

#include "..\wg_db\wg_db_DBDoc.h"
#include "..\wg_db\SectDB.h"
#include "..\wg_db\AttrCtrl.h"
#include "..\wg_db\PostCtrl.h"
#include "..\wg_db\DBLib.h"
#include "..\wg_base\wg_base_I_Factory_DlgViewPanel.h"//추가 SHIN ('06.02.15)

#include "..\wg_db\wg_db_QSort.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDgnSectionTool::CDgnSectionTool()
{
	m_pDoc = CDBDoc::GetDocPoint();
}

CDgnSectionTool::~CDgnSectionTool()
{

}

BOOL CDgnSectionTool::Get_SectionPolygonData_DlgView_ObjectArray(T_SECT_D& SectD, int iPosi, CArray<DlgView_Object*,DlgView_Object*>& arViewGeom, CArray<double, double&>& arPos, double dOffsetX, double dOffsetY)
{  
	int i, k;
	int nSize = arViewGeom.GetSize();
	DlgView_Object* pDVO;
	for(i = 0; i < nSize; i++)
	{
		pDVO = arViewGeom[i];
		delete pDVO;
	}
	arViewGeom.RemoveAll();
	arPos.RemoveAll(); 
		
	CString strSectShape = SectD.SectBefore.Shape;
	
	if(strSectShape!=D_SECT_SHAPE_REG_GEN)return FALSE;

	T_SECT_SECTBASE_D SectBase;
	if(iPosi != 0 && SectD.nStype==D_SECT_TYPE_TAPERED)
		SectBase = SectD.SectBefore.SectJ;
	else
		SectBase = SectD.SectBefore.SectI;
	
		
	int iOuterPolygonSize = SectBase.aOuterPolygon.GetSize();
	int iInnerPolygonSize = SectBase.aInnerPolygon.GetSize();
	if(iOuterPolygonSize < 1)return FALSE;
	if(iOuterPolygonSize > 1)ASSERT(0);// SHIN ('06.02.17) 외곽 라인이 1개가 아닐경우 확인요망
	iOuterPolygonSize = 1;// SHIN ('06.02.17) 외곽 라인은 1개로 고정
	arViewGeom.SetSize(iOuterPolygonSize + iInnerPolygonSize);

	DV_3dp section3D;
	T_GSEC_POLYGON PolygonData;
	T_GSEC_VERTEX vertex2D;
	int iPolygonMemberSize;    
	double dHigh, dLow, dLeft, dRight;
	dHigh = dLow = dLeft = dRight = 0.0; //기본적으로 첫번째 좌표값으로 초기화 해야 하나 도심점을 중심으로 설정
	//외곽라인 입력
	for(i=0 ; i<iOuterPolygonSize ; i++)
	{    
		PolygonData.Initialize();
		PolygonData = SectBase.aOuterPolygon.GetAt(i);
		iPolygonMemberSize = PolygonData.aVertex.GetSize();
		pDVO = new DlgView_Object;
		pDVO->Initialize();
		pDVO->m_FillColor = RGB(200,200,200);
		pDVO->m_EdgeColor = RGB(200,200,200);
		pDVO->m_cPolyFillMode  = 'F';
		pDVO->m_GData.m_nGeomType = 3;

		pDVO->m_GData.m_GeomData.SetSize(iPolygonMemberSize);
		for(k=0; k<iPolygonMemberSize ; k++)
		{
			vertex2D = PolygonData.aVertex.GetAt(k);      
			section3D.Set(vertex2D.dX+dOffsetX, vertex2D.dY+dOffsetY, 0.0);
			dHigh = max(dHigh,  section3D.m_p[1]);
			dLow  = min(dLow,   section3D.m_p[1]);
			dRight= max(dRight, section3D.m_p[0]);
			dLeft = min(dLeft,  section3D.m_p[0]);
			pDVO->m_GData.m_GeomData.SetAt(k, section3D);
		}
		arViewGeom.SetAt(i, pDVO);
	}
	//안쪽라인들 입력
	for(i=0 ; i<iInnerPolygonSize ; i++)
	{    
		PolygonData.Initialize();
		PolygonData = SectBase.aInnerPolygon.GetAt(i);
		iPolygonMemberSize = PolygonData.aVertex.GetSize();
		pDVO = new DlgView_Object;
		pDVO->Initialize();
		pDVO->m_FillColor = RGB(255,255,255);
		pDVO->m_EdgeColor = RGB(200,200,200);
		pDVO->m_cPolyFillMode  = 'F';
		pDVO->m_GData.m_nGeomType = 3;

		pDVO->m_GData.m_GeomData.SetSize(iPolygonMemberSize);
		for(k=0; k<iPolygonMemberSize ; k++)
		{
			vertex2D = PolygonData.aVertex.GetAt(k);
			section3D.Set(vertex2D.dX+dOffsetX, vertex2D.dY+dOffsetY, 0.0);
			pDVO->m_GData.m_GeomData.SetAt(k, section3D);
		}
		arViewGeom.SetAt(iOuterPolygonSize+i, pDVO);
	}

	arPos.SetSize(6);      
	arPos.SetAt(0, dOffsetX);   // offsetX
	arPos.SetAt(1, dHigh);      // maxY
	arPos.SetAt(2, dLow);       // minY
	arPos.SetAt(3, dLeft);      // minZ
	arPos.SetAt(4, dRight);     // maxY
	arPos.SetAt(5, dOffsetY);   // offsetY

	return TRUE;
}

BOOL CDgnSectionTool::Cng_From_REBT_To_RBTE(T_REBT_D InData, T_RBTE_D & OutData)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	OutData.Initialize();
	OutData.iCenterType  = InData.iCenterType;
	OutData.bIJBothMain  = InData.bIJBothMain;
	OutData.bIJBothShear = InData.bIJBothShear;

	int iRebarSize;

	T_RBTE_MABR mainRebarUnit_Out;
	T_REBT_MABR mainRebarUnit_In;
	for(int i=0 ; i<3 ; i++)
	{
		iRebarSize = InData.arMainRebar[i].GetSize();
		OutData.arMainRebar[i].SetSize(iRebarSize);
		for(int k=0 ; k<iRebarSize ; k++)
		{    
			mainRebarUnit_In = InData.arMainRebar[i].GetAt(k);
			mainRebarUnit_Out.dCenterY     = mainRebarUnit_In.dCenterY;
			mainRebarUnit_Out.dCenterZ     = mainRebarUnit_In.dCenterZ;
			mainRebarUnit_Out.iLabel       = mainRebarUnit_In.iLabel      ;
			mainRebarUnit_Out.strRebarName = mainRebarUnit_In.strRebarName;
			OutData.arMainRebar[i].SetAt(k, mainRebarUnit_Out);
		}
		OutData.ShearRebar[i].dSubRebarSpace =InData.ShearRebar[i].dSubRebarSpace ;
		OutData.ShearRebar[i].dSubRebarNum[0]=InData.ShearRebar[i].dSubRebarNum[0];
		OutData.ShearRebar[i].dSubRebarNum[1]=InData.ShearRebar[i].dSubRebarNum[1];
		OutData.ShearRebar[i].strSubRebarName=InData.ShearRebar[i].strSubRebarName;
	}  
	return TRUE;
}

BOOL CDgnSectionTool::Cng_From_RBTE_To_REBT(T_RBTE_D InData, T_REBT_D & OutData)
{
	// Before_Release_v825_Check - Rebar 구조체 변경되면서 수정되어야 하는 부분 
	OutData.Initialize();
	OutData.iCenterType  = InData.iCenterType;
	OutData.bIJBothMain  = InData.bIJBothMain;
	OutData.bIJBothShear = InData.bIJBothShear;

	int iRebarSize;

	T_REBT_MABR mainRebarUnit_Out;
	T_RBTE_MABR mainRebarUnit_In;
	for(int i=0 ; i<3 ; i++)
	{
		iRebarSize = InData.arMainRebar[i].GetSize();
		OutData.arMainRebar[i].SetSize(iRebarSize);
		for(int k=0 ; k<iRebarSize ; k++)
		{    
			mainRebarUnit_In = InData.arMainRebar[i].GetAt(k);
			mainRebarUnit_Out.dCenterY     = mainRebarUnit_In.dCenterY;
			mainRebarUnit_Out.dCenterZ     = mainRebarUnit_In.dCenterZ;
			mainRebarUnit_Out.iLabel       = mainRebarUnit_In.iLabel      ;
			mainRebarUnit_Out.strRebarName = mainRebarUnit_In.strRebarName;
			OutData.arMainRebar[i].SetAt(k, mainRebarUnit_Out);
		}
		OutData.ShearRebar[i].dSubRebarSpace =InData.ShearRebar[i].dSubRebarSpace ;
		OutData.ShearRebar[i].dSubRebarNum[0]=InData.ShearRebar[i].dSubRebarNum[0];
		OutData.ShearRebar[i].dSubRebarNum[1]=InData.ShearRebar[i].dSubRebarNum[1];
		OutData.ShearRebar[i].strSubRebarName=InData.ShearRebar[i].strSubRebarName;
	}  
	return TRUE;
}

BOOL CDgnSectionTool::GetRbte(int iElem, T_RBTE_D& RbteD)
{
	CDBDoc* pDoc = CDBDoc::GetDocPoint();// static이므로 채워야 함
	if(!(pDoc->m_pAttrCtrl->GetRbte(iElem, RbteD)))// 요소에 임의 철근 배치정보가 있을 경우 요소값을 사용 
	{// 요소에 임의 철근 배치정보가 없을 경우 Section에 연결된 철근 배근정보를 찾음
		T_ELEM_D ElemD; ElemD.Initialize();
	  if(!pDoc->m_pAttrCtrl->GetElem(iElem,ElemD))	return FALSE;
	  int iSect = ElemD.elpro;
		T_REBT_D sect_RebtDataD;   sect_RebtDataD.Initialize();
		if((pDoc->m_pAttrCtrl->GetRebt(iSect, sect_RebtDataD)))
		{      
			if(!Cng_From_REBT_To_RBTE(sect_RebtDataD, RbteD)) return FALSE;      
		}
		else
			return FALSE;
	}
	return TRUE;
}

//=============================================================================

BOOL CDgnSectionTool::Get_DgncCpplData(T_RCPL_K RcplKey, _DGNC_CPPL_D& DgncCpplD)
{
	DgncCpplD.Initialize();

	CDBDoc* pDoc = CDBDoc::GetDocPoint();
	T_RCPL_D RcplData; 
	if(!pDoc->m_pAttrCtrl->GetRcpl(RcplKey,RcplData)) {return FALSE;}

	int nElemListSize = RcplData.arElemK.GetSize();
	T_CPPL_D CpplData;

	int nNumCount, nCTCCount;
	// 가중치는 Node 1개당 1.0의 가중치를 가짐 혼합배근의 경우 각각 0.5의 가중치를 가짐
	double  arTotalNumTop[5],  arTotalNumBot[5],  arTotalCTCTop[5],  arTotalCTCBot[5];
	double  arWeightNumTop[5], arWeightNumBot[5], arWeightCTCTop[5], arWeightCTCBot[5];
	double  arTotalDt[5],  arTotalDb[5];
	double  arWeightDt[5], arWeightDb[5];
	CMap<CString, LPCTSTR, double, double&> arWeightBarNaTop[5];
	CMap<CString, LPCTSTR, double, double&> arWeightBarNaBot[5];
	//
	double dTotalSubBarNum,  dTotalSubBarDist;
	double dWeightSubBarNum, dWeightSubBarDist;
	CMap<CString, LPCTSTR, double, double&> arWeightSubBarNa;


	int i, j, k;
	nNumCount = nCTCCount = 0;
	for(i=0 ; i<5 ; i++)
	{
		arTotalNumTop[i]  = 0.0;     arTotalNumBot[i]  = 0.0; 
		arTotalCTCTop[i]  = 0.0;     arTotalCTCBot[i]  = 0.0;		
		arWeightNumTop[i] = 0.0;     arWeightNumBot[i] = 0.0; 
		arWeightCTCTop[i] = 0.0;     arWeightCTCBot[i] = 0.0;
		arTotalDt[i]      = 0.0;     arTotalDb[i]      = 0.0;
		arWeightDt[i]     = 0.0;     arWeightDb[i]     = 0.0;
	}	
	dTotalSubBarNum  = 0.0;  dTotalSubBarDist  = 0.0;
	dWeightSubBarNum = 0.0;  dWeightSubBarDist = 0.0;

	double  dWeightBar;
	double  dWeightBar_Delta;
	CString strBarNa1, strBarNa2;
	if(RcplData.iDir == 0)
	{// i:Elem Count   j:Node Count   k:Layer Count
		for(int i=0 ; i<nElemListSize ; i++)
		{
			if(!pDoc->m_pAttrCtrl->GetCppl(RcplData.arElemK.GetAt(i), CpplData)) continue;
			if(CpplData.bNum) nNumCount++;
			else              nCTCCount++;

			for(j=0 ; j<4 ; j++)
			{
				// Shear Rebar
				strBarNa1 = CpplData.strSubBarNa_x[j];				
				if(CpplData.dSubBarNum_x[j] != 0.0 && CpplData.dSubBarDist_x[j] != 0.0 && strBarNa1 != _T(""))
				{
					dWeightBar_Delta = 1.0;
					dTotalSubBarNum   += 1.0 * CpplData.dSubBarNum_x[j];
					dWeightSubBarNum  += 1.0;
					dTotalSubBarDist  += 1.0 * CpplData.dSubBarDist_x[j];
					dWeightSubBarDist += 1.0;
					if(strBarNa1 != _T(""))
					{
						if(!arWeightSubBarNa.Lookup(strBarNa1, dWeightBar))  dWeightBar = 0.0;
						dWeightBar += dWeightBar_Delta;
						arWeightSubBarNa.SetAt(strBarNa1, dWeightBar);
					}
				}
				// Main Rebar : Top				
				for(k=0 ; k<5 ; k++)
				{
					strBarNa1 = CpplData.strTopBarNa1_x[j][k];
					strBarNa2 = CpplData.strTopBarNa2_x[j][k];
					if(CpplData.dDt_x[j][k] > 0.0 && (CpplData.dTopBarNum_x[j][k] > 0.0 || CpplData.dTopBarCTC_x[j][k] > 0.0) && strBarNa1 != _T(""))
					{
						// Dt
						arTotalDt[k]  += 1.0 * CpplData.dDt_x[j][k];
						arWeightDt[k] += 1.0;
						// Num
						if(CpplData.dTopBarNum_x[j][k] > 0.0)
						{
							arTotalNumTop[k]  += 1.0 * CpplData.dTopBarNum_x[j][k];
							arWeightNumTop[k] += 1.0;
						}
						// CTC
						if(CpplData.dTopBarCTC_x[j][k] > 0.0)
						{
							arTotalCTCTop[k]  += 1.0 * CpplData.dTopBarCTC_x[j][k];
							arWeightCTCTop[k] += 1.0;
						}
						// BarNa						
						if(strBarNa1 != _T("") && strBarNa2 != _T("")) dWeightBar_Delta = 0.5;
						else                                           dWeightBar_Delta = 1.0;
						// - BarNa1
						if(strBarNa1 != _T(""))
						{
							if(!arWeightBarNaTop[k].Lookup(strBarNa1, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaTop[k].SetAt(strBarNa1, dWeightBar);
						}					
						// - BarNa2
						if(strBarNa1 != _T("") && strBarNa2 != _T(""))
						{
							if(!arWeightBarNaTop[k].Lookup(strBarNa2, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaTop[k].SetAt(strBarNa2, dWeightBar);
						}					
					}
					else
						break;//바깥쪽 레이어부터 검토하여 Dt가 입력된 곳까지 검토함. 중간의 값이 없으면 Stop
				}
				// Main Rebar : Bottom				
				for(k=0 ; k<5 ; k++)
				{
					strBarNa1 = CpplData.strBotBarNa1_x[j][k];
					strBarNa2 = CpplData.strBotBarNa2_x[j][k];
					if(CpplData.dDb_x[j][k] > 0.0 && (CpplData.dBotBarNum_x[j][k] > 0.0 || CpplData.dBotBarCTC_x[j][k] > 0.0) && strBarNa1 != _T(""))
					{
						// Dt
						arTotalDb[k]  += 1.0 * CpplData.dDb_x[j][k];
						arWeightDb[k] += 1.0;
						// Num
						if(CpplData.dBotBarNum_x[j][k] > 0.0)
						{
							arTotalNumBot[k]  += 1.0 * CpplData.dBotBarNum_x[j][k];
							arWeightNumBot[k] += 1.0;
						}
						// CTC
						if(CpplData.dBotBarCTC_x[j][k] > 0.0)
						{
							arTotalCTCBot[k]  += 1.0 * CpplData.dBotBarCTC_x[j][k];
							arWeightCTCBot[k] += 1.0;
						}
						// BarNa						
						if(strBarNa1 != _T("") && strBarNa2 != _T("")) dWeightBar_Delta = 0.5;
						else                                           dWeightBar_Delta = 1.0;
						// - BarNa1
						if(strBarNa1 != _T(""))
						{
							if(!arWeightBarNaBot[k].Lookup(strBarNa1, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaBot[k].SetAt(strBarNa1, dWeightBar);
						}					
						// - BarNa2
						if(strBarNa1 != _T("") && strBarNa2 != _T(""))
						{
							if(!arWeightBarNaBot[k].Lookup(strBarNa2, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaBot[k].SetAt(strBarNa2, dWeightBar);
						}					
					}
					else
						break;//바깥쪽 레이어부터 검토하여 Dt가 입력된 곳까지 검토함
				}			
			}			
		}
	}
	else
	{// i:Elem Count   j:Node Count   k:Layer Count
		for(int i=0 ; i<nElemListSize ; i++)
		{
			if(!pDoc->m_pAttrCtrl->GetCppl(RcplData.arElemK.GetAt(i), CpplData)) continue;
			if(CpplData.bNum) nNumCount++;
			else              nCTCCount++;

			for(j=0 ; j<4 ; j++)
			{
				// Shear Rebar
				strBarNa1 = CpplData.strSubBarNa_y[j];
				if(CpplData.dSubBarNum_y[j] != 0.0 && CpplData.dSubBarDist_y[j] != 0.0 && strBarNa1 != _T(""))
				{
					dWeightBar_Delta = 1.0;
					dTotalSubBarNum   += 1.0 * CpplData.dSubBarNum_y[j];
					dWeightSubBarNum  += 1.0;
					dTotalSubBarDist  += 1.0 * CpplData.dSubBarDist_y[j];
					dWeightSubBarDist += 1.0;
					if(strBarNa1 != _T(""))
					{
						if(!arWeightSubBarNa.Lookup(strBarNa1, dWeightBar))  dWeightBar = 0.0;
						dWeightBar += dWeightBar_Delta;
						arWeightSubBarNa.SetAt(strBarNa1, dWeightBar);
					}
				}
				// Main Rebar : Top
				for(k=0 ; k<5 ; k++)
				{
					strBarNa1 = CpplData.strTopBarNa1_y[j][k];
					strBarNa2 = CpplData.strTopBarNa2_y[j][k];
					if(CpplData.dDt_y[j][k] > 0.0 && (CpplData.dTopBarNum_y[j][k] > 0.0 || CpplData.dTopBarCTC_y[j][k] > 0.0) && strBarNa1 != _T(""))
					{
						// Dt
						arTotalDt[k]  += 1.0 * CpplData.dDt_y[j][k];
						arWeightDt[k] += 1.0;
						// Num
						if(CpplData.dTopBarNum_y[j][k] > 0.0)
						{
							arTotalNumTop[k]  += 1.0 * CpplData.dTopBarNum_y[j][k];
							arWeightNumTop[k] += 1.0;
						}
						// CTC
						if(CpplData.dTopBarCTC_y[j][k] > 0.0)
						{
							arTotalCTCTop[k]  += 1.0 * CpplData.dTopBarCTC_y[j][k];
							arWeightCTCTop[k] += 1.0;
						}
						// BarNa
						if(strBarNa1 != _T("") && strBarNa2 != _T("")) dWeightBar_Delta = 0.5;
						else                                           dWeightBar_Delta = 1.0;
						// - BarNa1
						if(strBarNa1 != _T(""))
						{
							if(!arWeightBarNaTop[k].Lookup(strBarNa1, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaTop[k].SetAt(strBarNa1, dWeightBar);
						}					
						// - BarNa2
						if(strBarNa1 != _T("") && strBarNa2 != _T(""))
						{
							if(!arWeightBarNaTop[k].Lookup(strBarNa2, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaTop[k].SetAt(strBarNa2, dWeightBar);
						}					
					}
					else
						break;//바깥쪽 레이어부터 검토하여 Dt가 입력된 곳까지 검토함. 중간의 값이 없으면 Stop
				}
				// Main Rebar : Bottom
				for(k=0 ; k<5 ; k++)
				{
					strBarNa1 = CpplData.strBotBarNa1_y[j][k];
					strBarNa2 = CpplData.strBotBarNa2_y[j][k];
					if(CpplData.dDb_y[j][k] > 0.0 && (CpplData.dBotBarNum_y[j][k] > 0.0 || CpplData.dBotBarCTC_y[j][k] > 0.0) && strBarNa1 != _T(""))
					{
						// Dt
						arTotalDb[k]  += 1.0 * CpplData.dDb_y[j][k];
						arWeightDb[k] += 1.0;
						// Num
						if(CpplData.dBotBarNum_y[j][k] > 0.0)
						{
							arTotalNumBot[k]  += 1.0 * CpplData.dBotBarNum_y[j][k];
							arWeightNumBot[k] += 1.0;
						}
						// CTC
						if(CpplData.dBotBarCTC_y[j][k] > 0.0)
						{
							arTotalCTCBot[k]  += 1.0 * CpplData.dBotBarCTC_y[j][k];
							arWeightCTCBot[k] += 1.0;
						}
						// BarNa
						if(strBarNa1 != _T("") && strBarNa2 != _T("")) dWeightBar_Delta = 0.5;
						else                                           dWeightBar_Delta = 1.0;
						// - BarNa1
						if(strBarNa1 != _T(""))
						{
							if(!arWeightBarNaBot[k].Lookup(strBarNa1, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaBot[k].SetAt(strBarNa1, dWeightBar);
						}					
						// - BarNa2
						if(strBarNa1 != _T("") && strBarNa2 != _T(""))
						{
							if(!arWeightBarNaBot[k].Lookup(strBarNa2, dWeightBar))  dWeightBar = 0.0;
							dWeightBar += dWeightBar_Delta;
							arWeightBarNaBot[k].SetAt(strBarNa2, dWeightBar);
						}					
					}
					else
						break;//바깥쪽 레이어부터 검토하여 Dt가 입력된 곳까지 검토함
				}			
			}			
		}
	}

	CString strIndex;
	double dWeightBar1, dWeightBar2;
	POSITION Pos;
	// Average Rebar Data /////////////////////////////////////////////////////
	DgncCpplD.bNum = (nNumCount > nCTCCount); 
	// Shear Rebar
	DgncCpplD.dSubBarNum  = (dWeightSubBarNum  == 0.0) ? 0.0 : dTotalSubBarNum /dWeightSubBarNum ;
	DgncCpplD.dSubBarDist = (dWeightSubBarDist == 0.0) ? 0.0 : dTotalSubBarDist/dWeightSubBarDist;
	strBarNa1   = _T("");
	dWeightBar1 = 0.0;
	Pos = arWeightSubBarNa.GetStartPosition();
	while(Pos)
	{			
		arWeightSubBarNa.GetNextAssoc(Pos,strIndex,dWeightBar);
		if(dWeightBar > dWeightBar1)
		{
			strBarNa1 = strIndex;
			dWeightBar1 = dWeightBar;
		}
	}		
	DgncCpplD.strSubBarNa = strBarNa1;
	
	// Main Rebar
	for(i=0 ; i<5 ; i++)
	{
		DgncCpplD.dDt[i]           = (arWeightDt[i]     == 0.0) ? 0.0 : arTotalDt[i]     /arWeightDt[i]   ;
		DgncCpplD.dDb[i]           = (arWeightDb[i]     == 0.0) ? 0.0 : arTotalDb[i]     /arWeightDb[i]   ;
		DgncCpplD.dTopBarNum[i]    = (arWeightNumTop[i] == 0.0) ? 0.0 : arTotalNumTop[i]/arWeightNumTop[i];
		DgncCpplD.dBotBarNum[i]    = (arWeightNumBot[i] == 0.0) ? 0.0 : arTotalNumBot[i]/arWeightNumBot[i];
		DgncCpplD.dTopBarCTC[i]    = (arWeightCTCTop[i] == 0.0) ? 0.0 : arTotalCTCTop[i]/arWeightCTCTop[i];
		DgncCpplD.dBotBarCTC[i]    = (arWeightCTCBot[i] == 0.0) ? 0.0 : arTotalCTCBot[i]/arWeightCTCBot[i];
		

		strBarNa1   = strBarNa2   = _T("");
		dWeightBar1 = dWeightBar2 = 0.0;
		Pos = arWeightBarNaTop[i].GetStartPosition();
		while(Pos)
		{			
			arWeightBarNaTop[i].GetNextAssoc(Pos,strIndex,dWeightBar);
			if(dWeightBar > dWeightBar1)
			{
				strBarNa2 = strBarNa1;
				strBarNa1 = strIndex;
				dWeightBar2 = dWeightBar1;
				dWeightBar1 = dWeightBar;
			}
			else if(dWeightBar > dWeightBar2)
			{
				strBarNa2 = strIndex;
				dWeightBar2 = dWeightBar;
			}
		}		
		if(strBarNa1 != _T("") && strBarNa2 != _T(""))
		{			
			if(strBarNa1.GetLength() < strBarNa2.GetLength())
			{
				CString strTemp = strBarNa1;
				strBarNa1 = strBarNa2;
				strBarNa2 = strTemp;
			}
			else if(strBarNa1.GetLength() == strBarNa2.GetLength())
			{
				CString* pStrBarNa = new CString[2];
				pStrBarNa[0] = strBarNa1;
				pStrBarNa[1] = strBarNa2;
				CQSort::QSortString(pStrBarNa, 2);
				strBarNa1 = pStrBarNa[1];
				strBarNa2 = pStrBarNa[0];
				delete[] pStrBarNa;			
			}
		}
		DgncCpplD.strTopBarNa1[i] = strBarNa1;
		DgncCpplD.strTopBarNa2[i] = strBarNa2;

		strBarNa1   = strBarNa2   = _T("");
		dWeightBar1 = dWeightBar2 = 0.0;
		Pos = arWeightBarNaBot[i].GetStartPosition();
		while(Pos)
		{			
			arWeightBarNaBot[i].GetNextAssoc(Pos,strIndex,dWeightBar);
			if(dWeightBar > dWeightBar1)
			{
				strBarNa2 = strBarNa1;
				strBarNa1 = strIndex;
				dWeightBar2 = dWeightBar1;
				dWeightBar1 = dWeightBar;
			}
			else if(dWeightBar > dWeightBar2)
			{
				strBarNa2 = strIndex;
				dWeightBar2 = dWeightBar;
			}
		}		
		if(strBarNa1 != _T("") && strBarNa2 != _T(""))
		{			
			if(strBarNa1.GetLength() < strBarNa2.GetLength())
			{
				CString strTemp = strBarNa1;
				strBarNa1 = strBarNa2;
				strBarNa2 = strTemp;
			}
			else if(strBarNa1.GetLength() == strBarNa2.GetLength())
			{
				CString* pStrBarNa = new CString[2];
				pStrBarNa[0] = strBarNa1;
				pStrBarNa[1] = strBarNa2;
				CQSort::QSortString(pStrBarNa, 2);
				strBarNa1 = pStrBarNa[1];
				strBarNa2 = pStrBarNa[0];
				delete[] pStrBarNa;			
			}
		}
		DgncCpplD.strBotBarNa1[i] = strBarNa1;
		DgncCpplD.strBotBarNa2[i] = strBarNa2;			
	}
	return TRUE;
}

BOOL CDgnSectionTool::Get_DgncCpplDataList(CArray<T_RCPL_K, T_RCPL_K>& arRcplKey, CArray<_DGNC_CPPL_D, _DGNC_CPPL_D&>& arDgncCpplD)
{	
	arDgncCpplD.RemoveAll();
	int nSize = arRcplKey.GetSize();
	if(nSize <= 0) return FALSE;
	
	arDgncCpplD.SetSize(nSize);
	_DGNC_CPPL_D DgncCpplD;
	for(int i=0 ; i<nSize ; i++)
	{
		arDgncCpplD[i].Initialize();
		Get_DgncCpplData(arRcplKey.GetAt(i), DgncCpplD);
		arDgncCpplD.SetAt(i, DgncCpplD);
	}
	return TRUE;
}


BOOL CDgnSectionTool::Get_DgncCpplSingleData(T_RCPL_K RcplKey, BOOL bTop, _DGNC_CPPL_SINGLE_D& DgncCpplSingleD)
{
	BOOL bChk = FALSE;
	_DGNC_CPPL_D DgncCpplD;	
	bChk = Get_DgncCpplData(RcplKey, DgncCpplD);
	DgncCpplD.Get_Single(bTop, DgncCpplSingleD);
	return bChk;
}

BOOL CDgnSectionTool::Get_DgncCpplSingleDataList(CArray<T_RCPL_K, T_RCPL_K>& arRcplKey, CArray<BOOL, BOOL>& arbTop, CArray<_DGNC_CPPL_SINGLE_D, _DGNC_CPPL_SINGLE_D&>& arDgncCpplSingleD)
{
	arDgncCpplSingleD.RemoveAll();
	int nKeySize  = arRcplKey.GetSize();
	int nbTopSize = arbTop.GetSize();
	if(nKeySize <= 0 || nKeySize != nbTopSize) return FALSE;
	arDgncCpplSingleD.SetSize(nKeySize);
	_DGNC_CPPL_SINGLE_D DgncCpplSingleD;
	for(int i=0 ; i<nKeySize ; i++)
	{
		arDgncCpplSingleD[i].Initialize();
		Get_DgncCpplSingleData(arRcplKey.GetAt(i), arbTop.GetAt(i), DgncCpplSingleD);
		arDgncCpplSingleD.SetAt(i, DgncCpplSingleD);
	}	
	return TRUE;
}
