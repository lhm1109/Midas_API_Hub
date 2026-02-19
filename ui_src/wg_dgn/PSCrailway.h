// PSCrailway.h: interface for the CPSCrailway class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSCRAILWAY_H__C9905C02_06CE_4BC9_BEB4_CE6FC1A15089__INCLUDED_)
#define AFX_PSCRAILWAY_H__C9905C02_06CE_4BC9_BEB4_CE6FC1A15089__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afxtempl.h>
//#include "..\MIT_Lib\rptostream.h"
#include "math.h"
#include "CRCFileBase.h"
#include "..\wg_db\DB_ST_DN_RESULT_CH.H"
#include "..\wg_db\TendonGrup.h"
#include "RCDataCtrl_CH.h"
#define PI 3.1415926

struct T_PSC_Degree_rail
{
    BOOL bIsBeam;
    UINT nElemK;
    UINT nPart;                  
    double dNp;
    double dNpey;
    double dNpez;
    _DGN_FORC_CRC aForce[12];
    _TB_PSC_SECT PscSect;

    void Initialize()
    {
        bIsBeam = TRUE;
        nElemK = 0;
        nPart = 0;
        dNp = dNpey = dNpez = 0.0;
        for(int i=0; i<12; i++)
            aForce[i].Initialize();
        PscSect.Initialize();    
    }  
};

struct T_Concrete_CS_rail
{
    BOOL bIsBeam;
    UINT nElemK;
    UINT nPart;                  //I-end, J-end
    double dalpha;
    double dfc;
    double dfct;
    CArray<_DGN_FORC_CRC,_DGN_FORC_CRC> aForceCS;
    _TB_PSC_SECT PscSect;

    void Initialize()
    {
        bIsBeam = TRUE;
        nElemK = 0;
        dalpha = dfc = dfct = 0.0;
        aForceCS.RemoveAll();
        PscSect.Initialize();    
    }  

	T_Concrete_CS_rail() {}
	T_Concrete_CS_rail(const T_Concrete_CS_rail& rData) {*this=rData;}
	T_Concrete_CS_rail& operator = (const T_Concrete_CS_rail& rData)
	{
        bIsBeam = rData.bIsBeam;
        nElemK  = rData.nElemK;
        nPart   = rData.nPart;
        dalpha  = rData.dalpha;
        dfc     = rData.dfc;
        dfct    = rData.dfct;
		aForceCS.Copy(rData.aForceCS);
        PscSect = rData.PscSect;
		return *this;
	}

};

struct T_Tendon_CS_rail
{
    BOOL bIsBeam;
	UINT iLoadType;
    UINT nTdnaK;
    UINT nElemK;
    double dfpk;
	CArray<T_TENDON_LOSS,T_TENDON_LOSS> aTendonLoss;
	CArray<INT,INT> aStag;
	CArray<INT,INT> aPart;
	CString TendonName;

    void Initialize()
    {
        bIsBeam = TRUE;
		iLoadType = 0;
        nTdnaK = 0;
		nElemK = 0;
        dfpk = 0.0;
        aTendonLoss.RemoveAll();
		aStag.RemoveAll();
		aPart.RemoveAll();
		TendonName = _T("");
    }  
	T_Tendon_CS_rail() {}
	T_Tendon_CS_rail(const T_Tendon_CS_rail& rData) {*this=rData;}
	T_Tendon_CS_rail& operator = (const T_Tendon_CS_rail& rData)
	{
        bIsBeam = rData.bIsBeam;
		iLoadType = rData.iLoadType;
        nElemK  = rData.nElemK;
        nTdnaK  = rData.nTdnaK;
        dfpk    = rData.dfpk;
		aTendonLoss.Copy(rData.aTendonLoss);
		aStag.Copy(rData.aStag);
		aPart.Copy(rData.aPart);
		TendonName = rData.TendonName;
		return *this;
	}
};

