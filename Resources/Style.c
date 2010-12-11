#import "Resource.h"

#define self resStyle

class {
	String style;
};

action(Switch) {
	if (String_Equals(this->style, $("black"))) {
		Session_SetData(sess, $("black"));
	} else if (String_Equals(this->style, $("brown"))) {
		Session_SetData(sess, $("brown"));
	} else if (String_Equals(this->style, $("white"))) {
		Session_SetData(sess, $("white"));
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
		{ .path   = $("/style/$style"),
		  .action = Action(Switch) }
	}
};

RouterConstructor;
