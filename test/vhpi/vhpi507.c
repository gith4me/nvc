#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   // Construct std_logic_vector(7 downto 0) and create a signal of it
   vhpiHandleT slv = nvc_vhpi_handle_by_type_name("std_logic_vector");
   check_handle(slv);

   vhpiHandleT slv8 = nvc_vhpi_create_array_subtype(slv, 7, 0);
   check_handle(slv8);

   vhpiHandleT vec = nvc_vhpi_create(vhpiSigDeclK, root, slv8, "vec");
   check_handle(vec);
   fail_unless(vhpi_get(vhpiSizeP, vec) == 8);

   // Construct a record type with a std_logic field and a vector field
   vhpiHandleT sl = nvc_vhpi_handle_by_type_name("std_logic");
   check_handle(sl);

   const char *fnames[] = { "a", "b" };
   vhpiHandleT ftypes[] = { sl, slv8 };
   vhpiHandleT rectype = nvc_vhpi_create_record_type("myrec", 2,
                                                     fnames, ftypes);
   check_handle(rectype);

   vhpiHandleT rec = nvc_vhpi_create(vhpiSigDeclK, root, rectype, "rec");
   check_handle(rec);

   // Both fields should be reachable and have the expected sizes
   vhpiHandleT fa = vhpi_handle_by_name("a", rec);
   check_handle(fa);
   fail_unless(vhpi_get(vhpiSizeP, fa) == 1);

   vhpiHandleT fb = vhpi_handle_by_name("b", rec);
   check_handle(fb);
   fail_unless(vhpi_get(vhpiSizeP, fb) == 8);

   // The scalar field starts at 'U'
   vhpiValueT value = {
      .format = vhpiLogicVal
   };
   vhpi_get_value(fa, &value);
   check_error();
   fail_unless(value.value.enumv == vhpiU);

   // The one-call record wrapper builds the type and signal together
   vhpiHandleT rec2 = nvc_vhpi_create_record(root, "rec2", 2, fnames, ftypes);
   check_handle(rec2);

   vhpiHandleT fb2 = vhpi_handle_by_name("b", rec2);
   check_handle(fb2);
   fail_unless(vhpi_get(vhpiSizeP, fb2) == 8);
   vhpi_release_handle(fb2);
   vhpi_release_handle(rec2);

   vhpi_printf("constructed std_logic_vector and record signals OK");

   vhpi_release_handle(fa);
   vhpi_release_handle(fb);
   vhpi_release_handle(rec);
   vhpi_release_handle(rectype);
   vhpi_release_handle(sl);
   vhpi_release_handle(vec);
   vhpi_release_handle(slv8);
   vhpi_release_handle(slv);
   vhpi_release_handle(root);

   vhpi_control(vhpiFinish);
   check_error();
}

void vhpi507_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