struct T_Cross_fracture_rail
{
    BOOL bIsBeam;
    UINT nElemK;
    UINT nPart;                  
    double dNp;
    double dNpey;
    double dNpez;
    double dalpha;
    double dfc;
    double dfct;
	double dfpApep[12],dfp_Ap_ep[12],dfsAses[12],dfs_As_es[12],dfpApZp[12],dfp_Ap_Zp[12],dfsAsZs[12],dfs_As_Zs[12];
	double dfpApap,dfp_Ap_ap,dfsAsas,dfs_As_as,dfpAp,dfp_Ap,dfsAs,dfs_As;
    _DGN_FORC_CRC aForce[12];
    _TB_PSC_SECT PscSect;
    double dTSelfStress[12];

    void Initialize()
    {
        bIsBeam = TRUE;
        nElemK = 0;
        nPart = 0;
        dNp = dNpey = dNpez = 0.0;
        dalpha = dfc = dfct = 0.0;
		dfpApap=dfp_Ap_ap=dfsAsas=dfs_As_as=dfpAp=dfp_Ap=dfsAs=dfs_As=.0;
        for(int i=0; i<12; i++)
		{
            aForce[i].Initialize();
	  dTSelfStress[i] = 0.0;
			dfpApep[i]=.0,dfp_Ap_ep[i]=.0,dfsAses[i]=.0,dfs_As_es[i]=.0;
			dfpApZp[i]=.0,dfp_Ap_Zp[i]=.0,dfsAsZs[i]=.0,dfs_As_Zs[i]=.0;
		}
        PscSect.Initialize();  
	
    }
};

struct T_Beam_incline_fracture_rail
{
    BOOL bIsBeam;
	UINT iSafeType;
    UINT nElemK;
    UINT nPart;                  
    double dalpha;
    double dfc;
    double dfct;
	BOOL bAddForce;

    void Initialize()
    {
        bIsBeam = TRUE;
		iSafeType = 0;
        nElemK = 0;
        nPart = 0;
        dalpha = dfc = dfct = 0.0;
		bAddForce = FALSE;
    }
};

struct T_Con_c_SS_rail
{
    BOOL bIsBeam;
    UINT nElemK;
    UINT nPart;                  
    double dNp;
    double dNpey;
    double dNpez;
    double dalpha;
    double dfc;
    double dfct;
    _DGN_FORC_CRC aForce[12];
    _TB_PSC_SECT PscSect;  
    double dTSelfStress[12];


    void Initialize()
    {
        bIsBeam = TRUE;
        nElemK = 0;
        nPart = 0;
        dNp = dNpey = dNpez = 0.0;
        dalpha = dfc = dfct = 0.0;
        for(int i=0; i<12; i++)
        {
		aForce[i].Initialize();
		dTSelfStress[i] = 0.0;
	}
        PscSect.Initialize();        
    }
};

struct T_Tendon_SS_rail
{
    BOOL bIsBeam;
    
    UINT nElemK;
    UINT nPart;                  
    double dfpk;
    double dn;
    double dYp;
    double dZp;
    _DGN_FORC_CRC aForce[12];
    _DGN_FORC_CRC ForceCSALL;
    
    _TB_PSC_SECT PscSect;  
    double dTendonLoss[6];
    CString TendonName;
    
    void Initialize()
    {
        bIsBeam = TRUE;
        
        nElemK = 0;
        nPart  = 0;
        dfpk = dn = dYp = dZp = 0.0;    
        for(int i=0; i<12; i++)
            aForce[i].Initialize();
        ForceCSALL.Initialize();
        
        PscSect.Initialize();
        for (int i=0; i<6; i++)
            dTendonLoss[i] = 0.0;
        TendonName=_T("");
    }  
    
};


