// DB_JUDResultEngine.cpp: implementation of the CDB_JUDResultEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "wg_db.h"
#include "DB_JUDResultEngine.h"
#include "DB_JUDResultCtrl.h"
#include "DesignResult.h"

#include "wg_db_DBDoc.h"
#include "wg_db_AttrCtrl.h"
#include "wg_db_PostCtrl.h"
#include "wg_db_MathFunc.h"

#include "..\wg_base\wg_base_Msgdll.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define		BFORC_COMPONENT_FX			0
#define		BFORC_COMPONENT_FY			1
#define		BFORC_COMPONENT_FZ			2
#define		BFORC_COMPONENT_MX			3
#define		BFORC_COMPONENT_MY			4
#define		BFORC_COMPONENT_MZ			5

#define		BFORC_OUTPUT_INODE			0
#define		BFORC_OUTPUT_JNODE			1
#define		BFORC_OUTPUT_CENTER			2
#define		BFORC_OUTPUT_MAX			  3 // Abs Max

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDB_JUDResultEngine::CDB_JUDResultEngine(CWnd *pParent/*=NULL*/)
{  
	m_pDoc = CDBDoc::GetDocPoint();  
	m_pResultCtrl = NULL;

	m_mapForceData.InitHashTable(HASHSIZEELEM);
	m_mapReacData.InitHashTable(HASHSIZENODE);
	m_mapDispData.InitHashTable(HASHSIZENODE);
	ResetEngine();
}

CDB_JUDResultEngine::~CDB_JUDResultEngine()
{
	
}

void CDB_JUDResultEngine::SetResultCtrl(CDB_JUDResultCtrl* pResultCtrl)
{
	m_pResultCtrl = pResultCtrl;
}

BOOL CDB_JUDResultEngine::SelectLoad(UINT LoadType, UINT LoadKey)
{
	m_bLoadDataSet = FALSE;

	if(!m_pDoc->IsPostMode()) return FALSE;  
	
	T_LCOM_D LcomD; LcomD.Initialize();
	if(!m_pDoc->m_pPostCtrl->MakeLoadComb(LoadType, LoadKey, LcomD)) return FALSE;
	m_pDoc->m_pPostCtrl->SelectLoadCombNew(LcomD);
		
	m_bLoadDataSet = TRUE;
	return TRUE;
}

int CDB_JUDResultEngine::GetResultField()
{
	return m_nResultField;
}

BOOL CDB_JUDResultEngine::InitEngine(int nMode, int nResultField, CArray<int,int>& aResultMode)
{
	m_aElemKeyList.RemoveAll();
	m_bResultDataSet = FALSE;
	if(!m_bLoadDataSet) return FALSE;
	
	if(!m_pResultCtrl->GetElemKeyList(nMode, m_aElemKeyList)) {ASSERT(0); return FALSE;}
	if(!m_pResultCtrl->GetNodeKeyList(nMode, m_aNodeKeyList)) {ASSERT(0); return FALSE;}
	
	BOOL bDiff = FALSE;
	if(m_nResultField != nResultField)
	{
		m_nResultField = nResultField;
		bDiff = TRUE;
	}
	m_aResultMode.RemoveAll();
	m_aResultMode.Copy(aResultMode);
	
	if(bDiff)
	{
	switch (nResultField) 
	{
		case D_RESULT_ANAL_FORCE_FIELD:
			MakeForceData();
			break; 
		case D_RESULT_ANAL_REAC_FIELD:
			MakeReacData();
			break; 
		case D_RESULT_ANAL_DISP_FIELD:
			MakeDispData();
			break;            
		case D_RESULT_RC_SECT_DGN_STRS_FIELD  : 
		case D_RESULT_RC_SECT_ALLOW_STRS_FIELD:
		case D_RESULT_RC_SECT_TOL_FIELD       :
		case D_RESULT_RC_SECT_ASRATIO_FILED   :
		case D_RESULT_RC_SECT_CONNECT_FIELD   :
		case D_RESULT_RC_SECT_STICK_FIELD     :
		case D_RESULT_RC_SECT_PARAM_FIELD     :
			MakeRCSectData();
			break; 
		/*
		case D_RESULT_SECT_ALLOW_STRS_FIELD:
			MakeAllowData();
			break; 
		*/  
		default:
			ASSERT(0);
			break;
		}      
	}    
	
	return TRUE;
}

