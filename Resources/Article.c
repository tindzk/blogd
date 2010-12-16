#import "Resource.h"

#define self resArticle

class {
	String file;
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
		.url    = Configuration_GetUrl(config),
		.flattr = Configuration_GetFlattr(config),

		.error = Article_IsNull(article)
			? $("Article not found.")
			: $(""),

		.article = article
	};

	if (Article_IsNull(article)) {
		Response_SetStatus(resp, HTTP_Status_ClientError_NotFound);
	}

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

action(ServeFile) {
	ConfigurationInstance config = Configuration_GetInstance();

	String path = String_Format($("%/%-%"),
		Configuration_GetArticlePath(config),
		this->article, this->file);

	FileResponse(resp, path, *(DateTime *) &req.lastModified);

	String_Destroy(&path);
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.members = {
		{ .name = $("file"),    Member(file)    },
		{ .name = $("article"), Member(article) }
	},

	.routes = {
		{ .path   = $("/article/$article/$file"),
		  .action = Action(ServeFile) },

		{ .path   = $("/article/$article"),
		  .action = Action(Article) }
	}
};

RouterConstructor;
