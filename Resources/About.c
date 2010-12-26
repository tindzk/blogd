#import "Resource.h"

#define self resAbout

class {

};

action(View) {
	ConfigurationInstance config = Configuration_GetInstance();

	AboutTemplate tpl = {
		Configuration_GetAbout(config)
	};

	MainTemplate main = GetMainTemplate(sess);

	main.page = pgAbout;
	main.body = tplAbout(&tpl);

	TemplateResponse(resp, tplMain(&main));
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.routes = {
		{ .path   = $("/about"),
		  .action = Action(View) }
	}
};

RouterConstructor;
