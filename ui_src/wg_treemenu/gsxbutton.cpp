// gsxbutton.cpp : implementation file
#include "stdafx.h"
#include "gsxbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GSxButton
GSxButton::GSxButton()
{
	m_bColorTab = FALSE;

	m_hIcon = 0;
	
	m_hBitmap = 0;
		m_hBitmapDisabled = 0;

	m_bDefault = FALSE;
	m_nOldState = 0;
	m_nOldAction = 0;

	m_nImageOffsetFromBorder = 4;
	m_nTextOffsetFromImage = 8;
	m_bUseOffset = TRUE;
	m_crTransparentMask = RGB(255,255,255);
	m_bIsTransparentBitmap = FALSE;
	
}

GSxButton::~GSxButton()
{
	//AfxMessageBox(" ~GSxButton() ");
	//VERIFY(::DeleteObject(m_hBitmap));
	//VERIFY(::DeleteObject(m_hBitmapDisabled));
	if(m_hBitmap != 0)
		::DeleteObject(m_hBitmap);
	if(m_hBitmapDisabled != 0)
		::DeleteObject(m_hBitmapDisabled);
	if(m_hIcon != 0)
		::DeleteObject(m_hIcon);
}


BEGIN_MESSAGE_MAP(GSxButton, CButton)
	//{{AFX_MSG_MAP(GSxButton)
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL GSxButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CButton::PreCreateWindow(cs);
}

inline void GSxButton::Redraw()
{
	if( m_hWnd != NULL )
		Invalidate();
}

void GSxButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButton::PreSubclassWindow();
		// Add Routine
	ModifyStyle(0, BS_OWNERDRAW);
}

//
//	Various Attribute setting functions
//

//	Image functions
BOOL GSxButton::SetIcon( UINT nID, int nWidth, int nHeight )
{
	m_hIcon = (HICON)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(nID), IMAGE_ICON, nWidth, nHeight, 0);

	if( m_hIcon == 0 )
		return FALSE;

	m_nImageWidth = nWidth;
	m_nImageHeight = nHeight;

	m_hBitmap = 0;

	Redraw();

	return TRUE;
}

BOOL GSxButton::SetBitmap( UINT nID, int nWidth, int nHeight )
{
	ASSERT(m_hBitmap == 0 && m_hBitmapDisabled == 0);
	return SetBitmapCommon( nID, nWidth, nHeight, 0, FALSE /* no mask */ );
}

BOOL GSxButton::SetMaskedBitmap( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask )
{
	ASSERT(m_hBitmap == 0 && m_hBitmapDisabled == 0);
		if( m_hBitmap != 0) return TRUE;

	m_MaskedBitmapID = nID;
	m_crTransparentMask = crTransparentMask;
	m_bIsTransparentBitmap = TRUE;
	return SetBitmapCommon( nID, nWidth, nHeight, crTransparentMask, TRUE /* mask */ );
}

void GSxButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();
	
	// TODO: Add your message handler code here
	if(m_bIsTransparentBitmap)
	{
		::DeleteObject(m_hBitmap);
		::DeleteObject(m_hBitmapDisabled);
		m_hBitmap = 0;
		m_hBitmapDisabled = 0;
		SetMaskedBitmap(m_MaskedBitmapID,m_nImageWidth,m_nImageHeight,m_crTransparentMask);
	}
}

