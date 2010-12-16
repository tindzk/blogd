#import <Date.h>
#import <String.h>

#import <docslib/Parser.h>

#define self Article

record(Section) {
	String title;
	Body body;
};

Array(Section, Sections);

class {
	Date date;
	Body descr;
	String lng;
	String tags;
	String path;
	String title;
	Body contents;
	Sections *sections;
};

ExtendClass;

Array(Instance(self), Articles);

def(void, Init);
def(void, Destroy);
def(void, SetPath, String path);
def(String, GetPath);
def(void, SetTitle, String title);
def(String, GetTitle);
def(void, SetLanguage, String lng);
def(String, GetLanguage);
def(void, SetDate, Date date);
def(Date, GetDate);
def(void, SetDescr, Body descr);
def(Body *, GetDescr);
def(void, SetContents, Body contents);
def(Body *, GetContents);
def(String, GetTags);
def(void, AddSection, String title, Body body);
def(Sections *, GetSections);
def(bool, IsEmpty);
sdef(short, ComparePath, ArticleInstance *a, ArticleInstance *b);
sdef(short, CompareDate, ArticleInstance *a, ArticleInstance *b);

#undef self
