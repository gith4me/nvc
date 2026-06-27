//
//  Copyright (C) 2026  Nick Gasson
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef VHPI_NVC_H
#define VHPI_NVC_H

#ifndef VHPI_USER_H
#error This file must be included after vhpi_user.h
#endif

#ifdef __cplusplus
extern "C" {
#endif

// NVC-specific extensions to the standard VHPI interface

// Create a new object in the design.  This is like the standard
// vhpi_create but additionally allows a name to be specified.
//
// Currently only signals can be created: pass vhpiSigDeclK for kind,
// a region for handle1, and a constrained type for handle2.  The type
// may be a scalar, a homogeneous array, or a record whose fields are
// themselves scalars, homogeneous arrays, or records.  Returns a handle
// to the new object or NULL on error.
vhpiHandleT nvc_vhpi_create(vhpiClassKindT kind, vhpiHandleT handle1,
                            vhpiHandleT handle2, const char *name);

// Look up a type declaration anywhere in the design by name and return
// a handle to it.  This searches the top-level design unit and every
// package the design depends on, including the standard and IEEE
// packages.  The name may be simple (e.g. "std_logic") or qualified
// (e.g. "ieee.std_logic_1164.std_logic").  Returns NULL if no matching
// type is found.  Useful for obtaining a type handle to pass to
// nvc_vhpi_create without needing an existing object of that type.
vhpiHandleT nvc_vhpi_handle_by_type_name(const char *name);

// Construct a constrained array subtype from an unconstrained array type
// (for example std_logic_vector) and the given bounds.  If left >= right
// the range is descending (downto) otherwise ascending (to).  Returns a
// type handle suitable for nvc_vhpi_create.
vhpiHandleT nvc_vhpi_create_array_subtype(vhpiHandleT base_type,
                                          int left, int right);

// Construct a new record type with the given name and fields.  The
// field_names and field_types arrays must each have nfields entries; each
// field type is a type handle.  Returns a type handle suitable for
// nvc_vhpi_create.
vhpiHandleT nvc_vhpi_create_record_type(const char *name, int nfields,
                                        const char *const *field_names,
                                        const vhpiHandleT *field_types);

#ifdef __cplusplus
}
#endif

#endif  // VHPI_NVC_H
