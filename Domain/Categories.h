#import <Tree.h>
#import <String.h>

#import "Article.h"
#import "Category.h"

#define self Categories

record(ref(Node)) {
	Tree_Define(ref(Node));

	size_t offset; /* Value's offset in category array. */
	bool isNode;
};

class {
	CategoryArray *categories;

	Tree tree;
	ref(Node) *node;
};

SingletonPrototype(self);

rsdef(self, New);
def(void, Destroy);
def(void, EnterNode);
def(void, LeaveNode);
def(ref(Node) *, GetRoot);
def(CategoryArray *, GetCategories);
def(void, Insert, CarrierString value);
def(ProtString, GetName, ref(Node) *node);
def(size_t, GetNumArticles, ref(Node) *node);
def(ssize_t, Resolve, ProtString category);
def(void, Link, size_t id, ArticleInstance article);
def(size_t, CountArticles, size_t cat);
def(Articles *, GetArticles, size_t cat, size_t offset, size_t limit);

#undef self
