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

#ifdef __cplusplus
}
#endif

#endif  // VHPI_NVC_H
