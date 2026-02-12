#ifndef __DB_ST_DN_RESULT_CH_H__
#define __DB_ST_DN_RESULT_CH_H__
//////////////////////////////////////////////////////////////////////////
//DB_ST_DN_RESULT_CH.H 
#define MIDAS_SDK

#include "DB_ST_DT_UNIT.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(1)
#pragma warning ( disable : 4244 )

//////////////////////////////////////////////////////////////////////////
// 正截面抗弯验算数据结构
struct T_CBCS_CH_BASE
{
    BOOL bCHK;		//是否验算的标识
    UINT LcomK;		//验算内力的组合id[Unit=None]
    int iMax;	    //MIN-MAX设计内力值 ( 酒丛=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dMuy;	//弯矩设计值Muy		[Unit=Moment]
    double drMuy; // r0*Muy         [Unit=Moment]
    double dMny;  //计算弯矩值      [Unit=Moment]
    double dRatY;	//设计/计算弯矩比	[Unit=None]

    double dx;	//  [Unit=Length]//add by juhonghua
    double dEb; //  [Unit=Length]
    double d2a;  //	[Unit=Length]
    
    double dReBarRatio;    // [Unit=None]
    double dMinReBarRatio;    // [Unit=None]

    BOOL bOK;

    void Initialize()
    {
        bCHK = FALSE;
        LcomK = 0;
        iMax = 0;
        dMuy = drMuy = dMny = dRatY = 0.0;
        dx = dEb = d2a = 0.0;
        dReBarRatio = dMinReBarRatio = 0.0;

        bOK = TRUE;
    }
};

struct T_CBCS_CH	// ElemK.
{
    T_CBCS_CH_BASE cbcsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            cbcsBase[i].Initialize();
    }
};

struct T_CBCS_CH_UNIT
{
    T_CBCS_CH_UNIT()
    {
        dMuy  = D_UNITSYS_BASE_MOMENT;
        drMuy = D_UNITSYS_BASE_MOMENT;
        dMny  = D_UNITSYS_BASE_MOMENT;
        dRatY = D_UNITSYS_NONE;
        dx = D_UNITSYS_BASE_LENGTH;	
        dEb= D_UNITSYS_BASE_LENGTH;	
        d2a = D_UNITSYS_BASE_LENGTH;  
    }
    int dMuy;
    int drMuy;
    int dMny;
    int dRatY;
    int dx;//add by juhonghua
    int dEb;
    int d2a;

};


//斜截面抗剪验算数据结构
struct T_CSIS_CH_BASE
{
    BOOL bCHK;		  //是否验算的标识
    UINT LcomK;		  //验算内力的组合id		[Unit=None]
    int iMax;	      // 	MIN-MAX设计内力值 ( 酒丛=0, FX-MAX=1~MZ-MIN=12) 	[Unit=None]
    double dVu;	    //剪力设计值	  	     [Unit=Force]
    double drVu;    // r0*Vu               [Unit=Force]
    double dMu;     //弯矩设计值           [Unit=Moment]
    double dVn;		  //剪力计算值		       [Unit=Force]
    double dRatV;   //设计/计算剪力比      [Unit=None]
    double dShrSect; //截面验算剪力值      [Unit=Force]
    double dShrLoad; //判断是否剪力验算的剪力值     [Unit=Force]
    double dMu_P;   //斜截面抗弯验算值     [Unit=Moment]
    BOOL bOK;
    void Initialize()
    {
        bCHK = FALSE;
        LcomK = 0;
        iMax = 0;
        dVu = drVu = dMu = dVn = dRatV = 0.0;
        dShrSect = 0.0;
        dShrLoad = 0.0;
        dMu_P = 0.0;
        bOK = TRUE;
    }
};
struct T_CSIS_CH	//ElemK.
{
    T_CSIS_CH_BASE CsisBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            CsisBase[i].Initialize();
    }
};

struct T_CSIS_CH_UNIT
{
    T_CSIS_CH_UNIT()
    {
        dMu      = D_UNITSYS_BASE_MOMENT;
        dMu_P    = D_UNITSYS_BASE_MOMENT;  
        dVu      = D_UNITSYS_BASE_FORCE;
        drVu     = D_UNITSYS_BASE_FORCE;
        dVn      = D_UNITSYS_BASE_FORCE;
        dShrSect = D_UNITSYS_BASE_FORCE;
        dShrLoad = D_UNITSYS_BASE_FORCE;
        dRatV    = D_UNITSYS_NONE;
    }
    int dVu;
    int drVu;
    int dMu;
    int dVn;
    int dRatV;
    int dShrSect;
    int dShrLoad;
    int dMu_P;
};

//抗扭验算数据结构
struct T_CTSS_CH_BASE
{
    BOOL bCHK;		  //是否验算的标识
    UINT LcomK;		  //验算内力的组合id		[Unit=None]
    int iMax;	     	//MIN-MAX设计内力值 (酒丛=0, FX-MAX=1~MZ-MIN=12)  [Unit=None]
    double dTu;		  //扭矩设计值    		  [Unit=Moment]
    double drTu;     //r0*Tu              [Unit=Moment]
    double dTn;    	//扭矩验算值        	[Unit=Moment]
    double dMu;     //弯矩设计值          [Unit=Moment]
    double dVu;     //剪力设计值          [Unit=Force]
    double drVu;    //r0*Vu               [Unit=Force]
    double dVn;     //剪力验算值          [Unit=Force]
    double dRatT;   //设计/计算扭矩比     [Unit=None]
    double dShrTorVal;  //由设计内力得到的验算值. [Unit=Stress] JTG D62-04, (5.5.3-1), (5.5.3-2) 曼绊 
    double dTorSect;    //截面验算的计算值  [Unit=Stress] 
    double dTorLoad;    //判断是否扭矩验算的计算值.                        [Unit=Stress] 
    BOOL bOK;

