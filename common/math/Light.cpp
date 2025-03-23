//// Light.cpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the light utils
//
///////////////////////////////////////////////////////////////////////////////

#include "Light.hpp"

namespace nexo::math {
	std::pair<float, float> computeAttenuationFromDistance(float distance)
	{
		// Clamp distance to the min/max of the table
		if (distance <= s_attenuationTable[0].distance)
		    return { s_attenuationTable[0].linear, s_attenuationTable[0].quadratic };

		if (distance >= s_attenuationTable[s_attenuationCount - 1].distance)
		    return { s_attenuationTable[s_attenuationCount - 1].linear,
		                s_attenuationTable[s_attenuationCount - 1].quadratic };

		// Find bounding entries
		int lowerIndex = 0;
		int upperIndex = 1;
		for (int i = 0; i < s_attenuationCount - 1; ++i)
		{
		    if (distance >= s_attenuationTable[i].distance &&
		        distance <= s_attenuationTable[i + 1].distance)
		    {
		        lowerIndex = i;
		        upperIndex = i + 1;
		        break;
		    }
		}

		// Compute interpolation factor t in [0, 1]
		float distLower = s_attenuationTable[lowerIndex].distance;
		float distUpper = s_attenuationTable[upperIndex].distance;
		float t = (distance - distLower) / (distUpper - distLower);

		float linear = s_attenuationTable[lowerIndex].linear +
		                t * (s_attenuationTable[upperIndex].linear - s_attenuationTable[lowerIndex].linear);

		float quadratic = s_attenuationTable[lowerIndex].quadratic +
		                    t * (s_attenuationTable[upperIndex].quadratic - s_attenuationTable[lowerIndex].quadratic);

		return { linear, quadratic };
	}
}
