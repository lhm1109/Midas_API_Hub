#include "stdafx.h"
#include "BaseTreeCtrl.h"

IMPLEMENT_DYNAMIC(CBaseTreeCtrl, SECTreeCtrl);
CBaseTreeCtrl::CBaseTreeCtrl(void)
{
}


CBaseTreeCtrl::~CBaseTreeCtrl(void)
{
}

void CBaseTreeCtrl::DoPaint(CDC* pDC)
{
	LvPaintContext* pPC;
	CRect rectInside(0,0,0,0),	//Inside Rectangle  = "client rect" - "width of vertical scroll" - "height of horizontal scroll" - "height of header"
		rectClient(0,0,0,0),  //Complete client area
		rectFill(0,0,0,0),    //Area to fill with background color.
		rectErase(0,0,0,0);   //small corner intersection of vertical and horizontal scroll bar ( lower right hand corner )

	GetInsideRect(rectInside);
	SECGetClientRect(rectClient);

	if ( pDC->IsPrinting() )
	{
		// Initialize Paint Context
		pPC = CreatePaintContext(pDC);
		pPC->da = DrawEntire;
		pPC->pdc = pDC;
		pPC->dwViewStyle = GetListCtrlStyle() & LVS_TYPEMASK;


		//Draw the header control...
		if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_REPORT 
			&& IsHeaderCtrlEnabled())
		{
			DrawHeader( pPC );				
		}


		DrawInvalidItems(pPC);

	}
	else	// not printing
	{
		CDC* pTempDC = NULL; // scratch pad dc

		BOOL bDelete = TRUE;
		BOOL bHScroll = !!(m_dwScrollStyle & WS_HSCROLL);
		BOOL bVScroll = !!(m_dwScrollStyle & WS_VSCROLL);


		//We only fill in the item area...normally we would want to fill the header as well,
		// but since the header can extend off the screen we don't want to confine it to our normal clipping rect...
		rectInside.DeflateRect(1, 0, -1, 0);
		
		rectFill = rectInside;
		rectFill.top = rectClient.top;

		//If I don't have a Horizontal scroll bar, I want my rectFill to extend all
		// the way to the bottom
		if( !bHScroll )
		{
			rectFill.bottom = rectClient.bottom;
		}

		//If I don't have a Vertical scroll bar, I want my rectFill to extend all
		// the way to the right
		if( !bVScroll )
		{
			rectFill.right = rectClient.right;
		}

		// If I have both a hscroll and vscroll, setup the rectangular area 
		//  where the scrollbars meet.
		if( bHScroll && bVScroll )
		{
			rectErase.left = rectInside.right + 1;
			rectErase.right = rectClient.right;
			rectErase.top = rectInside.bottom + 1;
			rectErase.bottom = rectClient.bottom;
		}

		//Create the memdc SEC_LISTBASEd on the fill rectangle.  That way there will be no flicker behind
		//  the scroll bars when they redraw ( hopefully :-)
		bDelete = CreateMemDC( pDC, pTempDC, rectFill );

		pTempDC->FillSolidRect( rectFill, GetBkColor() );

		// Initialize Paint Context
		pPC = CreatePaintContext(pTempDC);
		pPC->da = DrawEntire;
		pPC->pdc = pTempDC;
		pPC->dwViewStyle = GetListCtrlStyle() & LVS_TYPEMASK;

		//Draw the header control...
		if( (GetListCtrlStyle() & LVS_TYPEMASK) == LVS_REPORT 
			&& IsHeaderCtrlEnabled())
		{
			DrawHeader(pPC);

		}

		// We have to do this so drawn text doesn't overwrite the
		// vertical scrollbar which makes it flicker.
		pTempDC->IntersectClipRect(rectInside);

		// And finally, draw the contents of the list control
		DrawInvalidItems(pPC);

		if( bDelete ) 
			delete pTempDC;

		//All non-memdc drawing goes here...

		// Erase the rectangular area where the horizontal and vertical
		// scrollbars meet.  Must Erase with the screen dc and not mem dc because mem dc is created
		// SEC_LISTBASEd on rectFill which may not include this area.
		if ( bHScroll && bVScroll )
		{
			pDC->FillSolidRect(rectErase, ::GetSysColor(COLOR_BTNFACE));
		}
	}
	// Delete the paint context
	delete pPC;
}
