#include "vhpi_test.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static int64_t phys_to_i64(vhpiPhysT phys)
{
   return ((int64_t)phys.high << 32) | phys.low;
}

static void start_of_sim(const vhpiCbDataT *cb_data)
{
   // The predefined TIME type is a physical type and is always available
   vhpiHandleT type = nvc_vhpi_handle_by_type_name("time");
   check_handle(type);

   vhpiHandleT root = vhpi_handle(vhpiRootInst, NULL);
   check_handle(root);

   vhpiHandleT created = nvc_vhpi_create(vhpiSigDeclK, root, type,
                                         "created_time");
   check_handle(created);

   fail_unless(vhpi_get(vhpiKindP, created) == vhpiSigDeclK);

   vhpiValueT value = {
      .format = vhpiObjTypeVal
   };
   vhpi_get_value(created, &value);
   check_error();

   // The predefined TIME type reports its value as vhpiTimeVal
   fail_unless(value.format == vhpiTimeVal);
   vhpi_printf("created time signal initial value = %lld",
               (long long)phys_to_i64(value.value.phys));
   fail_unless(phys_to_i64(value.value.phys) == 0);

   vhpi_release_handle(created);
   vhpi_release_handle(type);
   vhpi_release_handle(root);

   vhpi_control(vhpiFinish);
   check_error();
}

void vhpi506_startup(void)
{
   vhpiCbDataT cb_data1 = {
      .reason = vhpiCbStartOfSimulation,
      .cb_rtn = start_of_sim,
   };
   vhpi_register_cb(&cb_data1, 0);
   check_error();
}
