#include <stdio.h>
#include <stdlib.h>

#include "uart.h"
#include "test_funcs.h"


/* Example test command
 * /dev/ttyUSB0 ver_info_cmd
 * /dev/ttyUSB0 get_volume_cmd
 * /dev/ttyUSB0 set_volume_nofade_cmd 20
 *
 */
int main(int argc, char **argv)
{
    printf("BDF BFsrv test ver: 0.0.6\n");
    if ((strcmp(argv[2], "ver_info_cmd") == 0))
    {
        bf_ver_info_cmd_test(argv[1]);
        return 0;
    }

  /*
    if (argc < 2)
    {
        printf("Usage: bdf_bfsrv_test <message> [parameters]\n");
        return 0;
    }

    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_BOOTED") == 0))
    {
        aseFep_Event_Booted();
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_SYSTEM_STATUS_STANDBY") == 0))
    {
        aseFep_Event_StatusStandby();
        return 0;
    }
  */  
  return 0;
}