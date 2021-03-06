#import "Resource.h"

#define self resListing

#define resListing_ArticlesPerPage 5
#define resListing_ArticlesInFeed 15

class {
	bool home;

	String page;
	String name;
	String category;
};

def(void, Init) {
	this->home = false;
}

def(size_t, GetPage) {
	size_t page = 0;

	try {
		page = UInt32_Parse(this->page.rd);
	} catchModule(Integer) {
	} finally {
	} tryEnd;

	return page;
}

/* All articles sorted by date. */
action(Archive) {
	ArticleListingInstance listing = ArticleListing_GetInstance();

	PaginationTemplate pg = {
		.page  = 1,
		.pages = 1,
		.path  = {
			.page = pgArchive,
			.name = $("")
		}
	};

	ListingTemplate tpl;
	tpl.error = $("");

	MainTemplate main = GetMainTemplate(sess);

	main.page = (this->home)
		? pgHome
		: pgArchive;

	main.body   = tplListing(&tpl);
	main.footer = tplPagination(&pg);

	size_t articles = ArticleListing_CountArticles(listing);

	if (articles == 0) {
		tpl.error = $("No articles found.");
	} else {
		pg.pages = Ceil((float) articles / ref(ArticlesPerPage));
		pg.page  = ClampValue(call(GetPage), 1, pg.pages);
	}

	tpl.articles = ArticleListing_GetArticles(listing,
		(pg.page - 1) * ref(ArticlesPerPage),
		ref(ArticlesPerPage));

	TemplateResponse(resp, tplMain(&main));

	Articles_Free(tpl.articles);
}

action(Home) {
	this->home = true;
	dispatch(Archive);
}

action(Category) {
	ssize_t catId;
	RdString catName = String_Trim(this->category.rd);

	PaginationTemplate pg = {
		.page  = 1,
		.pages = 1,
		.path  = {
			.page = pgCategories,
			.name = catName
		}
	};

	ListingTemplate tpl = {
		.error    = $(""),
		.articles = NULL
	};

	RdString path = $("");

	MainTemplate main = GetMainTemplate(sess);

	main.page   = pgCategories;
	main.body   = tplListing(&tpl);
	main.footer = tplPagination(&pg);

	if (catName.len == 0) {
		tpl.error = $("No category selected.");
	} else {
		catId = Categories_Resolve(main.categories, catName);

		if (catId == -1) {
			tpl.error = $("Category does not exist.");
		} else {
			main.catId = catId;

			path = catName;

			size_t articles = Categories_CountArticles(main.categories, catId);

			if (articles == 0) {
				tpl.error = $("No articles found.");
			} else {
				pg.pages = Ceil((float) articles / ref(ArticlesPerPage));
				pg.page  = ClampValue(call(GetPage), 1, pg.pages);

				tpl.articles = Categories_GetArticles(main.categories, catId,
					(pg.page - 1) * ref(ArticlesPerPage),
					ref(ArticlesPerPage));
			}
		}
	}

	ConfigurationInstance config = Configuration_GetInstance();

	String title = String_Format($("% » %"),
		Configuration_GetTitle(config), path);

	main.title = title.rd;

	TemplateResponse(resp, tplMain(&main));

	String_Destroy(&title);

	if (tpl.articles != NULL) {
		Articles_Free(tpl.articles);
	}
}

action(Feed) {
	ArticleListingInstance listing = ArticleListing_GetInstance();

	ConfigurationInstance config = Configuration_GetInstance();

	FeedTemplate feed;

	feed.url      = Configuration_GetUrl(config);
	feed.descr    = Configuration_GetDescr(config);
	feed.blogName = Configuration_GetTitle(config);
	feed.articles = ArticleListing_GetArticles(listing,
		0, ref(ArticlesInFeed));

	TemplateResponse(resp, tplFeed(&feed));

	Articles_Free(feed.articles);
}

ImplEx(Resource) = {
	.size = sizeof(self),

	.init = (void *) ref(Init),

	.members = {
		{ .name = $("page"),     Member(page)     },
		{ .name = $("name"),     Member(name)     },
		{ .name = $("category"), Member(category) },
	},

	.routes = {
		{ .path   = $("/category/:category/:page"),
		  .action = Action(Category) },

		{ .path   = $("/category/:category"),
		  .action = Action(Category) },

		{ .path   = $("/category"),
		  .action = Action(Category) },

		{ .path   = $("/archive/:page"),
		  .action = Action(Archive) },

		{ .path   = $("/archive"),
		  .action = Action(Archive) },

		{ .path   = $("/feed"),
		  .action = Action(Feed) },

		{ .path   = $("/"),
		  .action = Action(Home) }
	}
};

RouterConstructor;