struct T_Delta_tendon_rail
{
    BOOL bIsBeam;
    CString TendonName;   
    UINT nElemK;
    UINT nPart;                  
    double dfpk;
    double dn;
    double dYp;
    double dZp;
	UINT iLoadType;
    _DGN_FORC_CRC aForce[12];
    _DGN_FORC_CRC ForceTP;
    _DGN_FORC_CRC ForceTS;
    _TB_PSC_SECT PscSect;  
	//
	double dMy[2],dMy1[2],dMy2[2];
	UINT iMy[2];				
	double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];

    void Initialize()
    {
        bIsBeam = TRUE;
        TendonName = _T("");
        nElemK = 0;
        nPart  = 0;
		iLoadType = 0;
        dfpk = dn = dYp = dZp = 0.0;    
        for(int i=0; i<12; i++)
            aForce[i].Initialize();
        ForceTP.Initialize();
        ForceTS.Initialize();
        PscSect.Initialize();
		//
		for (int i=0; i<2; i++) 
		{
			dMy[i]=dMy1[i]=dMy2[i] = 0.0;
			iMy[i]=0;
		}
		for(int i=0;i<3;i++)
		{                       
			dstressp[i] = 0.0; 
            dstress_p[i] = 0.0;
			for(int j=0;j<3;j++)
			{ 
                dstresslossp[i][j] = 0.0; 
                dstressloss_p[i][j] = 0.0;
			}
		}
    }  
};

struct T_Shear_rail
{
    BOOL bIsBeam;
    UINT nElemK;
    UINT nPart;
    double dalpha;
    double dfc;
    double dfct;	
	double dSig_shear[6]; double dSig_tor[6]; double dSig_zz[6];double dSig[6];

	void Initialize()
    {
        bIsBeam = TRUE;
        nElemK = 0;
        nPart  = 0;
        dalpha = dfc = dfct = 0.0;    
		for (int i=0; i<6; i++)
		{
			dSig_shear[i] =.0;
			dSig_tor[i] = .0;
			dSig_zz[i] = .0;
			dSig[i] = .0;
		}
    }  
};

struct T_Beam_crack_rail
{
    BOOL bIsBeam;
    UINT nElemK;
    UINT nPart;
	UINT t;
	double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];
	double dfc,dfct,dfs,df_s,dfp,df_p,dfpk,dEc,dEs,dEp;
	double dMy,dMy1,dMy2;
	UINT 	iMy;
	int iLoadType;
    _TB_PSC_SECT PscSect;  

    void Initialize()
    {
        int i;
        bIsBeam = TRUE;
        nElemK = 0;
        nPart  = 0;
	  for(i=0; i<3; i++)
        {
            dstressp[i]=dstress_p[i]=0.0;
            for(int j=0; j<3; j++)
                dstresslossp[i][j]=dstressloss_p[i][j]=0.0;
        }
	  dfc=dfct=dfs=df_s=dfp=df_p=dfpk=dEc=dEs=dEp=0.0;
        dMy=dMy1=dMy2=0.0;
	  iMy=0;
		t  =0;
		iLoadType =0;
		PscSect.Initialize();
    }  

};

struct T_Beam_crossbend_rail
{
    UINT nElemK;
    UINT nPart;
	double df_s,dfs,dfp,df_p,dfc;
	double dK;
    _TB_PSC_SECT PscSect;  
	double dMy[2],dMy1[2],dMy2[2];
	UINT iMy[2];				
	double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];

    void Initialize()
    {
        nElemK = 0;
        nPart  = 0;
    	df_s = dfs = dfp = df_p = dfc =.0;
		dK = .0;
        PscSect.Initialize();
		
		for (int i=0; i<2; i++) 
		{
			dMy[i]=dMy1[i]=dMy2[i] = .0;
			iMy[i]=0;
		}
		for(int i=0;i<3;i++)
		{
			dstressp[i]=.0; 
            dstress_p[i]=.0;
			for(int j=0;j<3;j++)
			{ 
				dstresslossp[i][j]=.0; 
                dstressloss_p[i][j]=.0; 
			}
		}
    }  
};

struct T_Beam_inclineshear_rail
{
    UINT nElemK;
    UINT nPart;
	double dfs,dfp,dfct;
	double dK,dVz;
	int iVz;
    _TB_PSC_SECT PscSect;  
    void Initialize()
    {
        nElemK = 0;
        nPart  = 0;
		dfct = dfp = dfs = dK = dVz = .0;
		iVz = 0;
		PscSect.Initialize();
    }
};

