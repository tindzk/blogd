#import <Date.h>
#import <Number.h>
#import <HTML/Entities.h>

#import <tplgen/Template.h>
#import <docslib/TemplateHelper.h>

#import <Domain/Article.h>
#import <Domain/Categories.h>
#import <Domain/Configuration.h>

tpl(Listing) {
	String error;
	Articles *articles;
};

tpl(Feed) {
	Articles *articles;

	String blogName;
	String url;
	String descr;
};

set(Page) {
	pgHome,
	pgArchive,
	pgCategories,
	pgAbout,
	pgNone
};

record(PagePath) {
	Page page;
	String name;
};

tpl(Pagination) {
	size_t page;
	size_t pages;
	PagePath path;
};

tpl(Article) {
	String url;
	String error;
	String flattr;
	ArticleInstance article;
};

tpl(About) {
	String text;
};

tpl(License) {
	String text;
};

tpl(Categories) {
	Categories_Node *cats;
	CategoriesInstance categories;
};

tpl(Main) {
	Page page;

	String style;

	String title;
	String blogName;

	bool flattr;

	String mail;
	String author;
	String copyright;

	String url;
	String descr;

	ssize_t catId;
	Categories_Node *cats;
	CategoriesInstance categories;

	Template body;
	Template footer;

	ExternalArray *external;
};
