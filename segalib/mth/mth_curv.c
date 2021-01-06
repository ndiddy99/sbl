/*----------------------------------------------------------------------------
 *  mth_curv.c -- MTH ライブラリ 曲線計算 モジュール
 *  Copyright(c) 1994 SEGA
 *  Written by Y.K on 1994-04-13 Ver.0.80
 *
 *  このライブラリは曲線計算処理モジュールで、
 *  以下のルーチンを含む。
 *
 *  MTH_Curve2           -  曲線計算　２次元
 *  MTH_Curve2t          -  曲線計算　２次元　接線付き
 *  MTH_Curve3           -  曲線計算　３次元
 *  MTH_Curve3t          -  曲線計算　３次元　接線付き
 *
 *----------------------------------------------------------------------------
 */

/*
 * USER SUPPLIED INCLUDE FILES
 */
#include "sega_mth.h"
#ifdef _SH
#include <machine.h>
#endif

/*	ワークエリアのポインタ	*/
static Fixed32 *MTH_crvh;
static Fixed32 *MTH_crvd;
static Fixed32 *MTH_crvx;
static Fixed32 *MTH_crvy;
static Fixed32 *MTH_crvz;
static Fixed32 *MTH_crvp;
static Fixed32 *MTH_crva;
static Fixed32 *MTH_crvb;
static Fixed32 *MTH_crvc;

/****************************************************************************/
/*	平方根計算							    */
/****************************************************************************/
Fixed32 MTH_Sqrt(Fixed32 x)
{
	Fixed32 s, last;
	Sint32 count = 0;

	if(x > MTH_FIXED(0.0)) {
		s = (x > MTH_FIXED(1.0)) ? (x) : MTH_FIXED(1.0);
		do {
			last = s;
			s = (MTH_Div(x, s) + s) / 2;
			count++;
		} while(s < last);
		return last;
	} else if (x != MTH_FIXED(0.0)) {
		return MTH_FIXED(-1.0);
	} else {
		return MTH_FIXED(0.0);
	}
}

/****************************************************************************/
/*	斜辺の長さ計算							    */
/****************************************************************************/
Fixed32 MTH_Hypot(Fixed32 x, Fixed32 y)
{
	Sint32 bai;
	Fixed32 bun;

	x = ABS(x);
	y = ABS(y);
	if((x > MTH_FIXED(1.0)) || (y > MTH_FIXED(1.0))) {
		bai = (x > y) ? MTH_FixedToInt(x) : MTH_FixedToInt(y);
		x = x / bai;
		y = y / bai;
		return MTH_Sqrt(MTH_Mul(x, x) + MTH_Mul(y, y)) * bai;
	} else {
		bun = (x > y) ? (MTH_Div(MTH_FIXED(1.0), x)) :
                                (MTH_Div(MTH_FIXED(1.0), y));
		x = MTH_Mul(x, bun);
		y = MTH_Mul(y, bun);
		return MTH_Div(MTH_Sqrt(MTH_Mul(x, x) + MTH_Mul(y, y)), bun);
	}
}

/****************************************************************************/
/*	テーブル作成（低レベル）					    */
/****************************************************************************/
static void maketableL(Fixed32 x[], Fixed32 y[], Fixed32 z[], Uint32 np)
{
	Uint32 i;
	Fixed32 t;

	z[0] = MTH_FIXED(0.0);
	z[np - 1] = MTH_FIXED(0.0);
	for (i = 0; i < np - 1; i++) {
		MTH_crvh[i] = x[i + 1] - x[i];
		MTH_crvd[i + 1] = MTH_Div(y[i + 1] - y[i], MTH_crvh[i]);
	}
	z[1] = MTH_crvd[2] - MTH_crvd[1] - MTH_Mul(MTH_crvh[0], z[0]);
	MTH_crvd[1] = 2 * (x[2] - x[0]);
	for (i = 1; i < np - 2; i++) {
		t = MTH_Div(MTH_crvh[i], MTH_crvd [i]);
		z[i + 1] = MTH_crvd[i + 2] - MTH_crvd[i + 1] - MTH_Mul(z[i], t);
		MTH_crvd[i + 1] = 2 * (x[i + 2] - x[i]) - MTH_Mul(MTH_crvh[i], t);
	}
	z[np - 2] -= MTH_Mul(MTH_crvh[np - 2], z[np - 1]);
	for (i = np - 2; i > 0; i--) {
		z[i] = MTH_Div(z[i] - MTH_Mul(MTH_crvh[i], z[i + 1]), MTH_crvd[i]);
	}
}

