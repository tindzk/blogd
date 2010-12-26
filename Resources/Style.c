#import "Resource.h"

#define self resStyle

class {
	String style;
};

action(Switch) {
	UserSessionInstance usrSess = (UserSessionInstance) Session_GetData(sess);

	if (String_Equals(this->style, $("black"))
	 || String_Equals(this->style, $("brown"))
	 || String_Equals(this->style, $("white")))
	{
		UserSession_SetStyle(usrSess, this->style);
		Session_SetChanged(sess);
	}

	if (req.referer.len == 0) {
		RedirectResponse(resp, $("/"));
	} else {
		RedirectResponse(resp, req.referer);
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
