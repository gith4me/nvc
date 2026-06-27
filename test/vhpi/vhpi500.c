#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vhpiHandleT new_sig;

static void check_after(const vhpiCbDataT *cb_data)
{
   // The deposit scheduled in start_of_sim should have taken effect by
   // now so reading the created signal returns the new value
   vhpiValueT value = {
      .format = vhpiIntVal
   };
   vhpi_get_value(new_sig, &value);
   check_error();

   vhpi_printf("created signal value is now %d", value.value.intg);
   fail_unless(value.value.intg == 42);

   vhpi_release_handle(new_sig);

   vhpi_control(vhpiFinish);
   check_error();
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   // Obtain a type handle from an existing signal in the design
   vhpiHandleT x = vhpi_handle_by_name("x", root);
   check_handle(x);

   vhpiHandleT type = vhpi_handle(vhpiType, x);
   check_handle(type);

   // Create a brand new signal with the same type
   new_sig = vhpi_create(vhpiSigDeclK, root, type);
   check_handle(new_sig);

   fail_unless(vhpi_get(vhpiKindP, new_sig) == vhpiSigDeclK);

   const vhpiCharT *name = vhpi_get_str(vhpiNameP, new_sig);
   check_error();
   fail_if(name == NULL);
   vhpi_printf("created signal name is %s", name);

   // The new signal should be discoverable by name from the region it
   // was created in
   vhpiHandleT found = vhpi_handle_by_name((char *)name, root);
   check_handle(found);

   // The initial value should be zero
   vhpiValueT value = {
      .format = vhpiIntVal
   };
   vhpi_get_value(new_sig, &value);
   check_error();
   fail_unless(value.value.intg == 0);

   // Deposit a new value which will take effect after a delta cycle
   value.value.intg = 42;
   vhpi_put_value(new_sig, &value, vhpiDepositPropagate);
   check_error();

   vhpi_release_handle(found);
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

void vhpi500_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