/****************************************************************************/
/*	座標取得（低レベル）						    */
/****************************************************************************/
static Fixed32 splineL(Fixed32 t, Fixed32 x[], Fixed32 y[], Fixed32 z[],
 Uint32 np)
{
	Uint32 i, j, k;
	Fixed32 d, h;

	i = 0;
	j = np - 1;
	while (i < j) {
		k = (i + j) / 2;
		if (x[k] < t) {
			i = k + 1;
		} else {
			j = k;
		}
	}
	if ( i > 0) {
		i--;
	}
	h = x[i + 1] - x[i];
	d = t - x[i];
	return MTH_Mul(MTH_Mul(MTH_Div(MTH_Mul(z[i + 1] - z[i], d),
	 h) + z[i] * 3, d)
	 + (MTH_Div(y[i + 1] - y[i], h)
	 - MTH_Mul(z[i] * 2 + z[i + 1], h)), d) + y[i];
}

/****************************************************************************/
/*	座標取得（低レベル） 接線付き					    */
/****************************************************************************/
static Fixed32 splineLt(Fixed32 t, Fixed32 x[], Fixed32 y[], Fixed32 z[],
 Uint32 np, Fixed32 *out_tan)
{
	Uint32 i, j, k;
	Fixed32 d, h;

	i = 0;
	j = np - 1;
	while (i < j) {
		k = (i + j) / 2;
		if (x[k] < t) {
			i = k + 1;
		} else {
			j = k;
		}
	}
	if ( i > 0) {
		i--;
	}
	h = x[i + 1] - x[i];
	d = t - x[i];
	*out_tan = MTH_Mul(MTH_Div(MTH_Mul(z[i + 1] - z[i], d),
	 h) + z[i] * 2, d) * 3
	 + MTH_Div(y[i + 1] - y[i], h) - MTH_Mul(z[i] * 2 + z[i + 1], h);
	return MTH_Mul(MTH_Mul(MTH_Div(MTH_Mul(z[i + 1] - z[i], d),
	 h) + z[i] * 3, d)
	 + (MTH_Div(y[i + 1] - y[i], h)
	 - MTH_Mul(z[i] * 2 + z[i + 1], h)), d) + y[i];
}

/****************************************************************************/
/*	テーブル作成（高レベル） ２次元					    */
/****************************************************************************/
static void maketableH2(Fixed32 p[], Fixed32 x[], Fixed32 y[],
 Fixed32 a[], Fixed32 b[], Uint32 np)
{
	Uint32 i;
	Fixed32 t1, t2;

	p[0] = MTH_FIXED(0.0);
	for(i = 1; i < np; i++) {
		t1 = x[i] - x[i - 1];
		t2 = y[i] - y[i - 1];
		p[i] = p[i - 1] + MTH_Hypot(t1, t2);
	}
	for (i = 1; i < np; i++) {
		p[i] = MTH_Div(p[i], p[np - 1]);
	}
	maketableL(p, x, a, np);
	maketableL(p, y, b, np);
}

/****************************************************************************/
/*	テーブル作成（高レベル） ３次元					    */
/****************************************************************************/
static void maketableH3(Fixed32 p[], Fixed32 x[], Fixed32 y[], Fixed32 z[],
 Fixed32 a[], Fixed32 b[], Fixed32 c[], Uint32 np)
{
	Uint32 i;
	Fixed32 t1, t2, t3;

	p[0] = MTH_FIXED(0.0);
	for(i = 1; i < np; i++) {
		t1 = x[i] - x[i - 1];
		t2 = y[i] - y[i - 1];
		t3 = z[i] - z[i - 1];
		p[i] = p[i - 1] + MTH_Hypot(MTH_Hypot(t1, t2), t3);
	}
	for (i = 1; i < np; i++) {
		p[i] = MTH_Div(p[i], p[np - 1]);
	}
	maketableL(p, x, a, np);
	maketableL(p, y, b, np);
	maketableL(p, z, c, np);
}

