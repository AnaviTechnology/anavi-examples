#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "BMP180.h"

int main(int argc, char **argv)
{
        if ( 0 > calibration())
	{
		fprintf(stderr, "ERROR: Unable to access RabbitMax temperature sensor\n");
		exit(-1);
	}

        printf("Temperature\t%0.1f C\n", getTemperature());
        printf("Pressure\t%0.2f hPa\n", (double)getPressure()/100);
        printf("Altitude\t%0.2f m\n", getAltitude());

        return 0;
}