struct T_Column_compressive_rail
{
    BOOL bIsBeam;
    UINT nElemK;
    UINT nPart;
    _DGN_FORC_CRC Force;
    _TB_PSC_SECT PscSect;  
	_DGN_RC_MATL_CH MatlCH;
	double dfpk,dn;
	double dfpApep,dfp_Ap_ep,dfsAses,dfs_As_es,dfpApZp,dfp_Ap_Zp,dfsAsZs,dfs_As_Zs;
	double dfpApap,dfp_Ap_ap,dfsAsas,dfs_As_as,dfpAp,dfp_Ap,dfsAs,dfs_As;
    void Initialize()
    {
        bIsBeam = TRUE;
        nElemK = 0;
        nPart  = 0;
		dfpk = dn =  .0;
		dfpApep=.0;dfp_Ap_ep=.0;dfsAses=.0;dfs_As_es=.0;dfpApZp=.0;dfp_Ap_Zp=.0;dfsAsZs=.0;dfs_As_Zs=.0;
		dfpApap=.0;dfp_Ap_ap=.0;dfsAsas=.0;dfs_As_as=.0;dfpAp=.0;dfp_Ap=.0;dfsAs=.0;dfs_As=.0;
        PscSect.Initialize();
		MatlCH.Initialize();
        Force.Initialize();
    }
};

struct T_column_cross_fracture_rail
{
    UINT nElemK;
    UINT nPart;
	UINT t;
    _TB_PSC_SECT PscSect;  
	int iLoadType; // 0=Pre-tension, 1=Post-tension.
	double dstressp[3],dstresslossp[3][3],dstress_p[3],dstressloss_p[3][3];
	double dfc,dfct,dfs,df_s,dfp,df_p,dfpk,dEc,dEs,dEp;
	double dKf,dMy,dNx;
	int iMy,iNx;
    void Initialize()
    {
        nElemK = 0;
        nPart  = 0;
		t      = 0;
		PscSect.Initialize();
		iLoadType = 0;
		dfc=dfct=dfs=df_s=dfp=df_p=dfpk=dEc=dEs=dEp=0.0;
		dKf=dMy=dNx=.0;
		iMy=iNx=0;
		//
		for(int i=0;i<3;i++)                                             //
		{                                                                //
			dstressp[i]=.0; dstress_p[i]=.0;                                 //
			for(int j=0;j<3;j++)                                           //
			{                                                              //
				dstresslossp[i][j]=.0; dstressloss_p[i][j]=.0;                 //
			}		                                                           //
		}
    }

};

struct T_Column_tension_rail
{
    UINT nElemK;
    UINT nPart;
	UINT k;
	double dn;
	double dfpk;
	double dfpApap;
	double dfsAsas;
	double dfpAp;
	double dfsAs;
	double dfp_Ap_ap;
	double dfs_As_as;
	double dfp_Ap;
	double dfs_As;
    _DGN_FORC_CRC aForce[12];
    _TB_PSC_SECT PscSect;  
	_DGN_RC_MATL_CH MatlCH;

    void Initialize()
    {
		k      = 0;
        nElemK = 0;
        nPart  = 0;
		dn     = .0;
		dfpk   = .0;
		dfpApap    =.0;
		dfsAsas    =.0;
		dfpAp      =.0;
		dfsAs      =.0;
		dfp_Ap_ap  =.0;
		dfs_As_as  =.0;
		dfp_Ap     =.0;
		dfs_As     =.0;
		PscSect.Initialize();
		MatlCH.Initialize();
		for (int i=0; i<12; i++)
			aForce[i].Initialize();
    }
};

struct T_Tendon_Stress_D
{
	double dstressp[3];
	double dstresslossp[3][3];
	double dstress_p[3];
	double dstressloss_p[3][3];
	