    void Initialize()
    {
        bCHK = FALSE;
        LcomK = 0;
        iMax = 0;
        dTu =  drTu = dTn = dMu= dVu = drVu = dVn = dRatT = 0.0;
        dShrTorVal = 0.0;
        dTorSect   = 0.0;
        dTorLoad   = 0.0;
        bOK = TRUE;
    }
};
struct T_CTSS_CH	// ElemK.
{
    T_CTSS_CH_BASE CtssBase[6];	// 0=I-V-MAX, 1=I-V-MIN, 2=I-T-MAX, 3=J-V-MAX, 4=J-V-MIN, 5=J-T-MAX.
    void Initialize()
    {
        for(int i=0; i<6; i++)
            CtssBase[i].Initialize();
    }
};

struct T_CTSS_CH_UNIT
{
    T_CTSS_CH_UNIT()
    {
        dTu 	  	= D_UNITSYS_BASE_MOMENT; 
        drTu      = D_UNITSYS_BASE_MOMENT;   
        dTn       = D_UNITSYS_BASE_MOMENT; 
        dMu       = D_UNITSYS_BASE_MOMENT; 
        dVu       = D_UNITSYS_BASE_FORCE; 
        drVu      = D_UNITSYS_BASE_FORCE; 
        dVn       = D_UNITSYS_BASE_FORCE; 
        dRatT     = D_UNITSYS_NONE; 
        dShrTorVal= D_UNITSYS_BASE_STRESS;
        dTorSect  = D_UNITSYS_BASE_STRESS;
        dTorLoad  = D_UNITSYS_BASE_STRESS;
    }
    int dTu;		
    int drTu;   
    int dTn;    
    int dMu;    
    int dVu;    
    int drVu;   
    int dVn;    
    int dRatT;  
    int dShrTorVal; 
    int dTorSect;   
    int dTorLoad; 
};

//裂缝宽度验算数据结构
struct T_CCWS_CH_BASE
{
    BOOL bCHK;		//是否验算的标识
    UINT LcomK;
    int iMax;		     //MIN-MAX设计内力值 酒丛=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                          FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    int iKind;       //(0:轴心受拉, 1:偏心受拉, 2:偏心受压, 3:(圆形)偏心受压)
    double dFT;		//作用短期效应组合得到的内力值,梁为Ms,柱为Ns.		[Unit=Moment/Force]
    double dFB;		//对应短期效应组合的长期效应组合,梁为Ml,柱为Nl.	[Unit=Moment/Force]
    double dMsy;   //对应短期效应组合Ns的Msy(柱)[Unit=Moment]
    double dMsz;   //对应短期效应组合Ns的Msz(柱)[Unit=Moment]
    double dFRT;	//验算的受拉钢筋应力  [Unit=Stress]
    double dWC;	  //验算得到的裂缝宽度	[Unit=Length]
    double dAWC;	//允许裂缝宽度      	[Unit=Length]
    BOOL bOK;
    void Initialize()
    {
        bCHK = FALSE;
        LcomK = 0;
        iMax = 0;
        iKind = 0;
        dFT = dFB = dFRT = dWC = dAWC = 0.0;
        dMsy = dMsz = 0.0;
        bOK = TRUE;
    }
};
struct T_CCWS_CH	// ElemK.
{
    T_CCWS_CH_BASE ccwsBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            ccwsBase[i].Initialize();
    }
};
struct T_CCWS_CH_UNIT
{
    T_CCWS_CH_UNIT()
    {
        //dFT	 = D_UNITSYS_BASE_MOMENT; //梁和柱不一样,暂注释掉
        //dFB	 = D_UNITSYS_BASE_MOMENT; //梁和柱不一样,暂注释掉
        dMsy = D_UNITSYS_BASE_MOMENT; 
        dMsz = D_UNITSYS_BASE_MOMENT; 
        dFRT = D_UNITSYS_BASE_STRESS; 
        dWC	 = D_UNITSYS_BASE_LENGTH; 
        dAWC = D_UNITSYS_BASE_LENGTH; 
    }
    int dFT;	
    int dFB;	
    int dMsy; 
    int dMsz; 
    int dFRT;	
    int dWC;	
    int dAWC;	
};

//施工阶段正截面压应力数据结构
struct T_CSCC_CH_BASE
{
    BOOL bCHK;		//是否验算的标识
    UINT StagK;		  //施工阶段		[Unit=None]
    int iMax;		     //MIN-MAX设计值 酒丛=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)

    double dFT;   //顶部设计应力值	[Unit=Stress]
    double dFB;   //底部设计应力值	[Unit=Stress]
    double dFTL;	//顶左设计应力值	[Unit=Stress]
    double dFBL;	//底左设计应力值	[Unit=Stress]
    double dFTR;	//顶右设计应力值	[Unit=Stress]
    double dFBR;	//底右设计应力值	[Unit=Stress]
    double dFMAX;	//最大应力设计值	[Unit=Stress]
    double dALW;	//允许应力设计值	[Unit=Stress]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
		iMax = 0;
        StagK = 0;
        dFT = dFB = 0.0;
        dFTL = dFBL = dFTR = dFBR = dFMAX = dALW = 0;
        bOK = TRUE;
    }
};
struct T_CSCC_CH	// ElemK.
{
    T_CSCC_CH_BASE CsccBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            CsccBase[i].Initialize();
    }
};
struct T_CSCC_CH_UNIT
{
    T_CSCC_CH_UNIT()
    {
        dFT 	 = D_UNITSYS_BASE_STRESS; 
        dFB 	 = D_UNITSYS_BASE_STRESS; 
        dFTL   = D_UNITSYS_BASE_STRESS; 
        dFBL   = D_UNITSYS_BASE_STRESS; 
        dFTR   = D_UNITSYS_BASE_STRESS; 
        dFBR   = D_UNITSYS_BASE_STRESS; 
        dFMAX  = D_UNITSYS_BASE_STRESS; 
        dALW   = D_UNITSYS_BASE_STRESS;     
    }
    int dFT; 
    int dFB; 
    int dFTL;
    int dFBL;
    int dFTR;
    int dFBR;
    int dFMAX;
    int dALW;
};

