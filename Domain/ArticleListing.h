#import <Array.h>
#import <String.h>
#import <Integer.h>
#import <Directory.h>

#import "Utils.h"
#import "Article.h"
#import "Categories.h"

#define self ArticleListing

class {
	Categories cat;
	Parser     parser;
	Logger     *logger;
	Articles   *articles;
};

SingletonPrototype(self);

rsdef(self, New);
def(void, Destroy);
def(void, SetLogger, Logger *logger);
def(Date, ParseDate, RdString s);
def(size_t, CountArticles);
def(Article *, GetArticle, RdString name);
def(Articles *, GetArticles, size_t offset, size_t limit);
def(void, Populate, RdString path);

#undef self