BOOL GSxButton::SetBitmapCommon( UINT nID, int nWidth, int nHeight, COLORREF crTransparentMask, BOOL bUseMask )
{
	// If it is not a masked bitmap, just go through the
	// motions as if it was, but set then number of color mappings to 0
	COLORMAP	mapColor;
	mapColor.from = crTransparentMask;
	mapColor.to  = ::GetSysColor( COLOR_BTNFACE );
	HBITMAP bmTemp = (HBITMAP)::CreateMappedBitmap(AfxGetApp()->m_hInstance, nID, IMAGE_BITMAP, &mapColor, bUseMask ? 1 : 0 );
	m_hBitmap = (HBITMAP)::CopyImage( bmTemp, IMAGE_BITMAP, nWidth, nHeight, 0 );
		::DeleteObject(bmTemp);
	// Create disabled bitmap.  We need to make the masked area white so
	// it will appear transparent when the bitmap is rendered as an
	// 'embossed' (disabled) image in DrawItem() below.  Since DrawState 
	// converts the image to monochrome, white is transparent, black is 
	// graphics data.
	mapColor.to  = RGB( 255, 255, 255 );
	bmTemp = (HBITMAP)::CreateMappedBitmap(AfxGetApp()->m_hInstance, nID, IMAGE_BITMAP, &mapColor, bUseMask ? 1 : 0 );
	m_hBitmapDisabled = (HBITMAP)::CopyImage( bmTemp, IMAGE_BITMAP, nWidth, nHeight, 0 );
		::DeleteObject(bmTemp);
	if( m_hBitmap == 0 || m_hBitmapDisabled == 0 )
		return FALSE; 

	m_nImageWidth = nWidth;
	m_nImageHeight = nHeight;

	m_hIcon = 0;

	Redraw();

	return TRUE;
}

void GSxButton::SetColorTab(COLORREF crTab)
{	
	m_bColorTab = TRUE;
	m_crColorTab = crTab;

	Redraw();
}

BOOL GSxButton::SetDefaultButton( BOOL bState )
{
	CDialog *pDialog = (CDialog *)GetOwner();
	ASSERT( pDialog->IsKindOf( RUNTIME_CLASS( CDialog ) ) );

	pDialog->SetDefID( GetDlgCtrlID() );

	BOOL bPrevious = m_bDefault;
	m_bDefault = bState;

	Redraw();

	// Return previous state
	return bPrevious;
}

//	Positioning Functions
int GSxButton::SetImageOffset( int nPixels )
{
	int nPrevious = m_nImageOffsetFromBorder;

	m_bUseOffset = TRUE; 
	m_nImageOffsetFromBorder = nPixels; 

	Redraw();

	return nPrevious;
}

int GSxButton::SetTextOffset( int nPixels ) 
{ 
	int nPrevious = m_nTextOffsetFromImage;

	m_bUseOffset = TRUE; 
	m_nTextOffsetFromImage = nPixels; 

	Redraw();

	return nPrevious;
}

CPoint GSxButton::SetImagePos( CPoint p ) 
{ 
	CPoint pointPrevious = m_pointImage;

	m_bUseOffset = FALSE; 
	m_pointImage = p; 

	Redraw();

	return pointPrevious;
}
CPoint GSxButton::SetTextPos( CPoint p ) 
{ 
	CPoint pointPrevious = m_pointText;

	m_bUseOffset = FALSE; 
	m_pointText = p; 

	Redraw();

	return pointPrevious;
}

//	Centering a point helper function
void GSxButton::CheckPointForCentering( CPoint &p, int nWidth, int nHeight )
{
	CRect	rectControl;
	GetClientRect( rectControl );

	if( p.x == GSXBUTTON_CENTER )
		p.x = ( ( rectControl.Width() - nWidth ) >> 1 );
	if( p.y == GSXBUTTON_CENTER )
		p.y = ( ( rectControl.Height() - nHeight ) >> 1 );
}


