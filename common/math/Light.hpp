//// Light.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Header file for the light utils
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utility>

namespace nexo::math {
	// Example data structure for your attenuation table
	struct AttenuationData {
	    float distance;
	    float constant;
	    float linear;
	    float quadratic;
	};

	// Static table of recommended attenuation values
	static const AttenuationData s_attenuationTable[] = {
	    // distance, constant, linear, quadratic
	    { 7.0f,    1.0f, 0.70f,   1.8f   },
	    { 13.0f,   1.0f, 0.35f,   0.44f  },
	    { 20.0f,   1.0f, 0.22f,   0.20f  },
	    { 32.0f,   1.0f, 0.14f,   0.07f  },
	    { 50.0f,   1.0f, 0.09f,   0.032f },
	    { 65.0f,   1.0f, 0.07f,   0.017f },
	    { 100.0f,  1.0f, 0.045f,  0.0075f},
	    { 160.0f,  1.0f, 0.027f,  0.0028f},
	    { 200.0f,  1.0f, 0.022f,  0.0019f},
	    { 325.0f,  1.0f, 0.014f,  0.0007f},
	    { 600.0f,  1.0f, 0.007f,  0.0002f},
	    { 3250.0f, 1.0f, 0.0014f, 0.000007f}
	};

	inline constexpr int s_attenuationCount = sizeof(s_attenuationTable) / sizeof(s_attenuationTable[0]);

	std::pair<float, float> computeAttenuationFromDistance(float distance);
}
