/// ____________________________________________________________________ ///
///                                                                      ///
/// SoFiA 2.2.1 (Flagger.c) - Source Finding Application                 ///
/// Copyright (C) 2020 Tobias Westmeier                                  ///
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

#include <stdlib.h>
#include <stdarg.h>
#include "Flagger.h"


// ----------------------------------------------------------------- //
// Declaration of properties of class Flagger                        //
// ----------------------------------------------------------------- //

CLASS Flagger
{
	int n_par[SHAPE_COUNT];
	size_t size;
	int *shape;
	long int **parameters;
};



// ----------------------------------------------------------------- //
// Standard constructor                                              //
// ----------------------------------------------------------------- //
// Arguments:                                                        //
//                                                                   //
//   No arguments.                                                   //
//                                                                   //
// Return value:                                                     //
//                                                                   //
//   Pointer to newly created Flagger object.                        //
//                                                                   //
// Description:                                                      //
//                                                                   //
//   Standard constructor. Will create a new, empty Flagger object   //
//   and return a pointer to the newly created object. Note that the //
//   destructor will need to be called explicitly once the object is //
//   no longer required to release any memory allocated during the   //
//   lifetime of the object.                                         //
// ----------------------------------------------------------------- //

PUBLIC Flagger *Flagger_new(void)
{
	Flagger *self = (Flagger*)memory(MALLOC, 1, sizeof(Flagger));
	
	for(size_t i = SHAPE_COUNT; i--;) self->n_par[i] = 0;  // Initialise with 0 for safety reasons
	self->n_par[PIXEL]   = 2;
	self->n_par[CHANNEL] = 1;
	self->n_par[REGION]  = 6;
	self->n_par[CIRCLE]  = 3;
	self->size           = 0;
	self->shape          = NULL;
	self->parameters     = NULL;
	
	return self;
}


// ----------------------------------------------------------------- //
// Destructor                                                        //
// ----------------------------------------------------------------- //
// Arguments:                                                        //
//                                                                   //
//   (1) self     - Object self-reference.                           //
//                                                                   //
// Return value:                                                     //
//                                                                   //
//   No return value.                                                //
//                                                                   //
// Description:                                                      //
//                                                                   //
//   Destructor. Note that the destructor must be called explicitly  //
//   if the object is no longer required. This will release the me-  //
//   mory occupied by the object.                                    //
// ----------------------------------------------------------------- //

PUBLIC void Flagger_delete(Flagger *self)
{
	if(self != NULL)
	{
		for(size_t i = self->size; i--;) free(self->parameters[i]);
		free(self->parameters);
		free(self->shape);
		free(self);
	}
	
	return;
}



// ----------------------------------------------------------------- //
// Return number of flagging items                                   //
// ----------------------------------------------------------------- //
// Arguments:                                                        //
//                                                                   //
//   (1) self     - Object self-reference.                           //
//                                                                   //
// Return value:                                                     //
//                                                                   //
//   Number of currently held flagging instructions.                 //
//                                                                   //
// Description:                                                      //
//                                                                   //
//   Public method for returning the number of currently held flag-  //
//   ging instructions.                                              //
// ----------------------------------------------------------------- //

PUBLIC size_t Flagger_size(const Flagger *self)
{
	check_null(self);
	return self->size;
}



// ----------------------------------------------------------------- //
// Return number of parameters for given shape                       //
// ----------------------------------------------------------------- //
// Arguments:                                                        //
//                                                                   //
//   (1) self     - Object self-reference.                           //
//   (2) shape    - Shape to be enquired about.                      //
//                                                                   //
// Return value:                                                     //
//                                                                   //
//   Number of parameters required by the specified shape.           //
//                                                                   //
// Description:                                                      //
//                                                                   //
//   Public method for returning the number of parameters associated //
//   with the specified shape. For example, if shape is PIXEL, then  //
//   two parameters, x and y, will be required to define the shape.  //
// ----------------------------------------------------------------- //

