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

// Convenience wrappers for creating signals of the most common types.
// These look up the type and create the signal in one call and are
// equivalent to combining nvc_vhpi_handle_by_type_name,
// nvc_vhpi_create_array_subtype, and nvc_vhpi_create.
vhpiHandleT nvc_vhpi_create_std_logic(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_boolean(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_bit(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_character(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_integer(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_natural(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_positive(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_real(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_time(vhpiHandleT region, const char *name);
vhpiHandleT nvc_vhpi_create_std_logic_vector(vhpiHandleT region,
                                             const char *name,
                                             int left, int right);
vhpiHandleT nvc_vhpi_create_bit_vector(vhpiHandleT region, const char *name,
                                       int left, int right);
vhpiHandleT nvc_vhpi_create_string(vhpiHandleT region, const char *name,
                                   int left, int right);
// Fixed point types from ieee.fixed_pkg.  The bounds may be negative,
// for example nvc_vhpi_create_ufixed(region, "x", 3, -4).
vhpiHandleT nvc_vhpi_create_ufixed(vhpiHandleT region, const char *name,
                                   int left, int right);
vhpiHandleT nvc_vhpi_create_sfixed(vhpiHandleT region, const char *name,
                                   int left, int right);

// Create a record signal in one call, building the record type from the
// given fields.  Equivalent to nvc_vhpi_create_record_type followed by
// nvc_vhpi_create.
vhpiHandleT nvc_vhpi_create_record(vhpiHandleT region, const char *name,
                                   int nfields, const char *const *field_names,
                                   const vhpiHandleT *field_types);

// Create a new sub-region under the given parent region.  Equivalent to
// nvc_vhpi_create(vhpiBlockStmtK, parent, NULL, name).
vhpiHandleT nvc_vhpi_create_region(vhpiHandleT parent, const char *name);

#ifdef __cplusplus
}
#endif

#endif  // VHPI_NVC_H