//受拉钢筋的拉应力数据结构
struct T_CTST_CH	//
{
    BOOL bCHK;
    UINT iRbarNa;//钢筋名称
    UINT iposi;//钢筋位置
    UINT ilayer;//钢筋层数
    int  steelk;//能唯一标志出钢筋的名称
    double dFLL;	  //受拉钢筋的拉应力   [Unit=Stress]弹性阶段验算荷载组合计算得到的钢筋最大拉应力
    double dAFLL;   //受拉钢筋的允许应力 [Unit=Stress]
    double dDLL; //施工阶段荷载引起的钢筋拉应力 [Unit=Stress]
    double dADLL;//受拉钢筋的允许应力 [Unit=Stress]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        dFLL = 0.0;
        dAFLL = 0.0;
        dDLL = dADLL = 0.0;
        steelk = 0;
        iposi = ilayer = 0;
        iRbarNa = 0;
        bOK = TRUE;
    }
};

struct T_CTST_CH_UNIT
{
    T_CTST_CH_UNIT()
    {
        dFLL 	 = D_UNITSYS_BASE_STRESS; 
        dAFLL  = D_UNITSYS_BASE_STRESS; 
        dDLL 	 = D_UNITSYS_BASE_STRESS; 
        dADLL  = D_UNITSYS_BASE_STRESS; 
    }
    int dFLL;
    int dAFLL;
    int dDLL;
    int dADLL;
};

//中性轴处的主拉应力数据结构
struct T_CSNC_CH_BASE
{
    BOOL bCHK;		//是否验算的标识
    UINT StagK;		  //施工阶段		
    double dFM;   //中	[Unit=Stress]
    double dFML;	//中左	[Unit=Stress]
    double dFMR;	//中右	[Unit=Stress]
    double dFMAX;	//	[Unit=Stress]
    double dALW;	//	[Unit=Stress]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        StagK = 0;
        dFM = dFML = dFMR = dFMAX = dALW = 0.0;
        bOK = TRUE;
    }
};
struct T_CSNC_CH	// ElemK.
{
    T_CSNC_CH_BASE CsncBase[4];	// 0=I-MAX, 1=I-MIN, 2=J-MAX, 3=J-MIN.
    void Initialize()
    {
        for(int i=0; i<4; i++)
            CsncBase[i].Initialize();
    }
};
struct T_CSNC_CH_UNIT
{
    T_CSNC_CH_UNIT()
    {
        dFM   = D_UNITSYS_BASE_STRESS; 
        dFML  = D_UNITSYS_BASE_STRESS; 
        dFMR  = D_UNITSYS_BASE_STRESS; 
        dFMAX = D_UNITSYS_BASE_STRESS; 
        dALW  = D_UNITSYS_BASE_STRESS; 
    }
    int dFM; 
    int dFML;
    int dFMR;
    int dFMAX;
    int dALW;
};

//////////////////////////////////////////////////////////////////////////
//抗拉压验算数据结构
struct T_CSTCS_CH_BASE
{
    BOOL bCHK;		//汲拌搬苞蜡公
    UINT LcomK;
    int iMax;		     //MIN-MAX设计值 酒丛=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dNd;		//轴心力设计值		[Unit=Force]
    double dMd;   //弯矩设计值 	[Unit=Moment]
    double drNd;		//ro*轴心力设计值			[Unit=Force]
    double drNde;	//ro*轴心力设计值*e		[Unit=Moment]
    double drNde1;	  //ro*轴心力设计值*e'			[Unit=Moment]
    double dNn;	//轴心力允许值	[Unit=Force]
    double dNne;	//对应ro*Nd*e的轴心力允许值		[Unit=Moment]
    double dNne1;	  //对应ro*Nd*e'的轴心力允许值			[Unit=Moment]
	double dx;//受压区高度 	[Unit=Length]

