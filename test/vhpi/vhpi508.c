#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vhpiHandleT sl;

static void check_after(const vhpiCbDataT *cb_data)
{
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(sl, &value);
   check_error();
   fail_unless(value.value.enumv == vhpi1);

   vhpi_release_handle(sl);
   vhpi_control(vhpiFinish);
   check_error();
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   // One-call creation of a std_logic signal
   sl = nvc_vhpi_create_std_logic(root, "sl");
   check_handle(sl);
   fail_unless(vhpi_get(vhpiSizeP, sl) == 1);

   // One-call creation of a std_logic_vector(3 downto 0) signal
   vhpiHandleT slv = nvc_vhpi_create_std_logic_vector(root, "slv", 3, 0);
   check_handle(slv);
   fail_unless(vhpi_get(vhpiSizeP, slv) == 4);

   // The other scalar convenience wrappers
   vhpiHandleT b = nvc_vhpi_create_boolean(root, "b");
   check_handle(b);
   vhpiHandleT iv = nvc_vhpi_create_integer(root, "iv");
   check_handle(iv);
   vhpiHandleT rv = nvc_vhpi_create_real(root, "rv");
   check_handle(rv);
   vhpiHandleT tv = nvc_vhpi_create_time(root, "tv");
   check_handle(tv);

   vhpi_release_handle(b);
   vhpi_release_handle(iv);
   vhpi_release_handle(rv);
   vhpi_release_handle(tv);

   // The scalar starts at 'U'
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(sl, &value);
   check_error();
   fail_unless(value.value.enumv == vhpiU);

   // Drive it and read back after a delta
   value.value.enumv = vhpi1;
   vhpi_put_value(sl, &value, vhpiDepositPropagate);
   check_error();

   vhpi_printf("created std_logic and std_logic_vector signals OK");

   vhpi_release_handle(slv);
   vhpi_release_handle(root);

   vhpiTimeT time_1ns = {
      .low = 1000000
   };

   vhpiCbDataT cb_data2 = {
      .reason = vhpiCbAfterDelay,
      .cb_rtn = check_after,
      .time   = &time_1ns
   };
   vhpi_register_cb(&cb_data2, 0);
   check_error();
}

void vhpi508_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