PUBLIC int Flagger_npar(const Flagger *self, const int shape)
{
	check_null(self);
	ensure(shape >= 0 && shape < SHAPE_COUNT, ERR_USER_INPUT, "Flagging shape %d not recognised.", shape);
	return self->n_par[shape];
}



// ----------------------------------------------------------------- //
// Add flagging instruction to object                                //
// ----------------------------------------------------------------- //
// Arguments:                                                        //
//                                                                   //
//   (1) self     - Object self-reference.                           //
//   (2) shape    - Shape of the flagging region to be added         //
//   (3) ...      - Additional parameters required for defining the  //
//                  specified shape.                                 //
//                                                                   //
// Return value:                                                     //
//                                                                   //
//   Pointer to the object with the new instruction included.        //
//                                                                   //
// Description:                                                      //
//                                                                   //
//   Public method for adding a new flagging instruction to the cur- //
//   rent list of instructions held by the object. The shape must be //
//   one of the shapes supported by the class. The required number   //
//   of additional parameters defining the shape must be supplied.   //
//   These must be integer numbers in absolute pixel coordinates. A  //
//   pointer to the object after insertion of the new instruction    //
//   will be returned to allow chaining of insert commands.          //
// ----------------------------------------------------------------- //

PUBLIC Flagger *Flagger_add(Flagger *self, const int shape, ...)
{
	// Sanity checks
	check_null(self);
	ensure(shape >= 0 && shape < SHAPE_COUNT, ERR_USER_INPUT, "Flagging shape %d not recognised.", shape);
	
	// Reserve additional memory
	self->shape = (int *)memory_realloc(self->shape, self->size + 1, sizeof(int));
	self->parameters = (long int **)memory_realloc(self->parameters, self->size + 1, sizeof(long int *));
	self->parameters[self->size] = (long int *)memory(MALLOC, self->n_par[shape], sizeof(long int));
	
	// Update properties
	va_list ap;
	va_start(ap, shape);
	for(int i = 0; i < self->n_par[shape]; ++i) self->parameters[self->size][i] = va_arg(ap, long int);
	va_end(ap);
	
	self->shape[self->size] = shape;
	self->size += 1;
	
	return self;
}



// ----------------------------------------------------------------- //
// Extract flagging instruction at the specified index               //
// ----------------------------------------------------------------- //
// Arguments:                                                        //
//                                                                   //
//   (1) self       - Object self-reference.                         //
//   (2) index      - Index of the instruction to be extracted.      //
//   (3) shape      - Pointer to integer to hold shape information.  //
//   (4) parameters - Pointer to a pointer to integer to hold the    //
//                    array of parameters associated with the shape. //
//                                                                   //
// Return value:                                                     //
//                                                                   //
//   No return value.                                                //
//                                                                   //
// Description:                                                      //
//                                                                   //
//   Public method for extracting the flagging instruction at the    //
//   specified index. Two pointers will need to be provided: shape   //
//   must be a pointer to int for storing the shape information of   //
//   the item, while parameters must be a pointer to a pointer to    //
//   const long int and will point to the array of parameters asso-  //
//   ciated with the shape. Usage example:                           //
//                                                                   //
//     int *shape;                                                   //
//     const long int **parameters;                                  //
//     Flagger_get(flagger, 0, &shape, &parameters);                 //
//                                                                   //
//   shape and parameters will then contain the relevant information //
//   about the region to be flagged.                                 //
// ----------------------------------------------------------------- //

PUBLIC void Flagger_get(const Flagger *self, const size_t index, int *shape, const long int **parameters)
{
	// Sanity checks
	check_null(self);
	check_null(shape);
	check_null(parameters);
	ensure(index < self->size, ERR_INDEX_RANGE, "Index out of range in Flagger_get_item().");
	
	*shape = self->shape[index];
	*parameters = self->parameters[index];
	
	return;
}
