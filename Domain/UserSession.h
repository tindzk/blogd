#import <Debit/SessionManager.h>

#define self UserSession

class {
	String style;
};

def(void, SetStyle, RdString style);
def(RdString, GetStyle);

ExportImpl(BackendSession);

#undef self
