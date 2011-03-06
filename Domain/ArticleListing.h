#import <Array.h>
#import <String.h>
#import <Integer.h>
#import <Directory.h>

#import "Utils.h"
#import "Article.h"
#import "Categories.h"

#define self ArticleListing

class {
	Parser parser;
	Categories cat;
	Articles *articles;
};

SingletonPrototype(self);

rsdef(self, New);
def(void, Destroy);
def(Date, ParseDate, ProtString s);
def(size_t, CountArticles);
def(Article *, GetArticle, ProtString name);
def(Articles *, GetArticles, size_t offset, size_t limit);
def(void, Populate, ProtString path);

#undef self