BOOL CDB_JUDResultEngine::MakeRCSectData()
{
	T_ELEM_K elem_key ;  
	T_LCOM_D LcomD = m_pDoc->m_pPostCtrl->GetLoadComb();
	T_LCOM_K LcomK = m_pDoc->m_pAttrCtrl->GetLcomKey(LcomD.nLcomType, LcomD.LoadCombName);
	T_JUD_DGN_FORC_D JudDgnForcD;
	for (int i=0; i<m_aElemKeyList.GetSize(); i++)
	{ 
		elem_key = m_aElemKeyList[i];    
		JudDgnForcD.Initialize();
		
		int nMembType = GetMembType(elem_key);
		if(nMembType == 1 || nMembType == 3)
		{
			T_RCOL_D rcol_d; rcol_d.Initialize();
			if(!m_pDoc->m_pPostCtrl->GetDesignResult()->ReadConcreteDesignResultColumn(elem_key, LcomK, TRUE, 0, rcol_d)) {ASSERT(0); continue;}
			if(!rcol_d.bCheck) {ASSERT(0); continue;}
				
			JudDgnForcD.dForce_N = rcol_d.res_dPu;

			JudDgnForcD.dForce_M[0] = rcol_d.res_dMcy;        
			JudDgnForcD.dForce_M[1] = rcol_d.res_dMcz;        
			
			JudDgnForcD.dForce_Q[0] = rcol_d.res_dVu;        
			
			if(rcol_d.dblCodeRatio[0] != 0.0) JudDgnForcD.dForce_Na = rcol_d.res_dPu / rcol_d.dblCodeRatio[0];

			if(rcol_d.dblCodeRatio[3] != 0.0) JudDgnForcD.dForce_Ma[0] = rcol_d.res_dMcy / rcol_d.dblCodeRatio[3];
			if(rcol_d.dblCodeRatio[4] != 0.0) JudDgnForcD.dForce_Ma[1] = rcol_d.res_dMcz / rcol_d.dblCodeRatio[4];
			
			double dRatio = max(rcol_d.dblCodeRatio[1], rcol_d.dblCodeRatio[2]);      
			if(dRatio != 0.0) JudDgnForcD.dForce_Qa[0] = rcol_d.res_dVu / dRatio;         
			
			JudDgnForcD.dAsRatio_V[0] = rcol_d.res_dAsV;        
			JudDgnForcD.dAsRatio_H[0] = rcol_d.res_dAsH;        
			
			JudDgnForcD.dTAUa[0] = rcol_d.res_dAdhtay;      
			JudDgnForcD.dTAUa[1] = rcol_d.res_dAdhtaz;        
			
			JudDgnForcD.dFa[0] = rcol_d.res_dAdhfay;              
			JudDgnForcD.dFa[1] = rcol_d.res_dAdhfaz;              
		}	
		else if(nMembType == 2)
		{
			T_FBEM_D fbem_d; fbem_d.Initialize();      
			T_RBEM_D rbem_d; rbem_d.Initialize();

			if(LcomK == 0) // Max
			{
				if(!m_pDoc->m_pPostCtrl->GetDesignResult()->ReadConcreteDesignResultBeam(elem_key, rbem_d)) {ASSERT(0); continue;}
				if(!rbem_d.bCheck) {ASSERT(0); continue;}
				
				JudDgnForcD.dForce_M[0] = rbem_d.Result[0].res_dNMu;        
				JudDgnForcD.dForce_M[1] = rbem_d.Result[1].res_dNMu;        
				JudDgnForcD.dForce_M[2] = rbem_d.Result[2].res_dNMu;    
				JudDgnForcD.dForce_M[3] = rbem_d.Result[0].res_dPMu;        
				JudDgnForcD.dForce_M[4] = rbem_d.Result[1].res_dPMu;
				JudDgnForcD.dForce_M[5] = rbem_d.Result[2].res_dPMu;
				
				JudDgnForcD.dForce_Q[0] = rbem_d.Result[0].res_dVu;        
				JudDgnForcD.dForce_Q[1] = rbem_d.Result[1].res_dVu;        
				JudDgnForcD.dForce_Q[2] = rbem_d.Result[2].res_dVu;                
			}
			else
			{        
				if(!m_pDoc->m_pPostCtrl->GetDesignResult()->ReadConcreteDesignForceBeam(elem_key, fbem_d)) {ASSERT(0); continue;}        
				if(!fbem_d.bCheck) {ASSERT(0); continue;}
				
				BOOL bFind = FALSE;
				int nIndex;        
				int nSize = fbem_d.FbemBase[0].GetSize();        
				for (int j=0; j<nSize; j++)
				{
					T_FBEM_BASE findData = fbem_d.FbemBase[0].GetAt(j);
					if(findData.LcomK == LcomK) 
					{
						nIndex = j; 
						bFind = TRUE;
						break; 
					}          
				}
				if(!bFind) {ASSERT(0); continue;}
				
				T_FBEM_BASE fbem_base[3];
				fbem_base[0] = fbem_d.FbemBase[0].GetAt(nIndex);
				fbem_base[1] = fbem_d.FbemBase[2].GetAt(nIndex);
				fbem_base[2] = fbem_d.FbemBase[4].GetAt(nIndex);
				
				if(!fbem_base[0].bCheck || !fbem_base[1].bCheck || !fbem_base[2].bCheck) {ASSERT(0); continue;}
				
				JudDgnForcD.dForce_M[0] = fbem_base[0].dNMuy;                
				JudDgnForcD.dForce_M[1] = fbem_base[1].dNMuy;        
				JudDgnForcD.dForce_M[2] = fbem_base[2].dNMuy;    
				JudDgnForcD.dForce_M[3] = fbem_base[0].dPMuy;        
				JudDgnForcD.dForce_M[4] = fbem_base[1].dPMuy;
				JudDgnForcD.dForce_M[5] = fbem_base[2].dPMuy;
				
				JudDgnForcD.dForce_Q[0] = fbem_base[0].dFzz;        
				JudDgnForcD.dForce_Q[1] = fbem_base[1].dFzz;        
				JudDgnForcD.dForce_Q[2] = fbem_base[2].dFzz;                
			}
			
			JudDgnForcD.dForce_Ma[0] = rbem_d.Result[0].res_dpNMn;        
			JudDgnForcD.dForce_Ma[1] = rbem_d.Result[1].res_dpNMn;        
			JudDgnForcD.dForce_Ma[2] = rbem_d.Result[2].res_dpNMn;    
			JudDgnForcD.dForce_Ma[3] = rbem_d.Result[0].res_dpPMn;        
			JudDgnForcD.dForce_Ma[4] = rbem_d.Result[1].res_dpPMn;
			JudDgnForcD.dForce_Ma[5] = rbem_d.Result[2].res_dpPMn;
			
			JudDgnForcD.dForce_Qa[0] = rbem_d.Result[0].res_dpVc * rbem_d.Result[0].res_dpVs;        
			JudDgnForcD.dForce_Qa[1] = rbem_d.Result[1].res_dpVc * rbem_d.Result[1].res_dpVs;      
			JudDgnForcD.dForce_Qa[2] = rbem_d.Result[2].res_dpVc * rbem_d.Result[2].res_dpVs;               
			
			JudDgnForcD.dAsRatio_V[0] = rbem_d.Result[0].res_dAsTop;        
			JudDgnForcD.dAsRatio_V[1] = rbem_d.Result[1].res_dAsTop;        
			JudDgnForcD.dAsRatio_V[2] = rbem_d.Result[2].res_dAsTop;    
			JudDgnForcD.dAsRatio_V[3] = rbem_d.Result[0].res_dAsBot;        
			JudDgnForcD.dAsRatio_V[4] = rbem_d.Result[1].res_dAsBot;
			JudDgnForcD.dAsRatio_V[5] = rbem_d.Result[2].res_dAsBot;
			
			JudDgnForcD.dTAUa[0] = rbem_d.Result[0].res_dTopta;        
			JudDgnForcD.dTAUa[1] = rbem_d.Result[1].res_dTopta;        
			JudDgnForcD.dTAUa[2] = rbem_d.Result[2].res_dTopta;    
			JudDgnForcD.dTAUa[3] = rbem_d.Result[0].res_dBotta;        
			JudDgnForcD.dTAUa[4] = rbem_d.Result[1].res_dBotta;
			JudDgnForcD.dTAUa[5] = rbem_d.Result[2].res_dBotta;    
			
			JudDgnForcD.dFa[0] = rbem_d.Result[0].res_dTopfa;        
			JudDgnForcD.dFa[1] = rbem_d.Result[1].res_dTopfa;        
			JudDgnForcD.dFa[2] = rbem_d.Result[2].res_dTopfa;    
			JudDgnForcD.dFa[3] = rbem_d.Result[0].res_dBotfa;        
			JudDgnForcD.dFa[4] = rbem_d.Result[1].res_dBotfa;
			JudDgnForcD.dFa[5] = rbem_d.Result[2].res_dBotfa;        
		}	    
		else if(nMembType == 4)
		{
			T_RWAL_D rwal_d; rwal_d.Initialize();
		 
			UINT iInternalWallID;
			if(!m_pDoc->m_pPostCtrl->GetInternalWallId(elem_key, iInternalWallID)) {ASSERT(0); continue;}
			if(!m_pDoc->m_pPostCtrl->GetDesignResult()->ReadConcreteDesignResultWall(iInternalWallID, LcomK, FALSE, 0, rwal_d)) {ASSERT(0); continue;}
			if(!rwal_d.bCheck) {ASSERT(0); continue;}
			
			JudDgnForcD.dForce_N = rwal_d.res_dPu;
			
			JudDgnForcD.dForce_M[0] = rwal_d.res_dMcy;        
			JudDgnForcD.dForce_M[3] = rwal_d.res_dMcz;        
			
			JudDgnForcD.dForce_Q[0] = rwal_d.res_dVu;        
			
			double dRatio = max(rwal_d.res_dRatPy, rwal_d.res_dRatPz);
			if(dRatio != 0.0) JudDgnForcD.dForce_Na = rwal_d.res_dPu / dRatio;      
			
			if(rwal_d.dblCodeRatio[3] != 0.0) JudDgnForcD.dForce_Ma[0] = rwal_d.res_dMcy / rwal_d.dblCodeRatio[3];
			if(rwal_d.dblCodeRatio[4] != 0.0) JudDgnForcD.dForce_Ma[3] = rwal_d.res_dMcz / rwal_d.dblCodeRatio[4];
			
			dRatio = max(rwal_d.dblCodeRatio[1], rwal_d.dblCodeRatio[2]);      
			if(dRatio != 0.0) JudDgnForcD.dForce_Qa[0] = rwal_d.res_dVu / dRatio;         
			
			JudDgnForcD.dAsRatio_V[0] = rwal_d.res_dAsV;        
			JudDgnForcD.dAsRatio_H[0] = rwal_d.res_dAsH;              
		}
		else
			continue;
		
		m_mapDgnForceData.SetAt(elem_key, JudDgnForcD);	
	}
	
	m_bResultDataSet = TRUE;
	return TRUE;
}

