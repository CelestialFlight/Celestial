#include "complementaryFilter.h"
#include "common/error.h"

int ComplementaryFilterInit(
	struct ComplementaryFilter* cf, double alpha)
{
    // Defensive check.
    if (error(cf != 0)) return -1;

	cf->angle = 0;
	cf->alpha = alpha;

	return 0;
}

int ComplementaryFilterUpdate(
    struct ComplementaryFilter* cf,
    double newAngle,
    double rate,
    double dT)
{
    // Defensive check.
    if (error(cf != 0)) return -1;

	cf->angle += rate * dT;
	cf->angle = cf->angle * cf->alpha + newAngle * (1 - cf->alpha);

	return 0;
}
