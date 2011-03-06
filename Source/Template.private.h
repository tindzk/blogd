#import <Date.h>
#import <Number.h>
#import <HTML/Entities.h>

#import <tplgen/Template.h>
#import <docslib/TemplateHelper.h>

#import <Domain/Article.h>
#import <Domain/Categories.h>
#import <Domain/Configuration.h>

tpl(Listing) {
	ProtString error;
	Articles   *articles;
};

tpl(Feed) {
	Articles *articles;

	ProtString blogName;
	ProtString url;
	ProtString descr;
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
	ProtString name;
};

tpl(Pagination) {
	size_t page;
	size_t pages;
	PagePath path;
};

tpl(Article) {
	ProtString url;
	ProtString error;
	ProtString flattr;
	Article *article;
};

tpl(About) {
	ProtString text;
};

tpl(License) {
	ProtString text;
};

tpl(Categories) {
	Categories_Node *cats;
	CategoriesInstance categories;
};

tpl(Main) {
	Page page;

	ProtString style;

	ProtString title;
	ProtString blogName;

	bool flattr;

	ProtString mail;
	ProtString author;
	ProtString copyright;

	ProtString url;
	ProtString descr;

	ssize_t catId;
	Categories_Node *cats;
	CategoriesInstance categories;

	Template body;
	Template footer;

	ExternalArray *external;
};
