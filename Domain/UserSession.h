#import <Debit/SessionManager.h>

#define self UserSession

class {
	String style;
};

def(void, SetStyle, ProtString style);
def(ProtString, GetStyle);

ExportImpl(BackendSession);

#undef self