BOOL CDB_JUDResultEngine::MakeDispData()
{
	T_NODE_K node_key;
	T_DISP_D disp_d;	
	
	T_JUD_DISP_D JudDispD;
	for (int i=0; i<m_aNodeKeyList.GetSize(); i++)
	{ 
		node_key = m_aNodeKeyList[i];
		disp_d.Initialize();   
		JudDispD.Initialize();
		
		if(!m_pDoc->m_pPostCtrl->GetDispNew(node_key, &disp_d, NULL, NULL)) {ASSERT(0); continue;}    
		for (int j=0; j<2; j++) JudDispD.dDisp[j] = disp_d.dblDisp[j];
		m_mapDispData.SetAt(node_key, JudDispD);	
	}  
	
	m_bResultDataSet = TRUE;
	return TRUE;
}

BOOL CDB_JUDResultEngine::MakeReacData()
{
	T_NODE_K node_key;
	T_REAC_D reac_d;	
	
	T_JUD_REAC_D JudReacD;
	for (int i=0; i<m_aNodeKeyList.GetSize(); i++)
	{ 
		node_key = m_aNodeKeyList[i];
		reac_d.Initialize();   
		JudReacD.Initialize();

		if(!m_pDoc->m_pPostCtrl->GetReacNew(node_key, &reac_d, NULL, NULL)) {ASSERT(0); continue;}    
		JudReacD.dReac_N = reac_d.dblReac[2];
		JudReacD.dReac_M[0] = reac_d.dblReac[3]; // Mx
		JudReacD.dReac_M[1] = reac_d.dblReac[4]; // My    
		JudReacD.dReac_Q[0] = reac_d.dblReac[0]; // Fx
		JudReacD.dReac_Q[1] = reac_d.dblReac[1]; // Fy

		if(JudReacD.IsZero()) continue;       
		m_mapReacData.SetAt(node_key, JudReacD);	
	}  
	
	m_bResultDataSet = TRUE;
	return TRUE;
}

