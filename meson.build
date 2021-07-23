project('MangaDownloder', 'cpp',
    default_options : ['cpp_std=c++11', 'b_lundef=false', 'b_asneeded=false'])

gtkdep = dependency('gtk+-2.0')
curl = dependency('libcurl')
sqlite3 = dependency('sqlite3')
pthread = dependency('threads')

sources_names = ['mgreader', 'mgpark', 'mgfox', 'webtoons']
sources_libs = []
foreach source : sources_names
  sources_libs += library(source, 'manager/dlder/provider/'+source+'.cpp')
endforeach

dl_facto = library('dl_facto', 'manager/dlder/dl_facto.cpp', link_with : sources_libs)
browser = library('browser', 'manager/browser/browser.cpp')
dlder = library('dlder', 'manager/dlder/dlder.cpp')
config = library('config', 'manager/cfg/config.cpp')
gui = library('gui', 'manager/gui/gui.cpp', dependencies : [gtkdep])

executable('mgdlder', 'testing6.cpp',
  link_with : [browser, gui, dl_facto, dlder, config],
  dependencies : [gtkdep, curl, sqlite3, pthread])

