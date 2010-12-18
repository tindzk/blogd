#import "Resource.h"

#define self resCategories

class {

};

action(Categories) {
	MainTemplate main = GetMainTemplate(sess);

	CategoriesTemplate tpl = {
		.categories = main.categories,
		.cats       = main.cats
	};

	main.page = pgCategories;
	main.body = Template(Template_Categories, tpl);

	TemplateResponse(resp, Template_Main, &main);
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.routes = {
		{ .path   = $("/categories"),
		  .action = Action(Categories) }
	}
};

RouterConstructor;
