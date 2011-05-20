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

/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "source.h" /* TODO: include header files */


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SYourClass YourClass; /* TODO: SYour class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/* TODO: register/free functions */

/* local functions to register and free classes */
S_LOCAL void _s_your_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(S_OBJECTCLASS(&YourClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_your_class_reg",
			  "Failed to register SYourClass");
}


S_LOCAL void _s_your_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(S_OBJECTCLASS(&YourClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_your_class_free",
			  "Failed to free SYourClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SYour *self = obj;


	S_CLR_ERR(error);

	/* TODO: initialize object members */
}


static void Destroy(void *obj, s_erc *error)
{
	SYour *self = obj;


	S_CLR_ERR(error);

	/* TODO: free object members */
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/* TODO: implement class methods, Resize example below */

static void Resize(SYour *self, uint32 new_size, s_erc *error)
{
	float *samples;


	S_CLR_ERR(error);

	samples = S_CALLOC(float, new_size);
	if (samples == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "Resize",
				  "Failed to allocate memory for 'float' object");
		return;
	}

	if (self->samples != NULL)
	{
		memmove(samples, self->samples,
				sizeof(float) * ((new_size < self->num_samples) ? new_size : self->num_samples));

		S_FREE(self->samples);
	}

	self->samples = samples;
	self->num_samples = new_size;
}


/* TODO: initialize class */
/************************************************************************************/
/*                                                                                  */
/* SYour class initialization                                                       */
/*                                                                                  */
/************************************************************************************/

static SYourClass AudioClass =
{
	/* SObjectClass */
	{
		"SYour",
		sizeof(SYour),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SYourClass */
	/* TODO: add class methods */
};