	double dRa,dMinRa; //轴心受压构件和偏心受压构件：全部纵向钢筋的配筋率
	double dRs,dMinRs; //1.轴心受压构件和偏心受压构件：一侧钢筋的配筋率  2.偏心受拉构件及轴心受拉构件：一侧受拉钢筋的配筋率
	int iMembType;  //(0:轴心受拉, 1:偏心受拉, 2:偏心受压, 3:(圆形)偏心受压)
    BOOL bOK;
    void Initialize()
    {
        bCHK = FALSE;
        LcomK = 0;
        iMax = 0;
        dMd = 0.0;
        dNd = drNd = drNde = drNde1 = dNn = dNne = dNne1 = 0.0;
		dx = 0.0;
		dMinRa = dRa = 0.0;
		dMinRs = dRs = 0.0;
		iMembType = 0 ;
        bOK = TRUE;
    }
};
struct T_CSTCS_CH	// ElemK.
{
    T_CSTCS_CH_BASE cstcsBase[10];	// 0=轴心,1/2,Fx组内力的偏心,3/4My组内力的偏心.(0-4:i截面; 5-9:j截面)
    void Initialize()                
    {
        for(int i=0; i<10; i++)
            cstcsBase[i].Initialize();
    }
};
struct T_CSTCS_CH_UNIT
{
    T_CSTCS_CH_UNIT()
    {
        dNd     = D_UNITSYS_BASE_FORCE;
        dMd     = D_UNITSYS_BASE_MOMENT;
        drNd  	= D_UNITSYS_BASE_FORCE; 
        drNde 	= D_UNITSYS_BASE_MOMENT; 
        drNde1  = D_UNITSYS_BASE_MOMENT; 
        dNn     =	D_UNITSYS_BASE_FORCE;
        dNne    =	D_UNITSYS_BASE_MOMENT; 
        dNne1   = D_UNITSYS_BASE_MOMENT; 
        dx      = D_UNITSYS_BASE_LENGTH;
    }
    int dNd;
    int dMd;
    int drNd;	
    int drNde;	
    int drNde1;
    int dNn;	  
    int dNne;	
    int dNne1;	
	int dx;    
};


//梁的纵向钢筋估算数据结构
struct T_BRLR_CH_BASE 
{
    BOOL bCHK;		//是否计算的标识
    UINT LcomK;
    int iMax;		     //MIN-MAX设计内力值 酒丛=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                          FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dMd;     //                [Unit=Moment]
    double dREQTop;	//顶部计算配筋面积			[Unit=Area]
    double dREQBot;	//底部计算配筋面积			[Unit=Area]
    double dUSETop;	//use配筋面积			[Unit=Area]
    double dUSEBot;	//use配筋面积			[Unit=Area]
    double dRho;          //纵向钢筋百分比 
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dMd = dRho = 0.0;
        dREQTop = dREQBot = dUSETop = dUSEBot = 0.0;
        bOK = TRUE;
    }
};
struct T_BRLR_CH	// ElemK.
{
    T_BRLR_CH_BASE BrlrBase[4];	// 0=I-TOP, 1=I-BOT, 2:J-TOP, 3:J-Bot.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            BrlrBase[i].Initialize();
    }
};
struct T_BRLR_CH_UNIT
{
    T_BRLR_CH_UNIT()
    {
        dMd     = D_UNITSYS_BASE_MOMENT;
        dREQTop = D_UNITSYS_BASE_AREA; 
        dREQBot = D_UNITSYS_BASE_AREA; 
        dUSETop = D_UNITSYS_BASE_AREA; 
        dUSEBot =	D_UNITSYS_BASE_AREA;
        dRho    =	D_UNITSYS_NONE; 
    }
    int dMd;     
    int dREQTop;	
    int dREQBot;	
    int dUSETop;	
    int dUSEBot;	
    int dRho;    
};

//梁的普通箍筋估算数据结构
struct T_BRCR_CH_BASE 
{
    BOOL bCHK;		//是否计算的标识
    UINT LcomK;
    int iMax;		     //MIN-MAX设计内力值 酒丛=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                          FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dVd;     //                [Unit=Force]
    double dREQ;	//计算配筋面积			[Unit=Area]
    double dUSE;	//use配筋面积			[Unit=Area]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dVd = dREQ = dUSE = 0.0;
        bOK = TRUE;
    }
};
struct T_BRCR_CH	// ElemK.
{
    T_BRCR_CH_BASE BrcrBase[4];	// 0=I-TOP, 1=I-BOT, 2:J-TOP, 3:J-Bot.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            BrcrBase[i].Initialize();
    }
};
struct T_BRCR_CH_UNIT
{
    T_BRCR_CH_UNIT()
    {
        dVd  = D_UNITSYS_BASE_FORCE;
        dREQ = D_UNITSYS_BASE_AREA; 
        dUSE = D_UNITSYS_BASE_AREA; 
    }
    int dVd; 
    int dREQ;
    int dUSE;
};

//柱的纵向钢筋估算数据结构
struct T_CRLR_CH_BASE 
{
    BOOL bCHK;		//是否计算的标识
    UINT LcomK;
    int iMax;		     //MIN-MAX   设计内力值  FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dNd;      //                [Unit=Force]
    double dMyd;     //                [Unit=Moment]
    double dMzd;     //                [Unit=Moment]
    double dREQTop;	 //截面顶部的配筋面积			[Unit=Area]
    double dREQBot;	 //截面底部的配筋面积			[Unit=Area]
    double dREQLeft; //截面左侧的配筋面积			[Unit=Area]
    double dREQRight;//截面右侧的配筋面积			[Unit=Area]

