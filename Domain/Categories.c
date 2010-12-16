#import "Categories.h"

#define self Categories

Singleton(self);
SingletonDestructor(self);

extern Logger logger;

void ref(DestroyNode)(ref(Node) *node);

def(void, Init) {
	this->categories = CategoryArray_New(32);

	Tree_Init(&this->tree, (void *) ref(DestroyNode));

	Tree_Reset(&this->tree);

	this->node = (ref(Node) *) &this->tree.root;
}

void ref(DestroyNode)(__unused ref(Node) *node) {

}

def(void, Destroy) {
	Tree_Destroy(&this->tree);

	foreach (cat, this->categories) {
		String_Destroy(&cat->name);
		Articles_Free(cat->articles);
	}

	CategoryArray_Free(this->categories);
}

def(void, EnterNode) {
	this->node = Tree_AddNode(this->node);
	this->node->isNode = true;
}

def(void, LeaveNode) {
	this->node = this->node->parent;
}

def(ref(Node) *, GetRoot) {
	return (ref(Node) *) &this->tree.root;
}

def(CategoryArray *, GetCategories) {
	return this->categories;
}

def(void, Insert, String value) {
	if (String_Contains(value, $("/"))) {
		Logger_Error(&logger,
			$("Categories must be URI-compatible and thus cannot contain slashes"));

		return;
	}

	ref(Node) *treeNode = Tree_AddNode(this->node);

	Category cat;

	cat.name     = String_Clone(value);
	cat.articles = Articles_New(128);

	CategoryArray_Push(&this->categories, cat);

	treeNode->isNode = false;
	treeNode->offset = this->categories->len - 1;
}

def(String, GetName, ref(Node) *node) {
	if (!node->isNode) {
		Category *cat = &this->categories->buf[node->offset];
		return String_Disown(cat->name);
	}

	return $("");
}

def(size_t, GetNumArticles, ref(Node) *node) {
	if (!node->isNode) {
		Category *cat = &this->categories->buf[node->offset];
		return cat->articles->len;
	}

	return 0;
}

/* Returns category offset. */
def(ssize_t, Resolve, String name) {
	size_t i = 0;

	foreach (category, this->categories) {
		if (String_Equals(category->name, name)) {
			return i;
		}

		i++;
	}

	return -1;
}

def(void, Link, size_t id, ArticleInstance article) {
	if (id >= this->categories->len) {
		return;
	}

	Category *cat = &this->categories->buf[id];

	Articles_Push(&cat->articles, article);
}

def(size_t, CountArticles, size_t cat) {
	Category *ptr = &this->categories->buf[cat];

	return ptr->articles->len;
}

def(Articles *, GetArticles, size_t cat, size_t offset, size_t limit) {
	Category *ptr = &this->categories->buf[cat];

	Articles *arr = Articles_New(limit);

	for (size_t i = offset;
				i < ptr->articles->len && i < offset + limit;
				i++)
	{
		Articles_Push(&arr, ptr->articles->buf[i]);
	}

	Array_Sort(arr, Article_ComparePath);
	Array_Sort(arr, Article_CompareDate);

	return arr;
}
