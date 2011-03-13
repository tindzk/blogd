#import "Categories.h"

#define self Categories

Singleton(self);
SingletonDestructor(self);

sdef(void, DestroyNode, __unused ref(Node) *node);

rsdef(self, New) {
	return (self) {
		.tree = Tree_New((void *) ref(DestroyNode)),
		.node = NULL,
		.categories = CategoryArray_New(32)
	};
}

sdef(void, DestroyNode, __unused ref(Node) *node) {

}

def(void, Destroy) {
	Tree_Destroy(&this->tree);

	each(cat, this->categories) {
		CarrierString_Destroy(&cat->name);
		Articles_Free(cat->articles);
	}

	CategoryArray_Free(this->categories);
}

def(void, SetLogger, Logger *logger) {
	this->logger = logger;
}

def(void, EnterNode) {
	this->node = Tree_AddNode(&this->tree, this->node);
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

def(void, Insert, CarrierString value) {
	if (String_Contains(value.rd, $("/"))) {
		Logger_Error(this->logger,
			$("Categories must be URI-compatible and thus cannot contain slashes"));

		CarrierString_Destroy(&value);
		return;
	}

	ref(Node) *treeNode = Tree_AddNode(&this->tree, this->node);

	Category cat = {
		.name     = value,
		.articles = Articles_New(128)
	};

	CategoryArray_Push(&this->categories, cat);

	treeNode->isNode = false;
	treeNode->offset = this->categories->len - 1;
}

def(RdString, GetName, ref(Node) *node) {
	if (!node->isNode) {
		Category *cat = &this->categories->buf[node->offset];
		return cat->name.rd;
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
def(ssize_t, Resolve, RdString name) {
	size_t i = 0;

	each(category, this->categories) {
		if (String_Equals(category->name.rd, name)) {
			return i;
		}

		i++;
	}

	return -1;
}

def(void, Link, size_t id, Article *article) {
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
