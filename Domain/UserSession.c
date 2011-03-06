#import "UserSession.h"

#define self UserSession

static def(void, Init) {
	this->style = String_New(0);
}

static def(void, Destroy) {
	String_Destroy(&this->style);
}

def(void, SetStyle, ProtString style) {
	String_Copy(&this->style, style);
}

def(ProtString, GetStyle) {
	return this->style.prot;
}

Impl(BackendSession) = {
	.size = sizeof(self),

	.init    = (void *) ref(Init),
	.destroy = (void *) ref(Destroy)
};
