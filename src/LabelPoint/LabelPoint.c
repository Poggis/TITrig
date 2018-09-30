//
// Created by Decimation on 9/30/2018.
//

#include "LabelPoint.h"
#include "../Library.h"
#include "../Settings.h"


void lp_ZeroLabel(const labelpoint_t* p)
{
	lib_MemZero(p->label, kLabelPointLabelSize);
}

void lp_Clear(const labelpoint_t* p)
{
	const int w = gfx_GetStringWidth(p->label);
	gfx_SetColor(gfx_white);
	gfx_FillRectangle(p->point.x, p->point.y, w, 9);
	gfx_SetColor(gfx_blue);
}

void lp_Print(const labelpoint_t* p)
{
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
}

void lp_SetLabel(const labelpoint_t* p, const char* s)
{
	lp_Clear(p);
	lp_ZeroLabel(p);
	strncpy(p->label, s, strlen(s));
}

real_t io_gfx_ReadSqrt(labelpoint_t* point)
{
	real_t      tmp, initialValue;
	uint8_t     key;
	int         i       = 0;
	const int   strW    = gfx_GetStringWidth(point->label);
	static char chars[] = "\0\0\0\0\0\0\0\0\0\0\"-RMH\0\0?[69LG\0\0.258KFC\0 147JEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
	chars[33] = '0';
	chars[18] = '3';

	if (strlen(point->label) != 0) {
		initialValue = os_StrToReal(point->label, NULL);
	}
	else {
		initialValue = os_Int24ToReal(1);
	}

	lp_Clear(point);
	lp_Print(point);

	gfx_Line(point->point.x + strW, point->point.y + 3, point->point.x + strW + 2, point->point.y + 7);

	gfx_VertLine(point->point.x + strW + 3, point->point.y - 3, 11);

	gfx_HorizLine(point->point.x + strW + 3, point->point.y - 3, strW + 3);

	//gfx_Clear(point);
	lp_ZeroLabel(point);

	while ((key = os_GetCSC()) != sk_Enter) {
		if (key == sk_Del) {
			lp_Clear(point);
			point->label[--i] = '\0';
			lp_Clear(point);
			lp_Print(point);
		}

		else if (chars[key] && i + 1 <= g_digitThreshold) {
			point->label[i++] = chars[key];
			gfx_PrintStringXY(point->label, point->point.x + strW + 6, point->point.y);
		}
		gfx_HorizLine(point->point.x + strW + 3, point->point.y - 3, gfx_GetStringWidth(point->label) + 3);
	}

	gfx_SetColor(gfx_white);
	gfx_FillRectangle(point->point.x, point->point.y - 3, gfx_GetStringWidth(point->label) + strW + 6,
					  point->point.y + 3);
	gfx_SetColor(gfx_black);

	tmp = os_StrToReal(point->label, NULL);
	tmp = os_RealSqrt(&tmp);
	return os_RealMul(&initialValue, &tmp);
}

real_t io_gfx_ReadReal(labelpoint_t* point)
{
	bool        isNeg   = false;
	uint8_t     key, i  = 0;
	real_t      rbuffer;
	static char chars[] = "\0\0\0\0\0\0\0\0\0\0\"-RMH\0\0?[69LG\0\0.258KFC\0 147JEB\0\0XSNIDA\0\0\0\0\0\0\0\0";
	lp_Clear(point);
	lib_MemZero(point->label, 20);
	chars[33] = '0';
	chars[18] = '3';

	while ((key = os_GetCSC()) != sk_Enter) {

		if (key == sk_Del) {
			lp_Clear(point);
			point->label[--i] = '\0';
			lp_Clear(point);
			lp_Print(point);
		}

		if (key == 0x11) // todo: remove negative number support in GFX as our triangles can't have signed values
		{
			dbg_sprintf(dbgout, "Negative sign detected\n");
			point->label[i++] = char_Neg;
			lp_Clear(point);
			lp_Print(point);
			isNeg = true;
		}

		if (chars[key] == 'I') {

			rbuffer = io_gfx_ReadSqrt(point);
			return rbuffer;
		}

		else if (chars[key] && i + 1 <= g_digitThreshold) {
			point->label[i++] = chars[key];
		}
		lp_Print(point);
		gfx_HorizLine(point->point.x, point->point.y + 8, gfx_GetStringWidth(point->label));
	}

	rbuffer = os_StrToReal(point->label, NULL);
	if (isNeg) rbuffer = os_RealNeg(&rbuffer);

	/**/
	//lib_StrReplace(point->label, char_Neg, '-');
	//dbg_sprintf(dbgout, "[IO In] %s\n", point->label);

	return rbuffer;
}

bool lp_Equals(const labelpoint_t a, const labelpoint_t b)
{
	return (bool) (a.point.x == b.point.x && a.point.y == b.point.y);
}

void lp_SetFocus(labelpoint_t** currSel, labelpoint_t* from, labelpoint_t* to)
{
	lp_ClearHighlight(from);
	lp_HighlightPoint(to);
	*currSel = to;
	dbg_sprintf(dbgout, "[GFX] %s -> %s\n", from->label, to->label);
}

void lp_ClearHighlight(const labelpoint_t* p)
{
	lp_Clear(p);
	lp_PrintColor(p, gfx_black);
}

void lp_PrintColor(const labelpoint_t* p, uint8_t color)
{
	gfx_SetTextFGColor(color);
	gfx_PrintStringXY(p->label, p->point.x, p->point.y);
	gfx_SetTextFGColor(gfx_black);
}

void lp_HighlightPoint(const labelpoint_t* p)
{
	lp_PrintColor(p, gfx_red);
}