/// ____________________________________________________________________ ///
///                                                                      ///
/// SoFiA 2.0.0-beta (LinkerPar.h) - Source Finding Application          ///
/// Copyright (C) 2019 Tobias Westmeier                                  ///
/// ____________________________________________________________________ ///
///                                                                      ///
/// Address:  Tobias Westmeier                                           ///
///           ICRAR M468                                                 ///
///           The University of Western Australia                        ///
///           35 Stirling Highway                                        ///
///           Crawley WA 6009                                            ///
///           Australia                                                  ///
///                                                                      ///
/// E-mail:   tobias.westmeier [at] uwa.edu.au                           ///
/// ____________________________________________________________________ ///
///                                                                      ///
/// This program is free software: you can redistribute it and/or modify ///
/// it under the terms of the GNU General Public License as published by ///
/// the Free Software Foundation, either version 3 of the License, or    ///
/// (at your option) any later version.                                  ///
///                                                                      ///
/// This program is distributed in the hope that it will be useful,      ///
/// but WITHOUT ANY WARRANTY; without even the implied warranty of       ///
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         ///
/// GNU General Public License for more details.                         ///
///                                                                      ///
/// You should have received a copy of the GNU General Public License    ///
/// along with this program. If not, see http://www.gnu.org/licenses/.   ///
/// ____________________________________________________________________ ///
///                                                                      ///

#ifndef LINKERPAR_H
#define LINKERPAR_H

#include "common.h"

typedef class LinkerPar LinkerPar;


// ----------------------------------------------------------------- //
// Class 'LinkerPar'                                                 //
// ----------------------------------------------------------------- //
// The purpose of this class is to provide a structure for storing   //
// and updating source parameters handled by the linker implemented  //
// in the class 'DataCube'.                                          //
// ----------------------------------------------------------------- //

class LinkerPar
{
	size_t    size;
	size_t    block_size;
	size_t   *label;
	size_t   *n_pix;
	uint16_t *x_min;
	uint16_t *x_max;
	uint16_t *y_min;
	uint16_t *y_max;
	uint16_t *z_min;
	uint16_t *z_max;
};

// Constructor and destructor

public LinkerPar *LinkerPar_new        (void);
public void       LinkerPar_delete     (LinkerPar *this);

// Public methods

public void       LinkerPar_push       (LinkerPar *this, const uint16_t x, const uint16_t y, const uint16_t z);
public void       LinkerPar_update     (LinkerPar *this, const size_t index, const uint16_t x, const uint16_t y, const uint16_t z);
public size_t     LinkerPar_get_size   (const LinkerPar *this, const size_t index, const int axis);
public void       LinkerPar_set_label  (LinkerPar *this, const size_t index, const size_t label);
public size_t     LinkerPar_get_label  (const LinkerPar *this, const size_t index);
public void       LinkerPar_reduce     (LinkerPar *this);
public void       LinkerPar_print_info (const LinkerPar *this);

#endif