	void Initialize()
	{
		for(int i=0; i<3; i++)
		{
			dstressp[i] = 0.0;
			dstress_p[i] = 0.0;
			for(int j=0; j<3; j++)
			{
				dstresslossp[i][j]=0.0;
				dstressloss_p[i][j]=0.0;
			}

		}
	}
};


class CPSCrailway : public CCRCFileBase  
{
public:
	BOOL isaddforce[4]; //是否为主力加附加力组合
	BOOL m_beamcheckitem[14];//验算的项目（界面上用户输入）
    BOOL m_Columncheckitem[10];//验算的项目（界面上用户输入）

	double m_dK;//安全系数
	double m_dKf;//抗裂安全系数
	double m_dWfk;//裂缝宽度容许值  
	CArray<UINT, UINT> m_elemlist;

	CStdioFile file; 

protected:
    CDBDoc* m_pDoc;
    CRCDataCtrl_CH m_DataCtrl;
	CCRCForceCtrl m_ForceCtrl;
    CCRCDataCtrl*  m_pDataCtrl;
	UINT m_iDimType;
	double m_dZero;
    int m_iCastType;//0-预制,1-现浇; 影响系数K Kf    

public:
    int m_iTotLineNum;

    CArray<UINT,UINT> m_Beam_PSC_KList;                 //预应力度验算 
    CArray<UINT,UINT> m_Beam_cross_bend_KList;          //正截面抗弯验算
    CArray<UINT,UINT> m_Beam_incline_shear_KList;       //斜截面抗剪验算
    CArray<UINT,UINT> m_Beam_cross_fracture_KList;      //运营阶段－正截面抗裂验算
    CArray<UINT,UINT> m_Beam_incline_fracture_KList;    //运营阶段－斜截面抗裂验算
    CArray<UINT,UINT> m_Beam_incline_tp_KList;          //运营阶段－混凝土主拉应力
    CArray<UINT,UINT> m_Beam_con_c_SS_KList;            //运营阶段－混凝土压应力验算
    CArray<UINT,UINT> m_Beam_con_t_SS_KList;            //运营阶段－混凝土拉应力验算
    CArray<UINT,UINT> m_Beam_tendon_SS_KList;           //运营阶段－预应力钢筋应力验算 
    CArray<UINT,UINT> m_Beam_delta_tendon_KList;        //运营阶段－预应力钢筋应力幅验算
    CArray<UINT,UINT> m_Beam_shear_KList;               //运营阶段－混凝土剪应力验算
    CArray<UINT,UINT> m_Beam_crack_KList;               //运营阶段－裂缝宽度验算          
	CArray<UINT,UINT> m_Beam_tendon_CS_KList;           //预加应力阶段－预应力钢筋锚下控制应力验算
    CArray<UINT,UINT> m_Beam_con_CS_KList;              //预加应力阶段－混凝土法向应力验算

    CArray<UINT,UINT> m_Column_PSC_KList;               //预应力度验算
	CArray<UINT,UINT> m_Column_compressive_KList;       //正截面抗压强度验算
    CArray<UINT,UINT> m_Column_tension_KList;           //正截面抗拉强度验算

	CArray<UINT,UINT> m_Column_cross_fracture_KList;    //运营阶段－预应力柱正截面抗裂验算
    CArray<UINT,UINT> m_Column_con_c_SS_KList;          //运营阶段－混凝土压应力验算
	CArray<UINT,UINT> m_Column_con_t_SS_KList;          //运营阶段－混凝土拉应力验算
    CArray<UINT,UINT> m_Column_tendon_SS_KList;         //运营阶段－预应力钢筋应力验算
    CArray<UINT,UINT> m_Column_shear_KList;             //运营阶段－混凝土剪应力验算
	CArray<UINT,UINT> m_Column_tendon_CS_KList;         //预加应力阶段－预应力钢筋锚下控制应力验算
	CArray<UINT,UINT> m_Column_con_CS_KList;            ////预加应力阶段－混凝土法向应力验算
	CArray<UINT,UINT> m_Column_delta_tendon_KList;//运营阶段－预应力钢筋应力幅验算

