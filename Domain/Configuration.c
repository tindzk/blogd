#import "Configuration.h"

#define self Configuration

Singleton(self);
SingletonDestructor(self);

extern Logger logger;

def(self, New) {
	return (self) {
		.title     = String_New(0),
		.descr     = String_New(0),
		.author    = String_New(0),
		.copyright = String_New(0),
		.license   = String_New(0),
		.url       = String_New(0),
		.articles  = String_New(0),
		.about     = String_New(0),
		.filePath  = String_New(0),
		.mail      = String_New(0),
		.flattr    = String_New(0),
		.external  = ExternalArray_New(0)
	};
}

def(void, Destroy) {
	String_Destroy(&this->title);
	String_Destroy(&this->descr);
	String_Destroy(&this->author);
	String_Destroy(&this->copyright);
	String_Destroy(&this->license);
	String_Destroy(&this->url);
	String_Destroy(&this->articles);
	String_Destroy(&this->about);
	String_Destroy(&this->filePath);
	String_Destroy(&this->mail);
	String_Destroy(&this->flattr);

	foreach (item, this->external) {
		String_Destroy(&item->name);
		String_Destroy(&item->url);
	}

	ExternalArray_Free(this->external);
}

#define Get(name, member)         \
	def(ProtString, Get##name) {  \
		return this->member.prot; \
	}

Get(Title, title);
Get(Descr, descr);
Get(Author, author);
Get(Copyright, copyright);
Get(License, license);
Get(Url, url);
Get(ArticlePath, articles);
Get(About, about);
Get(FilePath, filePath);
Get(Mail, mail);
Get(Flattr, flattr);

def(ExternalArray *, GetExternal) {
	return this->external;
}

static def(void, ParseCategories, YAML_Node *node) {
	CategoriesInstance cat = Categories_GetInstance();

	if (node->type == YAML_NodeType_Item) {
		Categories_Insert(cat,
			String_ToCarrier(
				String_Clone(
					YAML_Item_GetValue(node))));
	} else if (node->type == YAML_NodeType_Section) {
		Categories_Insert(cat,
			String_ToCarrier(
				String_Clone(
					YAML_Section_GetName(node))));
	}

	if (node->len > 0) {
		Categories_EnterNode(cat);

		forward (i, node->len) {
			call(ParseCategories, node->buf[i]);
		}

		Categories_LeaveNode(cat);
	}
}

static def(void, AddExternal, YAML_Node *node) {
	if (node->type == YAML_NodeType_Item) {
		External ext = {
			.name = String_Clone(YAML_Item_GetKey(node)),
			.url  = String_Clone(YAML_Item_GetValue(node))
		};

		ExternalArray_Push(&this->external, ext);
	}

	if (node->len > 0) {
		forward (i, node->len) {
			call(AddExternal, node->buf[i]);
		}
	}
}

static def(void, ParseYaml, YAML_Node *node) {
	forward (i, node->len) {
		YAML_Node *child = node->buf[i];

		if (child->type == YAML_NodeType_Item) {
			ProtString name  = YAML_Item_GetKey(child);
			ProtString value = YAML_Item_GetValue(child);

			if (String_Equals(name, $("title"))) {
				String_Copy(&this->title, value);
			} else if (String_Equals(name, $("descr"))) {
				String_Copy(&this->descr, value);
			} else if (String_Equals(name, $("author"))) {
				String_Copy(&this->author, value);
			} else if (String_Equals(name, $("copyright"))) {
				String_Copy(&this->copyright, value);
			} else if (String_Equals(name, $("license"))) {
				String_Copy(&this->license, value);
			} else if (String_Equals(name, $("url"))) {
				String_Copy(&this->url, value);
			} else if (String_Equals(name, $("articles"))) {
				String_Copy(&this->articles, value);
			} else if (String_Equals(name, $("about"))) {
				String_Copy(&this->about, value);
			} else if (String_Equals(name, $("static"))) {
				String_Copy(&this->filePath, value);
			} else if (String_Equals(name, $("mail"))) {
				String_Copy(&this->mail, value);
			} else if (String_Equals(name, $("flattr"))) {
				String_Copy(&this->flattr, value);
			} else {
				Logger_Error(&logger, $("Unknown option '%'"),
					name);
			}
		} else if (child->type == YAML_NodeType_Section) {
			ProtString name = YAML_Section_GetName(child);

			if (String_Equals(name, $("categories"))) {
				forward (j, child->len) {
					call(ParseCategories, child->buf[j]);
				}
			} else if (String_Equals(name, $("external"))) {
				call(AddExternal, child);
			} else {
				Logger_Error(&logger, $("Unknown option '%'"),
					name);
			}
		}
	}
}

def(void, Parse, ProtString path) {
	File file;
	File_Open(&file, path, FileStatus_ReadOnly);

	BufferedStream stream = BufferedStream_New(File_AsStream(&file));
	BufferedStream_SetInputBuffer(&stream, 1024, 128);

	YAML yml = YAML_New(4, &BufferedStreamImpl, &stream);
	YAML_Parse(&yml);

	call(ParseYaml, YAML_GetRoot(&yml));

	YAML_Destroy(&yml);

	BufferedStream_Close(&stream);
	BufferedStream_Destroy(&stream);
}
