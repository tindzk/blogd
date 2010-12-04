#import "Resource.h"

#undef self
#define self resArticle

class {
	String article;
};

action(Article) {
	ConfigurationInstance config = Configuration_GetInstance();

	LicenseTemplate license = {
		Configuration_GetLicense(config)
	};

	ArticleListingInstance listing = ArticleListing_GetInstance();

	ArticleInstance article = ArticleListing_GetArticle(listing,
		Utils_ExtractName(String_Trim(this->article)));

	ArticleTemplate tpl = {
		.error = Article_IsNull(article)
			? $("Article not found.")
			: $(""),

		.article = article
	};

	MainTemplate main = GetMainTemplate(Session_GetData(sess));

	main.body   = Template(Template_Article, tpl);
	main.footer = Template(Template_License, license);

	if (!Article_IsNull(article)) {
		main.title = String_Format($("%: %"),
			Configuration_GetTitle(config),
			Article_GetTitle(article));
	}

	TemplateResponse(resp, Template_Main, &main);

	if (main.title.mutable) {
		String_Destroy(&main.title);
	}
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.members = {
		{ .name = $("article"), Member(article) }
	},

	.routes = {
		{ .path   = $("/article/$article"),
		  .action = Action(Article) }
	}
};

RouterConstructor;