    double dUSETop;  //截面实际顶部配筋面积		  	[Unit=Area]
    double dUSEBot;	 //截面实际底部的配筋面积			[Unit=Area]
    double dUSELeft; //截面实际左侧的配筋面积			[Unit=Area]
    double dUSERight;//截面实际右侧的配筋面积			[Unit=Area]
    BOOL bOK;
    void Initialize()
    {
        bCHK = 0;
        LcomK = 0;
        iMax = 0;
        dNd = dMyd = dMzd = 0.0;
        dREQTop = dREQBot = dREQLeft = dREQRight = 0.0;
        dUSETop = dUSEBot = dUSELeft = dUSERight = 0.0;
        bOK = TRUE;
    }
};
struct T_CRLR_CH	// ElemK.
{
    T_CRLR_CH_BASE CrlrBase[4];	// 0=I-tension, 1=I-compressive, 2:J-tension, 3:J-compressive.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            CrlrBase[i].Initialize();
    }
};
struct T_CRLR_CH_UNIT
{
    T_CRLR_CH_UNIT()
    {
        dNd      = D_UNITSYS_BASE_FORCE;
        dMyd     = D_UNITSYS_BASE_MOMENT; 
        dMzd     = D_UNITSYS_BASE_MOMENT; 
        dREQTop  = D_UNITSYS_BASE_AREA;	
        dREQBot  = D_UNITSYS_BASE_AREA;	
        dREQLeft = D_UNITSYS_BASE_AREA;	
        dREQRight= D_UNITSYS_BASE_AREA;

        dUSETop  = D_UNITSYS_BASE_AREA;	
        dUSEBot  = D_UNITSYS_BASE_AREA;	
        dUSELeft = D_UNITSYS_BASE_AREA;	
        dUSERight= D_UNITSYS_BASE_AREA;

    }
    int dNd;      
    int dMyd;     
    int dMzd;     
    int dREQTop;	
    int dREQBot;	
    int dREQLeft;	
    int dREQRight;

    int dUSETop;	
    int dUSEBot;	
    int dUSELeft;	
    int dUSERight;
};

//////////////////////////////////////////////////////////////////////////
// [21/02/2007 : maxiao] For CIVIL(CH Railway Design) 2007
//混凝土压应力验算数据结构
struct CONCRETE_C
{ 
    BOOL bCHK;		//是否计算的标识
	UINT Lcomk;//验算内力组合的ID
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
	double dM;//设计弯矩值
	double dN;//设计轴力值
    double dconcrete_c;//截面计算压应力
    double dconcrete_b;//截面容许压应力
	BOOL bOK;
	void Initialize()
	{ 
        bCHK = 0;
		Lcomk=0;
		itype=0;
	  dM=0;
		dN=0;
		dconcrete_c=0;
        dconcrete_b=0;
		bOK = TRUE;
	}
};
struct T_BEAM_CONCRETE_C_CH	// ElemK.
{
    CONCRETE_C dataBase[4];	// 0=I-tension, 1=I-compressive, 2:J-tension, 3:J-compressive.

