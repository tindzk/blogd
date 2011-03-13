#import <Date.h>
#import <Number.h>
#import <HTML/Entities.h>

#import <tplgen/Template.h>
#import <docslib/TemplateHelper.h>

#import <Domain/Article.h>
#import <Domain/Categories.h>
#import <Domain/Configuration.h>

tpl(Listing) {
	RdString error;
	Articles   *articles;
};

tpl(Feed) {
	Articles *articles;

	RdString blogName;
	RdString url;
	RdString descr;
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
	RdString name;
};

tpl(Pagination) {
	size_t page;
	size_t pages;
	PagePath path;
};

tpl(Article) {
	RdString url;
	RdString error;
	RdString flattr;
	Article *article;
};

tpl(About) {
	RdString text;
};

tpl(License) {
	RdString text;
};

tpl(Categories) {
	Categories_Node *cats;
	CategoriesInstance categories;
};

tpl(Main) {
	Page page;

	RdString style;

	RdString title;
	RdString blogName;

	bool flattr;

	RdString mail;
	RdString author;
	RdString copyright;

	RdString url;
	RdString descr;

	ssize_t catId;
	Categories_Node *cats;
	CategoriesInstance categories;

	Template body;
	Template footer;

	ExternalArray *external;
};
