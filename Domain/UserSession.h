#import <Debit/SessionManager.h>

#define self UserSession

class {
	String style;
};

def(void, SetStyle, String style);
def(String, GetStyle);

ExportImpl(BackendSession);

#undef self