BOOL CDB_JUDResultEngine::MakeForceData()
{
	T_ELEM_K elem_key ;
	T_STRB_D beam_d ;  
	T_STRT_D brace_d;  
	
	T_JUD_FORC_D JudForcD;
	for (int i=0; i<m_aElemKeyList.GetSize(); i++)
	{ 
		elem_key = m_aElemKeyList[i];
		beam_d.Initialize();
		brace_d.Initialize();
		JudForcD.Initialize();
		
		int nMembType = GetMembType(elem_key);
		if(nMembType == 1 || nMembType== 2)
		{
			if(!m_pDoc->m_pPostCtrl->GetStrbNew(elem_key, &beam_d, NULL, NULL, TRUE)) { ASSERT(0); return FALSE; }      
			JudForcD.dForce_N = CalcBForcValue(beam_d, BFORC_COMPONENT_FX, BFORC_OUTPUT_MAX);
			JudForcD.dForce_MY[0] = CalcBForcValue(beam_d, BFORC_COMPONENT_MY, BFORC_OUTPUT_INODE);
			JudForcD.dForce_MY[1] = CalcBForcValue(beam_d, BFORC_COMPONENT_MY, BFORC_OUTPUT_CENTER);
			JudForcD.dForce_MY[2] = CalcBForcValue(beam_d, BFORC_COMPONENT_MY, BFORC_OUTPUT_JNODE);    
			JudForcD.dForce_MZ[0] = CalcBForcValue(beam_d, BFORC_COMPONENT_MZ, BFORC_OUTPUT_INODE);
			JudForcD.dForce_MZ[1] = CalcBForcValue(beam_d, BFORC_COMPONENT_MZ, BFORC_OUTPUT_CENTER);
			JudForcD.dForce_MZ[2] = CalcBForcValue(beam_d, BFORC_COMPONENT_MZ, BFORC_OUTPUT_JNODE);    
			JudForcD.dForce_FY[0] = CalcBForcValue(beam_d, BFORC_COMPONENT_FY, BFORC_OUTPUT_INODE);
			JudForcD.dForce_FY[1] = CalcBForcValue(beam_d, BFORC_COMPONENT_FY, BFORC_OUTPUT_CENTER);
			JudForcD.dForce_FY[2] = CalcBForcValue(beam_d, BFORC_COMPONENT_FY, BFORC_OUTPUT_JNODE);    
			JudForcD.dForce_FZ[0] = CalcBForcValue(beam_d, BFORC_COMPONENT_FZ, BFORC_OUTPUT_INODE);
			JudForcD.dForce_FZ[1] = CalcBForcValue(beam_d, BFORC_COMPONENT_FZ, BFORC_OUTPUT_CENTER);
			JudForcD.dForce_FZ[2] = CalcBForcValue(beam_d, BFORC_COMPONENT_FZ, BFORC_OUTPUT_JNODE);    
		}	
		else if(nMembType == 3)
		{
			if(!m_pDoc->m_pPostCtrl->GetStrtNew(elem_key, &brace_d, NULL, NULL)) { ASSERT(0); return FALSE; }

		}	
		else
			continue;

		m_mapForceData.SetAt(elem_key, JudForcD);	
	}
	
	m_bResultDataSet = TRUE;
	return TRUE;
}