//
//	Owner Draw function, the grand-daddy
//
void GSxButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{	
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	CRect	rectControl( lpDIS->rcItem );
	
	UINT	nOffset = 0;			// For position adjustment of a pushed button
	UINT	nFrameStyle=0;
	BOOL	bDRAWFOCUSONLY = FALSE;	// Optimize a bit
	int		nStateFlag;				// Normal or Disabled
	HBITMAP	hBitmapToDraw;			// Normal or Disabled bitmap (not used if uses icon)
	
	UINT	nNewState = lpDIS->itemState;
	UINT	nNewAction = lpDIS->itemAction;
	
	//	Find out what state the control and set some drawing flags 
	//	according to the state.
		if ( nNewState & ODS_SELECTED)
		{
			nFrameStyle = DFCS_PUSHED;
			nOffset += 1;
		}

		if( nNewState & ODS_DISABLED )
		{
			nStateFlag = DSS_DISABLED;
			hBitmapToDraw = m_hBitmapDisabled;
		}
		else
		{
			nStateFlag = DSS_NORMAL;
			hBitmapToDraw = m_hBitmap;
		}

		// If only the focus is changing, don't redraw the whole control
		if (nNewAction == ODA_FOCUS )
			bDRAWFOCUSONLY = TRUE;
	
	// If this is the defualt button, deflate the control so everything 
	// we do below (icon, text, focus ) is adjusted properly
	if( m_bDefault )
		rectControl.DeflateRect( 1, 1 );

	if( !bDRAWFOCUSONLY )
	{
		//
		// Draw 'default button' rectangle
		//
		if( m_bDefault ) // Can't use ODS_DEFAULT w/owner draw!!
		{
			CPen *pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
							
			pDC->Rectangle( &lpDIS->rcItem ); // don't use deflated rectangle
			pDC->SelectObject( pOldPen );
		}

		//
		//	Draw button frame
		//
		pDC->DrawFrameControl(&rectControl, DFC_BUTTON, DFCS_BUTTONPUSH | nFrameStyle);

		//
		//	Draw color tab
		//
		if (m_bColorTab)
		{
			CPen penTab; 
			
			#define COLORTABSIZE 8
			if( penTab.CreatePen( PS_SOLID, 1, m_crColorTab) )
			{
				CPen* pOldPen = pDC->SelectObject( &penTab );
							
				int nXOffset = rectControl.left+1 + nOffset;
				int nYOffset = rectControl.top+1 + nOffset;
				for (UINT nStep = 0; nStep < COLORTABSIZE; nStep++)
				{
					pDC->MoveTo( nXOffset, nYOffset + nStep );
					pDC->LineTo( nXOffset + (COLORTABSIZE-nStep)-1, nYOffset + nStep );			
				}
				
				pDC->SelectObject( pOldPen );
				penTab.DeleteObject();
			}
		}

		//	Get control text
		CString		strTitle;
		this->GetWindowText(strTitle);

		//
		//	Draw Image
		//
		if( HasImage() )
		{
			CPoint pt;

			if( m_bUseOffset )
			{
				pt.x = strTitle.IsEmpty() ? GSXBUTTON_CENTER : rectControl.left + m_nImageOffsetFromBorder;
				pt.y = GSXBUTTON_CENTER;
			}
			else
				pt = m_pointImage;

			CheckPointForCentering( pt, m_nImageWidth, m_nImageHeight );
			
			pt.Offset( nOffset, nOffset );

			if( m_hIcon )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HICON)m_hIcon, DST_ICON | nStateFlag, (CBrush *)NULL );
			else if( m_hBitmap )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HBITMAP)hBitmapToDraw, DST_BITMAP | nStateFlag );
		}

		//	
		//	Draw Text
		//
		if ( !strTitle.IsEmpty() )
		{
			CPoint	pt;
			CSize	sizeText = pDC->GetTextExtent(strTitle);

			if( m_bUseOffset )
			{
				pt.x = !HasImage() ? GSXBUTTON_CENTER : m_nImageWidth + m_nTextOffsetFromImage + m_nImageOffsetFromBorder;
				pt.y = GSXBUTTON_CENTER; 
			}
			else
				pt = m_pointText;

			//	If we are centering the text vertically, it looks best of we
			//	center based on the height of the text, then move it up 1 more pixel
			int nOffsetFixY = pt.y == GSXBUTTON_CENTER ? -1 : 0;

			CheckPointForCentering( pt, sizeText.cx, sizeText.cy );

			pt.Offset( nOffset, nOffset + nOffsetFixY );

			pDC->DrawState( pt, CSize(0,0), strTitle, DST_PREFIXTEXT|nStateFlag, TRUE, 0, (CBrush*)NULL );
		}

	} // End !focus only

	//
	//	Draw focus rectange
	//
	if( !( nNewState & ODS_DISABLED ) )	// Make sure it's not disabled
	{
		// Redraw the focus if:
		//		1. There is a change in focus state 
		// OR	2. The entire control was just redrawn and Focus is set
		if( ( nNewState && ODS_FOCUS ) ^ ( m_nOldState && ODS_FOCUS ) ||
			( !bDRAWFOCUSONLY && ( nNewState & ODS_FOCUS ) ) )
		{
			#define FOCUSOFFSET 3
			CRect rect( rectControl );

			// As control gets smaller, decrease focus size
			int nDeflate = min( FOCUSOFFSET,
								min( rect.Width(), rect.Height() ) >> 2 );
			rect.DeflateRect( nDeflate, nDeflate);
			pDC->DrawFocusRect(&rect);
		}
	}

	m_nOldAction = nNewAction;
	m_nOldState = nNewState;
}

