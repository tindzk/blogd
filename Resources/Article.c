#import "Resource.h"

#import <docslib/TextBody.h>
#import <docslib/TextDocument.h>

#define self resArticle

class {
	String file;
	String article;
};

action(AsText) {
	ConfigurationInstance config = Configuration_GetInstance();

	ArticleListingInstance listing = ArticleListing_GetInstance();

	Article *article = ArticleListing_GetArticle(listing,
		Utils_ExtractName(String_Trim(this->article.rd)));

	TextDocument doc;
	TextDocument_Init(&doc, 80);

	TextDocument_Add(&doc, $("Title: "));
	TextDocument_Add(&doc, Article_GetTitle(article));
	TextDocument_AddLine(&doc);

	String date = Date_Format(Article_GetDate(article), true);
	TextDocument_Add(&doc, $("Date: "));
	TextDocument_Add(&doc, date.rd);
	TextDocument_AddLine(&doc);
	String_Destroy(&date);

	if (Article_GetDescr(article)->type != Body_Type_Empty) {
		TextDocument_Add(&doc, $("Description: "));
		TextBody_Process(Article_GetDescr(article), &doc);
		TextDocument_AddLine(&doc);
	}

	if (Article_GetContents(article)->type != Body_Type_Empty) {
		TextDocument_Add(&doc, $("Contents: "));
		TextBody_Process(Article_GetContents(article), &doc);
		TextDocument_AddLine(&doc);
	}

	TextDocument_AddLine(&doc);

	Sections *sects = Article_GetSections(article);

	each(sect, sects) {
		TextDocument_Add(&doc, $("== "));
		TextDocument_Add(&doc, sect->title.rd);
		TextDocument_AddLine(&doc);

		TextBody_Process(&sect->body, &doc);
		TextDocument_AddLine(&doc);
	}

	TextDocument_AddLine(&doc);
	TextDocument_AddLine(&doc);

	TextDocument_Add(&doc, $("--"));
	TextDocument_AddLine(&doc);

	TextDocument_Add(&doc, $("Copyright © "));
	TextDocument_Add(&doc, Configuration_GetCopyright(config));
	TextDocument_Add(&doc, $(" "));
	TextDocument_Add(&doc, Configuration_GetAuthor(config));

	TextDocument_AddLine(&doc);
	TextDocument_AddLine(&doc);

	TextDocument_Add(&doc, Configuration_GetLicense(config));

	Response_SetBufferBody (resp, String_ToCarrier(String_Clone(doc.doc.rd)));
	Response_SetContentType(resp, String_ToCarrier($$("text/plain; charset=utf-8")));

	TextDocument_Destroy(&doc);
}

action(Article) {
	ConfigurationInstance config = Configuration_GetInstance();

	LicenseTemplate license = {
		Configuration_GetLicense(config)
	};

	ArticleListingInstance listing = ArticleListing_GetInstance();

	Article *article = ArticleListing_GetArticle(listing,
		Utils_ExtractName(String_Trim(this->article.rd)));

	MainTemplate main = GetMainTemplate(sess);

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

	main.body   = tplArticle(&tpl);
	main.footer = tplLicense(&license);

	String title = String_New(0);

	if (!Article_IsNull(article)) {
		title = String_Format($("%: %"),
			Configuration_GetTitle(config),
			Article_GetTitle(article));

		main.title = title.rd;
	}

	TemplateResponse(resp, tplMain(&main));

	String_Destroy(&title);
}

action(ServeFile) {
	ConfigurationInstance config = Configuration_GetInstance();

	String path = String_Format($("%/%-%"),
		Configuration_GetArticlePath(config),
		this->article.rd, this->file.rd);

	Date_RFC822 lastModified = Request_GetLastModified(req);
	FileResponse(resp, path.rd, *(DateTime *) &lastModified);

	String_Destroy(&path);
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.members = {
		{ .name = $("file"),    Member(file)    },
		{ .name = $("article"), Member(article) }
	},

	.routes = {
		{ .path   = $("/article/:article/:file"),
		  .action = Action(ServeFile) },

		{ .path   = $("/article/{article}.txt"),
		  .action = Action(AsText) },

		{ .path   = $("/article/:article"),
		  .action = Action(Article) }
	}
};

RouterConstructor;
