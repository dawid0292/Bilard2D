#include <stdio.h>
#include "wave.h"

main(int argc,char **argv)
{
 int i;
 wfObject *obj;
 wfVertex min,max,center;
 float radius;
 for (i=1; i<argc; i++)
	if (obj = wfReadObject(argv[i]))
		{
		wfGetBoundingBox(obj,min,max);
		printf("%s:\n\tX: %f - %f\n\tY: %f - %f\n\tZ: %f - %f\n",
			argv[i],min[0],max[0],min[1],max[1],min[2],max[2]);
		wfGetBoundingSphere(obj,center,&radius);
		printf("\tCenter: (%f,%f,%f)  Radius: %f\n",center[0],
			center[1],center[2],radius);
		}
}