/****************************************************************************/
/*	座標取得（高レベル）　２次元					    */
/****************************************************************************/
static void splineH2(Fixed32 t, Fixed32 *px, Fixed32 *py,
 Fixed32 p[], Fixed32 x[], Fixed32 y[],
 Fixed32 a[], Fixed32 b[], Uint32 np)
{
	*px = splineL(t, p, x, a, np);
	*py = splineL(t, p, y, b, np);
}

/****************************************************************************/
/*	座標取得（高レベル）　２次元　接線付き				    */
/****************************************************************************/
static void splineH2t(Fixed32 t, Fixed32 *px, Fixed32 *py,
 Fixed32 p[], Fixed32 x[], Fixed32 y[],
 Fixed32 a[], Fixed32 b[], Uint32 np,
 Fixed32 *tx, Fixed32 *ty)
{
	Fixed32 ll;

	*px = splineLt(t, p, x, a, np, tx);
	*py = splineLt(t, p, y, b, np, ty);
	ll = MTH_Hypot(*tx, *ty);
	*tx = MTH_Div(*tx, ll);
	*ty = MTH_Div(*ty, ll);
}

/****************************************************************************/
/*	座標取得（高レベル）　３次元					    */
/****************************************************************************/
static void splineH3(Fixed32 t, Fixed32 *px, Fixed32 *py, Fixed32 *pz,
 Fixed32 p[], Fixed32 x[], Fixed32 y[], Fixed32 z[],
 Fixed32 a[], Fixed32 b[], Fixed32 c[], Uint32 np)
{
	*px = splineL(t, p, x, a, np);
	*py = splineL(t, p, y, b, np);
	*pz = splineL(t, p, z, c, np);
}

/****************************************************************************/
/*	座標取得（高レベル）　３次元　接線付き				    */
/****************************************************************************/
static void splineH3t(Fixed32 t, Fixed32 *px, Fixed32 *py, Fixed32 *pz,
 Fixed32 p[], Fixed32 x[], Fixed32 y[], Fixed32 z[],
 Fixed32 a[], Fixed32 b[], Fixed32 c[], Uint32 np,
 Fixed32 *tx, Fixed32 *ty, Fixed32 *tz)
{
	Fixed32 ll;

	*px = splineLt(t, p, x, a, np, tx);
	*py = splineLt(t, p, y, b, np, ty);
	*pz = splineLt(t, p, z, c, np, tz);
	ll = MTH_Hypot(MTH_Hypot(*tx, *ty), *tz);
	*tx = MTH_Div(*tx, ll);
	*ty = MTH_Div(*ty, ll);
	*tz = MTH_Div(*tz, ll);
}

/****************************************************************************/
/*	パラメータチェック						    */
/****************************************************************************/
static Uint32 paraChk(Uint32 in_n, Uint32 out_n)
{
	if(in_n < 3 || out_n < 2) {
		return 0;
	} else {
		return out_n;
	}
}

/****************************************************************************/
/*	ワークエリアセット			   			    */
/****************************************************************************/
static Uint32 setWork(MthCurveWork *work, Uint32 in_n)
{
	if(work->num < in_n) {
		return 0;
	}
	MTH_crvh = work->MTH_crvh;
	MTH_crvd = work->MTH_crvd;
	MTH_crvx = work->MTH_crvx;
	MTH_crvy = work->MTH_crvy;
	MTH_crvz = work->MTH_crvz;
	MTH_crvp = work->MTH_crvp;
	MTH_crva = work->MTH_crva;
	MTH_crvb = work->MTH_crvb;
	MTH_crvc = work->MTH_crvc;
	return 1;
}

/****************************************************************************/
/*	曲線計算　２次元					            */
/****************************************************************************/
Uint32 MTH_Curve2(MthCurveWork *work, MthXy *in_aray, Uint32 in_n,
 Uint32 out_n, MthXy *out_aray)
{
	Fixed32 t;
	Uint32 i;

	if (paraChk(in_n, out_n) == 0) {
		return 0;
	}
	if (setWork(work,in_n) == 0) {
		return 0;
	}
	for(i = 0; i < in_n; i++) {
		MTH_crvx[i] = in_aray[i].x;
		MTH_crvy[i] = in_aray[i].y;
	}
	maketableH2(MTH_crvp, MTH_crvx, MTH_crvy, MTH_crva, MTH_crvb, in_n);
	for(i = 0; i < out_n; i++) {
		t = MTH_Div(MTH_IntToFixed(i), MTH_IntToFixed(out_n - 1));
		splineH2(t, &out_aray[i].x, &out_aray[i].y, MTH_crvp,
		 MTH_crvx, MTH_crvy, MTH_crva, MTH_crvb, in_n);
	}
	return out_n;
}

