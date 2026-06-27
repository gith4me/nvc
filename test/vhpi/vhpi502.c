#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vhpiHandleT created;
static vhpiHandleT field_a;

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
   vhpi_release_handle(field_a);
   vhpi_release_handle(created);
   vhpi_control(vhpiFinish);
   check_error();
}

static void at_2ns(const vhpiCbDataT *cb_data)
{
   put_logic(field_a, vhpi0);
}

static void at_1ns(const vhpiCbDataT *cb_data)
{
   put_logic(field_a, vhpi1);
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   vhpiHandleT r = vhpi_handle_by_name("r", root);
   check_handle(r);

   vhpiHandleT type = vhpi_handle(vhpiType, r);
   check_handle(type);

   // Create a new record signal that will appear in the waveform
   created = nvc_vhpi_create(vhpiSigDeclK, root, type, "created_rec");
   check_handle(created);

   fail_unless(vhpi_get(vhpiKindP, created) == vhpiSigDeclK);

   // The fields of the new signal should be accessible by name
   field_a = vhpi_handle_by_name("a", created);
   check_handle(field_a);

   vhpiHandleT field_b = vhpi_handle_by_name("b", created);
   check_handle(field_b);
   fail_unless(vhpi_get(vhpiSizeP, field_b) == 2);

   // The initial value of the scalar field is 'U'
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(field_a, &value);
   check_error();
   fail_unless(value.value.enumv == vhpiU);

   vhpi_release_handle(field_b);
   vhpi_release_handle(type);
   vhpi_release_handle(r);
   vhpi_release_handle(root);

   register_after(1, at_1ns);
   register_after(2, at_2ns);
   register_after(3, at_3ns);
}

void vhpi502_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
