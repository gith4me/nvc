#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vhpiHandleT created;

static void check_after(const vhpiCbDataT *cb_data)
{
   vhpiValueT value = {
      .format = vhpiRealVal
   };
   vhpi_get_value(created, &value);
   check_error();

   vhpi_printf("created real signal value is now %f", value.value.real);
   fail_unless(value.value.real == 2.5);

   vhpi_release_handle(created);
   vhpi_control(vhpiFinish);
   check_error();
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   vhpiHandleT x = vhpi_handle_by_name("x", root);
   check_handle(x);

   vhpiHandleT type = vhpi_handle(vhpiType, x);
   check_handle(type);

   // Create a new signal of a real (floating point) type
   created = nvc_vhpi_create(vhpiSigDeclK, root, type, "created_real");
   check_handle(created);

   fail_unless(vhpi_get(vhpiKindP, created) == vhpiSigDeclK);

   // The initial value should be zero
   vhpiValueT value = {
      .format = vhpiRealVal
   };
   vhpi_get_value(created, &value);
   check_error();
   fail_unless(value.value.real == 0.0);

   // Deposit a new value which takes effect after a delta cycle
   value.value.real = 2.5;
   vhpi_put_value(created, &value, vhpiDepositPropagate);
   check_error();

   vhpi_release_handle(type);
   vhpi_release_handle(x);
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

void vhpi504_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
