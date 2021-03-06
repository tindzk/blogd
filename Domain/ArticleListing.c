#import "ArticleListing.h"

#define self ArticleListing

Singleton(self);
SingletonDestructor(self);

rsdef(self, New) {
	return (self) {
		.articles = Articles_New(1024),
		.parser   = Parser_New()
	};
}

def(void, Destroy) {
	Parser_Destroy(&this->parser);

	each(article, this->articles) {
		Article_Destroy(*article);
	}

	Articles_Free(this->articles);
}

def(void, SetLogger, Logger *logger) {
	this->logger = logger;
}

def(Date, ParseDate, RdString s) {
	Date date = Date_Empty();

	RdStringArray *items = String_Split(s, '-');

	if (items->len > 2) {
		date.year  = UInt16_Parse(items->buf[0]);
		date.month = UInt8_Parse(items->buf[1]);
		date.day   = UInt8_Parse(items->buf[2]);
	}

	RdStringArray_Free(items);

	return date;
}

static def(void, Process, RdString dir, RdString file) {
	String path = String_Format($("%/%"), dir, file);

	Logger_Info(this->logger, $("Processing %..."), path.rd);

	try {
		Parser_Parse(&this->parser, path.rd);
	} finally {
		String_Destroy(&path);
	} tryEnd;

	Article *article = Article_New();

	Articles_Push(&this->articles, article);

	Article_SetPath(article,
		String_Clone(Utils_ExtractName(file)));

	Article_SetTitle(article,
		String_Clone(
			Parser_GetMeta(&this->parser, $("title"))));

	Article_SetLanguage(article,
		String_Clone(
			Parser_GetMeta(&this->parser, $("language"))));

	Date date = call(ParseDate,
		Parser_GetMeta(&this->parser, $("date")));

	Article_SetDate(article, date);

	CategoriesInstance cat = Categories_GetInstance();

	RdStringArray *cats =
		Parser_GetMultiMeta(&this->parser, $("category"));

	each(catName, cats) {
		ssize_t catId = Categories_Resolve(cat, *catName);

		if (catId == -1) {
			Logger_Error(this->logger, $("Category '%' does not exist."), *catName);
		} else {
			Categories_Link(cat, catId, article);
		}
	}

	RdStringArray_Free(cats);

	Parser_Node node =
		Parser_GetNodeByName(&this->parser, $("descr"));

	if (node.node != NULL) {
		Article_SetDescr(article,
			Parser_GetBody(&this->parser, node.node, $("")));
	}

	/* optional */
	node = Parser_GetNodeByName(&this->parser, $("contents"));

	if (node.node != NULL) {
		Article_SetContents(article,
			Parser_GetBody(&this->parser, node.node, $("")));
	}

	/* optional */
	Parser_Nodes *nodes = Parser_GetNodesByName(&this->parser,
		Parser_GetRoot(&this->parser), $("section"));

	each(node, nodes) {
		Article_AddSection(article, String_Clone(node->options),
			Parser_GetBody(&this->parser, node->node, $("")));
	}

	Parser_Nodes_Free(nodes);
}

def(size_t, CountArticles) {
	return this->articles->len;
}

def(Article *, GetArticle, RdString name) {
	each(article, this->articles) {
		if (String_Equals(Article_GetPath(*article), name)) {
			return *article;
		}
	}

	return NULL;
}

def(Articles *, GetArticles, size_t offset, size_t limit) {
	Articles *arr = Articles_New(limit);

	for (size_t i = offset;
				i < this->articles->len && i < offset + limit;
				i++)
	{
		Articles_Push(&arr, this->articles->buf[i]);
	}

	return arr;
}

def(void, Populate, RdString path) {
	Directory dir;
	Directory_Init(&dir, path);

	Directory_Entry item;

	while (Directory_Read(&dir, &item)) {
		if (item.type == Directory_ItemType_Regular) {
			if (String_EndsWith(item.name, $(".tyo"))) {
				call(Process, path, item.name);
			}
		}
	}

	Directory_Destroy(&dir);

	Array_Sort(this->articles, Article_ComparePath);
	Array_Sort(this->articles, Article_CompareDate);
}
