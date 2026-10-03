#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static vhpiHandleT orig_b;
static vhpiHandleT field_a;
static vhpiHandleT field_b;
static vhpiHandleT field_c;

static const vhpiEnumT b_bits[8] = {
   vhpi1, vhpi0, vhpi1, vhpi0, vhpi0, vhpi1, vhpi0, vhpi1
};

static const vhpiEnumT c_bits[3] = { 1, 0, 1 };

static void check_bounds(vhpiHandleT field, int left, int right, int is_up)
{
   vhpiHandleT type = VHPI_CHECK(vhpi_handle(vhpiType, field));
   vhpiHandleT it = VHPI_CHECK(vhpi_iterator(vhpiConstraints, type));
   vhpiHandleT range = VHPI_CHECK(vhpi_scan(it));

   fail_unless(vhpi_get(vhpiLeftBoundP, range) == left);
   fail_unless(vhpi_get(vhpiRightBoundP, range) == right);
   fail_unless(vhpi_get(vhpiIsUpP, range) == is_up);

   vhpi_release_handle(range);
   vhpi_release_handle(it);
   vhpi_release_handle(type);
}

static void get_vec(vhpiHandleT h, vhpiEnumT *buf, int count)
{
   vhpiValueT value = {
      .format       = vhpiLogicVecVal,
      .bufSize      = count * sizeof(vhpiEnumT),
      .value.enumvs = buf,
   };
   vhpi_get_value(h, &value);
   check_error();
   fail_unless(value.numElems == count);
}

static void put_vec(vhpiHandleT h, const vhpiEnumT *bits, int count)
{
   vhpiValueT value = {
      .format       = vhpiLogicVecVal,
      .bufSize      = count * sizeof(vhpiEnumT),
      .numElems     = count,
      .value.enumvs = (vhpiEnumT *)bits,
   };
   vhpi_put_value(h, &value, vhpiDepositPropagate);
   check_error();
}

static void check_after(const vhpiCbDataT *cb_data)
{
   // The deposits scheduled in start_of_sim should have taken effect
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(field_a, &value);
   check_error();
   fail_unless(value.value.enumv == vhpi1);

   vhpiEnumT b[8];
   get_vec(field_b, b, 8);
   fail_unless(memcmp(b, b_bits, sizeof(b)) == 0);

   vhpiEnumT c[3];
   get_vec(field_c, c, 3);
   fail_unless(memcmp(c, c_bits, sizeof(c)) == 0);

   // The signal in the design with the same type must not be affected
   get_vec(orig_b, b, 8);
   for (int i = 0; i < 8; i++)
      fail_unless(b[i] == vhpiU);

   vhpi_printf("created record subtype signal OK");

   vhpi_release_handle(orig_b);
   vhpi_release_handle(field_a);
   vhpi_release_handle(field_b);
   vhpi_release_handle(field_c);

   vhpi_control(vhpiFinish);
   check_error();
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   vhpiHandleT r = vhpi_handle_by_name("r", root);
   check_handle(r);

   orig_b = vhpi_handle_by_name("b", r);
   check_handle(orig_b);

   // The type of R is a record subtype which constrains the array
   // fields that are unconstrained in the record type
   vhpiHandleT type = vhpi_handle(vhpiType, r);
   check_handle(type);

   vhpiHandleT created = nvc_vhpi_create(vhpiSigDeclK, root, type,
                                         "created_rec");
   check_handle(created);

   fail_unless(vhpi_get(vhpiKindP, created) == vhpiSigDeclK);

   field_a = vhpi_handle_by_name("a", created);
   check_handle(field_a);
   fail_unless(vhpi_get(vhpiSizeP, field_a) == 1);

   field_b = vhpi_handle_by_name("b", created);
   check_handle(field_b);
   fail_unless(vhpi_get(vhpiSizeP, field_b) == 8);
   check_bounds(field_b, 7, 0, 0);

   field_c = vhpi_handle_by_name("c", created);
   check_handle(field_c);
   fail_unless(vhpi_get(vhpiSizeP, field_c) == 3);
   check_bounds(field_c, 1, 3, 1);

   // Every field starts with the leftmost value of its element type
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(field_a, &value);
   check_error();
   fail_unless(value.value.enumv == vhpiU);

   vhpiEnumT b[8];
   get_vec(field_b, b, 8);
   for (int i = 0; i < 8; i++)
      fail_unless(b[i] == vhpiU);

   vhpiEnumT c[3];
   get_vec(field_c, c, 3);
   for (int i = 0; i < 3; i++)
      fail_unless(c[i] == 0);

   // Deposit new values which take effect after a delta cycle
   value.value.enumv = vhpi1;
   vhpi_put_value(field_a, &value, vhpiDepositPropagate);
   check_error();

   put_vec(field_b, b_bits, 8);
   put_vec(field_c, c_bits, 3);

   vhpi_release_handle(created);
   vhpi_release_handle(type);
   vhpi_release_handle(r);
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

void vhpi510_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