    void Initialize()
    {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_CONCRETE_C_CH	     // ElemK
{
    CONCRETE_C dataBase[8]; // 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 
	                               // 4=J-Fxmax, 5=J-Fxmin, 6=J-Mymax, 7=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<8; i++)
            dataBase[i].Initialize();
    }
};
struct T_CONCRETE_C_CH_UNIT
{
    T_CONCRETE_C_CH_UNIT()
    {
 	  dM         = D_UNITSYS_BASE_MOMENT;
		dN         = D_UNITSYS_BASE_FORCE;
		dconcrete_c= D_UNITSYS_BASE_STRESS;
        dconcrete_b= D_UNITSYS_BASE_STRESS;
    }
    int dM;      
    int dN;     
    int dconcrete_c;     
    int dconcrete_b;	
};

//混凝土剪应力验算数据结构
struct CONCRETE_TP
{ 
	BOOL bCHK;		           //是否验算的标识
	UINT Lcomk;//验算内力组合的ID
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
	double dV;//设计的剪力值
	double dM;//设计的弯矩值
    double dshear;//截面计算剪应力
    double dconcrete_shear;//截面容许剪应力
	BOOL bOK;
	void Initialize()
	{ 
		bCHK =0;
		Lcomk=0;
		itype=0;
		dV=dM=.0;
		dshear=0;
        dconcrete_shear=0;
		bOK =TRUE;
	}
};
struct T_BEAM_CONCRETE_TP_CH
{ 
    CONCRETE_TP dataBase[4];	//
    void Initialize()  {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_CONCRETE_TP_CH	        // ElemK
{
    CONCRETE_TP dataBase[12];	// 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 4=I-Fzmax, 5=I-Fzmin
	                                    // 6=J-Fxmax, 7=J-Fxmin, 8=J-Mymax, 9=J-Mymin, 10=J-Fzmax, 11=J-Fzmin
    void Initialize()
    {
        for(int i=0; i<12; i++)
            dataBase[i].Initialize();
    }
};
struct T_CONCRETE_TP_CH_UNIT
{
    T_CONCRETE_TP_CH_UNIT()
    {
		dV             =D_UNITSYS_BASE_FORCE;
		dM             =D_UNITSYS_BASE_MOMENT;
		dshear         =D_UNITSYS_BASE_STRESS;
        dconcrete_shear=D_UNITSYS_BASE_STRESS;
    }
    int dV;      
	int dM;
    int dshear;     
    int dconcrete_shear;  
};

//钢筋压应力验算数据结构
struct REBAR_C
{ 
	BOOL bCHK;		           //是否验算的标识
	UINT Lcomk;//验算内力组合的ID
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
	double dM;//设计弯矩值
	double dN;//设计轴力值
    double drebar_c;//截面钢筋计算压应力
    double drebar;//截面钢筋容许压应力
	BOOL bOK;
    void Initialize()
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;
        dM=0;
	  dN=0;
		drebar_c=0;
        drebar=0;
		bOK = TRUE;
	}
};
struct T_BEAM_REBAR_C_CH
{ 
    REBAR_C dataBase[4];	// 
    void Initialize()  {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_REBAR_C_CH	       // ElemK.
{
    REBAR_C dataBase[8]; // 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 
	                               // 4=J-Fxmax, 5=J-Fxmin, 6=J-Mymax, 7=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<8; i++)
            dataBase[i].Initialize();
    }
};
struct T_REBAR_C_CH_UNIT
{
    T_REBAR_C_CH_UNIT()
    {
        dM             =D_UNITSYS_BASE_MOMENT;;
	  dN             =D_UNITSYS_BASE_FORCE;
		drebar_c       =D_UNITSYS_BASE_STRESS;
        drebar         =D_UNITSYS_BASE_STRESS;
    }
    int dM;      
    int dN;     
    int drebar_c;  
    int drebar;  
};
//钢筋拉应力验算数据结构
struct REBAR_S
{ 
	BOOL bCHK;		    //是否验算的标识
	UINT Lcomk;//验算内力组合的ID
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
    double dM;//设计弯矩值
	double dN;//设计轴力值
    double drebar_s;//截面钢筋计算拉应力
    double drebar;//截面钢筋容许拉应力
	BOOL bOK;
    void Initialize()
	{ 
		bCHK =0;   
		Lcomk=0;		    
		itype=0;            
		dM=0;			
		dN=0;		    
		drebar_s=0;            
		drebar=0;
		bOK = TRUE;
	}
};
struct T_BEAM_REBAR_S_CH
{ 
    REBAR_S dataBase[4];	// 
    void Initialize()  {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_COLUMN_REBAR_S_CH	        // ElemK.
{
	REBAR_S dataBase[8];  // 0=I-Fxmax, 1=I-Fxmin, 2=I-Mymax, 3=I-Mymin, 
	                               // 4=J-Fxmax, 5=J-Fxmin, 6=J-Mymax, 7=J-Mymin.
    void Initialize()
    {
        for(int i=0; i<8; i++)
            dataBase[i].Initialize();
    }
};
struct T_REBAR_S_CH_UNIT
{
    T_REBAR_S_CH_UNIT()
    {
        dM             =D_UNITSYS_BASE_MOMENT;;
	  dN             =D_UNITSYS_BASE_FORCE;
		drebar_s       =D_UNITSYS_BASE_STRESS;
        drebar         =D_UNITSYS_BASE_STRESS;
    }
    int dM;      
    int dN;     
    int drebar_s;  
    int drebar;  
};
//T形或工字形梁截面上翼缘相交处剪应力验算数据结构
struct BEAM_SHEAR_T
{ 
	BOOL bCHK;		           //是否验算的标识
	UINT Lcomk;//验算内力组合的ID
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 
	double dM;//弯矩
	double dV;//剪力
	double dshear;//中性轴处的剪应力 
	double dshear_t;//计算剪应力
	double dconcrete_shear;//容许剪应力	   
	BOOL bOK;
	void Initialize()	   
	{    
		bCHK =0;
		Lcomk=0;		    
		itype=0;			
		dM = dV = .0;
		dshear=0;		    
		dshear_t=0;           
		dconcrete_shear=0;	   
		bOK = TRUE;
	}
};
struct T_BEAM_SHEAR_T_CH
{ 
    BEAM_SHEAR_T dataBase[4];	// 
    void Initialize()  {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_BEAM_SHEAR_T_CH_UNIT
{
    T_BEAM_SHEAR_T_CH_UNIT()
    {
		dM             =D_UNITSYS_BASE_MOMENT;
		dV             =D_UNITSYS_BASE_FORCE;
		dshear         =D_UNITSYS_BASE_STRESS;		    
		dshear_t       =D_UNITSYS_BASE_STRESS;           
		dconcrete_shear=D_UNITSYS_BASE_STRESS;	   
     }
	int dM;
	int dV;
    int dshear;      
    int dshear_t;     
    int dconcrete_shear;  
};
//工字形梁截面下翼缘相交处剪应力验算数据结构
struct BEAM_SHEAR_B
{    
	BOOL bCHK;		           //是否验算的标识
	UINT Lcomk;//验算内力组合的ID	   
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
	double dM;//弯矩      
	double dV;//剪力
	double dshear;//中性轴处的剪应力 	   
	double dshear_b;//截面计算剪应力      
	double dconcrete_shear;//截面容许剪应力	   
	BOOL bOK;
	void Initialize()	   
	{ 
		bCHK = 0;
		Lcomk=0;
		itype=0;	
		dM = dV = .0;
		dshear=0;
		dshear_b=0;       
		dconcrete_shear=0;
		bOK = TRUE;
	}
};
struct T_BEAM_SHEAR_B_CH
{ 
    BEAM_SHEAR_B dataBase[4];	// 
    void Initialize()  {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};
struct T_BEAM_SHEAR_B_CH_UNIT
{
    T_BEAM_SHEAR_B_CH_UNIT()
    {
		dM             =D_UNITSYS_BASE_MOMENT;
		dV             =D_UNITSYS_BASE_FORCE;
		dshear         =D_UNITSYS_BASE_STRESS;		    
		dshear_b       =D_UNITSYS_BASE_STRESS;           
		dconcrete_shear=D_UNITSYS_BASE_STRESS;	   
     }
    int dM;
    int dV;
    int dshear;      
    int dshear_b;     
    int dconcrete_shear;  
};
//截面裂缝宽度验算数据结构
struct CRACK
{
	BOOL bCHK;		           //是否验算的标识
	UINT Lcomk;//验算内力组合的ID	   
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)	   
	int iKind;//1:受弯 2:偏心受压	   
	double drebar_s;//受拉钢筋重心处的钢筋应力      
	double dwf;//裂缝宽度计算值      
	double dconcrete_wf;//裂缝宽度容许值      
	BOOL bOK;
	void Initialize()   
	{    
		bCHK = 0;
		Lcomk=0;		  
		itype=0;			
		iKind=0;			
		drebar_s=0;		    
		dwf=0;           
		dconcrete_wf=0;
		bOK = TRUE;
	}
};
struct T_BEAM_CRACK_CH
{ 
    CRACK dataBase[4];	// 
    void Initialize()  {
        for(int i=0; i<4; i++)
            dataBase[i].Initialize();
    }
};

struct T_COLUMN_CRACK_CH
{ 
    CRACK dataBase[8];	// 
    void Initialize()  {
        for(int i=0; i<8; i++)
            dataBase[i].Initialize();
    }
};

struct T_CRACK_CH_UNIT
{
    T_CRACK_CH_UNIT()
    {
		drebar_s       =D_UNITSYS_BASE_STRESS;		    
		dwf            =D_UNITSYS_BASE_LENGTH;           
		dconcrete_wf   =D_UNITSYS_BASE_LENGTH;
     }
    int drebar_s;      
    int dwf;     
    int dconcrete_wf;  
};


struct STABILITY
{  
	BOOL bCHK;		           //是否验算的标识
	UINT Lcomk;//验算内力组合的ID   
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 	   
	double dN;     
	double dconcrete_s;//截面计算压应力      
	double dconcrete_c;//截面容许压应力      
	BOOL bOK;
	void Initialize()	   
	{   
		bCHK = 0;
		Lcomk=0;    
		itype=0;
		dN=0;
		dconcrete_s=0;	
		dconcrete_c=0;
		bOK = TRUE;
	}
};
struct T_STABILITY_CH
{ 
    STABILITY dataBase[2];	// 
    void Initialize()  {
        for(int i=0; i<2; i++)
            dataBase[i].Initialize();
    }
};
struct T_STABILITY_CH_UNIT
{
    T_STABILITY_CH_UNIT()
    {
		dN                =D_UNITSYS_BASE_FORCE;
		dconcrete_s       =D_UNITSYS_BASE_STRESS;	
		dconcrete_c       =D_UNITSYS_BASE_STRESS;
     }
    int dN;      
    int dconcrete_s;     
    int dconcrete_c;  
};

//偏心距e
struct ECCENTRICITY
{ 
    BOOL bCHK;		//是否计算的标识
	UINT Lcomk;//验算内力组合的ID
	int itype;//验算荷载的类型 ( FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11 )
                                 //               ( FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12) 

    double de;    //计算偏心距
    double de_alw;//容许偏心距
	BOOL bOK;
	void Initialize()
	{ 
        bCHK = 0;
		Lcomk=0;
		itype=0;
		de=0;
        de_alw=0;
		bOK = TRUE;
	}
};
struct T_ECCENTRICITY_CH	      // ElemK
{
    ECCENTRICITY dataBase[12];      // 0=I-Fxmin（My）, 1=I-Fxmin（Mz）, 2=I-Mymax, 3=I-Mymin, 4=I-Mzmax, 5=I-Mzmin, 
	                                // 6=J-Fxmin（My）, 7=J-Fxmin（Mz）, 8=J-Mymax, 9=J-Mymin, 10=J-Mzmax, 11=J-Mzmin, 
    void Initialize()
    {
        for(int i=0; i<12; i++)
            dataBase[i].Initialize();
    }
};
struct T_ECCENTRICITY_CH_UNIT
{
    T_ECCENTRICITY_CH_UNIT()
    {
        de       =D_UNITSYS_NONE; 
		de_alw   =D_UNITSYS_NONE; 
    }
    int de;      
    int de_alw;     
};
//延性比
struct DUCTILITY
{  
	BOOL bCHK;		           //是否验算的标识
	UINT Lcomk;//验算的位置ID   
	int itype;//验算荷载的类型 ( Dx=0, Dy=1,Dz=2, Rx=3,Ry=4, Rz=5)
	double du;    //计算延性比        
	double du_alw;//容许延性比   
	BOOL bOK;
	void Initialize()	   
	{   
		bCHK = 0;
		Lcomk=0;    
		itype=0;
		du=0;	
		du_alw=0;
		bOK = TRUE;
	}
};
struct T_DUCTILITY_CH
{ 
    DUCTILITY dataBase[6];	// 
    void Initialize()  {
        for(int i=0; i<6; i++)
            dataBase[i].Initialize();
    }
};
struct T_DUCTILITY_CH_UNIT
{
    T_DUCTILITY_CH_UNIT()
    {
		du       =D_UNITSYS_NONE; 
		du_alw   =D_UNITSYS_NONE; 
     }     
    int du;     
    int du_alw;  
};

///////
//Add by maxiao for china seismic design  2009.04
struct T_FOUND_CH_BASE//Foundation
{
    BOOL bCHK;		//汲拌搬苞蜡公
    UINT LcomK;
    int iMax;		     //MIN-MAX设计值 酒丛=0, FX-MAX=1, FY-MAX=3, FZ-MAX=5, MX-MAX=7, MY-MAX=9, MZ-MAX=11)
                                     //                      FX-MIN=2, FY-MIN=4, FZ-MIN=6, MX-MIN=8, MY-MIN=10, MZ-MIN=12)
    double dPu1;	//轴压承载力  [Unit=Stress]
    double dPu2;	//偏压承载力	[Unit=Stress]
    double dPn1;	//Fa    [Unit=Stress]
    double dPn2;	//γRFa	[Unit=Stress]

    BOOL bOK;
    void Initialize()
    {
        bCHK = FALSE;
        LcomK = 0;
        iMax = 0;
		dPu1 = dPu2 = .0;
		dPn1 = dPn2 = .0;
        bOK = TRUE;
    }
};
struct T_FOUND_CH	//ElemK.
{
    T_FOUND_CH_BASE FoundBase[2];	// 0=I, 1=J.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            FoundBase[i].Initialize();
    }
};

struct T_FOUND_CH_UNIT
{
    T_FOUND_CH_UNIT()
    {
        dPu1      = D_UNITSYS_BASE_STRESS;
        dPn1      = D_UNITSYS_BASE_STRESS;
        dPu2      = D_UNITSYS_BASE_STRESS;
        dPn2      = D_UNITSYS_BASE_STRESS;
    }
    int dPu1, dPu2;
    int dPn1, dPn2;
};

//
struct T_SEIS_LINK_DISP
{
	UINT LcomK;
	CString LcomName;
	double dXDx;
	double dXDy;
	double dXHx;
	double dXHy;
    void Initialize()
    {
		LcomK = 0;
		LcomName = "";
		dXDx = dXDy = dXHx = dXHy = 0.0;
	}
};

struct T_SEIS_LINK_FORCE 
{
	UINT LcomK;
	CString LcomName;
	double dRb;
	double dEhzex;
	double dEhzey;
	double dEhzdx;
	double dEhzdy;
    void Initialize()
    {
		LcomK = 0;
		LcomName = "";
		dRb = dEhzex = dEhzey = dEhzdx = dEhzdy = 0.0;
	}
};

struct T_SUPPORTDISP_CH	//ElemK.
{
    BOOL bCHK;		//是否验算
    int iKey;             //支座编号
    int iType;            //支座类型 0-一般连接；1-弹性连接
    UINT LcomK;           //抗震荷载组合
    double dHoriDispX;     //水平位移合计X-顺桥向
    double dHoriDispY;     //水平位移合计Y-横桥向
    double dThick;        //橡胶层总厚度
    BOOL bOK;             //是否满足验算要求
    void Initialize()
    {
        bCHK = FALSE;
        iKey = iType = 0;
        LcomK = 0;
        dHoriDispX = dHoriDispY = dThick = .0;
        bOK = TRUE;
    }
};

struct T_SUPPORTDISP_CH_UNIT
{
    T_SUPPORTDISP_CH_UNIT()
    {
		dHoriDispX = D_UNITSYS_BASE_LENGTH;  
		dHoriDispY  = D_UNITSYS_BASE_LENGTH;  
		dThick     = D_UNITSYS_BASE_LENGTH;  
    }
    int dHoriDispX;
    int dHoriDispY;
    int dThick    ;
};

struct T_SUPPORTFORCE_CH
{
    BOOL bCHK;		//是否验算
    int iKey;             //支座编号
    int iType;            //支座类型 0-一般连接；1-弹性连接
    UINT LcomK;           //抗震荷载组合
    double dHoriForceX;     //水平力合计X-顺桥向
    double dHoriForceY;     //水平力合计Y-横桥向
    double dAllowForce;    //水平力容许值
    BOOL bOK;             //是否满足验算要求
    void Initialize()
    {
        bCHK = FALSE;
        iKey = iType = 0;
        LcomK = 0;
        dHoriForceX = dHoriForceY = dAllowForce = .0;
        bOK = TRUE;
    }
};

struct T_SUPPORTFORCE_CH_UNIT
{
    T_SUPPORTFORCE_CH_UNIT()
    {
		dHoriForceX  = D_UNITSYS_BASE_FORCE;  
		dHoriForceY  = D_UNITSYS_BASE_FORCE;  
		dAllowForce = D_UNITSYS_BASE_FORCE;  
    }
    int dHoriForceX;
    int dHoriForceY;
    int dAllowForce;
};
//
struct T_FRUSTATOPDISP_CH_BASE
{
    BOOL bCHK;		//是否验算
    int iKey;             //节点号
    UINT LcomK;           //抗震荷载组合
    int iDirect;          //支座变形方向0-Y；1-Z
    double dHoriDisp;     //水平位移
    double dAlwDisp;      //容许位移
    BOOL bOK;             //是否满足验算要求
    void Initialize()
    {
        bCHK = FALSE;
        iKey = LcomK = iDirect = 0;
        dHoriDisp = dAlwDisp = .0;
        bOK = TRUE;
    }
};

struct T_FRUSTATOPDISP_CH	//ElemK.
{
    T_FRUSTATOPDISP_CH_BASE DataBase[2];	// 0=X, 1=Y.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            DataBase[i].Initialize();
    }
};

struct T_FRUSTATOPDISP_CH_UNIT
{
    T_FRUSTATOPDISP_CH_UNIT()
    {
		dHoriDisp  = D_UNITSYS_BASE_LENGTH;  
		dAlwDisp = D_UNITSYS_BASE_LENGTH;  
    }
    int dHoriDisp ;
    int dAlwDisp ;
};


struct T_CORNERDISP_CH_BASE
{
    BOOL bCHK;		//是否验算
    int iKey;             //节点号
    UINT LcomK;           //抗震荷载组合
    int iDirect;          //支座变形方向0-Ry；1-Rz
    double dHoriDisp;     //转角位移
    double dAlwDisp;      //容许位移
    BOOL bOK;             //是否满足验算要求
    void Initialize()
    {
        bCHK = FALSE;
        iKey = LcomK = iDirect = 0;
        dHoriDisp = dAlwDisp = .0;
        bOK = TRUE;
    }
};

struct T_CORNERDISP_CH	//ElemK.
{
    T_CORNERDISP_CH_BASE DataBase[2];	// 0=X, 1=Y.
    void Initialize()
    {
        for(int i=0; i<2; i++)
            DataBase[i].Initialize();
    }
};
//////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif



