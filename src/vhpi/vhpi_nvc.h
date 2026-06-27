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

// Create a new signal of the given type in a region.  This is like
// vhpi_create(vhpiSigDeclK, region, type) but additionally allows the
// name of the new signal to be specified.  Returns a handle to the new
// signal or NULL on error.  The type must be constrained and have a
// scalar or homogeneous array type.
vhpiHandleT nvc_vhpi_create_signal(vhpiHandleT region, vhpiHandleT type,
                                   const char *name);

#ifdef __cplusplus
}
#endif

#endif  // VHPI_NVC_H
