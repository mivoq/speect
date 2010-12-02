/************************************************************************************/
/* LICENSE INFO                                                                     */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  :                                                                        */
/* DATE    :                                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Description, ...                                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/* see TODO:  where to change */

#ifndef _SPCT_PLUGIN_YOUR_H__ /* TODO: header guard */
#define _SPCT_PLUGIN_YOUR_H__


/* TODO: documentation */
/**
 * @file source.h
 * A simple description
 */


/**
 * @defgroup ClassName description
 * description
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"


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
 * @hideinitializer
 * Return the given #SYour child/parent class object as a
 * #SYour object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SYour* type.
 * @note This casting is not safety checked.
 */
#define S_YOUR(SELF)    ((SObjectName *)(SELF))  /* TODO: add object name */


/**
 * @hideinitializer
 * Call the given function method of the given #SYour,
 * see full description #S_AUDIO_CALL for usage.
 *
 * @param SELF The given #SYour*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_AUDIO_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_YOUR_CALL(SELF, FUNC)					\
	((SClassName *)S_OBJECT_CLS(SELF))->FUNC  /* TODO: add class name */



/**
 * @hideinitializer
 * Test if the given function method of the given #SYour
 * can be called.
 *
 * @param SELF The given #SYour*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_YOUR_METH_VALID(SELF, FUNC)		\
	S_YOUR_CALL(SELF, FUNC) ? TRUE : FALSE   /* TODO: macro names */



/************************************************************************************/
/*                                                                                  */
/* SYour definition                                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SYour structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/* TODO: Object members */
} SYour;


/************************************************************************************/
/*                                                                                  */
/* SYourClass definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The SYourClass structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass _inherit;

	/* Class methods */

	/* TODO: object methods */
} SYourClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* TODO: register/free functions */
/**
 * Register the #SYour plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_your_class_reg(s_erc *error);


/**
 * Free the #SYour plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_your_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_YOUR_H__ */
