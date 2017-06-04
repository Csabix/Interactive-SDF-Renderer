subroutine(Raytracer)
vec2 raytracer_22(const in Ray r, const in float stepsize, const in int stepcount)
{	//relaxed sphere trace with faster raymarch	
	float t = 0, ft, pft=0, msp = stepsize;
	const float or = TRACE_OR;
	const float orm1 = TRACE_OR-1;
	for(int i = 0; i < stepcount; ++i)
	{
		ft = sdf(RAY(r,t));
		if(ft < pft*orm1)
		{
			t -= orm1*pft;
			ft = sdf(RAY(r,t));
		}
		msp += TRACE_STEPINCR;
		if (ft <= 0) return vec2(t-msp, t);
		t += or*ft + msp;
		pft = ft; //próbáltam optimalizálni a ciklusmag 
	}			  //megkétszerezésével, de lassabb lett
	return vec2(t-msp, t);
}