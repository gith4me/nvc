#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   // ufixed(3 downto -4): four integer and four fractional bits
   vhpiHandleT uf = nvc_vhpi_create_ufixed(root, "uf", 3, -4);
   check_handle(uf);
   fail_unless(vhpi_get(vhpiSizeP, uf) == 8);

   // sfixed(7 downto -8): sixteen bits
   vhpiHandleT sf = nvc_vhpi_create_sfixed(root, "sf", 7, -8);
   check_handle(sf);
   fail_unless(vhpi_get(vhpiSizeP, sf) == 16);

   // string(1 to 5)
   vhpiHandleT str = nvc_vhpi_create_string(root, "str", 1, 5);
   check_handle(str);
   fail_unless(vhpi_get(vhpiSizeP, str) == 5);

   // bit_vector(7 downto 0)
   vhpiHandleT bv = nvc_vhpi_create_bit_vector(root, "bv", 7, 0);
   check_handle(bv);
   fail_unless(vhpi_get(vhpiSizeP, bv) == 8);

   vhpi_printf("created ufixed/sfixed/string/bit_vector signals OK");

   vhpi_release_handle(uf);
   vhpi_release_handle(sf);
   vhpi_release_handle(str);
   vhpi_release_handle(bv);
   vhpi_release_handle(root);

   vhpi_control(vhpiFinish);
   check_error();
}

void vhpi509_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
