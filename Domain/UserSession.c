#import "UserSession.h"

#define self UserSession

static def(void, Init) {
	this->style = HeapString(0);
}

static def(void, Destroy) {
	String_Destroy(&this->style);
}

def(void, SetStyle, String style) {
	String_Copy(&this->style, style);
}

def(String, GetStyle) {
	return String_Disown(this->style);
}

Impl(BackendSession) = {
	shareSize,
	share(Init, init),
	share(Destroy, destroy)
};
