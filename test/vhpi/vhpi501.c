#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vhpiHandleT created;

static void put_logic(vhpiHandleT h, vhpiEnumT v)
{
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   value.value.enumv = v;
   vhpi_put_value(h, &value, vhpiDepositPropagate);
   check_error();
}

static void register_after(uint64_t ns, void (*fn)(const vhpiCbDataT *))
{
   vhpiTimeT time = {
      .low = ns * 1000000   // Nanoseconds to femtoseconds
   };

   vhpiCbDataT cb_data = {
      .reason = vhpiCbAfterDelay,
      .cb_rtn = fn,
      .time   = &time
   };
   vhpi_register_cb(&cb_data, 0);
   check_error();
}

static void at_3ns(const vhpiCbDataT *cb_data)
{
   vhpi_release_handle(created);
   vhpi_control(vhpiFinish);
   check_error();
}

static void at_2ns(const vhpiCbDataT *cb_data)
{
   put_logic(created, vhpi0);
}

static void at_1ns(const vhpiCbDataT *cb_data)
{
   put_logic(created, vhpi1);
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   vhpiHandleT x = vhpi_handle_by_name("x", root);
   check_handle(x);

   vhpiHandleT type = vhpi_handle(vhpiType, x);
   check_handle(type);

   // Create a new std_logic signal that will appear in the waveform
   created = nvc_vhpi_create_signal(root, type, "created_sig");
   check_handle(created);

   vhpi_release_handle(type);
   vhpi_release_handle(x);
   vhpi_release_handle(root);

   register_after(1, at_1ns);
   register_after(2, at_2ns);
   register_after(3, at_3ns);
}

void vhpi501_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
