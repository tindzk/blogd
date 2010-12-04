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
def(void, SetDate, Date date);
def(Date, GetDate);
def(void, SetDescr, Body descr);
def(Body *, GetDescr);
def(void, SetContents, Body contents);
def(Body *, GetContents);
def(void, AddSection, String title, Body body);
def(Sections *, GetSections);
def(bool, IsEmpty);

#undef self