    CMap<UINT,UINT,UINT,UINT> m_DgnLcomKMap;
    CMap<UINT,UINT,UINT,UINT> m_DgnTdgrMap; //DgnTdgrK, TdgrK
    CMap<UINT,UINT,UINT,UINT> m_DgnTdnaMap; //DgnTdgrK, TdnaK
    CMap<_CRC_COMF_K, _CRC_COMF_K, _CRC_COMF_D, _CRC_COMF_D&> m_ComfDMap;
public:
	CPSCrailway();
	virtual ~CPSCrailway();

protected:
    //PSC method...

    double Get_Sigma(BOOL bZ,double Np,double A,double Npez,double Npey,double z,double y,double Iyy,double Izz);
	BOOL Get_Tendon_Stress(int Elemi,int iPosi,T_Tendon_Stress_D& TndnStress);
	BOOL Get_Metl(int Elemi, int iPosi,double& dfc,double& dfct,double& dfs,double& df_s,double& dfp,double& df_p,double& dfpk,double& dEc,double& dEs,double& dEp);
	BOOL Get_SubRebar(int Elemi, int iPosi,double& dfsv);
	void get_beam_M(int ElemK,int iPosi,double* dMy,UINT* iMy,double* dMy1,double* dMy2, BOOL bRemoveGS = FALSE); 
    void get_beam_bend(int ElemK,int iPosi,double* dMy,UINT* iMy,double* dMy1,double* dMy2); //by maxiao
	void get_beam_V(int Elemi, int iPosi, double* dVy, UINT* iVy, double* dM); 
	void get_coloumn_MN(int Elemi, int iPosi, double* dMy, UINT* iMy, double* dNx, UINT* iNx);
//	void get_beam_V(int Elemi, int iPosi, double dVy[2], UINT iVy[2]); 
//	void get_coloumn_MN(int Elemi, int iPosi, double dMy[4], UINT iMy[4], double dNx[4], UINT iNx[4]);
	void Isaddforce(int t);

    HINSTANCE m_hDllCalc;

    CString Get_DllFileName(int iCode);

public:
    BOOL PSCrailway_CH(BOOL isBeam);
    BOOL Write_ImportData(BOOL isBeam);
    BOOL MakeData(BOOL isBeam);


    //预应力梁验算所用到的函数 
	void PSCdegree(BOOL isBeam);        //预应力度验算 
	void Concrete_CS(BOOL isBeam);      //预加应力阶段－混凝土法向应力验算
    void Tendon_CS(BOOL isBeam);       //预加应力阶段－预应力钢筋锚下控制应力验算  
	void Cross_fracture(int iType,BOOL isBeam);   //运营阶段－正截面抗裂验算 
	void Shear(BOOL isBeam);            //运营阶段－混凝土剪应力验算 
	void Tendon_SS(BOOL isBeam);        //运营阶段－预应力钢筋应力验算 
	void Delta_tendon(int iType,BOOL isBeam);     //运营阶段－预应力钢筋应力幅验算
	void Con_c_SS(BOOL isBeam);         //运营阶段－混凝土压应力验算  

    void beam_incline_fracture(int iSafeType); //运营阶段－斜截面抗裂验算  
	BOOL get_con_tp(int Elemi, T_BEAM_CON_TP_CH& con_tp_elem);
	void beam_crack();            //运营阶段－裂缝宽度验算 
	void beam_crossbend();        //正截面抗弯验算
	void beam_inclineshear();     //斜截面抗剪验算  

	//预应力柱验算所用到的函数
    void column_cross_fracture(); //运营阶段－正截面抗裂验算 
	void column_compressive();    //正截面抗压强度验算
	void column_tension();        //正截面抗拉强度验算


};

#endif // !defined(AFX_PSCRAILWAY_H__C9905C02_06CE_4BC9_BEB4_CE6FC1A15089__INCLUDED_) 

