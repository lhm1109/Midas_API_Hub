// JUDResultEngine.h: interface for the CJUDResultEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DB_JUDRESULTENGINE_H__)
#define __DB_JUDRESULTENGINE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DB_ST_DT.h"

struct T_JUD_FORC_D
{           
	double dForce_N;     // Fx
	double dForce_MY[3]; // M : i,m,j
	double dForce_MZ[3];  
	double dForce_FY[3]; 
	double dForce_FZ[3];

	void Initialize()
	{
		dForce_N = 0.;   
		for (int i=0; i<3; i++) { dForce_MY[i] = dForce_MZ[i] = dForce_FY[i] = dForce_FZ[i] = 0.; }
	}                       
	
	T_JUD_FORC_D() { Initialize(); };
	T_JUD_FORC_D(T_JUD_FORC_D& src) { *this = src; }
	T_JUD_FORC_D& operator=(const T_JUD_FORC_D& src)
	{          
		dForce_N = src.dForce_N;   
		for (int i=0; i<3; i++)
		{ 
			dForce_MY[i] = src.dForce_MY[i];
			dForce_MZ[i] = src.dForce_MZ[i]; 
			dForce_FY[i] = src.dForce_FY[i];
			dForce_FZ[i] = src.dForce_FZ[i]; 
		}
		return *this;
	}	
};

struct T_JUD_DGN_FORC_D
{           
	double dForce_N;        
	double dForce_Na;  
	double dForce_M[6];   //0~2:Neg, 3~5:Pos  
	double dForce_Ma[6];  
	double dForce_Q[3];   //0:I, 1:J      
	double dForce_Qa[3];
	double dAsRatio_V[6]; //0~2:Top, 3~5:Bot
	double dAsRatio_H[6]; 
	double dTAUa[6];      //0~2:Top, 3~5:Bot  0:Fy, 3:Fz
	double dFa[6];        //0~2:Top, 3~5:Bot

	void Initialize()
	{
		dForce_N = 0.;
		dForce_Na = 0.;  
		for (int i=0; i<6; i++)
		{
			dForce_M[i] = dForce_Ma[i] = dAsRatio_V[i] = dAsRatio_H[i] = dTAUa[i] = dFa[i] = 0.;
		}
		for (i=0; i<3; i++) 
		{
			dForce_Q[i] = dForce_Qa[i] = 0.; 
		}
	}                       
	
	T_JUD_DGN_FORC_D() { Initialize(); };
	T_JUD_DGN_FORC_D(T_JUD_DGN_FORC_D& src) { *this = src; }
	T_JUD_DGN_FORC_D& operator=(const T_JUD_DGN_FORC_D& src)
	{          
		dForce_N  = src.dForce_N;   
		dForce_Na = src.dForce_Na;   
		for (int i=0; i<6; i++)
		{ 
			dForce_M[i]   = src.dForce_M[i]  ;
			dForce_Ma[i]  = src.dForce_Ma[i] ;
			dAsRatio_V[i] = src.dAsRatio_V[i];
			dAsRatio_H[i] = src.dAsRatio_H[i];      
			dTAUa[i]      = src.dTAUa[i]     ;
			dFa[i]        = src.dFa[i]       ;      
		}                                
		for (i=0; i<3; i++)
		{
			dForce_Q[i]  = src.dForce_Q[i] ;
			dForce_Qa[i] = src.dForce_Qa[i]; 
		}
		return *this;
	}	
};

struct T_JUD_REAC_D
{           
	double dReac_N;    // Fz
	double dReac_M[2]; // Mx, My : Y축열, X축열
	double dReac_Q[2]; // Fx, Fy
	
	void Initialize()
	{
		dReac_N = 0;      
		for (int i=0; i<2; i++) dReac_M[i] = dReac_Q[i] = 0.;    
	}                       
	
	T_JUD_REAC_D() { Initialize(); };
	T_JUD_REAC_D(T_JUD_REAC_D& src) { *this = src; }
	T_JUD_REAC_D& operator=(const T_JUD_REAC_D& src)
	{   
		dReac_N = src.dReac_N;      
		for (int i=0; i<2; i++) 
		{ 
			dReac_M[i] = src.dReac_M[i];
			dReac_Q[i] = src.dReac_Q[i];
		}
		return *this;
	}	

	BOOL IsZero()
	{
		if(dReac_N == 0.0)
		{
			for (int i=0; i<2; i++)
			{
				if(dReac_M[i] != 0.0 || dReac_Q[i] != 0.0)
					return FALSE;
			}
			return TRUE;
		}
		return FALSE;
	}
};

struct T_JUD_DISP_D
{           
	double dDisp[2]; // Dx, Dy
	
	void Initialize()
	{
		for (int i=0; i<2; i++) dDisp[i] = 0.;    
	}                       
	
	T_JUD_DISP_D() { Initialize(); };
	T_JUD_DISP_D(T_JUD_DISP_D& src) { *this = src; }
	T_JUD_DISP_D& operator=(const T_JUD_DISP_D& src)
	{   
		for (int i=0; i<2; i++) dDisp[i] = src.dDisp[i];
		return *this;
	}	  
};

#define D_RESULT_ANAL_FORCE_FIELD          100
#define D_RESULT_ANAL_FORCE_N              101
#define D_RESULT_ANAL_FORCE_M              102
#define D_RESULT_ANAL_FORCE_Q              103
																					 
