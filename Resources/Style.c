#import "Resource.h"

#define self resStyle

class {
	String style;
};

action(Switch) {
	UserSessionInstance usrSess = (UserSessionInstance) Session_GetData(sess);

	if (String_Equals(this->style.prot, $("black")) ||
		String_Equals(this->style.prot, $("brown")) ||
		String_Equals(this->style.prot, $("white")))
	{
		UserSession_SetStyle(usrSess, this->style.prot);
		Session_SetChanged(sess);
	}

	if (req.referer.len == 0) {
		RedirectResponse(resp, $("/"));
	} else {
		RedirectResponse(resp, String_Clone(req.referer.prot));
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
