#ifndef __PM_ARG_PARSER_H__
#define __PM_ARG_PARSER_H__

typedef int (*PM_ARG_PARSER_CALLBACK)();

typedef struct tagPM_ARGUMENT_ITEM
{
	const char* pszName;
	char chAlias;
	const char* pszHelp;
	const char* pszDefault;
}PM_ARGUMENT_ITEM;

typedef struct tagPM_RETURN_ITEM
{
	PM_INT	nRetValue;
	const char* pszHelp;
}PM_RETURN_ITEM;

typedef struct tagPM_ARGUMENT_INFO
{
	PM_ARG_PARSER_CALLBACK pfCallBack;
	const char* pszCommand;
	const char* pszHelp;
	PM_ARGUMENT_ITEM* pParaInfos;
	PM_RETURN_ITEM* pRetInfos;
	PM_BOOL bNotShowHelp;
}PM_ARGUMENT_INFO;

const char* pmarg_get_opt( const char* pszName );
int pmarg_get_opt_int( const char* pszName );
PM_BOOL pmarg_is_opt_exist( const char* pszName );

void pm_register_tool( PM_ARGUMENT_INFO* pInfo );
int pmarg_run_tool( int argc, char* argv[], const char* pszToolName, PM_ARG_PARSER_CALLBACK onVersion, PM_ARG_PARSER_CALLBACK _onHelp );


#define PMARG_DEFOPT_BEGIN static PM_ARGUMENT_ITEM g_paraInfos[] = {
#define PMARG_DEFOPT( name, alias, help, default ) { name, alias, help, default },
#define PMARG_DEFOPT_END { PM_NULL }};

#define PMARG_DEFRET_BEGIN static PM_RETURN_ITEM g_retInfos[] = {
#define PMARG_DEFRET( ret_value,  help) { ret_value, help},
#define PMARG_DEFRET_END { PM_NULL }};

#define PMARG_REGISTER_TOOL2( name, function, help, bnohelp ) int function();\
        class function##_init \
        {\
        public:\
            function##_init()\
            {\
                PM_ARGUMENT_INFO info = { function, name, help, g_paraInfos, PM_NULL, bnohelp };\
                pm_register_tool(&info);\
            }\
        };\
        static function##_init g_##function##_init

#define PMARG_REGISTER_TOOL_EX2( name, function, help, bnohelp ) int function();\
        class function##_init \
        {\
        public:\
            function##_init()\
            {\
                PM_ARGUMENT_INFO info = { function, name, help, g_paraInfos, g_retInfos, bnohelp };\
                pm_register_tool(&info);\
            }\
        };\
        static function##_init g_##function##_init

#define PMARG_REGISTER_TOOL( function, help ) PMARG_REGISTER_TOOL2( #function, function, help, 0 )
#define PMARG_REGISTER_TOOL_NOHELP( function, help ) PMARG_REGISTER_TOOL2( #function, function, help, 1 )

#define PMARG_REGISTER_TOOL_NOHELP_EX( function, help ) PMARG_REGISTER_TOOL_EX2( #function, function, help, 1 ) 
#define PMARG_REGISTER_TOOL_EX( function, help ) PMARG_REGISTER_TOOL_EX2( #function, function, help, 0 ) 

#endif

