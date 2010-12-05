#import "ArticleListing.h"

#define self ArticleListing

extern Logger logger;

Singleton(self);
SingletonDestructor(self);

def(void, Init) {
	this->articles = Articles_New(1024);

	Parser_Init(&this->parser);
}

def(void, Destroy) {
	Parser_Destroy(&this->parser);

	foreach (article, this->articles) {
		Article_Destroy(*article);
		Article_Free(*article);
	}

	Articles_Free(this->articles);
}

def(Date, ParseDate, String s) {
	Date date = Date_Empty();

	StringArray *items = String_Split(s, '-');

	if (items->len < 3) {
		return date;
	}

	date.year  = UInt16_Parse(items->buf[0]);
	date.month = UInt8_Parse(items->buf[1]);
	date.day   = UInt8_Parse(items->buf[2]);

	StringArray_Free(items);

	return date;
}

static def(void, Process, String dir, String file) {
	String path = String_Format($("%/%"), dir, file);

	Logger_Info(&logger, $("Processing %..."), path);

	try {
		Parser_Parse(&this->parser, path);
	} clean finally {
		String_Destroy(&path);
	} tryEnd;

	ArticleInstance article = Article_New();

	Articles_Push(&this->articles, article);

	Article_Init(article);

	Article_SetPath(article, Utils_ExtractName(file));

	Article_SetTitle(article,
		Parser_GetMeta(&this->parser, $("title")));

	Date date = call(ParseDate,
		Parser_GetMeta(&this->parser, $("date")));

	Article_SetDate(article, date);

	CategoriesInstance cat = Categories_GetInstance();

	StringArray *cats = Parser_GetMultiMeta(&this->parser, $("category"));

	foreach (catName, cats) {
		ssize_t catId = Categories_Resolve(cat, *catName);

		if (catId == -1) {
			Logger_Error(&logger, $("Category '%' does not exist."),
				*catName);
		} else {
			Categories_Link(cat, catId, article);
		}
	}

	StringArray_Free(cats);

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

	foreach (node, nodes) {
		Article_AddSection(article, node->options,
			Parser_GetBody(&this->parser, node->node, $("")));
	}

	Parser_Nodes_Free(nodes);
}

def(size_t, CountArticles) {
	return this->articles->len;
}

def(ArticleInstance, GetArticle, String name) {
	foreach (article, this->articles) {
		if (String_Equals(Article_GetPath(*article), name)) {
			return *article;
		}
	}

	return Article_Null();
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

def(void, Populate, String path) {
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
