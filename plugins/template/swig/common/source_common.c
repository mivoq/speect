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
/* SWIG helper functions                                                            */
/*                                                                                  */
/************************************************************************************/

%include "cstring.i"

%cstring_output_allocate_size(char **s, int *slen, free(*$1));


/************************************************************************************/
/*                                                                                  */
/* Your helper functions /* TODO: */                                                */
/*                                                                                  */
/************************************************************************************/

%inline
%{
	void saudio_samples(char **s, int *slen, const SAudio *audio, s_erc *error)
	{
		sint16 *samples;
		uint32 counter;


		samples = S_CALLOC(sint16, audio->num_samples);
		if (samples == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "saudio_samples",
					  "Failed to allocate audio samples");
			return;
		}

		for (counter = 0; counter < audio->num_samples; counter++)
			samples[counter] = (sint16)audio->samples[counter];

		*slen = audio->num_samples * sizeof(sint16);
		*s = (char*)samples;
	}
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SYour Class TODO: makes a Python class from your                      */
/* functions, see example below                                                     */
/*                                                                                  */
/************************************************************************************/


typedef struct
{
} SAudio;

%types(SAudio = SObject);

%nodefaultctor SAudio;

%nodefaultdtor SAudio;

%extend SAudio
{
	void resize(uint32 new_size, s_erc *error)
	{
		S_CLR_ERR(error);

		if (!S_AUDIO_METH_VALID($self, resize))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "resize",
					  "Audio method \"resize\" not implemented");
			return;
		}

		S_AUDIO_CALL($self, resize)($self, new_size, error);
		S_CHK_ERR(error, S_CONTERR,
				  "resize",
				  "Call to Audio method \"resize\" failed");
		return;
	}


	void scale(float factor, s_erc *error)
	{
		S_CLR_ERR(error);

		if (!S_AUDIO_METH_VALID($self, scale))
		{
			S_CTX_ERR(error, S_METHINVLD,
					  "scale",
					  "Audio method \"scale\" not implemented");
			return;
		}

		S_AUDIO_CALL($self, scale)($self, factor, error);
		S_CHK_ERR(error, S_CONTERR,
				  "scale",
				  "Call to Audio method \"scale\" failed");
		return;
	}


	uint32 num_samples()
	{
		return $self->num_samples;
	}


	uint32 sample_rate()
	{
		return $self->sample_rate;
	}
};

