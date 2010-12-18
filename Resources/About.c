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
	main.body = Template(Template_About, tpl);

	TemplateResponse(resp, Template_Main, &main);
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.routes = {
		{ .path   = $("/about"),
		  .action = Action(View) }
	}
};

RouterConstructor;