/////////////////////////////////////////////////////////////////////////////
// CButtonBitmap

CButtonBitmap::CButtonBitmap()
{
//	m_bCurrentStatus = FALSE;
//	m_IDActiveBitmap = 0;
//   m_IDInActiveBitmap = 0;
}

CButtonBitmap::~CButtonBitmap()
{
}

#define CButton GSxButton
BEGIN_MESSAGE_MAP(CButtonBitmap, CButton)
#undef CButton
	//{{AFX_MSG_MAP(CButtonBitmap)
	ON_WM_SETFOCUS()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CButtonBitmap message handlers

void CButtonBitmap::OnSetFocus(CWnd* pOldWnd) 
{
	GSxButton::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
}

void CButtonBitmap::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	CRect	rectControl( lpDIS->rcItem );
	
	UINT	nOffset = 0;			// For position adjustment of a pushed button
	UINT	nFrameStyle=0;
	BOOL	bDRAWFOCUSONLY = FALSE;	// Optimize a bit
	int		nStateFlag;				// Normal or Disabled
	HBITMAP	hBitmapToDraw;			// Normal or Disabled bitmap (not used if uses icon)
	
	UINT	nNewState = lpDIS->itemState;
	UINT	nNewAction = lpDIS->itemAction;
	
	//	Find out what state the control and set some drawing flags 
	//	according to the state.
	if ( nNewState & ODS_SELECTED)
	{
		nFrameStyle = DFCS_PUSHED;
		nOffset += 1;
	}

	if( nNewState & ODS_DISABLED )
	{
		nStateFlag = DSS_DISABLED;
		hBitmapToDraw = m_hBitmapDisabled;
	}
	else
	{
		nStateFlag = DSS_NORMAL;
		hBitmapToDraw = m_hBitmap;
	}
		
	// If only the focus is changing, don't redraw the whole control
	if (nNewAction == ODA_FOCUS )
		bDRAWFOCUSONLY = TRUE;
		
	// If this is the defualt button, deflate the control so everything 
	// we do below (icon, text, focus ) is adjusted properly
	if( m_bDefault )
		rectControl.DeflateRect( 1, 1 );

	if( !bDRAWFOCUSONLY )
	{
		//
		// Draw 'default button' rectangle
		//
		if( m_bDefault ) // Can't use ODS_DEFAULT w/owner draw!!
		{
			//CPen *pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);
							
			//pDC->Rectangle( &lpDIS->rcItem ); // don't use deflated rectangle
			//pDC->SelectObject( pOldPen );
		}

		//
		//	Draw button frame
		//
		//pDC->DrawFrameControl(&rectControl, DFC_BUTTON, DFCS_BUTTONPUSH | nFrameStyle);

		//
		//	Draw color tab
		//
		if (m_bColorTab)
		{
			CPen penTab; 
			
			#define COLORTABSIZE 8
			if( penTab.CreatePen( PS_SOLID, 1, m_crColorTab) )
			{
				CPen* pOldPen = pDC->SelectObject( &penTab );
							
				int nXOffset = rectControl.left+1 + nOffset;
				int nYOffset = rectControl.top+1 + nOffset;
				for (UINT nStep = 0; nStep < COLORTABSIZE; nStep++)
				{
					//pDC->MoveTo( nXOffset, nYOffset + nStep );
					//pDC->LineTo( nXOffset + (COLORTABSIZE-nStep)-1, nYOffset + nStep );			
				}
				
				pDC->SelectObject( pOldPen );
				penTab.DeleteObject();
			}
		}

		//	Get control text
		CString		strTitle;
		this->GetWindowText(strTitle);

		//
		//	Draw Image
		//
		if( HasImage() )
		{
			CPoint pt;

			if( m_bUseOffset )
			{
				pt.x = strTitle.IsEmpty() ? GSXBUTTON_CENTER : rectControl.left + m_nImageOffsetFromBorder;
				pt.y = GSXBUTTON_CENTER;
			}
			else
				pt = m_pointImage;

			CheckPointForCentering( pt, m_nImageWidth, m_nImageHeight );
			
			pt.Offset( nOffset, nOffset );

			if( m_hIcon )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HICON)m_hIcon, DST_ICON | nStateFlag, (CBrush *)NULL );
			else if( m_hBitmap )
				pDC->DrawState( pt, CSize(m_nImageWidth, m_nImageHeight), (HBITMAP)hBitmapToDraw, DST_BITMAP | nStateFlag );
		}

		//	
		//	Draw Text
		//
		if ( !strTitle.IsEmpty() )
		{
			CPoint	pt;
			CSize	sizeText = pDC->GetTextExtent(strTitle);

			if( m_bUseOffset )
			{
				pt.x = !HasImage() ? GSXBUTTON_CENTER : m_nImageWidth + m_nTextOffsetFromImage + m_nImageOffsetFromBorder;
				pt.y = GSXBUTTON_CENTER; 
			}
			else
				pt = m_pointText;

			//	If we are centering the text vertically, it looks best of we
			//	center based on the height of the text, then move it up 1 more pixel
			int nOffsetFixY = pt.y == GSXBUTTON_CENTER ? -1 : 0;

			CheckPointForCentering( pt, sizeText.cx, sizeText.cy );

			pt.Offset( nOffset, nOffset + nOffsetFixY );

			//pDC->DrawState( pt, CSize(0,0), strTitle, DST_PREFIXTEXT|nStateFlag, TRUE, 0, (CBrush*)NULL );
		}

	} // End !focus only

	//
	//	Draw focus rectange
	//
	if( !( nNewState & ODS_DISABLED ) )	// Make sure it's not disabled
	{
		// Redraw the focus if:
		//		1. There is a change in focus state 
		// OR	2. The entire control was just redrawn and Focus is set
		if( ( nNewState && ODS_FOCUS ) ^ ( m_nOldState && ODS_FOCUS ) ||
			( !bDRAWFOCUSONLY && ( nNewState & ODS_FOCUS ) ) )
		{
			#define FOCUSOFFSET 3
			CRect rect( rectControl );

			// As control gets smaller, decrease focus size
			int nDeflate = min( FOCUSOFFSET,
								min( rect.Width(), rect.Height() ) >> 2 );
			rect.DeflateRect( nDeflate, nDeflate);
			//pDC->DrawFocusRect(&rect);
		}
	}

	m_nOldAction = nNewAction;
	m_nOldState = nNewState;
}
/*
void CButtonBitmap::SetBitmaps(UINT IDBitmapActive,UINT IDBitmapInActive,COLORREF mask)
{
	SetBitmap(IDBitmapInActive,16,16);
	SetMaskedBitmap(IDBitmapInActive,16,16,mask);
		m_IDActiveBitmap = IDBitmapActive;
		m_IDInActiveBitmap = IDBitmapInActive;
}

void CButtonBitmap::SetActive()
{
	SetBitmap(m_IDActiveBitmap,16,16);
	SetMaskedBitmap(m_IDActiveBitmap,16,16,WHITE);
		m_bCurrentStatus = TRUE;
	Invalidate();
}

void CButtonBitmap::SetInActive()
{
	SetBitmap(m_IDInActiveBitmap,16,16);
	SetMaskedBitmap(m_IDInActiveBitmap,16,16,WHITE);
		m_bCurrentStatus = FALSE;
	Invalidate();
}

BOOL CButtonBitmap::GetCurrentStatus()
{
	 return m_bCurrentStatus;
}
*/
void CButtonBitmap::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	DrawItem(lpDrawItemStruct);
}