/****************************************************************************/
/*	曲線計算　２次元　接線付き			        	    */
/****************************************************************************/
Uint32 MTH_Curve2t(MthCurveWork *work, MthXy *in_aray, Uint32 in_n,
 Uint32 out_n, MthXy *out_aray, MthXy *tan_aray)
{
	Fixed32 t;
	Uint32 i;

	if (paraChk(in_n, out_n) == 0) {
		return 0;
	}
	if (setWork(work,in_n) == 0) {
		return 0;
	}
	for(i = 0; i < in_n; i++) {
		MTH_crvx[i] = in_aray[i].x;
		MTH_crvy[i] = in_aray[i].y;
	}
	maketableH2(MTH_crvp, MTH_crvx, MTH_crvy, MTH_crva, MTH_crvb, in_n);
	for(i = 0; i < out_n; i++) {
		t = MTH_Div(MTH_IntToFixed(i), MTH_IntToFixed(out_n - 1));
		splineH2t(t, &out_aray[i].x, &out_aray[i].y, MTH_crvp,
		 MTH_crvx, MTH_crvy, MTH_crva, MTH_crvb, in_n,
		 &tan_aray[i].x, &tan_aray[i].y);
	}
	return out_n;
}

/****************************************************************************/
/*	曲線計算　3次元							    */
/****************************************************************************/
Uint32 MTH_Curve3(MthCurveWork *work, MthXyz *in_aray, Uint32 in_n,
 Uint32 out_n, MthXyz *out_aray)
{
	Fixed32 t;
	Uint32 i;

	if (paraChk(in_n, out_n) == 0) {
		return 0;
	}
	if (setWork(work,in_n) == 0) {
		return 0;
	}
	for(i = 0; i < in_n; i++) {
		MTH_crvx[i] = in_aray[i].x;
		MTH_crvy[i] = in_aray[i].y;
		MTH_crvz[i] = in_aray[i].z;
	}
	maketableH3(MTH_crvp, MTH_crvx, MTH_crvy, MTH_crvz,
	 MTH_crva, MTH_crvb, MTH_crvc, in_n);
	for(i = 0; i < out_n; i++) {
		t = MTH_Div(MTH_IntToFixed(i), MTH_IntToFixed(out_n - 1));
		splineH3(t, &out_aray[i].x, &out_aray[i].y, &out_aray[i].z, MTH_crvp,
		 MTH_crvx, MTH_crvy, MTH_crvz, MTH_crva, MTH_crvb, MTH_crvc, in_n);
	}
	return out_n;
}

/****************************************************************************/
/*	曲線計算　3次元　接線付き					    */
/****************************************************************************/
Uint32 MTH_Curve3t(MthCurveWork *work, MthXyz *in_aray, Uint32 in_n,
 Uint32 out_n, MthXyz *out_aray, MthXyz *tan_aray)
{
	Fixed32 t;
	Uint32 i;

	if (paraChk(in_n, out_n) == 0) {
		return 0;
	}
	if (setWork(work,in_n) == 0) {
		return 0;
	}
	for(i = 0; i < in_n; i++) {
		MTH_crvx[i] = in_aray[i].x;
		MTH_crvy[i] = in_aray[i].y;
		MTH_crvz[i] = in_aray[i].z;
	}
	maketableH3(MTH_crvp, MTH_crvx, MTH_crvy, MTH_crvz,
	 MTH_crva, MTH_crvb, MTH_crvc, in_n);
	for(i = 0; i < out_n; i++) {
		t = MTH_Div(MTH_IntToFixed(i), MTH_IntToFixed(out_n - 1));
		splineH3t(t, &out_aray[i].x, &out_aray[i].y, &out_aray[i].z, MTH_crvp,
		 MTH_crvx, MTH_crvy, MTH_crvz, MTH_crva, MTH_crvb, MTH_crvc, in_n,
		 &tan_aray[i].x, &tan_aray[i].y, &tan_aray[i].z);
	}
	return out_n;
}