void CDB_JUDResultEngine::ResetEngine()
{	
	m_bResultDataSet = FALSE;  
	m_bLoadDataSet   = FALSE;	
	
	m_mapForceData.RemoveAll();
	m_mapReacData.RemoveAll();
	m_mapDispData.RemoveAll();

	m_nResultField = -1;
	m_aResultMode.RemoveAll();
}

double CDB_JUDResultEngine::CalcBForcValue(T_STRB_D &bforc_d, int component, int part, int node)
{
	double	len;
	
	switch (component) {
		case BFORC_COMPONENT_FX:
			len = bforc_d.dblForce[part][node*6+0];
			break;
		case BFORC_COMPONENT_FY:
			len = bforc_d.dblForce[part][node*6+1];
			break;
		case BFORC_COMPONENT_FZ:
			len = bforc_d.dblForce[part][node*6+2];
			break;
		case BFORC_COMPONENT_MX:
			len = bforc_d.dblForce[part][node*6+3];
			break;
		case BFORC_COMPONENT_MY:
			len = bforc_d.dblForce[part][node*6+4];
			break;
		case BFORC_COMPONENT_MZ:
			len = bforc_d.dblForce[part][node*6+5];
			break;
	}
	
	return len;
}

double CDB_JUDResultEngine::GetMaxElemBForc(T_STRB_D &bforc_d, int component)
{
	double len, max=-DBL_MAX;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<2;j++) 
		{ 
			len = CalcBForcValue(bforc_d, component, i, j);
			if (max < len)       
				max = len;
		}
	}
	return max;
}

