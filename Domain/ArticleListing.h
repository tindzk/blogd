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

def(void, Init);
def(void, Destroy);
def(Date, ParseDate, String s);
def(size_t, CountArticles);
def(ArticleInstance, GetArticle, String name);
def(Articles *, GetArticles, size_t offset, size_t limit);
def(void, Populate, String path);

#undef self
