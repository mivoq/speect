typedef struct
{
} SPlugin;


%types(SPlugin = SObject);


%extend SPlugin
{
	SPlugin(const char *path, s_erc *error)
	{
		SPlugin *plugin;


		plugin = s_pm_load_plugin(path, error);
		if (*error != S_SUCCESS)
			return NULL;

		return plugin;
	}

	~SPlugin()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SPlugin()", &error);
	}
}