double CDB_JUDResultEngine::GetMinElemBForc(T_STRB_D &bforc_d, int component)
{
	double len, min=DBL_MAX;
	for(int i=0;i<4;i++) 
	{
		for(int j=0;j<2;j++) 
		{
			len = CalcBForcValue(bforc_d, component, i, j);
			if(min > len)       
				min = len;
		}
	}  
	return min;
}

double CDB_JUDResultEngine::CalcBForcValue(T_STRB_D &bforc_d, int component, int out_sec)
{
	double len=0.0, min, max;
	
	switch(out_sec) 
	{
	case BFORC_OUTPUT_INODE:
		len = CalcBForcValue(bforc_d, component, 0, BFORC_OUTPUT_INODE);
		break;
	case BFORC_OUTPUT_JNODE:
		len = CalcBForcValue(bforc_d, component, 3, BFORC_OUTPUT_JNODE);
		break;
	case BFORC_OUTPUT_CENTER:
		len = CalcBForcValue(bforc_d, component, 1, BFORC_OUTPUT_JNODE);
		break;
	case BFORC_OUTPUT_MAX:
		min = GetMinElemBForc(bforc_d, component);
		max = GetMaxElemBForc(bforc_d, component);
		if ((-min) > max)
			len = min;
		else
			len = max;
		break;  
	}
	
	return len;
}


// 1=Column 2=Beam 3=Brace 4=Wall 
int CDB_JUDResultEngine::GetMembType(T_ELEM_K elemK)
{
	T_ELEM_D elem_d;
	if(!m_pDoc->m_pAttrCtrl->GetElem(elemK, elem_d)) {ASSERT(0); return 0;}
	if(m_pDoc->m_pAttrCtrl->IsWall(elem_d.eltyp)) {return 4;}
	
	int nMembType=0;  
	if(m_pDoc->m_pAttrCtrl->ExistMbtp(elemK))
	{
		T_MBTP_D mbtpD; mbtpD.Initialize();
		m_pDoc->m_pAttrCtrl->GetMbtp(elemK,mbtpD);
		nMembType = mbtpD.nMbType;        
	}
	else      
		nMembType = m_pDoc->m_pAttrCtrl->GetMemberType(elemK);
	
	return nMembType;
}