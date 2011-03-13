#import "Resource.h"

#define self resStyle

class {
	String style;
};

action(Switch) {
	UserSessionInstance usrSess = (UserSessionInstance) Session_GetData(sess);

	if (String_Equals(this->style.rd, $("black")) ||
		String_Equals(this->style.rd, $("brown")) ||
		String_Equals(this->style.rd, $("white")))
	{
		UserSession_SetStyle(usrSess, this->style.rd);
		Session_SetChanged(sess);
	}

	if (Request_GetReferer(req).len == 0) {
		RedirectResponse(resp, $$("/"));
	} else {
		RedirectResponse(resp,
			String_Clone(
				Request_GetReferer(req)));
	}
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.members = {
		{ .name = $("style"), Member(style) }
	},

	.routes = {
		{ .path   = $("/style/:style"),
		  .action = Action(Switch) }
	}
};

RouterConstructor;
