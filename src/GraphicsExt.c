//
// Created by Decimation on 4/27/2018.
//

#include "GraphicsExt.h"
#include "System.h"
#include "/LabelPoint/LabelPoint.h"
#include "Simplifiers.h"
#include <string.h>

void gfx_ClearFraction(gfx_point_t point)
{
	static const uint8_t w = 60;
	static const uint8_t h = 35;
	point.x -= 1;
	point.y -= 16;
	gfx_ClearArea(point, w, h);
}

void gfx_DispFunctionRatioRoot(gfx_point_t pt, real_t x, real_t y)
{
	int24_t rad[2], rad2[2];

	sp_SimplifyRadicalFromDecimal(x, rad);
	sp_SimplifyRadicalFromDecimal(y, rad2);
	gfx_RadicalFraction(pt, rad[0], rad[1], rad2[0], rad2[1]);
}


void gfx_ClearArea(gfx_point_t point, uint8_t w, uint8_t h)
{
	gfx_SetColor(gfx_white);
	gfx_FillRectangle(point.x, point.y, w, h);
	gfx_SetColor(gfx_black);
}

void gfx_RadicalFraction(gfx_point_t point, int24_t numOuter, int24_t numInner, int24_t denomOuter, int24_t denomInner)
{

	char numInnerBuf[10], numOuterBuf[10];
	char denomInnerBuf[10], denomOuterBuf[10];

	int numLength, denomLength;
	dbg_sprintf(dbgout, "gfx - rad frac\n");
	sprintf(numOuterBuf, "%d", numOuter);
	sprintf(numInnerBuf, "%d", numInner);

	sprintf(denomOuterBuf, "%d", denomOuter);
	sprintf(denomInnerBuf, "%d", denomInner);

	numLength   = (strlen(numOuterBuf) + strlen(numInnerBuf));
	denomLength = (strlen(denomOuterBuf) + strlen(denomInnerBuf));

	//todo: center the fractions in these two cases
	if (numLength > denomLength) {
		gfx_HorizLine(point.x - 1, point.y, gfx_GetStringWidth(numInnerBuf) + gfx_GetStringWidth(numOuterBuf) + 7);


	}
	else if (numLength < denomLength) {
		gfx_HorizLine(point.x - 1, point.y, gfx_GetStringWidth(denomInnerBuf) + gfx_GetStringWidth(denomOuterBuf) + 7);


	}
	else {
		gfx_HorizLine(point.x - 1, point.y, gfx_GetStringWidth(numInnerBuf) + gfx_GetStringWidth(numOuterBuf) + 7);


	}

	point.y -= 9;
	gfx_Sqrt(point, numOuter, numInner);

	point.y += 14;
	gfx_Sqrt(point, denomOuter, denomInner);
}

void gfx_Fraction(gfx_point_t point, real_t num, real_t denom)
{
	char numbuf[15];
	char denombuf[15];
	os_RealToStr(numbuf, &num, 7, 0, 6);
	os_RealToStr(denombuf, &denom, 7, 0, 6);
	gfx_FractionStr(point, numbuf, denombuf);
}

void gfx_FractionStr(gfx_point_t point, const char* num, const char* denom)
{
	if (strlen(num) > strlen(denom)) {
		gfx_HorizLine(point.x - 1, point.y + 3, gfx_GetStringWidth(num) + 1);

		gfx_PrintStringXY(denom, point.x + ((gfx_GetStringWidth(denom) + 1) / 2), point.y + 5);
		gfx_PrintStringXY(num, point.x, point.y - 5);
	}
	else if (strlen(num) < strlen(denom)) {
		gfx_HorizLine(point.x - 1, point.y + 3, gfx_GetStringWidth(denom) + 1);

		gfx_PrintStringXY(num, point.x + ((gfx_GetStringWidth(num) + 1) / 2), point.y - 5);
		gfx_PrintStringXY(denom, point.x, point.y + 5);
	}
	else {
		gfx_HorizLine(point.x - 1, point.y + 3, gfx_GetStringWidth(denom) + 1);

		gfx_PrintStringXY(num, point.x, point.y - 5);
		gfx_PrintStringXY(denom, point.x, point.y + 5);
	}
}

void gfx_Sqrt(gfx_point_t point, int24_t outer, int24_t inner)
{
	char buf[20];
	int  strW;
	dbg_sprintf(dbgout, "[GFX] Drawing sqrt\n");
	sprintf(buf, "%d", outer);
	strW = gfx_GetStringWidth(buf);

	//190, 155
	gfx_PrintStringXY(buf, point.x, point.y);

	//198, 158 -> 200, 160
	gfx_Line(point.x + strW, point.y + 3, point.x + strW + 2, point.y + 7);

	//205, 155
	gfx_VertLine(point.x + strW + 3, point.y - 3, 11);

	sprintf(buf, "%d", inner);
	gfx_PrintStringXY(buf, point.x + strW + 6, point.y);

	gfx_HorizLine(point.x + strW + 3, point.y - 3, gfx_GetStringWidth(buf) + 3);
}