#define D_RESULT_ANAL_REAC_FIELD           110
#define D_RESULT_ANAL_REAC_N               111
#define D_RESULT_ANAL_REAC_M               112
#define D_RESULT_ANAL_REAC_Q               113
																					 
#define D_RESULT_ANAL_DISP_FIELD           120
#define D_RESULT_ANAL_DISP                 121
#define D_RESULT_ANAL_DISP_RATIO           123
#define D_RESULT_ANAL_DISP_AMOUNT          124
																					 
#define D_RESULT_ANAL_STORY_SHEAR          130
																					 
#define D_RESULT_RC_SECT_DGN_STRS_FIELD    200
#define D_RESULT_RC_SECT_DGN_STRS_N        201
#define D_RESULT_RC_SECT_DGN_STRS_M        202
#define D_RESULT_RC_SECT_DGN_STRS_Q        203

#define D_RESULT_RC_SECT_ALLOW_STRS_FIELD  210
#define D_RESULT_RC_SECT_ALLOW_STRS_N      211
#define D_RESULT_RC_SECT_ALLOW_STRS_M      212
#define D_RESULT_RC_SECT_ALLOW_STRS_Q      213

#define D_RESULT_RC_SECT_TOL_FIELD         220
#define D_RESULT_RC_SECT_TOL_N             221
#define D_RESULT_RC_SECT_TOL_M             222
#define D_RESULT_RC_SECT_TOL_Q             223

#define D_RESULT_RC_SECT_ASRATIO_FILED     230
#define D_RESULT_RC_SECT_ASRATIO_VER       231
#define D_RESULT_RC_SECT_ASRATIO_HOR       232

#define D_RESULT_RC_SECT_CONNECT_FIELD     240 
#define D_RESULT_RC_SECT_CONNECT_QD        241
#define D_RESULT_RC_SECT_CONNECT_QA        242
#define D_RESULT_RC_SECT_CONNECT_TOL       243

#define D_RESULT_RC_SECT_STICK_FIELD       250
#define D_RESULT_RC_SECT_STICK_TAU         251
#define D_RESULT_RC_SECT_STICK_F           252
#define D_RESULT_RC_SECT_STICK_TOL         253

#define D_RESULT_RC_SECT_PARAM_FIELD       260 
#define D_RESULT_RC_SECT_PARAM_TAU         261 
#define D_RESULT_RC_SECT_PARAM_TAU_F       262
#define D_RESULT_RC_SECT_PARAM_SIGMA       263
#define D_RESULT_RC_SECT_PARAM_SIGMA_F     264
#define D_RESULT_RC_SECT_PARAM_M           265

#include "HeaderPre.h"
class CDBDoc;
class CDB_JUDResultCtrl;
class __MY_EXT_CLASS__ CDB_JUDResultEngine
{
public:	
	CDB_JUDResultEngine(CWnd* pParent=NULL);
	virtual ~CDB_JUDResultEngine();

public:
	BOOL m_bResultDataSet;
	BOOL m_bLoadDataSet;

	CArray<int,int> m_aResultMode;

protected:
	CDBDoc* m_pDoc;
	CDB_JUDResultCtrl* m_pResultCtrl;

	CArray<UINT,UINT> m_aElemKeyList;
	CArray<UINT,UINT> m_aNodeKeyList;
	
	CMapEx<UINT,UINT,T_JUD_FORC_D,T_JUD_FORC_D&> m_mapForceData;
	CMapEx<UINT,UINT,T_JUD_REAC_D,T_JUD_REAC_D&> m_mapReacData ;
	CMapEx<UINT,UINT,T_JUD_DISP_D,T_JUD_DISP_D&> m_mapDispData ;
	CMapEx<UINT,UINT,T_JUD_DGN_FORC_D,T_JUD_DGN_FORC_D&> m_mapDgnForceData;
	
protected:
	int m_nResultField;  

	BOOL MakeForceData();  
	BOOL MakeReacData();
	BOOL MakeDispData();  

	BOOL MakeRCSectData();

	double CalcBForcValue(T_STRB_D &bforc_d, int component, int out_sec);
	double CalcBForcValue(T_STRB_D &bforc_d, int component, int part, int node);
	double GetMaxElemBForc(T_STRB_D &bforc_d, int component);
	double GetMinElemBForc(T_STRB_D &bforc_d, int component);
	
public:
	void SetResultCtrl(CDB_JUDResultCtrl* pResultCtrl);
	void ResetEngine();
	BOOL InitEngine(int nMode, int nResultField, CArray<int,int>& aResultMode); // nMode : 0=Frame, 1=Story
	BOOL SelectLoad(UINT LoadType, UINT LoadKey);
	int  GetResultField();
	BOOL GetForceData(T_ELEM_K ElemK, T_JUD_FORC_D& JudForcD) { return m_mapForceData.Lookup(ElemK, JudForcD); }
	BOOL GetReacData (T_NODE_K NodeK, T_JUD_REAC_D& JudReacD) { return m_mapReacData.Lookup(NodeK, JudReacD); }
	BOOL GetDispData (T_NODE_K NodeK, T_JUD_DISP_D& JudDispD) { return m_mapDispData.Lookup(NodeK, JudDispD); }  
	BOOL GetDgnForceData (T_ELEM_K ElemK, T_JUD_DGN_FORC_D& JudDgnForcD) { return m_mapDgnForceData.Lookup(ElemK, JudDgnForcD); }  
	
	int GetMembType(T_ELEM_K elemK);
};
#include "HeaderPost.h"
#endif // !defined(AFX__DB_JUDRESULTENGINE_H__)
