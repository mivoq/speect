/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy     */
/* of this software and associated documentation files (the "Software"), to deal    */
/* in the Software without restriction, including without limitation the rights     */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        */
/* copies of the Software, and to permit persons to whom the Software is            */
/* furnished to do so, subject to the following conditions:                         */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        */
/* THE SOFTWARE.                                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : 03 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Macros for the base object.                                                      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_OBJECT_MACROS_H__
#define _SPCT_OBJECT_MACROS_H__


/**
 * @file object_macros.h
 * Macros for the base object and class.
 */


/**
 * @ingroup SObjSystem
 * @defgroup SObjMacros Object/Class Macros
 * Macros for various object and class functions.
 * Defines macros for the creation/deletion and casting of object of
 * all types as well as some miscelanuous macros to get the base
 * object, base class and reference count of objects.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @name New
 * @{
 */


/**
 * @hideinitializer
 * Create a new object of the given object type @a name.
 * Also initializes the object members and inherited members.
 * For example:
 * @code
 SMapList *myMapList = (SMapList*)S_NEW("SMapList", error);
 @endcode
 *
 * @param OBJTYPE_NAME String of the type of the object to create.
 * @param ERROR Error code (of type #s_erc*).
 *
 * @return Pointer to the newly created object (#SObject*).
 *
 * @todo add reference that this type must have been registered.
 */
#define S_NEW(OBJTYPE_NAME, ERROR)					\
	(SObjectNewFromName((OBJTYPE_NAME), (ERROR)))


/**
 * @}
 */


/**
 * @name Delete
 * @{
 */


/**
 * @hideinitializer
 * Delete an object.
 * The delete macro works in two stages, first a call is made to
 * #SObjectClass::dispose, then if the object is no longer referenced,
 * a call is made to #SObjectClass::destroy. The given object pointer,
 * @a SELF, is set to @c NULL, regardless of whether the object was
 * deleted or not.
 *
 *
 * This allows full control over the reference counting of an object.
 * For most normal objects the #SObjectClass::dispose function would
 * look like the @a SString dispose function in
 * <tt>src/base/objsystem/primitives.c</tt>:
 * @code
 static void DisposeString(void *obj, s_erc *error)
 {
 S_CLR_ERR(error);
 SObjectDecRef(S_OBJECT(obj));
 }
 @endcode
 * But, if the situation arrises that an object should not be deleted,
 * by users or other objects (for example read data), then one sets
 * the #SObjectClass::dispose function of that object's class
 * to @c NULL in the object class initialization and also a
 * #SObjectIncRef statement to the #SObjectClass::init of that
 * object. Therefore, when the object is initialized it's reference
 * counter is increased, and when S_DELETE is called it does not get
 * decreased by #SObjectClass::dispose, and not deleted. The object
 * would then have to be deleted by a call to #S_FORCE_DELETE.
 *
 * @param SELF Pointer to the object to delete.
 * @param FUNC_NAME The current function name (optional, can
 * be @c NULL, used for logging if an error occured).
 * @param ERROR Error code (of type #s_erc*).
 */
#define S_DELETE(SELF, FUNC_NAME, ERROR)					\
	do {													\
		if (SELF != NULL)									\
		{													\
			s_erc _local_err = S_SUCCESS;					\
			s_erc _delete_err = S_SUCCESS;					\
															\
															\
			SObjectDispose(S_OBJECT(SELF), &_delete_err);	\
			if (S_CHK_ERR(&_delete_err, S_CONTERR,			\
						  "S_DELETE",						\
						  "Call to \"SObjectDispose\" "		\
						  " failed"))						\
			{												\
				_local_err = _delete_err;					\
			}												\
															\
			S_CLR_ERR(&_delete_err);						\
															\
			if (S_OBJECT_REF(SELF) == 0)					\
			{												\
				SObjectDelete(S_OBJECT(SELF),				\
							  &_delete_err);				\
				if (S_CHK_ERR(&_delete_err, S_CONTERR,		\
							  "S_DELETE",					\
							  "Call to \"SObjectDelete"		\
							  "\" failed"))					\
				{											\
					_local_err = _delete_err;				\
				}											\
			}												\
															\
			if ((ERROR != NULL)								\
			    && (*ERROR == S_SUCCESS)					\
			    && (_local_err != S_SUCCESS))				\
			{												\
				if (FUNC_NAME != NULL)						\
					S_CTX_ERR(ERROR, _local_err,			\
							  FUNC_NAME,					\
							  "Failed to delete object");	\
				else										\
					*ERROR = _local_err;					\
			}												\
															\
			SELF = NULL;									\
		}													\
	} while (0)


/**
 * @hideinitializer
 * Force deletion of an object. The object is deleted, with a call
 * to #SObjectClass::destroy to free up the object resources,
 * regardless of whether it is referenced or not. The pointer to the
 * object will point to @c NULL after this operation.
 *
 * @param SELF Pointer to the object to delete.
 * @param FUNC_NAME The current function name (optional, can
 * be @c NULL, used for logging if an error occured).
 * @param ERROR Error code (of type #s_erc*).
 *
 * @sa S_DELETE
 */
