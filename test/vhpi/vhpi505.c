#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vhpiHandleT created;

static void check_after(const vhpiCbDataT *cb_data)
{
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(created, &value);
   check_error();

   vhpi_printf("created signal value is now %d", value.value.enumv);
   fail_unless(value.value.enumv == vhpi1);

   vhpi_release_handle(created);
   vhpi_control(vhpiFinish);
   check_error();
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   // Look up the std_logic type by name without needing an existing
   // signal of that type in the design
   vhpiHandleT type = nvc_vhpi_handle_by_type_name("std_logic");
   check_handle(type);

   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   created = nvc_vhpi_create(vhpiSigDeclK, root, type, "created_sig");
   check_handle(created);

   // The initial value should be 'U'
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(created, &value);
   check_error();
   fail_unless(value.value.enumv == vhpiU);

   // Drive a new value which takes effect after a delta cycle
   value.value.enumv = vhpi1;
   vhpi_put_value(created, &value, vhpiDepositPropagate);
   check_error();

   vhpi_release_handle(type);
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

void vhpi505_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
