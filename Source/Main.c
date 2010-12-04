#import <Server.h>
#import <String.h>
#import <Logger.h>
#import <Signal.h>
#import <Integer.h>
#import <Exception.h>
#import <GenericClientListener.h>

#import <Debit/HttpConnection.h>

#import <Domain/Configuration.h>
#import <Domain/ArticleListing.h>

Logger logger;

void OnLogMessage(__unused void *ptr, String msg, Logger_Level level, String file, int line) {
	String slevel = Logger_ResolveLevel(level);
	String sline  = Integer_ToString(line);

	String tmp;
	String_Print(tmp = String_Format(
		String("[%] % (%:%)\n"),
		slevel, msg, file, sline));

	String_Destroy(&tmp);
}

bool startServer(Server *server, GenericClientListener *listener) {
	try {
		Server_Init(server, 8080, &GenericClientListenerImpl, listener);
		Logger_Info(&logger, $("Server started."));
		excReturn true;
	} clean catch (Socket, excAddressInUse) {
		Logger_Error(&logger, $("The address is already in use!"));
		excReturn false;
	} finally {

	} tryEnd;

	return false;
}

int main(int argc, char *argv[]) {
	Signal0();

	Logger_Init(&logger, Callback(NULL, OnLogMessage),
		Logger_Level_Fatal |
		Logger_Level_Crit  |
		Logger_Level_Error |
		Logger_Level_Warn  |
		Logger_Level_Info  |
		Logger_Level_Debug |
		Logger_Level_Trace);

	String path = Blog_ConfigPath;

	if (argc > 1) {
		path = String_FromNul(argv[1]);
	}

	ConfigurationInstance config = Configuration_GetInstance();
	Configuration_Parse(config, path);

	ArticleListingInstance listing = ArticleListing_GetInstance();
	ArticleListing_Populate(listing,
		Configuration_GetArticlePath(config));

	GenericClientListener listener;
	GenericClientListener_Init(&listener, &HttpConnectionImpl);

	Server server;

	if (!startServer(&server, &listener)) {
		return ExitStatus_Failure;
	}

	try {
		while (true) {
			Server_Process(&server);
		}
	} clean catch(Signal, excSigInt) {
		Logger_Info(&logger, $("Server shutdown."));
	} catchAny {
		Exception_Print(e);

#if Exception_SaveTrace
		Backtrace_PrintTrace(__exc_mgr.e.trace, __exc_mgr.e.traceItems);
#endif

		excReturn ExitStatus_Failure;
	} finally {
		Server_Destroy(&server);
	} tryEnd;

	return ExitStatus_Success;
}