#define S_FORCE_DELETE(SELF, FUNC_NAME, ERROR)					\
	do {														\
		if (SELF != NULL)										\
		{														\
			s_erc _delete_err = S_SUCCESS;						\
																\
																\
			SObjectDelete(S_OBJECT(SELF), &_delete_err);		\
			if (S_CHK_ERR(&_delete_err, S_CONTERR,				\
						  "S_DELETE",							\
						  "Call to \"SObjectDelete"				\
						  "\" failed"))							\
			{													\
				if ((ERROR != NULL)								\
					&& (*ERROR == S_SUCCESS)					\
					&& (_delete_err != S_SUCCESS))				\
				{												\
					if (FUNC_NAME != NULL)						\
						S_CTX_ERR(ERROR, _delete_err,			\
								  FUNC_NAME,					\
								  "Failed to delete object");	\
					else										\
						*ERROR = _delete_err;					\
				}												\
			}													\
			 													\
			SELF = NULL;										\
		}														\
	} while (0)


/**
 * @}
 */


/**
 * @name Cast
 * @{
 */


/**
 * @hideinitializer
 * Safe cast the given object to the given type.
 *
 * @param OBJECT Pointer to the object to cast.
 * @param OBJTYPE The type of the object to cast the given object to.
 * @param ERROR Error code (of type #s_erc*).
 *
 * @return Pointer to Object casted to OBJTYPE, or @c NULL on error.
 */
#define S_CAST_SAFE(OBJECT, OBJTYPE, ERROR)							\
	((OBJTYPE*)s_safe_cast(S_OBJECT(OBJECT), ( # OBJTYPE), ERROR))


/**
 * @hideinitializer
 * Unsafe cast the given object to the given type.
 * This cast is unsafe (but faster than #S_CAST_SAFE),
 * and should only be used when you are sure of the object types.
 *
 * @param OBJECT Pointer to the object to cast.
 * @param OBJTYPE The type of the object to cast the given object to.
 *
 * @return Pointer to Object casted to OBJTYPE.
 */
#define S_CAST_UNSAFE(OBJECT, OBJTYPE) ((OBJTYPE*)S_OBJECT(OBJECT))


/**
 * @hideinitializer
 * @def S_CAST
 * Cast the given object to the given type. This cast reverts to
 * either #S_CAST_SAFE or #S_CAST_SAFE, depending on the build time
 * definition of SPCT_DO_SAFE_CAST.
 *
 * @param OBJECT Pointer to the object to cast.
 * @param OBJTYPE The type of the object to cast the given object to.
 * @param ERROR Error code (of type #s_erc*).
 *
 * @return Pointer to Object casted to OBJTYPE.
 *
 * @todo add link to build stuff.
 */
#ifdef SPCT_DO_SAFE_CAST
#  define S_CAST(OBJECT, OBJTYPE, ERROR) S_CAST_SAFE(OBJECT, OBJTYPE, ERROR)
#else /* !SPCT_DO_SAFE_CAST */
#  define S_CAST(OBJECT, OBJTYPE, ERROR) S_CAST_UNSAFE(OBJECT, OBJTYPE)
#endif /* SPCT_DO_SAFE_CAST */


/**
 * @}
 */


/**
 * @name Function Methods
 * @{
 */


/**
 * @hideinitializer
 * Call the given function method of the given #SObject,
 * see full description #S_OBJECT_CALL for usage.
 *
 * @param SELF The given #SObject*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_OBJECT_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_OBJECT_CALL(SELF, FUNC)				\
	S_OBJECT_CLS(SELF)->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SObject
 * can be called.
 *
 * @param SELF The given #SObject*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_OBJECT_METH_VALID(SELF, FUNC)			\
	S_OBJECT_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/**
 * @name Misc
 * @{
 */


/**
 * @hideinitializer
 * Return the given object as a base object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SObject* type.
 *
 * @note This casting is not safety checked.
 */
#define S_OBJECT(SELF) ((SObject*)(SELF))


/**
 * @hideinitializer
 * Get the given object's reference count.
 *
 * @param SELF The given object.
 *
 * @return Constant #uint32 reference count of the given object.
 *
 * @note This casting is not safety checked.
 */
#define S_OBJECT_REF(SELF) ((const uint32)(S_OBJECT(SELF)->ref))


/**
 * @}
 */


/**
 * @name Class
 * @{
 */


/**
 * @hideinitializer
 * Get the given object's base class.
 *
 * @param SELF The given object.
 *
 * @return Pointer to the #SObjectClass class of the given object.
 *
 * @note This casting is not safety checked.
 */
#define S_OBJECT_CLS(SELF) ((SObjectClass*)S_OBJECT(SELF)->cls)


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_OBJECT_MACROS_H__ */
