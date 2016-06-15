import os

def main():
	createclass('Settings', 'System')

def basepath(fullpath):
	return '/'.join(fullpath.split('/')[:-1]) + '/'

def createclass(classname, directory, baseclass = '', baseclassdir = ''):
	hpp_path = 'inc/szen/' + directory + '/' + classname + '.hpp'
	cpp_path = 'src/' + directory + '/' + classname + '.cpp'
	
	# Check if either file exists
	if os.path.exists(hpp_path) or os.path.exists(cpp_path):
		print('Class \'' + classname + '\' already exists. Aborting...')
		return
	
	try:
		os.makedirs(basepath(hpp_path))
	except FileExistsError:
		pass
	
	try:
		os.makedirs(basepath(cpp_path))
	except FileExistsError:
		pass
	
	keywords = [
		('[classname]', classname),
		('[^classname]', classname.upper()),
		('[directory]', directory),
	]
	
	if len(baseclass) > 0:
		keywords.append(('[inherit_include]', '#include <szen/' + baseclassdir + '/' + baseclass + '.hpp>\n\n'))
		keywords.append(('[inherit_class]', ' : public ' + baseclass))
		keywords.append(('[inherit_virtual]', 'virtual '))
	else:
		keywords.append(('[inherit_include]', ''))
		keywords.append(('[inherit_class]', ''))
		keywords.append(('[inherit_virtual]', ''))
	
	thpp = ("#ifndef SZEN_[^classname]_HPP\n"
	"#define SZEN_[^classname]_HPP\n"
	"\n[inherit_include]"
	"namespace sz\n"
	"{\n"
	"	\n"
	"	class [classname][inherit_class]\n"
	"	{\n"
	"	public:\n"
	"		[classname]();\n"
	"		[inherit_virtual]~[classname]();\n"
	"	};\n"
	"	\n"
	"}\n"
	"\n"
	"#endif // SZEN_[^classname]_HPP\n")

	tcpp = ("#include <szen/[directory]/[classname].hpp>\n"
	"\n"
	"using namespace sz;\n"
	"\n"
	"////////////////////////////////////////////////////\n"
	"[classname]::[classname]()\n"
	"{\n"
	"	\n"
	"}\n"
	"\n"
	"////////////////////////////////////////////////////\n"
	"[classname]::~[classname]()\n"
	"{\n"
	"	\n"
	"}\n")
	
	cpp = tcpp
	hpp = thpp
	
	for t in keywords:
		cpp = cpp.replace(t[0], t[1])
		hpp = hpp.replace(t[0], t[1])
	
	f = open(hpp_path, 'w')
	f.write(hpp)
	f.close()
	
	f = open(cpp_path, 'w')
	f.write(cpp)
	f.close()
	
	print('Class \'' + classname + '\' created.')

if __name__ == "__main__":
	main()
